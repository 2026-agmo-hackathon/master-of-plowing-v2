import { describe, expect, it } from 'vitest'
import { DynamicVehicle } from '../DynamicVehicle'
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

describe('slip clamp: multiple equilibria blocked', () => {
  it('scenario 4: draft 3000N — initial wheelOmega high (1.5) vs low (0) converge to same equilibrium', () => {
    // Case (a): wheelOmega starts high (simulates "wheel spinning freely" equilibrium)
    const vA = new DynamicVehicle()
    vA.setDraftForce(3000)
    ;(vA as any).wheelOmega = 1.5
    const outA = simulate(vA, { throttle: 1, manualGear: 1 }, 30)

    // Case (b): wheelOmega starts at 0 (normal startup)
    const vB = new DynamicVehicle()
    vB.setDraftForce(3000)
    const outB = simulate(vB, { throttle: 1, manualGear: 1 }, 30)

    // Both cases must converge to the same equilibrium (within 5% tolerance on vx)
    const vxDiff = Math.abs(outA.speed - outB.speed)
    const omegaDiff = Math.abs(outA.wheelOmega! - outB.wheelOmega!)

    expect(vxDiff).toBeLessThan(0.05)   // same vx equilibrium within 0.05 m/s
    expect(omegaDiff).toBeLessThan(0.05) // same wheelOmega equilibrium within 0.05 rad/s
  })
})
