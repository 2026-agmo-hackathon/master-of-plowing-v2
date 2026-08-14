export interface HeightmapRoughness {
  amplitude: number   // meters of peak deviation
  wavelengthM: number // characteristic horizontal length in meters
  seed?: number       // for deterministic noise; default 1
}

export type HeightmapSource =
  | { type: 'fn'; fn: 'flat'; roughness?: HeightmapRoughness }
  | { type: 'fn'; fn: 'single-hill'; roughness?: HeightmapRoughness }
  | { type: 'fn'; fn: 'tilted-plane'; axis: 'x' | '-x' | 'z' | '-z'; slopeRad: number; roughness?: HeightmapRoughness }
  | { type: 'png'; url: string; scale?: number }

export type SplatmapSource =
  | { type: 'procedural'; kind: 'default' | 'composite-field' }
  | { type: 'png'; url: string }

export type TextureLayer = {
  splatChannel: 'r' | 'g' | 'b' | 'a'
  diffuse: string  // texture URL or procedural key 'dirt' | 'grass' | 'cultivated' | 'road'
  normal?: string
  tileSize: number
}

export type MapField = {
  id: string
  polygon: [number, number][]  // [[x, z], ...] minimum 3 points
  crop?: 'rice' | 'wheat' | 'corn'
}

export type MapRoad = {
  polyline: [number, number][]
  width: number
}

export type MapDecorItem = {
  type: 'tree' | 'fence' | 'barn' | 'silo'
  position: [number, number]
  rotation?: number  // radians
  scale?: number
}

export type TerrainRegion =
  | {
      type: 'ramp'
      polygon: [number, number][]    // [[x, z], ...] minimum 3 points
      slopeRad: number               // ramp slope (positive = rising along axis direction)
      axis: 'x' | '-x' | 'z' | '-z'  // direction of ascent
      featherM: number               // smoothstep transition distance at edges (m)
    }
  | {
      type: 'ridge'
      polygon: [number, number][]    // [[x, z], ...] minimum 3 points
      periodM: number                // ridge spacing (m)
      heightM: number                // ridge amplitude (m)
      axis: 'x' | 'z'                // ridge progression axis
      featherM: number               // smoothstep transition distance at edges (m)
    }

import type { SoilTypeName } from './SoilType'

export interface MapDefinition {
  schemaVersion: 1
  id: string
  label: string
  size: {
    width: number
    height: number
    centerX?: number   // ENU bbox center X (scene meters); 0 if absent (legacy)
    centerZ?: number   // ENU bbox center Z
  }
  heightmap: HeightmapSource
  splatmap?: SplatmapSource
  textureLayers: TextureLayer[]
  fields?: MapField[]
  roads?: MapRoad[]
  decor?: MapDecorItem[]
  regions?: TerrainRegion[]
  originLatLng?: [number, number]
  /** Optional default SoilType for the whole map. If omitted, GameLoop falls back to 'loam'. */
  soilType?: SoilTypeName
}
