import { createNoise2D } from 'simplex-noise'
import type { TerrainRegion } from './MapDefinition'
import { pointInPolygon, polygonBoundingBox } from './PolygonUtils'

// Seed 42 fixed — alea PRNG or simple LCG. simplex-noise v4 accepts a RandomFn.
function seededRandom(seed: number): () => number {
  let state = seed
  return () => {
    state = (state * 1664525 + 1013904223) | 0
    return ((state >>> 0) / 0xffffffff)
  }
}

const noiseMedium = createNoise2D(seededRandom(42))
const noiseSmall = createNoise2D(seededRandom(13))

export function smoothstep(edge0: number, edge1: number, x: number): number {
  const t = Math.max(0, Math.min(1, (x - edge0) / (edge1 - edge0)))
  return t * t * (3 - 2 * t)
}

// 3-octave fBm, amplitude 0.3m, frequency 0.02
export function mediumFbm(x: number, z: number): number {
  let total = 0
  let amplitude = 1
  let frequency = 0.02
  let max = 0
  for (let i = 0; i < 3; i++) {
    total += noiseMedium(x * frequency, z * frequency) * amplitude
    max += amplitude
    amplitude *= 0.5
    frequency *= 2
  }
  return (total / max) * 0.3
}

// 5-octave fBm, amplitude 0.05m, frequency 0.5
export function smallFbm(x: number, z: number): number {
  let total = 0
  let amplitude = 1
  let frequency = 0.5
  let max = 0
  for (let i = 0; i < 5; i++) {
    total += noiseSmall(x * frequency, z * frequency) * amplitude
    max += amplitude
    amplitude *= 0.5
    frequency *= 2
  }
  return (total / max) * 0.05
}

// Shortest distance from polygon to edge (positive inside the cell, 0 outside).
// Fast approximation: bbox check + pointInPolygon.
function distanceToPolygonEdge(x: number, z: number, polygon: [number, number][]): number {
  // If inside pointInPolygon → distance to closest edge; outside → 0.
  if (!pointInPolygon([x, z], polygon)) return 0
  let minDist = Infinity
  for (let i = 0; i < polygon.length; i++) {
    const [ax, az] = polygon[i]
    const [bx, bz] = polygon[(i + 1) % polygon.length]
    const dx = bx - ax, dz = bz - az
    const lenSq = dx * dx + dz * dz
    if (lenSq === 0) continue
    let t = ((x - ax) * dx + (z - az) * dz) / lenSq
    t = Math.max(0, Math.min(1, t))
    const cx = ax + t * dx, cz = az + t * dz
    const ex = x - cx, ez = z - cz
    const d = Math.sqrt(ex * ex + ez * ez)
    if (d < minDist) minDist = d
  }
  return minDist === Infinity ? 0 : minDist
}

// Apply ramp + ridge from regions. Inside polygon + featherM transition zone uses smoothstep blend.
export function largeShape(x: number, z: number, regions: TerrainRegion[] | undefined): number {
  if (!regions || regions.length === 0) return 0
  let total = 0
  for (const r of regions) {
    const dEdge = distanceToPolygonEdge(x, z, r.polygon)
    if (dEdge <= 0) continue
    const blend = smoothstep(0, r.featherM, dEdge)
    if (r.type === 'ramp') {
      // Ramp local axis displacement → height
      let coord: number
      if (r.axis === 'x') coord = x
      else if (r.axis === '-x') coord = -x
      else if (r.axis === 'z') coord = z
      else coord = -z
      // Use bbox start coord as 0 so ramp begins at 0 at polygon start
      const bbox = polygonBoundingBox(r.polygon)
      let origin: number
      if (r.axis === 'x') origin = bbox.minX
      else if (r.axis === '-x') origin = -bbox.maxX
      else if (r.axis === 'z') origin = bbox.minZ
      else origin = -bbox.maxZ
      const localCoord = coord - origin
      const rampH = Math.tan(r.slopeRad) * Math.max(0, localCoord)
      total += rampH * blend
    } else {
      // Ridge: sine wave
      const coord = r.axis === 'z' ? z : x
      const ridgeH = r.heightM * 0.5 * (1 + Math.cos((2 * Math.PI * coord) / r.periodM))
      total += ridgeH * blend
    }
  }
  return total
}
