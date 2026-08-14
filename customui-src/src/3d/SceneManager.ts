import * as THREE from 'three'
import { RoomEnvironment } from 'three/examples/jsm/environments/RoomEnvironment.js'
import { ShadowSystem } from './ShadowSystem'
import { PostProcessing } from './PostProcessing'
import { QUALITY_PRESETS, type QualityPresetName, loadQualityPreset } from './QualityPreset'

export class SceneManager {
  readonly scene: THREE.Scene
  readonly camera: THREE.PerspectiveCamera
  readonly renderer: THREE.WebGLRenderer
  shadowSystem: ShadowSystem | null
  postProcessing: PostProcessing | null
  private sun: THREE.DirectionalLight
  private container: HTMLElement
  private envTex: THREE.Texture | null
  private currentPreset: QualityPresetName

  constructor(container: HTMLElement) {
    this.container = container
    this.scene = new THREE.Scene()
    this.camera = new THREE.PerspectiveCamera(55, container.clientWidth / container.clientHeight, 0.1, 3000)

    this.renderer = new THREE.WebGLRenderer({ antialias: true })
    this.renderer.setSize(container.clientWidth, container.clientHeight)
    this.renderer.toneMapping = THREE.ACESFilmicToneMapping
    this.renderer.toneMappingExposure = 1.0
    this.renderer.outputColorSpace = THREE.SRGBColorSpace
    this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 1.5))
    this.renderer.shadowMap.enabled = true
    this.renderer.shadowMap.type = THREE.PCFSoftShadowMap
    container.appendChild(this.renderer.domElement)

    this.sun = this.setupLighting()

    const pmrem = new THREE.PMREMGenerator(this.renderer)
    this.envTex = pmrem.fromScene(new RoomEnvironment(), 0.04).texture
    this.scene.environment = this.envTex
    pmrem.dispose()

    this.shadowSystem = new ShadowSystem(this.scene, this.camera, {
      enabled: true,
      cascades: 3,
      maxFar: 300,
      shadowMapSize: 2048,
      lightDirection: new THREE.Vector3(-1, -1, -0.5),
    })

    this.postProcessing = new PostProcessing(this.renderer, this.scene, this.camera, {
      enabled: true,
      bloom: true,
      ssao: true,
      antialias: true,
    })

    this.currentPreset = loadQualityPreset()
    this.applyQualityPreset(this.currentPreset)
  }

  private setupLighting(): THREE.DirectionalLight {
    this.scene.add(new THREE.AmbientLight(0xffffff, 0.25))
    const sun = new THREE.DirectionalLight(0xffffff, 1.0)
    sun.position.set(50, 80, 30)
    sun.castShadow = false
    this.scene.add(sun)
    return sun
  }

  getSun(): THREE.DirectionalLight {
    return this.sun
  }

  setupShadowMaterial(material: THREE.Material): void {
    this.shadowSystem?.setupMaterial(material)
  }

  applyQualityPreset(name: QualityPresetName): void {
    const spec = QUALITY_PRESETS[name]
    this.currentPreset = name
    this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, spec.pixelRatioCap))
    this.shadowSystem?.setEnabled(spec.shadow.enabled)
    this.shadowSystem?.setShadowMapSize(spec.shadow.shadowMapSize)
    this.postProcessing?.setEnabled(spec.post.enabled)
    this.postProcessing?.setOptions({
      enabled: spec.post.enabled,
      bloom: spec.post.bloom,
      ssao: spec.post.ssao,
      antialias: spec.post.antialias,
    })
  }

  resize(): void {
    const w = this.container.clientWidth
    const h = this.container.clientHeight
    this.camera.aspect = w / h
    this.camera.updateProjectionMatrix()
    this.renderer.setSize(w, h)
    this.postProcessing?.setSize(w, h)
  }

  render(): void {
    this.shadowSystem?.update()
    if (this.postProcessing?.enabled) {
      this.postProcessing.render()
    } else {
      this.renderer.render(this.scene, this.camera)
    }
  }

  dispose(): void {
    this.shadowSystem?.dispose()
    this.postProcessing?.dispose()
    this.envTex?.dispose()
    this.renderer.dispose()
    if (this.container.contains(this.renderer.domElement)) {
      this.container.removeChild(this.renderer.domElement)
    }
  }
}
