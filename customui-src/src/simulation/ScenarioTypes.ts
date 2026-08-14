export type ScenarioAction =
  | { type: 'engine'; value: boolean }
  | { type: 'throttle'; value: number }      // 0..1
  | { type: 'steer'; value: number }          // -1..1
  | { type: 'gear'; value: number }           // -1|0|1..
  | { type: 'inject_gps'; lat: number; lon: number }
  | { type: 'plow'; value: boolean }
  | { type: 'rotary'; value: boolean }

export interface ScenarioStep {
  t_ms: number
  action: ScenarioAction
}

export interface Scenario {
  id: string
  label: string
  map?: string                // default 'default'
  time_of_day?: number         // 0..24 hours
  duration_ms: number          // total run duration
  steps: ScenarioStep[]
}

function isPlainObject(v: unknown): v is Record<string, unknown> {
  return typeof v === 'object' && v !== null && !Array.isArray(v)
}

function validateAction(raw: unknown, ctx: string): ScenarioAction {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: action must be an object`)
  const t = raw.type
  switch (t) {
    case 'engine': {
      if (typeof raw.value !== 'boolean') throw new Error(`${ctx}: engine.value must be boolean`)
      return { type: 'engine', value: raw.value }
    }
    case 'throttle': {
      if (typeof raw.value !== 'number') throw new Error(`${ctx}: throttle.value must be number`)
      if (raw.value < 0 || raw.value > 1) throw new Error(`${ctx}: throttle.value must be in [0,1]`)
      return { type: 'throttle', value: raw.value }
    }
    case 'steer': {
      if (typeof raw.value !== 'number') throw new Error(`${ctx}: steer.value must be number`)
      if (raw.value < -1 || raw.value > 1) throw new Error(`${ctx}: steer.value must be in [-1,1]`)
      return { type: 'steer', value: raw.value }
    }
    case 'gear': {
      if (typeof raw.value !== 'number') throw new Error(`${ctx}: gear.value must be number`)
      return { type: 'gear', value: raw.value }
    }
    case 'inject_gps': {
      if (typeof raw.lat !== 'number' || typeof raw.lon !== 'number') {
        throw new Error(`${ctx}: inject_gps requires numeric lat/lon`)
      }
      return { type: 'inject_gps', lat: raw.lat, lon: raw.lon }
    }
    case 'plow': {
      if (typeof raw.value !== 'boolean') throw new Error(`${ctx}: plow.value must be boolean`)
      return { type: 'plow', value: raw.value }
    }
    case 'rotary': {
      if (typeof raw.value !== 'boolean') throw new Error(`${ctx}: rotary.value must be boolean`)
      return { type: 'rotary', value: raw.value }
    }
    default:
      throw new Error(`${ctx}: unknown action type: ${String(t)}`)
  }
}

export function validateScenario(raw: unknown): Scenario {
  if (!isPlainObject(raw)) throw new Error('scenario must be an object')
  if (typeof raw.id !== 'string' || raw.id.length === 0) throw new Error('scenario.id must be a non-empty string')
  if (typeof raw.label !== 'string' || raw.label.length === 0) throw new Error('scenario.label must be a non-empty string')
  if (typeof raw.duration_ms !== 'number' || raw.duration_ms <= 0) {
    throw new Error('scenario.duration_ms must be a positive number')
  }
  if (!Array.isArray(raw.steps)) throw new Error('scenario.steps must be an array')

  const map = raw.map === undefined ? 'default' : raw.map
  if (typeof map !== 'string') throw new Error('scenario.map must be a string when provided')

  let timeOfDay: number | undefined
  if (raw.time_of_day !== undefined) {
    if (typeof raw.time_of_day !== 'number' || raw.time_of_day < 0 || raw.time_of_day > 24) {
      throw new Error('scenario.time_of_day must be a number in [0,24]')
    }
    timeOfDay = raw.time_of_day
  }

  const steps: ScenarioStep[] = raw.steps.map((s, i) => {
    if (!isPlainObject(s)) throw new Error(`scenario.steps[${i}] must be an object`)
    if (typeof s.t_ms !== 'number' || s.t_ms < 0) {
      throw new Error(`scenario.steps[${i}].t_ms must be a non-negative number`)
    }
    const action = validateAction(s.action, `scenario.steps[${i}].action`)
    return { t_ms: s.t_ms, action }
  })

  return {
    id: raw.id,
    label: raw.label,
    map,
    time_of_day: timeOfDay,
    duration_ms: raw.duration_ms,
    steps,
  }
}
