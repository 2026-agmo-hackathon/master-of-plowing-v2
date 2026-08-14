export type QualityPresetName = 'high' | 'medium' | 'low' | 'super-low'

export interface QualityPresetSpec {
  name: QualityPresetName
  shadow: {
    enabled: boolean
    cascades: number
    shadowMapSize: number
  }
  post: {
    enabled: boolean
    bloom: boolean
    ssao: boolean
    antialias: boolean
  }
  pixelRatioCap: number
  terrainSegments: { x: number; z: number }
  decalResolution: { w: number; h: number }
  tireDecal: boolean
  cameraFar: number
  fogDensity: number
  shadowEnabled: boolean
}

export const QUALITY_PRESETS: Record<QualityPresetName, QualityPresetSpec> = {
  high: {
    name: 'high',
    shadow: { enabled: true, cascades: 3, shadowMapSize: 4096 },
    post: { enabled: true, bloom: true, ssao: true, antialias: true },
    pixelRatioCap: 2,
    terrainSegments: { x: 600, z: 1200 },
    decalResolution: { w: 2048, h: 4096 },
    tireDecal: true,
    cameraFar: 1000,
    fogDensity: 0.0012,
    shadowEnabled: true,
  },
  medium: {
    name: 'medium',
    shadow: { enabled: true, cascades: 3, shadowMapSize: 2048 },
    post: { enabled: true, bloom: true, ssao: false, antialias: true },
    pixelRatioCap: 1.5,
    terrainSegments: { x: 600, z: 1200 },
    decalResolution: { w: 2048, h: 4096 },
    tireDecal: true,
    cameraFar: 1000,
    fogDensity: 0.0012,
    shadowEnabled: true,
  },
  low: {
    name: 'low',
    shadow: { enabled: false, cascades: 1, shadowMapSize: 1024 },
    post: { enabled: false, bloom: false, ssao: false, antialias: false },
    pixelRatioCap: 1,
    terrainSegments: { x: 600, z: 1200 },
    decalResolution: { w: 2048, h: 4096 },
    tireDecal: true,
    cameraFar: 1000,
    fogDensity: 0.0012,
    shadowEnabled: false,
  },
  'super-low': {
    name: 'super-low',
    shadow: { enabled: false, cascades: 1, shadowMapSize: 1024 },
    post: { enabled: false, bloom: false, ssao: false, antialias: false },
    pixelRatioCap: 0.75,
    terrainSegments: { x: 150, z: 300 },
    decalResolution: { w: 512, h: 1024 },
    tireDecal: true,
    cameraFar: 300,
    fogDensity: 0.006,
    shadowEnabled: false,
  },
}

const LS_KEY = 'simulator.qualityPreset'

export function loadQualityPreset(): QualityPresetName {
  if (typeof localStorage === 'undefined') return 'low'
  const v = localStorage.getItem(LS_KEY)
  if (v === 'high' || v === 'medium' || v === 'low' || v === 'super-low') return v
  return 'low'
}

export function saveQualityPreset(name: QualityPresetName): void {
  if (typeof localStorage !== 'undefined') localStorage.setItem(LS_KEY, name)
}
