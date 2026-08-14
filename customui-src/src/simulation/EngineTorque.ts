export interface TorqueCurvePoint {
  rpm: number
  torqueNm: number
}

export interface EngineOutput {
  rpm: number
  stalled: boolean
}

const TWO_PI = 2 * Math.PI

/**
 * Linear interpolation of torque from a sorted RPM-torque curve.
 * Returns 0 below the minimum or above the maximum RPM.
 */
export function interpolateTorque(
  rpm: number,
  curve: readonly TorqueCurvePoint[],
): number {
  if (curve.length === 0) return 0
  if (rpm < curve[0].rpm) return 0
  if (rpm > curve[curve.length - 1].rpm) return 0

  for (let i = 0; i < curve.length - 1; i++) {
    const a = curve[i]
    const b = curve[i + 1]
    if (rpm === a.rpm) return a.torqueNm
    if (rpm > a.rpm && rpm <= b.rpm) {
      const t = (rpm - a.rpm) / (b.rpm - a.rpm)
      return a.torqueNm + (b.torqueNm - a.torqueNm) * t
    }
  }

  // Exact match on first point handled by loop, last point:
  return curve[curve.length - 1].torqueNm
}

/**
 * Governor-limited torque: available torque scaled by throttle (0..1).
 */
export function computeGovernedTorque(
  currentRpm: number,
  throttle: number,
  curve: readonly TorqueCurvePoint[],
): number {
  return interpolateTorque(currentRpm, curve) * throttle
}

/**
 * Internal engine friction torque (linear model).
 * Always positive — resists rotation.
 */
export function engineFrictionTorque(rpm: number): number {
  return 0.015 * rpm + 5
}

/**
 * Integrate engine RPM forward by dt given net torque and inertia.
 * Includes idle governor, stall detection, and over-rev protection.
 */
export function updateEngineRpm(
  currentRpm: number,
  netTorqueNm: number,
  inertiaKgm2: number,
  dt: number,
  idleRpm: number,
  stallRpm: number,
): EngineOutput {
  const alpha = netTorqueNm / inertiaKgm2 // rad/s²
  const rpmDot = (alpha * 60) / TWO_PI    // RPM/s
  let newRpm = currentRpm + rpmDot * dt

  // Idle governor: prevent dropping below idle when net torque is non-negative
  if (newRpm < idleRpm && netTorqueNm >= 0) {
    newRpm = idleRpm
  }

  // Stall detection
  if (newRpm < stallRpm) {
    return { rpm: 0, stalled: true }
  }

  // Over-rev protection: clamp to 2300 RPM
  if (newRpm > 2300) {
    newRpm = 2300
  }

  return { rpm: newRpm, stalled: false }
}
