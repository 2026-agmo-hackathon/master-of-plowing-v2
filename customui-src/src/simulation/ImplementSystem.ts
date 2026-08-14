import type { ImplementConfig, ImplementContact, ImplementEffect } from './ImplementTypes'
import type { SoilGrid } from './SoilGrid'
import { PlowEffect } from './PlowEffect'
import type { SoilTypeName } from './SoilType'
import { getSoilProperties } from './SoilType'
import { clamp } from './TractorConfig'

export class ImplementSystem {
  private grid: SoilGrid
  private effect: ImplementEffect
  private config: ImplementConfig
  private active = false
  private lastImplementPosition: { x: number; z: number } | null = null
  private soilTypeName: SoilTypeName = 'loam'
  private lastDraftForceN = 0

  constructor(grid: SoilGrid) {
    this.grid = grid
    this.effect = new PlowEffect()  // default
    this.config = { type: 'plow', width: 1.2, depth: 0.25, hitchOffset: 3.0 }
  }

  setActive(on: boolean): void {
    this.active = on
    if (!on) this.lastImplementPosition = null
  }
  isActive(): boolean { return this.active }
  setEffect(effect: ImplementEffect): void { this.effect = effect }
  setConfig(config: ImplementConfig): void {
    this.config = config
    this.lastImplementPosition = null
  }
  getConfig(): ImplementConfig { return this.config }
  getMassKg(): number { return this.config.mass ?? 0 }
  setSoilType(name: SoilTypeName): void { this.soilTypeName = name }
  getLastDraftForceN(): number { return this.lastDraftForceN }

  update(vehicleX: number, vehicleZ: number, headingRad: number, speed: number): { col: number; row: number }[] {
    if (!this.active) {
      this.lastImplementPosition = null
      this.lastDraftForceN = 0
      return []
    }

    if (Math.abs(speed) < 0.05) {
      this.lastDraftForceN = 0
    }

    const fwdX = Math.sin(headingRad)
    const fwdZ = Math.cos(headingRad)

    const implX = vehicleX - fwdX * this.config.hitchOffset
    const implZ = vehicleZ - fwdZ * this.config.hitchOffset
    const currentPosition = { x: implX, z: implZ }

    if (this.lastImplementPosition === null) {
      this.lastImplementPosition = currentPosition
      return []
    }

    const distanceX = implX - this.lastImplementPosition.x
    const distanceZ = implZ - this.lastImplementPosition.z
    const traveledDistance = Math.hypot(distanceX, distanceZ)
    if (traveledDistance < 0.02) {
      // Do NOT update lastImplementPosition here — let small deltas accumulate
      // across frames until traveledDistance crosses the threshold. Otherwise
      // sub-threshold movement (e.g. < 0.4 m/s) never triggers tillage.
      return []
    }

    const rightX = Math.cos(headingRad)
    const rightZ = -Math.sin(headingRad)
    const halfWidth = this.config.width / 2
    const sampleSpacing = Math.max(this.grid.cellSize * 0.25, 0.125)
    const longitudinalSamples = Math.max(1, Math.ceil(traveledDistance / sampleSpacing))
    const lateralSamples = Math.max(2, Math.ceil(this.config.width / sampleSpacing))
    const sliceDistance = traveledDistance / longitudinalSamples
    const contactsByCell = new Map<string, ImplementContact>()

    for (let sampleIndex = 0; sampleIndex < longitudinalSamples; sampleIndex++) {
      const ratio = (sampleIndex + 0.5) / longitudinalSamples
      const centerX = this.lastImplementPosition.x + distanceX * ratio
      const centerZ = this.lastImplementPosition.z + distanceZ * ratio
      const cellsInSlice = new Map<string, { col: number; row: number }>()

      for (let lateralIndex = 0; lateralIndex <= lateralSamples; lateralIndex++) {
        const lateralRatio = lateralIndex / lateralSamples
        const lateralOffset = -halfWidth + lateralRatio * this.config.width
        const wx = centerX + rightX * lateralOffset
        const wz = centerZ + rightZ * lateralOffset
        const gridPos = this.grid.worldToGrid(wx, wz)
        if (gridPos) {
          cellsInSlice.set(`${gridPos.col},${gridPos.row}`, gridPos)
        }
      }

      for (const [key, gridPos] of cellsInSlice) {
        const existing = contactsByCell.get(key)
        if (existing) {
          existing.distanceMeters += sliceDistance
        } else {
          contactsByCell.set(key, {
            col: gridPos.col,
            row: gridPos.row,
            distanceMeters: sliceDistance,
          })
        }
      }
    }

    this.lastImplementPosition = currentPosition
    const allContacts = Array.from(contactsByCell.values())
    const contacts = allContacts.filter((c) => this.grid.isFieldCell(c.col, c.row))
    if (contacts.length === 0) {
      this.lastDraftForceN = 0
      return []
    }

    this.effect.apply(this.grid, contacts, speed, this.config)

    if (Math.abs(speed) < 0.05) {
      this.lastDraftForceN = 0
    } else {
      const soilProps = getSoilProperties(this.soilTypeName)
      const baseN = soilProps.draftBaseN
      let multiplierSum = 0
      for (const { col, row } of contacts) {
        const compaction = this.grid.getCompaction(col, row)
        const moisture = this.grid.getMoisture(col, row)
        const tilled = this.grid.getTilled(col, row)
        const m = (1 + compaction * 0.2) * (1 + moisture * 0.15) * (tilled ? 0.7 : 1.0)
        multiplierSum += m
      }
      const cellAvg = multiplierSum / contacts.length
      const speedFactor = clamp(Math.abs(speed) / 2, 0.3, 1.5)
      const rotaryFactor = this.config.type === 'rotary' ? 0.6 : 1.0
      this.lastDraftForceN = baseN * (this.config.width / 1.0) * (this.config.depth / 0.25) * speedFactor * cellAvg * rotaryFactor
    }

    return contacts.map(({ col, row }) => ({ col, row }))
  }
}
