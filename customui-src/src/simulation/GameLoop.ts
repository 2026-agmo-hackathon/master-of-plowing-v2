import { DynamicVehicle, type WheelSoilState } from './DynamicVehicle'
import type { VehicleState } from './VehicleSystem'
import { SoilGrid } from './SoilGrid'
import { SoilSystem } from './SoilSystem'
import type { TerrainPatch, WheelContact } from './SoilSystem'
import { ImplementSystem } from './ImplementSystem'
import { PlowEffect } from './PlowEffect'
import { RotaryEffect } from './RotaryEffect'
import type { ImplementConfig, SimulationConfig } from './ImplementTypes'
import { DEFAULT_SIM_CONFIG } from './ImplementTypes'
import { appSteerCommandToVehicleNormalized } from './AppSignalConvention'
import { clamp, TRACTOR_CONFIG } from './TractorConfig'
import { applyWheelUntill, WheelUntillInput } from './WheelUntillEffect'
import { getOriginLat, getOriginLon, getMetersPerDegLat, getMetersPerDegLon } from './GpsOrigin'
import { ThreePointHitch } from './ThreePointHitch'
import type { SoilType, SoilTypeName } from './SoilType'
import { getSoilProperties, getSoilType } from './SoilType'

/**
 * Xorshift128 PRNG — deterministic, seeded pseudo-random number generator.
 * Produces values in [0, 1) matching the Math.random() contract.
 *
 * DETERMINISM NOTE:
 * WebGL rendering is non-deterministic (GPU floating-point differences across vendors/drivers).
 * Physics simulation and tilled state (score-relevant) are deterministic via fixed timestep + seeded PRNG.
 * Score calculation is not affected by WebGL non-determinism.
 */
class Xorshift128 {
  private a: number
  private b: number
  private c: number
  private d: number

  constructor(seed: number) {
    this.a = seed >>> 0
    this.b = (seed ^ 0x6c078965) >>> 0
    this.c = (seed ^ 0x9e3779b9) >>> 0
    this.d = (seed ^ 0x6a09e667) >>> 0
  }

  next(): number {
    // xorshift128 algorithm
    let t = this.d
    const s = this.a
    this.d = this.c
    this.c = this.b
    this.b = s
    t ^= t << 11
    t ^= t >>> 8
    this.a = (t ^ s ^ (s >>> 19)) >>> 0
    // Map uint32 to [0, 1)
    return this.a / 0x100000000
  }
}

export interface GameInput {
  throttle: number  // 0-1
  brake: number     // 0-1
  steer: number     // -1 to 1
  reverse?: boolean
  manualGear?: number | null  // null = auto
}

const TELEMETRY_STOP_SPEED_MPS = 0.02

const MAX_TRAJ_SAMPLES = 30000

export type TrajectoryPoint = {
  t: number;        // sim time seconds
  x: number;        // world meters
  y: number;        // world meters
  heading: number;  // radians
  engaged: boolean; // implement active
}

export type ProgressPayload = {
  progress: number;           // 0..1
  elapsed_s: number;          // sim time elapsed since start
  elapsed_s_to_95pct: number | null;  // sim time when 95% was first reached, null if not yet
  plowed_m2: number;
  total_m2: number;
  final: boolean;
  // Extended fields (only populated when final: true)
  distanceM?: number;
  avgDraftKn?: number;
  turns?: number;
  reversals?: number;
  deviationM?: number;          // RMS of CTE
  lostAreaM2?: number;
  bestLane?: { index: number; ratio: number };
  trajectory?: TrajectoryPoint[];
  fieldBboxM?: { minX: number; maxX: number; minY: number; maxY: number };
  coverageSeries?: Array<{ t: number; coverage: number }>;
  lanes?: number;
  headlandS?: number;
}

export class GameLoop {
  private vehicle: DynamicVehicle
  private running = false
  private animFrameId: number | null = null
  private timeoutId: ReturnType<typeof setTimeout> | null = null
  private lastTime: number | null = null
  private currentInput: GameInput = { throttle: 0, brake: 0, steer: 0 }
  private lastState: VehicleState | null = null
  private manualSteerActive = false
  private soilGrid: SoilGrid
  private soilSystem: SoilSystem
  private implementSystem: ImplementSystem
  private lastSoilUpdate = 0  // 10Hz throttle
  private prevWheelTickSeen: boolean = false
  private config: SimulationConfig = { ...DEFAULT_SIM_CONFIG }
  private soilTypeName: SoilTypeName = 'loam'
  private currentSoil: SoilType = getSoilType('loam')
  private hitch = new ThreePointHitch()

  // Acceleration and determinism
  private timeScale = 1
  private deterministicMode = false
  private prng: Xorshift128 | null = null

  // Progress broadcast (1Hz, separate from 10Hz soil update)
  private fieldPolygon: [number, number][] | null = null
  private elapsedSimS = 0
  public outsideField = false
  private elapsedTo95: number | null = null
  private lastProgressBroadcastS = 0
  private finalBroadcasted = false
  public onProgressUpdate?: (p: ProgressPayload) => void
  // Optional callback for 1Hz progress_mirror to backend
  public onProgressMirror?: (payload: {
    finalCoverage: number;
    plowedM2: number;
    totalM2: number;
    avgDraftKn: number;
    lostAreaM2: number;
    elapsedTo95S: number | null;
  }) => void

  // Run metric accumulators (reset on start())
  private accDistanceM = 0
  private accLastPos: { x: number; z: number } | null = null
  private accSumDraftKn = 0
  private accDraftSamples = 0
  private accLastHeading: number | null = null
  private accHeadingAccum = 0   // accumulated heading change (rad) for turn counting
  private accTurns = 0
  private accLastVelSign = 0    // sign of velocity for reversal detection
  private accLastReversalS = 0  // sim time of last reversal (debounce)
  private accReversals = 0
  private accSumCteSq = 0
  private accCteSamples = 0
  private accTrajectory: TrajectoryPoint[] = []
  private accLastTrajS = 0      // last trajectory sample time
  private trajSampleIntervalS = 0.2  // adaptive sampling interval (doubles on cap)
  private accCoverageSeries: Array<{ t: number; coverage: number }> = []
  private accLanes = 0
  private accPrevImplementOn = false
  private accHeadlandS = 0
  private rddfWaypoints: Array<{ x: number; z: number }> = []

  onStateUpdate: ((state: VehicleState) => void) | null = null
  onTerrainDirty: ((patches: TerrainPatch[]) => void) | null = null
  onTillageCells: ((cells: { col: number; row: number; depth?: number }[], gridCols: number) => void) | null = null
  /** Called once per sub-step with minimal position data for high-frequency transform push. */
  onPostSubStep: ((x: number, z: number, headingRad: number, speed: number) => void) | null = null

  constructor(
    terrainNoiseFn?: (x: number, z: number) => number,
    options: {
      fields?: { polygon: [number, number][] }[]
      soilType?: SoilTypeName
      size?: { width?: number; height?: number; centerX?: number; centerZ?: number }
    } = {},
  ) {
    const noiseFn = terrainNoiseFn ?? (() => 0)
    this.vehicle = new DynamicVehicle()
    this.soilGrid = new SoilGrid(noiseFn, {
      ...(options.fields ? { fields: options.fields } : {}),
      width:   options.size?.width,
      height:  options.size?.height,
      centerX: options.size?.centerX,
      centerZ: options.size?.centerZ,
    })
    this.soilSystem = new SoilSystem(this.soilGrid, this.config)
    this.implementSystem = new ImplementSystem(this.soilGrid)
    if (options.soilType) {
      this.setSoilType(options.soilType)
    } else {
      this.vehicle.setFrictionCoeff(this.currentSoil.friction)
    }

    // Read URL parameters for deterministic mode and seed
    if (typeof window !== 'undefined') {
      const params = new URLSearchParams(window.location.search)
      if (params.get('deterministic') === '1') {
        this.deterministicMode = true
      }
      const seedParam = params.get('seed')
      if (seedParam !== null) {
        this.setRandomSeed(parseInt(seedParam, 10))
      }
    }
  }

  /**
   * Set simulation time scale factor (1 = realtime, 5-10 = accelerated).
   * When timeScale > 1 and deterministicMode is enabled, uses setTimeout(0)
   * with fixed dt (1/60s * timeScale) instead of requestAnimationFrame.
   */
  setTimeScale(factor: number): void {
    this.timeScale = Math.max(1, factor)
  }

  getTimeScale(): number {
    return this.timeScale
  }

  /**
   * Replace Math.random with a seeded xorshift128 PRNG for deterministic simulation.
   * Only affects code paths that call Math.random() after this call.
   */
  setRandomSeed(seed: number): void {
    this.prng = new Xorshift128(seed)
    // Replace Math.random globally for deterministic physics
    Math.random = () => this.prng!.next()
  }

  setFields(fields: { polygon: [number, number][] }[]): void {
    this.soilGrid.setFields(fields)
    this.soilGrid.initRidges(fields, { heightM: 0.24, periodM: 2.5, axis: 'z' })
    this.prevWheelTickSeen = false

    // Emit a full patch so renderer can resync terrain heights
    const fullPatch: TerrainPatch = {
      startCol: 0,
      startRow: 0,
      width: this.soilGrid.cols,
      height: this.soilGrid.rows,
      data: new Float32Array(this.soilGrid.cols * this.soilGrid.rows),
    }
    for (let r = 0; r < this.soilGrid.rows; r++) {
      for (let c = 0; c < this.soilGrid.cols; c++) {
        fullPatch.data[r * this.soilGrid.cols + c] = this.soilGrid.getHeight(c, r)
      }
    }
    this.onTerrainDirty?.([fullPatch])
  }

  updateConfig(partial: Partial<SimulationConfig>): void {
    this.config = { ...this.config, ...partial }
    this.soilSystem.updateConfig(partial)

    // Update implement config if relevant fields changed
    const implFields: (keyof SimulationConfig)[] = ['plowDepth', 'plowWidth', 'rotaryDepth', 'rotaryWidth']
    const hasImplChange = implFields.some(k => k in partial)
    if (hasImplChange) {
      const current = this.implementSystem.getConfig()
      if (current) {
        if (current.type === 'plow') {
          this.implementSystem.setConfig({
            ...current,
            depth: this.config.plowDepth,
            width: this.config.plowWidth,
          })
        } else if (current.type === 'rotary') {
          this.implementSystem.setConfig({
            ...current,
            depth: this.config.rotaryDepth,
            width: this.config.rotaryWidth,
          })
        }
      }
    }
  }

  loadSoilGrid(data: { height: string; compaction: string; moisture: string; tilled: string }): void {
    this.soilGrid = SoilGrid.deserialize(data)
    this.soilSystem = new SoilSystem(this.soilGrid, this.config)
    this.implementSystem = new ImplementSystem(this.soilGrid)
    this.prevWheelTickSeen = false

    // Build full terrain patch
    const fullPatch: TerrainPatch = {
      startCol: 0,
      startRow: 0,
      width: this.soilGrid.cols,
      height: this.soilGrid.rows,
      data: new Float32Array(this.soilGrid.cols * this.soilGrid.rows)
    }
    for (let r = 0; r < this.soilGrid.rows; r++) {
      for (let c = 0; c < this.soilGrid.cols; c++) {
        fullPatch.data[r * this.soilGrid.cols + c] = this.soilGrid.getHeight(c, r)
      }
    }
    this.onTerrainDirty?.([fullPatch])
  }

  injectState(lat: number, lon: number, headingDeg: number, _speedKmh?: number): void {
    const x = (lon - getOriginLon()) * getMetersPerDegLon()
    const z = (lat - getOriginLat()) * getMetersPerDegLat()
    const headingRad = headingDeg * Math.PI / 180
    this.vehicle.teleport(x, z, headingRad)
  }

  // Restore vehicle pose from snapshot (reconnect, world-local meters).
  public setVehiclePose(x: number, z: number, headingRad: number): void {
    this.vehicle.teleport(x, z, headingRad)
  }

  // Restore tilled cells from sparse snapshot patch (reconnect).
  // Each cell carries a flat grid index computed as row * cols + col.
  // SoilGrid.cols is always 400 for this project.
  public applyTillagePatch(cells: { idx: number; depth: number }[]): void {
    const cols = this.soilGrid.cols
    const dirtyPatches: import('./SoilSystem').TerrainPatch[] = []

    for (const c of cells) {
      const col = c.idx % cols
      const row = Math.floor(c.idx / cols)
      this.soilGrid.setTilled(col, row, (c.depth ?? 0) > 0)
      // Collect a 1×1 patch for the renderer dirty-update
      const h = this.soilGrid.getHeight(col, row)
      const patch: import('./SoilSystem').TerrainPatch = {
        startCol: col,
        startRow: row,
        width: 1,
        height: 1,
        data: new Float32Array([h]),
      }
      dirtyPatches.push(patch)
    }

    if (dirtyPatches.length > 0) {
      this.onTerrainDirty?.(dirtyPatches)
    }
  }

  start(): void {
    this.running = true
    this.lastTime = null
    // Reset run metric accumulators
    this.accDistanceM = 0
    this.accLastPos = null
    this.accSumDraftKn = 0
    this.accDraftSamples = 0
    this.accLastHeading = null
    this.accHeadingAccum = 0
    this.accTurns = 0
    this.accLastVelSign = 0
    this.accLastReversalS = 0
    this.accReversals = 0
    this.accSumCteSq = 0
    this.accCteSamples = 0
    this.accTrajectory = []
    this.accLastTrajS = 0
    this.trajSampleIntervalS = 0.2
    this.accCoverageSeries = []
    this.accLanes = 0
    this.accPrevImplementOn = false
    this.accHeadlandS = 0
    // rddfWaypoints는 외부 주입이므로 reset하지 않음
    this.elapsedSimS = 0
    this.elapsedTo95 = null
    this.lastProgressBroadcastS = 0
    this.finalBroadcasted = false
    this.prevWheelTickSeen = false
    this.tick()
  }

  stop(): void {
    this.running = false
    if (this.animFrameId !== null) {
      cancelAnimationFrame(this.animFrameId)
      this.animFrameId = null
    }
    if (this.timeoutId !== null) {
      clearTimeout(this.timeoutId)
      this.timeoutId = null
    }
  }

  public isRunning(): boolean {
    return this.running
  }

  setInput(input: GameInput): void {
    // Preserve steer from applyAppCommand and manualGear from setManualGear.
    this.currentInput.throttle = input.throttle
    this.currentInput.brake = input.brake
    this.currentInput.reverse = input.reverse
    if (input.manualGear !== undefined) this.currentInput.manualGear = input.manualGear
  }

  setManualGear(gear: number | null): void {
    this.currentInput.manualGear = gear
  }

  // Called when app sends steering/throttle commands via MQTT
  private appMotorSpeed = 0       // raw motor speed command (±500)
  private appSteerStarted = false // reset steer on first app command
  applyAppCommand(type: 'steer' | 'throttle' | 'brake', value: number): void {
    if (type === 'steer') {
      if (this.manualSteerActive) return
      if (!this.appSteerStarted) {
        this.appSteerStarted = true
        this.vehicle.resetSteer()
      }
      this.appMotorSpeed = value * 500
      const target = appSteerCommandToVehicleNormalized(Math.max(-1, Math.min(1, value)))
      const current = this.vehicle.getMeasuredSteerNormalized()
      const error = target - current
      const KP_STEER_SERVO = 5.0  // gain; tune if too slow/fast
      const motorCmd = Math.max(-1, Math.min(1, error * KP_STEER_SERVO))
      this.currentInput = { ...this.currentInput, steer: motorCmd }
    } else if (type === 'throttle') {
      this.currentInput = { ...this.currentInput, throttle: Math.max(0, Math.min(1, value)) }
    } else if (type === 'brake') {
      this.currentInput = { ...this.currentInput, brake: Math.max(0, Math.min(1, value)) }
    }
  }

  getLastState(): VehicleState | null {
    return this.lastState
  }

  getCurrentSteer(): number {
    return this.currentInput.steer
  }

  setManualSteer(steerNormalized: number): void {
    this.manualSteerActive = true
    const clamped = Math.max(-1, Math.min(1, steerNormalized))
    this.appMotorSpeed = clamped * 500
    this.currentInput = { ...this.currentInput, steer: clamped }
  }

  clearManualSteer(): void {
    this.manualSteerActive = false
    // Don't reset steer to 0 — app's applyAppCommand may have set it
  }

  getSoilGrid(): SoilGrid {
    return this.soilGrid
  }

  setImplementActive(on: boolean): void {
    this.implementSystem.setActive(on)
  }

  isImplementActive(): boolean {
    return this.implementSystem.isActive()
  }

  public setRddfWaypoints(wps: Array<{ x: number; z: number }>): void {
    this.rddfWaypoints = wps
  }

  setSoilType(name: SoilTypeName): void {
    this.soilTypeName = name
    this.currentSoil = getSoilType(name)
    this.vehicle.setFrictionCoeff(this.currentSoil.friction)
    this.implementSystem.setSoilType(name)
  }

  getSoilType(): SoilType {
    return this.currentSoil
  }

  setImplementConfig(config: ImplementConfig): void {
    this.implementSystem.setConfig(config)
    if (config.type === 'plow') {
      this.implementSystem.setEffect(new PlowEffect())
    } else if (config.type === 'rotary') {
      this.implementSystem.setEffect(new RotaryEffect())
    }
  }

  setImplementPosition(pos: number): void {
    if (pos >= 0.5) {
      this.hitch.setTarget(1)
      this.vehicle.setPtoEngaged(true)
    } else {
      this.hitch.setTarget(0)
      this.vehicle.setPtoEngaged(false)
    }
  }

  setFieldPolygon(polygon: [number, number][]): void {
    this.fieldPolygon = polygon
  }

  public triggerFinal(): void {
    if (this.finalBroadcasted) return
    this.finalBroadcasted = true
    const ratio = this.fieldPolygon ? this.soilGrid.getFieldTilledRatio(this.fieldPolygon) : 0
    const totalM2 = this.computeTotalM2()

    // Derived end-of-run metrics
    const avgDraftKn = this.accDraftSamples > 0
      ? this.accSumDraftKn / this.accDraftSamples
      : undefined
    const deviationM = this.accCteSamples > 0
      ? Math.sqrt(this.accSumCteSq / this.accCteSamples)
      : undefined
    const lostAreaM2 = totalM2 > 0 ? Math.max(0, (1 - ratio) * totalM2) : undefined

    // Best lane (20 strips along long axis)
    let bestLane: { index: number; ratio: number } | undefined
    if (this.fieldPolygon && this.fieldPolygon.length >= 3) {
      const strips = this.soilGrid.computeStripCoverage(this.fieldPolygon, 20, 'z')
      let maxRatio = -1
      let maxIdx = 0
      for (let i = 0; i < strips.length; i++) {
        if (strips[i] > maxRatio) {
          maxRatio = strips[i]
          maxIdx = i
        }
      }
      if (maxRatio >= 0) {
        bestLane = { index: maxIdx + 1, ratio: maxRatio }
      }
    }

    // Field bbox for trajectory projection
    let fieldBboxM: { minX: number; maxX: number; minY: number; maxY: number } | undefined
    if (this.fieldPolygon && this.fieldPolygon.length >= 3) {
      let minX = Infinity, maxX = -Infinity, minY = Infinity, maxY = -Infinity
      for (const [x, z] of this.fieldPolygon) {
        if (x < minX) minX = x
        if (x > maxX) maxX = x
        if (z < minY) minY = z
        if (z > maxY) maxY = z
      }
      fieldBboxM = { minX, maxX, minY, maxY }
    }

    this.onProgressUpdate?.({
      progress: ratio,
      elapsed_s: this.elapsedSimS,
      elapsed_s_to_95pct: this.elapsedTo95,
      plowed_m2: ratio * totalM2,
      total_m2: totalM2,
      final: true,
      distanceM: this.accDistanceM > 0 ? this.accDistanceM : undefined,
      avgDraftKn,
      turns: this.accTurns > 0 ? this.accTurns : undefined,
      reversals: this.accReversals,
      deviationM,
      lostAreaM2,
      bestLane,
      trajectory: this.accTrajectory.length > 0 ? this.accTrajectory : undefined,
      fieldBboxM,
      coverageSeries: this.accCoverageSeries.length > 0 ? this.accCoverageSeries : undefined,
      lanes: this.accLanes > 0 ? this.accLanes : undefined,
      headlandS: this.accHeadlandS > 0 ? this.accHeadlandS : undefined,
    })
    this.running = false
  }

  private computeTotalM2(): number {
    if (!this.fieldPolygon || this.fieldPolygon.length < 3) return 0
    let area = 0
    for (let i = 0; i < this.fieldPolygon.length; i++) {
      const [x1, z1] = this.fieldPolygon[i]
      const [x2, z2] = this.fieldPolygon[(i + 1) % this.fieldPolygon.length]
      area += x1 * z2 - x2 * z1
    }
    return Math.abs(area / 2)
  }

  private computeCTE(px: number, pz: number): number {
    const wps = this.rddfWaypoints
    if (wps.length < 2) return 0
    let minDist = Infinity
    for (let i = 0; i < wps.length - 1; i++) {
      const ax = wps[i].x, az = wps[i].z
      const bx = wps[i + 1].x, bz = wps[i + 1].z
      const abx = bx - ax, abz = bz - az
      const apx = px - ax, apz = pz - az
      const denom = abx * abx + abz * abz
      let t = denom > 0 ? (apx * abx + apz * abz) / denom : 0
      if (t < 0) t = 0
      else if (t > 1) t = 1
      const cx = ax + t * abx
      const cz = az + t * abz
      const dx = px - cx, dz = pz - cz
      const d = Math.sqrt(dx * dx + dz * dz)
      if (d < minDist) minDist = d
    }
    return minDist === Infinity ? 0 : minDist
  }

  private tick = (): void => {
    if (!this.running) return

    // Determine if we should use fixed timestep (deterministic accelerated mode)
    const useFixedStep = this.deterministicMode && this.timeScale > 1

    const now = performance.now()
    let baseDt: number

    if (useFixedStep) {
      // Fixed timestep: base dt = 1/60s per sub-step (timeScale sub-steps per frame)
      baseDt = 1 / 60
    } else {
      if (this.lastTime === null) {
        this.lastTime = now
        this.animFrameId = requestAnimationFrame(this.tick)
        return
      }

      // Base dt: wall-clock elapsed, clamped to 1/30s regardless of timeScale
      baseDt = (now - this.lastTime) / 1000
      baseDt = Math.min(baseDt, 1 / 30)
    }

    this.lastTime = now

    // Sub-step count: repeat physics integration timeScale times per frame
    // instead of multiplying dt, keeping each step small to avoid Euler divergence
    const steps = Math.max(1, Math.round(this.timeScale))

    // Accumulate simulated elapsed time (total = baseDt * steps)
    this.elapsedSimS += baseDt * steps * (this.outsideField ? 10 : 1)

    // 1Hz progress broadcast (separate from 10Hz soil update)
    if (this.elapsedSimS - this.lastProgressBroadcastS >= 1.0) {
      this.lastProgressBroadcastS = this.elapsedSimS
      if (!this.finalBroadcasted && this.onProgressUpdate) {
        const ratio = this.fieldPolygon ? this.soilGrid.getFieldTilledRatio(this.fieldPolygon) : 0
        const totalM2 = this.computeTotalM2()
        const plowedM2 = ratio * totalM2
        if (ratio >= 0.95 && this.elapsedTo95 === null) {
          this.elapsedTo95 = this.elapsedSimS
        }
        // Accumulate 1Hz coverage series
        this.accCoverageSeries.push({ t: this.elapsedSimS, coverage: ratio })
        this.onProgressUpdate({
          progress: ratio,
          elapsed_s: this.elapsedSimS,
          elapsed_s_to_95pct: this.elapsedTo95,
          plowed_m2: plowedM2,
          total_m2: totalM2,
          final: false,
        })
        // 1Hz progress_mirror to backend
        if (this.onProgressMirror) {
          const avgDraftKn = this.accDraftSamples > 0
            ? this.accSumDraftKn / this.accDraftSamples
            : 0
          const lostAreaM2 = totalM2 > 0 ? Math.max(0, (1 - ratio) * totalM2) : 0
          this.onProgressMirror({
            finalCoverage: ratio,
            plowedM2,
            totalM2,
            avgDraftKn,
            lostAreaM2,
            elapsedTo95S: this.elapsedTo95,
          })
        }
      }
    }

    // 1. PTO engagement based on implement state
    const soilProps = getSoilProperties(this.soilTypeName)
    if (this.implementSystem.isActive()) {
      this.vehicle.setPtoEngaged(true)
      const implConfig = this.implementSystem.getConfig()
      const PTO_REFERENCE_WIDTH_M = 1.8
      const widthFactor = implConfig.width / PTO_REFERENCE_WIDTH_M
      if (implConfig.type === 'plow') {
        this.vehicle.setPtoLoad(soilProps.plowResistanceNm * widthFactor)
      } else if (implConfig.type === 'rotary') {
        // Rotary uses ~70% of plow resistance for same soil
        this.vehicle.setPtoLoad(soilProps.plowResistanceNm * 0.7 * widthFactor)
      }
    } else {
      this.vehicle.setPtoEngaged(false)
      this.vehicle.setPtoLoad(0)
    }

    // --- Sub-step physics loop ---
    // Instead of multiplying dt by timeScale (which causes Euler divergence at high timeScale),
    // we repeat the physics integration `steps` times with the original baseDt each iteration.
    // Only integration/physics calls go inside; rendering and network calls stay outside.
    // eslint-disable-next-line prefer-const
    let vo!: ReturnType<typeof this.vehicle.update>
    // Out-of-bounds limits: field edge + 10m buffer, centered on map's centerX/centerZ
    const halfW = this.soilGrid.fieldWidth  / 2 + 10
    const halfL = this.soilGrid.fieldLength / 2 + 10
    const cx = this.soilGrid.centerX
    const cz = this.soilGrid.centerZ

    for (let _step = 0; _step < steps; _step++) {
      const dt = baseDt

      // 1b. Three-point hitch dynamics
      this.hitch.update(dt)
      const hitchImplConfig = this.implementSystem.getConfig()
      const effectiveImplementLoad = TRACTOR_CONFIG.implementRearLoadKg + this.implementSystem.getMassKg()
      const hitchOffsetM = hitchImplConfig.hitchOffset
      const wt = this.hitch.computeWeightTransfer(effectiveImplementLoad, hitchOffsetM, TRACTOR_CONFIG.wheelbaseM)
      this.vehicle.setWeightTransfer(wt.frontLoadDelta, wt.rearLoadDelta)

      // 3. DynamicVehicle update

      // 60Hz Geometric Wheel Sink (Synchronous)
      if (this.currentSoil.deformable) {
        const _headingRad = this.vehicle.getHeadingRad()
        const cosH = Math.cos(_headingRad), sinH = Math.sin(_headingRad)
        const hw = TRACTOR_CONFIG.wheelbaseM / 2
        const frontHalfTrack = TRACTOR_CONFIG.frontTrackM / 2
        const rearHalfTrack = TRACTOR_CONFIG.rearTrackM / 2
        const wheelOffsets = [
          { dx: sinH * hw - cosH * frontHalfTrack, dz: cosH * hw + sinH * frontHalfTrack },  // FL
          { dx: sinH * hw + cosH * frontHalfTrack, dz: cosH * hw - sinH * frontHalfTrack },  // FR
          { dx: -sinH * hw - cosH * rearHalfTrack, dz: -cosH * hw + sinH * rearHalfTrack },  // RL
          { dx: -sinH * hw + cosH * rearHalfTrack, dz: -cosH * hw - sinH * rearHalfTrack },  // RR
        ]
        const allDirty: { col: number; row: number }[] = []
        for (const w of wheelOffsets) {
          const wx = this.vehicle.x + w.dx
          const wz = this.vehicle.z + w.dz
          // Compute per-wheel sink target from soil sample at wheel contact point
          const sample = this.soilGrid.getSoilSampleAtWorld(wx, wz)
          const currentH = this.soilGrid.getHeightAtWorld(wx, wz)
          const baseH = currentH + sample.sinkDepth  // restore base height from current sink
          const tilledMul = sample.tilled ? 1.3 : 1.0
          const sinkDepth = 0.06 * sample.moisture * (1 - 0.5 * sample.compaction) * tilledMul
          const targetH = baseH - sinkDepth
          const dirty = this.soilGrid.applyGeometricSink(wx, wz, targetH, TRACTOR_CONFIG.wheelInfluenceRadiusM)
          if (dirty.length > 0) allDirty.push(...dirty)
        }
        if (allDirty.length > 0) {
          this.soilSystem.markCellsDirty(allDirty)
          const patches = this.soilSystem.buildPatches()
          if (patches.length > 0) {
            this.onTerrainDirty?.(patches)
          }
        }
      }

      this.vehicle.setSoilState(this.sampleWheelSoilState(this.vehicle.x, this.vehicle.z, this.vehicle.getHeadingRad()))
      vo = this.vehicle.update(this.currentInput, dt)
      this.outsideField = !this.soilGrid.isInsideAnyField(this.vehicle.x, this.vehicle.z)

      // Out-of-bounds safety check: stop engine if vehicle exceeds field boundary by more than 10m
      if (Math.abs(vo.x - cx) > halfW || Math.abs(vo.z - cz) > halfL) {
        this.running = false
        return
      }

      // Notify per-sub-step listener with lightweight position data for high-frequency GPS push
      if (this.onPostSubStep) {
        const subStepIsCreeping = (vo.gear >= 1 || vo.gear === -1) && this.currentInput.throttle < 0.01 && this.currentInput.brake < 0.01
        const subStepTelemetryStopped = Math.abs(vo.speed) < TELEMETRY_STOP_SPEED_MPS && this.currentInput.throttle < 0.01 && !subStepIsCreeping
        const subStepSpeed = subStepTelemetryStopped ? 0 : Math.abs(vo.speed)
        this.onPostSubStep(vo.x, vo.z, vo.headingRad, subStepSpeed)
      }

      // HEADLAND: accumulate raw dt when implement is OFF (per sub-step)
      if (!this.implementSystem.isActive()) {
        this.accHeadlandS += dt
      }
    }
    // --- End sub-step physics loop ---

    // Per-frame chassis pose readout from SoilGrid current terrain (4 wheel samples).
    // Flat map ridges are injected into SoilGrid, not TerrainSampler's flat heightFn,
    // so state altitude/pitch/roll must come from SoilGrid to match what the
    // renderer and wheel deformation are using.
    // Computed every frame so the renderer's pitch/roll never falls back to 0
    // between 10Hz soil updates. Vehicle physics tilt application stays at
    // 10Hz below to preserve longitudinal-dynamics regression baselines.
    const cosHpose = Math.cos(vo.headingRad), sinHpose = Math.sin(vo.headingRad)
    const hwPose = TRACTOR_CONFIG.wheelbaseM / 2
    const ftHalfPose = TRACTOR_CONFIG.frontTrackM / 2
    const rtHalfPose = TRACTOR_CONFIG.rearTrackM / 2
    const flX = vo.x + sinHpose * hwPose - cosHpose * ftHalfPose
    const flZ = vo.z + cosHpose * hwPose + sinHpose * ftHalfPose
    const frX = vo.x + sinHpose * hwPose + cosHpose * ftHalfPose
    const frZ = vo.z + cosHpose * hwPose - sinHpose * ftHalfPose
    const rlX = vo.x - sinHpose * hwPose - cosHpose * rtHalfPose
    const rlZ = vo.z - cosHpose * hwPose + sinHpose * rtHalfPose
    const rrX = vo.x - sinHpose * hwPose + cosHpose * rtHalfPose
    const rrZ = vo.z - cosHpose * hwPose - sinHpose * rtHalfPose
    const hFLpose = this.soilGrid.getHeightAtWorld(flX, flZ)
    const hFRpose = this.soilGrid.getHeightAtWorld(frX, frZ)
    const hRLpose = this.soilGrid.getHeightAtWorld(rlX, rlZ)
    const hRRpose = this.soilGrid.getHeightAtWorld(rrX, rrZ)
    const frontAvgPose = (hFLpose + hFRpose) / 2
    const rearAvgPose = (hRLpose + hRRpose) / 2
    const leftAvgPose = (hFLpose + hRLpose) / 2
    const rightAvgPose = (hFRpose + hRRpose) / 2
    const frameY = (hFLpose + hFRpose + hRLpose + hRRpose) / 4
    const avgTrackPose = (TRACTOR_CONFIG.frontTrackM + TRACTOR_CONFIG.rearTrackM) / 2
    const framePitch = Math.atan2(frontAvgPose - rearAvgPose, TRACTOR_CONFIG.wheelbaseM)
    const frameRoll = Math.atan2(rightAvgPose - leftAvgPose, avgTrackPose)
    this.vehicle.y = frameY + 0.5  // ground clearance offset

    // 3. GPS coordinates from world position
    const latitude = getOriginLat() + vo.z / getMetersPerDegLat()
    const longitude = getOriginLon() + vo.x / getMetersPerDegLon()
    const headingDeg = vo.headingRad * 180 / Math.PI

    // 4. Build VehicleState
    const measuredSteerAngleDeg = vo.measuredSteerAngle * 180 / Math.PI
    const vehicleFrontWheelAngleDeg = vo.vehicleFrontWheelAngle * 180 / Math.PI
    // Sample soil moisture under vehicle center for HUD overlay
    let soilMoistureSample: number | undefined
    const soilCell = this.soilGrid.getCell(vo.x, vo.z)
    if (soilCell) soilMoistureSample = soilCell.moisture
    const isCreeping = (vo.gear >= 1 || vo.gear === -1) && this.currentInput.throttle < 0.01 && this.currentInput.brake < 0.01
    const telemetryStopped = Math.abs(vo.speed) < TELEMETRY_STOP_SPEED_MPS && this.currentInput.throttle < 0.01 && !isCreeping
    const telemetrySpeed = telemetryStopped ? 0 : Math.abs(vo.speed)
    const telemetrySignedSpeed = telemetryStopped ? 0 : vo.speed
    const state: VehicleState = {
      latitude, longitude, altitude: frameY,
      heading: headingDeg < 0 ? headingDeg + 360 : headingDeg,
      speed: telemetrySpeed, numSat: 12,
      rpm: Math.round(vo.rpm),
      throttle: this.currentInput.throttle,
      brake: this.currentInput.brake,
      steerAngle: vehicleFrontWheelAngleDeg,
      measuredSteerAngleDeg,
      vehicleFrontWheelAngleDeg,
      motorCmdRpm: this.appMotorSpeed,
      accel_x: telemetryStopped ? 0 : vo.accelX,
      accel_y: vo.accelY,
      gyro_z: vo.angularVelocityY,
      roll: frameRoll * 180 / Math.PI,
      pitch: framePitch * 180 / Math.PI,
      yaw: headingDeg < 0 ? headingDeg + 360 : headingDeg,
      gear: vo.gear,
      appSteerCommand: this.appMotorSpeed,  // raw motor speed command (±500)
      steerFeedbackActive: true,
      running: false,
      slip: vo.slip,
      tireLoad: vo.tireLoad,
      soilType: this.soilTypeName,
      soilMoisture: soilMoistureSample,
      wheelSpeed: telemetrySignedSpeed / TRACTOR_CONFIG.tireRadiusM,
      terrainPitchRad: framePitch,
      terrainRollRad: frameRoll,
      localX: vo.x,
      localZ: vo.z,
      headingRad: vo.headingRad,
      wheelOmega: vo.wheelOmega,
      wheelEquivRpm: vo.wheelEquivRpm,
      draftForceN: vo.draftForceN,
      ptoEngaged: vo.ptoEngaged,
      ptoLoad: vo.ptoLoad,
    }
    this.lastState = state

    // ---- Run metric accumulators ----

    // Distance: ‖pos - lastPos‖
    if (this.accLastPos !== null) {
      const dx = vo.x - this.accLastPos.x
      const dz = vo.z - this.accLastPos.z
      this.accDistanceM += Math.hypot(dx, dz)
    }
    this.accLastPos = { x: vo.x, z: vo.z }

    // Draft force (sample when implement engaged)
    if (this.implementSystem.isActive()) {
      const draftKn = this.implementSystem.getLastDraftForceN() / 1000
      if (draftKn > 0) {
        this.accSumDraftKn += draftKn
        this.accDraftSamples++
      }
    }

    // Turns: accumulate |headingDelta|; 90° = 1 turn
    if (this.accLastHeading !== null) {
      let dHeading = vo.headingRad - this.accLastHeading
      // Wrap to [-π, π]
      while (dHeading > Math.PI) dHeading -= 2 * Math.PI
      while (dHeading < -Math.PI) dHeading += 2 * Math.PI
      this.accHeadingAccum += Math.abs(dHeading)
      const fullTurns = Math.floor(this.accHeadingAccum / (Math.PI / 2))
      if (fullTurns > this.accTurns) {
        this.accTurns = fullTurns
      }
    }
    this.accLastHeading = vo.headingRad

    // Reversals: detect velocity sign change with 0.5s debounce
    const velSign = vo.speed > 0.1 ? 1 : vo.speed < -0.1 ? -1 : 0
    if (velSign !== 0 && this.accLastVelSign !== 0 && velSign !== this.accLastVelSign) {
      if (this.elapsedSimS - this.accLastReversalS >= 0.5) {
        this.accReversals++
        this.accLastReversalS = this.elapsedSimS
      }
    }
    if (velSign !== 0) this.accLastVelSign = velSign

    // Trajectory: adaptive-interval sample, cap at MAX_TRAJ_SAMPLES entries
    if (this.elapsedSimS - this.accLastTrajS >= this.trajSampleIntervalS) {
      if (this.accTrajectory.length >= MAX_TRAJ_SAMPLES) {
        // Compress by keeping even-indexed samples only, then double the interval
        this.accTrajectory = this.accTrajectory.filter((_, i) => i % 2 === 0)
        this.trajSampleIntervalS *= 2
      }
      this.accTrajectory.push({
        t: this.elapsedSimS,
        x: vo.x,
        y: vo.z,
        heading: vo.headingRad,
        engaged: this.implementSystem.isActive(),
      })
      this.accLastTrajS = this.elapsedSimS
    }

    // LANES: implement OFF->ON transition
    const __implOn = this.implementSystem.isActive()
    if (!this.accPrevImplementOn && __implOn) {
      this.accLanes++
    }
    this.accPrevImplementOn = __implOn

    // DEVIATION (CTE): perpendicular distance from RDDF path
    const __cte = this.computeCTE(vo.x, vo.z)
    if (__cte > 0) {
      this.accSumCteSq += __cte * __cte
      this.accCteSamples++
    }

    // 5. Soil system at 10Hz
    if (now - this.lastSoilUpdate >= 100) {
      this.lastSoilUpdate = now

      const cosH = Math.cos(vo.headingRad), sinH = Math.sin(vo.headingRad)
      const hw = TRACTOR_CONFIG.wheelbaseM / 2
      const frontHalfTrack = TRACTOR_CONFIG.frontTrackM / 2
      const rearHalfTrack = TRACTOR_CONFIG.rearTrackM / 2
      const implementRearLoadN = this.implementSystem.isActive() ? TRACTOR_CONFIG.implementRearLoadKg * 9.81 : 0
      const baseFrontLoadN = TRACTOR_CONFIG.frontAxleKg * 9.81
      const baseRearLoadN = TRACTOR_CONFIG.rearAxleKg * 9.81 + implementRearLoadN
      const totalLoadN = baseFrontLoadN + baseRearLoadN
      const loadTransferN = (TRACTOR_CONFIG.massKg * vo.accelX * TRACTOR_CONFIG.cgHeightM) / TRACTOR_CONFIG.wheelbaseM
      const frontLoadN = clamp(baseFrontLoadN - loadTransferN, totalLoadN * 0.2, totalLoadN * 0.75)
      const rearLoadN = totalLoadN - frontLoadN
      const wheels = [
        { dx: sinH * hw - cosH * frontHalfTrack, dz: cosH * hw + sinH * frontHalfTrack, normalForce: frontLoadN / 2 },
        { dx: sinH * hw + cosH * frontHalfTrack, dz: cosH * hw - sinH * frontHalfTrack, normalForce: frontLoadN / 2 },
        { dx: -sinH * hw - cosH * rearHalfTrack, dz: -cosH * hw + sinH * rearHalfTrack, normalForce: rearLoadN / 2 },
        { dx: -sinH * hw + cosH * rearHalfTrack, dz: -cosH * hw - sinH * rearHalfTrack, normalForce: rearLoadN / 2 },
      ]
      const contacts: WheelContact[] = wheels.map(w => ({
        worldPosition: { x: vo.x + w.dx, y: frameY, z: vo.z + w.dz },
        normal: { x: 0, y: 1, z: 0 },
        depth: TRACTOR_CONFIG.wheelContactDepthM,
        normalForce: w.normalForce,
      }))
      const wheelPatches = this.currentSoil.deformable ? this.soilSystem.update(contacts) : []

      // Apply terrain tilt to vehicle dynamics at 10Hz (matches legacy baseline).
      this.vehicle.setTerrainTilt(framePitch, frameRoll)

      // Ridge detection + progressive compaction at vehicle center
      const centerGrid = this.soilGrid.worldToGrid(vo.x, vo.z)
      if (centerGrid) {
        const baseH = this.soilGrid.getBaseHeight(centerGrid.col, centerGrid.row)
        const isRidge = Math.abs(baseH) > 0.1
        if (isRidge) {
          this.vehicle.setExtraRollingResistance(this.currentSoil.rollingResistance * 0.25)
          this.soilGrid.applyDeformation(centerGrid.col, centerGrid.row, -0.02, 0.05)
        } else {
          this.vehicle.setExtraRollingResistance(0)
        }
      } else {
        this.vehicle.setExtraRollingResistance(0)
      }

      // Implement system update
      let implPatches: TerrainPatch[] = []
      if (this.implementSystem.isActive() && this.currentSoil.deformable) {
        this.soilSystem.clearDirty()  // clear wheel dirty cells
        const implCells = this.implementSystem.update(vo.x, vo.z, vo.headingRad, vo.speed)
        if (implCells.length > 0) {
          this.soilSystem.addDirtyCells(implCells)
          implPatches = this.soilSystem.buildPatches()
          this.onTillageCells?.(implCells, this.soilGrid.cols)
        }
      }

      // Wheel untill: compact tire strips break up previously tilled soil
      // contacts order: [FL, FR, RL, RR] (indices 0,1=front, 2,3=rear)
      let wheelUntillPatches: TerrainPatch[] = []
      if (this.prevWheelTickSeen) {
        const wheelPositions: WheelUntillInput[] = contacts.map((c, i) => ({
          x: c.worldPosition.x,
          z: c.worldPosition.z,
          widthM: i < 2
            ? TRACTOR_CONFIG.frontWheelWidthM
            : TRACTOR_CONFIG.rearWheelWidthM,
        }))
        const wheelUntillCells = applyWheelUntill(this.soilGrid, wheelPositions)
        if (wheelUntillCells.length > 0) {
          this.soilSystem.addDirtyCells(wheelUntillCells)
          wheelUntillPatches = this.soilSystem.buildPatches()
          this.onTillageCells?.(
            wheelUntillCells.map(c => ({ ...c, depth: 0 })),
            this.soilGrid.cols,
          )
        }
      }
      this.prevWheelTickSeen = true

      // Draft force feedback into vehicle longitudinal dynamics
      this.vehicle.setDraftForce(this.implementSystem.getLastDraftForceN())

      // Soil -> vehicle friction feedback (cell under vehicle center)
      const vehicleCell = this.soilGrid.getCell(vo.x, vo.z)
      const baseFriction = getSoilProperties(this.soilTypeName).friction
      if (vehicleCell) {
        const mu = baseFriction
          * (1 + vehicleCell.compaction * 0.1)
          * (vehicleCell.tilled ? 0.85 : 1.0)
        this.vehicle.setFrictionCoeff(mu)
      } else {
        this.vehicle.setFrictionCoeff(baseFriction)
      }

      const allPatches = [...wheelPatches, ...implPatches, ...wheelUntillPatches]
      if (allPatches.length > 0) {
        this.onTerrainDirty?.(allPatches)
      }
    }

    // 6. Callback + next frame
    if (this.onStateUpdate) {
      this.onStateUpdate(state)
    }

    // Schedule next frame: fixed-step accelerated mode uses setTimeout(0) for determinism,
    // normal mode (timeScale=1 or non-deterministic) uses requestAnimationFrame
    if (useFixedStep) {
      this.timeoutId = setTimeout(this.tick, 0)
    } else {
      this.animFrameId = requestAnimationFrame(this.tick)
    }
  }

  private sampleWheelSoilState(vehicleX: number, vehicleZ: number, headingRad: number): WheelSoilState[] {
    const cosH = Math.cos(headingRad)
    const sinH = Math.sin(headingRad)
    const hw = TRACTOR_CONFIG.wheelbaseM / 2
    const frontHalfTrack = TRACTOR_CONFIG.frontTrackM / 2
    const rearHalfTrack = TRACTOR_CONFIG.rearTrackM / 2
    const wheels = [
      { dx: sinH * hw - cosH * frontHalfTrack, dz: cosH * hw + sinH * frontHalfTrack },
      { dx: sinH * hw + cosH * frontHalfTrack, dz: cosH * hw - sinH * frontHalfTrack },
      { dx: -sinH * hw - cosH * rearHalfTrack, dz: -cosH * hw + sinH * rearHalfTrack },
      { dx: -sinH * hw + cosH * rearHalfTrack, dz: -cosH * hw - sinH * rearHalfTrack },
    ]
    return wheels.map(w => this.soilGrid.getSoilSampleAtWorld(vehicleX + w.dx, vehicleZ + w.dz))
  }
}
