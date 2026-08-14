import { beforeEach, describe, expect, it, vi } from 'vitest'
import { GameLoop } from '../GameLoop'
import { getSoilProperties } from '../SoilType'

vi.stubGlobal('requestAnimationFrame', vi.fn())
vi.stubGlobal('cancelAnimationFrame', vi.fn())

let _clock = 1000

function resetClock(): void {
  _clock = 1000
}

function simulateFrames(loop: GameLoop, frames: number, dtMs: number = 16): void {
  const tick = (loop as any).tick as () => void
  for (let i = 0; i < frames; i++) {
    _clock += dtMs
    ;(globalThis as any).performance = { now: () => _clock }
    tick()
  }
}

function startLoop(loop: GameLoop): void {
  ;(globalThis as any).performance = { now: () => _clock }
  loop.start()
}

describe('GameLoop soil/draft feedback', () => {
  beforeEach(() => {
    vi.mocked(requestAnimationFrame).mockReset()
    vi.mocked(cancelAnimationFrame).mockReset()
    resetClock()
  })

  it('plow active: GameLoop forwards ImplementSystem draft into vehicle.setDraftForce', () => {
    const loop = new GameLoop(() => 0)
    const vehicle = (loop as any).vehicle
    const implementSystem = (loop as any).implementSystem

    loop.setImplementConfig({ type: 'plow', width: 1.2, depth: 0.25, hitchOffset: 3.0 })
    loop.setImplementActive(true)
    loop.setSoilType('loam')

    // Force ImplementSystem to report a positive draft regardless of motion so
    // we can verify the GameLoop wiring without depending on vehicle dynamics.
    const FAKE_DRAFT_N = 1234
    vi.spyOn(implementSystem, 'getLastDraftForceN').mockReturnValue(FAKE_DRAFT_N)
    const draftSpy = vi.spyOn(vehicle, 'setDraftForce')

    startLoop(loop)
    loop.setInput({ throttle: 0, brake: 1, steer: 0 })
    simulateFrames(loop, 60)

    const matchingCalls = draftSpy.mock.calls.filter(call => (call[0] as number) === FAKE_DRAFT_N)
    expect(matchingCalls.length).toBeGreaterThan(0)
  })

  it('soil friction feedback: setFrictionCoeff called with value differing from base', () => {
    const loop = new GameLoop(() => 0)
    const vehicle = (loop as any).vehicle
    const grid = loop.getSoilGrid()

    // Pre-mark a wide area covering everywhere the vehicle could move during the test.
    for (let col = 0; col < grid.cols; col++) {
      for (let row = 0; row < grid.rows; row++) {
        const center = grid.gridToWorld(col, row)
        if (!center) continue
        grid.setCellCompaction(center.x, center.z, 0.5)
        grid.setCellTilled(center.x, center.z, true)
      }
    }

    loop.setSoilType('loam')
    const base = getSoilProperties('loam').friction

    const frictionSpy = vi.spyOn(vehicle, 'setFrictionCoeff')

    startLoop(loop)
    // Keep vehicle stationary so the cell under it never changes.
    loop.setInput({ throttle: 0, brake: 1, steer: 0 })
    simulateFrames(loop, 60)

    const deviated = frictionSpy.mock.calls.filter(call => Math.abs((call[0] as number) - base) > 1e-6)
    expect(deviated.length).toBeGreaterThan(0)

    // Confirm formula matches actual cell state under the vehicle at the end.
    // Wheel load may further compact the cell during the run, so derive the
    // expected mu from the live cell rather than the pre-set values.
    const finalCell = grid.getCell(vehicle.x, vehicle.z)!
    const expected = base
      * (1 + finalCell.compaction * 0.1)
      * (finalCell.tilled ? 0.85 : 1.0)
    const last = frictionSpy.mock.calls[frictionSpy.mock.calls.length - 1][0] as number
    expect(last).toBeCloseTo(expected, 5)
  })

  it('setSoilType propagates to ImplementSystem', () => {
    const loop = new GameLoop(() => 0)
    const implementSystem = (loop as any).implementSystem
    const setSoilTypeSpy = vi.spyOn(implementSystem, 'setSoilType')

    loop.setSoilType('clay')

    expect(setSoilTypeSpy).toHaveBeenCalledWith('clay')
  })
})
