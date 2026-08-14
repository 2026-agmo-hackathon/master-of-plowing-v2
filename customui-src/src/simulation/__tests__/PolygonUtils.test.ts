import { describe, expect, it } from 'vitest'
import { pointInPolygon, polygonBoundingBox } from '../PolygonUtils'

describe('PolygonUtils.pointInPolygon', () => {
  it('1. Convex triangle: inside point returns true', () => {
    const triangle: [number, number][] = [
      [0, 0],
      [10, 0],
      [5, 10],
    ]
    expect(pointInPolygon([5, 3], triangle)).toBe(true)
  })

  it('2. Convex triangle: outside point returns false', () => {
    const triangle: [number, number][] = [
      [0, 0],
      [10, 0],
      [5, 10],
    ]
    expect(pointInPolygon([20, 20], triangle)).toBe(false)
    expect(pointInPolygon([-1, -1], triangle)).toBe(false)
  })

  it('3. Square polygon: near-corner interior points classify correctly', () => {
    const square: [number, number][] = [
      [0, 0],
      [10, 0],
      [10, 10],
      [0, 10],
    ]
    // slightly inside each corner
    expect(pointInPolygon([0.1, 0.1], square)).toBe(true)
    expect(pointInPolygon([9.9, 0.1], square)).toBe(true)
    expect(pointInPolygon([9.9, 9.9], square)).toBe(true)
    expect(pointInPolygon([0.1, 9.9], square)).toBe(true)
    // slightly outside each corner
    expect(pointInPolygon([-0.1, -0.1], square)).toBe(false)
    expect(pointInPolygon([10.1, 10.1], square)).toBe(false)
  })

  it('4. Concave L-shape: inside arm point returns true', () => {
    // L-shape: 10x10 outer, carved notch from (5,5)-(10,10)
    const lShape: [number, number][] = [
      [0, 0],
      [10, 0],
      [10, 5],
      [5, 5],
      [5, 10],
      [0, 10],
    ]
    // bottom-left arm
    expect(pointInPolygon([2, 2], lShape)).toBe(true)
    // bottom-right arm
    expect(pointInPolygon([8, 2], lShape)).toBe(true)
    // top-left arm
    expect(pointInPolygon([2, 8], lShape)).toBe(true)
  })

  it('5. Concave L-shape: point inside the notch returns false', () => {
    const lShape: [number, number][] = [
      [0, 0],
      [10, 0],
      [10, 5],
      [5, 5],
      [5, 10],
      [0, 10],
    ]
    // in the carved-out notch (upper-right) → outside the polygon
    expect(pointInPolygon([8, 8], lShape)).toBe(false)
    expect(pointInPolygon([7, 7], lShape)).toBe(false)
  })

  it('6. polygonBoundingBox returns tight bbox', () => {
    const poly: [number, number][] = [
      [-5, -3],
      [4, -3],
      [4, 7],
      [-5, 7],
    ]
    const bbox = polygonBoundingBox(poly)
    expect(bbox.minX).toBe(-5)
    expect(bbox.maxX).toBe(4)
    expect(bbox.minZ).toBe(-3)
    expect(bbox.maxZ).toBe(7)
  })
})
