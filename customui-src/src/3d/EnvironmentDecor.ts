import * as THREE from 'three'
import { seededRandom } from './ProceduralTextures'
import type { TerrainSampler } from '../simulation/TerrainSampler'

/**
 * Deterministic environment decoration using InstancedMesh.
 * Places trees, fences, barns, silos outside the central field rectangle.
 */
export class EnvironmentDecor {
  private scene: THREE.Scene
  private meshes: THREE.InstancedMesh[] = []
  private materials: THREE.Material[] = []
  private geometries: THREE.BufferGeometry[] = []
  private counts: { tree: number; fence: number; barn: number; silo: number } = {
    tree: 0, fence: 0, barn: 0, silo: 0,
  }
  // Area: 500x500m, exclusion rectangle |x|<100, |z|<200.
  private readonly AREA_HALF = 250
  private readonly EXCLUDE_HALF_X = 100
  private readonly EXCLUDE_HALF_Z = 200
  // Per-type instance capacity (<= 500 per TODO constraint).
  // TODO 5: tree instance removed — CAP_TREE dropped (effectively 0); fence/barn/silo caps raised to preserve total placement >= 800.
  private readonly CAP_FENCE = 300
  private readonly CAP_BARN = 300
  private readonly CAP_SILO = 300
  private readonly TARGET_TOTAL = 900
  private readonly MAX_ATTEMPTS = 20000

  constructor(scene: THREE.Scene, terrainSampler: TerrainSampler) {
    this.scene = scene

    // TODO 5: tree instance removed — tree geometry/material/mesh no longer created or added to scene.
    const fenceGeo = new THREE.BoxGeometry(2.0, 1.0, 0.1)
    const barnGeo = new THREE.BoxGeometry(6, 4, 8)
    const siloGeo = new THREE.CylinderGeometry(1.5, 1.5, 8, 16)
    this.geometries.push(fenceGeo, barnGeo, siloGeo)

    const fenceMat = new THREE.MeshStandardMaterial({ color: 0x6b4423, roughness: 0.85, metalness: 0.0 })
    const barnMat = new THREE.MeshStandardMaterial({ color: 0x8b0000, roughness: 0.7, metalness: 0.05 })
    const siloMat = new THREE.MeshStandardMaterial({ color: 0xb0b0b0, roughness: 0.4, metalness: 0.6 })
    this.materials.push(fenceMat, barnMat, siloMat)

    const fenceMesh = new THREE.InstancedMesh(fenceGeo, fenceMat, this.CAP_FENCE)
    const barnMesh = new THREE.InstancedMesh(barnGeo, barnMat, this.CAP_BARN)
    const siloMesh = new THREE.InstancedMesh(siloGeo, siloMat, this.CAP_SILO)
    fenceMesh.castShadow = true; fenceMesh.receiveShadow = true
    barnMesh.castShadow = true; barnMesh.receiveShadow = true
    siloMesh.castShadow = true; siloMesh.receiveShadow = true

    // Vertical offsets to seat each geometry on ground (y = terrain height).
    // Fence (Box 2x1x0.1): origin center, height 1 → base at -0.5. Lift by +0.5.
    // Barn (Box 6x4x8): origin center → base at -2. Lift by +2.
    // Silo (Cylinder h=8): origin center → base at -4. Lift by +4.
    // TODO 5: tree instance removed — tree entry dropped from typeCycle / caps / cursors.
    const yOffsets = { fence: 0.5, barn: 2.0, silo: 4.0 }

    const rnd = seededRandom(7777)
    const dummy = new THREE.Object3D()
    const typeCycle: Array<'fence' | 'barn' | 'silo'> = ['fence', 'barn', 'silo']
    const caps = { fence: this.CAP_FENCE, barn: this.CAP_BARN, silo: this.CAP_SILO }
    const cursors = { fence: 0, barn: 0, silo: 0 }

    let placed = 0
    let attempts = 0
    while (placed < this.TARGET_TOTAL && attempts < this.MAX_ATTEMPTS) {
      attempts++
      const x = (rnd() - 0.5) * this.AREA_HALF * 2
      const z = (rnd() - 0.5) * this.AREA_HALF * 2
      if (Math.abs(x) < this.EXCLUDE_HALF_X && Math.abs(z) < this.EXCLUDE_HALF_Z) {
        continue  // inside central field exclusion zone
      }
      const type = typeCycle[placed % typeCycle.length]
      if (cursors[type] >= caps[type]) {
        // Cap hit for this type — try next type in cycle for this iteration.
        let found: 'fence' | 'barn' | 'silo' | null = null
        for (const t of typeCycle) {
          if (cursors[t] < caps[t]) { found = t; break }
        }
        if (!found) break
        const y = terrainSampler.getHeight(x, z) + yOffsets[found]
        const rotY = rnd() * Math.PI * 2
        const scale = 0.9 + rnd() * 0.4
        dummy.position.set(x, y, z)
        dummy.rotation.set(0, rotY, 0)
        dummy.scale.setScalar(scale)
        dummy.updateMatrix()
        const mesh = this.meshForType(found, fenceMesh, barnMesh, siloMesh)
        mesh.setMatrixAt(cursors[found], dummy.matrix)
        cursors[found]++
        placed++
        continue
      }
      const y = terrainSampler.getHeight(x, z) + yOffsets[type]
      const rotY = rnd() * Math.PI * 2
      const scale = 0.9 + rnd() * 0.4
      dummy.position.set(x, y, z)
      dummy.rotation.set(0, rotY, 0)
      dummy.scale.setScalar(scale)
      dummy.updateMatrix()
      const mesh = this.meshForType(type, fenceMesh, barnMesh, siloMesh)
      mesh.setMatrixAt(cursors[type], dummy.matrix)
      cursors[type]++
      placed++
    }

    // Finalize: set count and mark matrix dirty on each mesh.
    // TODO 5: tree instance removed — tree count always 0.
    fenceMesh.count = cursors.fence
    barnMesh.count = cursors.barn
    siloMesh.count = cursors.silo
    fenceMesh.instanceMatrix.needsUpdate = true
    barnMesh.instanceMatrix.needsUpdate = true
    siloMesh.instanceMatrix.needsUpdate = true

    this.counts = {
      tree: 0,
      fence: cursors.fence,
      barn: cursors.barn,
      silo: cursors.silo,
    }

    // TODO 5: tree instance removed — tree mesh not added to scene.
    scene.add(fenceMesh, barnMesh, siloMesh)
    this.meshes.push(fenceMesh, barnMesh, siloMesh)
  }

  private meshForType(
    type: 'fence' | 'barn' | 'silo',
    fence: THREE.InstancedMesh,
    barn: THREE.InstancedMesh,
    silo: THREE.InstancedMesh,
  ): THREE.InstancedMesh {
    if (type === 'fence') return fence
    if (type === 'barn') return barn
    return silo
  }

  getMaterials(): THREE.Material[] {
    return this.materials
  }

  getCounts(): { tree: number; fence: number; barn: number; silo: number } {
    return { ...this.counts }
  }

  dispose(): void {
    for (const m of this.meshes) {
      this.scene.remove(m)
    }
    for (const g of this.geometries) g.dispose()
    for (const m of this.materials) m.dispose()
  }
}
