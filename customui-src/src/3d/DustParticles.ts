import * as THREE from 'three'
import { seededRandom } from './ProceduralTextures'

export class DustParticles {
  private particles: THREE.Points
  private geometry: THREE.BufferGeometry
  private positions: Float32Array
  private velocities: Float32Array
  private lifetimes: Float32Array
  private readonly maxParticles = 200
  private nextParticle = 0
  private rnd = seededRandom(5150)

  constructor(scene: THREE.Scene) {
    this.positions = new Float32Array(this.maxParticles * 3)
    this.velocities = new Float32Array(this.maxParticles * 3)
    this.lifetimes = new Float32Array(this.maxParticles)

    // Initialize all particles below ground (hidden)
    for (let i = 0; i < this.maxParticles; i++) {
      this.positions[i * 3 + 1] = -100
    }

    this.geometry = new THREE.BufferGeometry()
    this.geometry.setAttribute(
      'position',
      new THREE.BufferAttribute(this.positions, 3),
    )

    const material = new THREE.PointsMaterial({
      color: 0xb8860b, // dark goldenrod (dust color)
      size: 0.5,
      transparent: true,
      opacity: 0.4,
      depthWrite: false,
      sizeAttenuation: true,
    })

    this.particles = new THREE.Points(this.geometry, material)
    scene.add(this.particles)
  }

  emit(x: number, y: number, z: number, speed: number): void {
    if (speed < 0.3) return // no dust when slow

    const count = Math.min(3, Math.floor(speed * 2)) // more particles at higher speed
    for (let i = 0; i < count; i++) {
      const idx = this.nextParticle % this.maxParticles
      this.positions[idx * 3] = x + (this.rnd() - 0.5) * 1.5
      this.positions[idx * 3 + 1] = y + 0.1
      this.positions[idx * 3 + 2] = z + (this.rnd() - 0.5) * 1.5
      this.velocities[idx * 3] = (this.rnd() - 0.5) * 0.5
      this.velocities[idx * 3 + 1] = 0.5 + this.rnd() * 1.0 // upward
      this.velocities[idx * 3 + 2] = (this.rnd() - 0.5) * 0.5
      this.lifetimes[idx] = 1.5 + this.rnd() * 1.0 // 1.5-2.5 seconds
      this.nextParticle++
    }
  }

  update(dt: number): void {
    for (let i = 0; i < this.maxParticles; i++) {
      if (this.lifetimes[i] <= 0) continue
      this.lifetimes[i] -= dt
      this.positions[i * 3] += this.velocities[i * 3] * dt
      this.positions[i * 3 + 1] += this.velocities[i * 3 + 1] * dt
      this.positions[i * 3 + 2] += this.velocities[i * 3 + 2] * dt
      this.velocities[i * 3 + 1] -= 1.5 * dt // gravity
      if (this.lifetimes[i] <= 0) {
        this.positions[i * 3 + 1] = -100 // hide below ground
      }
    }
    this.geometry.attributes.position.needsUpdate = true
  }

  dispose(): void {
    this.geometry.dispose()
    ;(this.particles.material as THREE.Material).dispose()
  }
}
