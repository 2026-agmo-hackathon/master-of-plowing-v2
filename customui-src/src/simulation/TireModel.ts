export interface TireParams {
  B: number  // stiffness factor
  C: number  // shape factor
  D: number  // peak value (mu * Fz)
  E: number  // curvature factor
}

export interface TireForces {
  longitudinal: number  // N, forward traction force
  lateral: number       // N, cornering force
  slipRatio: number     // longitudinal slip ratio [-1, 1]
  slipAngle: number     // lateral slip angle (rad)
}

/** Pacejka Magic Formula: F = D * sin(C * atan(B*x - E*(B*x - atan(B*x)))) */
export function magicFormula(slip: number, params: TireParams): number {
  const { B, C, D, E } = params
  const Bx = B * slip
  return D * Math.sin(C * Math.atan(Bx - E * (Bx - Math.atan(Bx))))
}

/**
 * Longitudinal slip ratio.
 * kappa = (omega*r - v) / max(|v|, |omega*r|, 0.1)
 * Positive = wheelspin, negative = braking. Clamped to [-1, 1].
 */
export function computeSlipRatio(
  wheelAngularVelocity: number,
  tireRadius: number,
  vehicleSpeed: number,
): number {
  const wheelSpeed = wheelAngularVelocity * tireRadius
  const denom = Math.max(Math.abs(vehicleSpeed), Math.abs(wheelSpeed), 0.1)
  const ratio = (wheelSpeed - vehicleSpeed) / denom
  return Math.max(-1, Math.min(1, ratio))
}

/**
 * Lateral slip angle.
 * alpha = atan2(vLat, max(|vLong|, 0.1))
 */
export function computeSlipAngle(
  lateralVelocity: number,
  longitudinalVelocity: number,
): number {
  // Use signed vLong to preserve correct sign in reverse motion.
  // Clamp magnitude to avoid singularity at very low speeds.
  const minSpeed = 0.1
  if (Math.abs(longitudinalVelocity) < minSpeed) {
    return Math.atan2(lateralVelocity, Math.sign(longitudinalVelocity) * minSpeed || minSpeed)
  }
  return Math.atan2(lateralVelocity, longitudinalVelocity)
}


/**
 * Full tire force computation with combined slip friction circle scaling.
 *
 * @param lateralPriority - When true, lateral force is preserved first within the
 *   friction circle and longitudinal force is clipped to the remaining capacity.
 *   Use for driven axles where cornering ability should not be consumed by traction.
 */
export function computeTireForces(
  wheelOmega: number,
  vLong: number,
  vLat: number,
  normalLoad: number,
  frictionCoeff: number,
  tireRadius: number,
  config: { B: number; C: number; E: number },
  lateralPriority = false,
): TireForces {
  const slipRatio = computeSlipRatio(wheelOmega, tireRadius, vLong)
  const slipAngle = computeSlipAngle(vLat, vLong)

  const D = frictionCoeff * normalLoad
  const params: TireParams = { B: config.B, C: config.C, D, E: config.E }

  let longitudinal = magicFormula(slipRatio, params)
  let lateral = magicFormula(slipAngle, params)

  // Combined slip: friction circle constraint on tire force magnitude.
  const forceMagnitude = Math.sqrt(longitudinal * longitudinal + lateral * lateral)
  if (D > 1e-6 && forceMagnitude > D) {
    if (lateralPriority) {
      // Lateral-priority: preserve lateral capacity first, clip longitudinal to remainder.
      // This prevents driven-axle traction from consuming cornering grip (oversteer mitigation).
      const lateralClamped = Math.max(-D, Math.min(D, lateral))
      const longCapacity = Math.sqrt(Math.max(0, D * D - lateralClamped * lateralClamped))
      longitudinal = Math.max(-longCapacity, Math.min(longCapacity, longitudinal))
      lateral = lateralClamped
    } else {
      // Default: scale both components proportionally.
      const scale = D / forceMagnitude
      longitudinal *= scale
      lateral *= scale
    }
  }

  return { longitudinal, lateral, slipRatio, slipAngle }
}

/** Rolling resistance: F = Fz * Crr */
export function computeRollingResistance(
  normalLoad: number,
  coeffRolling: number,
): number {
  return normalLoad * coeffRolling
}
