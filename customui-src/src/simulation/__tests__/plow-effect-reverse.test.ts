import { describe, it, expect, beforeEach } from 'vitest'
import { SoilGrid } from '../SoilGrid'
import { PlowEffect } from '../PlowEffect'
import type { ImplementConfig } from '../ImplementTypes'

describe('PlowEffect reverse unplow', () => {
  let grid: SoilGrid
  let effect: PlowEffect
  let config: ImplementConfig

  beforeEach(() => {
    grid = new SoilGrid()
    effect = new PlowEffect()
    config = {
      type: 'plow',
      width: 1.2,
      depth: 0.25,
      hitchOffset: 0.5,
    }
  })

  it('forward sets tilled=true and changes height', () => {
    const contacts = [{ col: 5, row: 5, distanceMeters: 0.5 }]
    const heightBefore = grid.getHeight(5, 5)
    expect(grid.getTilled(5, 5)).toBe(false)

    effect.apply(grid, contacts, 2, config)

    expect(grid.getTilled(5, 5)).toBe(true)
    expect(grid.getHeight(5, 5)).not.toBe(heightBefore)
  })

  it('reverse sets tilled=false and does NOT change height', () => {
    grid.setTilled(5, 5, true)
    const heightBefore = grid.getHeight(5, 5)
    const contacts = [{ col: 5, row: 5, distanceMeters: 0.5 }]

    effect.apply(grid, contacts, -2, config)

    expect(grid.getTilled(5, 5)).toBe(false)
    expect(grid.getHeight(5, 5)).toBe(heightBefore)
  })

  it('forward then reverse on same cell ends up tilled=false', () => {
    const contacts = [{ col: 5, row: 5, distanceMeters: 0.5 }]

    // Forward pass
    effect.apply(grid, contacts, 2, config)
    expect(grid.getTilled(5, 5)).toBe(true)

    const heightAfterForward = grid.getHeight(5, 5)

    // Reverse pass
    effect.apply(grid, contacts, -2, config)

    expect(grid.getTilled(5, 5)).toBe(false)
    expect(grid.getHeight(5, 5)).toBe(heightAfterForward)
  })

  it('reverse on already-untilled cell is no-op', () => {
    expect(grid.getTilled(5, 5)).toBe(false)
    const contacts = [{ col: 5, row: 5, distanceMeters: 0.5 }]
    const heightBefore = grid.getHeight(5, 5)

    effect.apply(grid, contacts, -2, config)

    expect(grid.getTilled(5, 5)).toBe(false)
    expect(grid.getHeight(5, 5)).toBe(heightBefore)
  })
})
