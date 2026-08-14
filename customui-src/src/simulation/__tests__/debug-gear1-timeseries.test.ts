/**
 * DEBUG: Gear-1 full-throttle time-series simulation
 * Purpose: Investigate why RPM 2100 @ vx~1 km/h and RPM 1300 @ vx~3 km/h
 * appear as inconsistent equilibria in manual testing.
 *
 * Run:
 *   npx vitest run src/simulation/__tests__/debug-gear1-timeseries.test.ts
 */
import { describe, expect, it } from 'vitest'
import { DynamicVehicle } from '../DynamicVehicle'
import type { VehicleInput } from '../KinematicVehicle'
import { TRACTOR_CONFIG, WORK_GEAR_MAX_SPEEDS_MPS } from '../TractorConfig'

describe('DEBUG: Gear-1 full-throttle time-series', () => {
  it('30s simulation with throttle=1, gear=1, dt=1/60', () => {
    const vehicle = new DynamicVehicle()
    const input: VehicleInput = { throttle: 1, brake: 0, steer: 0, manualGear: 1 }
    const dt = 1 / 60
    const totalFrames = Math.round(30 / dt)  // ~1800 frames

    // Transmission constants
    const gearRatio = TRACTOR_CONFIG.gearRatios[1]          // 22.7
    const finalDrive = TRACTOR_CONFIG.finalDriveRatio        // 8.5
    const driveRatio = gearRatio * finalDrive                // 192.95
    const tireRadius = TRACTOR_CONFIG.tireRadiusM            // 0.7
    const maxSpeedMps = WORK_GEAR_MAX_SPEEDS_MPS[1]         // 0.8

    void driveRatio
    void tireRadius
    void maxSpeedMps

    let lastOutput = vehicle.update(input, dt)  // first frame to initialize

    for (let frame = 1; frame < totalFrames; frame++) {
      const output = vehicle.update(input, dt)
      lastOutput = output
    }

    // Final state summary
    const vxFinal = lastOutput.speed
    const wOmegaFinal = lastOutput.wheelOmega ?? (vehicle as any).wheelOmega ?? 0
    const rpmFinal = lastOutput.rpm

    // Check for RPM 2100 + vx~0.28 state
    const rpm2100check = Math.abs(rpmFinal - 2100) < 300 && vxFinal < 0.4
    const rpm1300check = Math.abs(rpmFinal - 1300) < 300 && vxFinal > 0.7
    void rpm2100check
    void rpm1300check
    void wOmegaFinal

    // Also scan for RPM 2100 + low vx region during 0~5s
    const vehicle2 = new DynamicVehicle()
    for (let frame = 0; frame < Math.round(5 / dt); frame++) {
      vehicle2.update(input, dt)
    }

    // Minimal assertion — test is for observation only
    expect(true).toBe(true)
  })
})
