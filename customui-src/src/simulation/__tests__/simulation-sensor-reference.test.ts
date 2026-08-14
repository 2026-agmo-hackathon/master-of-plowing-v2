/// <reference types="node" />

import { readFileSync } from 'node:fs'
import { fileURLToPath } from 'node:url'
import { describe, expect, it } from 'vitest'

interface Point2D {
  x: number
  y: number
}

interface LegacySimulationSensorSample {
  eastingM: number
  northingM: number
  headingDeg: number
  speedMps: number
  // Empirical binary phase flag from the legacy dump:
  // 0 stays close to simulation_path.txt, 1 is the off-path transition/headland phase.
  pathPhaseFlag: 0 | 1
  steerMotorCmdRaw: number
  gyroZRadPerSec: number
}

interface DistanceStats {
  avg: number
  p95: number
  max: number
}

function normalizeAngleDifference(angleDeg: number): number {
  let normalized = angleDeg
  while (normalized > 180) normalized -= 360
  while (normalized < -180) normalized += 360
  return normalized
}

const DATA_DIR = new URL(
  '../../../../references/seamos-agmo-solution/agmo_solution/disk/simulation/',
  import.meta.url,
)

function readLines(name: string): string[] {
  return readFileSync(fileURLToPath(new URL(name, DATA_DIR)), 'utf8')
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter(Boolean)
}

function parsePoint2DFile(name: string): Point2D[] {
  return readLines(name).map((line) => {
    const [x, y] = line.split(/\s+/).map(Number)
    return { x, y }
  })
}

function parseLegacySimulationSensor(): LegacySimulationSensorSample[] {
  return readLines('simulation_sensor.txt').map((line) => {
    const [eastingM, northingM, headingDeg, speedMps, pathPhaseFlag, steerMotorCmdRaw, gyroZRadPerSec] =
      line.split(/\s+/).map(Number)

    return {
      eastingM,
      northingM,
      headingDeg,
      speedMps,
      pathPhaseFlag: pathPhaseFlag as 0 | 1,
      steerMotorCmdRaw,
      gyroZRadPerSec,
    }
  })
}

function distanceToNearestPathPoint(sample: LegacySimulationSensorSample, path: Point2D[]): number {
  let best = Number.POSITIVE_INFINITY

  for (const point of path) {
    const distance = Math.hypot(sample.eastingM - point.x, sample.northingM - point.y)
    if (distance < best) {
      best = distance
    }
  }

  return best
}

function summarizeDistances(distances: number[]): DistanceStats {
  const sorted = [...distances].sort((left, right) => left - right)
  const avg = distances.reduce((sum, value) => sum + value, 0) / distances.length
  const p95Index = Math.min(sorted.length - 1, Math.floor(sorted.length * 0.95))

  return {
    avg,
    p95: sorted[p95Index],
    max: sorted[sorted.length - 1],
  }
}

describe('legacy simulation sensor reference', () => {
  it('keeps the legacy dump format stable', () => {
    const samples = parseLegacySimulationSensor()
    const path = parsePoint2DFile('simulation_path.txt')
    const boundary = parsePoint2DFile('simulation_boundary.txt')

    expect(samples).toHaveLength(6738)
    expect(path).toHaveLength(850)
    expect(boundary).toHaveLength(5)

    const first = samples[0]
    expect(first.eastingM).toBeCloseTo(370913.5566, 4)
    expect(first.northingM).toBeCloseTo(193134.2696, 4)
    expect(first.headingDeg).toBeCloseTo(183.495038, 6)
    expect(first.speedMps).toBeCloseTo(0.137, 3)
    expect(first.pathPhaseFlag).toBe(1)
    expect(first.steerMotorCmdRaw).toBe(50)
    expect(first.gyroZRadPerSec).toBeCloseTo(-0.062317, 6)
  })

  it('maps the seven sensor fields into bounded driving signals', () => {
    const samples = parseLegacySimulationSensor()
    const phaseValues = [...new Set(samples.map((sample) => sample.pathPhaseFlag))].sort()

    expect(phaseValues).toEqual([0, 1])

    for (const sample of samples) {
      expect(Number.isFinite(sample.eastingM)).toBe(true)
      expect(Number.isFinite(sample.northingM)).toBe(true)
      expect(sample.headingDeg).toBeGreaterThanOrEqual(0)
      expect(sample.headingDeg).toBeLessThan(360)
      expect(sample.speedMps).toBeGreaterThanOrEqual(0)
      expect(sample.speedMps).toBeLessThan(1)
      expect(sample.steerMotorCmdRaw).toBeGreaterThanOrEqual(-1000)
      expect(sample.steerMotorCmdRaw).toBeLessThanOrEqual(1000)
      expect(Math.abs(sample.gyroZRadPerSec)).toBeLessThan(0.5)
    }
  })

  it('stays inside the simulation boundary envelope', () => {
    const samples = parseLegacySimulationSensor()
    const path = parsePoint2DFile('simulation_path.txt')
    const boundary = parsePoint2DFile('simulation_boundary.txt')

    const xs = boundary.map((point) => point.x)
    const ys = boundary.map((point) => point.y)
    const minX = Math.min(...xs)
    const maxX = Math.max(...xs)
    const minY = Math.min(...ys)
    const maxY = Math.max(...ys)

    for (const point of path) {
      expect(point.x).toBeGreaterThanOrEqual(minX)
      expect(point.x).toBeLessThanOrEqual(maxX)
      expect(point.y).toBeGreaterThanOrEqual(minY)
      expect(point.y).toBeLessThanOrEqual(maxY)
    }

    for (const sample of samples) {
      expect(sample.eastingM).toBeGreaterThanOrEqual(minX)
      expect(sample.eastingM).toBeLessThanOrEqual(maxX)
      expect(sample.northingM).toBeGreaterThanOrEqual(minY)
      expect(sample.northingM).toBeLessThanOrEqual(maxY)
    }
  })

  it('uses the binary phase flag to separate path tracking from transition motion', () => {
    const samples = parseLegacySimulationSensor()
    const path = parsePoint2DFile('simulation_path.txt')

    const onPathDistances: number[] = []
    const transitionDistances: number[] = []

    for (const sample of samples) {
      const distance = distanceToNearestPathPoint(sample, path)
      if (sample.pathPhaseFlag === 0) {
        onPathDistances.push(distance)
      } else {
        transitionDistances.push(distance)
      }
    }

    const onPath = summarizeDistances(onPathDistances)
    const transition = summarizeDistances(transitionDistances)

    expect(onPath.avg).toBeLessThan(1)
    expect(onPath.p95).toBeLessThan(2)
    expect(onPath.max).toBeLessThan(4)

    expect(transition.avg).toBeGreaterThan(10)
    expect(transition.p95).toBeGreaterThan(20)
    expect(transition.max).toBeGreaterThan(25)
  })

  it('uses an east-zero counter-clockwise heading convention', () => {
    const samples = parseLegacySimulationSensor()
    const headingErrorsDeg: number[] = []

    for (let index = 1; index < samples.length; index += 1) {
      const previous = samples[index - 1]
      const current = samples[index]
      const dx = current.eastingM - previous.eastingM
      const dy = current.northingM - previous.northingM
      if (Math.abs(dx) + Math.abs(dy) < 1e-6) continue

      const motionHeadingDeg = ((Math.atan2(dy, dx) * 180) / Math.PI + 360) % 360
      headingErrorsDeg.push(Math.abs(normalizeAngleDifference(current.headingDeg - motionHeadingDeg)))
    }

    const sorted = headingErrorsDeg.sort((left, right) => left - right)
    const p50 = sorted[Math.floor(sorted.length * 0.5)]
    const p95 = sorted[Math.floor(sorted.length * 0.95)]

    expect(p50).toBeLessThan(1)
    expect(p95).toBeLessThan(15)
  })
})
