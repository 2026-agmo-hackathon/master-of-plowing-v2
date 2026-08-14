import { VehicleInput, VehicleOutput } from './KinematicVehicle'
import {
  computeTireForces,
  computeSlipAngle,
  magicFormula,
  computeRollingResistance,
} from './TireModel'
import {
  computeGovernedTorque,
  engineFrictionTorque,
} from './EngineTorque'
import {
  clamp,
  steeringWheelToFrontWheelAngle,
  TRACTOR_CONFIG,
  WORK_GEAR_MAX_SPEEDS_MPS,
} from './TractorConfig'

const DEG2RAD = Math.PI / 180
const CREEP_SERVO_K = 2.5
const MAX_ENGINE_BRAKE_MPS2 = 1.5

export interface WheelSoilState {
  sinkDepth: number
  moisture: number
  compaction: number
  tilled: boolean
}

export class DynamicVehicle {
  x = 0
  y = 0
  z = 0

  // Internal state
  private vx = 0
  private vy = 0
  private headingRad = 0
  private rpm = TRACTOR_CONFIG.idleRpm as number
  private gear = 1
  private upshiftCooldown = 0   // seconds until next upshift allowed
  private downshiftCooldown = 0 // seconds until next downshift allowed
  private _wheelOmegaL = 0
  private _wheelOmegaR = 0
  private _currentYawRate: number = 0  // actual yaw rate, lags target with τ=0.3s
  get wheelOmega(): number { return (this._wheelOmegaL + this._wheelOmegaR) / 2 }
  set wheelOmega(v: number) { this._wheelOmegaL = v; this._wheelOmegaR = v }
  private measuredSteerAngle = 0
  private frontWheelAngle = 0
  private frictionCoeff = TRACTOR_CONFIG.soilFrictionDry as number
  private motorCmdNormalized = 0
  private ptoEngaged = false
  private ptoLoad = 0
  private frontLoadDelta = 0
  private rearLoadDelta = 0
  private draftForceN = 0
  private terrainPitchRad: number = 0
  private terrainRollRad: number = 0
  private extraRollingResistance: number = 0
  private wheelSoilState: WheelSoilState[] = [
    { sinkDepth: 0, moisture: 0, compaction: 0, tilled: false },
    { sinkDepth: 0, moisture: 0, compaction: 0, tilled: false },
    { sinkDepth: 0, moisture: 0, compaction: 0, tilled: false },
    { sinkDepth: 0, moisture: 0, compaction: 0, tilled: false },
  ]

  setTerrainTilt(pitchRad: number, rollRad: number): void {
    // Dead-zone wide enough to absorb procedural noise from heightmap sampling
    // on visually-flat terrain (~5.7°). Real ramps in test-terrain are ~10°.
    const DZ = 0.10
    this.terrainPitchRad = Math.abs(pitchRad) < DZ ? 0 : pitchRad
    this.terrainRollRad = Math.abs(rollRad) < DZ ? 0 : rollRad
  }

  setExtraRollingResistance(value: number): void {
    this.extraRollingResistance = Math.max(0, value)
  }

  setSoilState(perWheel: WheelSoilState[]): void {
    this.wheelSoilState = perWheel.slice(0, 4).map(s => ({
      sinkDepth: Math.max(0, s.sinkDepth),
      moisture: clamp(s.moisture, 0, 1),
      compaction: clamp(s.compaction, 0, 1),
      tilled: s.tilled,
    }))
    while (this.wheelSoilState.length < 4) {
      this.wheelSoilState.push({ sinkDepth: 0, moisture: 0, compaction: 0, tilled: false })
    }
  }

  getTerrainPitchRad(): number { return this.terrainPitchRad }
  getTerrainRollRad(): number { return this.terrainRollRad }

  setFrictionCoeff(mu: number): void {
    this.frictionCoeff = mu
  }

  setPtoEngaged(engaged: boolean): void {
    this.ptoEngaged = engaged
  }

  setPtoLoad(torqueNm: number): void {
    this.ptoLoad = torqueNm
  }

  setWeightTransfer(frontDelta: number, rearDelta: number): void {
    this.frontLoadDelta = frontDelta
    this.rearLoadDelta = rearDelta
  }

  setDraftForce(forceN: number): void {
    this.draftForceN = forceN
  }

  resetSteer(): void {
    this.frontWheelAngle = 0
    this.measuredSteerAngle = 0
    this.motorCmdNormalized = 0
  }

  teleport(x: number, z: number, headingRad: number): void {
    this.x = x; this.z = z; this.headingRad = headingRad
    this.vx = 0; this.vy = 0
    this._currentYawRate = 0
    this.frontWheelAngle = 0
    this.measuredSteerAngle = 0
    this.motorCmdNormalized = 0
    this.wheelOmega = 0
    this.upshiftCooldown = 0
    this.downshiftCooldown = 0
  }

  getHeadingRad(): number {
    return this.headingRad
  }

  getMeasuredSteerNormalized(): number {
    const maxDeg = TRACTOR_CONFIG.maxFrontWheelAngleDeg * TRACTOR_CONFIG.steeringRatio
    if (maxDeg <= 0) return 0
    const currentDeg = this.measuredSteerAngle * 180 / Math.PI  // radian → deg
    return Math.max(-1, Math.min(1, currentDeg / maxDeg))
  }

  update(input: VehicleInput, dt: number): VehicleOutput {
    // Low-speed kinematic fallback blend factor.
    // blend=0: pure kinematic (no lateral tire forces), blend=1: pure dynamic.
    const vxAbs = Math.abs(this.vx)
    const KINEMATIC_FULL_SPEED = 1.0  // m/s — below this, full kinematic
    const DYNAMIC_FULL_SPEED   = 2.5  // m/s — above this, full dynamic
    let blend: number
    if (vxAbs <= KINEMATIC_FULL_SPEED) blend = 0
    else if (vxAbs >= DYNAMIC_FULL_SPEED) blend = 1
    else blend = (vxAbs - KINEMATIC_FULL_SPEED) / (DYNAMIC_FULL_SPEED - KINEMATIC_FULL_SPEED)

    // Step 1: Steering (copied from KinematicVehicle)
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
      if (Math.abs(this.measuredSteerAngle) < 0.0005) this.measuredSteerAngle = 0
    } else if (Math.abs(this.measuredSteerAngle) < 0.001) {
      this.measuredSteerAngle = 0
    }
    this.measuredSteerAngle = clamp(
      this.measuredSteerAngle,
      -TRACTOR_CONFIG.maxSteeringWheelAngleRad,
      TRACTOR_CONFIG.maxSteeringWheelAngleRad,
    )

    const targetFrontWheelAngle = steeringWheelToFrontWheelAngle(this.measuredSteerAngle)
    const steerSpeedRatio = Math.min(1, Math.abs(this.vx) / 2.0)
    const frontWheelTau = TRACTOR_CONFIG.frontWheelTauSec * (1 + TRACTOR_CONFIG.frontWheelTauSpeedScale * steerSpeedRatio)
    const steeringLinkResponse = 1 - Math.exp(-dt / frontWheelTau)
    this.frontWheelAngle += (targetFrontWheelAngle - this.frontWheelAngle) * steeringLinkResponse
    if (Math.abs(this.frontWheelAngle) < 0.0005 && Math.abs(targetFrontWheelAngle) < 0.0005) {
      this.frontWheelAngle = 0
    }

    // Step 2: Gear selection
    const absSpeed = Math.abs(this.vx)
    this.upshiftCooldown = Math.max(0, this.upshiftCooldown - dt)
    this.downshiftCooldown = Math.max(0, this.downshiftCooldown - dt)
    if (input.manualGear !== undefined && input.manualGear !== null) {
      // Manual override: driver selects gear directly.
      const mg = input.manualGear | 0
      if (mg !== this.gear) {
        this.gear = mg === -1 ? -1 : Math.max(0, Math.min(WORK_GEAR_MAX_SPEEDS_MPS.length - 1, mg))
        this.upshiftCooldown = TRACTOR_CONFIG.upshiftCooldownSec
        this.downshiftCooldown = TRACTOR_CONFIG.upshiftCooldownSec
      }
    }

    // Per-gear forward speed cap
    const currentMaxForwardSpeedMps = this.gear === -1
      ? 0
      : WORK_GEAR_MAX_SPEEDS_MPS[Math.max(0, Math.min(WORK_GEAR_MAX_SPEEDS_MPS.length - 1, this.gear))]
    let throttleScale: number
    if (this.gear === -1) {
      // Reverse: no throttle reduction until vx reaches 95% of reverse cap; then linearly taper to 0 at 100%.
      const ratio = TRACTOR_CONFIG.reverseGearMaxSpeedMps > 0
        ? Math.max(0, Math.min(1, -this.vx / TRACTOR_CONFIG.reverseGearMaxSpeedMps))
        : 0
      const tapeStart = 0.95
      throttleScale = ratio <= tapeStart
        ? 1
        : Math.max(0, 1 - (ratio - tapeStart) / (1 - tapeStart))
    } else if (currentMaxForwardSpeedMps > 0) {
      // No throttle reduction until vx reaches 95% of cap; then linearly taper to 0 at 100%.
      const ratio = Math.max(0, Math.min(1, this.vx / currentMaxForwardSpeedMps))
      const tapeStart = 0.95
      throttleScale = ratio <= tapeStart
        ? 1
        : Math.max(0, 1 - (ratio - tapeStart) / (1 - tapeStart))
    } else {
      throttleScale = 0  // gear=0 sentinel
    }
    // Step 3: Engine torque (use raw throttle so governor setpoint is not throttled down by speed cap)
    const availableTorque = computeGovernedTorque(this.rpm, input.throttle, TRACTOR_CONFIG.engineTorqueCurve)
    const friction = engineFrictionTorque(this.rpm)

    // Step 4: Drivetrain (throttleScale applied to wheel torque only, not to engine model)
    const gearRatio = this.gear === -1 ? TRACTOR_CONFIG.gearRatios[1] : TRACTOR_CONFIG.gearRatios[this.gear]
    const driveRatio = gearRatio * TRACTOR_CONFIG.finalDriveRatio
    const driveTorqueAtWheel = (availableTorque - friction) * driveRatio * (this.gear === -1 ? -1 : 1) * throttleScale
    const drivenTorqueAtWheel = input.throttle > 0.01 ? driveTorqueAtWheel : 0
    const brakeTorque = input.brake * TRACTOR_CONFIG.maxBrakeTorqueNm

    // Step 5: Normal loads (with 3-point hitch weight transfer)
    const totalWeight = TRACTOR_CONFIG.massKg * TRACTOR_CONFIG.gravityMps2
    const frontLoad = totalWeight * TRACTOR_CONFIG.frontWeightRatio + this.frontLoadDelta
    const rearLoad = totalWeight * (1 - TRACTOR_CONFIG.frontWeightRatio) + this.rearLoadDelta
    const frontSoil = this.averageSoilState(0, 2)
    const rearSoil = this.averageSoilState(2, 4)
    const allSoil = this.averageSoilState(0, 4)
    const frontMu = this.effectiveFrictionCoeff(frontSoil.moisture)
    const rearMu = this.effectiveFrictionCoeff(rearSoil.moisture)

    // Step 6: Gravity forces from terrain slope
    const gravLong = -TRACTOR_CONFIG.massKg * TRACTOR_CONFIG.gravityMps2 * Math.sin(this.terrainPitchRad)
    const gravLat = TRACTOR_CONFIG.massKg * TRACTOR_CONFIG.gravityMps2 * Math.sin(this.terrainRollRad)

    // Compute yaw rate first (used by both rear and front axle slip computations)
    // Standard bicycle-model convention: curvature = tan(delta) / L (positive for left turn)
    const rawCurvature = Math.tan(this.frontWheelAngle) / TRACTOR_CONFIG.wheelbaseM
    const maxCurvature = 1 / TRACTOR_CONFIG.minimumTurningRadiusM
    const curvature = Math.max(-maxCurvature, Math.min(maxCurvature, rawCurvature))
    const angularVelocity = this.vx * curvature
    const lrRear = TRACTOR_CONFIG.wheelbaseM * TRACTOR_CONFIG.frontWeightRatio
    const rearVLat = this.vy - lrRear * angularVelocity

    // Step 7: Rear tire forces (drive axle)
    const [rearLoadL, rearLoadR] = this.splitRearLoad(rearLoad, this.terrainRollRad)
    const { B, C, E } = { B: TRACTOR_CONFIG.tireBStiffnessRear, C: TRACTOR_CONFIG.tireCShape, E: TRACTOR_CONFIG.tireECurvature }
    // lateralPriority=true: rear driven axle preserves cornering grip when throttle is applied (oversteer mitigation).
    const rearTireL = computeTireForces(this._wheelOmegaL, this.vx, rearVLat, rearLoadL, rearMu, TRACTOR_CONFIG.tireRadiusM, { B, C, E }, true)
    const rearTireR = computeTireForces(this._wheelOmegaR, this.vx, rearVLat, rearLoadR, rearMu, TRACTOR_CONFIG.tireRadiusM, { B, C, E }, true)
    const rearLongTotal = rearTireL.longitudinal + rearTireR.longitudinal
    const rearLatTotal = rearTireL.lateral + rearTireR.lateral

    // Step 8: Front tire (non-driven, lateral only)
    const frontVLat = this.vy + (TRACTOR_CONFIG.wheelbaseM * (1 - TRACTOR_CONFIG.frontWeightRatio)) * angularVelocity
    const frontSlipAngle = computeSlipAngle(frontVLat, this.vx) - this.frontWheelAngle
    const frontLateral = magicFormula(frontSlipAngle, {
      B: TRACTOR_CONFIG.tireBStiffness,
      C: TRACTOR_CONFIG.tireCShape,
      D: frontMu * frontLoad,
      E: TRACTOR_CONFIG.tireECurvature,
    })

    // Step 9: Rolling resistance
    const rollingCoeff = this.effectiveRollingResistanceCoeff(allSoil)
    const rollingR = computeRollingResistance(totalWeight, rollingCoeff)
    const rollingForce = this.vx > 0.01 ? -rollingR : this.vx < -0.01 ? rollingR : 0
    const viscousSoilForce = -TRACTOR_CONFIG.soilViscousDragCoef * allSoil.moisture * this.vx

    // Step 10: Net forces and acceleration.
    // In this simulator, tire-soil static friction is treated as braking hold,
    // not as an automatic parking brake. With brake released and no throttle,
    // wheels free-roll and gravity can move the tractor down a slope.
    const slopeNormalScale = Math.cos(this.terrainPitchRad) * Math.cos(this.terrainRollRad)
    const brakeHoldLimit = Math.min(frontMu, rearMu) * totalWeight * slopeNormalScale * clamp(input.brake, 0, 1)

    const lowSpeedBlend = input.throttle > 0.05
      ? clamp(1 - absSpeed / TRACTOR_CONFIG.lowSpeedTractionBlendMps, 0, 1)
      : 0
    const torqueLimitedDrive = drivenTorqueAtWheel / TRACTOR_CONFIG.tireRadiusM
    const tractionLimit = rearMu * rearLoad * TRACTOR_CONFIG.tractionLimitRatio
    const lowSpeedDrive = clamp(torqueLimitedDrive, -tractionLimit, tractionLimit)
    const longitudinalDrive = rearLongTotal * (1 - lowSpeedBlend) + lowSpeedDrive * lowSpeedBlend
    // Suppress large engine braking when throttle is applied: tire model can produce negative
    // longitudinal force (engine braking) when wheel omega lags behind vx, but with positive
    // throttle the engine should not actively brake. Allow mild negative to cover transients.
    const adjustedLongitudinalDrive = (input.throttle > 0.05 && longitudinalDrive < -rollingR)
      ? -rollingR
      : longitudinalDrive

    let Fx = rollingForce + viscousSoilForce + gravLong
    if (input.brake > 0.01) {
      const brakeDirection = Math.abs(this.vx) > 0.01 ? Math.sign(this.vx) : Math.sign(Fx)
      if (brakeDirection !== 0) {
        const brakeApplied = (brakeTorque / TRACTOR_CONFIG.tireRadiusM) * brakeDirection
        Fx -= brakeApplied
      }
    } else {
      const creep = TRACTOR_CONFIG.creepSpeedMps
      const target = this.gear === -1 ? -creep[1]
                   : this.gear >= 1 && this.gear <= 4 ? creep[this.gear]
                   : 0
      const servoForce = TRACTOR_CONFIG.massKg * CREEP_SERVO_K * (target - this.vx)
      if (input.throttle > 0.01) {
        const clampedServo = this.gear === -1 ? Math.min(0, servoForce) : Math.max(0, servoForce)
        Fx += clampedServo
        Fx += adjustedLongitudinalDrive
      } else {
        const maxBrakeForce = TRACTOR_CONFIG.massKg * MAX_ENGINE_BRAKE_MPS2
        let appliedForce = servoForce
        const isEngineBraking = (this.vx > target && this.vx > 0) || (this.vx < target && this.vx < 0)
        if (isEngineBraking) {
          appliedForce = Math.sign(servoForce) * Math.min(Math.abs(servoForce), maxBrakeForce)
        }
        Fx += appliedForce
        this.wheelOmega = this.vx / TRACTOR_CONFIG.tireRadiusM
      }
    }
    // Fade out draft force near max speed to guarantee the gear speed ceiling is reachable
    const speedRatio = currentMaxForwardSpeedMps > 0 ? Math.abs(this.vx) / currentMaxForwardSpeedMps : 0
    let draftAttenuation = 1.0
    if (speedRatio > 0.85) {
      // Linear fade: 1.0 at 85% of max speed → 0.0 at 100%
      draftAttenuation = Math.max(0, (1 - speedRatio) / 0.15)
    }
    const effectiveDraftForce = this.draftForceN * draftAttenuation
    if (Math.abs(this.vx) >= 1e-3) {
      Fx -= effectiveDraftForce * Math.sign(this.vx)
    }
    const extraRollN = this.extraRollingResistance * TRACTOR_CONFIG.massKg * TRACTOR_CONFIG.gravityMps2
    if (Math.abs(this.vx) > 0.01) {
      Fx -= extraRollN * Math.sign(this.vx)
    }
    if (absSpeed < 0.1 && input.throttle < 0.01 && input.brake > 0.01 && Math.abs(Fx) < brakeHoldLimit) {
      Fx = 0
      this.vx = 0
    }
    const idleStabilize =
      this.gear === 0 &&
      absSpeed < 0.08 &&
      input.throttle < 0.01 &&
      input.brake < 0.01 &&
      Math.abs(this.terrainPitchRad) < 0.14 &&
      Math.abs(this.terrainRollRad) < 0.14
    if (idleStabilize) {
      Fx = 0
      this.vx = 0
      this.wheelOmega = 0
    }

    // Lateral: tire cornering forces OPPOSE slip (restoring), gravity adds.
    // Scale lateral tire forces by blend to suppress Pacejka saturation at low speed.
    const effectiveFrontLateral = frontLateral * blend
    const effectiveRearLatTotal = rearLatTotal * blend
    let Fy = -effectiveFrontLateral - effectiveRearLatTotal + gravLat
    if (absSpeed < 0.1 && input.brake > 0.01 && Math.abs(Fy) < brakeHoldLimit) {
      Fy = 0
    }
    if (idleStabilize) {
      Fy = 0
      this.vy = 0
    }

    const ax = Fx / TRACTOR_CONFIG.massKg
    const ay = Fy / TRACTOR_CONFIG.massKg

    // Step 11: Velocity integration
    this.vx += ax * dt
    if (this.gear === -1) {
      // Reverse gear: clamp to [-reverseMax, 0)
      this.vx = Math.max(this.vx, -TRACTOR_CONFIG.reverseGearMaxSpeedMps)
      this.vx = Math.min(this.vx, 0)
      // When at reverse cap, prevent wheel from running away (>10% slip in either direction)
      if (this.vx <= -TRACTOR_CONFIG.reverseGearMaxSpeedMps + 0.001) {
        const wheelLimit = Math.abs(this.vx) / TRACTOR_CONFIG.tireRadiusM * 1.1
        this._wheelOmegaL = Math.max(-wheelLimit, Math.min(this._wheelOmegaL, 0))
        this._wheelOmegaR = Math.max(-wheelLimit, Math.min(this._wheelOmegaR, 0))
      }
    } else {
      // Forward gear: clamp to [-reverseMax, currentMaxForwardSpeedMps]
      this.vx = Math.max(this.vx, -TRACTOR_CONFIG.reverseGearMaxSpeedMps)
      this.vx = Math.min(this.vx, currentMaxForwardSpeedMps)
      // When at forward cap, prevent wheel from running away beyond no-slip + 10% slip allowance
      if (currentMaxForwardSpeedMps > 0 && this.vx >= currentMaxForwardSpeedMps - 0.001) {
        const wheelLimit = this.vx / TRACTOR_CONFIG.tireRadiusM * 1.1
        this._wheelOmegaL = Math.min(this._wheelOmegaL, wheelLimit)
        this._wheelOmegaR = Math.min(this._wheelOmegaR, wheelLimit)
      }
    }
    this.vy += ay * dt
    // Lateral damping (dt-independent). Time constant ~200ms (rate=5/s).
    // Tires already provide Fy restoring force; this represents residual
    // numerical damping for stability at low speeds.
    const LATERAL_DAMPING_RATE = 10.0  // 1/s (time constant ~100ms, allows vy to follow heading)
    this.vy *= Math.exp(-LATERAL_DAMPING_RATE * dt)
    // Extra damping boost in kinematic/blend region: removes residual vy when lateral Fy is suppressed.
    if (blend < 1) {
      const dampingBoost = (1 - blend) * 20.0  // blend=0 → +20/s extra damping
      this.vy *= Math.exp(-dampingBoost * dt)
    }
    // Kill tiny lateral drift at rest
    if (absSpeed < 0.1 && Math.abs(this.vy) < 0.02) this.vy = 0

    // Step 12: Yaw rate and heading
    // Camera mapping: simulator screen-right maps to world -X, so +steer must
    // decrease heading. Negate yawRate for heading integration only; slip-angle
    // calculations above use the standard (positive) bicycle-model convention.
    //
    // First-order lag (τ=0.3s) approximates yaw inertia (Iz). Without this,
    // kinematic yawRate is applied instantly, causing spin/carousel behavior
    // at full steer + low speed where CG translation lags behind heading rotation.
    // max sustainable yaw rate from tire lateral capacity: a_max ≈ mu * g, omega_max ≈ a_max / |vx|
    const muAvg = (frontMu + rearMu) * 0.5
    const speedAbs = Math.max(Math.abs(this.vx), 0.5)
    const maxSustainableYawRate = (muAvg * TRACTOR_CONFIG.gravityMps2) / speedAbs
    const kinematicYawRate = -this.vx * curvature
    const targetYawRate = Math.sign(kinematicYawRate) * Math.min(Math.abs(kinematicYawRate), maxSustainableYawRate)
    const YAW_TAU_SEC = 0.6
    const yawAlpha = 1 - Math.exp(-dt / YAW_TAU_SEC)
    this._currentYawRate += (targetYawRate - this._currentYawRate) * yawAlpha
    const yawRate = this._currentYawRate
    this.headingRad += yawRate * dt
    this.headingRad = ((this.headingRad % (2 * Math.PI)) + 2 * Math.PI) % (2 * Math.PI)

    // Step 13: World position
    const cosH = Math.cos(this.headingRad)
    const sinH = Math.sin(this.headingRad)
    this.x += (sinH * this.vx + cosH * this.vy) * dt
    this.z += (cosH * this.vx - sinH * this.vy) * dt

    // Step 14-15: Engine + wheel coupled via drivetrain
    // Engine and wheels are rigidly coupled through gears.
    // Combined inertia: J_eff = J_engine * ratio² + J_wheel
    const ptoTorque = this.ptoEngaged ? this.ptoLoad : 0
    const wheelInertia = 5.0
    // Clutch slip model: real tractors use clutch slip to overcome high gear-ratio
    // inertia at low speed/low gear. The rigid model here would otherwise lock the
    // engine RPM to wheel speed, making 1st-gear startup against implement draft impossible.
    const startupSlip = absSpeed < 0.3 ? 0.2 : absSpeed < 1.0 ? 0.5 : 1.0
    const gearSlip = this.gear === 1 ? 0.3 : this.gear === 2 ? 0.55 : this.gear === 3 ? 0.8 : 1.0
    const slipFactor = Math.min(startupSlip, gearSlip)
    const effectiveInertia = TRACTOR_CONFIG.engineInertiaKgm2 * driveRatio * driveRatio * slipFactor + wheelInertia

    // Net torque at wheel: drive - brake - tire reaction - draft load
    // PTO uses its own ~4:1 ratio (engine 2100 RPM ↔ PTO 540 RPM), NOT the wheel driveRatio.
    // Multiplying by wheel driveRatio (55~193) made high gears unable to overcome PTO load.
    // Open differential: shared torques (drive, brake, PTO, draft, rolling) split evenly L/R.
    // Left/right differ only by their respective tire longitudinal reaction forces.
    const PTO_DRIVE_RATIO = 4.0
    const draftWheelTorque = effectiveDraftForce * TRACTOR_CONFIG.tireRadiusM
    const sharedNetTorque = (
        drivenTorqueAtWheel
      - brakeTorque * Math.sign(this.wheelOmega)
      - ptoTorque * PTO_DRIVE_RATIO
      - draftWheelTorque * Math.sign(this.vx)
      - rollingR * TRACTOR_CONFIG.tireRadiusM * Math.sign(this.wheelOmega)
      - extraRollN * TRACTOR_CONFIG.tireRadiusM * Math.sign(this.wheelOmega)
    ) / 2

    const wheelNetTorqueL = sharedNetTorque - rearTireL.longitudinal * TRACTOR_CONFIG.tireRadiusM
    const wheelNetTorqueR = sharedNetTorque - rearTireR.longitudinal * TRACTOR_CONFIG.tireRadiusM

    const halfInertia = effectiveInertia / 2
    this._wheelOmegaL += (wheelNetTorqueL / halfInertia) * dt
    this._wheelOmegaR += (wheelNetTorqueR / halfInertia) * dt

    // Absolute wheel speed limit: never exceed what rated engine RPM (+ small margin) would produce.
    // Prevents wheelOmega ODE from diverging when tire reaction is too weak to balance drive torque
    // (e.g., 1st gear with very high driveRatio against draft load).
    const wheelOmegaCeiling = (TRACTOR_CONFIG.ratedRpm * 1.05) * (2 * Math.PI) / 60 / driveRatio
    if (this._wheelOmegaL > wheelOmegaCeiling) {
      this._wheelOmegaL = wheelOmegaCeiling
    } else if (this._wheelOmegaL < -wheelOmegaCeiling) {
      this._wheelOmegaL = -wheelOmegaCeiling
    }
    if (this._wheelOmegaR > wheelOmegaCeiling) {
      this._wheelOmegaR = wheelOmegaCeiling
    } else if (this._wheelOmegaR < -wheelOmegaCeiling) {
      this._wheelOmegaR = -wheelOmegaCeiling
    }

    // One-sided wheel slip cap: prevent wheel from spinning too fast relative to vx (loss of traction).
    // Lower bound NOT clamped — engine braking and natural deceleration must remain free.
    if (Math.abs(this.vx) > 0.01) {
      const noSlipOmega = this.vx / TRACTOR_CONFIG.tireRadiusM
      const slipCapRatio = 0.20  // allow up to +20% slip (agricultural bias-ply tuned)
      const upperBound = noSlipOmega + Math.abs(noSlipOmega) * slipCapRatio
      if (this._wheelOmegaL > 0 && this._wheelOmegaL > upperBound) {
        this._wheelOmegaL = upperBound
      } else if (this._wheelOmegaL < 0 && this._wheelOmegaL < -Math.abs(upperBound)) {
        this._wheelOmegaL = -Math.abs(upperBound)
      }
      if (this._wheelOmegaR > 0 && this._wheelOmegaR > upperBound) {
        this._wheelOmegaR = upperBound
      } else if (this._wheelOmegaR < 0 && this._wheelOmegaR < -Math.abs(upperBound)) {
        this._wheelOmegaR = -Math.abs(upperBound)
      }
    }

    if (absSpeed < 0.01 && input.throttle < 0.01) {
      this._wheelOmegaL *= 0.9
      this._wheelOmegaR *= 0.9
    }

    // Clutch-out behaviour: heavy brake with no throttle decouples drive train
    // so the wheel decelerates from brake friction alone (engine inertia stops
    // dominating). Without this, brakes feel useless on high-ratio gears.
    const clutchOut = input.brake > 0.3 && input.throttle < 0.05
    if (clutchOut) {
      const wheelDecay = Math.pow(0.5, dt * 60 * input.brake)
      this._wheelOmegaL *= wheelDecay
      this._wheelOmegaR *= wheelDecay
    }

    // Engine RPM model — load-responsive governor ODE (1st-order).
    const MAX_ENGINE_RPM = 2200  // governor cap; above this the torque curve falls off a cliff

    // 1) Setpoint: throttle pedal determines the upper bound for RPM (raw throttle, not speed-scaled)
    const setpointRpm = TRACTOR_CONFIG.idleRpm + Math.sqrt(Math.max(0, input.throttle)) * (TRACTOR_CONFIG.ratedRpm - TRACTOR_CONFIG.idleRpm)

    // 2) Wheel-equivalent RPM: what engine RPM the wheel speed demands through the drivetrain
    const wheelEquivRpm = Math.abs(this.wheelOmega) * driveRatio * 60 / (2 * Math.PI)

    // 3) Target RPM: clutchOut → spin down to idle; otherwise follow wheel load within [idle, setpoint]
    let targetRpm: number
    if (clutchOut) {
      targetRpm = TRACTOR_CONFIG.idleRpm
    } else {
      // Floor: idle (governor prevents drop below idle)
      // Ceiling: setpoint (governor cuts fuel above throttle-commanded RPM)
      // Engine rises freely to setpoint; wheelEquivRpm can pull RPM higher if overspeeding,
      // but never suppresses it below the throttle setpoint.
      targetRpm = Math.max(TRACTOR_CONFIG.idleRpm, setpointRpm)
    }

    // 4) First-order lag: smooth RPM response
    const tau = targetRpm > this.rpm
      ? TRACTOR_CONFIG.engineGovernorTauUp
      : TRACTOR_CONFIG.engineGovernorTauDown
    this.rpm += (targetRpm - this.rpm) * (1 - Math.exp(-dt / tau))

    // 5) Safety clamp: never below idle, never above MAX_ENGINE_RPM
    this.rpm = Math.max(TRACTOR_CONFIG.idleRpm, Math.min(this.rpm, MAX_ENGINE_RPM))

    // Step 16: Build VehicleOutput
    // RL/RR loads and slip ratios now reflect left/right weight transfer from roll.
    const frontLoadHalf = frontLoad / 2
    return {
      x: this.x,
      y: this.y,
      z: this.z,
      headingRad: this.headingRad,
      speed: this.vx,
      rpm: this.rpm,
      gear: this.gear,
      measuredSteerAngle: this.measuredSteerAngle,
      vehicleFrontWheelAngle: this.frontWheelAngle,
      roll: this.terrainRollRad,
      pitch: this.terrainPitchRad,
      accelX: ax,
      accelY: ay,
      angularVelocityY: yawRate,
      slip: [frontSlipAngle, frontSlipAngle, rearTireL.slipRatio, rearTireR.slipRatio],
      tireLoad: [frontLoadHalf, frontLoadHalf, rearLoadL, rearLoadR],
      terrainPitchRad: this.terrainPitchRad,
      terrainRollRad: this.terrainRollRad,
      wheelOmega: this.wheelOmega,
      wheelOmegaL: this._wheelOmegaL,
      wheelOmegaR: this._wheelOmegaR,
      wheelEquivRpm,
      draftForceN: this.draftForceN,
      ptoEngaged: this.ptoEngaged,
      ptoLoad: this.ptoLoad,
    }
  }

  private splitRearLoad(totalRearLoad: number, rollRad: number): [number, number] {
    const delta = totalRearLoad * (Math.tan(rollRad) * TRACTOR_CONFIG.cgHeightM / TRACTOR_CONFIG.rearTrackM) / 2
    return [totalRearLoad / 2 + delta, totalRearLoad / 2 - delta]  // [L, R] — roll>0 = right higher, left inner heavier
  }

  private averageSoilState(start: number, end: number): WheelSoilState {
    let sinkDepth = 0
    let moisture = 0
    let compaction = 0
    let tilledCount = 0
    const count = Math.max(1, end - start)
    for (let i = start; i < end; i++) {
      const s = this.wheelSoilState[i] ?? { sinkDepth: 0, moisture: 0, compaction: 0, tilled: false }
      sinkDepth += s.sinkDepth
      moisture += s.moisture
      compaction += s.compaction
      if (s.tilled) tilledCount++
    }
    return {
      sinkDepth: sinkDepth / count,
      moisture: moisture / count,
      compaction: compaction / count,
      tilled: tilledCount > count / 2,
    }
  }

  private effectiveFrictionCoeff(moisture: number): number {
    return this.frictionCoeff * (1 - TRACTOR_CONFIG.moistureFrictionLoss * clamp(moisture, 0, 1))
  }

  private effectiveRollingResistanceCoeff(soil: WheelSoilState): number {
    const compactedSink = soil.sinkDepth * (1 - TRACTOR_CONFIG.compactionRecoveryFactor * soil.compaction)
    const tilledPenalty = soil.tilled ? TRACTOR_CONFIG.tilledPenalty : 1
    return TRACTOR_CONFIG.rollingResistanceBase
      + TRACTOR_CONFIG.rollingResistanceSinkCoef * compactedSink * tilledPenalty
  }
}
