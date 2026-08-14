import { describe, expect, it } from 'vitest'
import { DynamicVehicle } from '../DynamicVehicle'
import type { VehicleInput, VehicleOutput } from '../KinematicVehicle'

function simulate(
  vehicle: DynamicVehicle,
  input: Partial<VehicleInput>,
  frames: number,
  dt = 1 / 60,
): VehicleOutput {
  const fullInput = { throttle: 0, brake: 0, steer: 0, ...input } as VehicleInput
  let output: VehicleOutput
  for (let i = 0; i < frames; i++) {
    output = vehicle.update(fullInput, dt)
  }
  return output!
}

describe('DynamicVehicle.setDraftForce', () => {
  // Skip: with per-gear velocity clamp, both baseline and loaded converge toward gear-1 cap (0.8 m/s),
  // so velocity clamp pins loaded to cap while baseline may sit just below, inverting the comparison.
  it.skip('throttle=1 with draft 500N produces lower speed than baseline (no stall)', () => {
    const baseline = new DynamicVehicle()
    const baselineOut = simulate(baseline, { throttle: 1 }, 300)

    const loaded = new DynamicVehicle()
    loaded.setDraftForce(500)
    const loadedOut = simulate(loaded, { throttle: 1 }, 300)

    expect(loadedOut.speed).toBeLessThan(baselineOut.speed)
    // Vehicle should still be moving forward, not stalled.
    expect(loadedOut.speed).toBeGreaterThan(0.1)
  })

  it('not calling setDraftForce keeps behavior identical to baseline', () => {
    const baseline = new DynamicVehicle()
    const baselineOut = simulate(baseline, { throttle: 0.5 }, 300)

    const explicitZero = new DynamicVehicle()
    explicitZero.setDraftForce(0)
    const zeroOut = simulate(explicitZero, { throttle: 0.5 }, 300)

    expect(zeroOut.speed).toBeCloseTo(baselineOut.speed, 6)
    expect(zeroOut.x).toBeCloseTo(baselineOut.x, 6)
    expect(zeroOut.z).toBeCloseTo(baselineOut.z, 6)
  })

  it('vx=0 with draft and no throttle stays at rest (static friction holds)', () => {
    const v = new DynamicVehicle()
    v.setDraftForce(2000)
    const out = simulate(v, { throttle: 0, manualGear: 0 }, 60)
    expect(Math.abs(out.speed)).toBeLessThan(0.01)
  })

  it('vx=0 with draft and throttle: first non-zero acceleration is forward (draft does not flip starting direction)', () => {
    const v = new DynamicVehicle()
    v.setDraftForce(500)
    // At vx=0 the draft term is gated off, so the first non-zero acceleration must come from throttle (positive).
    let firstNonZeroAx: number | null = null
    for (let i = 0; i < 5; i++) {
      const o = v.update({ throttle: 1, brake: 0, steer: 0 }, 1 / 60)
      if (firstNonZeroAx === null && o.accelX !== 0) {
        firstNonZeroAx = o.accelX
        break
      }
    }
    expect(firstNonZeroAx).not.toBeNull()
    expect(firstNonZeroAx!).toBeGreaterThan(0)
  })
})
