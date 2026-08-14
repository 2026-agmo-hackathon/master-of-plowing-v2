import { describe, expect, it } from 'vitest'
import { SoilGrid } from '../SoilGrid'
import type { TerrainPatch } from '../SoilSystem'
import {
  DEFORM_AMPLIFY,
  FULL_REUPLOAD_THRESHOLD,
  applyPatchesToTextureData,
  fillTextureDataFromGrid,
  shouldFullReupload,
} from '../../components/r3f/FieldTerrain'

function makePatch(startCol: number, startRow: number, width: number, height: number): TerrainPatch {
  return { startCol, startRow, width, height, data: new Float32Array(width * height) }
}

function readCell(data: Uint8Array, cols: number, col: number, row: number): [number, number, number, number] {
  const i = (row * cols + col) * 4
  return [data[i], data[i + 1], data[i + 2], data[i + 3]]
}

describe('FieldTerrain dirty-patch transform', () => {
  it('applies 5 patches and writes expected (col, row) bytes from SoilGrid', () => {
    const grid = new SoilGrid()
    const cols = grid.cols
    const rows = grid.rows
    const data = new Uint8Array(cols * rows * 4)

    // Seed five distinct cells with known state.
    const cells: { col: number; row: number; compaction: number; moisture: number; tilled: boolean; deltaH: number }[] = [
      { col: 5, row: 7, compaction: 0.0, moisture: 0.3, tilled: false, deltaH: 0 },
      { col: 100, row: 200, compaction: 0.5, moisture: 0.5, tilled: true, deltaH: 0 },
      { col: 250, row: 500, compaction: 1.0, moisture: 1.0, tilled: false, deltaH: -1.0 },
      { col: 10, row: 700, compaction: 0.25, moisture: 0.75, tilled: true, deltaH: 1.0 },
      { col: 399, row: 0, compaction: 0.8, moisture: 0.2, tilled: false, deltaH: 0 },
    ]
    for (const c of cells) {
      const center = grid.gridToWorld(c.col, c.row)!
      grid.setCellCompaction(center.x, center.z, c.compaction)
      grid.setMoisture(c.col, c.row, c.moisture)
      grid.setTilled(c.col, c.row, c.tilled)
      // adjust height so delta = deltaH (baseHeight is 0 by default with no noise fn).
      grid.setHeight(c.col, c.row, c.deltaH)
    }

    const patches: TerrainPatch[] = cells.map((c) => makePatch(c.col, c.row, 1, 1))
    applyPatchesToTextureData(data, cols, rows, patches, grid)

    for (const c of cells) {
      const [r, g, b, a] = readCell(data, cols, c.col, c.row)
      expect(r).toBe(Math.round(c.compaction * 255))
      expect(g).toBe(Math.round(c.moisture * 255))
      expect(b).toBe(c.tilled ? 255 : 0)
      const normalized = Math.max(0, Math.min(1, (c.deltaH / DEFORM_AMPLIFY) * 0.5 + 0.5))
      const expectedAlpha = Math.round(normalized * 255)
      expect(a).toBe(expectedAlpha)
    }
  })

  it('clamps out-of-bounds patches without throwing and skips fully-outside patches', () => {
    const grid = new SoilGrid()
    const cols = grid.cols
    const rows = grid.rows
    const data = new Uint8Array(cols * rows * 4)

    grid.setCellCompaction(grid.gridToWorld(0, 0)!.x, grid.gridToWorld(0, 0)!.z, 0.4)

    const patches: TerrainPatch[] = [
      // partly outside (negative origin) — should clamp and still cover (0, 0).
      makePatch(-5, -5, 10, 10),
      // fully outside on the right — should be a no-op.
      makePatch(cols + 10, rows + 10, 5, 5),
      // straddles the right edge — should clamp without crashing.
      makePatch(cols - 2, rows - 2, 10, 10),
    ]

    expect(() => applyPatchesToTextureData(data, cols, rows, patches, grid)).not.toThrow()

    // (0, 0) should reflect the seeded compaction.
    const [r0] = readCell(data, cols, 0, 0)
    expect(r0).toBe(Math.round(0.4 * 255))
    // (cols-1, rows-1) should be touched by the straddling patch (default 0).
    const [, , , a99] = readCell(data, cols, cols - 1, rows - 1)
    // baseHeight 0, height 0 → delta 0 → encoded byte 128.
    expect(a99).toBe(128)
  })

  it('shouldFullReupload triggers above the threshold and not below', () => {
    // Below threshold: 70 × 70 = 4900 cells.
    const small: TerrainPatch[] = [makePatch(0, 0, 70, 70)]
    expect(shouldFullReupload(small)).toBe(false)

    // Above threshold: 80 × 80 = 6400 cells.
    const big: TerrainPatch[] = [makePatch(0, 0, 80, 80)]
    expect(shouldFullReupload(big)).toBe(true)

    // Aggregate across many small patches that sum past the threshold.
    const many: TerrainPatch[] = []
    for (let i = 0; i < 100; i++) many.push(makePatch(0, 0, 10, 10))
    expect(shouldFullReupload(many)).toBe(true)

    // Sanity: threshold constant matches spec.
    expect(FULL_REUPLOAD_THRESHOLD).toBe(5000)
  })

  it('fillTextureDataFromGrid produces a fully populated buffer matching grid state', () => {
    const grid = new SoilGrid()
    const cols = grid.cols
    const rows = grid.rows
    const data = new Uint8Array(cols * rows * 4)

    // Set a couple of cells so we can spot-check.
    grid.setCellCompaction(grid.gridToWorld(3, 4)!.x, grid.gridToWorld(3, 4)!.z, 0.6)
    grid.setTilled(3, 4, true)

    fillTextureDataFromGrid(data, cols, rows, grid)

    const [r, g, b, a] = readCell(data, cols, 3, 4)
    expect(r).toBe(Math.round(0.6 * 255))
    // default moisture is 0.3.
    expect(g).toBe(Math.round(0.3 * 255))
    expect(b).toBe(255)
    // delta height is 0 by default → byte 128.
    expect(a).toBe(128)
  })
})
