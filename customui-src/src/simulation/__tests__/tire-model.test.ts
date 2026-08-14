import { describe, expect, it } from 'vitest'
import {
  magicFormula,
  computeSlipRatio,
  computeSlipAngle,
  computeRollingResistance,
  computeTireForces,
} from '../TireModel'
import type { TireParams } from '../TireModel'

const defaultParams: TireParams = { B: 10, C: 1.9, D: 6000, E: 0.97 }

describe('magicFormula', () => {
  it('returns 0 for zero slip', () => {
    expect(magicFormula(0, defaultParams)).toBe(0)
  })

  it('returns positive force for positive slip', () => {
    expect(magicFormula(0.1, defaultParams)).toBeGreaterThan(0)
  })

  it('returns negative force for negative slip', () => {
    expect(magicFormula(-0.1, defaultParams)).toBeLessThan(0)
  })

  it('reaches near peak D at slip ~0.15', () => {
    const force = magicFormula(0.15, defaultParams)
    // Should be close to D (6000), within 10%
    expect(force).toBeGreaterThan(5400)
    expect(force).toBeLessThanOrEqual(6000)
  })
})

describe('computeSlipRatio', () => {
  it('returns 0 when both stationary', () => {
    expect(computeSlipRatio(0, 0.5, 0)).toBe(0)
  })

  it('returns positive for wheelspin (omega*r > v)', () => {
    // omega=20, r=0.5 => wheelSpeed=10, v=5
    expect(computeSlipRatio(20, 0.5, 5)).toBeGreaterThan(0)
  })

  it('returns negative for braking (omega*r < v)', () => {
    // omega=5, r=0.5 => wheelSpeed=2.5, v=10
    expect(computeSlipRatio(5, 0.5, 10)).toBeLessThan(0)
  })
})

describe('computeSlipAngle', () => {
  it('returns 0 for straight driving (no lateral velocity)', () => {
    expect(computeSlipAngle(0, 10)).toBe(0)
  })

  it('returns nonzero with lateral velocity', () => {
    expect(computeSlipAngle(2, 10)).not.toBe(0)
  })
})

describe('computeRollingResistance', () => {
  it('computes Fz * Crr correctly', () => {
    expect(computeRollingResistance(10000, 0.04)).toBe(400)
  })
})

describe('computeTireForces', () => {
  const config = { B: 10, C: 1.9, E: 0.97 }

  it('returns near-zero forces when stationary', () => {
    const forces = computeTireForces(0, 0, 0, 5000, 0.8, 0.5, config)
    expect(Math.abs(forces.longitudinal)).toBeLessThan(1)
    expect(Math.abs(forces.lateral)).toBeLessThan(1)
    expect(forces.slipRatio).toBe(0)
    expect(forces.slipAngle).toBe(0)
  })

  it('produces positive longitudinal force during acceleration (wheelspin)', () => {
    // omega*r > vLong => positive slip ratio => positive longitudinal
    const forces = computeTireForces(30, 5, 0, 5000, 0.8, 0.5, config)
    expect(forces.longitudinal).toBeGreaterThan(0)
    expect(forces.slipRatio).toBeGreaterThan(0)
  })

  it('produces nonzero lateral force during cornering', () => {
    const forces = computeTireForces(20, 10, 3, 5000, 0.8, 0.5, config)
    expect(forces.lateral).not.toBe(0)
    expect(forces.slipAngle).not.toBe(0)
  })
})
