import { describe, expect, it } from 'vitest'
import { validateScenario } from '../ScenarioTypes'

describe('validateScenario', () => {
  it('accepts a minimal valid scenario', () => {
    const s = validateScenario({
      id: 'demo',
      label: 'Demo',
      duration_ms: 1000,
      steps: [
        { t_ms: 0, action: { type: 'engine', value: true } },
        { t_ms: 500, action: { type: 'throttle', value: 0.5 } },
      ],
    })
    expect(s.id).toBe('demo')
    expect(s.map).toBe('default')
    expect(s.steps).toHaveLength(2)
  })

  it('rejects throttle out of range', () => {
    expect(() =>
      validateScenario({
        id: 'bad',
        label: 'Bad',
        duration_ms: 1000,
        steps: [{ t_ms: 0, action: { type: 'throttle', value: 1.5 } }],
      })
    ).toThrow(/throttle/)
  })

  it('rejects unknown action type', () => {
    expect(() =>
      validateScenario({
        id: 'bad',
        label: 'Bad',
        duration_ms: 1000,
        steps: [{ t_ms: 0, action: { type: 'boom', value: 1 } }],
      })
    ).toThrow(/unknown action type/)
  })

  it('rejects negative duration', () => {
    expect(() =>
      validateScenario({
        id: 'bad',
        label: 'Bad',
        duration_ms: -1,
        steps: [],
      })
    ).toThrow(/duration_ms/)
  })
})
