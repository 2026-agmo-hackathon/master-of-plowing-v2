import * as THREE from 'three'
import { Sky } from 'three/examples/jsm/objects/Sky.js'

export class SkyRenderer {
  private sky: Sky
  private sunDir: THREE.Vector3

  constructor(scene: THREE.Scene) {
    this.sky = new Sky()
    this.sky.scale.setScalar(500)
    const uniforms = this.sky.material.uniforms
    uniforms.turbidity.value = 10
    uniforms.rayleigh.value = 2
    uniforms.mieCoefficient.value = 0.005
    uniforms.mieDirectionalG.value = 0.8
    this.sunDir = new THREE.Vector3(0, 1, 0)
    scene.add(this.sky)
    this.setTimeOfDay(12)

    scene.fog = new THREE.FogExp2(0xb7d4e8, 0.0008)
  }

  setTimeOfDay(hours: number): void {
    const altitude = ((hours - 6) / 6) * (Math.PI / 2)
    const azimuth = Math.PI / 2 + (hours / 24) * Math.PI
    const sinAlt = Math.sin(altitude)
    const cosAlt = Math.cos(altitude)
    this.sunDir.set(
      cosAlt * Math.cos(azimuth),
      sinAlt,
      cosAlt * Math.sin(azimuth),
    )
    this.sky.material.uniforms.sunPosition.value.copy(this.sunDir)
  }

  update(sunDir: THREE.Vector3): void {
    this.sunDir.copy(sunDir)
    this.sky.material.uniforms.sunPosition.value.copy(this.sunDir)
  }

  getSunDirection(): THREE.Vector3 {
    return this.sunDir
  }

  dispose(): void {
    this.sky.geometry.dispose()
    ;(this.sky.material as THREE.Material).dispose()
  }
}
