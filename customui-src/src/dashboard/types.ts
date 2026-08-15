// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 백엔드 pp_telemetry 프레임의 타입. 모든 필드가 optional 인 것이 계약이다 —
// 「값 없음」과 0 을 구분해야 하므로 UI 는 undefined 를 절대 0 으로 메우지 않는다.
// JSON 키는 백엔드 그대로 둔다(ltd/hde). 팀 용어(LTD/HDE)는
// 표기 레이어에서만 바꾼다.
//
// Types for the backend pp_telemetry frame. Every field is optional by contract:
// "absent" and 0 must stay distinguishable, so the UI never coerces undefined
// to 0. JSON keys stay as the backend sends them (ltd/hde); the
// team vocabulary (LTD/HDE) is applied only at the label layer.
// └────────────────────────────────────────────────────────────────────────────┘

export interface PpTelemetry {
  kind: 'pp_telemetry'
  elapsedS?: number
  dtMs?: number
  machineTimeScale?: number
  machineClockCalibrated?: boolean
  machineClockValid?: boolean
  machineClockSourceLoss?: boolean
  machineClockRegression?: boolean
  machineClockImplausibleJump?: boolean
  machineClockStatus?: 'calibrating' | 'valid' | 'source_loss' | 'regression' | 'implausible_jump'
  poseSampleId?: number
  poseTimestampDeltaMs?: number
  trackingPeriodMs?: number
  processTimerPeriodMs?: number
  /** LTD · Lateral Deviation (m) — 키 이름은 백엔드 계약 / key name is backend contract */
  ltd?: number
  /** HDE · Heading Error (rad) */
  hde?: number
  speedErr?: number
  targetSpeedKmh?: number
  targetIdx?: number
  closestIdx?: number
  commandIdx?: number
  steerTargetDeg?: number
  steerMeasDeg?: number
  steerErrDeg?: number
  steerP?: number
  steerI?: number
  steerD?: number
  /** 모터 지령, ±600 / motor command, ±600 */
  steerCmd?: number
  steerMeasValid?: boolean
  steerBlocked?: boolean
  speedMps?: number
  /** 0=북, 시계방향 + / 0=north, clockwise positive */
  headingDeg?: number
  enuX?: number
  enuZ?: number
  rollDeg?: number
  pitchDeg?: number
  attitudeValid?: boolean
  yawRateDegS?: number
  yawRateValid?: boolean
  accelX?: number
  accelY?: number
  encRpm?: number
  encDeg?: number
  accV1?: number
  accV2?: number
  controlAuthority?: boolean
  /** 실제 송신 MachineOutputs.authority / actual commanded output authority */
  outputAuthority?: boolean
  /** 0모름 / 1 R / 2 N / 3 F */
  fnrState?: number
  /** 0모름 / 1 Low / 2 Mid / 3 High */
  shiftState?: number
  hitchPct?: number
  /** NMEA GGA: 4 RTK Fixed, 5 RTK Float, 2 DGPS, 1 Single, 0 No Fix */
  gpsFix?: number
  gpsSats?: number
  gpsHdop?: number
  motorFaultBits?: number

  /** 주행 상태 — 10Hz 텔레메트리로도 실려 와 즉시 반응한다. */
  runState?: RunState
  readyToStart?: boolean
  waypointCount?: number
  lookaheadDist?: number
  xLocal?: number
  zLocal?: number
  implementClosestIdx?: number
  vehicleCmdIdx?: number
  cfTerm?: number
  hdeTerm?: number
  ltdTerm?: number
  ppTerm?: number
  yawDampTerm?: number
  longP?: number
  longI?: number
  longD?: number
  longTargetMps?: number
  longReversing?: number | boolean
  longRequestedMps?: number
  // 개도 변조기. 이 기계의 개도는 릴레이라(0보다 크기만 하면 그 단의 속도),
  // 단이 목표보다 빠르면 개도를 끊었다 열었다 해서 평균을 맞춥니다.
  // longDebtM 은 "초과속으로 앞서 나간 거리"이고, 스위칭 문턱을 미는 값입니다.
  // The opening modulator. The opening on this machine is a relay — anything
  // above zero gives the stage's speed — so when the stage is faster than the
  // target the opening is pulsed to average it out. longDebtM is the distance
  // gained by overspeeding, which is what pushes the switching threshold.
  longThrottleGated?: number | boolean
  longDebtM?: number
  longCoastDecel?: number
  longBelowSlowestStage?: number | boolean
  // 단 선택 폐루프. longStageMeasured 가 거짓이면 longStageKmh 는 아직 실측이
  // 아니라 사전값이므로 0 으로 옵니다.
  // Closed-loop stage selection. A false longStageMeasured means longStageKmh is
  // still a prior rather than a measurement, and arrives as 0.
  longStageKmh?: number
  longStageMeasured?: number | boolean
  longFloorKmh?: number
}

export interface TrackComplete {
  kind: 'track_complete'
  elapsedS?: number
}

export interface CatalogItem {
  id: string
  label: string
  widthM?: number
  wheelbaseM?: number
  maxWheelAngleRad?: number
  steeringRatio?: number
  maxForwardGear?: number
}

export interface OrchestrationState {
  kind: 'sim_setup_state' | 'orchestration_state'
  phase: 'idle' | 'refreshing' | 'applying_setup' | 'starting' | 'running' | 'finishing' | 'failed'
  finalization?: 'idle' | 'in_progress' | 'completed' | 'retryable_error'
  postRun?: 'none'|'awaiting_action'|'capture_in_progress'|'capture_durable'|'submitted'|'reset_retryable'|'reset_cleanup_preparing'|'reset_cleanup_pending'
  runGeneration?: number
  resetGeneration?: number
  busy: boolean
  error?: string
  teamName?: string
  setupApplying?: boolean
  setupGeneration?: number
  recordingId?: string
  recordingSha256?: string
  maps: CatalogItem[]
  tractors: CatalogItem[]
  implements: CatalogItem[]
  selection: {
    mapId?: string
    tractorId?: string
    implementId?: string
    stopped: boolean
    reactRunning?: boolean
    live: boolean
    snapshotAgeMs: number
    /** Local monotonic receipt time; added by useTelemetry, never sent by backend. */
    receivedAtMonoMs?: number
  }
  desiredSetup?: { mapId?:string; tractorId?:string; implementId?:string }
  confirmedSetup?: { mapId?:string; tractorId?:string; implementId?:string }
}

export interface LeaderboardState {
  kind: 'leaderboard_state'
  status: 'idle'|'capture_pending'|'capture_error'|'pending'|'submitting'|'submitted'|'invalid'|'failed_terminal'|'retry_wait'
  attempt: number
  backlog?: number
  runId?: string
  verdict?: string
  mapId?: string
  score?: number
  reason?: string
  error?: string
  kept?: boolean
  applied?: boolean
  verified?: boolean
  retryAllowed?: boolean
}

export interface UnsubmittedRecord {
  runId:string
  teamName:string
  createdAt:string
  bytes:number
  csvAvailable:boolean
  error?:string
  outboxStatus?:LeaderboardState['status']
  outboxError?:string
}

/** 백엔드 tuning_state 프레임 — 튜닝 채널의 유일한 하행 메시지.
 *  params 의 키는 web/TuningProtocol.cpp 의 PARAM_FIELDS 와 동일한 계약이다.
 *  Backend tuning_state frame, the tuning channel's only downstream message.
 *  Param keys are the same contract as PARAM_FIELDS in web/TuningProtocol.cpp. */
/** 주행 상태. 백엔드 AppMain::RunState 와 이름이 1:1 대응한다. */
export type RunState = 'idle' | 'running' | 'paused' | 'finished'

export interface TuningState {
  kind: 'tuning_state'
  tracker?: string
  available?: string[]
  params?: Record<string, number>
  /** 주행 중이면 파라미터·알고리즘 변경이 백엔드에서 거부된다. */
  runState?: RunState
  paramsLocked?: boolean
  readyToStart?: boolean
  waypointCount?: number
}

export const RUN_STATE_LABEL: Record<RunState, string> = {
  idle: '대기',
  running: '주행 중',
  paused: '일시정지',
  finished: '완주',
}

/** bit0 부터 순서대로 / in bit order from bit0 */
export const MOTOR_FAULT_NAMES = [
  'Hall 센서',
  'CAN 단절',
  '모터 스톨',
  'Disabled',
  '과전압',
  'HW 보호',
  'EEPROM',
  '저전압',
  '과전류',
  '모드 오류',
] as const

/** bit3(Disabled)은 조향 지령 0 일 때 앱이 스스로 켜는 정상 상태라 차단성 판정에서 뺀다.
 *  bit3 (Disabled) turns on during normal operation (app disables the motor at
 *  zero steer command), so it is excluded from the blocking-fault mask. */
export const BLOCKING_FAULT_MASK = ~(1 << 3)

export function gpsFixLabel(fix: number | undefined): string {
  if (fix === undefined) return '—'
  switch (fix) {
    case 4: return 'RTK Fixed'
    case 5: return 'RTK Float'
    case 2: return 'DGPS'
    case 1: return 'Single'
    case 0: return 'No Fix'
    default: return `Fix ${fix}`
  }
}

export function fnrLabel(v: number | undefined): string {
  if (v === undefined) return '—'
  return ['모름', 'R', 'N', 'F'][v] ?? '—'
}

export function shiftLabel(v: number | undefined): string {
  if (v === undefined) return '—'
  return ['모름', 'Low', 'Mid', 'High'][v] ?? '—'
}
