import * as THREE from 'three'
import type { MapType } from '../simulation/MapTypes'
import {
  makeCultivatedTexture,
  makeDirtTexture,
  makeGrassTexture,
  makeRoadTexture,
  seededRandom,
} from './ProceduralTextures'

export class TerrainRenderer {
  private groundMesh: THREE.Mesh
  private groundGeo: THREE.BufferGeometry
  private scene: THREE.Scene
  private disposables: (THREE.BufferGeometry | THREE.Material | THREE.Texture)[] = []
  private materials: THREE.Material[] = []
  private dirtTex: THREE.DataTexture
  private grassTex: THREE.DataTexture
  private cultivatedTex: THREE.DataTexture | null = null
  private roadTex: THREE.DataTexture | null = null
  private splatmap: THREE.Texture | null = null
  private mapSize: { width: number; height: number }

  constructor(
    scene: THREE.Scene,
    terrainNoiseFn: (x: number, z: number) => number,
    mapType: MapType,
    splatmap?: THREE.Texture,
  ) {
    this.scene = scene
    const FIELD_W = 1000, FIELD_H = 2000
    const HALF_W = FIELD_W / 2, HALF_H = FIELD_H / 2
    this.mapSize = { width: FIELD_W + 200, height: FIELD_H + 200 }

    // Procedural textures for triplanar blending
    this.dirtTex = makeDirtTexture()
    this.grassTex = makeGrassTexture()
    this.disposables.push(this.dirtTex, this.grassTex)

    if (splatmap) {
      this.splatmap = splatmap
      this.cultivatedTex = makeCultivatedTexture()
      this.roadTex = makeRoadTexture()
      this.disposables.push(this.cultivatedTex, this.roadTex)
    }

    // Ground mesh — 200x200 segments for visible ruts (~3m per segment)
    this.groundGeo = new THREE.PlaneGeometry(FIELD_W + 200, FIELD_H + 200, 200, 200)
    const gPos = this.groundGeo.attributes.position
    for (let i = 0; i < gPos.count; i++) {
      gPos.setZ(i, terrainNoiseFn(gPos.getX(i), gPos.getY(i)))
    }
    this.groundGeo.computeVertexNormals()

    // Vertex colors: height-based gradient for natural contour feeling
    const colors = new Float32Array(gPos.count * 3)
    for (let i = 0; i < gPos.count; i++) {
      const h = gPos.getZ(i)
      TerrainRenderer.setHeightColor(colors, i, h)
    }
    this.groundGeo.setAttribute('color', new THREE.BufferAttribute(colors, 3))

    const groundMat = new THREE.MeshStandardMaterial({
      vertexColors: true,
      roughness: 0.9,
      metalness: 0.0,
    })
    // Inject triplanar shader — XYZ 3-face UV blend with dirt/grass textures.
    this.injectTriplanarShader(groundMat)
    this.disposables.push(this.groundGeo, groundMat)
    this.materials.push(groundMat)
    this.groundMesh = new THREE.Mesh(this.groundGeo, groundMat)
    this.groundMesh.rotation.x = -Math.PI / 2
    this.groundMesh.receiveShadow = true
    scene.add(this.groundMesh)

    // Grass instancing — small green blades scattered across field
    // this.addGrass(terrainNoiseFn, FIELD_W, FIELD_H) // Kept for future re-enable

    // Crop rows — activated externally via addCropRowsForFields()

    if (mapType !== 'flat') {
      this.addLevees(FIELD_W, FIELD_H, HALF_W, HALF_H)
      // this.addTrees(FIELD_W, FIELD_H, HALF_W, HALF_H) // Kept for future re-enable
      // this.addWater(FIELD_W, HALF_H) // Kept for future re-enable
    }

    // Reference deprecated helpers so TS noUnusedLocals doesn't complain.
    // Kept for future re-enable.
    void this.addGrass
    void this.addTrees
    void this.addWater
  }

  private injectTriplanarShader(material: THREE.MeshStandardMaterial): void {
    const dirtTex = this.dirtTex
    const grassTex = this.grassTex
    const splatmap = this.splatmap
    const cultivatedTex = this.cultivatedTex
    const roadTex = this.roadTex
    const mapSize = this.mapSize

    if (splatmap) {
      material.defines = { ...(material.defines || {}), HAS_SPLATMAP: '' }
    }

    material.onBeforeCompile = (shader) => {
      shader.uniforms.uDirtTex = { value: dirtTex }
      shader.uniforms.uGrassTex = { value: grassTex }
      shader.uniforms.uTileSize = { value: 0.25 }

      if (splatmap && cultivatedTex && roadTex) {
        shader.uniforms.uSplatmap = { value: splatmap }
        shader.uniforms.uCultivatedTex = { value: cultivatedTex }
        shader.uniforms.uRoadTex = { value: roadTex }
        shader.uniforms.uMapSize = { value: new THREE.Vector2(mapSize.width, mapSize.height) }
      }

      shader.vertexShader = shader.vertexShader
        .replace(
          '#include <common>',
          `#include <common>\nvarying vec3 vWorldPos;\nvarying vec3 vWorldNormal;`
        )
        .replace(
          '#include <worldpos_vertex>',
          `#include <worldpos_vertex>\n  vWorldPos = (modelMatrix * vec4(transformed, 1.0)).xyz;\n  vWorldNormal = normalize(mat3(modelMatrix) * objectNormal);`
        )

      const commonInjection = `#include <common>
uniform sampler2D uDirtTex;
uniform sampler2D uGrassTex;
uniform float uTileSize;
varying vec3 vWorldPos;
varying vec3 vWorldNormal;
#ifdef HAS_SPLATMAP
uniform sampler2D uSplatmap;
uniform sampler2D uCultivatedTex;
uniform sampler2D uRoadTex;
uniform vec2 uMapSize;
#endif
vec3 triplanar(sampler2D t, vec3 p, vec3 n, float scale) {
  vec3 blend = abs(n);
  blend /= (blend.x + blend.y + blend.z + 1e-5);
  vec3 cx = texture2D(t, p.yz * scale).rgb;
  vec3 cy = texture2D(t, p.xz * scale).rgb;
  vec3 cz = texture2D(t, p.xy * scale).rgb;
  return cx * blend.x + cy * blend.y + cz * blend.z;
}`

      const colorInjection = `#include <color_fragment>
  vec3 nrm = normalize(vWorldNormal);
  vec3 triDirt = triplanar(uDirtTex, vWorldPos, nrm, uTileSize);
  vec3 triGrass = triplanar(uGrassTex, vWorldPos, nrm, uTileSize);
#ifdef HAS_SPLATMAP
  vec2 mapUV = (vWorldPos.xz + uMapSize * 0.5) / uMapSize;
  vec4 splat = texture2D(uSplatmap, mapUV);
  float sum = max(splat.r + splat.g + splat.b + splat.a, 0.001);
  vec3 triCult = triplanar(uCultivatedTex, vWorldPos, nrm, uTileSize);
  vec3 triRoad = triplanar(uRoadTex, vWorldPos, nrm, uTileSize);
  vec3 blended = (triDirt * splat.r + triGrass * splat.g + triCult * splat.b + triRoad * splat.a) / sum;
  diffuseColor.rgb *= blended * 2.0;
#else
  vec3 triDirtBright = triDirt * 1.05;
  float w = clamp(abs(nrm.y), 0.0, 1.0);
  vec3 triCol = mix(triDirt, triDirtBright, w);
  diffuseColor.rgb *= triCol * 2.0;
#endif`

      shader.fragmentShader = shader.fragmentShader
        .replace('#include <common>', commonInjection)
        .replace('#include <color_fragment>', colorInjection)
    }
  }

  /** @deprecated Kept for future re-enable. Currently not called. */
  private addGrass(noiseFn: (x: number, z: number) => number, fw: number, fh: number): void {
    const rnd = seededRandom(42)
    const bladeGeo = new THREE.PlaneGeometry(0.1, 0.4)
    bladeGeo.translate(0, 0.2, 0)  // pivot at base
    const bladeMat = new THREE.MeshStandardMaterial({
      color: 0x4CAF50,
      side: THREE.DoubleSide,
      roughness: 0.9,
      metalness: 0.0,
    })
    const count = 3000
    const mesh = new THREE.InstancedMesh(bladeGeo, bladeMat, count)
    const dummy = new THREE.Object3D()
    for (let i = 0; i < count; i++) {
      const x = (rnd() - 0.5) * fw * 0.9
      const z = (rnd() - 0.5) * fh * 0.9
      const y = noiseFn(x, z)
      dummy.position.set(x, y, z)
      dummy.rotation.y = rnd() * Math.PI * 2
      dummy.scale.setScalar(0.8 + rnd() * 0.4)
      dummy.updateMatrix()
      mesh.setMatrixAt(i, dummy.matrix)
    }
    mesh.instanceMatrix.needsUpdate = true
    this.scene.add(mesh)
    this.disposables.push(bladeGeo, bladeMat)
    this.materials.push(bladeMat)
  }

  /**
   * 각 field 폴리곤 영역 안에 밭고랑(furrow/ridge)을 렌더링합니다.
   * - furrow(고랑): 어두운 흙색 0x3e2010
   * - ridge(두둑): 경작지 흙색 0x7a5c3a
   * crop별 고랑 간격: rice=0.6m, wheat=0.25m, corn=0.8m, 기본=0.5m
   */
  addCropRows(
    noiseFn: (x: number, z: number) => number,
    fields: Array<{ polygon: [number, number][]; crop?: string }>,
  ): void {
    const furrowMat = new THREE.MeshStandardMaterial({ color: 0x3e2010, roughness: 1.0, metalness: 0.0 })
    const ridgeMat  = new THREE.MeshStandardMaterial({ color: 0x7a5c3a, roughness: 1.0, metalness: 0.0 })
    this.disposables.push(furrowMat, ridgeMat)
    this.materials.push(furrowMat, ridgeMat)

    const spacingByCrop: Record<string, number> = {
      rice: 0.6,
      wheat: 0.25,
      corn: 0.8,
    }

    for (const field of fields) {
      const poly = field.polygon  // [x, z][] in world units
      if (!poly || poly.length < 3) continue

      const spacing = spacingByCrop[field.crop ?? ''] ?? 0.5

      // Bounding box of polygon
      let minX = Infinity, maxX = -Infinity, minZ = Infinity, maxZ = -Infinity
      for (const [px, pz] of poly) {
        if (px < minX) minX = px
        if (px > maxX) maxX = px
        if (pz < minZ) minZ = pz
        if (pz > maxZ) maxZ = pz
      }

      const fieldWidth  = maxX - minX
      const furrowH     = 0.06   // 고랑 높이(m)
      const ridgeH      = 0.12   // 두둑 높이(m)
      const furrowDepth = spacing * 0.4   // 고랑 Z 폭
      const ridgeDepth  = spacing * 0.6   // 두둑 Z 폭

      let rowIdx = 0
      for (let z = minZ + spacing / 2; z <= maxZ - spacing / 2; z += spacing, rowIdx++) {
        const isFurrow = rowIdx % 2 === 0
        const h     = isFurrow ? furrowH : ridgeH
        const depth = isFurrow ? furrowDepth : ridgeDepth
        const mat   = isFurrow ? furrowMat : ridgeMat

        // X range clipped to polygon bounding box (polygon is axis-aligned rectangle in all default fields)
        const geo = new THREE.BoxGeometry(fieldWidth, h, depth)
        const centerX = (minX + maxX) / 2
        const mesh = new THREE.Mesh(geo, mat)
        const groundY = noiseFn(centerX, z)
        mesh.position.set(centerX, groundY + h / 2, z)
        mesh.frustumCulled = false
        this.scene.add(mesh)
        this.disposables.push(geo)
      }
    }
  }

  private addLevees(fw: number, fh: number, hw: number, hh: number): void {
    const levMat = new THREE.MeshStandardMaterial({ color: 0x5D4037, roughness: 0.9, metalness: 0.0 })
    this.disposables.push(levMat)
    this.materials.push(levMat)
    const LEVEE_H = 4, LEVEE_W = 6
    // North/South
    for (const zPos of [-hh - LEVEE_W / 2, hh + LEVEE_W / 2]) {
      const geo = new THREE.BoxGeometry(fw + LEVEE_W * 2, LEVEE_H, LEVEE_W)
      const lev = new THREE.Mesh(geo, levMat)
      lev.position.set(0, LEVEE_H / 2, zPos)
      lev.receiveShadow = true; lev.castShadow = true
      this.scene.add(lev)
      this.disposables.push(geo)
    }
    // East/West
    for (const xPos of [-hw - LEVEE_W / 2, hw + LEVEE_W / 2]) {
      const geo = new THREE.BoxGeometry(LEVEE_W, LEVEE_H, fh + LEVEE_W * 2)
      const lev = new THREE.Mesh(geo, levMat)
      lev.position.set(xPos, LEVEE_H / 2, 0)
      lev.receiveShadow = true; lev.castShadow = true
      this.scene.add(lev)
      this.disposables.push(geo)
    }
  }

  /** @deprecated Kept for future re-enable. Currently not called. */
  private addTrees(fw: number, fh: number, hw: number, hh: number): void {
    const rnd = seededRandom(2024)
    const LEVEE_W = 6
    const trunkMat = new THREE.MeshStandardMaterial({ color: 0x5D4037, roughness: 0.9, metalness: 0.0 })
    const leafMats = [
      new THREE.MeshStandardMaterial({ color: 0x2E7D32, roughness: 0.9, metalness: 0.0 }),
      new THREE.MeshStandardMaterial({ color: 0x43A047, roughness: 0.9, metalness: 0.0 }),
    ]
    this.disposables.push(trunkMat, ...leafMats)
    this.materials.push(trunkMat, ...leafMats)
    for (let i = 0; i < 80; i++) {
      const side = Math.floor(rnd() * 4)
      let tx: number, tz: number
      const offset = LEVEE_W + 5 + rnd() * 15
      if (side === 0) { tx = -hw - offset; tz = (rnd() - 0.5) * fh }
      else if (side === 1) { tx = hw + offset; tz = (rnd() - 0.5) * fh }
      else if (side === 2) { tx = (rnd() - 0.5) * fw; tz = -hh - offset }
      else { tx = (rnd() - 0.5) * fw; tz = hh + offset }
      const tH = 3 + rnd() * 4
      const tGeo = new THREE.CylinderGeometry(0.2, 0.3, tH, 6)
      const trunk = new THREE.Mesh(tGeo, trunkMat)
      trunk.position.set(tx, tH / 2, tz); trunk.castShadow = true
      this.scene.add(trunk)
      const lR = 1.5 + rnd() * 2
      const lGeo = new THREE.SphereGeometry(lR, 8, 6)
      const leaf = new THREE.Mesh(lGeo, leafMats[i % 2])
      leaf.position.set(tx, tH + lR * 0.5, tz); leaf.castShadow = true
      this.scene.add(leaf)
      this.disposables.push(tGeo, lGeo)
    }
  }

  /** @deprecated Kept for future re-enable. Currently not called. */
  private addWater(fw: number, hh: number): void {
    const geo = new THREE.BoxGeometry(fw, 0.1, 3)
    const mat = new THREE.MeshStandardMaterial({
      color: 0x1976D2,
      transparent: true,
      opacity: 0.6,
      roughness: 0.2,
      metalness: 0.0,
    })
    const water = new THREE.Mesh(geo, mat)
    water.position.set(0, 0.05, hh + 1)
    this.scene.add(water)
    this.disposables.push(geo, mat)
    this.materials.push(mat)
  }

  static readonly SEGMENTS = 200

  /**
   * Set vertex color based on height using a dirt-tone palette (4 bands).
   * h < -0.1: dark dirt (#4A3728)
   * -0.1 <= h < 0.05: mid dirt (#6B4A2E)
   * 0.05 <= h < 0.25: light dirt (#8B6B3E)
   * h >= 0.25: dry dirt (#A88957)
   */
  static setHeightColor(colors: Float32Array, i: number, h: number): void {
    let r: number, g: number, b: number
    if (h < -0.1) {
      r = 0.29; g = 0.22; b = 0.16
    } else if (h < 0.05) {
      r = 0.42; g = 0.29; b = 0.18
    } else if (h < 0.25) {
      r = 0.55; g = 0.42; b = 0.24
    } else {
      r = 0.66; g = 0.54; b = 0.34
    }
    colors[i * 3] = r
    colors[i * 3 + 1] = g
    colors[i * 3 + 2] = b
  }

  getGroundMesh(): THREE.Mesh {
    return this.groundMesh
  }

  getGroundGeometry(): THREE.BufferGeometry {
    return this.groundGeo
  }

  /** All materials owned by this renderer — callers (e.g. SceneManager) may
   * register them for CSM shadow setup. */
  getMaterials(): THREE.Material[] {
    return this.materials
  }

  dispose(): void {
    for (const d of this.disposables) d.dispose()
  }
}
