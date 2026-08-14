import { createNoise2D } from 'simplex-noise'
import { pointInPolygon, polygonBoundingBox, pointToPolygonEdgeDistance } from './PolygonUtils'
import { smoothstep } from './TerrainNoise'

// Fixed seed so hills are deterministic across reloads.
function hillSeed(): () => number {
  let s = 0xC0FFEE
  return () => {
    s = (s * 1664525 + 1013904223) >>> 0
    return s / 0x100000000
  }
}
const hillNoise2D = createNoise2D(hillSeed())

export interface SoilCell {
  row: number
  col: number
  worldX: number
  worldZ: number
  height: number
  compaction: number
  moisture: number
  tilled: boolean
}

export interface SoilSample {
  sinkDepth: number
  moisture: number
  compaction: number
  tilled: boolean
}

interface SparseCell {
  height: number
  compaction: number
  moisture: number
  tilled: number
  base: number
}

interface FieldBBox {
  polygon: [number, number][]
  bbox: { minX: number; minZ: number; maxX: number; maxZ: number }
}

// Default sparse cell values (used when cell not yet materialized)
const DEFAULT_HEIGHT = 0
const DEFAULT_COMPACTION = 0
const DEFAULT_MOISTURE = 0.3
const DEFAULT_TILLED = 0
const DEFAULT_BASE_HEIGHT = 0

// Approximate per-cell bytes for sparse storage footprint:
// 4 (height) + 4 (compaction) + 4 (moisture) + 1 (tilled) ≈ 13 bytes
// plus Map entry overhead (key string ~16 + bucket ~16 + object header ~16) ≈ 48 bytes total
const SPARSE_CELL_BYTES = 48
// Dense bytes per cell: Float32×3 + Uint8×1 = 13
const DENSE_CELL_BYTES = 13

export class SoilGrid {
  readonly cols: number
  readonly rows: number
  readonly cellSize = 0.25
  readonly fieldWidth: number
  readonly fieldLength: number
  readonly centerX: number
  readonly centerZ: number

  // Dense storage (used when sparse=false)
  private height!: Float32Array
  private compaction!: Float32Array
  private moisture!: Float32Array
  private tilled!: Uint8Array
  private baseHeight!: Float32Array

  // Sparse storage (used when sparse=true)
  private sparse: boolean = false
  private sparseCells: Map<number, SparseCell> | null = null
  private fields: FieldBBox[] = []

  constructor(
    terrainNoiseFn?: (x: number, z: number) => number,
    options?: {
      fields?: { polygon: [number, number][] }[]
      width?: number
      height?: number
      centerX?: number
      centerZ?: number
    }
  ) {
    this.fieldWidth  = options?.width   ?? 200
    this.fieldLength = options?.height  ?? 400
    this.centerX     = options?.centerX ?? 0
    this.centerZ     = options?.centerZ ?? 0
    // Cell counts driven by field extent and fixed cell size
    this.cols = Math.ceil(this.fieldWidth  / this.cellSize)
    this.rows = Math.ceil(this.fieldLength / this.cellSize)

    const size = this.cols * this.rows

    if (options?.fields && options.fields.length > 0) {
      this.fields = options.fields.map((f) => ({
        polygon: f.polygon,
        bbox: polygonBoundingBox(f.polygon),
      }))
      this.sparse = true
      this.sparseCells = new Map()
    }

    // Always use dense storage so the visual renderer's delta-from-noise
    // calculation (originalHeight + (newHeight - noise) * AMP) works uniformly.
    this.height = new Float32Array(size)
    this.compaction = new Float32Array(size)
    this.moisture = new Float32Array(size).fill(0.3)
    this.tilled = new Uint8Array(size)
    this.baseHeight = new Float32Array(size)

    if (terrainNoiseFn) {
      for (let row = 0; row < this.rows; row++) {
        for (let col = 0; col < this.cols; col++) {
          // World coordinate = grid offset from map center (centerX, centerZ)
          const worldX = (col + 0.5) * this.cellSize - this.fieldWidth  / 2 + this.centerX
          const worldZ = (row + 0.5) * this.cellSize - this.fieldLength / 2 + this.centerZ
          const h = terrainNoiseFn(worldX, worldZ)
          this.height[this.index(col, row)] = h
          this.baseHeight[this.index(col, row)] = h
        }
      }
    }

  }

  setFields(fields: { polygon: [number, number][] }[]): void {
    this.fields = fields.map((f) => ({
      polygon: f.polygon,
      bbox: polygonBoundingBox(f.polygon),
    }))
  }

  public isInsideAnyField(worldX: number, worldZ: number): boolean {
    for (const f of this.fields) {
      if (
        worldX < f.bbox.minX || worldX > f.bbox.maxX ||
        worldZ < f.bbox.minZ || worldZ > f.bbox.maxZ
      ) {
        continue
      }
      if (pointInPolygon([worldX, worldZ], f.polygon)) return true
    }
    return false
  }

  isFieldCell(col: number, row: number): boolean {
    const center = this.gridToWorld(col, row)
    if (!center) return false
    return this.isInsideAnyField(center.x, center.z)
  }

  private getOrCreateSparseCell(idx: number): SparseCell {
    let cell = this.sparseCells!.get(idx)
    if (!cell) {
      cell = {
        height: DEFAULT_HEIGHT,
        compaction: DEFAULT_COMPACTION,
        moisture: DEFAULT_MOISTURE,
        tilled: DEFAULT_TILLED,
        base: DEFAULT_BASE_HEIGHT,
      }
    }
    return cell
  }

  private readSparseCell(idx: number): SparseCell {
    const cell = this.sparseCells!.get(idx)
    if (cell) return cell
    return {
      height: DEFAULT_HEIGHT,
      compaction: DEFAULT_COMPACTION,
      moisture: DEFAULT_MOISTURE,
      tilled: DEFAULT_TILLED,
      base: DEFAULT_BASE_HEIGHT,
    }
  }

  worldToGrid(worldX: number, worldZ: number): { col: number; row: number } | null {
    // Shift world coordinates into the grid's local frame before mapping to cell indices
    const col = Math.floor((worldX - this.centerX + this.fieldWidth  / 2) / this.cellSize)
    const row = Math.floor((worldZ - this.centerZ + this.fieldLength / 2) / this.cellSize)
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) {
      // In sparse mode, clamp out-of-grid coordinates so callers can still
      // pass them through; isInsideAnyField will reject writes as no-ops.
      if (this.sparse) {
        return {
          col: Math.max(0, Math.min(this.cols - 1, col)),
          row: Math.max(0, Math.min(this.rows - 1, row)),
        }
      }
      return null
    }
    return { col, row }
  }

  gridToWorld(col: number, row: number): { x: number; z: number } | null {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) {
      return null
    }
    // Map cell indices back to world coordinates in the shifted frame
    const x = (col + 0.5) * this.cellSize - this.fieldWidth  / 2 + this.centerX
    const z = (row + 0.5) * this.cellSize - this.fieldLength / 2 + this.centerZ
    return { x, z }
  }

  getCell(worldX: number, worldZ: number): SoilCell | null {
    const grid = this.worldToGrid(worldX, worldZ)
    if (grid === null) {
      // In sparse mode, out-of-grid coordinates are treated as outside-field defaults
      if (this.sparse) {
        return {
          row: -1,
          col: -1,
          worldX,
          worldZ,
          height: DEFAULT_HEIGHT,
          compaction: DEFAULT_COMPACTION,
          moisture: DEFAULT_MOISTURE,
          tilled: false,
        }
      }
      return null
    }
    const { col, row } = grid
    const idx = this.index(col, row)
    const center = this.gridToWorld(col, row)!

    if (this.sparse) {
      if (!this.isInsideAnyField(worldX, worldZ)) {
        return {
          row,
          col,
          worldX: center.x,
          worldZ: center.z,
          height: DEFAULT_HEIGHT,
          compaction: DEFAULT_COMPACTION,
          moisture: DEFAULT_MOISTURE,
          tilled: false,
        }
      }
      const cell = this.readSparseCell(idx)
      return {
        row,
        col,
        worldX: center.x,
        worldZ: center.z,
        height: cell.height,
        compaction: cell.compaction,
        moisture: cell.moisture,
        tilled: cell.tilled >= 128,
      }
    }

    return {
      row,
      col,
      worldX: center.x,
      worldZ: center.z,
      height: this.height[idx],
      compaction: this.compaction[idx],
      moisture: this.moisture[idx],
      tilled: this.tilled[idx] >= 128,
    }
  }

  getSoilAt(worldX: number, worldZ: number): SoilCell | null {
    return this.getCell(worldX, worldZ)
  }

  setCellCompaction(worldX: number, worldZ: number, value: number): void {
    const grid = this.worldToGrid(worldX, worldZ)
    if (grid === null) return
    const idx = this.index(grid.col, grid.row)
    if (this.sparse) {
      if (!this.isInsideAnyField(worldX, worldZ)) return
      const cell = this.getOrCreateSparseCell(idx)
      cell.compaction = value
      this.sparseCells!.set(idx, cell)
      return
    }
    this.compaction[idx] = value
  }

  setCellTilled(worldX: number, worldZ: number, value: boolean): void {
    const grid = this.worldToGrid(worldX, worldZ)
    if (grid === null) return
    const { col, row } = grid
    const centerIntensity = value ? 255 : 0

    // Write center cell
    if (this.sparse) {
      if (!this.isInsideAnyField(worldX, worldZ)) return
      const idx = this.index(col, row)
      const cell = this.getOrCreateSparseCell(idx)
      cell.tilled = centerIntensity
      this.sparseCells!.set(idx, cell)
    } else {
      this.tilled[this.index(col, row)] = centerIntensity
    }

    if (!value) return  // Only spread gradient for tilling (not un-tilling)

    // 4-ring (9x9) gradient using Chebyshev distance: covers ~1m radius at 0.25m/cell.
    // intensity = max(0, 255 * (1 - chebyshevDist / 4.5)), so ring4 ≈ 27, ring1 ≈ 198.
    // This gives ~2m physical transition zone (4 cells × 0.25m/cell) matching the
    // prior 2-ring zone at 0.5m/cell.
    for (let dr = -4; dr <= 4; dr++) {
      for (let dc = -4; dc <= 4; dc++) {
        if (dc === 0 && dr === 0) continue  // center already written
        const chebyshev = Math.max(Math.abs(dc), Math.abs(dr))
        const intensity = Math.max(0, Math.round(255 * (1 - chebyshev / 4.5)))
        if (intensity <= 0) continue
        const nc = col + dc
        const nr = row + dr
        if (nc < 0 || nc >= this.cols || nr < 0 || nr >= this.rows) continue
        const nIdx = this.index(nc, nr)
        if (this.sparse) {
          const nCell = this.getOrCreateSparseCell(nIdx)
          if (nCell.tilled < intensity) {
            nCell.tilled = intensity
            this.sparseCells!.set(nIdx, nCell)
          }
        } else {
          if (this.tilled[nIdx] < intensity) {
            this.tilled[nIdx] = intensity
          }
        }
      }
    }
  }

  getCellsInRadius(worldX: number, worldZ: number, radius: number): SoilCell[] {
    const result: SoilCell[] = []
    // Convert world-space search bounds to grid indices using the shifted frame
    const minCol = Math.max(0, Math.floor((worldX - radius - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const maxCol = Math.min(this.cols - 1, Math.floor((worldX + radius - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const minRow = Math.max(0, Math.floor((worldZ - radius - this.centerZ + this.fieldLength / 2) / this.cellSize))
    const maxRow = Math.min(this.rows - 1, Math.floor((worldZ + radius - this.centerZ + this.fieldLength / 2) / this.cellSize))

    const radiusSq = radius * radius
    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const center = this.gridToWorld(col, row)!
        const dx = center.x - worldX
        const dz = center.z - worldZ
        if (dx * dx + dz * dz <= radiusSq) {
          const idx = this.index(col, row)
          if (this.sparse) {
            if (!this.isInsideAnyField(center.x, center.z)) {
              result.push({
                row,
                col,
                worldX: center.x,
                worldZ: center.z,
                height: DEFAULT_HEIGHT,
                compaction: DEFAULT_COMPACTION,
                moisture: DEFAULT_MOISTURE,
                tilled: false,
              })
              continue
            }
            const cell = this.readSparseCell(idx)
            result.push({
              row,
              col,
              worldX: center.x,
              worldZ: center.z,
              height: cell.height,
              compaction: cell.compaction,
              moisture: cell.moisture,
              tilled: cell.tilled >= 128,
            })
          } else {
            result.push({
              row,
              col,
              worldX: center.x,
              worldZ: center.z,
              height: this.height[idx],
              compaction: this.compaction[idx],
              moisture: this.moisture[idx],
              tilled: this.tilled[idx] >= 128,
            })
          }
        }
      }
    }
    return result
  }

  applyDeformation(col: number, row: number, deltaHeight: number, deltaCompaction: number): void {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return
    const idx = this.index(col, row)
    if (this.sparse) {
      const center = this.gridToWorld(col, row)!
      if (!this.isInsideAnyField(center.x, center.z)) return
      const cell = this.getOrCreateSparseCell(idx)
      cell.height += deltaHeight
      cell.compaction = Math.min(1, Math.max(0, cell.compaction + deltaCompaction))
      this.sparseCells!.set(idx, cell)
      return
    }
    this.height[idx] += deltaHeight
    this.compaction[idx] = Math.min(1, Math.max(0, this.compaction[idx] + deltaCompaction))
  }

  applyWheelLoad(col: number, row: number, depthDelta: number, compactionDelta: number): void {
    this.applyDeformation(col, row, depthDelta, compactionDelta)
    const base = this.getBaseHeight(col, row)
    const min = base - 0.15
    const current = this.getHeight(col, row)
    if (current < min) {
      this.setHeight(col, row, min)
    }
  }

  applyGeometricSink(worldX: number, worldZ: number, targetH: number, radius: number): { col: number; row: number }[] {
    const center = this.worldToGrid(worldX, worldZ)
    if (!center) return []

    const N = Math.ceil(radius / this.cellSize)
    const centerCol = center.col
    const centerRow = center.row

    const dirtyList: { col: number; row: number }[] = []

    const minCol = Math.max(0, centerCol - N)
    const maxCol = Math.min(this.cols - 1, centerCol + N)
    const minRow = Math.max(0, centerRow - N)
    const maxRow = Math.min(this.rows - 1, centerRow + N)

    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const cellCenter = this.gridToWorld(col, row)
        if (!cellCenter) continue

        const dx = cellCenter.x - worldX
        const dz = cellCenter.z - worldZ
        const dist = Math.sqrt(dx * dx + dz * dz)
        if (dist > radius) continue

        const w = Math.max(0, 1 - dist / radius)
        const currentH = this.getHeight(col, row)
        const baseH = this.getBaseHeight(col, row)

        let newH = currentH + (targetH - currentH) * w
        newH = Math.min(currentH, newH)
        newH = Math.max(baseH - 0.15, newH)

        if (currentH - newH < 1e-6) continue

        this.setHeight(col, row, newH)
        dirtyList.push({ col, row })
      }
    }

    return dirtyList
  }

  getHeight(col: number, row: number): number {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return 0
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.height : DEFAULT_HEIGHT
    }
    return this.height[this.index(col, row)]
  }

  getHeightAtWorld(worldX: number, worldZ: number): number {
    // Shift world coords into grid-local frame before computing fractional cell position
    const colF = (worldX - this.centerX + this.fieldWidth  / 2) / this.cellSize - 0.5
    const rowF = (worldZ - this.centerZ + this.fieldLength / 2) / this.cellSize - 0.5
    const col0 = Math.floor(colF)
    const col1 = col0 + 1
    const fx = colF - col0
    const row0 = Math.floor(rowF)
    const row1 = row0 + 1
    const fz = rowF - row0
    if (
      (col0 < 0 || col0 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col0 < 0 || col0 >= this.cols || row1 < 0 || row1 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row1 < 0 || row1 >= this.rows)
    ) {
      return 0
    }
    const h00 = this.getHeight(col0, row0)
    const h10 = this.getHeight(col1, row0)
    const h01 = this.getHeight(col0, row1)
    const h11 = this.getHeight(col1, row1)
    const h0 = h00 * (1 - fx) + h10 * fx
    const h1 = h01 * (1 - fx) + h11 * fx
    return h0 * (1 - fz) + h1 * fz
  }

  // Bilinear sample of original baseHeight (without deformation accumulation).
  // Use this for chassis-tilt calculations so the vehicle doesn't tilt into its
  // own ruts — only the underlying terrain shape matters.
  getBaseHeightAtWorld(worldX: number, worldZ: number): number {
    // Shift world coords into grid-local frame before computing fractional cell position
    const colF = (worldX - this.centerX + this.fieldWidth  / 2) / this.cellSize - 0.5
    const rowF = (worldZ - this.centerZ + this.fieldLength / 2) / this.cellSize - 0.5
    const col0 = Math.floor(colF)
    const col1 = col0 + 1
    const fx = colF - col0
    const row0 = Math.floor(rowF)
    const row1 = row0 + 1
    const fz = rowF - row0
    if (
      (col0 < 0 || col0 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col0 < 0 || col0 >= this.cols || row1 < 0 || row1 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row1 < 0 || row1 >= this.rows)
    ) {
      return 0
    }
    const h00 = this.getBaseHeight(col0, row0)
    const h10 = this.getBaseHeight(col1, row0)
    const h01 = this.getBaseHeight(col0, row1)
    const h11 = this.getBaseHeight(col1, row1)
    const h0 = h00 * (1 - fx) + h10 * fx
    const h1 = h01 * (1 - fx) + h11 * fx
    return h0 * (1 - fz) + h1 * fz
  }

  getCompactionAtWorld(worldX: number, worldZ: number): number {
    return this.sampleScalarAtWorld(worldX, worldZ, (col, row) => this.getCompaction(col, row))
  }

  getMoistureAtWorld(worldX: number, worldZ: number): number {
    return this.sampleScalarAtWorld(worldX, worldZ, (col, row) => this.getMoisture(col, row))
  }

  getTilledAtWorld(worldX: number, worldZ: number): boolean {
    const grid = this.worldToGrid(worldX, worldZ)
    if (!grid) return false
    return this.getTilled(grid.col, grid.row)
  }

  getSinkDepthAtWorld(worldX: number, worldZ: number): number {
    return Math.max(0, this.getBaseHeightAtWorld(worldX, worldZ) - this.getHeightAtWorld(worldX, worldZ))
  }

  getSoilSampleAtWorld(worldX: number, worldZ: number): SoilSample {
    return {
      sinkDepth: this.getSinkDepthAtWorld(worldX, worldZ),
      moisture: this.getMoistureAtWorld(worldX, worldZ),
      compaction: this.getCompactionAtWorld(worldX, worldZ),
      tilled: this.getTilledAtWorld(worldX, worldZ),
    }
  }

  getBaseHeight(col: number, row: number): number {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return 0
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.base : DEFAULT_BASE_HEIGHT
    }
    return this.baseHeight[this.index(col, row)]
  }

  initRidges(
    fields: { polygon: [number, number][] }[],
    params?: { heightM?: number; periodM?: number; axis?: 'x' | 'z' }
  ): void {
    const heightM = params?.heightM ?? 0.25
    const periodM = params?.periodM ?? 3
    const axis: 'x' | 'z' = params?.axis ?? 'z'

    let ridgeCellCount = 0

    for (const field of fields) {
      const polygon = field.polygon
      const bbox = polygonBoundingBox(polygon)

      // Convert polygon bbox world coords to grid indices using the shifted frame
      const minCol = Math.max(0, Math.floor((bbox.minX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
      const maxCol = Math.min(this.cols - 1, Math.floor((bbox.maxX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
      const minRow = Math.max(0, Math.floor((bbox.minZ - this.centerZ + this.fieldLength / 2) / this.cellSize))
      const maxRow = Math.min(this.rows - 1, Math.floor((bbox.maxZ - this.centerZ + this.fieldLength / 2) / this.cellSize))

      for (let row = minRow; row <= maxRow; row++) {
        for (let col = minCol; col <= maxCol; col++) {
          const center = this.gridToWorld(col, row)
          if (!center) continue
          if (!pointInPolygon([center.x, center.z], polygon)) continue

          const coord = axis === 'z' ? center.z : center.x
          const baseH = heightM * 0.5 * (1 + Math.cos((2 * Math.PI * coord) / periodM))

          const idx = this.index(col, row)
          // Add ridge bump ON TOP of existing noise baseline so visual
          // amplification yields a visible ridge without destroying the
          // underlying terrain shape.
          if (this.sparse) {
            const cell = this.getOrCreateSparseCell(idx)
            cell.base = (cell.base ?? DEFAULT_BASE_HEIGHT) + baseH
            cell.height = cell.base
            this.sparseCells!.set(idx, cell)
          } else {
            const prev = this.baseHeight[idx]
            this.baseHeight[idx] = prev + baseH
            this.height[idx] = prev + baseH
          }
          ridgeCellCount++
        }
      }
    }

  }

  /**
   * Bake small noise-driven hills into the interior of each field polygon.
   * Cells within `marginM` of the boundary stay flat; from `marginM` to
   * `marginM + fadeM` the hill amplitude fades in smoothly.
   */
  initInteriorHills(
    fields: { polygon: [number, number][] }[],
    params?: { marginM?: number; fadeM?: number; heightM?: number; wavelengthM?: number }
  ): void {
    const marginM = params?.marginM ?? 5
    const fadeM = params?.fadeM ?? 3
    const heightM = params?.heightM ?? 0.4
    const wavelengthM = params?.wavelengthM ?? 14
    // simplex-noise returns [-1, 1]. freq = 1/wavelength so input units are meters.
    const freq = 1 / wavelengthM

    let hillCellCount = 0
    let peakBumpM = 0

    for (const field of fields) {
      const polygon = field.polygon
      const bbox = polygonBoundingBox(polygon)

      // Convert polygon bbox world coords to grid indices using the shifted frame
      const minCol = Math.max(0, Math.floor((bbox.minX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
      const maxCol = Math.min(this.cols - 1, Math.floor((bbox.maxX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
      const minRow = Math.max(0, Math.floor((bbox.minZ - this.centerZ + this.fieldLength / 2) / this.cellSize))
      const maxRow = Math.min(this.rows - 1, Math.floor((bbox.maxZ - this.centerZ + this.fieldLength / 2) / this.cellSize))

      for (let row = minRow; row <= maxRow; row++) {
        for (let col = minCol; col <= maxCol; col++) {
          const center = this.gridToWorld(col, row)
          if (!center) continue
          if (!pointInPolygon([center.x, center.z], polygon)) continue

          const edgeDist = pointToPolygonEdgeDistance([center.x, center.z], polygon)
          if (edgeDist <= marginM) continue

          // Fade amplitude over [marginM, marginM+fadeM].
          const fade = smoothstep(marginM, marginM + fadeM, edgeDist)

          // Two-octave fBm: large mounds plus subtle texture. Threshold negative
          // halves to zero so each peak reads as a discrete "hill" instead of
          // continuous rolling terrain.
          const n =
            hillNoise2D(center.x * freq, center.z * freq) * 0.7 +
            hillNoise2D(center.x * freq * 2.1, center.z * freq * 2.1) * 0.3
          const bump = Math.max(0, n) * heightM * fade
          if (bump <= 0) continue

          const idx = this.index(col, row)
          if (this.sparse) {
            const cell = this.getOrCreateSparseCell(idx)
            cell.base = (cell.base ?? DEFAULT_BASE_HEIGHT) + bump
            cell.height = cell.base
            this.sparseCells!.set(idx, cell)
          } else {
            this.baseHeight[idx] += bump
            this.height[idx] += bump
          }
          hillCellCount++
          if (bump > peakBumpM) peakBumpM = bump
        }
      }
    }

  }

  getHeights(): Float32Array {
    if (this.sparse) {
      // Materialize dense heights on demand (rarely used in sparse mode)
      const out = new Float32Array(this.cols * this.rows)
      if (this.sparseCells) {
        for (const [idx, cell] of this.sparseCells) {
          out[idx] = cell.height
        }
      }
      return out
    }
    return this.height
  }

  getCompaction(col: number, row: number): number {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return 0
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.compaction : DEFAULT_COMPACTION
    }
    return this.compaction[this.index(col, row)]
  }

  getMoisture(col: number, row: number): number {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return 0
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.moisture : DEFAULT_MOISTURE
    }
    return this.moisture[this.index(col, row)]
  }

  getTilled(col: number, row: number): boolean {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return false
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.tilled >= 128 : false
    }
    return this.tilled[this.index(col, row)] >= 128
  }

  // Returns raw 0-255 intensity for the tilled channel (used by texture data write).
  getTilledIntensity(col: number, row: number): number {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return 0
    if (this.sparse) {
      const idx = this.index(col, row)
      const cell = this.sparseCells!.get(idx)
      return cell ? cell.tilled : 0
    }
    return this.tilled[this.index(col, row)]
  }

  setHeight(col: number, row: number, value: number): void {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return
    if (this.sparse) {
      const center = this.gridToWorld(col, row)!
      if (!this.isInsideAnyField(center.x, center.z)) return
      const idx = this.index(col, row)
      const cell = this.getOrCreateSparseCell(idx)
      cell.height = value
      this.sparseCells!.set(idx, cell)
      return
    }
    this.height[this.index(col, row)] = value
  }

  setTilled(col: number, row: number, value: boolean): void {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return
    if (this.sparse) {
      const center = this.gridToWorld(col, row)!
      if (!this.isInsideAnyField(center.x, center.z)) return
      const idx = this.index(col, row)
      const cell = this.getOrCreateSparseCell(idx)
      cell.tilled = value ? 255 : 0
      this.sparseCells!.set(idx, cell)
      return
    }
    this.tilled[this.index(col, row)] = value ? 255 : 0
  }

  setMoisture(col: number, row: number, value: number): void {
    if (col < 0 || col >= this.cols || row < 0 || row >= this.rows) return
    if (this.sparse) {
      const center = this.gridToWorld(col, row)!
      if (!this.isInsideAnyField(center.x, center.z)) return
      const idx = this.index(col, row)
      const cell = this.getOrCreateSparseCell(idx)
      cell.moisture = value
      this.sparseCells!.set(idx, cell)
      return
    }
    this.moisture[this.index(col, row)] = value
  }

  getMemoryFootprintBytes(): number {
    if (this.sparse) {
      const size = this.sparseCells ? this.sparseCells.size : 0
      return size * SPARSE_CELL_BYTES
    }
    return this.cols * this.rows * DENSE_CELL_BYTES
  }

  serialize(): { height: string; compaction: string; moisture: string; tilled: string } {
    if (this.sparse) {
      // Materialize dense buffers for serialization compatibility
      const size = this.cols * this.rows
      const h = new Float32Array(size)
      const c = new Float32Array(size)
      const m = new Float32Array(size).fill(DEFAULT_MOISTURE)
      const t = new Uint8Array(size)
      if (this.sparseCells) {
        for (const [idx, cell] of this.sparseCells) {
          h[idx] = cell.height
          c[idx] = cell.compaction
          m[idx] = cell.moisture
          t[idx] = cell.tilled
        }
      }
      return {
        height: SoilGrid._arrayToBase64(h.buffer as ArrayBuffer),
        compaction: SoilGrid._arrayToBase64(c.buffer as ArrayBuffer),
        moisture: SoilGrid._arrayToBase64(m.buffer as ArrayBuffer),
        tilled: SoilGrid._arrayToBase64(t.buffer as ArrayBuffer),
      }
    }
    return {
      height: SoilGrid._arrayToBase64(this.height.buffer as ArrayBuffer),
      compaction: SoilGrid._arrayToBase64(this.compaction.buffer as ArrayBuffer),
      moisture: SoilGrid._arrayToBase64(this.moisture.buffer as ArrayBuffer),
      tilled: SoilGrid._arrayToBase64(this.tilled.buffer as ArrayBuffer),
    }
  }

  static deserialize(
    data: { height: string; compaction: string; moisture: string; tilled: string },
    terrainNoiseFn?: (x: number, z: number) => number
  ): SoilGrid {
    const grid = new SoilGrid(terrainNoiseFn)
    grid._loadFromSerialized(
      new Float32Array(SoilGrid._base64ToArrayBuffer(data.height)),
      new Float32Array(SoilGrid._base64ToArrayBuffer(data.compaction)),
      new Float32Array(SoilGrid._base64ToArrayBuffer(data.moisture)),
      new Uint8Array(SoilGrid._base64ToArrayBuffer(data.tilled))
    )
    return grid
  }

  private _loadFromSerialized(
    height: Float32Array,
    compaction: Float32Array,
    moisture: Float32Array,
    tilled: Uint8Array
  ): void {
    this.height.set(height)
    this.compaction.set(compaction)
    this.moisture.set(moisture)
    this.tilled.set(tilled)
  }

  private static _arrayToBase64(buffer: ArrayBuffer): string {
    const bytes = new Uint8Array(buffer)
    let binary = ''
    for (let i = 0; i < bytes.length; i++) {
      binary += String.fromCharCode(bytes[i])
    }
    return btoa(binary)
  }

  private static _base64ToArrayBuffer(base64: string): ArrayBuffer {
    const binary = atob(base64)
    const bytes = new Uint8Array(binary.length)
    for (let i = 0; i < binary.length; i++) {
      bytes[i] = binary.charCodeAt(i)
    }
    return bytes.buffer
  }

  getFieldTilledRatio(polygon: [number, number][]): number {
    const bbox = polygonBoundingBox(polygon)

    // Convert polygon bbox world coords to grid indices using the shifted frame
    const minCol = Math.max(0, Math.floor((bbox.minX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const maxCol = Math.min(this.cols - 1, Math.floor((bbox.maxX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const minRow = Math.max(0, Math.floor((bbox.minZ - this.centerZ + this.fieldLength / 2) / this.cellSize))
    const maxRow = Math.min(this.rows - 1, Math.floor((bbox.maxZ - this.centerZ + this.fieldLength / 2) / this.cellSize))

    let total = 0
    let tilledCount = 0

    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const center = this.gridToWorld(col, row)
        if (!center) continue
        if (!this._pointInPolygon([center.x, center.z], polygon)) continue

        total++

        if (this.sparse) {
          // In sparse mode, only materialized cells can be tilled
          const idx = this.index(col, row)
          const cell = this.sparseCells ? this.sparseCells.get(idx) : undefined
          if (cell && cell.tilled >= 128) tilledCount++
        } else {
          if (this.tilled[this.index(col, row)] >= 128) tilledCount++
        }
      }
    }

    if (total === 0) return 0
    return tilledCount / total
  }

  /**
   * Divide the field bounding box into `stripCount` strips along the specified
   * long axis and return the tilled-cell ratio [0..1] for each strip.
   * @param polygon   field polygon (world meters)
   * @param stripCount number of strips (e.g. 20)
   * @param longAxis  'x' or 'z' — axis along which strips are sliced
   */
  computeStripCoverage(
    polygon: [number, number][],
    stripCount: number,
    longAxis: 'x' | 'z',
  ): number[] {
    const ratios: number[] = new Array(stripCount).fill(0)
    const totals: number[] = new Array(stripCount).fill(0)
    const tilledCounts: number[] = new Array(stripCount).fill(0)

    const bbox = polygonBoundingBox(polygon)
    // Convert polygon bbox world coords to grid indices using the shifted frame
    const minCol = Math.max(0, Math.floor((bbox.minX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const maxCol = Math.min(this.cols - 1, Math.floor((bbox.maxX - this.centerX + this.fieldWidth  / 2) / this.cellSize))
    const minRow = Math.max(0, Math.floor((bbox.minZ - this.centerZ + this.fieldLength / 2) / this.cellSize))
    const maxRow = Math.min(this.rows - 1, Math.floor((bbox.maxZ - this.centerZ + this.fieldLength / 2) / this.cellSize))

    const axisMin = longAxis === 'x' ? bbox.minX : bbox.minZ
    const axisMax = longAxis === 'x' ? bbox.maxX : bbox.maxZ
    const axisRange = axisMax - axisMin

    if (axisRange <= 0) return ratios

    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        const center = this.gridToWorld(col, row)
        if (!center) continue
        if (!this._pointInPolygon([center.x, center.z], polygon)) continue

        const axisVal = longAxis === 'x' ? center.x : center.z
        const stripIdx = Math.min(
          stripCount - 1,
          Math.floor(((axisVal - axisMin) / axisRange) * stripCount),
        )
        totals[stripIdx]++

        const idx = this.index(col, row)
        const isTilled = this.sparse
          ? (this.sparseCells?.get(idx)?.tilled ?? 0) >= 128
          : this.tilled[idx] >= 128
        if (isTilled) tilledCounts[stripIdx]++
      }
    }

    for (let i = 0; i < stripCount; i++) {
      ratios[i] = totals[i] > 0 ? tilledCounts[i] / totals[i] : 0
    }
    return ratios
  }

  private _pointInPolygon(point: [number, number], polygon: [number, number][]): boolean {
    const [px, pz] = point
    let inside = false
    const n = polygon.length
    for (let i = 0, j = n - 1; i < n; j = i++) {
      const [xi, zi] = polygon[i]
      const [xj, zj] = polygon[j]
      const intersects =
        zi > pz !== zj > pz &&
        px < ((xj - xi) * (pz - zi)) / (zj - zi) + xi
      if (intersects) inside = !inside
    }
    return inside
  }

  private index(col: number, row: number): number {
    return row * this.cols + col
  }

  private sampleScalarAtWorld(
    worldX: number,
    worldZ: number,
    read: (col: number, row: number) => number,
  ): number {
    // Shift world coords into grid-local frame before computing fractional cell position
    const colF = (worldX - this.centerX + this.fieldWidth  / 2) / this.cellSize - 0.5
    const rowF = (worldZ - this.centerZ + this.fieldLength / 2) / this.cellSize - 0.5
    const col0 = Math.floor(colF)
    const row0 = Math.floor(rowF)
    const col1 = col0 + 1
    const row1 = row0 + 1
    const fx = Math.max(0, Math.min(1, colF - col0))
    const fz = Math.max(0, Math.min(1, rowF - row0))
    if (
      (col0 < 0 || col0 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row0 < 0 || row0 >= this.rows) &&
      (col0 < 0 || col0 >= this.cols || row1 < 0 || row1 >= this.rows) &&
      (col1 < 0 || col1 >= this.cols || row1 < 0 || row1 >= this.rows)
    ) {
      return 0
    }
    const v00 = read(col0, row0)
    const v10 = read(col1, row0)
    const v01 = read(col0, row1)
    const v11 = read(col1, row1)
    const v0 = v00 * (1 - fx) + v10 * fx
    const v1 = v01 * (1 - fx) + v11 * fx
    return v0 * (1 - fz) + v1 * fz
  }
}
