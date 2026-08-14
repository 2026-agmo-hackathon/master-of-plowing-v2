export interface HitchState {
  position: number       // 0 = fully raised, 1 = fully lowered (ground contact)
  implementMassKg: number
  hitchOffsetM: number   // distance behind rear axle
}

export interface WeightTransfer {
  frontLoadDelta: number    // N (negative = front lighter)
  rearLoadDelta: number     // N (positive = rear heavier)
  pitchDelta: number        // rad (negative = nose up from heavy rear)
}

export class ThreePointHitch {
  private position = 0         // 0=raised, 1=lowered
  private targetPosition = 0
  private readonly liftSpeed = 0.5  // positions per second (2 seconds full travel)

  setTarget(position: number): void {
    this.targetPosition = Math.max(0, Math.min(1, position))
  }

  update(dt: number): void {
    const diff = this.targetPosition - this.position
    if (Math.abs(diff) < 1e-6) {
      this.position = this.targetPosition
      return
    }
    const step = this.liftSpeed * dt
    if (Math.abs(diff) <= step) {
      this.position = this.targetPosition
    } else {
      this.position += Math.sign(diff) * step
    }
  }

  getPosition(): number {
    return this.position
  }

  computeWeightTransfer(
    implementMassKg: number,
    hitchOffsetM: number,
    wheelbaseM: number,
  ): WeightTransfer {
    // When lowered, implement weight transfers to rear axle
    // Moment about front axle: F_rear_delta = implementMass * g * (wheelbase + hitchOffset) / wheelbase
    // Front axle loses: F_front_delta = -implementMass * g * hitchOffset / wheelbase
    // Scale by position (0=no effect, 1=full effect)
    const g = 9.81
    const effectiveWeight = implementMassKg * g * this.position
    const rearDelta = effectiveWeight * (wheelbaseM + hitchOffsetM) / wheelbaseM
    const frontDelta = -effectiveWeight * hitchOffsetM / wheelbaseM
    const pitchDelta = -Math.atan(this.position * implementMassKg * 0.0001)  // subtle nose-up
    return { frontLoadDelta: frontDelta, rearLoadDelta: rearDelta, pitchDelta }
  }
}
