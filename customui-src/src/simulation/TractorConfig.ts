const DEG2RAD = Math.PI / 180

export const STEER_ENCODER_DEG_PER_COUNT = 0.036

const BASE_TRACTOR_CONFIG = {
  // JD 5050E chassis
  wheelbaseM: 2.05,           // JD 5050E: 2,050mm
  frontTrackM: 1.47,          // JD 5050E: 1,474mm
  rearTrackM: 1.52,           // JD 5050E: ~1,520mm
  frontWheelWidthM: 0.4,      // JD 5050E front tire width (m)
  rearWheelWidthM: 0.55,      // JD 5050E rear tire width (m)
  frontTireRadiusM: 0.55,     // JD 5050E front tire visual radius (m)
  rearTireRadiusM: 0.7,       // JD 5050E rear tire visual radius (m)
  minimumTurningRadiusM: 3.2, // JD 5050E: 3,181mm
  // Steering geometry
  maxFrontWheelAngleDeg: 32,
  maxFrontWheelAngleRad: 32 * DEG2RAD,
  steeringRatio: 20,          // hydrostatic effective ratio (~1080° / 55° ≈ 20)
  maxSteeringWheelAngleDeg: 45 * 20,   // ~900° ≈ 2.5 turns each side
  maxSteeringWheelAngleRad: 45 * 20 * DEG2RAD,
  // Mass
  massKg: 2500,              // JD 5050E: ~2,500kg ballasted
  frontAxleKg: 1000,
  rearAxleKg: 1500,
  implementRearLoadKg: 500,
  cgHeightM: 0.95,
  // Steering motor (KY170C + hydrostatic)
  steeringWheelMaxRateDegPerSec: 1080,  // lock-to-lock ~1.67s (1.5x boost over 720°/s for faster recovery from large steer commands)
  steeringWheelTauSec: 0.12,            // faster LPF response (reduced from 0.18s) — 3τ settling time ~0.36s
  steeringCenteringRate: 0.7,
  steeringSoftLimitStartRatio: 0.8,
  steeringSoftLimitMinRateScale: 0.3,
  frontWheelTauSec: 0.05,       // hydrostatic: nearly instant response
  frontWheelTauSpeedScale: 0.05,
  idleRpm: 850,
  ratedRpm: 2100,
  rpmTauSec: 0.5,
  engineGovernorTauUp: 0.15,
  engineGovernorTauDown: 0.3,
  speedTauSec: 2.0,
  accelMaxFieldMps2: 0.8,
  brakeMaxFieldMps2: 1.5,
  engineDragMps2: 0.15,
  lateralAccelMaxMps2: 1.1,
  wheelContactDepthM: 0.02,
  wheelInfluenceRadiusM: 0.4,
  gpsOriginLatDeg: 37.5665,
  gpsOriginLonDeg: 126.9780,
  fastLoopIntervalMs: 50,
  slowLoopIntervalMs: 1000,
  // Tire model (Pacejka simplified)
  tireBStiffness: 12,
  tireBStiffnessRear: 12,
  tireCShape: 1.5,
  tireECurvature: 0.97,
  soilFrictionDry: 1.2,
  soilFrictionWet: 0.7,
  concreteFriction: 1.4,
  rollingResistanceSoil: 0.04,
  rollingResistanceConcrete: 0.02,
  rollingResistanceBase: 0.015,
  rollingResistanceSinkCoef: 0.5,
  moistureFrictionLoss: 0.3,
  compactionRecoveryFactor: 0.7,
  tilledPenalty: 1.5,
  soilViscousDragCoef: 450,
  lowSpeedTractionBlendMps: 5.0,
  tractionLimitRatio: 1.00,
  tireRadiusM: 0.7,
  // Engine torque curve (JD 5050E approximation)
  engineTorqueCurve: [
    { rpm: 850,  torqueNm: 160 },
    { rpm: 1200, torqueNm: 210 },
    { rpm: 1400, torqueNm: 220 },
    { rpm: 1800, torqueNm: 200 },
    { rpm: 2100, torqueNm: 180 },
    { rpm: 2200, torqueNm: 80 },   // governor cutoff begins
    { rpm: 2300, torqueNm: 30 },   // governed idle torque at rev limit
  ],
  engineInertiaKgm2: 0.5,
  stallRpm: 600,
  // Transmission
  gearRatios: [0, 22.7, 13.9, 8.8, 6.5],
  creepSpeedMps: [0, 0.40, 0.55, 0.74, 1.00],
  finalDriveRatio: 8.5,
  reverseGearMaxSpeedMps: 1.0,
  upshiftCooldownSec: 0.4,
  downshiftCooldownSec: 0.4,
  throttleDeltaDownshiftReset: 0.05,
  // Physics
  gravityMps2: 9.81,
  frontWeightRatio: 0.55,
  maxBrakeTorqueNm: 8000,
  // Soil types
  soilTypes: {
    sandy: { friction: 0.5, rollingResistance: 0.03, sinkSensitivity: 0.00005, compactionRate: 0.005, plowResistanceNm: 80, draftBaseN: 750 },
    loam: { friction: 0.6, rollingResistance: 0.04, sinkSensitivity: 0.0001, compactionRate: 0.01, plowResistanceNm: 120, draftBaseN: 1200 },
    clay: { friction: 0.7, rollingResistance: 0.06, sinkSensitivity: 0.0002, compactionRate: 0.02, plowResistanceNm: 200, draftBaseN: 2200 },
    wet: { friction: 0.35, rollingResistance: 0.08, sinkSensitivity: 0.0004, compactionRate: 0.005, plowResistanceNm: 250, draftBaseN: 1700 },
    weedy: { friction: 0.6, rollingResistance: 0.045, sinkSensitivity: 0.00008, compactionRate: 0.007, plowResistanceNm: 90, draftBaseN: 900 },
  },
  // PTO (Power Take-Off)
  ptoRpm540: 540,
  ptoRpm1000: 1000,
  ptoTorqueMultiplier: 0.3,
}

export const TRACTOR_CONFIG = { ...BASE_TRACTOR_CONFIG,
  engineTorqueCurve: BASE_TRACTOR_CONFIG.engineTorqueCurve.map(p => ({...p})),
  soilTypes: structuredClone(BASE_TRACTOR_CONFIG.soilTypes),
  gearRatios: [...BASE_TRACTOR_CONFIG.gearRatios],
  creepSpeedMps: [...BASE_TRACTOR_CONFIG.creepSpeedMps],
}

export const WORK_GEAR_MAX_SPEEDS_MPS = [0, 0.8, 1.55, 2.45, 3.33] as const
export const GEAR_UPSHIFT_THRESHOLDS_MPS = [0, 0.72, 1.42, 2.25, Number.POSITIVE_INFINITY] as const
export const GEAR_DOWNSHIFT_THRESHOLDS_MPS = [0, 0, 1.05, 1.85, 2.7] as const

export function clamp(value: number, min: number, max: number): number {
  return Math.max(min, Math.min(max, value))
}

export function steeringWheelToFrontWheelAngle(steeringWheelAngleRad: number): number {
  return clamp(
    steeringWheelAngleRad / TRACTOR_CONFIG.steeringRatio,
    -TRACTOR_CONFIG.maxFrontWheelAngleRad,
    TRACTOR_CONFIG.maxFrontWheelAngleRad,
  )
}

export type TractorPresetKey = '50hp' | '120hp' | '200hp'

const TRACTOR_PRESET_MULTIPLIERS: Record<TractorPresetKey, { scale: number; hpRatio: number }> = {
  '50hp':  { scale: 1.00, hpRatio: 1.0 },
  '120hp': { scale: 1.28, hpRatio: 2.4 },
  '200hp': { scale: 1.53, hpRatio: 4.0 },
}

export function applyTractorPreset(presetKey: TractorPresetKey): void {
  const m = TRACTOR_PRESET_MULTIPLIERS[presetKey]
  if (!m) return
  const { scale, hpRatio } = m

  // Dimension properties (scale-proportional)
  TRACTOR_CONFIG.wheelbaseM = BASE_TRACTOR_CONFIG.wheelbaseM * scale
  TRACTOR_CONFIG.frontTrackM = BASE_TRACTOR_CONFIG.frontTrackM * scale
  TRACTOR_CONFIG.rearTrackM = BASE_TRACTOR_CONFIG.rearTrackM * scale
  TRACTOR_CONFIG.minimumTurningRadiusM = BASE_TRACTOR_CONFIG.minimumTurningRadiusM * scale
  TRACTOR_CONFIG.cgHeightM = BASE_TRACTOR_CONFIG.cgHeightM * scale
  TRACTOR_CONFIG.tireRadiusM = BASE_TRACTOR_CONFIG.tireRadiusM * scale
  TRACTOR_CONFIG.frontTireRadiusM = BASE_TRACTOR_CONFIG.frontTireRadiusM * scale
  TRACTOR_CONFIG.rearTireRadiusM = BASE_TRACTOR_CONFIG.rearTireRadiusM * scale
  TRACTOR_CONFIG.frontWheelWidthM = BASE_TRACTOR_CONFIG.frontWheelWidthM * scale
  TRACTOR_CONFIG.rearWheelWidthM = BASE_TRACTOR_CONFIG.rearWheelWidthM * scale
  TRACTOR_CONFIG.wheelInfluenceRadiusM = BASE_TRACTOR_CONFIG.wheelInfluenceRadiusM * scale

  // Mass properties (hp-proportional)
  TRACTOR_CONFIG.massKg = BASE_TRACTOR_CONFIG.massKg * hpRatio
  TRACTOR_CONFIG.frontAxleKg = BASE_TRACTOR_CONFIG.frontAxleKg * hpRatio
  TRACTOR_CONFIG.rearAxleKg = BASE_TRACTOR_CONFIG.rearAxleKg * hpRatio
  TRACTOR_CONFIG.implementRearLoadKg = BASE_TRACTOR_CONFIG.implementRearLoadKg * hpRatio

  // Force properties (hp-proportional)
  TRACTOR_CONFIG.engineTorqueCurve = BASE_TRACTOR_CONFIG.engineTorqueCurve.map((p) => ({
    rpm: p.rpm,
    torqueNm: p.torqueNm * hpRatio,
  }))
  TRACTOR_CONFIG.maxBrakeTorqueNm = BASE_TRACTOR_CONFIG.maxBrakeTorqueNm * hpRatio
  TRACTOR_CONFIG.engineInertiaKgm2 = BASE_TRACTOR_CONFIG.engineInertiaKgm2 * hpRatio
}
