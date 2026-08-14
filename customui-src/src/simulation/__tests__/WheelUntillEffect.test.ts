import { beforeEach, describe, expect, it } from 'vitest'
import { SoilGrid } from '../SoilGrid'
import { applyWheelUntill } from '../WheelUntillEffect'

// SoilGrid constants: cols=800, rows=1600, cellSize=0.25, fieldWidth=200, fieldLength=400
// worldToGrid(x, z): col = floor((x + 100) / 0.25), row = floor((z + 200) / 0.25)
// worldToGrid(0, 0) => col=400, row=800

describe('applyWheelUntill', () => {
  let grid: SoilGrid

  beforeEach(() => {
    grid = new SoilGrid()
  })

  it('Case 1: single rear wheel (widthM=0.55) unmarks 4 tilled cells', () => {
    // halfW=0.275, minX=-0.275, maxX=0.275
    // minCol=floor(99.725/0.25)=398, maxCol=floor(100.275/0.25)=401 → 4 cells
    const tilledCells = [
      { col: 398, row: 800 },
      { col: 399, row: 800 },
      { col: 400, row: 800 },
      { col: 401, row: 800 },
    ]
    for (const { col, row } of tilledCells) {
      grid.setTilled(col, row, true)
    }

    const changed = applyWheelUntill(grid, [{ x: 0, z: 0, widthM: 0.55 }])

    expect(changed.length).toBe(4)
    for (const { col, row } of tilledCells) {
      expect(grid.getTilled(col, row)).toBe(false)
    }
  })

  it('Case 2: single front wheel (widthM=0.4) unmarks 2 tilled cells', () => {
    // halfW=0.2, minX=-0.2, maxX=0.2
    // minCol=floor(99.8/0.25)=399, maxCol=floor(100.2/0.25)=400 → 2 cells
    const tilledCells = [
      { col: 399, row: 800 },
      { col: 400, row: 800 },
    ]
    for (const { col, row } of tilledCells) {
      grid.setTilled(col, row, true)
    }

    const changed = applyWheelUntill(grid, [{ x: 0, z: 0, widthM: 0.4 }])

    expect(changed.length).toBe(2)
    for (const { col, row } of tilledCells) {
      expect(grid.getTilled(col, row)).toBe(false)
    }
  })

  it('Case 3: 4 wheels simultaneously unmark all pre-tilled cells (FL:2, FR:2, RL:4, RR:4)', () => {
    // FL: x=-0.735, z=1.025, widthM=0.4 → cols 396-397, row=804 (2 cells)
    // FR: x=0.735,  z=1.025, widthM=0.4 → cols 402-403, row=804 (2 cells)
    // RL: x=-0.760, z=-1.025, widthM=0.55 → cols 395-398, row=795 (4 cells)
    // RR: x=0.760,  z=-1.025, widthM=0.55 → cols 401-404, row=795 (4 cells)
    const tilledCells = [
      // FL
      { col: 396, row: 804 }, { col: 397, row: 804 },
      // FR
      { col: 402, row: 804 }, { col: 403, row: 804 },
      // RL
      { col: 395, row: 795 }, { col: 396, row: 795 }, { col: 397, row: 795 }, { col: 398, row: 795 },
      // RR
      { col: 401, row: 795 }, { col: 402, row: 795 }, { col: 403, row: 795 }, { col: 404, row: 795 },
    ]
    for (const { col, row } of tilledCells) {
      grid.setTilled(col, row, true)
    }

    const changed = applyWheelUntill(grid, [
      { x: -0.735, z: 1.025, widthM: 0.4 },
      { x:  0.735, z: 1.025, widthM: 0.4 },
      { x: -0.760, z: -1.025, widthM: 0.55 },
      { x:  0.760, z: -1.025, widthM: 0.55 },
    ])

    expect(changed.length).toBe(tilledCells.length)
    for (const { col, row } of tilledCells) {
      expect(grid.getTilled(col, row)).toBe(false)
    }
  })

  it('Case 4: cells already untilled → returns empty array', () => {
    // All cells default to tilled=false; no prior setTilled calls
    const changed = applyWheelUntill(grid, [{ x: 0, z: 0, widthM: 0.55 }])

    expect(changed.length).toBe(0)
  })

  it('Case 5: wheel position outside grid bounds → returns empty array without throwing', () => {
    // worldToGrid(9999, 9999) returns null in dense mode → wheel is skipped
    expect(() => {
      const changed = applyWheelUntill(grid, [{ x: 9999, z: 9999, widthM: 0.55 }])
      expect(changed.length).toBe(0)
    }).not.toThrow()
  })
})
