import type { Scenario, ScenarioStep } from './ScenarioTypes'

export interface ScenarioHandlers {
  onEngine: (on: boolean) => void
  onThrottle: (value: number) => void
  onSteer: (value: number) => void
  onGear: (value: number) => void
  onInjectGps: (lat: number, lon: number) => void
  onPlow?: (on: boolean) => void
  onRotary?: (on: boolean) => void
  onComplete?: () => void
}

export class ScenarioRunner {
  private scenario: Scenario
  private handlers: ScenarioHandlers
  private startTime: number | null = null
  private executed = new Set<number>()
  private rafId: number | null = null
  private stopped = false

  constructor(scenario: Scenario, handlers: ScenarioHandlers) {
    this.scenario = scenario
    this.handlers = handlers
  }

  start(): void {
    this.stopped = false
    this.executed.clear()
    this.startTime = performance.now()
    this.tick()
  }

  stop(): void {
    if (this.stopped) return
    this.stopped = true
    if (this.rafId !== null) {
      cancelAnimationFrame(this.rafId)
      this.rafId = null
    }
    this.handlers.onEngine(false)
  }

  private runStep(step: ScenarioStep): void {
    const a = step.action
    switch (a.type) {
      case 'engine':
        this.handlers.onEngine(a.value)
        break
      case 'throttle':
        this.handlers.onThrottle(a.value)
        break
      case 'steer':
        this.handlers.onSteer(a.value)
        break
      case 'gear':
        this.handlers.onGear(a.value)
        break
      case 'inject_gps':
        this.handlers.onInjectGps(a.lat, a.lon)
        break
      case 'plow':
        this.handlers.onPlow?.(a.value)
        break
      case 'rotary':
        this.handlers.onRotary?.(a.value)
        break
    }
  }

  private tick = (): void => {
    if (this.stopped || this.startTime === null) return
    const elapsed = performance.now() - this.startTime

    for (let i = 0; i < this.scenario.steps.length; i++) {
      if (this.executed.has(i)) continue
      const step = this.scenario.steps[i]
      if (step.t_ms <= elapsed) {
        this.executed.add(i)
        this.runStep(step)
      }
    }

    if (elapsed >= this.scenario.duration_ms) {
      this.stopped = true
      if (this.rafId !== null) {
        cancelAnimationFrame(this.rafId)
        this.rafId = null
      }
      this.handlers.onEngine(false)
      this.handlers.onComplete?.()
      return
    }

    this.rafId = requestAnimationFrame(this.tick)
  }
}
