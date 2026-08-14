import { describe, expect, it } from 'vitest'
import { appSteerCommandToVehicleNormalized } from '../AppSignalConvention'
import { GameLoop } from '../GameLoop'

describe('App steer command adapter', () => {
  it('passes through backend steer-motor commands unchanged', () => {
    expect(appSteerCommandToVehicleNormalized(0.6)).toBeCloseTo(0.6, 8)
    expect(appSteerCommandToVehicleNormalized(-0.25)).toBeCloseTo(-0.25, 8)
    expect(appSteerCommandToVehicleNormalized(0)).toBe(0)
  })

  it('app steer commands drive motor in the correct sign direction', () => {
    // P servo: when current steer = 0 and target = +0.4,
    // error = 0.4, motorCmd = clamp(0.4 * KP, -1, 1) which is positive.
    const loop = new GameLoop()
    loop.applyAppCommand('steer', 0.4)
    expect(loop.getCurrentSteer()).toBeGreaterThan(0)

    const loop2 = new GameLoop()
    loop2.applyAppCommand('steer', -0.4)
    expect(loop2.getCurrentSteer()).toBeLessThan(0)
  })
})
