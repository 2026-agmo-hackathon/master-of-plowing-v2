import { SoilGrid } from './SoilGrid'
import type { SimulationConfig } from './ImplementTypes'
import { DEFAULT_SIM_CONFIG } from './ImplementTypes'
import { TRACTOR_CONFIG } from './TractorConfig'

export interface WheelContact {
  worldPosition: { x: number; y: number; z: number }
  normal: { x: number; y: number; z: number }
  depth: number
  normalForce: number
}

export interface TerrainPatch {
  startCol: number
  startRow: number
  width: number
  height: number
  data: Float32Array
}

const TIRE_RADIUS = TRACTOR_CONFIG.wheelInfluenceRadiusM

export class SoilSystem {
  private grid: SoilGrid
  private config: SimulationConfig
  private dirtySet: Set<string> = new Set()  // tracks dirty cells as "col,row" strings

  constructor(grid: SoilGrid, config?: SimulationConfig) {
    this.grid = grid
    this.config = config ? { ...config } : { ...DEFAULT_SIM_CONFIG }
  }

  updateConfig(partial: Partial<SimulationConfig>): void {
    this.config = { ...this.config, ...partial }
  }

  /**
   * Deforms soil based on wheel contact information and returns dirty patches.
   */
  update(wheelContacts: WheelContact[]): TerrainPatch[] {
    this.dirtySet.clear()

    for (const contact of wheelContacts) {
      // Get cells around the wheel contact point
      const cells = this.grid.getCellsInRadius(
        contact.worldPosition.x,
        contact.worldPosition.z,
        TIRE_RADIUS
      )

      for (const cell of cells) {
        const dx = cell.worldX - contact.worldPosition.x
        const dz = cell.worldZ - contact.worldPosition.z
        const dist = Math.sqrt(dx * dx + dz * dz)
        const footprint = Math.max(0, 1 - dist / TIRE_RADIUS)
        const pressure = footprint * footprint
        if (pressure <= 0) continue

        // Compaction only: sinkage is handled externally.
        const deltaCompaction = this.config.compactionRate * (0.35 + pressure) * (1 - cell.compaction * 0.5)

        if (deltaCompaction > 0.0001) {
          this.grid.applyDeformation(cell.col, cell.row, 0, deltaCompaction)
          this.dirtySet.add(`${cell.col},${cell.row}`)
        }
      }
    }

    // Bundle dirty cells into TerrainPatches
    // Simple approach: one patch per bounding box of all dirty cells
    if (this.dirtySet.size === 0) return []

    return this.buildPatches()
  }

  buildPatches(): TerrainPatch[] {
    // Compute bounding box of dirty cells
    let minCol = Infinity, maxCol = -Infinity
    let minRow = Infinity, maxRow = -Infinity

    for (const key of this.dirtySet) {
      const [c, r] = key.split(',').map(Number)
      if (c < minCol) minCol = c
      if (c > maxCol) maxCol = c
      if (r < minRow) minRow = r
      if (r > maxRow) maxRow = r
    }

    const width = maxCol - minCol + 1
    const height = maxRow - minRow + 1
    const data = new Float32Array(width * height)

    // Fill heights array matching heightfield layout
    for (let r = 0; r < height; r++) {
      for (let c = 0; c < width; c++) {
        data[r * width + c] = this.grid.getHeight(minCol + c, minRow + r)
      }
    }

    return [{
      startCol: minCol,
      startRow: minRow,
      width,
      height,
      data
    }]
  }

  addDirtyCells(cells: { col: number; row: number }[]): void {
    for (const { col, row } of cells) {
      this.dirtySet.add(`${col},${row}`)
    }
  }

  markCellsDirty(cells: { col: number; row: number }[]): void {
    for (const cell of cells) {
      this.dirtySet.add(`${cell.col},${cell.row}`)
    }
  }

  clearDirty(): void { this.dirtySet.clear() }

  getGrid(): SoilGrid {
    return this.grid
  }
}
