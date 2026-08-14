// WebSocket message type definitions for UI ↔ C++ communication.
// This module is type-only (no runtime classes/functions) except for preset constants.

// ---------------------------------------------------------------------------
// Shared sub-types
// ---------------------------------------------------------------------------

export interface MapListEntry {
  id: number
  label: string
  description?: string
  area_m2?: number
  origin?: { lat: number; lon: number }
  start_lat?: number
  start_lon?: number
  start_heading_deg?: number
  has_start?: boolean
  polygonLL?: [number, number][]   // raw [lat, lon] pairs from server (single ring per map)
  rddfAvailable?: boolean
  kind?: string
  totalAreaM2?: number
}

export interface TransformPayload {
  x: number
  z: number
  heading: number
  speed: number
  altitude: number
}

export interface ControlPayload {
  engineOn: boolean
  plowDown: boolean
  throttle: number
}

export interface StatsPayload {
  progress: number
  elapsed_s: number
  cross_track_error: number
  heading_drift: number
  area_done_m2: number
  area_total_m2: number
  tracking_cr_unit: number
  overhead_scale_km: number
  outsideField?: boolean
}

export interface TillageCellPatch {
  idx: number
  depth: number
}

export interface RddfWaypointPayload {
  lat: number
  lon: number
  speed: number
  implementFlag: number
}

// ---------------------------------------------------------------------------
// UI → C++ (outgoing / request messages)
// ---------------------------------------------------------------------------

export interface GetSessionStateMsg {
  kind: 'get_session_state'
}

export interface SetTractorPresetMsg {
  kind: 'set_tractor_preset'
  value: string
}

export interface SetImplementPresetMsg {
  kind: 'set_implement_preset'
  value: string
}

export interface SetSelectedMapMsg {
  kind: 'set_selected_map'
  value: number
}

export interface StartSessionMsg {
  kind: 'start_session'
  teamName: string
  implementWidth: number
}

export interface ResetSessionMsg {
  kind: 'reset_session'
}

export interface UpdateTransformMsg {
  kind: 'update_transform'
  x: number
  z: number
  heading: number
  speed: number
  altitude: number
  engineOn: boolean
  plowDown: boolean
  throttle: number
  progress: number
  elapsed_s: number
  outsideField?: boolean
}

export interface UpdateTillageMsg {
  kind: 'update_tillage'
  cells: TillageCellPatch[]
}

export interface RequestMapListMsg {
  kind: 'request_map_list'
}


export interface GetRddfPathMsg {
  kind: 'get_rddf_path'
}

export interface SetSpeedMultiplierMsg {
  kind: 'set_speed_multiplier'
  value: 1 | 2 | 4 | 8
}

export interface ProgressMirrorMsg {
  kind: 'progress_mirror'
  finalCoverage: number
  plowedM2: number
  totalM2: number
  avgDraftKn: number
  lostAreaM2: number
  elapsedTo95S: number | null
  bestLane?: { index: number; ratio: number }
}

export interface GetRunSummaryMsg {
  kind: 'get_run_summary'
  runNumber: number
}

// ---------------------------------------------------------------------------
// C++ → UI (incoming / response + broadcast messages)
// ---------------------------------------------------------------------------

export interface SessionStateMsg {
  kind: 'session_state'
  tractorPreset: string
  implementPreset: string
  selectedMapId: number
  started: boolean
  mission_name: string
  run_number: number
  started_at: string
  coverage_goal: number
  speed_multiplier: number
}

export interface MapListMsg {
  kind: 'map_list'
  maps: MapListEntry[]
}


export interface SessionStateChangedMsg {
  kind: 'session_state_changed'
  tractorPreset: string
  implementPreset: string
  selectedMapId: number
  started: boolean
  mission_name: string
  run_number: number
  started_at: string
  coverage_goal: number
  speed_multiplier: number
}

export interface SimulationSnapshotPatchedMsg {
  kind: 'simulation_snapshot_patched'
  transform?: TransformPayload
  control?: ControlPayload
  stats?: StatsPayload
  tillagePatch?: TillageCellPatch[]
}

export interface RddfPathMsg {
  kind: 'rddf_path'
  originLat?: number
  originLon?: number
  waypoints: RddfWaypointPayload[]
}

export interface RunSummaryMessage {
  kind: 'run_summary'
  runNumber: number
  fieldName: string
  planName: string
  startedAt: string
  elapsedS: number
  distanceM: number
  turns: number
  reversals: number
  lanes: number
  headlandS: number
  deviationM: number
  avgSpeedKmh: number
  finalCoverage: number
  plowedM2: number
  totalM2: number
  avgDraftKn: number
  lostAreaM2: number
  elapsedTo95S: number | null
  bestLane?: { index: number; ratio: number }
  finalized: boolean
  notFound?: boolean
  penaltyS?: number
  finalTimeS?: number
}

// RDDF Staging messages (incoming from C++)
export interface RddfPendingMsg {
  kind: 'rddf_pending'
  mapId: number
  receivedAt: string
}

export interface RddfAppliedMsg {
  kind: 'rddf_applied'
  mapId: number
}

export interface RddfCancelledMsg {
  kind: 'rddf_cancelled'
  mapId: number
}

export interface RddfErrorMsg {
  kind: 'rddf_error'
  mapId?: number
  reason: string
}

export interface MapListErrorMsg {
  kind: 'map_list_error';
  reason: string;
  code?: 'timeout' | 'http_error' | 'parse_error';
}


export interface LeaderboardErrorMsg {
  kind: 'leaderboard_error';
  op: 'start' | 'finished' | 'dnf';
  status: number;
  reason: string;
  runId: string;
  code?: 'timeout' | 'http_error' | 'parse_error';
}

export interface StartSessionErrorMsg {
  kind: 'start_session_error';
  reason: string;
  code?: 'no_rddf' | 'invalid_rddf' | 'too_few_points';
}

// RDDF Staging messages (outgoing to C++)
export interface RddfApplyMsg {
  kind: 'rddf_apply'
  mapId: number
}

export interface RddfCancelMsg {
  kind: 'rddf_cancel'
  mapId: number
}

// ---------------------------------------------------------------------------
// Union types
// ---------------------------------------------------------------------------

export type SessionMessageKind =
  | 'get_session_state'
  | 'set_tractor_preset'
  | 'set_implement_preset'
  | 'set_selected_map'
  | 'start_session'
  | 'reset_session'
  | 'update_transform'
  | 'update_tillage'
  | 'request_map_list'
  | 'get_rddf_path'
  | 'set_speed_multiplier'
  | 'progress_mirror'
  | 'get_run_summary'
  | 'session_state'
  | 'map_list'
  | 'session_state_changed'
  | 'simulation_snapshot_patched'
  | 'rddf_path'
  | 'run_summary'
  | 'rddf_pending'
  | 'rddf_applied'
  | 'rddf_cancelled'
  | 'rddf_error'
  | 'rddf_apply'
  | 'rddf_cancel'
  | 'map_list_error'
  | 'leaderboard_error'
  | 'start_session_error'

export type OutgoingMessage =
  | GetSessionStateMsg
  | SetTractorPresetMsg
  | SetImplementPresetMsg
  | SetSelectedMapMsg
  | StartSessionMsg
  | ResetSessionMsg
  | UpdateTransformMsg
  | UpdateTillageMsg
  | RequestMapListMsg
  | GetRddfPathMsg
  | SetSpeedMultiplierMsg
  | ProgressMirrorMsg
  | GetRunSummaryMsg
  | RddfApplyMsg
  | RddfCancelMsg

export type IncomingMessage =
  | SessionStateMsg
  | MapListMsg
  | SessionStateChangedMsg
  | SimulationSnapshotPatchedMsg
  | RddfPathMsg
  | RunSummaryMessage
  | RddfPendingMsg
  | RddfAppliedMsg
  | RddfCancelledMsg
  | RddfErrorMsg
  | MapListErrorMsg
  | LeaderboardErrorMsg
  | StartSessionErrorMsg

// ---------------------------------------------------------------------------
// Preset constants (reused in TODO-5B)
// ---------------------------------------------------------------------------

export const TRACTOR_PRESETS = {
  '50hp':  { hp: 50,  scale: 1.00, lengthRefM: 3.6 },
  '120hp': { hp: 120, scale: 1.28, lengthRefM: 4.6 },
  '200hp': { hp: 200, scale: 1.53, lengthRefM: 5.5 },
} as const

export type TractorPresetKey = keyof typeof TRACTOR_PRESETS

export const IMPLEMENT_PRESETS = {
  'plow_2.3m':   { type: 'plow' as const,   width: 2.3, depth: 0.25, mass: 150 },
  'plow_3.5m':   { type: 'plow' as const,   width: 3.5, depth: 0.25, mass: 300 },
  'plow_5.5m':   { type: 'plow' as const,   width: 5.5, depth: 0.25, mass: 550 },
  'rotary_2.3m': { type: 'rotary' as const, width: 2.3, depth: 0.15, mass: 250 },
  'rotary_3.5m': { type: 'rotary' as const, width: 3.5, depth: 0.15, mass: 500 },
  'rotary_5.5m': { type: 'rotary' as const, width: 5.5, depth: 0.15, mass: 900 },
} as const

export type ImplementPresetKey = keyof typeof IMPLEMENT_PRESETS
