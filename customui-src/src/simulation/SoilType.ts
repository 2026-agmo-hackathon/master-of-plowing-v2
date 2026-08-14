import { TRACTOR_CONFIG } from './TractorConfig'

export interface SoilProperties {
  friction: number
  rollingResistance: number
  sinkSensitivity: number
  compactionRate: number
  plowResistanceNm: number
  draftBaseN: number
}

export type SoilTypeName = 'sandy' | 'loam' | 'clay' | 'wet' | 'hardpack' | 'weedy'

/**
 * Soil type as a behaviour-bearing object. New types are added by extending
 * SoilType, registering in SOIL_TYPES, and (optionally) referencing them from
 * a MapDefinition's `soilType` field.
 *
 * Base properties (friction, rollingResistance, ...) are kept compatible with
 * the legacy `SoilProperties` shape so existing call sites that destructured
 * those fields continue to work.
 */
export abstract class SoilType implements SoilProperties {
  abstract readonly id: SoilTypeName
  abstract readonly displayName: string
  abstract readonly friction: number
  abstract readonly rollingResistance: number
  abstract readonly sinkSensitivity: number
  abstract readonly compactionRate: number
  abstract readonly plowResistanceNm: number
  abstract readonly draftBaseN: number
  /** When false, wheels and implements do not deform soil (rock-hard ground). */
  readonly deformable: boolean = true
}

class SandySoil extends SoilType {
  readonly id = 'sandy' as const
  readonly displayName = 'Sandy'
  readonly friction = TRACTOR_CONFIG.soilTypes.sandy.friction
  readonly rollingResistance = TRACTOR_CONFIG.soilTypes.sandy.rollingResistance
  readonly sinkSensitivity = TRACTOR_CONFIG.soilTypes.sandy.sinkSensitivity
  readonly compactionRate = TRACTOR_CONFIG.soilTypes.sandy.compactionRate
  readonly plowResistanceNm = TRACTOR_CONFIG.soilTypes.sandy.plowResistanceNm
  readonly draftBaseN = TRACTOR_CONFIG.soilTypes.sandy.draftBaseN
}

class LoamSoil extends SoilType {
  readonly id = 'loam' as const
  readonly displayName = 'Loam'
  readonly friction = TRACTOR_CONFIG.soilTypes.loam.friction
  readonly rollingResistance = TRACTOR_CONFIG.soilTypes.loam.rollingResistance
  readonly sinkSensitivity = TRACTOR_CONFIG.soilTypes.loam.sinkSensitivity
  readonly compactionRate = TRACTOR_CONFIG.soilTypes.loam.compactionRate
  readonly plowResistanceNm = TRACTOR_CONFIG.soilTypes.loam.plowResistanceNm
  readonly draftBaseN = TRACTOR_CONFIG.soilTypes.loam.draftBaseN
}

class ClaySoil extends SoilType {
  readonly id = 'clay' as const
  readonly displayName = 'Clay'
  readonly friction = TRACTOR_CONFIG.soilTypes.clay.friction
  readonly rollingResistance = TRACTOR_CONFIG.soilTypes.clay.rollingResistance
  readonly sinkSensitivity = TRACTOR_CONFIG.soilTypes.clay.sinkSensitivity
  readonly compactionRate = TRACTOR_CONFIG.soilTypes.clay.compactionRate
  readonly plowResistanceNm = TRACTOR_CONFIG.soilTypes.clay.plowResistanceNm
  readonly draftBaseN = TRACTOR_CONFIG.soilTypes.clay.draftBaseN
}

class WetSoil extends SoilType {
  readonly id = 'wet' as const
  readonly displayName = 'Wet'
  readonly friction = TRACTOR_CONFIG.soilTypes.wet.friction
  readonly rollingResistance = TRACTOR_CONFIG.soilTypes.wet.rollingResistance
  readonly sinkSensitivity = TRACTOR_CONFIG.soilTypes.wet.sinkSensitivity
  readonly compactionRate = TRACTOR_CONFIG.soilTypes.wet.compactionRate
  readonly plowResistanceNm = TRACTOR_CONFIG.soilTypes.wet.plowResistanceNm
  readonly draftBaseN = TRACTOR_CONFIG.soilTypes.wet.draftBaseN
}

/**
 * Hardpack: rock-hard ground that does not deform under wheels or implements.
 * Higher friction than loam (more grip on dry compacted earth). Used by maps
 * that should behave like a static mesh — e.g. `single-hill`.
 */
class HardpackSoil extends SoilType {
  readonly id = 'hardpack' as const
  readonly displayName = 'Hardpack'
  readonly friction = 0.75
  readonly rollingResistance = 0.025
  readonly sinkSensitivity = 0
  readonly compactionRate = 0
  readonly plowResistanceNm = 400
  readonly draftBaseN = 6000
  override readonly deformable = false
}

class WeedySoil extends SoilType {
  readonly id = 'weedy' as const
  readonly displayName = 'Weedy'
  readonly friction = TRACTOR_CONFIG.soilTypes.weedy.friction
  readonly rollingResistance = TRACTOR_CONFIG.soilTypes.weedy.rollingResistance
  readonly sinkSensitivity = TRACTOR_CONFIG.soilTypes.weedy.sinkSensitivity
  readonly compactionRate = TRACTOR_CONFIG.soilTypes.weedy.compactionRate
  readonly plowResistanceNm = TRACTOR_CONFIG.soilTypes.weedy.plowResistanceNm
  readonly draftBaseN = TRACTOR_CONFIG.soilTypes.weedy.draftBaseN
}

export const SOIL_TYPES: Record<SoilTypeName, SoilType> = {
  sandy: new SandySoil(),
  loam: new LoamSoil(),
  clay: new ClaySoil(),
  wet: new WetSoil(),
  hardpack: new HardpackSoil(),
  weedy: new WeedySoil(),
}

export function getSoilType(id: SoilTypeName): SoilType {
  return SOIL_TYPES[id]
}

/** Legacy shape — same object, narrower view. Kept for back-compat. */
export function getSoilProperties(name: SoilTypeName): SoilProperties {
  return SOIL_TYPES[name]
}
