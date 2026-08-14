import { TRACTOR_CONFIG } from './TractorConfig'

export class TerrainSampler {
  private noiseFn: (x: number, z: number) => number

  constructor(noiseFn: (x: number, z: number) => number) {
    this.noiseFn = noiseFn
  }

  /** Get interpolated height at world position */
  getHeight(x: number, z: number): number {
    return this.noiseFn(x, z)
  }

  /**
   * Sample 4 points around (x,z) to compute surface pitch and roll.
   * sampleDist = distance between sample points (default 1.0m)
   * Returns pitch (rotation around X axis) and roll (rotation around Z axis) in radians.
   */
  getSurfaceNormal(x: number, z: number, sampleDist = 1.0): { pitch: number; roll: number; normalY: number } {
    const hN = this.noiseFn(x, z - sampleDist)  // north
    const hS = this.noiseFn(x, z + sampleDist)  // south
    const hW = this.noiseFn(x - sampleDist, z)  // west
    const hE = this.noiseFn(x + sampleDist, z)  // east

    // pitch = slope in Z direction (forward/back tilt)
    const pitch = Math.atan2(hS - hN, 2 * sampleDist)
    // roll = slope in X direction (left/right tilt)
    const roll = Math.atan2(hE - hW, 2 * sampleDist)
    // normalY for upward component
    const normalY = 1.0 / Math.sqrt(1 + pitch * pitch + roll * roll)

    return { pitch, roll, normalY }
  }

  /**
   * Get vehicle body height and orientation given 4 wheel positions.
   * wheelbase = front-to-rear axle distance
   * trackWidth = left-to-right wheel distance
   * Returns center height, pitch, roll
   */
  getVehiclePose(
    x: number, z: number, headingRad: number,
    wheelbase = TRACTOR_CONFIG.wheelbaseM,
    frontTrackWidth = TRACTOR_CONFIG.frontTrackM,
    rearTrackWidth = TRACTOR_CONFIG.rearTrackM,
  ): { y: number; pitch: number; roll: number } {
    const cosH = Math.cos(headingRad)
    const sinH = Math.sin(headingRad)
    const hw = wheelbase / 2
    const frontHalfTrack = frontTrackWidth / 2
    const rearHalfTrack = rearTrackWidth / 2

    // 4 wheel positions in world coords
    const fl = this.getHeight(x + sinH * hw - cosH * frontHalfTrack, z + cosH * hw + sinH * frontHalfTrack)  // front-left
    const fr = this.getHeight(x + sinH * hw + cosH * frontHalfTrack, z + cosH * hw - sinH * frontHalfTrack)  // front-right
    const rl = this.getHeight(x - sinH * hw - cosH * rearHalfTrack, z - cosH * hw + sinH * rearHalfTrack)  // rear-left
    const rr = this.getHeight(x - sinH * hw + cosH * rearHalfTrack, z - cosH * hw - sinH * rearHalfTrack)  // rear-right

    const frontAvg = (fl + fr) / 2
    const rearAvg = (rl + rr) / 2
    const leftAvg = (fl + rl) / 2
    const rightAvg = (fr + rr) / 2
    const centerY = (fl + fr + rl + rr) / 4
    const avgTrackWidth = (frontTrackWidth + rearTrackWidth) / 2

    const pitch = Math.atan2(frontAvg - rearAvg, wheelbase)
    const roll = Math.atan2(rightAvg - leftAvg, avgTrackWidth)

    return { y: centerY, pitch, roll }
  }
}
