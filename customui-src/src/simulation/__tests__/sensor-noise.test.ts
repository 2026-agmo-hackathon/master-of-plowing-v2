import { describe, expect, it } from 'vitest'
import { SensorNoise, gaussianRandom } from '../SensorNoise'

describe('SensorNoise', () => {
  it('with stdDev=0 and drift=0, apply returns exact clean value', () => {
    const noise = new SensorNoise(0, 0)
    expect(noise.apply(5.0, 0.05)).toBe(5.0)
    expect(noise.apply(-3.2, 1.0)).toBe(-3.2)
    expect(noise.apply(0, 0.1)).toBe(0)
  })

  it('with stdDev=1.0, mean is close to clean value and stddev is close to 1.0', () => {
    const noise = new SensorNoise(1.0, 0)
    const cleanValue = 10.0
    const samples: number[] = []
    for (let i = 0; i < 1000; i++) {
      samples.push(noise.apply(cleanValue, 0.05))
    }
    const mean = samples.reduce((a, b) => a + b, 0) / samples.length
    const variance = samples.reduce((a, b) => a + (b - mean) ** 2, 0) / samples.length
    const stddev = Math.sqrt(variance)

    expect(mean).toBeCloseTo(cleanValue, 0)
    expect(Math.abs(mean - cleanValue)).toBeLessThan(0.1)
    expect(Math.abs(stddev - 1.0)).toBeLessThan(0.1)
  })

  it('bias drifts after many calls', () => {
    const noise = new SensorNoise(0.1, 0.01)
    for (let i = 0; i < 500; i++) {
      noise.apply(0, 1.0)
    }
    expect(noise.getBias()).not.toBe(0)
  })

  it('resetBias() sets bias back to 0', () => {
    const noise = new SensorNoise(0.1, 0.01)
    for (let i = 0; i < 100; i++) {
      noise.apply(0, 1.0)
    }
    expect(noise.getBias()).not.toBe(0)
    noise.resetBias()
    expect(noise.getBias()).toBe(0)
  })
})

describe('gaussianRandom', () => {
  it('10000 samples have mean ~0 and stddev ~1', () => {
    const samples: number[] = []
    for (let i = 0; i < 10000; i++) {
      samples.push(gaussianRandom())
    }
    const mean = samples.reduce((a, b) => a + b, 0) / samples.length
    const variance = samples.reduce((a, b) => a + (b - mean) ** 2, 0) / samples.length
    const stddev = Math.sqrt(variance)

    expect(Math.abs(mean)).toBeLessThan(0.05)
    expect(Math.abs(stddev - 1.0)).toBeLessThan(0.1)
  })
})
