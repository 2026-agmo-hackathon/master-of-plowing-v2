import { useMemo, useRef, useState, useEffect, Suspense } from 'react'
import type { JSX, RefObject } from 'react'
import { useFrame } from '@react-three/fiber'
import { useGLTF } from '@react-three/drei'
import * as THREE from 'three'
import type { VehicleState } from '../../simulation/VehicleSystem'
import { TRACTOR_CONFIG } from '../../simulation/TractorConfig'
import { getOriginLat, getOriginLon, getMetersPerDegLat, getMetersPerDegLon } from '../../simulation/GpsOrigin'
import { useSessionStore } from '../../state/sessionStore'

export interface WheelGeometry {
  axle: 'front' | 'rear'
  /** model-local X (after assembly orientation correction), already scaled */
  localX: number
  /** model-local Z (after assembly orientation correction), already scaled */
  localZ: number
  /** tire axial width in meters, already scaled */
  widthM: number
  /** tire radius in meters, already scaled */
  radiusM: number
}

export interface TractorGeometryData {
  wheels: WheelGeometry[]
}

export interface TractorProps {
  vehicleStateRef: RefObject<VehicleState>
  implementType: 'plow' | 'rotary' | null
  scale?: number
  implementWidth?: number   // meters, default 2.8
  implementDepth?: number   // meters, default 0.25 (visual hint only)
  hitchDown?: boolean
  geometryRef?: RefObject<TractorGeometryData | null>
}

const DEG2RAD = Math.PI / 180
const VISUAL_GROUND_BIAS_M = 0.08

const GLB_PATH = `${import.meta.env.BASE_URL}models/tractor.glb`

interface TractorAssembly {
  group: THREE.Group
  flSteer: THREE.Group | null
  flSpin: THREE.Group | null
  frSteer: THREE.Group | null
  frSpin: THREE.Group | null
  rlSpin: THREE.Group | null
  rrSpin: THREE.Group | null
  frontWheelRadius: number
  rearWheelRadius: number
  bboxDepth: number
  wheels: WheelGeometry[]
}

function buildTractorAssembly(sourceScene: THREE.Object3D, scale: number = 1): TractorAssembly {
  const tractor = new THREE.Group()
  const model = sourceScene.clone(true) as THREE.Object3D

  // Scale/orientation correction: align longest axis to +Z (forward).
  const bbox = new THREE.Box3().setFromObject(model)
  const size = bbox.getSize(new THREE.Vector3())
  if (size.x > size.z) {
    model.rotation.y = -Math.PI / 2
    model.updateMatrixWorld(true)
  }
  // Recompute bbox after rotation, then recenter X/Z and drop min.y to 0.
  const bbox2 = new THREE.Box3().setFromObject(model)
  const center = bbox2.getCenter(new THREE.Vector3())
  model.position.x -= center.x
  model.position.z -= center.z
  model.position.y -= bbox2.min.y

  // Shadows + occlusion of the RDDF path. Tractor writes stencil=1 so the
  // (transparent) path material can test stencil≠1 and hide behind the body.
  // renderOrder=2 is kept as a defensive ordering hint.
  model.traverse((c) => {
    const mesh = c as THREE.Mesh
    if (mesh.isMesh) {
      mesh.castShadow = true
      mesh.receiveShadow = true
      mesh.renderOrder = 2
      const mats = Array.isArray(mesh.material) ? mesh.material : [mesh.material]
      for (const m of mats) {
        if (!m) continue
        m.stencilWrite = true
        m.stencilRef = 1
        m.stencilFunc = THREE.AlwaysStencilFunc
        m.stencilFail = THREE.KeepStencilOp
        m.stencilZFail = THREE.KeepStencilOp
        m.stencilZPass = THREE.ReplaceStencilOp
      }
    }
  })

  // Green tint pass — preserve tires (very dark) and glass (transparent).
  const GREEN = 0x6fc070
  const LIGHT_GREEN = 0xa0d878
  model.traverse((c) => {
    const mesh = c as THREE.Mesh
    if (!mesh.isMesh) return
    const mats = Array.isArray(mesh.material) ? mesh.material : [mesh.material]
    for (const mat of mats) {
      if (!mat || !('color' in mat)) continue
      const std = mat as THREE.MeshStandardMaterial
      const col = std.color
      const avg = (col.r + col.g + col.b) / 3
      if (avg < 0.15) continue
      if (std.transparent && std.opacity < 0.8) continue
      if (avg > 0.75) {
        std.color.setHex(LIGHT_GREEN)
      } else {
        std.color.setHex(GREEN)
      }
      // Slight self-illumination so the tractor body doesn't appear black in
      // shadowed areas / cloudy lighting.
      std.emissive.setHex(0x1a3320)
      std.emissiveIntensity = 0.4
    }
  })

  // Apply uniform scale on top of the auto-correction above.
  model.scale.multiplyScalar(scale)

  tractor.add(model)
  model.updateMatrixWorld(true)

  // Identify wheel meshes by Material_4 family.
  interface TireMesh { mesh: THREE.Mesh; cx: number; cy: number; cz: number; radius: number; widthX: number }
  const allTires: TireMesh[] = []
  model.traverse((n) => {
    const mesh = n as THREE.Mesh
    if (!mesh.isMesh) return
    const mat = mesh.material as THREE.Material | THREE.Material[]
    const mats = Array.isArray(mat) ? mat : [mat]
    const matName = mats[0]?.name ?? ''
    if (!/_4([\._]|$)/.test(matName) && matName !== 'Material_4') return
    const bb = new THREE.Box3().setFromObject(mesh)
    const c = bb.getCenter(new THREE.Vector3())
    const s = bb.getSize(new THREE.Vector3())
    // radius = larger of vertical/forward axes (diameter / 2). widthX = lateral (axial) extent.
    const radius = Math.max(s.y, s.z) / 2
    const widthX = s.x
    allTires.push({ mesh, cx: c.x, cy: c.y, cz: c.z, radius, widthX })
  })

  // 4 corners in tractor local space.
  const fullBox = new THREE.Box3().setFromObject(model)
  const w = fullBox.max.x - fullBox.min.x
  const d = fullBox.max.z - fullBox.min.z
  const cornerOffX = w * 0.35
  const cornerOffZ = d * 0.35
  const corners = {
    FL: new THREE.Vector2(-cornerOffX, -cornerOffZ),
    FR: new THREE.Vector2(+cornerOffX, -cornerOffZ),
    RL: new THREE.Vector2(-cornerOffX, +cornerOffZ),
    RR: new THREE.Vector2(+cornerOffX, +cornerOffZ),
  }

  const selectedByCorner: Record<'FL' | 'FR' | 'RL' | 'RR', TireMesh | null> = {
    FL: null, FR: null, RL: null, RR: null,
  }
  const used = new Set<THREE.Mesh>()
  for (const key of ['FL', 'FR', 'RL', 'RR'] as const) {
    let best: TireMesh | null = null
    let bestDist = Infinity
    for (const t of allTires) {
      if (used.has(t.mesh)) continue
      const dx = t.cx - corners[key].x
      const dz = t.cz - corners[key].y
      const dist = Math.sqrt(dx * dx + dz * dz)
      if (dist < bestDist) { bestDist = dist; best = t }
    }
    if (best) {
      selectedByCorner[key] = best
      used.add(best.mesh)
    }
  }

  const makePivotsForCorner = (
    tire: TireMesh | null,
    withSteer: boolean,
  ): { steer: THREE.Group | null; spin: THREE.Group | null; radius: number } => {
    if (!tire) {
      return { steer: null, spin: null, radius: 0.5 }
    }
    const spin = new THREE.Group()
    let steer: THREE.Group | null = null
    const pivotY = tire.radius
    const centerVec = new THREE.Vector3(tire.cx, pivotY, tire.cz)
    if (withSteer) {
      steer = new THREE.Group()
      steer.position.copy(centerVec)
      spin.position.set(0, 0, 0)
      steer.add(spin)
      tractor.add(steer)
    } else {
      spin.position.copy(centerVec)
      tractor.add(spin)
    }
    spin.attach(tire.mesh)
    return { steer, spin, radius: tire.radius }
  }

  const fl = makePivotsForCorner(selectedByCorner.FL, true)
  const fr = makePivotsForCorner(selectedByCorner.FR, true)
  const rl = makePivotsForCorner(selectedByCorner.RL, false)
  const rr = makePivotsForCorner(selectedByCorner.RR, false)

  // Attach rim/hub meshes (Material_11) to the nearest corner's spin pivot so they rotate with the tire.
  const cornerSpins: { key: 'FL' | 'FR' | 'RL' | 'RR'; spin: THREE.Group | null; tire: TireMesh | null }[] = [
    { key: 'FL', spin: fl.spin, tire: selectedByCorner.FL },
    { key: 'FR', spin: fr.spin, tire: selectedByCorner.FR },
    { key: 'RL', spin: rl.spin, tire: selectedByCorner.RL },
    { key: 'RR', spin: rr.spin, tire: selectedByCorner.RR },
  ]
  model.traverse((n) => {
    const mesh = n as THREE.Mesh
    if (!mesh.isMesh) return
    const mats = Array.isArray(mesh.material) ? mesh.material : [mesh.material]
    const matName = mats[0]?.name ?? ''
    // Only Material_11 family (rims). Skip already attached and non-rim parts.
    if (!/^Material_11(\.|$)/.test(matName)) return
    const bb = new THREE.Box3().setFromObject(mesh)
    const c = bb.getCenter(new THREE.Vector3())
    let bestKey: typeof cornerSpins[number] | null = null
    let bestDist = Infinity
    for (const cs of cornerSpins) {
      if (!cs.spin || !cs.tire) continue
      const dx = c.x - cs.tire.cx
      const dz = c.z - cs.tire.cz
      const dy = c.y - cs.tire.cy
      const dist = Math.sqrt(dx * dx + dy * dy + dz * dz)
      if (dist < bestDist) { bestDist = dist; bestKey = cs }
    }
    if (bestKey?.spin) {
      bestKey.spin.attach(mesh)
    }
  })
  const frontWheelRadius = Math.max(fl.radius, fr.radius, 0.3) * scale
  const rearWheelRadius = Math.max(rl.radius, rr.radius, 0.3) * scale

  const buildWheelGeometry = (tire: TireMesh | null, axle: 'front' | 'rear'): WheelGeometry | null => {
    if (!tire) return null
    // tire.cx/cz/radius/widthX are already in scaled tractor-local coordinates
    // because they were measured from setFromObject(mesh) after model.scale was applied.
    return {
      axle,
      localX: tire.cx,
      localZ: tire.cz,
      widthM: tire.widthX,
      radiusM: tire.radius,
    }
  }
  const wheels: WheelGeometry[] = []
  const flGeo = buildWheelGeometry(selectedByCorner.FL, 'front')
  const frGeo = buildWheelGeometry(selectedByCorner.FR, 'front')
  const rlGeo = buildWheelGeometry(selectedByCorner.RL, 'rear')
  const rrGeo = buildWheelGeometry(selectedByCorner.RR, 'rear')
  for (const g of [flGeo, frGeo, rlGeo, rrGeo]) {
    if (g) wheels.push(g)
  }

  const finalBox = new THREE.Box3().setFromObject(model)
  const bboxDepth = finalBox.max.z - finalBox.min.z

  return {
    group: tractor,
    flSteer: fl.steer,
    flSpin: fl.spin,
    frSteer: fr.steer,
    frSpin: fr.spin,
    rlSpin: rl.spin,
    rrSpin: rr.spin,
    frontWheelRadius,
    rearWheelRadius,
    bboxDepth,
    wheels,
  }
}

interface StrutProps {
  from: [number, number, number]
  to: [number, number, number]
  radius: number
  color: string
  metalness?: number
  roughness?: number
}

function Strut({ from, to, radius, color, metalness = 0.6, roughness = 0.55 }: StrutProps): JSX.Element {
  const { position, quaternion, length } = useMemo(() => {
    const start = new THREE.Vector3(from[0], from[1], from[2])
    const end = new THREE.Vector3(to[0], to[1], to[2])
    const dir = end.clone().sub(start)
    const len = dir.length()
    const mid = start.clone().add(end).multiplyScalar(0.5)
    const quat = new THREE.Quaternion().setFromUnitVectors(
      new THREE.Vector3(0, 1, 0),
      dir.clone().normalize(),
    )
    return { position: mid, quaternion: quat, length: len }
  }, [from, to])

  return (
    <mesh position={position} quaternion={quaternion} castShadow receiveShadow>
      <cylinderGeometry args={[radius, radius, length, 10]} />
      <meshStandardMaterial color={color} metalness={metalness} roughness={roughness} />
    </mesh>
  )
}

function TractorFallback(): JSX.Element {
  return (
    <mesh>
      <boxGeometry args={[2, 1, 3]} />
      <meshStandardMaterial color="red" />
    </mesh>
  )
}

function TractorGLB(props: TractorProps): JSX.Element {
  const { vehicleStateRef, implementType, scale, geometryRef } = props

  const plowWidth = props.implementWidth ?? 2.8
  const rotaryWidth = props.implementWidth ?? 2.79
  const drumLength = rotaryWidth - 0.12   // 0.06m side panel clearance on each side
  const ROTARY_PANEL_INSET = 0.030        // side panel inset from rotary edge
  const FLANGE_SPACING = 0.45
  const flangeCount = Math.max(2, Math.round(drumLength / FLANGE_SPACING))
  const flangeActualSpacing = drumLength / (flangeCount - 1)
  const flangeYs = Array.from(
    { length: flangeCount },
    (_, i) => -drumLength / 2 + i * flangeActualSpacing,
  )
  const SHARE_SPACING = 0.65
  const shareCount = Math.max(2, Math.round(plowWidth / SHARE_SPACING))
  const actualSpacing = plowWidth / (shareCount - 1)
  const shareXs = Array.from({ length: shareCount }, (_, i) => -plowWidth / 2 + i * actualSpacing)

  const speedMultiplier = useSessionStore(s => s.speedMultiplier)

  const gltf = useGLTF(GLB_PATH)
  const implementGroupRef = useRef<THREE.Group>(null)
  const rotaryDrumRef = useRef<THREE.Group>(null)
  const armLowerLeftRef = useRef<THREE.Group | null>(null)
  const armLowerRightRef = useRef<THREE.Group | null>(null)
  const armUpperRef = useRef<THREE.Group | null>(null)
  // 1.0 = fully raised; 0.0 = fully lowered
  const hitchYRef = useRef(props.hitchDown ? 0 : 1)

  const assembly = useMemo(
    () => buildTractorAssembly(gltf.scene, scale ?? 1),
    [gltf.scene, scale],
  )

  useEffect(() => {
    if (geometryRef) {
      geometryRef.current = { wheels: assembly.wheels }
    }
    return () => {
      if (geometryRef) {
        geometryRef.current = null
      }
    }
  }, [assembly, geometryRef])

  // Apply renderOrder + stencil write to every mesh under the tractor
  // assembly (including r3f-mounted implement children). The RDDF path
  // material tests stencil≠1 to hide behind these meshes. Re-runs when
  // implementType changes since plow/rotary mount new meshes.
  useEffect(() => {
    assembly.group.traverse((c) => {
      const mesh = c as THREE.Mesh
      if (!mesh.isMesh) return
      mesh.renderOrder = 2
      const mats = Array.isArray(mesh.material) ? mesh.material : [mesh.material]
      for (const m of mats) {
        if (!m) continue
        m.stencilWrite = true
        m.stencilRef = 1
        m.stencilFunc = THREE.AlwaysStencilFunc
        m.stencilFail = THREE.KeepStencilOp
        m.stencilZFail = THREE.KeepStencilOp
        m.stencilZPass = THREE.ReplaceStencilOp
      }
    })
  }, [assembly, props.implementType, props.implementWidth, props.implementDepth, props.hitchDown])

  const tmpEuler = useRef(new THREE.Euler()).current

  const implementZ = assembly.bboxDepth / 2 + 0.2

  // 3-point hitch geometry (tractor local frame; +Z = behind tractor)
  const HINGE_LOWER_Y = 0.45
  const HINGE_UPPER_Y = 0.95
  const HINGE_LOWER_HALF_W = 0.55      // half-width for the two lower hinges
  const HINGE_Z = implementZ - 0.42    // 0.42m forward of implement origin

  // Attachment offsets from the implement group origin (move with implement)
  const ATTACH_LOWER_OFFSET_Y = -0.10
  const ATTACH_UPPER_OFFSET_Y = 0.30
  const ATTACH_LOWER_HALF_W = 0.42
  const ATTACH_OFFSET_Z = -0.10        // slightly inside (toward tractor) from group origin

  // Shared linkage material constants — unified greasy-steel look for arms, brackets, cross-tube, mast
  const LINKAGE_COLOR = '#5a5a5a'
  const LINKAGE_METAL = 0.55
  const LINKAGE_ROUGH = 0.55

  const pentagonGeo = useMemo(() => {
    const shape = new THREE.Shape()
    shape.moveTo(0.58, -0.20)
    shape.lineTo(1.32, -0.20)
    shape.lineTo(1.32, 0.30)
    const r = 0.06
    const backLen = Math.sqrt(0.37 * 0.37 + 0.20 * 0.20)
    const tBack = 1 - r / backLen
    const preZ = 1.32 + (0.95 - 1.32) * tBack
    const preY = 0.30 + (0.50 - 0.30) * tBack
    shape.lineTo(preZ, preY)
    const frontLen = Math.sqrt(0.37 * 0.37 + 0.20 * 0.20)
    const tFront = r / frontLen
    const postZ = 0.58 + (0.95 - 0.58) * tFront
    const postY = 0.30 + (0.50 - 0.30) * tFront
    shape.quadraticCurveTo(0.95, 0.50, postZ, postY)
    shape.lineTo(0.58, 0.30)
    shape.closePath()
    return new THREE.ExtrudeGeometry(shape, { depth: 0.04, bevelEnabled: false })
  }, [])

  const rearPanelGeo = useMemo(() => {
    const shape = new THREE.Shape()
    // Profile in (z, y) plane — \|||\ shape connecting smoothly to top panel and bottom edge.
    shape.moveTo(1.30, 0.30)   // A: top-front (meets top panel rear edge)
    shape.lineTo(1.34, 0.20)   // B: top of vertical mid-section (after top \ slant)
    shape.lineTo(1.34, -0.10)  // C: bottom of vertical mid-section
    shape.lineTo(1.30, -0.20)  // D: bottom-front (after bottom \ slant)
    shape.closePath()
    return new THREE.ExtrudeGeometry(shape, { depth: rotaryWidth, bevelEnabled: false })
  }, [rotaryWidth])

  const hitchBracketGeo = useMemo(() => {
    const shape = new THREE.Shape()
    // Triangle profile in (z, y) plane — base on top panel surface, apex above peak shaft.
    shape.moveTo(0.65, 0.32)   // front-bottom on top panel surface
    shape.lineTo(1.20, 0.32)   // back-bottom on top panel surface
    shape.lineTo(0.92, 0.62)   // apex (top-link pin location)
    shape.closePath()
    return new THREE.ExtrudeGeometry(shape, { depth: 0.20, bevelEnabled: false })
  }, [])

  const tmpStart = useMemo(() => new THREE.Vector3(), [])
  const tmpEnd   = useMemo(() => new THREE.Vector3(), [])
  const tmpDir   = useMemo(() => new THREE.Vector3(), [])
  const tmpMid   = useMemo(() => new THREE.Vector3(), [])
  const tmpUp    = useMemo(() => new THREE.Vector3(0, 1, 0), [])
  const tmpQ     = useMemo(() => new THREE.Quaternion(), [])

  // Static A-frame mast strut transforms (in implement-local space).
  // Computed once per implementDepth change; struts never move relative to implement.
  const aframeMast = useMemo(() => {
    const ax0 = -ATTACH_LOWER_HALF_W + 0.05
    const ay0 = ATTACH_LOWER_OFFSET_Y + 0.05
    const az0 = ATTACH_OFFSET_Z
    const ax1 = 0
    const ay1 = ATTACH_UPPER_OFFSET_Y
    const az1 = ATTACH_OFFSET_Z
    const bx0 = ATTACH_LOWER_HALF_W - 0.05
    const by0 = ATTACH_LOWER_OFFSET_Y + 0.05

    function strut(x0: number, y0: number, x1: number, y1: number) {
      const dx = x1 - x0
      const dy = y1 - y0
      const len = Math.sqrt(dx * dx + dy * dy)
      const midX = (x0 + x1) / 2
      const midY = (y0 + y1) / 2
      // Rotate box Y-axis to align with the strut direction (in XY plane)
      const angle = Math.atan2(dx, dy)
      return { midX, midY, midZ: az0, rotZ: -angle, len }
    }

    return {
      strutA: strut(ax0, ay0, ax1, ay1),
      strutB: strut(bx0, by0, ax1, ay1),
      topZ: az1,
    }
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [props.implementDepth])

  function alignArm(ref: RefObject<THREE.Group | null>, sx: number, sy: number, sz: number, ex: number, ey: number, ez: number) {
    const g = ref.current
    if (!g) return
    tmpStart.set(sx, sy, sz)
    tmpEnd.set(ex, ey, ez)
    tmpDir.subVectors(tmpEnd, tmpStart)
    const len = tmpDir.length()
    if (len < 1e-4) return
    tmpMid.copy(tmpStart).add(tmpEnd).multiplyScalar(0.5)
    g.position.copy(tmpMid)
    tmpDir.divideScalar(len)
    tmpQ.setFromUnitVectors(tmpUp, tmpDir)
    g.quaternion.copy(tmpQ)
    g.scale.set(1, len, 1)
  }

  useFrame((_state, dt) => {
    const s = vehicleStateRef.current
    if (!s) return

    const body = assembly.group
    const x = (s.longitude - getOriginLon()) * getMetersPerDegLon()
    const z = (s.latitude - getOriginLat()) * getMetersPerDegLat()
    const headingRad = s.heading * DEG2RAD
    const pitch = s.terrainPitchRad ?? 0
    const roll = s.terrainRollRad ?? 0
    const poseDrop = Math.max(
      VISUAL_GROUND_BIAS_M,
      Math.abs(Math.sin(pitch)) * TRACTOR_CONFIG.wheelbaseM * 0.22,
      Math.abs(Math.sin(roll)) * Math.max(TRACTOR_CONFIG.frontTrackM, TRACTOR_CONFIG.rearTrackM) * 0.18,
    )
    body.position.set(x, (s.altitude ?? 0) - poseDrop, z)
    // Roll sign: heading rotation here (`+π`) flips the local Z axis vs world,
    // so a positive terrainRollRad (right side higher) needs negation to make
    // the body lean visually correctly (right side up).
    tmpEuler.set(pitch, headingRad + Math.PI, -roll, 'YXZ')
    body.quaternion.setFromEuler(tmpEuler)

    const frontOmega = s.wheelSpeed ?? (s.speed / assembly.frontWheelRadius)
    const rearOmega = s.wheelSpeed ?? (s.speed / assembly.rearWheelRadius)
    const frontAngle = -frontOmega * dt * speedMultiplier
    const rearAngle = -rearOmega * dt * speedMultiplier
    if (assembly.flSpin) assembly.flSpin.rotateX(frontAngle)
    if (assembly.frSpin) assembly.frSpin.rotateX(frontAngle)
    if (assembly.rlSpin) assembly.rlSpin.rotateX(rearAngle)
    if (assembly.rrSpin) assembly.rrSpin.rotateX(rearAngle)

    const steerRad = -s.vehicleFrontWheelAngleDeg * DEG2RAD
    if (assembly.flSteer) assembly.flSteer.rotation.y = steerRad
    if (assembly.frSteer) assembly.frSteer.rotation.y = steerRad

    const impl = implementGroupRef.current
    if (impl) {
      impl.visible = implementType !== null

      // Lerp hitch toward raised (1) or lowered (0) over ~0.6s.
      const hitchTarget = props.hitchDown ? 0 : 1
      hitchYRef.current += (hitchTarget - hitchYRef.current) * Math.min(1, dt / 0.6)
      const baseY = 0.5 - (props.implementDepth ?? 0.25) * 0.4
      // Working = baseY + 0.55, raised = baseY + 1.10
      const liftMeters = hitchYRef.current * 0.55
      impl.position.y = baseY + 0.55 + liftMeters
      impl.rotation.x = 0

      // Update 3-point linkage arm positions
      const iy = impl.position.y
      const lowerLeftEndX  = -ATTACH_LOWER_HALF_W
      const lowerLeftEndY  = iy + ATTACH_LOWER_OFFSET_Y
      const lowerLeftEndZ  = implementZ + ATTACH_OFFSET_Z
      alignArm(armLowerLeftRef,  -HINGE_LOWER_HALF_W, HINGE_LOWER_Y, HINGE_Z,
                                  lowerLeftEndX,     lowerLeftEndY,  lowerLeftEndZ)
      alignArm(armLowerRightRef,  HINGE_LOWER_HALF_W, HINGE_LOWER_Y, HINGE_Z,
                                  ATTACH_LOWER_HALF_W, iy + ATTACH_LOWER_OFFSET_Y, lowerLeftEndZ)
      alignArm(armUpperRef,       0, HINGE_UPPER_Y, HINGE_Z,
                                  0, iy + ATTACH_UPPER_OFFSET_Y, lowerLeftEndZ + 0.02)
    }

    if (implementType === 'rotary' && rotaryDrumRef.current) {
      // Down-cut tillage: drum spins around its own axis (world X /
      // tractor left-right) so tines at the bottom move opposite to the
      // tractor's direction of travel. The drum is wrapped in an outer
      // group with rotation.z = π/2 (lay sideways) and the ref points to
      // an INNER group with no static rotation, so updating rotation.y
      // here rotates around the inner group's body Y axis = the drum's
      // own cylinder axis (which the outer group has mapped to parent X).
      // Sign is negative for down-cut (bottom tines move opposite soil).
      const omega = (Math.abs(s.speed) / 0.25) * 3
      rotaryDrumRef.current.rotation.y -= omega * dt
    }
  })

  return (
    <primitive object={assembly.group}>
      <group ref={implementGroupRef} position={[0, 0.5 - (props.implementDepth ?? 0.25) * 0.4 + 0.55 + (props.hitchDown ? 0 : 0.55), implementZ]} rotation-x={0}>
        {implementType === 'plow' && (
          <>
            {/* === Hitch linkage: yoke (3 short struts converging at a
                central pivot) + single thick shaft into the tractor body === */}
            {/* Three implement-side struts converging to the central pivot
                at (0, 0.5, -0.15). Endpoints on the implement: top pin
                (0, 0.55, 0.18) and the two lower hitch stubs at
                (±0.3, 0.06, 0.04). Strut helper computes 3D rotation. */}
            <Strut from={[0, 0.55, 0.18]} to={[0, 0.5, -0.15]} radius={0.025} color="#2a2a2a" />
            <Strut from={[-0.3, 0.06, 0.04]} to={[0, 0.5, -0.15]} radius={0.03} color="#2a2a2a" />
            <Strut from={[0.3, 0.06, 0.04]} to={[0, 0.5, -0.15]} radius={0.03} color="#2a2a2a" />

            {/* Central pivot block — small visible "ball" where the three
                struts converge, also the front anchor of the main shaft. */}
            <mesh castShadow receiveShadow position={[0, 0.5, -0.15]}>
              <boxGeometry args={[0.12, 0.12, 0.12]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>

            {/* Single thick main shaft — runs from the pivot back into the
                tractor body. Far end z = -1.0 sinks 0.5m inside the
                tractor's rear bbox so the shaft visually emerges from the
                body instead of floating. Length 0.85, axis along Z so the
                cylinder's default Y axis rotates by π/2 about X. */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.5, -0.575]}
              rotation={[Math.PI / 2, 0, 0]}
            >
              <cylinderGeometry args={[0.07, 0.07, 0.85, 14]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>

            {/* === A-frame 3-point hitch tower (front of implement) === */}
            {/* Top pin block */}
            <mesh castShadow receiveShadow position={[0, 0.55, 0.18]}>
              <boxGeometry args={[0.14, 0.1, 0.14]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            {/* Two upper diagonal links converging to top pin */}
            {[-1, 1].map((side) => (
              <mesh
                key={`uplink-${side}`}
                castShadow
                receiveShadow
                position={[side * 0.16, 0.32, 0.22]}
                rotation={[0, 0, side * 0.5]}
              >
                <boxGeometry args={[0.07, 0.5, 0.07]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
              </mesh>
            ))}
            {/* Lower cross-bar (between the two lower 3-point hitch link pins) */}
            <mesh castShadow receiveShadow position={[0, 0.06, 0.22]}>
              <boxGeometry args={[0.62, 0.1, 0.12]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            {/* Two lower hitch pin stubs pointing forward */}
            {[-1, 1].map((side) => (
              <mesh
                key={`pin-${side}`}
                castShadow
                receiveShadow
                position={[side * 0.3, 0.06, 0.13]}
              >
                <boxGeometry args={[0.08, 0.08, 0.18]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
              </mesh>
            ))}

            {/* === 3-point linkage attachment hardware (implement-side) === */}
            {/* Front cross-tube — horizontal bar where the two lower arms attach */}
            <mesh castShadow receiveShadow position={[0, ATTACH_LOWER_OFFSET_Y, ATTACH_OFFSET_Z]}>
              <boxGeometry args={[ATTACH_LOWER_HALF_W * 2 + 0.18, 0.10, 0.10]} />
              <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
            </mesh>
            {/* Ball-cup housings at each end of the cross-tube (lower arm pin sockets) */}
            <mesh castShadow receiveShadow position={[-ATTACH_LOWER_HALF_W, ATTACH_LOWER_OFFSET_Y, ATTACH_OFFSET_Z]}>
              <sphereGeometry args={[0.075, 12, 8]} />
              <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
            </mesh>
            <mesh castShadow receiveShadow position={[ATTACH_LOWER_HALF_W, ATTACH_LOWER_OFFSET_Y, ATTACH_OFFSET_Z]}>
              <sphereGeometry args={[0.075, 12, 8]} />
              <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
            </mesh>
            {/* A-frame mast — two slanted struts rising from cross-tube ends to upper attach point */}
            <group
              position={[aframeMast.strutA.midX, aframeMast.strutA.midY, aframeMast.strutA.midZ]}
              rotation={[0, 0, aframeMast.strutA.rotZ]}
            >
              <mesh castShadow receiveShadow>
                <boxGeometry args={[0.05, aframeMast.strutA.len, 0.05]} />
                <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
              </mesh>
            </group>
            <group
              position={[aframeMast.strutB.midX, aframeMast.strutB.midY, aframeMast.strutB.midZ]}
              rotation={[0, 0, aframeMast.strutB.rotZ]}
            >
              <mesh castShadow receiveShadow>
                <boxGeometry args={[0.05, aframeMast.strutB.len, 0.05]} />
                <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
              </mesh>
            </group>
            {/* Upper attachment lug at the top of the A-frame mast */}
            <mesh castShadow receiveShadow position={[0, ATTACH_UPPER_OFFSET_Y, aframeMast.topZ]}>
              <sphereGeometry args={[0.06, 10, 6]} />
              <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
            </mesh>

            {/* === Cradle / sub-frame — bridges the hitch tower base to the
                center of the diagonal main beam, replacing the gap that opened
                when the beam moved further back to span 5 bodies. */}
            <mesh castShadow receiveShadow position={[0, 0.15, 0.655]}>
              <boxGeometry args={[0.22, 0.18, 0.94]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>

            {/* === Main diagonal beam — threads through all plow bodies ===
                Spans shareXs[0]..shareXs[last] in X and z=0.45..1.45 in Z.
                Midpoint and yaw are computed dynamically from plowWidth. */}
            {(() => {
              const beamDz = 1.0
              const beamYaw = Math.atan2(plowWidth, beamDz)
              const beamLen = Math.sqrt(plowWidth * plowWidth + beamDz * beamDz) + 0.4
              return (
                <mesh
                  castShadow
                  receiveShadow
                  position={[0, 0.20, 0.95]}
                  rotation={[0, beamYaw, 0]}
                >
                  <boxGeometry args={[0.14, 0.16, beamLen]} />
                  <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
                </mesh>
              )
            })()}

            {/* === Plow bodies hanging from the main beam (dynamic count) === */}
            {shareXs.map((x, i) => {
              const z = 0.45 + (i / Math.max(1, shareCount - 1)) * 1.0
              return (
                <group key={`body-${i}`} position={[x, 0, z]}>
                  {/* Standard — slightly back-tilted vertical leg */}
                  <mesh
                    castShadow
                    receiveShadow
                    position={[0, -0.2, -0.06]}
                    rotation={[-0.1, 0, 0]}
                  >
                    <boxGeometry args={[0.09, 0.55, 0.13]} />
                    <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
                  </mesh>
                  {/* Frog — joining block at the base of the standard */}
                  <mesh castShadow receiveShadow position={[0, -0.5, -0.05]}>
                    <boxGeometry args={[0.18, 0.16, 0.22]} />
                    <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
                  </mesh>
                  {/* Landside — vertical steel plate stabilizing against furrow wall */}
                  <mesh castShadow receiveShadow position={[-0.14, -0.5, 0.05]}>
                    <boxGeometry args={[0.025, 0.2, 0.42]} />
                    <meshStandardMaterial color="#bcbcbc" metalness={0.9} roughness={0.25} />
                  </mesh>
                  {/* Share — polished cutting blade, forward-tilted and yawed */}
                  <mesh
                    castShadow
                    receiveShadow
                    position={[0.07, -0.58, -0.12]}
                    rotation={[0.2, 0.35, 0]}
                  >
                    <boxGeometry args={[0.42, 0.04, 0.42]} />
                    <meshStandardMaterial color="#bcbcbc" metalness={0.9} roughness={0.25} />
                  </mesh>
                  {/* Moldboard — curved shell sitting on the share */}
                  <mesh
                    castShadow
                    receiveShadow
                    position={[0.11, -0.38, -0.08]}
                    rotation={[Math.PI / 2, 0.35, 0.45]}
                  >
                    <cylinderGeometry args={[0.28, 0.28, 0.42, 14, 1, true, 0, Math.PI * 0.85]} />
                    <meshStandardMaterial
                      color="#bcbcbc"
                      metalness={0.9}
                      roughness={0.25}
                      side={THREE.DoubleSide}
                    />
                  </mesh>
                </group>
              )
            })}

            {/* === Coulter discs ahead of each share (dynamic count) === */}
            {shareXs.map((x, i) => {
              const z = 0.45 + (i / Math.max(1, shareCount - 1)) * 1.0
              return (
                <group key={`coulter-${i}`} position={[x - 0.04, -0.4, z - 0.32]}>
                  {/* Bracket arm down from main beam */}
                  <mesh castShadow receiveShadow position={[0, 0.2, 0.04]}>
                    <boxGeometry args={[0.04, 0.42, 0.08]} />
                    <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
                  </mesh>
                  {/* Coulter disc — vertical cutting wheel, plane parallel to motion */}
                  <mesh castShadow receiveShadow rotation={[0, 0, Math.PI / 2]}>
                    <cylinderGeometry args={[0.15, 0.15, 0.025, 24]} />
                    <meshStandardMaterial color="#bcbcbc" metalness={0.9} roughness={0.25} />
                  </mesh>
                </group>
              )
            })}
          </>
        )}
        {implementType === 'rotary' && (
          <>
            {/* Top link — rigid rod from new bracket apex to tractor upper rear bracket. */}
            <Strut from={[0, 0.62, 0.92]} to={[0, 0.55, -0.25]} radius={0.05} color="#2a2a2a" />
            {/* Top link tractor-side mounting bracket — small block on tractor rear face where rod attaches */}
            <mesh castShadow receiveShadow position={[0, 0.55, -0.20]}>
              <boxGeometry args={[0.18, 0.20, 0.14]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>

            {/* Implement-side top-link A-frame — single thick triangular plate truss extruded across X, base sitting on top panel, apex above peak shaft. Plate spans world X = [-0.10, +0.10]. */}
            <mesh
              castShadow
              receiveShadow
              geometry={hitchBracketGeo}
              rotation={[0, -Math.PI / 2, 0]}
              position={[0.10, 0, 0]}
            >
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            {/* Top-link pin cross-bar — horizontal cylinder along world X at the apex, protrudes ~0.05 past each side of the plate. */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.62, 0.92]}
              rotation={[0, 0, Math.PI / 2]}
            >
              <cylinderGeometry args={[0.03, 0.03, 0.30, 14]} />
              <meshStandardMaterial color="#1a1a1a" metalness={0.7} roughness={0.4} />
            </mesh>

            {/* Implement frame — two diagonal members connecting the top panel front-bottom edge down to the lower hitch pin stubs. Anchors the lower hitch assembly to the implement body. */}
            <Strut from={[-0.30, 0.28, 0.58]} to={[-0.30, 0.06, 0.13]} radius={0.04} color="#2a2a2a" />
            <Strut from={[0.30, 0.28, 0.58]} to={[0.30, 0.06, 0.13]} radius={0.04} color="#2a2a2a" />
            {/* Frame top weld plates — small blocks where the diagonals meet the top panel front-bottom edge */}
            <mesh castShadow receiveShadow position={[-0.30, 0.28, 0.58]}>
              <boxGeometry args={[0.10, 0.06, 0.06]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            <mesh castShadow receiveShadow position={[0.30, 0.28, 0.58]}>
              <boxGeometry args={[0.10, 0.06, 0.06]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            {/* Lower hitch cross-bar between the two lower 3-point pins */}
            <mesh castShadow receiveShadow position={[0, 0.06, 0.22]}>
              <boxGeometry args={[0.62, 0.1, 0.12]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
            </mesh>
            {/* Two lower hitch pin stubs pointing forward */}
            {[-1, 1].map((side) => (
              <mesh
                key={`pin-${side}`}
                castShadow
                receiveShadow
                position={[side * 0.3, 0.06, 0.13]}
              >
                <boxGeometry args={[0.08, 0.08, 0.18]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
              </mesh>
            ))}

            {/* Lower 3-point links — from tractor body rear (z=-0.2, low) outward and back to lower hitch pin stubs */}
            <Strut from={[-0.45, 0.25, -0.2]} to={[-0.3, 0.06, 0.13]} radius={0.035} color="#2a2a2a" />
            <Strut from={[0.45, 0.25, -0.2]} to={[0.3, 0.06, 0.13]} radius={0.035} color="#2a2a2a" />
            {/* Lower link tractor-side mounting brackets */}
            {[-1, 1].map((side) => (
              <mesh
                key={`ll-bracket-${side}`}
                castShadow
                receiveShadow
                position={[side * 0.45, 0.25, -0.20]}
              >
                <boxGeometry args={[0.08, 0.10, 0.08]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
              </mesh>
            ))}

            {/* === PTO drive shaft + universal joint cover at front. Real PTO has
                a U-joint just in front of the gearbox; render a slightly fatter
                cylinder section over that area. */}
            <Strut from={[0, 0.30, -0.7]} to={[0, 0.18, 0.4]} radius={0.05} color="#5a4030" />
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.13, 0.32]}
              rotation={[1.408, 0, 0]}
            >
              <cylinderGeometry args={[0.075, 0.075, 0.12, 12]} />
              <meshStandardMaterial color="#3a3a3a" metalness={0.65} roughness={0.5} />
            </mesh>

            {/* === Gearbox housing — center-top, where PTO power converts to a
                side-chain drive that turns the rotor. */}
            {/* Cylindrical gearbox housing on the PTO line */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.18, 0.5]}
              rotation={[Math.PI / 2, 0, 0]}
            >
              <cylinderGeometry args={[0.16, 0.16, 0.3, 16]} />
              <meshStandardMaterial color="#3a3a3a" metalness={0.7} roughness={0.5} />
            </mesh>
            {/* Gearbox rear mounting flange — slightly larger disc behind
                the cylinder where it bolts to the housing top plate. */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.18, 0.66]}
              rotation={[Math.PI / 2, 0, 0]}
            >
              <cylinderGeometry args={[0.2, 0.2, 0.04, 18]} />
              <meshStandardMaterial color="#3a3a3a" metalness={0.7} roughness={0.5} />
            </mesh>

            {/* Left side panel — pentagon house-shape, rotation.y=+π/2 maps
                shape-X→worldZ, shape-Y→worldY, extrusion-Z→world-(-X).
                Extrusion depth 0.04; local Z=0.02 is slab center → worldX=-0.02.
                Desired world center X=-0.93 → position.x = -0.93+0.02 = -0.91. */}
            <mesh
              castShadow
              receiveShadow
              geometry={pentagonGeo}
              rotation={[0, -Math.PI / 2, 0]}
              position={[-(rotaryWidth / 2 - ROTARY_PANEL_INSET), 0, 0]}
            >
              <meshStandardMaterial color="#d4c41a" metalness={0.4} roughness={0.6} />
            </mesh>
            {/* Right side panel — rotation.y=-π/2 maps extrusion-Z→world+X.
                Local Z=0.02 → worldX=+0.02; desired center X=+0.93 → position.x=+0.91. */}
            <mesh
              castShadow
              receiveShadow
              geometry={pentagonGeo}
              rotation={[0, -Math.PI / 2, 0]}
              position={[rotaryWidth / 2 - ROTARY_PANEL_INSET, 0, 0]}
            >
              <meshStandardMaterial color="#d4c41a" metalness={0.4} roughness={0.6} />
            </mesh>

            {/* Flat horizontal top panel — connects front-shoulder line (z=0.58) to back-shoulder line (z=1.32) at y=0.30 (shoulder height). Pentagon peaks rise above it. */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.30, 0.95]}
            >
              <boxGeometry args={[rotaryWidth, 0.04, 0.74]} />
              <meshStandardMaterial color="#d4c41a" metalness={0.4} roughness={0.6} />
            </mesh>

            {/* Rear external panel — \|||\ profile: short top slant, vertical mid, short bottom slant. Spans full housing width X = ±1.395. */}
            <mesh
              castShadow
              receiveShadow
              geometry={rearPanelGeo}
              rotation={[0, -Math.PI / 2, 0]}
              position={[rotaryWidth / 2, 0, 0]}
            >
              <meshStandardMaterial color="#d4c41a" metalness={0.4} roughness={0.6} />
            </mesh>

            {/* Peak ridge shaft along world X at (0, 0.44, 0.95) */}
            <mesh
              castShadow
              receiveShadow
              position={[0, 0.44, 0.95]}
              rotation={[0, 0, Math.PI / 2]}
            >
              <cylinderGeometry args={[0.06, 0.06, rotaryWidth - 0.09, 20]} />
              <meshStandardMaterial color="#2a2a2a" metalness={0.85} roughness={0.35} />
            </mesh>

            {/* === Two skid shoes — narrow rails under each end cap that ride
                on the soil surface and set tilling depth. */}
            {[-1, 1].map((side) => (
              <mesh
                key={`skid-${side}`}
                castShadow
                receiveShadow
                position={[side * (rotaryWidth / 2), -0.22, 0.95]}
              >
                <boxGeometry args={[0.05, 0.05, 0.55]} />
                <meshStandardMaterial color="#2a2a2a" metalness={0.6} roughness={0.55} />
              </mesh>
            ))}

            {/* === Rotor drum + L-shaped tines.
                Outer group lays the cylinder horizontally (axis along world X);
                inner group spins around its local Y, which after the outer's
                R_z(π/2) lay-sideways rotation points along world ±X — i.e., the
                cylinder's own axis. Without this nesting the per-frame Y update
                combines with the lay-sideways Z rotation under Three.js's default
                'XYZ' Euler order and the drum precesses around world Y instead of
                spinning around its own axis. */}
            <group position={[0, 0.05, 0.95]} rotation={[0, 0, Math.PI / 2]}>
              <group ref={rotaryDrumRef}>
              {/* Drum core */}
              <mesh castShadow receiveShadow>
                <cylinderGeometry args={[0.10, 0.10, drumLength, 20]} />
                <meshStandardMaterial color="#5a4030" metalness={0.5} roughness={0.6} />
              </mesh>

              {/* 6 tine flanges along the drum axis (body Y), each with 6
                  L-shaped tines. Each flange is rotationally offset by π/8 to
                  create a subtle helical pattern. Adjacent tines alternate
                  their L-bend axial direction (+Y / -Y) so the rotor
                  distributes soil to both sides. */}
              {flangeYs.map((flangeY, flangeIdx) => {
                const flangeRotOffset = flangeIdx * (Math.PI / 8)
                return (
                  <group key={`flange-${flangeIdx}`}>
                    {/* Flange disc — thin along Y (perpendicular to drum axis).
                        Default cylinder is already along Y; no extra rotation. */}
                    <mesh castShadow receiveShadow position={[0, flangeY, 0]}>
                      <cylinderGeometry args={[0.12, 0.12, 0.02, 20]} />
                      <meshStandardMaterial color="#2a2a2a" metalness={0.7} roughness={0.5} />
                    </mesh>

                    {/* 6 L-shaped tines around this flange, alternating bend dir */}
                    {[0, 1, 2, 3, 4, 5].map((tineIdx) => {
                      const angle = flangeRotOffset + (tineIdx / 6) * Math.PI * 2
                      const bendSign = (flangeIdx + tineIdx) % 2 === 0 ? 1 : -1
                      return (
                        <group
                          key={`tine-${tineIdx}`}
                          position={[0, flangeY, 0]}
                          rotation={[0, angle, 0]}
                        >
                          {/* Radial part — flat steel blade extending in body
                              +X (radial outward). Inner edge sits on flange rim
                              (flange r = 0.2, tine length 0.18 → center at 0.29).
                              Box dims: X = radial long, Y = axial width (small),
                              Z = tangential thickness (thin) so the broad face
                              faces the rotation direction for soil cutting. */}
                          <mesh castShadow receiveShadow position={[0.17, 0, 0]}>
                            <boxGeometry args={[0.10, 0.04, 0.012]} />
                            <meshStandardMaterial
                              color="#bcbcbc"
                              metalness={0.85}
                              roughness={0.3}
                            />
                          </mesh>
                          {/* L-bend tip — extends axially (body Y) in ±Y
                              depending on bendSign, attached to the outer end
                              of the radial part. Same flat-blade orientation. */}
                          <mesh
                            castShadow
                            receiveShadow
                            position={[0.20, bendSign * 0.05, 0]}
                          >
                            <boxGeometry args={[0.04, 0.10, 0.012]} />
                            <meshStandardMaterial
                              color="#bcbcbc"
                              metalness={0.85}
                              roughness={0.3}
                            />
                          </mesh>
                        </group>
                      )
                    })}
                  </group>
                )
              })}
              </group>
            </group>
          </>
        )}
      </group>
      {/* === Tractor-side hitch brackets — fixed to tractor body, not implement === */}
      {(implementType === 'plow' || implementType === 'rotary') && (
        <>
          {/* Left lower hinge bracket */}
          <mesh castShadow receiveShadow position={[-HINGE_LOWER_HALF_W, HINGE_LOWER_Y, HINGE_Z]}>
            <boxGeometry args={[0.08, 0.12, 0.10]} />
            <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
          </mesh>
          {/* Right lower hinge bracket */}
          <mesh castShadow receiveShadow position={[HINGE_LOWER_HALF_W, HINGE_LOWER_Y, HINGE_Z]}>
            <boxGeometry args={[0.08, 0.12, 0.10]} />
            <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
          </mesh>
          {/* Upper hinge bracket */}
          <mesh castShadow receiveShadow position={[0, HINGE_UPPER_Y, HINGE_Z]}>
            <boxGeometry args={[0.10, 0.14, 0.10]} />
            <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
          </mesh>
        </>
      )}
      {(implementType === 'plow' || implementType === 'rotary') && (
        [armLowerLeftRef, armLowerRightRef, armUpperRef].map((ref, idx) => (
          <group key={idx} ref={ref}>
            <mesh castShadow>
              <cylinderGeometry args={[0.055, 0.055, 1, 12]} />
              <meshStandardMaterial color={LINKAGE_COLOR} metalness={LINKAGE_METAL} roughness={LINKAGE_ROUGH} />
            </mesh>
          </group>
        ))
      )}
    </primitive>
  )
}

export function Tractor(props: TractorProps): JSX.Element {
  const [loadError, setLoadError] = useState(false)

  useEffect(() => {
    fetch(GLB_PATH, { method: 'HEAD' })
      .then((r) => {
        if (!r.ok) {
          setLoadError(true)
        }
      })
      .catch(() => {
        setLoadError(true)
      })
  }, [])

  if (loadError) {
    return <TractorFallback />
  }
  return (
    <Suspense fallback={null}>
      <TractorGLB {...props} />
    </Suspense>
  )
}

export default Tractor

useGLTF.preload(GLB_PATH)
