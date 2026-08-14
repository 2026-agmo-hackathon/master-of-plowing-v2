import { describe, expect, it } from 'vitest'
import type { TerrainRegion } from '../MapDefinition'
import { largeShape, mediumFbm, smallFbm, smoothstep } from '../TerrainNoise'

describe('TerrainNoise.mediumFbm', () => {
  it('1. is deterministic — same coord returns same value across calls', () => {
    const a = mediumFbm(12.5, -7.25)
    const b = mediumFbm(12.5, -7.25)
    expect(a).toBe(b)

    const c = mediumFbm(0, 0)
    const d = mediumFbm(0, 0)
    expect(c).toBe(d)
  })

  it('2. amplitude stays within ~0.3m bound across sampled coords', () => {
    const samples: Array<[number, number]> = [
      [0, 0],
      [10, 10],
      [-10, -10],
      [25, -3],
      [-50, 50],
      [100, 100],
      [-100, 200],
      [33.3, 77.7],
      [1, -1],
      [500, -500],
    ]
    let maxAbs = 0
    for (const [x, z] of samples) {
      const v = mediumFbm(x, z)
      if (Math.abs(v) > maxAbs) maxAbs = Math.abs(v)
    }
    expect(maxAbs).toBeLessThan(0.4)
  })
})

describe('TerrainNoise.smallFbm', () => {
  it('3. is deterministic and stays within ~0.05m bound', () => {
    const a = smallFbm(3.14, 2.71)
    const b = smallFbm(3.14, 2.71)
    expect(a).toBe(b)

    const samples: Array<[number, number]> = [
      [0, 0],
      [1, 1],
      [-1, -1],
      [5, 5],
      [10, -10],
      [50, 50],
      [-50, 50],
      [100, 100],
      [25, 25],
      [-25, -25],
    ]
    let maxAbs = 0
    for (const [x, z] of samples) {
      const v = smallFbm(x, z)
      if (Math.abs(v) > maxAbs) maxAbs = Math.abs(v)
    }
    expect(maxAbs).toBeLessThan(0.08)
  })
})

describe('TerrainNoise.smoothstep', () => {
  it('4. clamps at edges and is monotone in between', () => {
    expect(smoothstep(0, 1, -0.5)).toBe(0)
    expect(smoothstep(0, 1, 0)).toBe(0)
    expect(smoothstep(0, 1, 1)).toBe(1)
    expect(smoothstep(0, 1, 1.5)).toBe(1)
    expect(smoothstep(0, 1, 0.5)).toBeCloseTo(0.5, 5)
    const a = smoothstep(0, 1, 0.25)
    const b = smoothstep(0, 1, 0.75)
    expect(a).toBeGreaterThan(0)
    expect(a).toBeLessThan(b)
    expect(b).toBeLessThan(1)
  })
})

describe('TerrainNoise.largeShape', () => {
  it('5. returns 0 when regions is undefined or empty', () => {
    expect(largeShape(5, 5, undefined)).toBe(0)
    expect(largeShape(5, 5, [])).toBe(0)
  })

  it('6. returns 0 outside polygon', () => {
    const regions: TerrainRegion[] = [
      {
        type: 'ramp',
        polygon: [
          [0, 0],
          [100, 0],
          [100, 100],
          [0, 100],
        ],
        slopeRad: 0.1,
        axis: 'x',
        featherM: 5,
      },
    ]
    expect(largeShape(-10, 50, regions)).toBe(0)
    expect(largeShape(200, 50, regions)).toBe(0)
    expect(largeShape(50, -10, regions)).toBe(0)
    expect(largeShape(50, 200, regions)).toBe(0)
  })

  it('7. ramp produces increasing height along axis when fully inside (blend ~1)', () => {
    const regions: TerrainRegion[] = [
      {
        type: 'ramp',
        polygon: [
          [0, 0],
          [100, 0],
          [100, 100],
          [0, 100],
        ],
        slopeRad: 0.1,
        axis: 'x',
        featherM: 5,
      },
    ]
    // Deep inside (>= featherM from any edge), blend should be ~1
    const hNear = largeShape(20, 50, regions)
    const hFar = largeShape(80, 50, regions)
    expect(hFar).toBeGreaterThan(hNear)
    // Approximately tan(0.1) * 80 at fully blended deep interior
    expect(hFar).toBeCloseTo(Math.tan(0.1) * 80, 1)
  })

  it('8. featherM transition zone produces blend between 0 and 1 near edge', () => {
    const regions: TerrainRegion[] = [
      {
        type: 'ramp',
        polygon: [
          [0, 0],
          [100, 0],
          [100, 100],
          [0, 100],
        ],
        slopeRad: 0.1,
        axis: 'x',
        featherM: 10,
      },
    ]
    // Just inside edge at x=1 (1m from edge, featherM=10)
    const hEdge = largeShape(1, 50, regions)
    const hMid = largeShape(50, 50, regions)
    // Edge sample should be smaller than mid sample due to smaller blend AND smaller localCoord
    expect(hEdge).toBeLessThan(hMid)
    expect(hEdge).toBeGreaterThan(0)
    // Compare to fully blended ramp at same x
    const fullBlendH = Math.tan(0.1) * 1
    expect(hEdge).toBeLessThan(fullBlendH)
  })

  it('9. ridge type produces sinusoidal pattern within polygon', () => {
    const regions: TerrainRegion[] = [
      {
        type: 'ridge',
        polygon: [
          [0, 0],
          [100, 0],
          [100, 100],
          [0, 100],
        ],
        periodM: 10,
        heightM: 0.2,
        axis: 'x',
        featherM: 5,
      },
    ]
    // Deep inside: blend ~1. cos(2π*x/10) cycles every 10m.
    // At x=0: cos(0) = 1 → height = 0.2
    // At x=5: cos(π) = -1 → height = 0
    const hPeak = largeShape(50, 50, regions)
    const hValley = largeShape(55, 50, regions)
    expect(hPeak).toBeGreaterThan(hValley)
    // hPeak at x=50: 2π*50/10 = 10π → cos = 1 → 0.2 * blend
    expect(hPeak).toBeCloseTo(0.2, 2)
    // hValley at x=55: 2π*55/10 = 11π → cos = -1 → 0
    expect(hValley).toBeCloseTo(0, 2)
  })
})
