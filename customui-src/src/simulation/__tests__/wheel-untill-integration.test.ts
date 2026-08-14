import { beforeEach, describe, expect, it, vi } from 'vitest'
import { GameLoop } from '../GameLoop'

vi.stubGlobal('requestAnimationFrame', vi.fn())
vi.stubGlobal('cancelAnimationFrame', vi.fn())

// Shared clock that performance.now() reads from.
// Start at 0 so that (now - lastSoilUpdate=0) >= 100 triggers on the very first real tick.
let _clock = 0

function resetClock(): void {
  _clock = 0
}

/**
 * Advance the clock and invoke the private tick() method directly.
 * Each call represents one animation frame.
 */
function advanceTick(loop: GameLoop, dtMs: number = 16): void {
  _clock += dtMs
  ;(globalThis as any).performance = { now: () => _clock }
  ;(loop as any).tick()
}

/**
 * Initialize performance.now() and call loop.start().
 * start() calls tick() internally; when lastTime===null the tick sets lastTime and returns early.
 */
function startLoop(loop: GameLoop): void {
  ;(globalThis as any).performance = { now: () => _clock }
  loop.start()
}

// Wheel position cells for tractor at (0,0) heading=0 — taken from WheelUntillEffect.test.ts Case 3.
// FL: x=-0.735, z=1.025, widthM=0.4 → cols 396-397, row=804
// FR: x=0.735,  z=1.025, widthM=0.4 → cols 402-403, row=804
// RL: x=-0.760, z=-1.025, widthM=0.55 → cols 395-398, row=795
// RR: x=0.760,  z=-1.025, widthM=0.55 → cols 401-404, row=795
const WHEEL_CELLS = [
  // FL
  { col: 396, row: 804 },
  { col: 397, row: 804 },
  // FR
  { col: 402, row: 804 },
  { col: 403, row: 804 },
  // RL
  { col: 395, row: 795 },
  { col: 396, row: 795 },
  { col: 397, row: 795 },
  { col: 398, row: 795 },
  // RR
  { col: 401, row: 795 },
  { col: 402, row: 795 },
  { col: 403, row: 795 },
  { col: 404, row: 795 },
]

describe('wheel-untill integration', () => {
  beforeEach(() => {
    vi.mocked(requestAnimationFrame).mockReset()
    vi.mocked(cancelAnimationFrame).mockReset()
    resetClock()
  })

  it('Case 1: tick1 leaves cells tilled; tick2 unmarks all 4-wheel cells and emits depth===0', () => {
    const loop = new GameLoop(() => 0)
    const grid = loop.getSoilGrid()

    // Pre-till all wheel contact cells
    for (const { col, row } of WHEEL_CELLS) {
      grid.setTilled(col, row, true)
    }

    // Keep tractor stationary at (0,0)
    loop.setInput({ throttle: 0, brake: 1, steer: 0 })

    const capturedCells: { col: number; row: number; depth?: number }[][] = []
    loop.onTillageCells = (cells) => {
      capturedCells.push(cells)
    }

    // start() → first internal tick: lastTime===null, sets lastTime=_clock=0, returns early.
    // prevWheelTickSeen is reset to false inside start().
    startLoop(loop)

    // Tick 1: clock jumps to 110ms → (now=110) - (lastSoilUpdate=0) >= 100 → 10Hz block fires.
    // prevWheelTickSeen===false → wheel untill NOT applied → cells stay tilled.
    // After 10Hz block: prevWheelTickSeen = true.
    advanceTick(loop, 110)

    // All cells must still be tilled after tick 1.
    for (const { col, row } of WHEEL_CELLS) {
      expect(grid.getTilled(col, row)).toBe(true)
    }

    // No depth===0 cell emitted in tick 1.
    const depth0AfterTick1 = capturedCells.flat().filter(c => c.depth === 0)
    expect(depth0AfterTick1.length).toBe(0)

    // Tick 2: clock jumps another 110ms → (now=220) - (lastSoilUpdate=110) >= 100 → 10Hz block fires.
    // prevWheelTickSeen===true → applyWheelUntill runs → cells get untilled.
    advanceTick(loop, 110)

    // All wheel contact cells must now be untilled.
    for (const { col, row } of WHEEL_CELLS) {
      expect(grid.getTilled(col, row)).toBe(false)
    }

    // onTillageCells must have been called with depth===0 items from tick 2.
    const depth0AfterTick2 = capturedCells.flat().filter(c => c.depth === 0)
    expect(depth0AfterTick2.length).toBeGreaterThan(0)
  })

  it('Case 2: implement cells carry no depth field; wheel-untill cells carry depth===0', () => {
    // A large field polygon that fully contains the tractor trajectory and implement path.
    const fieldPolygon: [number, number][] = [
      [-50, -50], [50, -50], [50, 50], [-50, 50],
    ]
    const loop = new GameLoop(() => 0, { fields: [{ polygon: fieldPolygon }] })
    const grid = loop.getSoilGrid()

    // Enable plow implement
    loop.setImplementConfig({ type: 'plow', width: 1.2, depth: 0.25, hitchOffset: 3.0 })
    loop.setImplementActive(true)

    // Pre-till wheel positions so wheel untill has something to unmark on tick 2+.
    for (const { col, row } of WHEEL_CELLS) {
      grid.setTilled(col, row, true)
    }

    // Move tractor forward so the implement travels > 0.02m per 10Hz interval.
    loop.setInput({ throttle: 1, brake: 0, steer: 0 })

    const allEmitted: { col: number; row: number; depth?: number }[] = []
    loop.onTillageCells = (cells) => {
      allEmitted.push(...cells)
    }

    startLoop(loop)

    // Run enough ticks so that both implement and wheel-untill fire.
    // Each tick with dtMs=110 triggers a 10Hz block (now - lastSoilUpdate >= 100).
    // Tick 1: prevWheelTickSeen false → no wheel untill; implement fires (if vehicle moved).
    // Tick 2+: prevWheelTickSeen true → wheel untill fires.
    // 20 ticks × 110ms = 2.2s simulated, giving the vehicle time to accelerate and move.
    for (let i = 0; i < 20; i++) {
      advanceTick(loop, 110)
    }

    // Expect at least one cell without a depth field (from implement).
    const implCells = allEmitted.filter(c => !('depth' in c))
    expect(implCells.length).toBeGreaterThan(0)

    // Expect at least one cell with depth===0 (from wheel untill).
    const wheelUntillCells = allEmitted.filter(c => 'depth' in c && c.depth === 0)
    expect(wheelUntillCells.length).toBeGreaterThan(0)

    // Implement cells must have depth===undefined (i.e. no depth key).
    for (const c of implCells) {
      expect(c.depth).toBeUndefined()
    }
  })
})
