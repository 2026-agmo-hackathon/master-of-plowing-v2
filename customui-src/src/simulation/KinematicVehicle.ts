import {
  clamp,
  GEAR_DOWNSHIFT_THRESHOLDS_MPS,
  GEAR_UPSHIFT_THRESHOLDS_MPS,
  steeringWheelToFrontWheelAngle,
  TRACTOR_CONFIG,
  WORK_GEAR_MAX_SPEEDS_MPS,
} from './TractorConfig'

export interface VehicleOutput {
  x: number; y: number; z: number
  headingRad: number
  speed: number
  rpm: number
  gear: number
  measuredSteerAngle: number   // radians, the only steering angle fed back to the app
  vehicleFrontWheelAngle: number   // radians, used by the vehicle model
  roll: number; pitch: number
  accelX: number; accelY: number
  angularVelocityY: number
  // Optional debug telemetry — order: [FL, FR, RL, RR]
  slip?: [number, number, number, number]
  tireLoad?: [number, number, number, number]
  terrainPitchRad?: number
  terrainRollRad?: number
  wheelOmega?: number
  wheelOmegaL?: number
  wheelOmegaR?: number
  wheelEquivRpm?: number
  draftForceN?: number
  ptoEngaged?: boolean
  ptoLoad?: number
}

export interface VehicleInput {
  throttle: number    // 0-1
  brake: number       // 0-1
  steer: number       // -1 to 1 = motor RPM normalized (controls steer RATE, not angle)
  reverse?: boolean
  manualGear?: number | null  // null/undefined = auto; -1=R, 0=N, 1..4 = forced forward gear
}

const DEG2RAD = Math.PI / 180
const MIN_CURVATURE = 1e-4

export class KinematicVehicle {
  x = 0; y = 0; z = 0
  headingRad = 0
  speed = 0
  vehicleFrontWheelAngle = 0
  measuredSteerAngle = 0
  gear = 1
  rpm = TRACTOR_CONFIG.idleRpm
  private prevSpeed = 0
  private motorCmdNormalized = 0

  update(dt: number, input: VehicleInput): VehicleOutput {
    const targetMotor = clamp(input.steer, -1, 1)
    const motorResponse = 1 - Math.exp(-dt / TRACTOR_CONFIG.steeringWheelTauSec)
    this.motorCmdNormalized += (targetMotor - this.motorCmdNormalized) * motorResponse

    let measuredSteerRate = this.motorCmdNormalized * TRACTOR_CONFIG.steeringWheelMaxRateDegPerSec * DEG2RAD
    if (Math.abs(this.motorCmdNormalized) <= 0.005) {
      measuredSteerRate -= this.measuredSteerAngle * TRACTOR_CONFIG.steeringCenteringRate
    }

    const steerAbs = Math.abs(this.measuredSteerAngle)
    const steerMax = TRACTOR_CONFIG.maxSteeringWheelAngleRad
    const softStart = steerMax * TRACTOR_CONFIG.steeringSoftLimitStartRatio
    const turningOutward = Math.sign(measuredSteerRate) === Math.sign(this.measuredSteerAngle)
    if (turningOutward && steerAbs > softStart) {
      const softProgress = (steerAbs - softStart) / Math.max(steerMax - softStart, 1e-6)
      const clampProgress = clamp(softProgress, 0, 1)
      const rateScale = 1 - clampProgress * (1 - TRACTOR_CONFIG.steeringSoftLimitMinRateScale)
      measuredSteerRate *= rateScale
    }

    this.measuredSteerAngle += measuredSteerRate * dt
    if (Math.abs(this.motorCmdNormalized) > 0.005) {
      // keep angle state from sticking near zero due finite precision after integration
      if (Math.abs(this.measuredSteerAngle) < 0.0005) this.measuredSteerAngle = 0
    } else if (Math.abs(this.measuredSteerAngle) < 0.001) {
      this.measuredSteerAngle = 0
    }
    this.measuredSteerAngle = clamp(
      this.measuredSteerAngle,
      -TRACTOR_CONFIG.maxSteeringWheelAngleRad,
      TRACTOR_CONFIG.maxSteeringWheelAngleRad,
    )

    const absSpeed = Math.abs(this.speed)
    this.selectGear(absSpeed, input.throttle)

    const targetFrontWheelAngle = steeringWheelToFrontWheelAngle(this.measuredSteerAngle)
    const steerSpeedRatio = Math.min(1, Math.abs(this.speed) / 2.0)
    const frontWheelTau = TRACTOR_CONFIG.frontWheelTauSec * (1 + TRACTOR_CONFIG.frontWheelTauSpeedScale * steerSpeedRatio)
    const steeringLinkResponse = 1 - Math.exp(-dt / frontWheelTau)
    this.vehicleFrontWheelAngle += (targetFrontWheelAngle - this.vehicleFrontWheelAngle) * steeringLinkResponse
    if (Math.abs(this.vehicleFrontWheelAngle) < 0.0005 && Math.abs(targetFrontWheelAngle) < 0.0005) {
      this.vehicleFrontWheelAngle = 0
    }

    const curvature = Math.tan(this.vehicleFrontWheelAngle) / TRACTOR_CONFIG.wheelbaseM
    const curvatureAbs = Math.abs(curvature)
    const curveSpeedLimit = curvatureAbs > MIN_CURVATURE
      ? Math.sqrt(TRACTOR_CONFIG.lateralAccelMaxMps2 / curvatureAbs)
      : Number.POSITIVE_INFINITY
    const maxSpeedForGear = WORK_GEAR_MAX_SPEEDS_MPS[Math.max(1, this.gear)]
    const throttleTargetSpeed = input.brake > 0.01 ? 0 : maxSpeedForGear * clamp(input.throttle, 0, 1)
    const targetSpeed = Math.min(throttleTargetSpeed, curveSpeedLimit)
    const speedError = targetSpeed - this.speed

    let accel = speedError / TRACTOR_CONFIG.speedTauSec
    if (speedError >= 0) {
      accel = Math.min(accel, TRACTOR_CONFIG.accelMaxFieldMps2)
    } else {
      const passiveDecel = TRACTOR_CONFIG.engineDragMps2 + Math.min(Math.abs(speedError), TRACTOR_CONFIG.accelMaxFieldMps2)
      const decelLimit = input.brake > 0.01
        ? TRACTOR_CONFIG.brakeMaxFieldMps2 * Math.max(input.brake, 0.2)
        : passiveDecel
      accel = Math.max(accel, -decelLimit)
    }
    this.speed = Math.max(0, this.speed + accel * dt)
    if (this.speed < 0.01 && targetSpeed < 0.01) this.speed = 0

    const speedRatio = this.speed / Math.max(maxSpeedForGear, 0.1)
    const curvatureLoad = Math.min(1, curvatureAbs * TRACTOR_CONFIG.wheelbaseM * 1.6)
    const rpmTarget = Math.max(
      TRACTOR_CONFIG.idleRpm,
      TRACTOR_CONFIG.idleRpm
        + input.throttle * (TRACTOR_CONFIG.ratedRpm - TRACTOR_CONFIG.idleRpm) * 0.8
        + speedRatio * 180
        + curvatureLoad * 140,
    )
    this.rpm += (rpmTarget - this.rpm) * (1 - Math.exp(-dt / TRACTOR_CONFIG.rpmTauSec))

    let headingRate = 0
    if (Math.abs(this.speed) > 0.01) {
      headingRate = this.speed * curvature
      this.headingRad += headingRate * dt
      this.headingRad = ((this.headingRad % (2 * Math.PI)) + 2 * Math.PI) % (2 * Math.PI)
    }

    this.x += Math.sin(this.headingRad) * this.speed * dt
    this.z += Math.cos(this.headingRad) * this.speed * dt

    const accelX = (this.speed - this.prevSpeed) / Math.max(dt, 0.001)
    const accelY = Math.abs(this.speed) > 0.1 ? headingRate * this.speed : 0
    this.prevSpeed = this.speed

    return {
      x: this.x, y: this.y, z: this.z,
      headingRad: this.headingRad,
      speed: this.speed,
      rpm: Math.round(this.rpm),
      gear: this.gear,
      measuredSteerAngle: this.measuredSteerAngle,
      vehicleFrontWheelAngle: this.vehicleFrontWheelAngle,
      roll: 0, pitch: 0,
      accelX, accelY,
      angularVelocityY: headingRate,
    }
  }

  private selectGear(absSpeed: number, throttle: number): void {
    if (throttle <= 0.01 && absSpeed < 0.1) {
      this.gear = 1
      return
    }
    if (
      this.gear < WORK_GEAR_MAX_SPEEDS_MPS.length - 1 &&
      absSpeed > GEAR_UPSHIFT_THRESHOLDS_MPS[this.gear]
    ) {
      this.gear += 1
    } else if (
      this.gear > 1 &&
      absSpeed < GEAR_DOWNSHIFT_THRESHOLDS_MPS[this.gear]
    ) {
      this.gear -= 1
    }
  }

  resetSteer(): void {
    this.vehicleFrontWheelAngle = 0
    this.measuredSteerAngle = 0
    this.motorCmdNormalized = 0
  }

  teleport(x: number, z: number, headingRad: number): void {
    this.x = x; this.z = z; this.headingRad = headingRad
    this.speed = 0
    this.vehicleFrontWheelAngle = 0
    this.measuredSteerAngle = 0
    this.motorCmdNormalized = 0
    this.rpm = TRACTOR_CONFIG.idleRpm
    this.prevSpeed = 0
  }
}
