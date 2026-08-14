import { describe, expect, it } from 'vitest'
import { KinematicVehicle } from '../KinematicVehicle'
import { TRACTOR_CONFIG, clamp } from '../TractorConfig'

const DEG2RAD = Math.PI / 180
const RAD2DEG = 180 / Math.PI
const PHYSICS_DT = 1 / 60
const CONTROLLER_DT = 0.1
const MAX_MOTOR_CMD_RPM = 500
const HEADING_GAIN = 1.1
const STANLEY_GAIN = 1.35
const CONTROL_SPEED_SOFTENER_MPS = 0.6
const TARGET_FRONT_WHEEL_SLEW_DEG_PER_SEC = 80
const MOTOR_KP = 2.6
const MOTOR_KI = 0.025
const MOTOR_KD = 0.08
const MOTOR_INTEGRAL_LIMIT = 1200
const STABLE_WINDOW = 150
const STABLE_LATERAL_THRESHOLD_M = 0.25
const STABLE_HEADING_THRESHOLD_DEG = 20

type PathSample = {
  lateralDeviationM: number
  headingErrorDeg: number
}

interface PathModel {
  sample(x: number, z: number, headingRad: number): PathSample
}

interface ScenarioConfig {
  durationSec: number
  throttle: number
  path: PathModel
  initialX?: number
  initialZ?: number
  initialHeadingDeg?: number
  initialSpeedMps?: number
  feedbackLossAtSec?: number
}

interface ScenarioResult {
  finalSpeedMps: number
  finalAbsLateralDeviationM: number
  finalAbsHeadingErrorDeg: number
  finalAbsMotorCmdRpm: number
  maxAbsMotorCmdRpm: number
  maxAbsLateralDeviationM: number
  maxAbsHeadingErrorDeg: number
  maxAbsMeasuredSteerAngleDeg: number
  stableTailSeconds: number
  stableEverTrue: boolean
  reversingEverTrue: boolean
  humanInterventionEverTrue: boolean
}

function normalizeAngleDeg(angleDeg: number): number {
  let normalized = angleDeg
  while (normalized > 180) normalized -= 360
  while (normalized < -180) normalized += 360
  return normalized
}

function straightPath(): PathModel {
  return {
    sample(x, _z, headingRad) {
      const desiredHeadingDeg = 0
      return {
        lateralDeviationM: -x,
        headingErrorDeg: normalizeAngleDeg(desiredHeadingDeg - headingRad * RAD2DEG),
      }
    },
  }
}

function circularPath(radiusM: number, direction: 'left' | 'right'): PathModel {
  const centerX = direction === 'right' ? radiusM : -radiusM
  const centerZ = 0
  const clockwise = direction === 'right'

  return {
    sample(x, z, headingRad) {
      const radialX = x - centerX
      const radialZ = z - centerZ
      const radialLength = Math.max(Math.hypot(radialX, radialZ), 1e-6)
      const pathX = centerX + (radialX / radialLength) * radiusM
      const pathZ = centerZ + (radialZ / radialLength) * radiusM
      const theta = Math.atan2(pathZ - centerZ, pathX - centerX)
      const tangent = clockwise
        ? { x: Math.sin(theta), z: -Math.cos(theta) }
        : { x: -Math.sin(theta), z: Math.cos(theta) }
      const desiredHeadingDeg = Math.atan2(tangent.x, tangent.z) * RAD2DEG
      const offsetX = x - pathX
      const offsetZ = z - pathZ
      return {
        lateralDeviationM: tangent.x * offsetZ - tangent.z * offsetX,
        headingErrorDeg: normalizeAngleDeg(desiredHeadingDeg - headingRad * RAD2DEG),
      }
    },
  }
}

function sCurvePath(amplitudeM: number, wavelengthM: number): PathModel {
  const waveNumber = (2 * Math.PI) / wavelengthM

  return {
    sample(x, z, headingRad) {
      const pathX = amplitudeM * Math.sin(waveNumber * z)
      const derivative = amplitudeM * waveNumber * Math.cos(waveNumber * z)
      const tangentLength = Math.hypot(derivative, 1)
      const tangent = { x: derivative / tangentLength, z: 1 / tangentLength }
      const desiredHeadingDeg = Math.atan2(tangent.x, tangent.z) * RAD2DEG
      const offsetX = x - pathX
      const offsetZ = 0
      return {
        lateralDeviationM: tangent.x * offsetZ - tangent.z * offsetX,
        headingErrorDeg: normalizeAngleDeg(desiredHeadingDeg - headingRad * RAD2DEG),
      }
    },
  }
}

function runScenario(config: ScenarioConfig): ScenarioResult {
  const vehicle = new KinematicVehicle()
  vehicle.teleport(config.initialX ?? 0, config.initialZ ?? 0, (config.initialHeadingDeg ?? 0) * DEG2RAD)
  vehicle.speed = config.initialSpeedMps ?? 0

  let controllerTimer = 0
  let motorCmdRpm = 0
  let integral = 0
  let previousError = 0
  let filteredTargetFrontWheelAngleDeg = 0
  let cachedMeasuredSteerAngleDeg = 0
  let stableHistory: boolean[] = []
  let stableTailFrames = 0
  let maxAbsMotorCmdRpm = 0
  let maxAbsLateralDeviationM = 0
  let maxAbsHeadingErrorDeg = 0
  let maxAbsMeasuredSteerAngleDeg = 0
  let stableEverTrue = false
  let reversingEverTrue = false
  const humanInterventionEverTrue = false
  let lastSample: PathSample = { lateralDeviationM: 0, headingErrorDeg: 0 }

  const totalSteps = Math.ceil(config.durationSec / PHYSICS_DT)
  for (let step = 0; step < totalSteps; step += 1) {
    const timeSec = step * PHYSICS_DT
    const output = vehicle.update(PHYSICS_DT, {
      throttle: config.throttle,
      brake: 0,
      steer: motorCmdRpm / MAX_MOTOR_CMD_RPM,
    })

    const feedbackLost = config.feedbackLossAtSec !== undefined && timeSec >= config.feedbackLossAtSec
    const measuredSteerAngleDeg = feedbackLost
      ? cachedMeasuredSteerAngleDeg
      : output.measuredSteerAngle * RAD2DEG
    if (!feedbackLost) cachedMeasuredSteerAngleDeg = measuredSteerAngleDeg

    lastSample = config.path.sample(output.x, output.z, output.headingRad)
    maxAbsLateralDeviationM = Math.max(maxAbsLateralDeviationM, Math.abs(lastSample.lateralDeviationM))
    maxAbsHeadingErrorDeg = Math.max(maxAbsHeadingErrorDeg, Math.abs(lastSample.headingErrorDeg))
    maxAbsMeasuredSteerAngleDeg = Math.max(maxAbsMeasuredSteerAngleDeg, Math.abs(measuredSteerAngleDeg))
    reversingEverTrue ||= output.speed < -0.05

    const instantStable = Math.abs(lastSample.lateralDeviationM) <= STABLE_LATERAL_THRESHOLD_M
      && Math.abs(lastSample.headingErrorDeg) <= STABLE_HEADING_THRESHOLD_DEG
    stableHistory = [...stableHistory.slice(-(STABLE_WINDOW - 1)), instantStable]
    const stableFlag = stableHistory.length === STABLE_WINDOW && stableHistory.every(Boolean)
    stableEverTrue ||= stableFlag
    stableTailFrames = stableFlag ? stableTailFrames + 1 : 0

    controllerTimer += PHYSICS_DT
    if (controllerTimer >= CONTROLLER_DT) {
      controllerTimer -= CONTROLLER_DT

      const controlSpeedMps = Math.max(Math.abs(output.speed), 0.15)
      const lateralCorrectionDeg = Math.atan2(
        STANLEY_GAIN * lastSample.lateralDeviationM,
        controlSpeedMps + CONTROL_SPEED_SOFTENER_MPS,
      ) * RAD2DEG
      const rawTargetFrontWheelAngleDeg = clamp(
        HEADING_GAIN * lastSample.headingErrorDeg + lateralCorrectionDeg,
        -TRACTOR_CONFIG.maxFrontWheelAngleDeg,
        TRACTOR_CONFIG.maxFrontWheelAngleDeg,
      )
      const maxTargetStepDeg = TARGET_FRONT_WHEEL_SLEW_DEG_PER_SEC * CONTROLLER_DT
      filteredTargetFrontWheelAngleDeg += clamp(
        rawTargetFrontWheelAngleDeg - filteredTargetFrontWheelAngleDeg,
        -maxTargetStepDeg,
        maxTargetStepDeg,
      )

      const targetMeasuredAngleDeg = filteredTargetFrontWheelAngleDeg * TRACTOR_CONFIG.steeringRatio
      const error = targetMeasuredAngleDeg - measuredSteerAngleDeg
      integral = clamp(
        integral + error * CONTROLLER_DT,
        -MOTOR_INTEGRAL_LIMIT,
        MOTOR_INTEGRAL_LIMIT,
      )
      const derivative = (error - previousError) / CONTROLLER_DT
      motorCmdRpm = clamp(
        MOTOR_KP * error + MOTOR_KI * integral + MOTOR_KD * derivative,
        -MAX_MOTOR_CMD_RPM,
        MAX_MOTOR_CMD_RPM,
      )
      previousError = error
      maxAbsMotorCmdRpm = Math.max(maxAbsMotorCmdRpm, Math.abs(motorCmdRpm))
    }
  }

  return {
    finalSpeedMps: vehicle.speed,
    finalAbsLateralDeviationM: Math.abs(lastSample.lateralDeviationM),
    finalAbsHeadingErrorDeg: Math.abs(lastSample.headingErrorDeg),
    finalAbsMotorCmdRpm: Math.abs(motorCmdRpm),
    maxAbsMotorCmdRpm,
    maxAbsLateralDeviationM,
    maxAbsHeadingErrorDeg,
    maxAbsMeasuredSteerAngleDeg,
    stableTailSeconds: stableTailFrames * PHYSICS_DT,
    stableEverTrue,
    reversingEverTrue,
    humanInterventionEverTrue,
  }
}

describe('Autosteer loop scenarios', () => {
  it('launches from standstill into a straight line without persistent steering command', () => {
    const result = runScenario({
      durationSec: 12,
      throttle: 1,
      path: straightPath(),
      initialSpeedMps: 0,
    })

    expect(result.finalSpeedMps).toBeGreaterThan(1)
    expect(result.finalAbsLateralDeviationM).toBeLessThan(0.2)
    expect(result.finalAbsMotorCmdRpm).toBeLessThan(30)
    expect(result.reversingEverTrue).toBe(false)
  })

  it('holds a straight path for 30 seconds and settles near zero steering command', () => {
    const result = runScenario({
      durationSec: 30,
      throttle: 0.9,
      path: straightPath(),
      initialX: 0.5,
      initialHeadingDeg: 4,
      initialSpeedMps: 0.5,
    })

    expect(result.finalAbsLateralDeviationM).toBeLessThan(0.25)
    expect(result.finalAbsHeadingErrorDeg).toBeLessThan(5)
    expect(result.finalAbsMotorCmdRpm).toBeLessThan(45)
    expect(result.stableTailSeconds).toBeGreaterThan(3)
  })

  it('tracks a gentle right curve without divergence', () => {
    const result = runScenario({
      durationSec: 20,
      throttle: 0.9,
      path: circularPath(40, 'right'),
      initialSpeedMps: 0.6,
    })

    expect(result.maxAbsLateralDeviationM).toBeLessThan(1.2)
    expect(result.finalAbsHeadingErrorDeg).toBeLessThan(10)
    expect(result.maxAbsMotorCmdRpm).toBeLessThanOrEqual(MAX_MOTOR_CMD_RPM)
  })

  it('tracks a gentle left curve with the opposite steering sign', () => {
    const result = runScenario({
      durationSec: 20,
      throttle: 0.9,
      path: circularPath(40, 'left'),
      initialSpeedMps: 0.6,
    })

    expect(result.maxAbsLateralDeviationM).toBeLessThan(1.2)
    expect(result.finalAbsHeadingErrorDeg).toBeLessThan(10)
    expect(result.maxAbsMotorCmdRpm).toBeLessThanOrEqual(MAX_MOTOR_CMD_RPM)
  })

  it('follows an S-curve without runaway oscillation', () => {
    const result = runScenario({
      durationSec: 30,
      throttle: 0.9,
      path: sCurvePath(2.5, 60),
      initialSpeedMps: 0.8,
    })

    expect(result.maxAbsLateralDeviationM).toBeLessThan(1.8)
    expect(result.maxAbsHeadingErrorDeg).toBeLessThan(20)
    expect(result.maxAbsMeasuredSteerAngleDeg).toBeLessThan(TRACTOR_CONFIG.maxSteeringWheelAngleDeg)
  })

  it('re-enters the path from a 1.5m offset without divergence', () => {
    const result = runScenario({
      durationSec: 32,
      throttle: 0.75,
      path: straightPath(),
      initialX: 1.5,
      initialHeadingDeg: 12,
      initialSpeedMps: 0.6,
    })

    expect(result.maxAbsLateralDeviationM).toBeLessThan(2.2)
    expect(result.finalAbsLateralDeviationM).toBeLessThan(0.4)
    expect(result.finalAbsHeadingErrorDeg).toBeLessThan(8)
  })

  it('stays bounded when steering feedback is lost mid-run', () => {
    const result = runScenario({
      durationSec: 20,
      throttle: 0.8,
      path: straightPath(),
      initialX: 1.0,
      initialHeadingDeg: 8,
      initialSpeedMps: 0.8,
      feedbackLossAtSec: 8,
    })

    expect(result.maxAbsMotorCmdRpm).toBeLessThanOrEqual(MAX_MOTOR_CMD_RPM)
    expect(result.finalSpeedMps).toBeGreaterThan(0.5)
    expect(result.maxAbsLateralDeviationM).toBeLessThan(2.5)
  })
})
