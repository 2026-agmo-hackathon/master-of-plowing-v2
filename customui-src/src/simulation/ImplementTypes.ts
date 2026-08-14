import type { SoilGrid } from './SoilGrid'

export type ImplementConfig = {
  type: 'plow' | 'rotary'
  width: number
  depth: number
  hitchOffset: number
  mass?: number
}

export type ImplementContact = {
  col: number
  row: number
  distanceMeters: number
}

export interface ImplementEffect {
  apply(grid: SoilGrid, contacts: ImplementContact[], speed: number, config: ImplementConfig): void
}

export interface SimulationConfig {
  sinkSensitivity: number    // default 0.0001
  compactionRate: number     // default 0.01
  moistureFactor: number     // default 1.5
  soilHardness: number       // default 1.0
  plowDepth: number          // default 0.25
  rotaryDepth: number        // default 0.15
  plowWidth: number          // default 1.2
  rotaryWidth: number        // default 2.0
}

export const DEFAULT_SIM_CONFIG: SimulationConfig = {
  sinkSensitivity: 0.0001,
  compactionRate: 0.01,
  moistureFactor: 1.5,
  soilHardness: 1.0,
  plowDepth: 0.25,
  rotaryDepth: 0.15,
  plowWidth: 1.2,
  rotaryWidth: 2.0,
}
