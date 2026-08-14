import type { ImplementConfig, ImplementContact, ImplementEffect } from './ImplementTypes'
import type { SoilGrid } from './SoilGrid'

const PLOW_DEPTH_PER_METER = 0.16

export class PlowEffect implements ImplementEffect {
  apply(grid: SoilGrid, contacts: ImplementContact[], speed: number, config: ImplementConfig): void {
    if (speed < 0) {
      for (const { col, row } of contacts) {
        grid.setTilled(col, row, false)
      }
      return
    }

    const speedFactor = Math.max(0.6, Math.min(1.2, 0.7 + Math.abs(speed) * 0.15))

    for (const { col, row, distanceMeters } of contacts) {
      const moisture = grid.getMoisture(col, row)
      const baseHeight = grid.getBaseHeight(col, row)
      const moistureFactor = 0.5 + moisture
      const cutDepth = config.depth * PLOW_DEPTH_PER_METER * distanceMeters * moistureFactor * speedFactor
      if (cutDepth <= 0) continue

      grid.setHeight(col, row, baseHeight - cutDepth)
      grid.setTilled(col, row, true)

      if (speed > 2.0) {
        for (const offset of [-2, -1, 1, 2]) {
          const adjCol = col + offset
          const adjBase = grid.getBaseHeight(adjCol, row)
          grid.setHeight(adjCol, row, adjBase + cutDepth * 0.08)
        }
      } else {
        for (const offset of [-1, 1]) {
          const adjCol = col + offset
          const adjBase = grid.getBaseHeight(adjCol, row)
          grid.setHeight(adjCol, row, adjBase + cutDepth * 0.18)
        }
      }
    }
  }
}
