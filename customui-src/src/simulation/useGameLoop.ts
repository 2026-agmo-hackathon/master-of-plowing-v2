import * as React from 'react'
import { useCallback, useEffect, useRef, useState } from 'react'
import { GameLoop } from './GameLoop'
import { SignalControllerClient } from './SignalControllerClient'
import type { BackendCommand, ImplementKind } from './SignalControllerClient'
import { registerApiBridge } from './ApiBridge'
import { loadMap } from './MapLoader'
import type { LoadedMap } from './MapLoader'
import { SoilGrid } from './SoilGrid'
import { getOriginLat, getOriginLon } from './GpsOrigin'
import type { Scenario } from './ScenarioTypes'
import type { VehicleState } from './VehicleSystem'
import type { TerrainPatch } from './SoilSystem'
import { getSessionClient } from '../protocol/SessionClient'
import type { TillageCellPatch } from '../protocol/SessionProtocol'
import { useSessionStore } from '../state/sessionStore'

export interface UseGameLoopOptions {
  scenario?: Scenario | null
  initialMapId: string
  onStateUpdate?: (s: VehicleState) => void
  onGameLoopReady?: (loop: GameLoop) => void
  onTerrainDirty?: (patches: TerrainPatch[]) => void
  /** High-level commands the caller must handle (engine, gear, implement, reset). */
  onBackendCommand?: (cmd: BackendCommand) => void
  /** Provider used to enrich outgoing telemetry with current implement state. */
  getImplement?: () => ImplementKind
}

export interface UseGameLoopResult {
  gameLoop: React.RefObject<GameLoop | null>
  vehicleStateRef: React.RefObject<VehicleState>
  soilGridRef: React.RefObject<SoilGrid | null>
  soilGrid: SoilGrid | null
  running: boolean
  setRunning: (b: boolean) => void
  lightState: VehicleState
  swapMap: (newMapId: string) => Promise<void>
  resetMap: () => Promise<void>
  currentMapId: string
  mapRevision: number
  mapSize: { width: number; height: number; centerX: number; centerZ: number }
}

function initState(): VehicleState {
  return {
    latitude: 37.5665, longitude: 126.978, altitude: 0,
    heading: 0, speed: 0, numSat: 12,
    rpm: 900, throttle: 0, brake: 0, steerAngle: 0,
    measuredSteerAngleDeg: 0, vehicleFrontWheelAngleDeg: 0, motorCmdRpm: 0,
    accel_x: 0, accel_y: 0, gyro_z: 0,
    roll: 0, pitch: 0, yaw: 0, gear: 1, appSteerCommand: 0, steerFeedbackActive: true,
    running: false,
  }
}

export function useGameLoop(opts: UseGameLoopOptions): UseGameLoopResult {
  const { scenario, initialMapId } = opts

  const [mapId, setMapId] = useState<string>(initialMapId)
  const [mapReloadToken, setMapReloadToken] = useState(0)

  const gameLoopRef = useRef<GameLoop | null>(null)
  const vehicleStateRef = useRef<VehicleState>(initState())
  const soilGridRef = useRef<SoilGrid | null>(null)

  interface TerrainState {
    soilGrid: SoilGrid
    size: { width: number; height: number; centerX: number; centerZ: number }
    revision: number
  }
  const [terrain, setTerrain] = useState<TerrainState | null>(null)
  const scClientRef = useRef<SignalControllerClient | null>(null)

  const onStateUpdateRef = useRef(opts.onStateUpdate)
  const onTerrainDirtyRef = useRef(opts.onTerrainDirty)
  const onGameLoopReadyRef = useRef(opts.onGameLoopReady)
  const onBackendCommandRef = useRef(opts.onBackendCommand)
  const getImplementRef = useRef(opts.getImplement)
  onStateUpdateRef.current = opts.onStateUpdate
  onTerrainDirtyRef.current = opts.onTerrainDirty
  onGameLoopReadyRef.current = opts.onGameLoopReady
  onBackendCommandRef.current = opts.onBackendCommand
  getImplementRef.current = opts.getImplement

  const [running, setRunning] = useState(false)
  const runningRef = useRef(running)
  runningRef.current = running

  const [lightState, setLightState] = useState<VehicleState>(vehicleStateRef.current)

  // Construct GameLoop + SignalControllerClient when map/scenario changes.
  useEffect(() => {
    let cancelled = false
    let loop: GameLoop | null = null
    let scClient: SignalControllerClient | null = null
    // TODO-6A: tillage flush interval — declared in effect scope so cleanup can clear it
    let flushIntervalId: number | null = null

    void (async () => {
      console.log('[useGameLoop] effect start', { mapId, mapReloadToken })
      let loaded: LoadedMap
      try {
        loaded = await loadMap(mapId)
        console.log('[useGameLoop] loadMap resolved', { mapId, hasSpawn: !!loaded.spawn, size: loaded.def.size })
      } catch (err) {
        console.warn('[useGameLoop] loadMap(' + mapId + ') threw, trying fallback "1"', err)
        try {
          loaded = await loadMap('1')
          console.log('[useGameLoop] loadMap fallback "1" resolved', { hasSpawn: !!loaded.spawn })
        } catch (err2) {
          console.error('[useGameLoop] both loadMap calls failed', err2)
          return
        }
      }
      if (cancelled) {
        console.log('[useGameLoop] cancelled after loadMap')
        return
      }

      try {
        const { width, height, centerX = 0, centerZ = 0 } = loaded.def.size

        const fields = loaded.def.fields
        const soilType = loaded.def.soilType
        loop = new GameLoop(loaded.heightFn, {
          ...(fields ? { fields } : {}),
          ...(soilType ? { soilType } : {}),
          size: loaded.def.size,
        })
        console.log('[useGameLoop] new GameLoop OK')
        gameLoopRef.current = loop
        const grid = loop.getSoilGrid()
        soilGridRef.current = grid

        setTerrain(prev => ({
          soilGrid: grid,
          size: { width, height, centerX, centerZ },
          revision: (prev?.revision ?? 0) + 1,
        }))
        console.log('[useGameLoop] setTerrain fired', { revision: '+1' })
        useSessionStore.getState().bumpOriginVersion()

        // Spawn vehicle at map's startPosition if available, otherwise fall back to default.
        if (loaded.spawn) {
          loop.injectState(loaded.spawn.lat, loaded.spawn.lon, loaded.spawn.headingDeg)
          console.log('[useGameLoop] injectState from spawn', loaded.spawn)
        } else {
          // Fallback: spawn at the current GpsOrigin (scene 0,0) so the vehicle lands
          // wherever the map was anchored — never at the hardcoded Seoul default.
          const lat = getOriginLat(), lon = getOriginLon()
          loop.injectState(lat, lon, 0)
          console.log('[useGameLoop] injectState from origin (no spawn)', { lat, lon })
        }

        // Reflect the injected pose in lightState so the RightConsole shows the
        // real spawn lat/lon before the engine starts ticking.
        const injectedLat = loaded.spawn ? loaded.spawn.lat : getOriginLat()
        const injectedLon = loaded.spawn ? loaded.spawn.lon : getOriginLon()
        const injectedHeading = loaded.spawn ? loaded.spawn.headingDeg : 0
        setLightState(prev => ({
          ...prev,
          latitude: injectedLat,
          longitude: injectedLon,
          heading: injectedHeading,
        }))
        vehicleStateRef.current = {
          ...vehicleStateRef.current,
          latitude: injectedLat,
          longitude: injectedLon,
          heading: injectedHeading,
        }
      } catch (err) {
        console.error('[useGameLoop] EXCEPTION in terrain setup', err)
        return
      }

      scClient = new SignalControllerClient({ sessionClient: getSessionClient() })
      scClient.setImplementProvider(() => getImplementRef.current?.() ?? null)
      scClient.setSpeedMultiplierProvider(() => useSessionStore.getState().speedMultiplier || 1)
      scClientRef.current = scClient
      scClient.start()

      const scClientLocal = scClient
      const loopLocal = loop

      // transform push throttle: wall ~16ms / speedMult, minimum 4ms to prevent flooding
      let lastTransformPushMs = 0
      let lastProgress = 0
      let lastElapsedS = 0
      // TODO-6A: dirty cell buffer for 100ms tillage flush
      let dirtyBuffer: TillageCellPatch[] = []

      // Start 100ms interval to flush dirty tillage cells to SessionClient
      flushIntervalId = window.setInterval(() => {
        if (dirtyBuffer.length === 0) return
        const cells = dirtyBuffer
        dirtyBuffer = []
        getSessionClient().sendUpdateTillage(cells)
      }, 100)

      loop.onStateUpdate = (s: VehicleState) => {
        const stateWithRunning = { ...s, running: true }
        vehicleStateRef.current = stateWithRunning
        onStateUpdateRef.current?.(stateWithRunning)
        scClientLocal.sendVehicleState(stateWithRunning)
      }

      // Per-sub-step transform push: throttle = max(4ms, 16ms / speedMult)
      // At 1x: ~16ms interval (same as before); at 8x: 2ms attempted → 4ms floor applied.
      loop.onPostSubStep = (x: number, z: number, headingRad: number, speed: number) => {
        const now = performance.now()
        const speedMult = useSessionStore.getState().speedMultiplier || 1
        const throttleMs = Math.max(4, 16 / speedMult)
        if (now - lastTransformPushMs >= throttleMs) {
          lastTransformPushMs = now
          const s = vehicleStateRef.current
          getSessionClient().sendUpdateTransform({
            x,
            z,
            heading: headingRad,
            speed,
            altitude: s.altitude ?? 0,
            engineOn: loopLocal.isRunning(),
            plowDown: loopLocal.isImplementActive(),
            throttle: s.throttle ?? 0,
            progress: lastProgress,
            elapsed_s: lastElapsedS,
            cross_track_error: 0,
            heading_drift: 0,
            area_done_m2: 0,
            area_total_m2: 0,
            tracking_cr_unit: 0,
            overhead_scale_km: 0,
            outsideField: loopLocal.outsideField,
          })
        }
      }

      loop.onTerrainDirty = (patches) => {
        onTerrainDirtyRef.current?.(patches)
      }

      // TODO-6A: accumulate tilled cells into buffer
      loop.onTillageCells = (cells, gridCols) => {
        for (const c of cells) {
          dirtyBuffer.push({ idx: c.row * gridCols + c.col, depth: c.depth ?? 0.25 })
        }
      }

      loop.onProgressUpdate = (p) => {
        lastProgress = p.progress
        lastElapsedS = p.elapsed_s
        scClientLocal.sendProgress(p)
      }

      if (loaded.def.fields && loaded.def.fields.length > 0) {
        loop.setFields(loaded.def.fields)
        loop.setFieldPolygon(loaded.def.fields[0].polygon)
      }

      // Backend → sim command dispatch.
      // Low-level commands (steer/throttle/brake/inject_state) hit GameLoop directly.
      // Higher-level state mutations (engine/gear/implement/reset) bubble up via
      // onBackendCommand because they live in the React caller.
      scClient.onCommand((cmd) => {
        switch (cmd.kind) {
          case 'steer_motor':
            loopLocal.applyAppCommand('steer', cmd.speedRpm / 300)
            onBackendCommandRef.current?.(cmd)
            return
          case 'throttle':
            loopLocal.applyAppCommand('throttle', cmd.value)
            onBackendCommandRef.current?.(cmd)
            return
          case 'brake':
            loopLocal.applyAppCommand('brake', cmd.value)
            onBackendCommandRef.current?.(cmd)
            return
          case 'inject_state':
            loopLocal.injectState(cmd.latitude, cmd.longitude, cmd.heading ?? 0)
            return
          case 'track_complete':
            onBackendCommandRef.current?.({ kind: 'track_complete' })
            return
          case 'pp_telemetry':
            onBackendCommandRef.current?.(cmd)
            return
          default:
            onBackendCommandRef.current?.(cmd)
        }
      })

      // New map starts with engine OFF — caller can turn on via setRunning.
      setRunning(false)

      onGameLoopReadyRef.current?.(loop)
    })()

    return () => {
      cancelled = true
      scClient?.stop()
      if (flushIntervalId !== null) {
        window.clearInterval(flushIntervalId)
        flushIntervalId = null
      }
      if (loop) {
        loop.stop()
        loop.onStateUpdate = null
        loop.onTerrainDirty = null
        loop.onTillageCells = null
        loop.onProgressUpdate = undefined
        loop.onPostSubStep = null
      }
      gameLoopRef.current = null
      scClientRef.current = null
      soilGridRef.current = null
      setTerrain(null)
    }
  }, [mapId, mapReloadToken, scenario])

  // Start/stop physics based on `running`.
  useEffect(() => {
    const loop = gameLoopRef.current
    if (!loop) return
    if (running) loop.start()
    else loop.stop()
  }, [running])

  // EMA smoothed speed to reduce flickering in the HUD speed display.
  const smoothedSpeedRef = useRef(0)

  // 50ms throttled HUD state — avoid per-frame setState.
  useEffect(() => {
    const ALPHA = 0.25 // smoothing factor: smaller = smoother but more lag
    const STOP_CLAMP_MPS = 0.02 // clamp to 0 when smoothed speed is below this
    const id = setInterval(() => {
      const curr = vehicleStateRef.current
      if (curr) {
        smoothedSpeedRef.current = ALPHA * curr.speed + (1 - ALPHA) * smoothedSpeedRef.current
        if (smoothedSpeedRef.current < STOP_CLAMP_MPS) smoothedSpeedRef.current = 0
        setLightState({ ...curr, speed: smoothedSpeedRef.current })
      }
    }, 50)
    return () => { clearInterval(id) }
  }, [])

  // REST API bridge — re-register when `running` changes so getState/setEngine see fresh value.
  useEffect(() => {
    const unregister = registerApiBridge({
      getState: () => ({ running: runningRef.current, state: vehicleStateRef.current }),
      setEngine: (on: boolean) => {
        setRunning(on)
      },
      setControl: (throttle: number | undefined, steerAngle: number | undefined) => {
        if (steerAngle !== undefined) {
          const normalized = steerAngle / 45
          gameLoopRef.current?.setManualSteer(normalized)
        }
        if (throttle !== undefined) {
          gameLoopRef.current?.applyAppCommand('throttle', throttle)
        }
      },
    })
    return unregister
  }, [running])

  const swapMap = useCallback(async (newMapId: string) => {
    setMapId(newMapId)
  }, [])

  const resetMap = useCallback(async () => {
    setMapReloadToken(v => v + 1)
  }, [])

  return {
    gameLoop: gameLoopRef,
    vehicleStateRef,
    soilGridRef,
    soilGrid: terrain?.soilGrid ?? null,
    running,
    setRunning,
    lightState,
    swapMap,
    resetMap,
    currentMapId: mapId,
    mapRevision: terrain?.revision ?? 0,
    mapSize: terrain?.size ?? { width: 200, height: 400, centerX: 0, centerZ: 0 },
  }
}
