/** Standard ray-cast point-in-polygon (x-axis ray). Handles concave + holes via winding. */
export function pointInPolygon(point: [number, number], polygon: [number, number][]): boolean {
  const [x, y] = point
  let inside = false
  for (let i = 0, j = polygon.length - 1; i < polygon.length; j = i++) {
    const xi = polygon[i][0], yi = polygon[i][1]
    const xj = polygon[j][0], yj = polygon[j][1]
    const intersect = (yi > y) !== (yj > y) &&
      x < ((xj - xi) * (y - yi)) / (yj - yi + 1e-12) + xi
    if (intersect) inside = !inside
  }
  return inside
}

export function polygonBoundingBox(polygon: [number, number][]): { minX: number; minZ: number; maxX: number; maxZ: number } {
  let minX = Infinity, minZ = Infinity, maxX = -Infinity, maxZ = -Infinity
  for (const [x, z] of polygon) {
    if (x < minX) minX = x
    if (x > maxX) maxX = x
    if (z < minZ) minZ = z
    if (z > maxZ) maxZ = z
  }
  return { minX, minZ, maxX, maxZ }
}

/** Shortest Euclidean distance from a point to any polygon edge (closed ring). */
export function pointToPolygonEdgeDistance(point: [number, number], polygon: [number, number][]): number {
  const [px, py] = point
  let minDistSq = Infinity
  for (let i = 0, j = polygon.length - 1; i < polygon.length; j = i++) {
    const x1 = polygon[j][0], y1 = polygon[j][1]
    const x2 = polygon[i][0], y2 = polygon[i][1]
    const dx = x2 - x1, dy = y2 - y1
    const lenSq = dx * dx + dy * dy
    let t = lenSq > 0 ? ((px - x1) * dx + (py - y1) * dy) / lenSq : 0
    if (t < 0) t = 0
    else if (t > 1) t = 1
    const cx = x1 + t * dx, cy = y1 + t * dy
    const ddx = px - cx, ddy = py - cy
    const dsq = ddx * ddx + ddy * ddy
    if (dsq < minDistSq) minDistSq = dsq
  }
  return Math.sqrt(minDistSq)
}
