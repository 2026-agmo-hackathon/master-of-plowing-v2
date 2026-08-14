import { describe, expect, it } from 'vitest'
import { ImplementSystem } from '../ImplementSystem'
import { SoilGrid } from '../SoilGrid'

// A large rectangle covering the full field so isFieldCell returns true
const FULL_FIELD_POLYGON: [number, number][] = [[-100, -200], [100, -200], [100, 200], [-100, 200]]

function createPlowSystem() {
  const grid = new SoilGrid(undefined, { fields: [{ polygon: FULL_FIELD_POLYGON }] })
  const system = new ImplementSystem(grid)
  system.setConfig({ type: 'plow', width: 1.2, depth: 0.25, hitchOffset: 3.0 })
  system.setActive(true)
  return { grid, system }
}

function totalCutDepth(grid: SoilGrid, cells: { col: number; row: number }[]): number {
  return cells.reduce((sum, cell) => sum + Math.max(0, -grid.getHeight(cell.col, cell.row)), 0)
}

describe('Implement soil effects', () => {
  it('does not cut soil until the implement has actually travelled', () => {
    const { grid, system } = createPlowSystem()

    const initial = system.update(0, 0, 0, 1)
    const traveled = system.update(0, 1, 0, 1)

    expect(initial).toEqual([])
    expect(traveled.length).toBeGreaterThan(0)
    expect(totalCutDepth(grid, traveled)).toBeGreaterThan(0)
    expect(totalCutDepth(grid, traveled)).toBeLessThan(0.05)
  })

  it('applies more plow deformation over a longer travelled distance', () => {
    const shortPass = createPlowSystem()
    shortPass.system.update(0, 0, 0, 1)
    const shortCells = shortPass.system.update(0, 0.2, 0, 1)
    const shortCut = totalCutDepth(shortPass.grid, shortCells)

    const longPass = createPlowSystem()
    longPass.system.update(0, 0, 0, 1)
    const longCells = longPass.system.update(0, 1.0, 0, 1)
    const longCut = totalCutDepth(longPass.grid, longCells)

    expect(shortCut).toBeGreaterThan(0)
    expect(longCut).toBeGreaterThan(shortCut)
  })

  it('stays idle while stationary after a cut pass', () => {
    const { grid, system } = createPlowSystem()

    system.update(0, 0, 0, 1)
    const firstPass = system.update(0, 1.0, 0, 1)
    const depthAfterFirstPass = totalCutDepth(grid, firstPass)
    const stationary = system.update(0, 1.0, 0, 1)
    const depthAfterStationary = totalCutDepth(grid, firstPass)

    expect(stationary).toEqual([])
    expect(depthAfterStationary).toBeCloseTo(depthAfterFirstPass, 8)
  })
})
