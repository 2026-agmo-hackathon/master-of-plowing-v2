import type { SoilGrid } from './SoilGrid'

export interface WheelUntillInput {
  x: number       // world meters
  z: number       // world meters
  widthM: number  // tire contact width (meters)
}

export function applyWheelUntill(
  soilGrid: SoilGrid,
  wheels: WheelUntillInput[],
): { col: number; row: number }[] {
  const changed: { col: number; row: number }[] = []
  const seen = new Set<number>()

  for (const w of wheels) {
    const halfW = w.widthM / 2
    const cellSize = soilGrid.cellSize
    const cols = soilGrid.cols
    const minX = w.x - halfW
    const maxX = w.x + halfW
    const minCol = Math.floor((minX + soilGrid.fieldWidth / 2) / cellSize)
    const maxCol = Math.floor((maxX + soilGrid.fieldWidth / 2) / cellSize)
    const centerGrid = soilGrid.worldToGrid(w.x, w.z)
    if (!centerGrid) continue
    const row = centerGrid.row

    for (let col = minCol; col <= maxCol; col++) {
      if (col < 0 || col >= cols) continue
      const key = row * cols + col
      if (seen.has(key)) continue
      seen.add(key)
      if (soilGrid.getTilled(col, row)) {
        soilGrid.setTilled(col, row, false)
        changed.push({ col, row })
      }
    }
  }
  return changed
}
