import { describe, it, expect } from 'vitest'
import { SoilGrid } from '../SoilGrid'

describe('SoilGrid.getFieldTilledRatio', () => {
  it('returns 0 for empty grid', () => {
    const grid = new SoilGrid()
    const polygon: [number, number][] = [[-50, -100], [50, -100], [50, 100], [-50, 100]]

    const ratio = grid.getFieldTilledRatio(polygon)

    expect(ratio).toBe(0)
  })

  it('returns 1 when all cells in polygon are tilled', () => {
    const polygon: [number, number][] = [[-10, -10], [10, -10], [10, 10], [-10, 10]]
    const grid = new SoilGrid()

    // Get all cells within the polygon and set them to tilled
    const { minX, maxX, minZ, maxZ } = { minX: -10, maxX: 10, minZ: -10, maxZ: 10 }
    const minCol = Math.max(0, Math.floor((minX + grid.fieldWidth / 2) / grid.cellSize))
    const maxCol = Math.min(grid.cols - 1, Math.floor((maxX + grid.fieldWidth / 2) / grid.cellSize))
    const minRow = Math.max(0, Math.floor((minZ + grid.fieldLength / 2) / grid.cellSize))
    const maxRow = Math.min(grid.rows - 1, Math.floor((maxZ + grid.fieldLength / 2) / grid.cellSize))

    // Set all cells in the polygon to tilled
    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const center = grid.gridToWorld(col, row)
        if (!center) continue
        // Simple check: if center is within bounds
        if (center.x >= minX && center.x <= maxX && center.z >= minZ && center.z <= maxZ) {
          grid.setTilled(col, row, true)
        }
      }
    }

    const ratio = grid.getFieldTilledRatio(polygon)

    expect(ratio).toBeCloseTo(1, 6)
  })

  it('returns approximately 0.5 when 50% of cells in polygon are tilled', () => {
    const polygon: [number, number][] = [[-20, -20], [20, -20], [20, 20], [-20, 20]]
    const grid = new SoilGrid()

    const { minX, maxX, minZ, maxZ } = { minX: -20, maxX: 20, minZ: -20, maxZ: 20 }
    const minCol = Math.max(0, Math.floor((minX + grid.fieldWidth / 2) / grid.cellSize))
    const maxCol = Math.min(grid.cols - 1, Math.floor((maxX + grid.fieldWidth / 2) / grid.cellSize))
    const minRow = Math.max(0, Math.floor((minZ + grid.fieldLength / 2) / grid.cellSize))
    const maxRow = Math.min(grid.rows - 1, Math.floor((maxZ + grid.fieldLength / 2) / grid.cellSize))

    // Set approximately half the cells to tilled (every other row)
    let count = 0
    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const center = grid.gridToWorld(col, row)
        if (!center) continue
        if (center.x >= minX && center.x <= maxX && center.z >= minZ && center.z <= maxZ) {
          if (row % 2 === 0) {
            grid.setTilled(col, row, true)
            count++
          }
        }
      }
    }

    // We set roughly 50% to tilled
    expect(count).toBeGreaterThan(0)

    const ratio = grid.getFieldTilledRatio(polygon)

    // Expect roughly 50%, allowing some tolerance
    expect(ratio).toBeGreaterThan(0.3)
    expect(ratio).toBeLessThan(0.7)
  })

  it('excludes cells outside the polygon from tilled ratio calculation', () => {
    const polygon: [number, number][] = [[-10, -10], [10, -10], [10, 10], [-10, 10]]
    const grid = new SoilGrid()

    // Set cells outside the polygon to tilled
    // These should NOT be counted in the ratio
    const outsideCol = 300 // Far outside the polygon area
    const outsideRow = 400
    grid.setTilled(outsideCol, outsideRow, true)

    // Set some cells inside the polygon to tilled
    const { minX, maxX, minZ, maxZ } = { minX: -10, maxX: 10, minZ: -10, maxZ: 10 }
    const minCol = Math.max(0, Math.floor((minX + grid.fieldWidth / 2) / grid.cellSize))
    const maxCol = Math.min(grid.cols - 1, Math.floor((maxX + grid.fieldWidth / 2) / grid.cellSize))
    const minRow = Math.max(0, Math.floor((minZ + grid.fieldLength / 2) / grid.cellSize))
    const maxRow = Math.min(grid.rows - 1, Math.floor((maxZ + grid.fieldLength / 2) / grid.cellSize))

    // Set one cell inside to tilled
    if (minCol <= maxCol && minRow <= maxRow) {
      const center = grid.gridToWorld(minCol, minRow)
      if (center && center.x >= minX && center.x <= maxX && center.z >= minZ && center.z <= maxZ) {
        grid.setTilled(minCol, minRow, true)
      }
    }

    const ratio = grid.getFieldTilledRatio(polygon)

    // The ratio should be calculated only from cells inside the polygon
    // Even though we set a cell outside to tilled, it should not affect the result
    expect(ratio).toBeGreaterThan(0)
    expect(ratio).toBeLessThan(1)
  })
})
