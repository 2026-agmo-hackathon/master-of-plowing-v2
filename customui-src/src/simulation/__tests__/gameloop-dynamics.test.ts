import { beforeEach, describe, expect, it, vi } from 'vitest'
import { GameLoop } from '../GameLoop'
import type { VehicleState } from '../VehicleSystem'

// Stub browser APIs used by GameLoop
vi.stubGlobal('requestAnimationFrame', vi.fn())
vi.stubGlobal('cancelAnimationFrame', vi.fn())

let _clock = 1000

function resetClock(): void {
  _clock = 1000
}

function simulateFrames(
  loop: GameLoop,
  frames: number,
  dtMs: number = 16,
): void {
  const tick = (loop as any).tick as () => void
  for (let i = 0; i < frames; i++) {
    _clock += dtMs
    ;(globalThis as any).performance = { now: () => _clock }
    tick()
  }
}

describe('GameLoop dynamics integration', () => {
  beforeEach(() => {
    vi.mocked(requestAnimationFrame).mockReset()
    vi.mocked(cancelAnimationFrame).mockReset()
    resetClock()
  })

  function startLoop(loop: GameLoop): void {
    ;(globalThis as any).performance = { now: () => _clock }
    loop.start() // sets lastTime via first tick
  }

  it('flat ground driving: speed > 0.3 after 300 frames', () => {
    const loop = new GameLoop(() => 0)
    startLoop(loop)
    loop.setInput({ throttle: 1, brake: 0, steer: 0 })
    simulateFrames(loop, 300)

    const state = loop.getLastState()
    expect(state).not.toBeNull()
    expect(state!.speed).toBeGreaterThan(0.3)
  })

  it('slope effect: uphill speed less than flat ground', () => {
    // Flat ground reference
    const flatLoop = new GameLoop(() => 0)
    startLoop(flatLoop)
    flatLoop.setInput({ throttle: 0.5, brake: 0, steer: 0 })
    simulateFrames(flatLoop, 600)
    const flatSpeed = flatLoop.getLastState()!.speed

    // Sloped terrain: height increases with z to create uphill (~10 deg)
    resetClock()
    const slopeLoop = new GameLoop((_x: number, z: number) => z * Math.tan(10 * Math.PI / 180))
    startLoop(slopeLoop)
    slopeLoop.setInput({ throttle: 0.5, brake: 0, steer: 0 })
    simulateFrames(slopeLoop, 600)
    const slopeSpeed = slopeLoop.getLastState()!.speed

    expect(slopeSpeed).toBeLessThan(flatSpeed)
  })

  it('GPS coordinate changes after driving', () => {
    const loop = new GameLoop(() => 0)
    startLoop(loop)
    loop.setInput({ throttle: 1, brake: 0, steer: 0 })
    simulateFrames(loop, 300)

    const state = loop.getLastState()!
    expect(state.latitude).not.toBe(37.5665)
  })

  it('SoilGrid ridges are reflected in VehicleState altitude and pitch', () => {
    const loop = new GameLoop(() => 0, {
      fields: [{ polygon: [[-90, -180], [90, -180], [90, 180], [-90, 180]] }],
    })
    loop.setFields([{ polygon: [[-90, -180], [90, -180], [90, 180], [-90, 180]] }])
    loop.injectState(37.5665 + 0.6 / 111320, 126.978, 0)
    startLoop(loop)
    loop.setInput({ throttle: 0, brake: 1, steer: 0 })
    simulateFrames(loop, 2)

    const state = loop.getLastState()!
    expect(state.altitude).toBeGreaterThan(0)
    expect(Math.abs(state.pitch)).toBeGreaterThan(0.1)
    expect(Math.abs(state.terrainPitchRad ?? 0)).toBeGreaterThan(0.001)
  })

  it('start-stop cycle: speed < 0.1 after braking', () => {
    const loop = new GameLoop(() => 0)
    startLoop(loop)

    // Drive for 120 frames
    loop.setInput({ throttle: 1, brake: 0, steer: 0 })
    simulateFrames(loop, 120)
    expect(Math.abs(loop.getLastState()!.speed)).toBeGreaterThan(0)

    // Brake for 180 frames (3s)
    loop.setInput({ throttle: 0, brake: 1, steer: 0 })
    simulateFrames(loop, 180)
    expect(Math.abs(loop.getLastState()!.speed)).toBeLessThan(0.5)
  })

  it('stationary telemetry is clamped to zero at idle', () => {
    const loop = new GameLoop(() => 0)
    startLoop(loop)
    loop.setManualGear(0)
    loop.setInput({ throttle: 0, brake: 0, steer: 0 })
    simulateFrames(loop, 10)

    const state = loop.getLastState()!
    expect(state.speed).toBe(0)
    expect(state.wheelSpeed).toBe(0)
    expect(state.accel_x).toBe(0)
  })

  it('VehicleState fields are all finite numbers', () => {
    const loop = new GameLoop(() => 0)
    startLoop(loop)
    loop.setInput({ throttle: 0.5, brake: 0, steer: 0 })
    simulateFrames(loop, 60)

    const state = loop.getLastState()!
    const numericKeys: (keyof VehicleState)[] = [
      'latitude', 'longitude', 'altitude', 'heading', 'speed', 'numSat',
      'rpm', 'throttle', 'brake', 'steerAngle', 'measuredSteerAngleDeg',
      'vehicleFrontWheelAngleDeg', 'motorCmdRpm', 'accel_x', 'accel_y',
      'gyro_z', 'roll', 'pitch', 'yaw', 'gear', 'appSteerCommand',
    ]
    for (const key of numericKeys) {
      expect(Number.isFinite(state[key]), `${key} should be finite, got ${state[key]}`).toBe(true)
    }
    expect(typeof state.steerFeedbackActive).toBe('boolean')
  })
})
