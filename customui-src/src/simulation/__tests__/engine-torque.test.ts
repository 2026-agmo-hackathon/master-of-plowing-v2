import { describe, expect, it } from 'vitest'
import {
  interpolateTorque,
  computeGovernedTorque,
  updateEngineRpm,
} from '../EngineTorque'
import { TRACTOR_CONFIG } from '../TractorConfig'

const curve = TRACTOR_CONFIG.engineTorqueCurve

describe('interpolateTorque', () => {
  it('returns exact torque at defined points', () => {
    expect(interpolateTorque(850, curve)).toBe(160)
    expect(interpolateTorque(1400, curve)).toBe(220)
    expect(interpolateTorque(2100, curve)).toBe(180)
  })

  it('linearly interpolates between points', () => {
    const result = interpolateTorque(1100, curve)
    // 1100 is between 850 (160) and 1200 (210)
    expect(result).toBeGreaterThan(160)
    expect(result).toBeLessThan(210)
    // exact: 160 + (210-160) * (1100-850)/(1200-850) = 160 + 50 * 250/350 ≈ 195.71
    expect(result).toBeCloseTo(195.71, 1)
  })

  it('returns 0 for rpm below curve minimum', () => {
    expect(interpolateTorque(500, curve)).toBe(0)
  })

  it('returns 0 for rpm above curve maximum', () => {
    expect(interpolateTorque(2500, curve)).toBe(0)
  })
})

describe('computeGovernedTorque', () => {
  it('returns 0 when throttle is 0', () => {
    expect(computeGovernedTorque(1400, 0, curve)).toBe(0)
  })

  it('returns full torque when throttle is 1', () => {
    expect(computeGovernedTorque(1400, 1, curve)).toBe(220)
  })

  it('returns half torque when throttle is 0.5', () => {
    expect(computeGovernedTorque(1400, 0.5, curve)).toBe(110)
  })
})

describe('updateEngineRpm', () => {
  const inertia = TRACTOR_CONFIG.engineInertiaKgm2
  const idleRpm = TRACTOR_CONFIG.idleRpm
  const stallRpm = TRACTOR_CONFIG.stallRpm

  it('increases RPM with positive net torque', () => {
    const result = updateEngineRpm(1200, 100, inertia, 0.05, idleRpm, stallRpm)
    expect(result.rpm).toBeGreaterThan(1200)
    expect(result.stalled).toBe(false)
  })

  it('decreases RPM with negative net torque', () => {
    const result = updateEngineRpm(1200, -50, inertia, 0.05, idleRpm, stallRpm)
    expect(result.rpm).toBeLessThan(1200)
    expect(result.stalled).toBe(false)
  })

  it('returns stalled when rpm drops below stallRpm', () => {
    // Large negative torque to push below stall threshold
    const result = updateEngineRpm(650, -500, inertia, 0.1, idleRpm, stallRpm)
    expect(result.stalled).toBe(true)
    expect(result.rpm).toBe(0)
  })
})
