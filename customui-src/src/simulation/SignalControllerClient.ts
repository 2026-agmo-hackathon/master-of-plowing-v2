/**
 * Bidirectional WebSocket bridge to the backend on port 1456.
 *  - sim → backend: telemetry envelopes (≤10 Hz, throttled)
 *  - backend → sim: typed BackendCommand messages
 *
 * When a SessionClient is provided the class shares its socket instead of
 * opening its own WebSocket connection.
 */
import type { VehicleState } from './VehicleSystem'
import type { ProgressPayload } from './GameLoop'
import { isStandaloneMode } from './StandaloneMode'
import type { SessionClient } from '../protocol/SessionClient'

export type ImplementKind = 'plow' | 'rotary' | null

export interface TelemetryState {
  latitude: number
  longitude: number
  altitude: number
  heading: number       // degrees, 0=north, CW+
  speed: number         // m/s, signed (negative = reverse)
  rpm: number
  throttle: number
  brake: number
  steerAngle: number
  measuredSteerAngleDeg: number
  vehicleFrontWheelAngleDeg: number
  motorCmdRpm: number
  accelX: number
  accelY: number
  gyroZ: number
  roll: number
  pitch: number
  yaw: number
  gear: number
  running: boolean
}

export interface TelemetryEnvelope {
  kind: 'telemetry'
  ts: number
  state: TelemetryState
  implement: ImplementKind
}

export type BackendCommand =
  | { kind: 'steer_motor'; speedRpm: number }
  | { kind: 'throttle'; value: number }
  | { kind: 'brake'; value: number }
  | { kind: 'engine'; on: boolean }
  | { kind: 'gear'; gear: -1 | null | 1 | 2 | 3 | 4 }
  | { kind: 'implement'; type: ImplementKind }
  | { kind: 'inject_state'; latitude: number; longitude: number; altitude?: number; heading?: number }
  | { kind: 'reset' }
  | { kind: 'track_complete' }
  | { kind: 'pp_telemetry'; crossTrack: number; headingErr: number; lookaheadDist: number; targetIdx: number; speedErr: number; xLocal: number; zLocal: number; closestIdx: number; targetSpeedKmh: number; commandIdx: number; implementClosestIdx: number }

type CommandListener = (cmd: BackendCommand) => void

export class SignalControllerClient {
  private sessionClient: SessionClient | null = null
  private unsubscribe: (() => void) | null = null
  private commandListeners: Set<CommandListener> = new Set()
  private lastSendTime = 0
  // Base throttle period at 1x speed. The effective period is recomputed per
  // send via getSpeedMultiplier() so 8x sessions get proportionally more
  // telemetry frames and the backend PP loop sees fresh GPS at its faster rate.
  private static readonly BASE_SEND_INTERVAL_MS = 100
  private static readonly MIN_SEND_INTERVAL_MS = 5
  private standalone: boolean
  public onSend?: (envelope: TelemetryEnvelope) => void
  private getImplement: () => ImplementKind = () => null
  private getSpeedMultiplier: () => number = () => 1

  constructor(options?: { standalone?: boolean; sessionClient?: SessionClient; url?: string })
  constructor(url?: string)
  constructor(optionsOrUrl?: string | { standalone?: boolean; sessionClient?: SessionClient; url?: string }) {
    let standalone: boolean | undefined
    let sessionClient: SessionClient | undefined

    if (typeof optionsOrUrl === 'string') {
      // url-only form — no sessionClient
    } else if (optionsOrUrl && typeof optionsOrUrl === 'object') {
      standalone = optionsOrUrl.standalone
      sessionClient = optionsOrUrl.sessionClient
    }

    this.standalone = standalone ?? isStandaloneMode()
    this.sessionClient = sessionClient ?? null
  }

  start(): void {
    if (this.standalone) {
      return
    }
    if (this.sessionClient) {
      this.unsubscribe = this.sessionClient.onRawMessage((data) => this.handleRawMessage(data))
      return
    }
  }

  stop(): void {
    if (this.unsubscribe) {
      this.unsubscribe()
      this.unsubscribe = null
    }
  }

  setImplementProvider(getImplement: () => ImplementKind): void {
    this.getImplement = getImplement
  }

  setSpeedMultiplierProvider(getSpeedMultiplier: () => number): void {
    this.getSpeedMultiplier = getSpeedMultiplier
  }

  onCommand(listener: CommandListener): () => void {
    this.commandListeners.add(listener)
    return () => { this.commandListeners.delete(listener) }
  }

  sendVehicleState(state: VehicleState): void {
    const envelope: TelemetryEnvelope = {
      kind: 'telemetry',
      ts: Date.now(),
      state: {
        latitude: state.latitude,
        longitude: state.longitude,
        altitude: state.altitude,
        heading: state.heading,
        speed: state.speed,
        rpm: state.rpm,
        throttle: state.throttle,
        brake: state.brake,
        steerAngle: state.steerAngle,
        measuredSteerAngleDeg: state.measuredSteerAngleDeg,
        vehicleFrontWheelAngleDeg: state.vehicleFrontWheelAngleDeg,
        motorCmdRpm: state.motorCmdRpm,
        accelX: state.accel_x,
        accelY: state.accel_y,
        gyroZ: state.gyro_z,
        roll: state.roll,
        pitch: state.pitch,
        yaw: state.yaw,
        gear: state.gear,
        running: state.running,
      },
      implement: this.getImplement(),
    }

    if (this.standalone) {
      this.onSend?.(envelope)
      return
    }
    if (!this.sessionClient?.isConnected()) return
    if (!state.running) return

    const now = Date.now()
    const mult = Math.max(1, this.getSpeedMultiplier() || 1)
    const intervalMs = Math.max(
      SignalControllerClient.MIN_SEND_INTERVAL_MS,
      SignalControllerClient.BASE_SEND_INTERVAL_MS / mult,
    )
    if (now - this.lastSendTime < intervalMs) return
    this.lastSendTime = now

    this.sessionClient.sendEnvelope(envelope)
  }

  sendProgress(payload: ProgressPayload): void {
    if (!this.sessionClient?.isConnected()) return
    const msg = { kind: 'progress', ts: Date.now(), ...payload }
    this.sessionClient.sendEnvelope(msg)
  }

  isConnected(): boolean {
    if (this.standalone) return true
    return this.sessionClient?.isConnected() ?? false
  }

  private handleRawMessage(data: unknown): void {
    const cmd = parseBackendCommandFromObject(data)
    if (!cmd) return
    for (const listener of this.commandListeners) listener(cmd)
  }
}

function parseBackendCommandFromObject(raw: unknown): BackendCommand | null {
  if (!raw || typeof raw !== 'object') return null
  const obj = raw as Record<string, unknown>
  switch (obj.kind) {
    case 'steer_motor':
      return typeof obj.speedRpm === 'number' ? { kind: 'steer_motor', speedRpm: obj.speedRpm } : null
    case 'throttle':
      return typeof obj.value === 'number' ? { kind: 'throttle', value: obj.value } : null
    case 'brake':
      return typeof obj.value === 'number' ? { kind: 'brake', value: obj.value } : null
    case 'engine':
      return typeof obj.on === 'boolean' ? { kind: 'engine', on: obj.on } : null
    case 'gear': {
      const g = obj.gear
      if (g === null || g === -1 || g === 1 || g === 2 || g === 3 || g === 4) {
        return { kind: 'gear', gear: g as -1 | null | 1 | 2 | 3 | 4 }
      }
      return null
    }
    case 'implement': {
      const t = obj.type
      if (t === null || t === 'plow' || t === 'rotary') {
        return { kind: 'implement', type: t as ImplementKind }
      }
      return null
    }
    case 'inject_state': {
      if (typeof obj.latitude !== 'number' || typeof obj.longitude !== 'number') return null
      const cmd: BackendCommand = {
        kind: 'inject_state',
        latitude: obj.latitude,
        longitude: obj.longitude,
      }
      if (typeof obj.altitude === 'number') cmd.altitude = obj.altitude
      if (typeof obj.heading === 'number') cmd.heading = obj.heading
      return cmd
    }
    case 'reset':
      return { kind: 'reset' }
    case 'track_complete':
      return { kind: 'track_complete' }
    case 'pp_telemetry':
      if (typeof obj.crossTrack !== 'number' || typeof obj.headingErr !== 'number'
          || typeof obj.lookaheadDist !== 'number' || typeof obj.targetIdx !== 'number'
          || typeof obj.speedErr !== 'number'
          || typeof obj.xLocal !== 'number' || typeof obj.zLocal !== 'number'
          || typeof obj.closestIdx !== 'number' || typeof obj.targetSpeedKmh !== 'number'
          || typeof obj.commandIdx !== 'number' || typeof obj.implementClosestIdx !== 'number') return null;
      return { kind: 'pp_telemetry',
        crossTrack: obj.crossTrack, headingErr: obj.headingErr,
        lookaheadDist: obj.lookaheadDist, targetIdx: obj.targetIdx,
        speedErr: obj.speedErr, xLocal: obj.xLocal, zLocal: obj.zLocal,
        closestIdx: obj.closestIdx, targetSpeedKmh: obj.targetSpeedKmh,
        commandIdx: obj.commandIdx, implementClosestIdx: obj.implementClosestIdx };
    default:
      return null
  }
}
