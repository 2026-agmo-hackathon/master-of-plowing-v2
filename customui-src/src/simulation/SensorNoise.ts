export class SensorNoise {
  private bias: number = 0
  private readonly biasDriftRate: number
  private readonly noiseStdDev: number

  constructor(noiseStdDev: number, biasDriftRate: number = 0) {
    this.noiseStdDev = noiseStdDev
    this.biasDriftRate = biasDriftRate
  }

  /** Apply noise + bias to a clean value */
  apply(cleanValue: number, dt: number): number {
    // Bias random walk: bias += gaussianNoise * biasDriftRate * sqrt(dt)
    this.bias += gaussianRandom() * this.biasDriftRate * Math.sqrt(dt)
    // White noise
    const noise = gaussianRandom() * this.noiseStdDev
    return cleanValue + this.bias + noise
  }

  /** Reset bias (e.g., on recalibration) */
  resetBias(): void {
    this.bias = 0
  }

  getBias(): number {
    return this.bias
  }
}

/** Box-Muller transform for Gaussian random */
function gaussianRandom(): number {
  let u1 = 0, u2 = 0
  while (u1 === 0) u1 = Math.random()
  while (u2 === 0) u2 = Math.random()
  return Math.sqrt(-2 * Math.log(u1)) * Math.cos(2 * Math.PI * u2)
}

export { gaussianRandom }
