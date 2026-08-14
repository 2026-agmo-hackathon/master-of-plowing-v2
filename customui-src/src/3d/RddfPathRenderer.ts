import * as THREE from 'three'
import { TRACTOR_CONFIG } from '../simulation/TractorConfig'

export interface RddfWaypointEnu {
  x: number
  z: number
  speed: number
  implementFlag: number
}

// Visual constants — adjust here for quick tuning
const REPEAT_M          = 2.0   // meters per one texture tile (V axis)
const SCROLL_SPEED      = 0.08  // UV units per second for V-axis animation
const Y_OFFSET          = 0.30  // above ridge crest (~0.24m) so ribbon is not hidden by ridges
const MITER_MIN_DENOM   = 0.25  // lower bound for miter denominator (≈ TARGET_MITER_MIN_DENOM)
const HALF_WIDTH_FACTOR = 0.6   // halfWidth = rearTrackM * HALF_WIDTH_FACTOR / 2? No — full: rearTrackM * factor

// halfWidth = rearTrackM * HALF_WIDTH_FACTOR  (total visual width = rearTrackM * factor * 2)
// Set HALF_WIDTH_FACTOR to control visual ribbon width relative to rear track
const HALF_WIDTH = TRACTOR_CONFIG.rearTrackM * HALF_WIDTH_FACTOR

interface RibbonEntry {
  mesh: THREE.Mesh
  material: THREE.MeshBasicMaterial
  isForward: boolean
}

export class RddfPathRenderer {
  private scene: THREE.Scene
  private forwardTexture: THREE.Texture
  private backwardTexture: THREE.Texture
  private ribbons: RibbonEntry[] = []

  constructor(scene: THREE.Scene) {
    this.scene = scene
    console.log('[RddfPathRenderer] attached to scene')

    const loader = new THREE.TextureLoader()

    this.forwardTexture = loader.load('./textures/graphic_texture_targetpath_forward.png')
    this.forwardTexture.wrapS = THREE.ClampToEdgeWrapping
    this.forwardTexture.wrapT = THREE.RepeatWrapping
    this.forwardTexture.anisotropy = 4

    this.backwardTexture = loader.load('./textures/graphic_texture_targetpath_backward.png')
    this.backwardTexture.wrapS = THREE.ClampToEdgeWrapping
    this.backwardTexture.wrapT = THREE.RepeatWrapping
    this.backwardTexture.anisotropy = 4
  }

  // ── Private: build one continuous ribbon mesh from a strip of waypoints ────

  private buildRibbonMesh(
    strip: RddfWaypointEnu[],
    isForward: boolean,
  ): THREE.Mesh | null {
    const n = strip.length
    if (n < 2) return null

    // Per-vertex positions and UVs for 2*n vertices (left, right alternated)
    const positions = new Float32Array(n * 2 * 3) // [L0, R0, L1, R1, ..., L(n-1), R(n-1)]
    const uvs       = new Float32Array(n * 2 * 2)

    // Compute accumulated arc-length for V mapping
    const arcLen = new Float32Array(n)
    arcLen[0] = 0
    for (let i = 1; i < n; i++) {
      const dx = strip[i].x - strip[i - 1].x
      const dz = strip[i].z - strip[i - 1].z
      arcLen[i] = arcLen[i - 1] + Math.sqrt(dx * dx + dz * dz)
    }

    // Build left/right vertices with miter join per waypoint
    for (let i = 0; i < n; i++) {
      const curr = strip[i]

      // Compute perpendicular (left side) in XZ plane for incoming/outgoing segments
      let perpInX = 0, perpInZ = 0   // rotate90(dirIn)  = (-dirIn.z, dirIn.x)
      let perpOutX = 0, perpOutZ = 0  // rotate90(dirOut) = (-dirOut.z, dirOut.x)
      let hasPerpIn  = false
      let hasPerpOut = false

      if (i > 0) {
        const dx = curr.x - strip[i - 1].x
        const dz = curr.z - strip[i - 1].z
        const len = Math.sqrt(dx * dx + dz * dz)
        if (len > 1e-6) {
          perpInX = -dz / len
          perpInZ =  dx / len
          hasPerpIn = true
        }
      }
      if (i < n - 1) {
        const dx = strip[i + 1].x - curr.x
        const dz = strip[i + 1].z - curr.z
        const len = Math.sqrt(dx * dx + dz * dz)
        if (len > 1e-6) {
          perpOutX = -dz / len
          perpOutZ =  dx / len
          hasPerpOut = true
        }
      }

      // Miter offset — mirrors agmo_solution _buildRibbon logic exactly:
      //   perpIn + perpOut as n_avg, denom = 1 + dot(perpIn, perpOut), scale = halfWidth / max(denom, MIN)
      let offsetX: number
      let offsetZ: number

      if (!hasPerpIn && hasPerpOut) {
        offsetX = perpOutX * HALF_WIDTH
        offsetZ = perpOutZ * HALF_WIDTH
      } else if (hasPerpIn && !hasPerpOut) {
        offsetX = perpInX * HALF_WIDTH
        offsetZ = perpInZ * HALF_WIDTH
      } else if (hasPerpIn && hasPerpOut) {
        const denom = Math.max(MITER_MIN_DENOM, 1 + perpInX * perpOutX + perpInZ * perpOutZ)
        offsetX = (perpInX + perpOutX) * (HALF_WIDTH / denom)
        offsetZ = (perpInZ + perpOutZ) * (HALF_WIDTH / denom)
      } else {
        // degenerate single-point — zero offset
        offsetX = 0
        offsetZ = 0
      }

      // Vertex index layout: vertex 2*i = left, 2*i+1 = right
      const vBase = i * 2
      const pLeft  = vBase * 3
      const pRight = (vBase + 1) * 3
      const uLeft  = vBase * 2
      const uRight = (vBase + 1) * 2

      positions[pLeft]      = curr.x + offsetX
      positions[pLeft + 1]  = 0
      positions[pLeft + 2]  = curr.z + offsetZ

      positions[pRight]     = curr.x - offsetX
      positions[pRight + 1] = 0
      positions[pRight + 2] = curr.z - offsetZ

      const v = arcLen[i] / REPEAT_M
      uvs[uLeft]      = 0   // left edge U = 0
      uvs[uLeft + 1]  = v
      uvs[uRight]     = 1   // right edge U = 1
      uvs[uRight + 1] = v
    }

    // Build index buffer: each adjacent pair (i, i+1) → 2 triangles
    // Tri 1: L_i, R_i, L_{i+1}   = [2i, 2i+1, 2(i+1)]
    // Tri 2: R_i, R_{i+1}, L_{i+1} = [2i+1, 2(i+1)+1, 2(i+1)]
    const indexCount = (n - 1) * 6
    const indices = new Uint32Array(indexCount)
    let idx = 0
    for (let i = 0; i < n - 1; i++) {
      const L0 = 2 * i
      const R0 = 2 * i + 1
      const L1 = 2 * (i + 1)
      const R1 = 2 * (i + 1) + 1

      indices[idx++] = L0
      indices[idx++] = R0
      indices[idx++] = L1

      indices[idx++] = R0
      indices[idx++] = R1
      indices[idx++] = L1
    }

    const geometry = new THREE.BufferGeometry()
    geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3))
    geometry.setAttribute('uv',       new THREE.BufferAttribute(uvs, 2))
    geometry.setIndex(new THREE.BufferAttribute(indices, 1))

    const texture  = isForward ? this.forwardTexture : this.backwardTexture
    // Transparent queue (alpha blending preserved) + depthTest:false so the
    // ribbon is drawn through terrain. To avoid drawing through the tractor,
    // we read the stencil buffer: tractor meshes write stencil=1, and this
    // material only draws where stencil≠1. Three.js requires stencilWrite=true
    // for STENCIL_TEST to be enabled at all; we set writeMask=0 and all ops to
    // Keep so this material never modifies the stencil buffer itself.
    const material = new THREE.MeshBasicMaterial({
      map:         texture,
      transparent: true,
      opacity:     1.0,
      depthWrite:  false,
      depthTest:   false,
      side:        THREE.DoubleSide,
      toneMapped:  false,
    })
    material.stencilWrite = true
    material.stencilWriteMask = 0
    material.stencilFunc = THREE.NotEqualStencilFunc
    material.stencilRef = 1
    material.stencilFuncMask = 0xff
    material.stencilFail = THREE.KeepStencilOp
    material.stencilZFail = THREE.KeepStencilOp
    material.stencilZPass = THREE.KeepStencilOp

    const mesh = new THREE.Mesh(geometry, material)
    return mesh
  }

  // ── Public API ─────────────────────────────────────────────────────────────

  setWaypoints(waypoints: RddfWaypointEnu[]): void {
    console.log('[RddfPathRenderer] setWaypoints called', {
      count: waypoints.length,
      first: waypoints[0],
      last: waypoints[waypoints.length - 1],
    })
    // Dispose all existing ribbon meshes
    for (const entry of this.ribbons) {
      this.scene.remove(entry.mesh)
      entry.mesh.geometry.dispose()
      entry.material.dispose()
    }
    this.ribbons = []

    if (waypoints.length < 2) return

    // Split waypoints into strips at speed-sign boundaries.
    // The boundary waypoint is shared between adjacent strips (continuous ribbon).
    let stripStart = 0
    while (stripStart < waypoints.length - 1) {
      const isForward = waypoints[stripStart].speed >= 0

      // Find end of this sign run
      let stripEnd = stripStart + 1
      while (
        stripEnd < waypoints.length - 1 &&
        (waypoints[stripEnd].speed >= 0) === isForward
      ) {
        stripEnd++
      }
      // stripEnd is inclusive last index of this strip

      const strip = waypoints.slice(stripStart, stripEnd + 1)
      const mesh  = this.buildRibbonMesh(strip, isForward)

      if (mesh) {
        mesh.position.y = Y_OFFSET
        mesh.renderOrder = 1
        this.scene.add(mesh)
        this.ribbons.push({
          mesh,
          material: mesh.material as THREE.MeshBasicMaterial,
          isForward,
        })
      }

      // Next strip starts at the boundary waypoint (shared for continuity)
      stripStart = stripEnd
    }
  }

  update(deltaTime: number): void {
    // Both forward and backward textures use the same upward arrow and scroll
    // toward V+ (the waypoint progression / actual motion direction). Color
    // distinguishes pass type (cyan = forward, orange = backward).
    for (const entry of this.ribbons) {
      const tex = entry.material.map
      if (!tex) continue
      tex.offset.y -= deltaTime * SCROLL_SPEED
    }
  }

  dispose(): void {
    for (const entry of this.ribbons) {
      this.scene.remove(entry.mesh)
      entry.mesh.geometry.dispose()
      entry.material.dispose()
    }
    this.ribbons = []

    this.forwardTexture.dispose()
    this.backwardTexture.dispose()
  }

  getMaterials(): THREE.Material[] {
    return this.ribbons.map(e => e.material)
  }
}
