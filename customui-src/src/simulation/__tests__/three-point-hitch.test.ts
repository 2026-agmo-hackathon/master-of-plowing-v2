import { describe, expect, it } from 'vitest'
import { ThreePointHitch } from '../ThreePointHitch'

describe('ThreePointHitch', () => {
  it('starts at position 0 (raised)', () => {
    const hitch = new ThreePointHitch()
    expect(hitch.getPosition()).toBe(0)
  })

  it('lowers to position ~1 after 2 seconds', () => {
    const hitch = new ThreePointHitch()
    hitch.setTarget(1)
    // liftSpeed = 0.5 pos/s => 2s for full travel
    const dt = 0.01
    for (let i = 0; i < 200; i++) {
      hitch.update(dt)
    }
    expect(hitch.getPosition()).toBeCloseTo(1, 2)
  })

  it('raises back to position ~0 after 2 seconds', () => {
    const hitch = new ThreePointHitch()
    // First lower fully
    hitch.setTarget(1)
    for (let i = 0; i < 200; i++) {
      hitch.update(0.01)
    }
    expect(hitch.getPosition()).toBeCloseTo(1, 2)

    // Now raise
    hitch.setTarget(0)
    for (let i = 0; i < 200; i++) {
      hitch.update(0.01)
    }
    expect(hitch.getPosition()).toBeCloseTo(0, 2)
  })

  it('computes weight transfer at position=1: rearDelta > 0, frontDelta < 0', () => {
    const hitch = new ThreePointHitch()
    hitch.setTarget(1)
    for (let i = 0; i < 200; i++) {
      hitch.update(0.01)
    }

    const wt = hitch.computeWeightTransfer(500, 1.5, 2.05)
    expect(wt.rearLoadDelta).toBeGreaterThan(0)
    expect(wt.frontLoadDelta).toBeLessThan(0)
    expect(wt.pitchDelta).toBeLessThan(0)
  })

  it('computes weight transfer at position=0: both deltas ~0', () => {
    const hitch = new ThreePointHitch()
    // position stays 0
    const wt = hitch.computeWeightTransfer(500, 1.5, 2.05)
    expect(wt.rearLoadDelta).toBeCloseTo(0)
    expect(wt.frontLoadDelta).toBeCloseTo(0)
    expect(wt.pitchDelta).toBeCloseTo(0, 4)
  })
})
