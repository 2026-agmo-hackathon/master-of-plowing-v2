import { describe, expect, it } from 'vitest'
import { ImplementSystem } from '../ImplementSystem'
import { RotaryEffect } from '../RotaryEffect'
import { SoilGrid } from '../SoilGrid'
import type { ImplementConfig } from '../ImplementTypes'

const PLOW_CONFIG: ImplementConfig = { type: 'plow', width: 1.2, depth: 0.25, hitchOffset: 3.0 }
const ROTARY_CONFIG: ImplementConfig = { type: 'rotary', width: 1.2, depth: 0.25, hitchOffset: 3.0 }

// A large rectangle covering the full field so isFieldCell returns true
const FULL_FIELD_POLYGON: [number, number][] = [[-100, -200], [100, -200], [100, 200], [-100, 200]]

function createSystem(active: boolean, config: ImplementConfig = PLOW_CONFIG): { grid: SoilGrid; system: ImplementSystem } {
  const grid = new SoilGrid(undefined, { fields: [{ polygon: FULL_FIELD_POLYGON }] })
  const system = new ImplementSystem(grid)
  system.setConfig(config)
  system.setSoilType('loam')
  system.setActive(active)
  return { grid, system }
}

describe('ImplementSystem.getLastDraftForceN', () => {
  it('plow active, speed=1.5, loam -> draft > 500 N', () => {
    // Draft is computed after PlowEffect.apply() marks contacts as tilled,
    // so the tilled multiplier (0.7) is applied. With cellSize=0.25m the
    // expected steady-state draft is ~790 N; threshold updated accordingly.
    const { system } = createSystem(true)
    system.update(0, 0, 0, 1.5)
    system.update(0, 1, 0, 1.5)
    expect(system.getLastDraftForceN()).toBeGreaterThan(500)
  })

  it('plow active, speed=0 -> draft = 0', () => {
    const { system } = createSystem(true)
    system.update(0, 0, 0, 1.5)
    system.update(0, 1, 0, 0)
    expect(system.getLastDraftForceN()).toBe(0)
  })

  it('plow inactive -> draft = 0', () => {
    const { system } = createSystem(false)
    system.update(0, 0, 0, 1.5)
    system.update(0, 1, 0, 1.5)
    expect(system.getLastDraftForceN()).toBe(0)
  })

  it('rotary draft is approximately 0.6x of plow draft on identical cells (within 5%)', () => {
    const plow = createSystem(true, PLOW_CONFIG)
    plow.system.update(0, 0, 0, 1.5)
    plow.system.update(0, 1, 0, 1.5)
    const plowN = plow.system.getLastDraftForceN()

    const rotary = createSystem(true, ROTARY_CONFIG)
    rotary.system.setEffect(new RotaryEffect())
    rotary.system.update(0, 0, 0, 1.5)
    rotary.system.update(0, 1, 0, 1.5)
    const rotaryN = rotary.system.getLastDraftForceN()

    const ratio = rotaryN / plowN
    expect(ratio).toBeGreaterThan(0.6 * 0.95)
    expect(ratio).toBeLessThan(0.6 * 1.05)
  })

  it('higher compaction cell yields larger draft than zero-compaction cell', () => {
    const lowGrid = new SoilGrid(undefined, { fields: [{ polygon: FULL_FIELD_POLYGON }] })
    const lowSystem = new ImplementSystem(lowGrid)
    lowSystem.setConfig(PLOW_CONFIG)
    lowSystem.setSoilType('loam')
    lowSystem.setActive(true)
    // Establish prior position; second call generates contacts.
    lowSystem.update(0, 0, 0, 1.5)
    lowSystem.update(0, 1, 0, 1.5)
    const lowN = lowSystem.getLastDraftForceN()

    const highGrid = new SoilGrid(undefined, { fields: [{ polygon: FULL_FIELD_POLYGON }] })
    // Pre-set compaction across the full implement footprint area.
    for (let col = 0; col < highGrid.cols; col++) {
      for (let row = 0; row < highGrid.rows; row++) {
        const center = highGrid.gridToWorld(col, row)
        if (!center) continue
        if (Math.abs(center.x) <= 2 && center.z >= -4 && center.z <= 0) {
          highGrid.setCellCompaction(center.x, center.z, 0.5)
        }
      }
    }
    const highSystem = new ImplementSystem(highGrid)
    highSystem.setConfig(PLOW_CONFIG)
    highSystem.setSoilType('loam')
    highSystem.setActive(true)
    highSystem.update(0, 0, 0, 1.5)
    highSystem.update(0, 1, 0, 1.5)
    const highN = highSystem.getLastDraftForceN()

    expect(highN).toBeGreaterThan(lowN)
  })
})
