import * as THREE from 'three'
import {
  BloomEffect,
  EffectComposer,
  EffectPass,
  FXAAEffect,
  NormalPass,
  RenderPass,
  SMAAEffect,
  SSAOEffect,
  ToneMappingEffect,
  ToneMappingMode,
} from 'postprocessing'

export interface PostProcessingOptions {
  enabled: boolean
  bloom?: boolean
  ssao?: boolean
  antialias?: boolean
}

type AntialiasKind = 'smaa' | 'fxaa' | 'none'

export class PostProcessing {
  public composer: EffectComposer
  public enabled: boolean

  private renderer: THREE.WebGLRenderer
  private originalToneMapping: THREE.ToneMapping
  private scene: THREE.Scene
  private camera: THREE.Camera
  private opts: PostProcessingOptions

  private renderPass: RenderPass | null = null
  private normalPass: NormalPass | null = null
  private effectPass: EffectPass | null = null
  private aaPass: EffectPass | null = null

  constructor(
    renderer: THREE.WebGLRenderer,
    scene: THREE.Scene,
    camera: THREE.Camera,
    opts: PostProcessingOptions,
  ) {
    this.renderer = renderer
    this.originalToneMapping = renderer.toneMapping
    this.scene = scene
    this.camera = camera
    this.opts = {
      enabled: opts.enabled,
      bloom: opts.bloom ?? true,
      ssao: opts.ssao ?? true,
      antialias: opts.antialias ?? true,
    }
    this.enabled = this.opts.enabled

    this.composer = new EffectComposer(renderer)
    this.buildPipeline()

    if (this.enabled) this.renderer.toneMapping = THREE.NoToneMapping
  }

  private buildPipeline(): void {
    // Clear existing passes.
    for (const p of [...this.composer.passes]) {
      this.composer.removePass(p)
    }
    this.renderPass = null
    this.normalPass = null
    this.effectPass = null
    this.aaPass = null

    this.renderPass = new RenderPass(this.scene, this.camera)
    this.composer.addPass(this.renderPass)

    const effects: Array<BloomEffect | SSAOEffect | ToneMappingEffect> = []

    if (this.opts.ssao) {
      this.normalPass = new NormalPass(this.scene, this.camera)
      this.composer.addPass(this.normalPass)

      const ssao = new SSAOEffect(this.camera, this.normalPass.texture, {
        samples: 9,
        rings: 7,
        radius: 0.1825,
        intensity: 1.0,
        bias: 0.025,
        fade: 0.01,
      })
      effects.push(ssao)
    }

    if (this.opts.bloom) {
      const bloom = new BloomEffect({
        luminanceThreshold: 0.95,
        intensity: 0.3,
      })
      effects.push(bloom)
    }

    // Tone mapping must be the last color-transforming effect (before AA).
    const tmEffect = new ToneMappingEffect({ mode: ToneMappingMode.ACES_FILMIC })
    effects.push(tmEffect)

    if (effects.length > 0) {
      this.effectPass = new EffectPass(this.camera, ...effects)
      this.composer.addPass(this.effectPass)
    }

    if (this.opts.antialias) {
      // Prefer SMAA when available; fall back to FXAA.
      const aa = this.createAntialias('smaa')
      this.aaPass = new EffectPass(this.camera, aa)
      this.composer.addPass(this.aaPass)
    }
  }

  private createAntialias(kind: AntialiasKind): SMAAEffect | FXAAEffect {
    if (kind === 'fxaa') return new FXAAEffect()
    return new SMAAEffect()
  }

  render(): void {
    if (!this.enabled) return
    this.composer.render()
  }

  setSize(w: number, h: number): void {
    this.composer.setSize(w, h)
  }

  setEnabled(v: boolean): void {
    this.enabled = v
    this.opts.enabled = v
    if (v) this.renderer.toneMapping = THREE.NoToneMapping
    else this.renderer.toneMapping = this.originalToneMapping
  }

  setOptions(opts: Partial<PostProcessingOptions>): void {
    this.opts = { ...this.opts, ...opts }
    this.enabled = this.opts.enabled
    if (this.enabled) this.renderer.toneMapping = THREE.NoToneMapping
    else this.renderer.toneMapping = this.originalToneMapping
    this.buildPipeline()
  }

  dispose(): void {
    this.composer.dispose()
    this.renderer.toneMapping = this.originalToneMapping
  }
}
