import { describe, expect, it } from 'vitest'
import { KinematicVehicle } from '../KinematicVehicle'
import { TRACTOR_CONFIG } from '../TractorConfig'

const DT = 1 / 60

function stepVehicle(
  vehicle: KinematicVehicle,
  seconds: number,
  input: { throttle: number; brake: number; steer: number },
) {
  let output = vehicle.update(DT, input)
  const steps = Math.ceil(seconds / DT)
  for (let index = 1; index < steps; index++) {
    output = vehicle.update(DT, input)
  }
  return output
}

describe('KinematicVehicle tractor model', () => {
  it('separates steering wheel angle from front wheel angle', () => {
    const vehicle = new KinematicVehicle()

    const output = stepVehicle(vehicle, 1.0, { throttle: 0, brake: 0, steer: 1 })

    expect(output.measuredSteerAngle).toBeGreaterThan(0)
    expect(output.vehicleFrontWheelAngle).toBeGreaterThan(0)
    expect(output.measuredSteerAngle).toBeGreaterThan(output.vehicleFrontWheelAngle)
    expect(output.vehicleFrontWheelAngle).toBeLessThanOrEqual(TRACTOR_CONFIG.maxFrontWheelAngleRad)

    const ratio = output.measuredSteerAngle / Math.max(output.vehicleFrontWheelAngle, 1e-6)
    expect(ratio).toBeGreaterThan(10)
    expect(ratio).toBeLessThan(40)
  })

  it('launches in first gear before upshifting under sustained throttle', () => {
    const vehicle = new KinematicVehicle()

    const firstStep = vehicle.update(DT, { throttle: 1, brake: 0, steer: 0 })
    const cruise = stepVehicle(vehicle, 12, { throttle: 1, brake: 0, steer: 0 })

    expect(firstStep.gear).toBe(1)
    expect(cruise.gear).toBeGreaterThan(1)
    expect(cruise.speed).toBeGreaterThan(1.5)
  })

  it('uses curvature-based limiting so full-lock turns run slower than straight driving', () => {
    const straightVehicle = new KinematicVehicle()
    const turningVehicle = new KinematicVehicle()

    const straight = stepVehicle(straightVehicle, 20, { throttle: 1, brake: 0, steer: 0 })
    const turning = stepVehicle(turningVehicle, 20, { throttle: 1, brake: 0, steer: 1 })

    expect(straight.speed).toBeGreaterThan(turning.speed)
    expect(turning.angularVelocityY).toBeGreaterThan(0)
    expect(turning.vehicleFrontWheelAngle).toBeGreaterThan(0)
  })
})
