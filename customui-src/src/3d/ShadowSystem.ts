import * as THREE from 'three'
import { CSM } from 'three/examples/jsm/csm/CSM.js'

export interface ShadowSystemOptions {
  enabled: boolean
  cascades?: number
  maxFar?: number
  lightDirection?: THREE.Vector3
  shadowMapSize?: number
}

export class ShadowSystem {
  private csm: CSM | null = null
  private enabled: boolean
  private scene: THREE.Scene
  private camera: THREE.Camera
  private opts: ShadowSystemOptions

  constructor(scene: THREE.Scene, camera: THREE.Camera, opts: ShadowSystemOptions) {
    this.scene = scene
    this.camera = camera
    this.opts = opts
    this.enabled = opts.enabled
    if (this.enabled) this.create()
  }

  private create(): void {
    this.csm = new CSM({
      maxFar: this.opts.maxFar ?? 300,
      cascades: this.opts.cascades ?? 3,
      mode: 'practical',
      parent: this.scene,
      shadowMapSize: this.opts.shadowMapSize ?? 2048,
      lightDirection: (this.opts.lightDirection ?? new THREE.Vector3(-1, -1, -0.5)).normalize(),
      camera: this.camera as THREE.PerspectiveCamera,
    })
  }

  isEnabled(): boolean {
    return this.enabled
  }

  setupMaterial(material: THREE.Material): void {
    if (this.csm) this.csm.setupMaterial(material)
  }

  update(): void {
    if (this.csm) this.csm.update()
  }

  setEnabled(enabled: boolean): void {
    if (this.enabled === enabled) return
    this.enabled = enabled
    if (enabled) {
      if (!this.csm) this.create()
    } else {
      this.dispose()
    }
  }

  setShadowMapSize(size: number): void {
    this.opts.shadowMapSize = size
    if (this.enabled) {
      this.dispose()
      this.enabled = true
      this.create()
    }
  }

  dispose(): void {
    if (this.csm) {
      this.csm.dispose()
      this.csm = null
    }
  }
}
