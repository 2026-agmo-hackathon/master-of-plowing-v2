import type { ImplementConfig, ImplementContact, ImplementEffect } from './ImplementTypes'
import type { SoilGrid } from './SoilGrid'

export class RotaryEffect implements ImplementEffect {
  apply(grid: SoilGrid, contacts: ImplementContact[], _speed: number, _config: ImplementConfig): void {
    for (const { col, row } of contacts) {
      const baseHeight = grid.getBaseHeight(col, row)
      const currentCompaction = grid.getCompaction(col, row)

      grid.setTilled(col, row, true)
      grid.setHeight(col, row, baseHeight)
      grid.applyDeformation(col, row, 0, -currentCompaction)
    }
  }
}
