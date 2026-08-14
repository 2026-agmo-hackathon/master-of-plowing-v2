import { describe, expect, it } from 'vitest'
import { SoilGrid } from '../SoilGrid'

describe('SoilGrid sparse/dense', () => {
  it('1. dense mode (no fields) behaves like the original grid', () => {
    const grid = new SoilGrid()
    // A known in-bounds point at grid origin
    const cell = grid.getCell(0, 0)
    expect(cell).not.toBeNull()
    expect(cell!.compaction).toBe(0)
    expect(cell!.moisture).toBeCloseTo(0.3, 5)
    expect(cell!.tilled).toBe(false)

    // write + read
    grid.setCellCompaction(10, 20, 0.55)
    const c = grid.getCell(10, 20)
    expect(c!.compaction).toBeCloseTo(0.55, 5)

    // every in-bounds position returns a cell (not null)
    expect(grid.getCell(-80, -180)).not.toBeNull()
    expect(grid.getCell(80, 180)).not.toBeNull()
    // out-of-bounds returns null
    expect(grid.getCell(-200, 0)).toBeNull()
  })

  it('2. sparse mode (200x400m field) stores only inside-field disturbances; outside is no-op', () => {
    // Rectangle field: [-100,-200]-[100,200] → 200×400m
    const polygon: [number, number][] = [
      [-100, -200],
      [100, -200],
      [100, 200],
      [-100, 200],
    ]
    const grid = new SoilGrid(undefined, { fields: [{ polygon }] })

    // Inside field: write is stored
    grid.setCellCompaction(0, 0, 0.7)
    const inside = grid.getCell(0, 0)
    expect(inside).not.toBeNull()
    expect(inside!.compaction).toBeCloseTo(0.7, 5)

    // Inside with applyDeformation
    const g = grid.worldToGrid(50, 50)!
    grid.applyDeformation(g.col, g.row, -0.05, 0.2)
    const deformed = grid.getCell(50, 50)
    expect(deformed!.height).toBeCloseTo(-0.05, 5)
    expect(deformed!.compaction).toBeCloseTo(0.2, 5)

    // Outside field: write is no-op, read returns defaults
    grid.setCellCompaction(300, 300, 0.9)
    const outside = grid.getCell(300, 300)
    expect(outside).not.toBeNull()
    expect(outside!.compaction).toBe(0) // default
    expect(outside!.moisture).toBeCloseTo(0.3, 5)
    expect(outside!.tilled).toBe(false)

    // Outside applyDeformation is no-op too
    const g2 = grid.worldToGrid(300, 300)!
    grid.applyDeformation(g2.col, g2.row, 0.5, 0.5)
    const stillDefault = grid.getCell(300, 300)
    expect(stillDefault!.height).toBe(0)
    expect(stillDefault!.compaction).toBe(0)

    // getSoilAt alias behaves like getCell
    const viaAlias = grid.getSoilAt(0, 0)
    expect(viaAlias!.compaction).toBeCloseTo(0.7, 5)
  })

  it('3. memory footprint: sparse saves >=40% vs dense for a 200x400m field', () => {
    const polygon: [number, number][] = [
      [-100, -200],
      [100, -200],
      [100, 200],
      [-100, 200],
    ]
    const dense = new SoilGrid()
    const sparse = new SoilGrid(undefined, { fields: [{ polygon }] })

    // Apply disturbance on a coarse grid inside field (every 10m → 20x40 = 800 cells)
    for (let x = -100; x < 100; x += 10) {
      for (let z = -200; z < 200; z += 10) {
        sparse.setCellCompaction(x + 0.5, z + 0.5, 0.4)
      }
    }

    const denseBytes = dense.getMemoryFootprintBytes()
    const sparseBytes = sparse.getMemoryFootprintBytes()

    expect(denseBytes).toBeGreaterThan(0)
    expect(sparseBytes).toBeGreaterThan(0)

    const savings = 1 - sparseBytes / denseBytes
    expect(savings).toBeGreaterThanOrEqual(0.4)
  })
})

describe('SoilGrid initRidges (TODO 1)', () => {
  const polygon: [number, number][] = [[-80, -100], [80, -100], [80, 80], [-80, 80]]

  it('내부 셀 baseHeight > 0 (cos 최대 근처)', () => {
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })
    g.initRidges([{ polygon }], { heightM: 0.25, periodM: 3, axis: 'z' })
    const at0 = g.worldToGrid(0, 3)
    expect(at0).not.toBeNull()
    const h = g.getBaseHeight(at0!.col, at0!.row)
    expect(h).toBeGreaterThan(0.2)
    expect(h).toBeLessThanOrEqual(0.25 + 1e-6)
  })

  it('폴리곤 외부 셀 baseHeight = 0', () => {
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })
    g.initRidges([{ polygon }], { heightM: 0.25, periodM: 3, axis: 'z' })
    const outside = g.worldToGrid(300, 300)
    if (outside) {
      expect(g.getBaseHeight(outside.col, outside.row)).toBe(0)
    }
  })

  it('initRidges 후 currentHeight == baseHeight', () => {
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })
    g.initRidges([{ polygon }], { heightM: 0.25, periodM: 3, axis: 'z' })
    const at0 = g.worldToGrid(0, 0)!
    expect(g.getHeight(at0.col, at0.row)).toBeCloseTo(g.getBaseHeight(at0.col, at0.row), 5)
  })
})

describe('SoilGrid applyWheelLoad (TODO 2)', () => {
  const polygon: [number, number][] = [[-80, -100], [80, -100], [80, 80], [-80, 80]]

  it('내부 셀 반복 호출 시 baseHeight - 0.15 로 하한 clamp', () => {
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })
    g.initRidges([{ polygon }], { heightM: 0.25, periodM: 3, axis: 'z' })
    const cell = g.worldToGrid(0, 3)!
    const base = g.getBaseHeight(cell.col, cell.row)
    for (let i = 0; i < 1000; i++) g.applyWheelLoad(cell.col, cell.row, -0.05, 0.05)
    const h = g.getHeight(cell.col, cell.row)
    expect(h).toBeGreaterThanOrEqual(base - 0.15 - 1e-6)
    expect(g.getCompaction(cell.col, cell.row)).toBe(1)
  })

  it('폴리곤 외부 호출 no-op', () => {
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })
    g.initRidges([{ polygon }], { heightM: 0.25, periodM: 3, axis: 'z' })
    const outside = g.worldToGrid(300, 300)
    if (outside) {
      const beforeHeight = g.getHeight(outside.col, outside.row)
      g.applyWheelLoad(outside.col, outside.row, -0.1, 0.5)
      expect(g.getHeight(outside.col, outside.row)).toBe(beforeHeight)
    }
  })
})

describe('SoilGrid getHeightAtWorld (TODO 8)', () => {
  it('정확한 셀 중심 위치는 해당 셀의 height와 동일', () => {
    const g = new SoilGrid()
    const col = 10
    const row = 20
    g.setHeight(col, row, 1.5)
    const center = g.gridToWorld(col, row)!
    const h = g.getHeightAtWorld(center.x, center.z)
    expect(h).toBeCloseTo(1.5, 5)
  })

  it('두 셀 사이 중간 위치는 두 셀 평균', () => {
    const g = new SoilGrid()
    const col = 10
    const row = 20
    g.setHeight(col, row, 1.0)
    g.setHeight(col + 1, row, 3.0)
    const c0 = g.gridToWorld(col, row)!
    const c1 = g.gridToWorld(col + 1, row)!
    const midX = (c0.x + c1.x) / 2
    const h = g.getHeightAtWorld(midX, c0.z)
    expect(h).toBeCloseTo(2.0, 5)
  })

  it('그리드 밖 좌표는 0 반환', () => {
    const g = new SoilGrid()
    const h = g.getHeightAtWorld(-10000, -10000)
    expect(h).toBe(0)
  })
})

describe('SoilGrid soil sampling helpers', () => {
  it('getSinkDepthAtWorld returns positive base-current depth only', () => {
    const g = new SoilGrid()
    const cell = g.worldToGrid(0, 0)!
    const center = g.gridToWorld(cell.col, cell.row)!
    g.setHeight(cell.col, cell.row, -0.08)
    expect(g.getSinkDepthAtWorld(center.x, center.z)).toBeCloseTo(0.08, 5)

    g.setHeight(cell.col, cell.row, 0.08)
    expect(g.getSinkDepthAtWorld(center.x, center.z)).toBe(0)
  })

  it('getSoilSampleAtWorld includes moisture, compaction, and tilled state', () => {
    const g = new SoilGrid()
    const cell = g.worldToGrid(0, 0)!
    const center = g.gridToWorld(cell.col, cell.row)!
    g.setCellCompaction(center.x, center.z, 0.4)
    g.setMoisture(cell.col, cell.row, 0.7)
    g.setTilled(cell.col, cell.row, true)
    g.setHeight(cell.col, cell.row, -0.05)

    const sample = g.getSoilSampleAtWorld(center.x, center.z)
    expect(sample.sinkDepth).toBeCloseTo(0.05, 5)
    expect(sample.compaction).toBeCloseTo(0.4, 5)
    expect(sample.moisture).toBeCloseTo(0.7, 5)
    expect(sample.tilled).toBe(true)
  })
})

describe('SoilGrid applyGeometricSink', () => {
  it('1. 중심 셀이 targetH로 정확히 lerp (w=1.0)', () => {
    const g = new SoilGrid()
    // Pick a cell near the grid center
    const cell = g.worldToGrid(0, 0)!
    g.setHeight(cell.col, cell.row, 0.10)
    const center = g.gridToWorld(cell.col, cell.row)!

    // Calling exactly at cell center → dist=0, w=1 → newH = targetH
    const dirty = g.applyGeometricSink(center.x, center.z, -0.05, 0.4)

    expect(g.getHeight(cell.col, cell.row)).toBeCloseTo(-0.05, 4)
    expect(dirty.some((d) => d.col === cell.col && d.row === cell.row)).toBe(true)
  })

  it('2. 거리 감쇠 lerp (dist=0.05, radius=0.4 → w=0.875)', () => {
    // cellSize=0.25, so adjacent cell center is 0.25 m away (inside radius=0.4).
    // Shift the sink origin 0.2 m in the +x direction from the cell center so
    // the same cell is now 0.2 m away and the next cell is 0.25-0.2=0.05 m away.
    const g = new SoilGrid()
    const cell = g.worldToGrid(0, 0)!
    const adjCell = { col: cell.col + 1, row: cell.row }

    // Set both cells to height=0.10
    g.setHeight(cell.col, cell.row, 0.10)
    g.setHeight(adjCell.col, adjCell.row, 0.10)

    const center = g.gridToWorld(cell.col, cell.row)!
    // Sink origin shifted +0.2 m → adjacent cell center is 0.25-0.2=0.05 m away
    const sinkX = center.x + 0.2
    const sinkZ = center.z

    // w_adj = 1 - 0.05/0.4 = 0.875
    // newH_adj = 0.10 + (-0.05 - 0.10) * 0.875 = -0.03125
    g.applyGeometricSink(sinkX, sinkZ, -0.05, 0.4)

    expect(g.getHeight(adjCell.col, adjCell.row)).toBeCloseTo(-0.03125, 4)
  })

  it('3. baseHeight - 0.15 하한 클램프', () => {
    // Default baseHeight=0. setHeight to 0.10 then apply very low targetH.
    // w=1 → lerp result = -0.30, but clamped to max(0-0.15, -0.30) = -0.15
    const g = new SoilGrid()
    const cell = g.worldToGrid(0, 0)!
    g.setHeight(cell.col, cell.row, 0.10)
    const center = g.gridToWorld(cell.col, cell.row)!

    const dirty = g.applyGeometricSink(center.x, center.z, -0.30, 0.4)

    expect(g.getHeight(cell.col, cell.row)).toBeCloseTo(-0.15, 4)
    expect(dirty.some((d) => d.col === cell.col && d.row === cell.row)).toBe(true)
  })

  it('4. sparse mode 외부 셀: setHeight no-op → 실제 height 변경 없음', () => {
    const polygon: [number, number][] = [
      [-100, -200],
      [100, -200],
      [100, 200],
      [-100, 200],
    ]
    const g = new SoilGrid(undefined, { fields: [{ polygon }] })

    // Apply sink far outside the polygon
    // Any cells touched will have their setHeight silently rejected
    g.applyGeometricSink(300, 300, -0.05, 0.4)

    // All cells near (300,300) should remain at default height (0)
    const nearCell = g.worldToGrid(300, 300)!
    expect(g.getHeight(nearCell.col, nearCell.row)).toBe(0)
  })

  it('5. targetH > currentH の場合: 変化なし (dirtyList 비어있음)', () => {
    // targetH > currentH → newH = currentH + (targetH-currentH)*w > currentH
    // min(currentH, newH) = currentH, so currentH - newH = 0 < 1e-6 → skipped
    const g = new SoilGrid()
    const cell = g.worldToGrid(0, 0)!
    g.setHeight(cell.col, cell.row, 0.10)
    const center = g.gridToWorld(cell.col, cell.row)!

    const dirty = g.applyGeometricSink(center.x, center.z, 0.20, 0.4)

    expect(g.getHeight(cell.col, cell.row)).toBeCloseTo(0.10, 4)
    expect(dirty.length).toBe(0)
  })
})
