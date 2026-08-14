import { useCallback, useEffect, useMemo, type JSX, type RefObject } from 'react'
import { useFrame } from '@react-three/fiber'
import { useTexture } from '@react-three/drei'
import * as THREE from 'three'
import type { SoilGrid } from '../../simulation/SoilGrid'
import type { TerrainPatch } from '../../simulation/SoilSystem'
import type { VehicleState } from '../../simulation/VehicleSystem'
import { TRACTOR_CONFIG } from '../../simulation/TractorConfig'
import { getOriginLat, getOriginLon, getMetersPerDegLat, getMetersPerDegLon } from '../../simulation/GpsOrigin'

// Vertical deformation amplification used to encode/decode the signed
// height-delta in the alpha channel. The encoded byte stores
// (delta / DEFORM_AMPLIFY) mapped from [-1, 1] → [0, 255] (centered at 128).
export const DEFORM_AMPLIFY = 0.3

// When a frame's dirty queue exceeds this many cells, fall back to a single
// full-texture re-upload instead of many small subimage updates.
export const FULL_REUPLOAD_THRESHOLD = 5000

const PLANE_WIDTH = 200
const PLANE_LENGTH = 400

const DECAL_W = 2048
const DECAL_H = 4096
const TERRAIN_SEG_X = 600
const TERRAIN_SEG_Z = 1200

type SoilShader = {
  uniforms: Record<string, { value: unknown }>
  vertexShader: string
  fragmentShader: string
}

function clamp01(v: number): number {
  if (v < 0) return 0
  if (v > 1) return 1
  return v
}

function encodeHeightDelta(delta: number): number {
  // Map signed delta in [-DEFORM_AMPLIFY, DEFORM_AMPLIFY] → [0, 255] centered at 128.
  const normalized = clamp01((delta / DEFORM_AMPLIFY) * 0.5 + 0.5)
  return Math.round(normalized * 255)
}

function writeCellToData(
  data: Uint8Array,
  cols: number,
  col: number,
  row: number,
  soilGrid: SoilGrid,
): void {
  const idx = (row * cols + col) * 4
  const compaction = soilGrid.getCompaction(col, row)
  const moisture = soilGrid.getMoisture(col, row)
  const tilledIntensity = soilGrid.getTilledIntensity(col, row)
  const heightDelta = soilGrid.getHeight(col, row) - soilGrid.getBaseHeight(col, row)
  data[idx] = Math.round(clamp01(compaction) * 255)
  data[idx + 1] = Math.round(clamp01(moisture) * 255)
  data[idx + 2] = tilledIntensity
  data[idx + 3] = encodeHeightDelta(heightDelta)
}

/**
 * Apply a list of dirty terrain patches to a packed RGBA Uint8 texture buffer.
 * Patches with bounds outside the grid are clamped to the valid range; fully
 * out-of-bounds patches become no-ops. Pure function — extracted for testing.
 */
export function applyPatchesToTextureData(
  data: Uint8Array,
  gridCols: number,
  gridRows: number,
  patches: TerrainPatch[],
  soilGrid: SoilGrid,
): void {
  for (const patch of patches) {
    const startCol = Math.max(0, patch.startCol)
    const startRow = Math.max(0, patch.startRow)
    const endCol = Math.min(gridCols, patch.startCol + patch.width)
    const endRow = Math.min(gridRows, patch.startRow + patch.height)
    if (endCol <= startCol || endRow <= startRow) continue
    for (let r = startRow; r < endRow; r++) {
      for (let c = startCol; c < endCol; c++) {
        writeCellToData(data, gridCols, c, r, soilGrid)
      }
    }
  }
}

/**
 * Fill the entire RGBA Uint8 texture buffer from the SoilGrid state.
 * Used on initialization and as the > FULL_REUPLOAD_THRESHOLD fallback path.
 */
export function fillTextureDataFromGrid(
  data: Uint8Array,
  gridCols: number,
  gridRows: number,
  soilGrid: SoilGrid,
): void {
  for (let r = 0; r < gridRows; r++) {
    for (let c = 0; c < gridCols; c++) {
      writeCellToData(data, gridCols, c, r, soilGrid)
    }
  }
}

/**
 * Returns true when the dirty patches collectively exceed the threshold cell
 * count and a full re-upload should be performed instead of incremental writes.
 */
export function shouldFullReupload(patches: TerrainPatch[], threshold: number = FULL_REUPLOAD_THRESHOLD): boolean {
  let total = 0
  for (const p of patches) {
    total += p.width * p.height
    if (total > threshold) return true
  }
  return false
}

export interface FieldTerrainProps {
  soilGrid: SoilGrid
  dirtyPatchesRef: RefObject<TerrainPatch[]>
  vehicleStateRef?: RefObject<VehicleState>
  terrainSegX?: number
  terrainSegZ?: number
  decalW?: number
  decalH?: number
  tireDecalEnabled?: boolean
  tractorScale?: number   // tractor visual scale (e.g. 1.0 / 1.28 / 1.53). Default 1.
  geometryRef?: RefObject<import('./Tractor').TractorGeometryData | null>
  mapWidth?: number    // terrain plane width in meters; defaults to PLANE_WIDTH (legacy)
  mapHeight?: number   // terrain plane height in meters; defaults to PLANE_LENGTH (legacy)
  mapCenterX?: number  // ENU bbox center X for mesh position; defaults to 0 (legacy)
  mapCenterZ?: number  // ENU bbox center Z for mesh position; defaults to 0 (legacy)
}


function drawTireTread(
  ctx: CanvasRenderingContext2D,
  px: number,
  py: number,
  heading: number,
  widthPx: number,
  lengthPx: number,
): void {
  ctx.save()
  ctx.translate(px, py)
  ctx.rotate(-heading)

  // Base darker compressed band
  ctx.fillStyle = 'rgba(255, 255, 255, 1.0)'
  ctx.beginPath()
  ctx.roundRect(-widthPx * 0.5, -lengthPx * 0.5, widthPx, lengthPx, widthPx * 0.08)
  ctx.fill()

  // 3 chevron lug bars
  ctx.fillStyle = 'rgba(255, 255, 255, 1.0)'
  const lugCount = 3
  const lugThickness = lengthPx * 0.16
  const stride = lengthPx / lugCount
  const halfW = widthPx * 0.5
  const chevronTilt = lengthPx * 0.08   // each side of the V is tilted by this
  for (let i = 0; i < lugCount; i++) {
    const y = -lengthPx * 0.5 + i * stride + (stride - lugThickness) * 0.5
    // Left half of chevron: rises from outer to center
    ctx.beginPath()
    ctx.moveTo(-halfW,                 y + chevronTilt)
    ctx.lineTo(0,                       y)
    ctx.lineTo(0,                       y + lugThickness)
    ctx.lineTo(-halfW,                 y + chevronTilt + lugThickness)
    ctx.closePath()
    ctx.fill()
    // Right half of chevron
    ctx.beginPath()
    ctx.moveTo( halfW,                 y + chevronTilt)
    ctx.lineTo(0,                       y)
    ctx.lineTo(0,                       y + lugThickness)
    ctx.lineTo( halfW,                 y + chevronTilt + lugThickness)
    ctx.closePath()
    ctx.fill()
  }

  ctx.restore()
}

export default function FieldTerrain(props: FieldTerrainProps): JSX.Element {
  const { soilGrid, dirtyPatchesRef, vehicleStateRef, terrainSegX, terrainSegZ, decalW: decalWProp, decalH: decalHProp, tireDecalEnabled, tractorScale = 1, geometryRef, mapWidth, mapHeight, mapCenterX, mapCenterZ } = props
  const cols = soilGrid.cols
  const rows = soilGrid.rows
  const resolvedSegX = terrainSegX ?? TERRAIN_SEG_X
  const resolvedSegZ = terrainSegZ ?? TERRAIN_SEG_Z
  const resolvedDecalW = decalWProp ?? DECAL_W
  const resolvedDecalH = decalHProp ?? DECAL_H
  const planeWidth = mapWidth ?? PLANE_WIDTH
  const planeLength = mapHeight ?? PLANE_LENGTH
  const centerX = mapCenterX ?? 0
  const centerZ = mapCenterZ ?? 0
  const [albedo, normalMap, roughnessMap] = useTexture([
    './textures/soil/albedo.jpg',
    './textures/soil/normal.jpg',
    './textures/soil/roughness.jpg',
  ])

  useEffect(() => {
    for (const tex of [albedo, normalMap, roughnessMap]) {
      tex.wrapS = THREE.RepeatWrapping
      tex.wrapT = THREE.RepeatWrapping
      tex.repeat.set(15, 30)
      tex.needsUpdate = true
    }
  }, [albedo, normalMap, roughnessMap])

  const groundGeometry = useMemo(() => {
    // Terrain plane sized from MapDefinition.size; centered at bbox center (centerX, centerZ).
    const geo = new THREE.PlaneGeometry(planeWidth, planeLength, resolvedSegX, resolvedSegZ)
    geo.rotateX(-Math.PI / 2)
    const pos = geo.attributes.position as THREE.BufferAttribute
    for (let i = 0; i < pos.count; i++) {
      // pos.x/z are mesh-local; translate to world for height sampling.
      const worldX = pos.getX(i) + centerX
      const worldZ = pos.getZ(i) + centerZ
      pos.setY(i, soilGrid.getHeightAtWorld(worldX, worldZ))
    }
    pos.needsUpdate = true
    geo.computeVertexNormals()
    return geo
  }, [soilGrid, resolvedSegX, resolvedSegZ, planeWidth, planeLength, centerX, centerZ])

  useEffect(() => {
    return () => {
      groundGeometry.dispose()
    }
  }, [groundGeometry])

  const dataTexture = useMemo(() => {
    const data = new Uint8Array(cols * rows * 4)
    fillTextureDataFromGrid(data, cols, rows, soilGrid)
    const tex = new THREE.DataTexture(data, cols, rows, THREE.RGBAFormat, THREE.UnsignedByteType)
    tex.wrapS = THREE.ClampToEdgeWrapping
    tex.wrapT = THREE.ClampToEdgeWrapping
    tex.minFilter = THREE.LinearFilter
    tex.magFilter = THREE.LinearFilter
    tex.generateMipmaps = false
    tex.needsUpdate = true
    return tex
  }, [soilGrid, cols, rows])

  useEffect(() => {
    return () => {
      dataTexture.dispose()
    }
  }, [dataTexture])

  const decalCanvas = useMemo(() => {
    const c = document.createElement('canvas')
    c.width = resolvedDecalW
    c.height = resolvedDecalH
    const ctx = c.getContext('2d')!
    ctx.clearRect(0, 0, c.width, c.height)
    return c
  }, [resolvedDecalW, resolvedDecalH])

  const decalTexture = useMemo(() => {
    const tex = new THREE.CanvasTexture(decalCanvas)
    tex.wrapS = THREE.ClampToEdgeWrapping
    tex.wrapT = THREE.ClampToEdgeWrapping
    tex.flipY = false
    tex.minFilter = THREE.LinearFilter
    tex.magFilter = THREE.LinearFilter
    return tex
  }, [decalCanvas])

  useEffect(() => {
    return () => {
      decalTexture.dispose()
    }
  }, [decalTexture])

  const patchTerrainMaterial = useCallback((shader: SoilShader) => {
    shader.uniforms.uSoilData = { value: dataTexture }
    shader.uniforms.uTrackDecal = { value: decalTexture }
    shader.uniforms.uPlaneSize = { value: new THREE.Vector2(planeWidth, planeLength) }
    shader.uniforms.uDynamicHeightScale = { value: 2.1 }

    shader.vertexShader = shader.vertexShader
      .replace(
        '#include <common>',
        `#include <common>
uniform sampler2D uSoilData;
uniform sampler2D uTrackDecal;
uniform vec2 uPlaneSize;
uniform float uDynamicHeightScale;
varying vec2 vFieldUv;
float decodeHeightDelta(float encoded) {
  return (encoded * 2.0 - 1.0) * ${DEFORM_AMPLIFY.toFixed(8)};
}
vec2 fieldUvFromPlane(vec3 p) {
  return vec2(
    clamp((p.x + uPlaneSize.x * 0.5) / uPlaneSize.x, 0.0, 1.0),
    clamp((p.z + uPlaneSize.y * 0.5) / uPlaneSize.y, 0.0, 1.0)
  );
}`,
      )
      .replace(
        '#include <begin_vertex>',
        `#include <begin_vertex>
vFieldUv = fieldUvFromPlane(position);
vec4 soilVertex = texture2D(uSoilData, vFieldUv);
float treadMask = texture2D(uTrackDecal, vFieldUv).a;
float tilledVtx = soilVertex.b;
float footprintPress = smoothstep(0.30, 0.55, treadMask) * 0.028 * (1.0 - tilledVtx);
transformed.y += decodeHeightDelta(soilVertex.a) * uDynamicHeightScale;
transformed.y -= footprintPress;`,
      )

    shader.fragmentShader = shader.fragmentShader
      .replace(
        '#include <common>',
        `#include <common>
uniform sampler2D uSoilData;
uniform sampler2D uTrackDecal;
varying vec2 vFieldUv;
vec2 treadTexel() {
  return vec2(${(1 / DECAL_W).toFixed(10)}, ${(1 / DECAL_H).toFixed(10)});
}
float treadPressAt(vec2 uv) {
  float a = texture2D(uTrackDecal, clamp(uv, 0.0, 1.0)).a;
  return smoothstep(0.34, 0.40, a);
}`,
      )
      .replace(
        '#include <normal_fragment_begin>',
        `#include <normal_fragment_begin>
// 3x3 Gaussian blur (9-tap) for normal-perturbation gate — eliminates diagonal staircase.
vec2 tilledTexelSizeN = vec2(1.0 / ${soilGrid.cols}.0, 1.0 / ${soilGrid.rows}.0);
float tN_c0r0 = texture2D(uSoilData, vFieldUv).b;
float tN_p1r0 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSizeN.x,  0.0)).b;
float tN_m1r0 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSizeN.x,  0.0)).b;
float tN_c0p1 = texture2D(uSoilData, vFieldUv + vec2( 0.0,  tilledTexelSizeN.y)).b;
float tN_c0m1 = texture2D(uSoilData, vFieldUv + vec2( 0.0, -tilledTexelSizeN.y)).b;
float tN_p1p1 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSizeN.x,  tilledTexelSizeN.y)).b;
float tN_m1p1 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSizeN.x,  tilledTexelSizeN.y)).b;
float tN_p1m1 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSizeN.x, -tilledTexelSizeN.y)).b;
float tN_m1m1 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSizeN.x, -tilledTexelSizeN.y)).b;
float tBlurredN = (tN_c0r0 * 4.0
  + (tN_p1r0 + tN_m1r0 + tN_c0p1 + tN_c0m1) * 2.0
  + (tN_p1p1 + tN_m1p1 + tN_p1m1 + tN_m1m1) * 1.0) / 16.0;
float tilledMaskN = smoothstep(0.05, 0.95, tBlurredN);
vec2 treadStep = treadTexel();
float treadLeft = treadPressAt(vFieldUv - vec2(treadStep.x, 0.0));
float treadRight = treadPressAt(vFieldUv + vec2(treadStep.x, 0.0));
float treadDown = treadPressAt(vFieldUv - vec2(0.0, treadStep.y));
float treadUp = treadPressAt(vFieldUv + vec2(0.0, treadStep.y));
vec2 treadGradient = vec2(treadRight - treadLeft, treadUp - treadDown);
float decalGate = 1.0 - tilledMaskN;
normal = normalize(normal + vec3(-treadGradient.x * 0.45 * decalGate, treadGradient.y * 0.45 * decalGate, 0.0));`,
      )
      .replace(
        '#include <color_fragment>',
        `#include <color_fragment>
vec4 soilState = texture2D(uSoilData, vFieldUv);
float compaction = soilState.r;
float moisture = soilState.g;
// 3x3 Gaussian blur (9-tap, weights 1-2-1 / 2-4-2 / 1-2-1, sum=16) — eliminates diagonal staircase.
vec2 tilledTexelSize = vec2(1.0 / ${soilGrid.cols}.0, 1.0 / ${soilGrid.rows}.0);
float tC    = soilState.b;
float tP1R0 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSize.x,  0.0)).b;
float tM1R0 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSize.x,  0.0)).b;
float tC0P1 = texture2D(uSoilData, vFieldUv + vec2( 0.0,  tilledTexelSize.y)).b;
float tC0M1 = texture2D(uSoilData, vFieldUv + vec2( 0.0, -tilledTexelSize.y)).b;
float tP1P1 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSize.x,  tilledTexelSize.y)).b;
float tM1P1 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSize.x,  tilledTexelSize.y)).b;
float tP1M1 = texture2D(uSoilData, vFieldUv + vec2( tilledTexelSize.x, -tilledTexelSize.y)).b;
float tM1M1 = texture2D(uSoilData, vFieldUv + vec2(-tilledTexelSize.x, -tilledTexelSize.y)).b;
float tBlurred = (tC * 4.0
  + (tP1R0 + tM1R0 + tC0P1 + tC0M1) * 2.0
  + (tP1P1 + tM1P1 + tP1M1 + tM1M1) * 1.0) / 16.0;
float tilledMask = smoothstep(0.05, 0.95, tBlurred);
float rut = smoothstep(0.53, 0.68, abs(soilState.a - 0.5) + compaction * 0.2);
// Tilled (worked soil): very dark, slightly reddish brown — clearly distinct from tire tracks.
vec3 tilledColor = vec3(0.06, 0.035, 0.015);
vec3 compactedColor = vec3(0.24, 0.17, 0.10);
vec4 tireDecal = texture2D(uTrackDecal, vFieldUv);
float trackPress = smoothstep(0.34, 0.40, tireDecal.a);
trackPress *= (1.0 - tilledMask);
vec2 bevelStep = treadTexel();
float pressL = treadPressAt(vFieldUv - vec2(bevelStep.x, 0.0));
float pressR = treadPressAt(vFieldUv + vec2(bevelStep.x, 0.0));
float pressD = treadPressAt(vFieldUv - vec2(0.0, bevelStep.y));
float pressU = treadPressAt(vFieldUv + vec2(0.0, bevelStep.y));
float bevelLight = clamp((pressL - pressR) * 0.2 + (pressD - pressU) * 0.35, -1.0, 1.0);
// Tire track: cool-gray pressed soil, no bright raised edge (kept subtle / dark).
vec3 pressedSoilColor = vec3(0.22, 0.18, 0.13);
vec3 raisedEdgeColor = vec3(0.38, 0.30, 0.20);
// Tilled mix first with stronger weight so worked soil dominates.
diffuseColor.rgb = mix(diffuseColor.rgb, tilledColor, tilledMask * 0.97);
diffuseColor.rgb = mix(diffuseColor.rgb, compactedColor, clamp(compaction * 0.5 + rut * 0.35, 0.0, 0.75));
// Tire tracks: subtler raised edge + stronger pressed darkening.
diffuseColor.rgb = mix(diffuseColor.rgb, raisedEdgeColor, trackPress * 0.06);
diffuseColor.rgb = mix(diffuseColor.rgb, pressedSoilColor, trackPress * 0.70);
diffuseColor.rgb += max(bevelLight, 0.0) * trackPress * vec3(0.05, 0.04, 0.02);
diffuseColor.rgb -= max(-bevelLight, 0.0) * trackPress * vec3(0.14, 0.10, 0.05);
// Sparse weed clumps — value noise on field UV with two scales for varied size.
float wHashA = fract(sin(dot(floor(vFieldUv * vec2(100.0, 200.0)), vec2(127.1, 311.7))) * 43758.5453);
float wHashB = fract(sin(dot(floor(vFieldUv * vec2(46.0, 92.0)), vec2(269.5, 183.3))) * 43758.5453);
float weedMaskFine   = smoothstep(0.86, 0.93, wHashA);
float weedMaskCoarse = smoothstep(0.80, 0.92, wHashB);
float weedMask = clamp(weedMaskFine * 0.55 + weedMaskCoarse * 0.45, 0.0, 1.0);
vec3 weedColor = vec3(0.34, 0.42, 0.20);   // olive green
diffuseColor.rgb = mix(diffuseColor.rgb, weedColor, (1.0 - tilledMask) * weedMask * 0.22);
diffuseColor.rgb *= 1.0 - moisture * 0.16;`,
      )
  }, [dataTexture, decalTexture, planeWidth, planeLength])

  useFrame(() => {
    const queue = dirtyPatchesRef.current
    if (queue && queue.length > 0) {
      const data = dataTexture.image.data as Uint8Array
      if (shouldFullReupload(queue)) {
        fillTextureDataFromGrid(data, cols, rows, soilGrid)
      } else {
        applyPatchesToTextureData(data, cols, rows, queue, soilGrid)
      }
      dataTexture.needsUpdate = true
      queue.length = 0
    }

    const s = vehicleStateRef?.current
    if (!s) return
    const x = (s.longitude - getOriginLon()) * getMetersPerDegLon()
    const z = (s.latitude - getOriginLat()) * getMetersPerDegLat()
    const heading = (s.heading * Math.PI) / 180

    const cosH = Math.cos(heading)
    const sinH = Math.sin(heading)
    const scale = tractorScale ?? 1

    // Wheels: prefer GLB-measured geometry; fallback to TRACTOR_CONFIG spec if unavailable.
    type DecalWheel = { axle: 'front' | 'rear'; dx: number; dz: number; widthM: number; radiusM: number }
    const wheels: DecalWheel[] = []
    const geom = geometryRef?.current
    if (geom && geom.wheels.length === 4) {
      // Body rotation is `headingRad + Math.PI` (see Tractor.tsx). Local (lx, lz) maps to world delta:
      //   dx = cos(h+π) * lx + sin(h+π) * lz = -cosH*lx - sinH*lz
      //   dz = -sin(h+π) * lx + cos(h+π) * lz =  sinH*lx - cosH*lz
      for (const w of geom.wheels) {
        const dx = -cosH * w.localX - sinH * w.localZ
        const dz =  sinH * w.localX - cosH * w.localZ
        wheels.push({ axle: w.axle, dx, dz, widthM: w.widthM, radiusM: w.radiusM })
      }
    } else {
      // Fallback: spec-based positions matching legacy convention (forward = (sinH, cosH)).
      const hw = (TRACTOR_CONFIG.wheelbaseM / 2) * scale
      const ftHalf = (TRACTOR_CONFIG.frontTrackM / 2) * scale
      const rtHalf = (TRACTOR_CONFIG.rearTrackM / 2) * scale
      const frontW = TRACTOR_CONFIG.frontWheelWidthM * scale
      const rearW = TRACTOR_CONFIG.rearWheelWidthM * scale
      const frontR = TRACTOR_CONFIG.frontTireRadiusM * scale
      const rearR = TRACTOR_CONFIG.rearTireRadiusM * scale
      wheels.push(
        { axle: 'front', dx: sinH * hw - cosH * ftHalf, dz: cosH * hw + sinH * ftHalf, widthM: frontW, radiusM: frontR },
        { axle: 'front', dx: sinH * hw + cosH * ftHalf, dz: cosH * hw - sinH * ftHalf, widthM: frontW, radiusM: frontR },
        { axle: 'rear',  dx: -sinH * hw - cosH * rtHalf, dz: -cosH * hw + sinH * rtHalf, widthM: rearW, radiusM: rearR },
        { axle: 'rear',  dx: -sinH * hw + cosH * rtHalf, dz: -cosH * hw - sinH * rtHalf, widthM: rearW, radiusM: rearR },
      )
    }

    if (tireDecalEnabled !== false) {
      const ctx = decalCanvas.getContext('2d')!
      const contactDepth = TRACTOR_CONFIG.wheelContactDepthM * scale
      const computeFootprintLength = (radiusM: number): number => {
        const r = radiusM
        const d = Math.min(contactDepth, r)
        return 2 * Math.sqrt(Math.max(0, 2 * r * d - d * d))
      }
      const movingDir = Math.sign(s.wheelSpeed ?? 0)
      if (!(movingDir === 0 || s.speed < 0.015)) {
        let drew = false
        for (const w of wheels) {
          const wx = x + w.dx
          const wz = z + w.dz
          const px = (((wx - centerX) + planeWidth / 2) / planeWidth) * resolvedDecalW
          const py = (((wz - centerZ) + planeLength / 2) / planeLength) * resolvedDecalH
          if (px < 0 || px >= resolvedDecalW || py < 0 || py >= resolvedDecalH) continue
          const widthPx = Math.max(2, (w.widthM / planeWidth) * resolvedDecalW)
          const lengthM = computeFootprintLength(w.radiusM)
          const lengthPx = Math.max(4, (lengthM / planeLength) * resolvedDecalH)
          drawTireTread(ctx, px, py, heading, widthPx, lengthPx)
          drew = true
        }
        if (drew) {
          decalTexture.needsUpdate = true
        }
      }
    }
  })

  return (
    <>
      <mesh receiveShadow position={[centerX, 0, centerZ]}>
        <primitive object={groundGeometry} attach="geometry" />
        <meshStandardMaterial
          map={albedo}
          normalMap={normalMap}
          roughnessMap={roughnessMap}
          color="#b59778"
          roughness={1.0}
          metalness={0.0}
          side={THREE.DoubleSide}
          onBeforeCompile={patchTerrainMaterial}
          customProgramCacheKey={() => 'field-terrain-soil-v2'}
        />
      </mesh>
    </>
  )
}
