import { useMemo } from 'react'
import * as THREE from 'three'
import { useTexture } from '@react-three/drei'
import { useSessionStore } from '../../state/sessionStore'
import { gpsToEnu } from '../../simulation/GpsOrigin'

const BUND_WIDTH = 3.2    // meters
const BUND_HEIGHT = 0.8   // meters
const RIDGE_BASE_Y = 0.03 // sit above the field interior overlay

const UV_SCALE = 0.5 // 1 worldUnit = 0.5 texture repeat

export function FieldBoundaryRidge() {
  const polygons = useSessionStore(s => s.mapPolygonsLL)
  const originVersion = useSessionStore(s => s.originVersion)
  const soilMap = useTexture('./textures/soil/albedo.jpg')
  soilMap.wrapS = THREE.RepeatWrapping
  soilMap.wrapT = THREE.RepeatWrapping
  soilMap.anisotropy = 8
  const geometry = useMemo(() => {
    if (!polygons.length) return null
    // Build a single merged BufferGeometry using continuous ring cross-sections.
    // Each polygon vertex gets one cross-section (4 pts: baseL, baseR, topL, topR)
    // computed via miter bisector of adjacent edges, eliminating box-seam artifacts.
    const positions: number[] = []
    const uvs: number[] = []
    const indices: number[] = []
    let vertOffset = 0

    for (const poly of polygons) {
      // Determine the ring vertices in ENU space.
      // poly[i] = [lat, lon]. Deduplicate closed ring (first == last).
      let raw: Array<[number, number]> = poly as Array<[number, number]>
      const last = raw[raw.length - 1]
      const first = raw[0]
      if (last[0] === first[0] && last[1] === first[1]) {
        raw = raw.slice(0, raw.length - 1)
      }
      const N = raw.length
      if (N < 2) continue

      // Convert to ENU XZ
      const pts: Array<{ x: number; z: number }> = raw.map(([lat, lon]) => {
        const { x, z } = gpsToEnu(lat, lon)
        return { x, z }
      })

      const w = BUND_WIDTH / 2
      const h = BUND_HEIGHT

      // Per-vertex cross-section indices: [baseL, baseR, topL, topR]
      const crossSection: Array<[number, number, number, number]> = []

      for (let i = 0; i < N; i++) {
        const prev = pts[(i - 1 + N) % N]
        const cur  = pts[i]
        const next = pts[(i + 1) % N]

        // Edge directions
        const epx = cur.x - prev.x, epz = cur.z - prev.z
        const enx = next.x - cur.x, enz = next.z - cur.z
        const lenP = Math.hypot(epx, epz)
        const lenN = Math.hypot(enx, enz)

        // Skip degenerate vertices; reuse previous cross-section or skip
        if (lenP < 1e-6 || lenN < 1e-6) {
          crossSection.push([-1, -1, -1, -1])
          continue
        }

        // Unit tangents
        const etpx = epx / lenP, etpz = epz / lenP
        const etnx = enx / lenN, etnz = enz / lenN

        // XZ plane outward normals (right perpendicular of tangent)
        const npx = etpz, npz = -etpx   // normal of prev edge
        const nnx = etnz, nnz = -etnx   // normal of next edge

        // Bisector of the two normals
        let bx = npx + nnx, bz = npz + nnz
        const bLen = Math.hypot(bx, bz)
        if (bLen < 1e-9) {
          // Parallel edges — bisector degenerate, use prev normal
          bx = npx; bz = npz
        } else {
          bx /= bLen; bz /= bLen
        }

        // Miter scale: 1 / cos(half-angle) = 1 / dot(bisector, n_prev)
        const dotVal = bx * npx + bz * npz
        const miterScale = 1.0 / Math.max(0.3, dotVal)

        const cx = cur.x, cz = cur.z
        const offx = bx * w * miterScale
        const offz = bz * w * miterScale

        // 4 cross-section points for this vertex
        const vBaseL = [cx + offx, RIDGE_BASE_Y, cz + offz]
        const vBaseR = [cx - offx, RIDGE_BASE_Y, cz - offz]
        const vTopL  = [cx + offx * 0.4, h, cz + offz * 0.4]
        const vTopR  = [cx - offx * 0.4, h, cz - offz * 0.4]

        const base = vertOffset
        positions.push(vBaseL[0], vBaseL[1], vBaseL[2])
        positions.push(vBaseR[0], vBaseR[1], vBaseR[2])
        positions.push(vTopL[0],  vTopL[1],  vTopL[2])
        positions.push(vTopR[0],  vTopR[1],  vTopR[2])

        uvs.push(vBaseL[0] * UV_SCALE, vBaseL[2] * UV_SCALE)
        uvs.push(vBaseR[0] * UV_SCALE, vBaseR[2] * UV_SCALE)
        uvs.push(vTopL[0]  * UV_SCALE, vTopL[2]  * UV_SCALE)
        uvs.push(vTopR[0]  * UV_SCALE, vTopR[2]  * UV_SCALE)

        // base: 0=baseL, 1=baseR, 2=topL, 3=topR
        crossSection.push([base, base + 1, base + 2, base + 3])
        vertOffset += 4
      }

      // Connect consecutive cross-sections with quads (closed ring)
      for (let i = 0; i < N; i++) {
        const cs0 = crossSection[i]
        const cs1 = crossSection[(i + 1) % N]
        // Skip degenerate cross-sections
        if (cs0[0] < 0 || cs1[0] < 0) continue

        const [bL0, bR0, tL0, tR0] = cs0  // baseL, baseR, topL, topR at i
        const [bL1, bR1, tL1, tR1] = cs1  // at i+1

        // Top face
        indices.push(tL0, tL1, tR0,  tR0, tL1, tR1)
        // Outer slope (baseL side)
        indices.push(bL0, bL1, tL0,  tL0, bL1, tL1)
        // Inner slope (baseR side)
        indices.push(bR0, tR0, bR1,  bR1, tR0, tR1)
      }
    }

    if (!positions.length) return null
    const geom = new THREE.BufferGeometry()
    geom.setAttribute('position', new THREE.Float32BufferAttribute(positions, 3))
    geom.setAttribute('uv', new THREE.Float32BufferAttribute(uvs, 2))
    geom.setIndex(indices)
    geom.computeVertexNormals()
    return geom
  }, [polygons, originVersion])
  if (!geometry) return null
  return (
    <mesh geometry={geometry} castShadow receiveShadow>
      <meshStandardMaterial map={soilMap} roughness={0.9} metalness={0} side={THREE.DoubleSide} />
    </mesh>
  )
}
