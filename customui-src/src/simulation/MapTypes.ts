import { createNoise2D } from 'simplex-noise'

export type MapType = 'flat'

export type TerrainNoiseFn = (x: number, z: number) => number

export function flatTerrainNoise(_x: number, _z: number): number {
  return 0
}

export function singleHillTerrainNoise(x: number, z: number): number {
  // Forward (+Z): broad gentle base + steeper peak (sum of two Gaussians).
  // - Outer region: ~2-3° (easy 1단 climb)
  // - Mid region (r~20m): ~22-25°
  // - Near peak (r~12m): ~32° (just above static-friction limit on loam,
  //   so the tractor slides back if it stalls there)
  const centerX = 0
  const centerZ = 45
  const dx = x - centerX
  const dz = z - centerZ

  // Wide gentle base
  const broadR = 40
  const broadMound = Math.exp(-((dx * dx) / (2 * broadR * broadR) + (dz * dz) / (2 * broadR * broadR)))
  const broadH = broadMound * 3

  // Sharp top — concentrates the steep slope near the summit
  const peakR = 12
  const peakMound = Math.exp(-((dx * dx) / (2 * peakR * peakR) + (dz * dz) / (2 * peakR * peakR)))
  const peakH = peakMound * 12

  const hillHeight = broadH + peakH

  // Left (-X) descending slope at ~15° starting from the spawn line (x=0).
  // +X side stays flat.
  const SLOPE_RAD = 15 * Math.PI / 180
  const leftDescent = x < 0 ? Math.tan(SLOPE_RAD) * x : 0

  return hillHeight + leftDescent
}

export function tiltedPlaneTerrainNoise(
  axis: 'x' | '-x' | 'z' | '-z',
  slopeRad: number,
): TerrainNoiseFn {
  const slope = Math.tan(slopeRad)
  return (x: number, z: number) => {
    switch (axis) {
      case 'x':  return slope * x
      case '-x': return -slope * x
      case 'z':  return slope * z
      case '-z': return -slope * z
    }
  }
}

// Simple seeded RNG (mulberry32) so seeds give deterministic noise.
function mulberry32(seed: number): () => number {
  let s = seed >>> 0
  return function() {
    s = (s + 0x6D2B79F5) >>> 0
    let t = s
    t = Math.imul(t ^ (t >>> 15), t | 1)
    t ^= t + Math.imul(t ^ (t >>> 7), t | 61)
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296
  }
}

export function withRoughness(
  base: TerrainNoiseFn,
  amplitude: number,
  wavelengthM: number,
  seed: number = 1,
): TerrainNoiseFn {
  if (amplitude <= 0 || wavelengthM <= 0) return base
  const rng = mulberry32(seed)
  const n2 = createNoise2D(rng)
  const invWL = 1 / wavelengthM
  return (x: number, z: number) => base(x, z) + n2(x * invWL, z * invWL) * amplitude
}

export function getTerrainNoiseFn(_mapType: MapType): TerrainNoiseFn {
  return flatTerrainNoise
}
