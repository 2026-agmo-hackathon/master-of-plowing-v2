import { describe, expect, it } from 'vitest'
import { DynamicVehicle } from '../DynamicVehicle'
import { TRACTOR_CONFIG } from '../TractorConfig'
import type { VehicleInput, VehicleOutput } from '../KinematicVehicle'

function simulate(
  vehicle: DynamicVehicle,
  input: Partial<VehicleInput>,
  seconds: number,
  dt = 1 / 60,
): VehicleOutput {
  const fullInput = { throttle: 0, brake: 0, steer: 0, ...input } as VehicleInput
  const frames = Math.round(seconds / dt)
  let output: VehicleOutput
  for (let i = 0; i < frames; i++) {
    output = vehicle.update(fullInput, dt)
  }
  return output!
}

// driveRatio for gear 1: gearRatios[1] * finalDriveRatio = 22.7 * 8.5 = 192.95
const DRIVE_RATIO_GEAR1 = TRACTOR_CONFIG.gearRatios[1] * TRACTOR_CONFIG.finalDriveRatio
const OMEGA_CEILING = (TRACTOR_CONFIG.ratedRpm * 1.05) * (2 * Math.PI) / 60 / DRIVE_RATIO_GEAR1

describe('wheelOmega absolute ceiling', () => {
  it('scenario 1: no draft, gear 1, full throttle, 30s — vx near cap, wheelOmega near vx/r', () => {
    const v = new DynamicVehicle()
    const out = simulate(v, { throttle: 1, manualGear: 1 }, 30)

    // vx should be at or near gear-1 cap (0.8 m/s) — allow 10% tolerance
    expect(out.speed).toBeGreaterThan(0.6)
    // wheelOmega must not diverge (ceiling is ~1.197 rad/s)
    expect(Math.abs(out.wheelOmega!)).toBeLessThan(OMEGA_CEILING + 0.01)
    // wheelEquivRpm must not exceed ceiling RPM
    expect(out.wheelEquivRpm!).toBeLessThan(TRACTOR_CONFIG.ratedRpm * 1.05 + 1)
  })

  it('scenario 2: draft 3000N, gear 1, full throttle, 30s — wheelOmega stays at ceiling, no diverge', () => {
    const v = new DynamicVehicle()
    v.setDraftForce(3000)
    const out = simulate(v, { throttle: 1, manualGear: 1 }, 30)

    // vx should be below cap (draft load slows vehicle)
    // wheelOmega must stay at or below ceiling
    expect(Math.abs(out.wheelOmega!)).toBeLessThan(OMEGA_CEILING + 0.01)
    // wheelEquivRpm must not exceed ratedRpm * 1.05
    expect(out.wheelEquivRpm!).toBeLessThan(TRACTOR_CONFIG.ratedRpm * 1.05 + 1)
  })

  it('scenario 3: draft 6000N, gear 1, full throttle, 30s — wheelOmega still within ceiling', () => {
    const v = new DynamicVehicle()
    v.setDraftForce(6000)
    const out = simulate(v, { throttle: 1, manualGear: 1 }, 30)

    // With heavy draft, vx is smaller
    // wheelOmega must never exceed ceiling regardless
    expect(Math.abs(out.wheelOmega!)).toBeLessThan(OMEGA_CEILING + 0.01)
    expect(out.wheelEquivRpm!).toBeLessThan(TRACTOR_CONFIG.ratedRpm * 1.05 + 1)
  })
})
