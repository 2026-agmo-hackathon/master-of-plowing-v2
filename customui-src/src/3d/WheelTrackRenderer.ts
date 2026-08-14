import * as THREE from 'three'
import { TRACTOR_CONFIG } from '../simulation/TractorConfig'

export class WheelTrackRenderer {
  private trackGeometry: THREE.BufferGeometry
  private trackMaterial: THREE.MeshStandardMaterial
  private trackMesh: THREE.Mesh
  private vertices: number[] = []
  private maxVertices = 20000 // limit for performance
  private lastPositions: {
    fl: THREE.Vector3
    fr: THREE.Vector3
    rl: THREE.Vector3
    rr: THREE.Vector3
  } | null = null

  constructor(scene: THREE.Scene) {
    this.trackMaterial = new THREE.MeshStandardMaterial({
      color: 0x3e2723, // dark brown
      transparent: true,
      opacity: 0.6,
      depthWrite: false,
      side: THREE.DoubleSide,
      roughness: 0.9,
      metalness: 0.0,
    })
    this.trackGeometry = new THREE.BufferGeometry()
    this.trackMesh = new THREE.Mesh(this.trackGeometry, this.trackMaterial)
    this.trackMesh.position.y = 0.01 // slightly above ground to prevent z-fighting
    this.trackMesh.receiveShadow = true
    scene.add(this.trackMesh)
  }

  getMaterials(): THREE.Material[] {
    return [this.trackMaterial]
  }

  update(
    vehicleX: number,
    vehicleZ: number,
    heading: number,
    speed: number,
    frontTrack: number,
    rearTrack: number,
    wheelbase: number,
    groundY: number,
  ): void {
    if (Math.abs(speed) < 0.05) return // no tracks when stopped

    // Compute 4 wheel positions in world space
    const cos = Math.cos(heading)
    const sin = Math.sin(heading)
    const hw = wheelbase / 2
    const fht = frontTrack / 2 // front half track width
    const rht = rearTrack / 2 // rear half track width
    const fl = new THREE.Vector3(
      vehicleX + sin * hw - cos * fht,
      groundY + 0.02,
      vehicleZ + cos * hw + sin * fht,
    )
    const fr = new THREE.Vector3(
      vehicleX + sin * hw + cos * fht,
      groundY + 0.02,
      vehicleZ + cos * hw - sin * fht,
    )
    const rl = new THREE.Vector3(
      vehicleX - sin * hw - cos * rht,
      groundY + 0.02,
      vehicleZ - cos * hw + sin * rht,
    )
    const rr = new THREE.Vector3(
      vehicleX - sin * hw + cos * rht,
      groundY + 0.02,
      vehicleZ - cos * hw - sin * rht,
    )

    if (this.lastPositions) {
      // Create quad strips for each rear wheel track
      const trackWidth = TRACTOR_CONFIG.rearWheelWidthM // tire width in meters
      this.addTrackSegment(this.lastPositions.rl, rl, trackWidth, heading)
      this.addTrackSegment(this.lastPositions.rr, rr, trackWidth, heading)

      this.rebuildGeometry()
    }

    this.lastPositions = { fl, fr, rl, rr }
  }

  private addTrackSegment(
    prev: THREE.Vector3,
    curr: THREE.Vector3,
    width: number,
    heading: number,
  ): void {
    if (this.vertices.length >= this.maxVertices * 3) {
      // Remove oldest vertices (shift array)
      this.vertices.splice(0, 18) // remove 2 triangles (6 verts * 3 components)
    }

    const perpX = (Math.cos(heading) * width) / 2
    const perpZ = (-Math.sin(heading) * width) / 2

    // Two triangles forming a quad
    this.vertices.push(
      prev.x - perpX, prev.y, prev.z - perpZ,
      prev.x + perpX, prev.y, prev.z + perpZ,
      curr.x - perpX, curr.y, curr.z - perpZ,

      curr.x - perpX, curr.y, curr.z - perpZ,
      prev.x + perpX, prev.y, prev.z + perpZ,
      curr.x + perpX, curr.y, curr.z + perpZ,
    )
  }

  private rebuildGeometry(): void {
    const posArray = new Float32Array(this.vertices)
    this.trackGeometry.setAttribute(
      'position',
      new THREE.BufferAttribute(posArray, 3),
    )
    this.trackGeometry.computeVertexNormals()
  }

  dispose(): void {
    this.trackGeometry.dispose()
    this.trackMaterial.dispose()
  }
}
