import { lazy, Suspense, useCallback, useEffect, useMemo, useRef, useState } from 'react'
import FinalReport from './FinalReport'
import type { FinalReportMetrics } from './FinalReport'
import { Canvas, useFrame, useThree } from '@react-three/fiber'
import { PCFShadowMap } from 'three'
import type { JSX } from 'react'
import { useGameLoop } from '../simulation/useGameLoop'
import type { BackendCommand } from '../simulation/SignalControllerClient'
import type { GameInput } from '../simulation/GameLoop'
import type { TerrainPatch } from '../simulation/SoilSystem'
import { loadQualityPreset, saveQualityPreset, QUALITY_PRESETS } from '../3d/QualityPreset'
import { reprojectPolygons } from '../simulation/MapLoader'
import { RddfPathRenderer } from '../3d/RddfPathRenderer'
import type { QualityPresetName } from '../3d/QualityPreset'
import { useSessionStore } from '../state/sessionStore'
import { getSessionClient } from '../protocol/SessionClient'
import { TRACTOR_PRESETS, IMPLEMENT_PRESETS } from '../protocol/SessionProtocol'
import type { TractorPresetKey, ImplementPresetKey } from '../protocol/SessionProtocol'
import { applyTractorPreset } from '../simulation/TractorConfig'
import { gpsToEnu } from '../simulation/GpsOrigin'
import type { TractorPresetKey as TractorPhysicsPresetKey } from '../simulation/TractorConfig'
import FieldTerrain from './r3f/FieldTerrain'
import FarGround from './r3f/FarGround'
import { FieldBoundaryRidge } from './r3f/FieldBoundaryRidge'
import SkyAndLights from './r3f/SkyAndLights'
import Tractor from './r3f/Tractor'
import type { TractorGeometryData } from './r3f/Tractor'
import CameraRig from './r3f/CameraRig'
import SensorOverlay from './SensorOverlay'
import TopBar from './console/TopBar'
import RightConsole from './console/RightConsole'
import SpeedWarningDialog, { isSpeedWarningDismissed } from './SpeedWarningDialog'

// Dev-only Stats panel (drei). Lazy import + DEV guard so production builds
// tree-shake the chunk entirely.
const DevStats = import.meta.env.DEV
  ? lazy(() => import('@react-three/drei').then((m) => ({ default: m.Stats })))
  : null

function RddfPathDriver(): null {
  const { scene } = useThree()
  const rendererRef = useRef<RddfPathRenderer | null>(null)
  const rddfWaypointsLL = useSessionStore(s => s.rddfWaypointsLL)
  const originVersion = useSessionStore(s => s.originVersion)

  useEffect(() => {
    const r = new RddfPathRenderer(scene)
    rendererRef.current = r
    console.log('[RddfPathDriver] RddfPathRenderer created', { hasRenderer: !!rendererRef.current })
    return () => {
      r.dispose()
      rendererRef.current = null
    }
  }, [scene])

  useEffect(() => {
    if (!rendererRef.current) return
    const enuWaypoints = rddfWaypointsLL.map(w => {
      const { x, z } = gpsToEnu(w.lat, w.lon)
      return { x, z, speed: w.speed, implementFlag: w.implementFlag }
    })
    console.log('[RddfPathDriver] rddfWaypoints changed', { count: enuWaypoints.length, hasRenderer: true })
    rendererRef.current.setWaypoints(enuWaypoints)
  }, [rddfWaypointsLL, originVersion])

  useFrame((_, delta) => {
    rendererRef.current?.update(delta)
  })

  return null
}

interface DrawCallStatsProps {
  onSample: (fps: number, drawCalls: number) => void
}

// Dev-only per-frame sampler that reads gl.info.render.calls and reports FPS.
// Throttles state updates to once per ~60 frames to avoid React churn.
function DrawCallStats({ onSample }: DrawCallStatsProps): null {
  const { gl } = useThree()
  const frameCountRef = useRef(0)
  const lastTimeRef = useRef(performance.now())
  useFrame(() => {
    frameCountRef.current += 1
    if (frameCountRef.current >= 60) {
      const now = performance.now()
      const elapsed = now - lastTimeRef.current
      const fps = elapsed > 0 ? Math.round((frameCountRef.current * 1000) / elapsed) : 0
      const drawCalls = gl.info.render.calls
      onSample(fps, drawCalls)
      frameCountRef.current = 0
      lastTimeRef.current = now
    }
  })
  return null
}

interface QualityApplierProps {
  cameraFar: number
}

function QualityApplier({ cameraFar }: QualityApplierProps): null {
  const { camera } = useThree()
  useEffect(() => {
    camera.far = cameraFar
    camera.updateProjectionMatrix()
  }, [camera, cameraFar])
  return null
}

interface InputDriverProps {
  heldKeysRef: React.RefObject<Set<string>>
  throttleRef: React.RefObject<number>
  brakeRef: React.RefObject<number>
  keysRef: React.RefObject<Set<string>>
  manualSteerRef: React.RefObject<number>
  inputRef: React.RefObject<GameInput>
  gameLoopRef: ReturnType<typeof useGameLoop>['gameLoop']
  isAutonomous?: boolean
}

function InputDriver(props: InputDriverProps): null {
  const { heldKeysRef, throttleRef, brakeRef, keysRef, manualSteerRef, inputRef, gameLoopRef, isAutonomous } = props
  useFrame((_state, delta) => {
    const loop = gameLoopRef.current
    if (!loop) return

    // Skip throttle/brake/steer key processing in autonomous mode.
    if (isAutonomous) return

    const keys = keysRef.current
    const held = heldKeysRef.current
    const dt = Math.min(delta, 0.05)

    const brakeHeld = held.has('arrowdown') || held.has(' ')
    const throttleHeld = held.has('arrowup') && !brakeHeld

    if (throttleHeld) {
      throttleRef.current = Math.min(1, throttleRef.current + dt / 1.5)
    } else {
      throttleRef.current = Math.max(0, throttleRef.current - dt / 0.4)
    }

    if (brakeHeld) {
      brakeRef.current = Math.min(1, brakeRef.current + dt / 0.8)
    } else {
      brakeRef.current = Math.max(0, brakeRef.current - dt / 0.25)
    }

    const inp = inputRef.current
    inp.throttle = throttleRef.current
    inp.brake = brakeRef.current

    const steerDir =
      (keys.has('arrowleft') ? -1 : 0) +
      (keys.has('arrowright') ? 1 : 0)
    if (steerDir !== 0) {
      manualSteerRef.current = Math.max(-1, Math.min(1, manualSteerRef.current + steerDir * 2.0 * delta))
      loop.setManualSteer(manualSteerRef.current)
    } else if (Math.abs(manualSteerRef.current) > 0.01) {
      const decay = 3.0 * delta
      if (manualSteerRef.current > 0) {
        manualSteerRef.current = Math.max(0, manualSteerRef.current - decay)
      } else {
        manualSteerRef.current = Math.min(0, manualSteerRef.current + decay)
      }
      loop.setManualSteer(manualSteerRef.current)
    } else {
      manualSteerRef.current = 0
      loop.clearManualSteer()
    }

    loop.setInput({ throttle: inp.throttle, brake: inp.brake, steer: loop.getCurrentSteer() })
  })
  return null
}

export default function FieldSimulatorView(): JSX.Element {
  const isWebGL2 = useMemo(() => {
    try {
      return !!document.createElement('canvas').getContext('webgl2')
    } catch {
      return false
    }
  }, [])

  const tractorPreset = useSessionStore(s => s.tractorPreset)
  const implementPreset = useSessionStore(s => s.implementPreset)
  const selectedMapId = useSessionStore(s => s.selectedMapId)
  const mapList = useSessionStore(s => s.mapList)
  const rddfWaypointsLL = useSessionStore(s => s.rddfWaypointsLL)
  const originVersion = useSessionStore(s => s.originVersion)
  const rddfWaypoints = useMemo(() => rddfWaypointsLL.map(w => {
    const { x, z } = gpsToEnu(w.lat, w.lon)
    return { x, z, speed: w.speed, implementFlag: w.implementFlag }
  }), [rddfWaypointsLL, originVersion])
  const rddfOrigin = useSessionStore(s => s.rddfOrigin)
  const rawPolygonsLonLat = useSessionStore(s => s.rawPolygonsLonLat)
  const isAutonomous = useSessionStore(s => s.isAutonomous)
  const setIsAutonomous = useSessionStore(s => s.setIsAutonomous)
  const coverageGoal = useSessionStore(s => s.coverageGoal)
  const setStarted = useSessionStore(s => s.setStarted)
  const started = useSessionStore(s => s.started)
  const wsConnected = useSessionStore(s => s.wsConnected)
  const speedMultiplier = useSessionStore(s => s.speedMultiplier)
  const tractorScale = TRACTOR_PRESETS[tractorPreset as TractorPresetKey]?.scale ?? 1.0

  // Track session start timestamp so onRunSummary can reject stale echoes.
  const sessionStartedAtRef = useRef<number | null>(null)

  // Guard: track which selectedMapId's teleport has already been applied.
  // Prevents re-teleporting to stale waypoints when rddfWaypoints change
  // for a prior map (e.g. due to race between reset and backend broadcast).
  const appliedMapIdRef = useRef<string | null>(null)

  const implementSpec = useMemo(() => {
    const key = implementPreset as keyof typeof IMPLEMENT_PRESETS | ''
    if (!key) return null
    const p = IMPLEMENT_PRESETS[key as keyof typeof IMPLEMENT_PRESETS]
    if (!p) return null
    return { type: p.type as 'plow' | 'rotary', width: p.width, depth: p.depth }
  }, [implementPreset])

  const dirtyPatchesRef = useRef<TerrainPatch[]>([])

  const onTerrainDirty = useCallback((patches: TerrainPatch[]) => {
    dirtyPatchesRef.current.push(...patches)
  }, [])

  const onStateUpdate = useCallback((): void => {
    // Vehicle state is simulated but not displayed in any overlay
  }, [])

  const [implementType, setImplementType] = useState<'plow' | 'rotary' | null>(null)
  const [speedWarningOpen, setSpeedWarningOpen] = useState(false)
  // Track whether the warning has been shown this session (avoid re-showing on 1x→2x→1x→2x)
  const speedWarningShownRef = useRef(false)
  const prevCommandIdxRef = useRef<number>(-1)
  const implementTypeRef = useRef<'plow' | 'rotary' | null>(null)
  implementTypeRef.current = implementType
  const backendCommandHandlerRef = useRef<((cmd: BackendCommand) => void) | null>(null)

  const effectiveMapId = selectedMapId != null
    ? String(selectedMapId)
    : (mapList[0]?.id != null ? String(mapList[0].id) : '1')

  const {
    gameLoop: gameLoopRef,
    vehicleStateRef,
    soilGrid,
    running,
    setRunning,
    lightState,
    swapMap,
    resetMap,
    currentMapId,
    mapRevision,
    mapSize,
  } = useGameLoop({
    initialMapId: effectiveMapId,
    onTerrainDirty,
    onStateUpdate,
    onBackendCommand: (cmd) => backendCommandHandlerRef.current?.(cmd),
    getImplement: () => implementTypeRef.current,
  })
  const [cameraMode, setCameraMode] = useState<'follow' | 'free'>('follow')
  const [sensorOverlayVisible, setSensorOverlayVisible] = useState(false)
  const [progress, setProgress] = useState<number | undefined>()
  const [elapsedS, setElapsedS] = useState<number | undefined>()
  const [showFinalReport, setShowFinalReport] = useState(false)
  const [finalReportMetrics, setFinalReportMetrics] = useState<FinalReportMetrics | null>(null)
  const [crossTrackError, setCrossTrackError] = useState<number>(0)
  const [headingDrift, setHeadingDrift] = useState<number>(0)
  const [areaDoneM2, setAreaDoneM2] = useState<number>(0)
  const [areaTotalM2, setAreaTotalM2] = useState<number>(0)
  const [trackingCrUnit, setTrackingCrUnit] = useState<number>(0)
  const [overheadScaleKm, setOverheadScaleKm] = useState<number>(0)
  const [qualityPreset, setQualityPreset] = useState<QualityPresetName>(() => loadQualityPreset())
  const spec = QUALITY_PRESETS[qualityPreset]
  const [manualGear, setManualGear] = useState<number | null>(1)
  const [contextLost, setContextLost] = useState(false)
  const [perfStats, setPerfStats] = useState<{ fps: number; drawCalls: number }>({ fps: 0, drawCalls: 0 })
  const handlePerfSample = useCallback((fps: number, drawCalls: number) => {
    setPerfStats({ fps, drawCalls })
  }, [])
  const canvasDomRef = useRef<HTMLCanvasElement | null>(null)
  const lostHandlerRef = useRef<((e: Event) => void) | null>(null)
  const restoredHandlerRef = useRef<((e: Event) => void) | null>(null)

  const keysRef = useRef(new Set<string>())
  const heldKeysRef = useRef(new Set<string>())
  const throttleRef = useRef(0)
  const brakeRef = useRef(0)
  const inputRef = useRef<GameInput>({ throttle: 0, brake: 0, steer: 0 })
  const manualSteerRef = useRef(0)
  const tractorGeometryRef = useRef<TractorGeometryData | null>(null)

  // Apply tractor physical parameters whenever preset changes
  useEffect(() => {
    applyTractorPreset(tractorPreset as TractorPhysicsPresetKey)
  }, [tractorPreset])

  // Wire onProgressUpdate to update progress/elapsedS state for console gauge
  useEffect(() => {
    const loop = gameLoopRef.current
    if (!loop) return
    const prev = loop.onProgressUpdate
    loop.onProgressUpdate = (p) => {
      setProgress(p.progress)
      setElapsedS(p.elapsed_s)
      prev?.(p)
    }
    return () => {
      loop.onProgressUpdate = prev
    }
  }, [gameLoopRef, mapRevision])

  // Ref to hold latest local final payload for merging with backend run_summary.
  const localFinalPayloadRef = useRef<import('../simulation/GameLoop').ProgressPayload | null>(null)

  // Wire onProgressUpdate to show Final Report on final=true.
  // Score submission is delegated to the "Submit to leaderboard" button in FinalReport.
  // This path is a FALLBACK if run_summary never arrives (e.g. dev env without backend).
  useEffect(() => {
    const loop = gameLoopRef.current
    if (!loop) return
    const prev = loop.onProgressUpdate
    loop.onProgressUpdate = (p) => {
      prev?.(p)
      if (!p.final) return
      // Store local payload; run_summary handler will overlay backend values on top.
      localFinalPayloadRef.current = p
      setFinalReportMetrics({
        finalCoverage: p.progress,
        elapsedS: p.elapsed_s,
        elapsedTo95S: p.elapsed_s_to_95pct,
        plowedM2: p.plowed_m2,
        totalM2: p.total_m2,
        fieldName: currentMapId,
        distance: p.distanceM,
        avgDraft: p.avgDraftKn,
        turns: p.turns,
        reversals: p.reversals,
        deviationM: p.deviationM,
        lostAreaM2: p.lostAreaM2,
        bestLane: p.bestLane,
        trajectory: p.trajectory,
        fieldBboxM: p.fieldBboxM,
        coverageSeries: p.coverageSeries,
        headlandS: p.headlandS,
        lanes: p.lanes,
        penaltyS: (p.deviationM ?? 0) * 1000.0,
        finalTimeS: p.elapsed_s + (p.deviationM ?? 0) * 1000.0,
      })
      setShowFinalReport(true)
    }
    // Wire onProgressMirror to send progress_mirror to backend at 1Hz
    const client = getSessionClient()
    loop.onProgressMirror = (payload) => {
      client.sendProgressMirror(payload)
    }
    return () => {
      loop.onProgressUpdate = prev
      loop.onProgressMirror = undefined
    }
  }, [gameLoopRef, currentMapId, mapRevision])

  // When session transitions to started=true, record the timestamp so
  // onRunSummary can reject stale backend echoes on new session start.
  useEffect(() => {
    if (started) {
      sessionStartedAtRef.current = Date.now()
    } else {
      sessionStartedAtRef.current = null
    }
  }, [started])

  // Subscribe to backend run_summary messages. Merge backend values on top of
  // existing metrics using ?? fallback so local SoilGrid values fill in when
  // backend fields are zero/missing (e.g. progress_mirror didn't reach backend).
  useEffect(() => {
    const client = getSessionClient()
    const off = client.onRunSummary((msg) => {
      if (msg.notFound) return
      // Layer B: reject run_summary messages that belong to a different run.
      // Covers stale echoes fetched by the reconnect/idle hook after the user
      // resets and starts a new run. Accept both camelCase and snake_case in
      // case the wire format varies.
      const currentRun = useSessionStore.getState().runNumber
      const msgRun = (msg as any).runNumber ?? (msg as any).run_number
      if (msgRun != null && currentRun != null && msgRun !== currentRun) {
        return  // stale run_summary from a previous run — ignore
      }
      // Reject stale echoes: if the summary arrives within 3 seconds of session
      // start AND carries no real driving data, it is likely a leftover signal
      // from the previous run's endpoint position — discard it.
      const elapsedSinceStart = sessionStartedAtRef.current != null
        ? (Date.now() - sessionStartedAtRef.current)
        : Infinity
      const hasRealData = (msg.distanceM ?? 0) > 0 || (msg.elapsedS ?? 0) > 3 || (msg.finalCoverage ?? 0) > 0.001
      if (elapsedSinceStart < 3000 && !hasRealData) {
        return
      }
      // Layer C: stale finalized run guard — if the response's startedAt does not
      // match the current session's startedAt, this is a disk-cached summary from
      // a previous run. Skip both setFinalReportMetrics and setShowFinalReport.
      const currentStartedAt = useSessionStore.getState().startedAt
      if (msg.startedAt && currentStartedAt && msg.startedAt !== currentStartedAt) {
        return  // stale finalized run from disk — ignore entirely
      }
      const local = localFinalPayloadRef.current
      setFinalReportMetrics(prev => {
        // Helper: prefer backend value when > 0, else fall back to local then previous.
        const preferNonZero = (backendVal: number, ...fallbacks: (number | undefined | null)[]) => {
          if (backendVal > 0) return backendVal
          for (const f of fallbacks) { if (f != null && f > 0) return f }
          return backendVal // keep 0 if all fallbacks are also 0/null
        }
        const merged: FinalReportMetrics = {
          // Backend fields are authoritative; fall back to local SoilGrid values when
          // backend sent 0 (e.g. progress_mirror didn't reach backend, distanceM=0).
          finalCoverage: preferNonZero(msg.finalCoverage, local?.progress, prev?.finalCoverage),
          elapsedS: preferNonZero(msg.elapsedS, prev?.elapsedS),
          elapsedTo95S: msg.elapsedTo95S ?? local?.elapsed_s_to_95pct ?? prev?.elapsedTo95S ?? null,
          plowedM2: preferNonZero(msg.plowedM2, local?.plowed_m2, prev?.plowedM2),
          totalM2: preferNonZero(msg.totalM2, local?.total_m2, prev?.totalM2),
          distance: preferNonZero(msg.distanceM, local?.distanceM, prev?.distance),
          avgSpeed: msg.avgSpeedKmh > 0 ? msg.avgSpeedKmh : prev?.avgSpeed,
          turns: msg.turns > 0 ? msg.turns : (local?.turns ?? prev?.turns),
          reversals: msg.reversals > 0 ? msg.reversals : (local?.reversals ?? prev?.reversals),
          headlandS: msg.headlandS > 0 ? msg.headlandS : prev?.headlandS,
          lanes: preferNonZero(msg.lanes ?? 0, local?.lanes, prev?.lanes),
          deviationM: msg.deviationM > 0 ? msg.deviationM : (local?.deviationM ?? prev?.deviationM),
          avgDraft: msg.avgDraftKn > 0 ? msg.avgDraftKn : (local?.avgDraftKn ?? prev?.avgDraft),
          lostAreaM2: msg.lostAreaM2 > 0 ? msg.lostAreaM2 : (local?.lostAreaM2 ?? prev?.lostAreaM2),
          bestLane: msg.bestLane ?? local?.bestLane ?? prev?.bestLane,
          runNumber: msg.runNumber ?? prev?.runNumber,
          fieldName: msg.fieldName || prev?.fieldName,
          planName: msg.planName || prev?.planName,
          penaltyS: msg.penaltyS ?? prev?.penaltyS ?? 0,
          finalTimeS: msg.finalTimeS ?? prev?.finalTimeS ?? 0,
          // Local-only fields (needed for replay viz and chart)
          trajectory: local?.trajectory ?? prev?.trajectory,
          fieldBboxM: local?.fieldBboxM ?? prev?.fieldBboxM,
          coverageSeries: local?.coverageSeries ?? prev?.coverageSeries,
        }
        return merged
      })
      // Layer A: only show FinalReport when a session is actively started,
      // OR when the server marks the run as finalized (restore FinishScreen on refresh).
      // Guards against a stale run_summary arriving after the user reset and
      // before the next run begins.
      if (useSessionStore.getState().started || msg.finalized === true) setShowFinalReport(true)
    })
    return () => off()
  }, [])

  useEffect(() => {
    if (selectedMapId == null) return
    if (String(selectedMapId) !== currentMapId) {
      void swapMap(String(selectedMapId))
    }
  }, [selectedMapId, currentMapId, swapMap])

  useEffect(() => {
    gameLoopRef.current?.setManualGear(manualGear)
  }, [manualGear, gameLoopRef])

  useEffect(() => {
    gameLoopRef.current?.setTimeScale(speedMultiplier)
  }, [speedMultiplier, gameLoopRef])

  // Show one-time speed warning when user selects a multiplier > 1
  // Reset the flag when speed returns to 1x, allowing re-trigger on next elevation
  useEffect(() => {
    if (speedMultiplier === 1) {
      speedWarningShownRef.current = false
      return
    }
    if (speedMultiplier > 1 && !speedWarningShownRef.current && !isSpeedWarningDismissed()) {
      speedWarningShownRef.current = true
      setSpeedWarningOpen(true)
    }
  }, [speedMultiplier])

  useEffect(() => {
    const ip = IMPLEMENT_PRESETS[implementPreset as ImplementPresetKey]
    if (!ip || !gameLoopRef.current) return
    gameLoopRef.current.setImplementConfig({
      type: ip.type,
      width: ip.width,
      depth: ip.depth,
      hitchOffset: 3.0,
      mass: ip.mass,
    })
  }, [implementPreset, gameLoopRef, mapRevision])

  const applyImplement = useCallback((type: 'plow' | 'rotary' | null) => {
    const loop = gameLoopRef.current
    if (!loop) return
    if (type === null) {
      loop.setImplementActive(false)
      return
    }
    // Prefer the user-selected preset if its type matches.
    const currentPreset = IMPLEMENT_PRESETS[implementPreset as ImplementPresetKey]
    let cfgWidth: number, cfgDepth: number, cfgMass: number
    if (currentPreset && currentPreset.type === type) {
      cfgWidth = currentPreset.width
      cfgDepth = currentPreset.depth
      cfgMass = currentPreset.mass
    } else {
      // Fallback: pick first preset matching this type.
      const fallback = Object.values(IMPLEMENT_PRESETS).find((p) => p.type === type)
      cfgWidth = fallback?.width ?? (type === 'plow' ? 1.2 : 2.0)
      cfgDepth = fallback?.depth ?? (type === 'plow' ? 0.25 : 0.15)
      cfgMass = fallback?.mass ?? 0
    }
    loop.setImplementConfig({ type, width: cfgWidth, depth: cfgDepth, hitchOffset: 3.0, mass: cfgMass })
    loop.setImplementActive(true)
  }, [gameLoopRef, implementPreset])

  const togglePlow = useCallback(() => {
    setImplementType(prev => {
      const next = prev === 'plow' ? null : 'plow'
      applyImplement(next)
      return next
    })
  }, [applyImplement])

  const toggleRotary = useCallback(() => {
    setImplementType(prev => {
      const next = prev === 'rotary' ? null : 'rotary'
      applyImplement(next)
      return next
    })
  }, [applyImplement])

  const [hitchDown, setHitchDown] = useState(false)
  const onToggleHitch = useCallback(() => {
    setHitchDown((prev) => {
      const next = !prev
      const loop = gameLoopRef.current
      if (loop) {
        loop.setImplementPosition(next ? 1 : 0)
        loop.setImplementActive(next)
      }
      return next
    })
  }, [gameLoopRef])

  useEffect(() => {
    if (manualGear === -1 && hitchDown) {
      setHitchDown(false)
      const loop = gameLoopRef.current
      if (loop) {
        loop.setImplementPosition(0)
        loop.setImplementActive(false)
      }
    }
  }, [manualGear, hitchDown, gameLoopRef])


  const handleResetCurrentMap = useCallback(() => {
    keysRef.current.clear()
    inputRef.current = { throttle: 0, brake: 0, steer: 0 }
    throttleRef.current = 0
    brakeRef.current = 0
    heldKeysRef.current.clear()
    manualSteerRef.current = 0
    dirtyPatchesRef.current.length = 0
    setImplementType(null)
    void resetMap()
  }, [resetMap])

  // Clear all run-specific UI state back to first-launch values.
  // Called from both handleReset and handleFinalReportNewRun.
  const clearAllRunUIState = useCallback(() => {
    setProgress(undefined)
    setElapsedS(undefined)
    setCrossTrackError(0)
    setHeadingDrift(0)
    setAreaDoneM2(0)
    setAreaTotalM2(0)
    setShowFinalReport(false)
    setFinalReportMetrics(null)
    localFinalPayloadRef.current = null
    setHitchDown(false)
    setManualGear(1)
  }, [])

  const handleReset = useCallback(() => {
    clearAllRunUIState()
    handleResetCurrentMap()
    useSessionStore.getState().coldStartReset()
    getSessionClient().sendResetSession()
  }, [clearAllRunUIState, handleResetCurrentMap])

  // Final Report: "New run" closes the report, resets, and returns to start screen
  const handleFinalReportNewRun = useCallback(() => {
    clearAllRunUIState()
    handleReset()
    setStarted(false)
  }, [clearAllRunUIState, handleReset, setStarted])

  // 8C + 8B: Restore vehicle pose and tilled cells from snapshot on reconnect.
  // Fires on every simulation_snapshot_patched; first receipt restores transform,
  // subsequent receipts apply incremental tillage from other clients (multi-UI sync).
  useEffect(() => {
    const restoredPoseRef = { current: false }
    const off = getSessionClient().onSnapshotPatched((m) => {
      const loop = gameLoopRef.current
      if (!loop) return

      // 8C: restore tractor pose — first snapshot only, and only when no RDDF
      // has been loaded. Once an RDDF arrives we own the pose (teleport to wp0).
      const store = useSessionStore.getState()
      const rddfActive = store.rddfWaypointsLL.length > 0 || store.isAutonomous
      if (!restoredPoseRef.current && m.transform && !rddfActive) {
        const t = m.transform
        loop.setVehiclePose(t.x, t.z, t.heading)
        restoredPoseRef.current = true
      }

      // 8B: apply sparse tillage patch to SoilGrid
      if (m.tillagePatch && m.tillagePatch.length > 0) {
        loop.applyTillagePatch(m.tillagePatch)
      }

      // TODO-11: extract tracking stats from snapshot
      if (m.stats) {
        setProgress(m.stats.progress)
        setElapsedS(m.stats.elapsed_s)
        const areaDoneM2 = m.stats.area_done_m2 ?? 0
        setAreaDoneM2(areaDoneM2)
        setAreaTotalM2(m.stats.area_total_m2 ?? 0)
        setTrackingCrUnit(m.stats.tracking_cr_unit ?? 0)
        setOverheadScaleKm(m.stats.overhead_scale_km ?? 0)
      }
    })
    return () => off()
  }, [gameLoopRef])

  // Refresh recovery: when WS reconnects, request run_summary for the current run number
  // so FinalReport can be restored after a page refresh.
  useEffect(() => {
    const client = getSessionClient()
    let prevConnected = false
    const unsub = useSessionStore.subscribe((state) => {
      const becameConnected = state.wsConnected && !prevConnected
      prevConnected = state.wsConnected
      if (becameConnected && state.runNumber > 0) {
        client.sendGetRunSummary(state.runNumber)
      }
    })
    return () => unsub()
  }, [])

  // Mount-time recovery: if wsConnected is already true when this component mounts
  // (e.g. page refresh with existing WS session), the subscribe edge-detector above
  // won't fire. Request run_summary directly so FinishScreen can be restored.
  useEffect(() => {
    if (!wsConnected) return
    const state = useSessionStore.getState()
    if (state.runNumber > 0) {
      getSessionClient().sendGetRunSummary(state.runNumber)
    }
  }, [wsConnected])

  // Re-project field polygon to the RDDF ENU origin so polygon and RDDF share the same frame.
  // Runs whenever rddfOrigin or rawPolygonsLonLat change (i.e. after rddf_path arrives).
  useEffect(() => {
    if (!rddfOrigin || rawPolygonsLonLat.length === 0) return
    const loop = gameLoopRef.current
    if (!loop) return
    const reprojected = reprojectPolygons(rawPolygonsLonLat, rddfOrigin)
    if (reprojected.length === 0) return
    loop.setFields(reprojected)
    loop.setFieldPolygon(reprojected[0].polygon)
  }, [rddfOrigin, rawPolygonsLonLat, gameLoopRef, mapRevision])

  // Teleport vehicle to first waypoint, start engine, enter autonomous mode when rddfWaypoints arrives.
  // headingRad uses GPS compass convention (0 = north, CW+, in radians).
  // Waypoints are flipped to UI convention (+z = north) at the protocol boundary
  // in SessionClient, so dz here is already the north component:
  //   heading = atan2(east, north) = atan2(dx, dz)
  useEffect(() => {
    if (rddfWaypoints.length === 0) return
    // Guard: skip teleport if no map is selected, or if we already applied
    // teleport for this exact selectedMapId (prevents stale-map re-teleport).
    if (!selectedMapId) return
    if (appliedMapIdRef.current === String(selectedMapId)) return
    const wp0 = rddfWaypoints[0]
    let heading = 0
    for (let i = 1; i < rddfWaypoints.length; i++) {
      const wpi = rddfWaypoints[i]
      const dx = wpi.x - wp0.x
      const dz = wpi.z - wp0.z
      if (dx * dx + dz * dz > 1e-6) {
        heading = Math.atan2(dx, dz)
        break
      }
    }
    const loop = gameLoopRef.current
    if (!loop) return
    // Mark this map as applied before the async frame so concurrent updates
    // to rddfWaypoints (same mapId) don't trigger a second teleport.
    appliedMapIdRef.current = String(selectedMapId)
    // Defer teleport to the next frame so it wins against any synchronous
    // snapshot-driven pose restore that may run in the same tick.
    requestAnimationFrame(() => {
      loop.setVehiclePose(wp0.x, wp0.z, heading)
      loop.setInput({ throttle: 0, brake: 0, steer: 0 })
      setRunning(true)
      setManualGear(null)
      setIsAutonomous(true)
    })
    // mapRevision is in the deps so this re-fires once GameLoop finishes its
    // async init — otherwise an rddf_path that arrived before mount leaves the
    // tractor stranded at its initial injectState() pose.
  }, [rddfWaypoints, selectedMapId, setIsAutonomous, mapRevision])

  // Reset appliedMapIdRef when selectedMapId changes so the next map's teleport
  // is always allowed. This covers both map-switch and reset (selectedMapId -> '').
  useEffect(() => {
    appliedMapIdRef.current = null
  }, [selectedMapId])

  // Reset command index when a new RDDF path is loaded.
  useEffect(() => {
    prevCommandIdxRef.current = -1
  }, [rddfWaypoints])

  // Inject RDDF waypoints into GameLoop whenever they change so CTE computation
  // has the latest reference path available.
  useEffect(() => {
    gameLoopRef.current?.setRddfWaypoints(rddfWaypoints.map(wp => ({ x: wp.x, z: wp.z })))
  }, [rddfWaypoints, mapRevision])

  // Wire backend WS commands that need React state mutations.
  useEffect(() => {
    backendCommandHandlerRef.current = (cmd: BackendCommand) => {
      switch (cmd.kind) {
        case 'steer_motor':
          break
        case 'throttle':
          break
        case 'brake':
          break
        case 'engine':
          setRunning(cmd.on)
          break
        case 'gear':
          setManualGear(cmd.gear)
          break
        case 'implement': {
          const down = cmd.type !== null
          setHitchDown(down)
          setImplementType(cmd.type)  // sync visual with backend command
          const loop = gameLoopRef.current
          if (loop) {
            loop.setImplementPosition(down ? 1 : 0)
            loop.setImplementActive(down)
          }
          break
        }
        case 'pp_telemetry': {
          // Track command index for next pp_telemetry update
          const nextIdx = cmd.commandIdx
          prevCommandIdxRef.current = nextIdx
          // Update TRACKING section: crossTrack is in metres, headingErr is in radians → convert to degrees
          setCrossTrackError(cmd.crossTrack)
          setHeadingDrift(cmd.headingErr * (180 / Math.PI))
          return
        }
        case 'reset':
          handleResetCurrentMap()
          break
        case 'track_complete': {
          setIsAutonomous(false)
          setRunning(false)
          setManualGear(1)
          if (throttleRef && throttleRef.current !== undefined) throttleRef.current = 0
          if (brakeRef && brakeRef.current !== undefined) brakeRef.current = 0
          const loop = gameLoopRef.current
          if (loop) {
            loop.setInput({ throttle: 0, brake: 0, steer: 0 })
            loop.triggerFinal()
          }
          break
        }
      }
    }
  }, [setRunning, setHitchDown, handleResetCurrentMap, setIsAutonomous])

  // Keyboard input
  useEffect(() => {
    const down = (e: KeyboardEvent) => {
      const key = e.key.toLowerCase()
      keysRef.current.add(key)
      heldKeysRef.current.add(key)
      if (['arrowup', 'arrowdown', 'arrowleft', 'arrowright', ' ', 'p', 'r', 'c'].includes(key)) {
        e.preventDefault()
      }
      if (key === 'p') togglePlow()
      if (key === 'r') toggleRotary()
      if (key === 'l') { onToggleHitch(); return }
      if (key === 'h') setSensorOverlayVisible(v => !v)
      if (key === 'c') setCameraMode(m => (m === 'follow' ? 'free' : 'follow'))
    }
    const up = (e: KeyboardEvent) => {
      const key = e.key.toLowerCase()
      keysRef.current.delete(key)
      heldKeysRef.current.delete(key)
    }
    const onBlur = () => { heldKeysRef.current.clear() }
    const onVisChange = () => { if (document.hidden) heldKeysRef.current.clear() }
    window.addEventListener('keydown', down)
    window.addEventListener('keyup', up)
    window.addEventListener('blur', onBlur)
    document.addEventListener('visibilitychange', onVisChange)
    return () => {
      window.removeEventListener('keydown', down)
      window.removeEventListener('keyup', up)
      window.removeEventListener('blur', onBlur)
      document.removeEventListener('visibilitychange', onVisChange)
    }
  }, [togglePlow, toggleRotary, onToggleHitch])

  // Block accidental page refresh / tab close while a run is in progress.
  // FinishScreen (showFinalReport=true) is already finalized, so no guard needed there.
  useEffect(() => {
    if (showFinalReport) return
    if (!started) return

    const handler = (e: BeforeUnloadEvent) => {
      e.preventDefault()
      // Some browsers require returnValue to be set to trigger the dialog.
      e.returnValue = ''
      return ''
    }
    window.addEventListener('beforeunload', handler)
    return () => window.removeEventListener('beforeunload', handler)
  }, [started, showFinalReport])

  const handleQualityChange = useCallback((name: QualityPresetName) => {
    setQualityPreset(name)
    saveQualityPreset(name)
  }, [])

  // Cleanup webgl context lost/restored listeners on unmount
  useEffect(() => {
    return () => {
      const dom = canvasDomRef.current
      const lost = lostHandlerRef.current
      const restored = restoredHandlerRef.current
      if (dom && lost) dom.removeEventListener('webglcontextlost', lost as EventListener, false)
      if (dom && restored) dom.removeEventListener('webglcontextrestored', restored as EventListener, false)
    }
  }, [])

  if (!isWebGL2) {
    return (
      <div
        style={{
          position: 'fixed',
          inset: 0,
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          background: '#111',
          color: 'white',
          fontSize: '1.2rem',
          textAlign: 'center',
          padding: '1rem',
        }}
      >
        WebGL2 is required. <a href="/" style={{ color: '#7ec8ff', marginLeft: 8 }}>Open default mode</a>
      </div>
    )
  }

  const vehicleEnu = gpsToEnu(lightState.latitude, lightState.longitude);

  return (
    <div style={{ position: 'fixed', inset: 0, display: 'flex', flexDirection: 'column', background: '#000' }}>
      <TopBar qualityPreset={qualityPreset} onQualityChange={handleQualityChange} onReset={handleReset} />
      <div style={{ position: 'relative', flex: 1, minHeight: 0, display: 'flex' }}>
      <div style={{ flex: 1, minWidth: 0, position: 'relative' }}>
        <Canvas
          shadows={{ type: PCFShadowMap }}
          dpr={spec.pixelRatioCap}
          camera={{ position: [0, 10, 16], fov: 55, far: spec.cameraFar }}
          gl={{ stencil: true, antialias: true, alpha: true }}
          style={{ width: '100%', height: '100%' }}
          onCreated={({ gl }) => {
            const dom = gl.domElement
            canvasDomRef.current = dom
            const onLost = (e: Event) => {
              e.preventDefault()
              setContextLost(true)
              gameLoopRef.current?.stop()
            }
            const onRestored = () => {
              setContextLost(false)
              // Force full re-upload by exceeding shouldFullReupload threshold
              const emptyData = new Float32Array(0)
              for (let i = 0; i < 5001; i++) {
                dirtyPatchesRef.current.push({ startCol: 0, startRow: 0, width: 0, height: 0, data: emptyData })
              }
              if (running) {
                gameLoopRef.current?.start()
              }
            }
            lostHandlerRef.current = onLost
            restoredHandlerRef.current = onRestored
            dom.addEventListener('webglcontextlost', onLost as EventListener, false)
            dom.addEventListener('webglcontextrestored', onRestored as EventListener, false)
          }}
        >
          <QualityApplier cameraFar={spec.cameraFar} />
          <SkyAndLights
            shadowEnabled={spec.shadowEnabled}
            shadowMapSize={spec.shadow?.shadowMapSize ?? 2048}
            fogDensity={spec.fogDensity}
          />
          <FarGround />
          {soilGrid && (
            <FieldTerrain
              key={`${mapRevision}-${qualityPreset}`}
              soilGrid={soilGrid}
              dirtyPatchesRef={dirtyPatchesRef}
              vehicleStateRef={vehicleStateRef}
              terrainSegX={spec.terrainSegments.x}
              terrainSegZ={spec.terrainSegments.z}
              decalW={spec.decalResolution.w}
              decalH={spec.decalResolution.h}
              tireDecalEnabled={spec.tireDecal}
              tractorScale={tractorScale}
              geometryRef={tractorGeometryRef}
              mapWidth={mapSize.width}
              mapHeight={mapSize.height}
              mapCenterX={mapSize.centerX}
              mapCenterZ={mapSize.centerZ}
            />
          )}
          <FieldBoundaryRidge />
          <RddfPathDriver />
          <Suspense fallback={null}>
            <Tractor
              vehicleStateRef={vehicleStateRef}
              implementType={implementSpec?.type ?? implementType}
              implementWidth={implementSpec?.width}
              implementDepth={implementSpec?.depth}
              scale={tractorScale}
              hitchDown={hitchDown}
              geometryRef={tractorGeometryRef}
            />
          </Suspense>
          <CameraRig targetRef={vehicleStateRef} mode={cameraMode} />
          <InputDriver
            heldKeysRef={heldKeysRef}
            throttleRef={throttleRef}
            brakeRef={brakeRef}
            keysRef={keysRef}
            manualSteerRef={manualSteerRef}
            inputRef={inputRef}
            gameLoopRef={gameLoopRef}
            isAutonomous={isAutonomous}
          />
          {import.meta.env.DEV && <DrawCallStats onSample={handlePerfSample} />}
          {DevStats && (
            <Suspense fallback={null}>
              <DevStats />
            </Suspense>
          )}
        </Canvas>

        {import.meta.env.DEV && (
          <div
            style={{
              position: 'absolute',
              top: 8,
              right: 8,
              zIndex: 25,
              color: '#0f0',
              fontFamily: 'monospace',
              fontSize: 12,
              background: 'rgba(0,0,0,0.5)',
              padding: 4,
              pointerEvents: 'none',
            }}
          >
            FPS: {perfStats.fps} | DC: {perfStats.drawCalls}
          </div>
        )}

        {contextLost && (
          <div
            style={{
              position: 'absolute',
              inset: 0,
              background: 'rgba(0,0,0,0.7)',
              color: 'white',
              zIndex: 25,
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              fontSize: '1.5rem',
            }}
          >
            Recovering...
          </div>
        )}

        <SensorOverlay vehicleState={lightState} soilGrid={soilGrid} visible={sensorOverlayVisible} />
      </div>

        <RightConsole
          progress={progress ?? 0}
          elapsedS={elapsedS ?? 0}
          speed={lightState.speed}
          heading={lightState.heading}
          posX={lightState.latitude}
          posY={lightState.longitude}
          crossTrackError={crossTrackError}
          headingDrift={headingDrift}
          vehicleX={vehicleEnu.x}
          vehicleZ={vehicleEnu.z}
          vehicleHeading={lightState.heading}
          coverageGoal={coverageGoal}
          areaDoneM2={areaDoneM2}
          areaTotalM2={areaTotalM2}
          trackingCrUnit={trackingCrUnit}
          overheadScaleKm={overheadScaleKm}
        />
      </div>

      {/* Final Report overlay — shown when run completes */}
      {showFinalReport && finalReportMetrics && (
        <FinalReport
          metrics={finalReportMetrics}
          onNewRun={handleFinalReportNewRun}
        />
      )}

      {/* One-time speed multiplier warning dialog */}
      <SpeedWarningDialog
        open={speedWarningOpen}
        onClose={() => setSpeedWarningOpen(false)}
      />
    </div>
  )
}
