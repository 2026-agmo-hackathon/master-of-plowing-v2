import * as THREE from 'three'
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js'
import { seededRandom } from './ProceduralTextures'
import type { TerrainSampler } from '../simulation/TerrainSampler'
import { TRACTOR_CONFIG } from '../simulation/TractorConfig'
import { useSessionStore } from '../state/sessionStore'

// Suspension visual offset clamp (meters). Visual-only, does not affect physics.
const SUSPENSION_CLAMP_M = 0.15

export class VehicleRenderer {
  private tractor: THREE.Group
  private dustParticles: THREE.Points
  private dustPositions: Float32Array
  private dustVelocities: Float32Array
  private dustCount = 50
  private materials: THREE.Material[] = []
  private geometries: THREE.BufferGeometry[] = []
  private rnd = seededRandom(9001)

  // Procedural tractor parts (pivot Groups + Meshes).
  private proceduralRoot: THREE.Group | null = null
  private frontAxle: THREE.Group | null = null
  private frontLeftPivot: THREE.Group | null = null
  private frontRightPivot: THREE.Group | null = null
  private frontLeftWheel: THREE.Mesh | null = null
  private frontRightWheel: THREE.Mesh | null = null
  private rearLeftWheel: THREE.Mesh | null = null
  private rearRightWheel: THREE.Mesh | null = null
  private steeringColumn: THREE.Group | null = null
  private steeringWheelMesh: THREE.Mesh | null = null
  private hitch: THREE.Group | null = null
  private gltfUsed = false

  // Rear pivots are added just to allow suspension Y offset for rear wheels too.
  private rearLeftPivot: THREE.Group | null = null
  private rearRightPivot: THREE.Group | null = null
  // Baseline local Y of pivots (before suspension offset) — captured after build.
  private frontPivotBaseY = 0
  private rearPivotBaseY = 0

  // GLB mode pivots (Stage 2). Steer groups rotate on Y for steering, spin groups
  // rotate on X for wheel roll. Rear wheels have no steer group.
  private gltfFrontLeftSteer: THREE.Group | null = null
  private gltfFrontLeftSpin: THREE.Group | null = null
  private gltfFrontRightSteer: THREE.Group | null = null
  private gltfFrontRightSpin: THREE.Group | null = null
  private gltfRearLeftSpin: THREE.Group | null = null
  private gltfRearRightSpin: THREE.Group | null = null
  private gltfFrontWheelRadius = 0.5
  private gltfRearWheelRadius = 0.75
  private gltfFrontWheelAxis: 'x' | 'y' | 'z' = 'x'
  private gltfRearWheelAxis: 'x' | 'y' | 'z' = 'x'

  private readonly terrainSampler: TerrainSampler | null

  constructor(scene: THREE.Scene, terrainSampler?: TerrainSampler) {
    this.terrainSampler = terrainSampler ?? null
    // Tractor group
    this.tractor = new THREE.Group()
    scene.add(this.tractor)

    // Attempt GLB load first. Adopt the model if it has bones (rigged) OR enough
    // meshes to be a parts-separated multi-mesh asset. Otherwise fall back to procedural.
    const loader = new GLTFLoader()
    loader.load(`${import.meta.env.BASE_URL}models/tractor.glb`, (gltf) => {
      const model = gltf.scene
      let hasBones = false
      let meshCount = 0
      model.traverse((c) => {
        if ((c as THREE.Bone).isBone) hasBones = true
        if ((c as THREE.Mesh).isMesh) meshCount++
      })

      // Empty scene only → keep procedural fallback.
      if (!hasBones && meshCount < 1) {
        this.disposeObject3D(model)
        return
      }

      this.gltfUsed = true
      // Remove procedural build if we already added it.
      if (this.proceduralRoot) {
        this.tractor.remove(this.proceduralRoot)
        this.disposeObject3D(this.proceduralRoot)
        this.proceduralRoot = null
      }

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

      // Shadows + register materials for CSM setupShadowMaterial compatibility.
      model.traverse((c) => {
        const mesh = c as THREE.Mesh
        if (mesh.isMesh) {
          mesh.castShadow = true
          mesh.receiveShadow = true
          const mat = mesh.material
          if (mat) {
            const mats = Array.isArray(mat) ? mat : [mat]
            for (const m of mats) {
              if (!this.materials.includes(m)) this.materials.push(m)
            }
          }
        }
      })

      // Stage 2: green tint pass — preserve tires (very dark) and glass (transparent).
      // Near-white materials get a lighter green so roof/cab panels read as tractor green.
      const GREEN = 0x2f7a3a
      const LIGHT_GREEN = 0x5a9440
      model.traverse((c) => {
        const mesh = c as THREE.Mesh
        if (!mesh.isMesh) return
        const mats = Array.isArray(mesh.material) ? mesh.material : [mesh.material]
        for (const mat of mats) {
          if (!mat || !('color' in mat)) continue
          const std = mat as THREE.MeshStandardMaterial
          const col = std.color
          const avg = (col.r + col.g + col.b) / 3
          // Tires / dark rubber: keep as-is.
          if (avg < 0.15) continue
          // Glass / semi-transparent: keep as-is.
          if (std.transparent && std.opacity < 0.8) continue
          if (avg > 0.75) {
            std.color.setHex(LIGHT_GREEN)
          } else {
            std.color.setHex(GREEN)
          }
        }
      })

      this.tractor.add(model)

      // Stage 2: identify wheel meshes and wrap them in steer/spin pivot groups.
      model.updateMatrixWorld(true)

      // 1) Material_#4 계열 원판 mesh 수집
      interface TireMesh { mesh: THREE.Mesh; cx: number; cy: number; cz: number; radius: number }
      const allTires: TireMesh[] = []
      model.traverse((n) => {
        const mesh = n as THREE.Mesh
        if (!mesh.isMesh) return
        const mat = mesh.material as THREE.Material | THREE.Material[]
        const mats = Array.isArray(mat) ? mat : [mat]
        const matName = mats[0]?.name ?? ''
        // Material_4 계열 (AGMO GLB의 타이어)
        if (!/_4([\._]|$)/.test(matName) && matName !== 'Material_4') return
        const bb = new THREE.Box3().setFromObject(mesh)
        const c = bb.getCenter(new THREE.Vector3())
        const s = bb.getSize(new THREE.Vector3())
        const radius = Math.max(s.x, s.y, s.z) / 2
        allTires.push({ mesh, cx: c.x, cy: c.y, cz: c.z, radius })
      })

      // 2) 4 corner 좌표 (현재 트랙터 local에서)
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

      // 3) 각 corner에 가장 가까운 tire 1개씩 할당 (중복 금지)
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

      // 4) 각 바퀴 pivot 생성 + reparent
      const makePivotsForCorner = (
        tire: TireMesh | null,
        withSteer: boolean,
      ): { steer: THREE.Group | null; spin: THREE.Group | null; radius: number } => {
        if (!tire) return { steer: null, spin: null, radius: 0.5 }
        const spin = new THREE.Group()
        let steer: THREE.Group | null = null
        const pivotY = tire.radius  // 지면이 y=0이므로 허브 높이 = radius
        const centerVec = new THREE.Vector3(tire.cx, pivotY, tire.cz)
        if (withSteer) {
          steer = new THREE.Group()
          steer.position.copy(centerVec)
          spin.position.set(0, 0, 0)
          steer.add(spin)
          this.tractor.add(steer)
        } else {
          spin.position.copy(centerVec)
          this.tractor.add(spin)
        }
        spin.attach(tire.mesh)  // world transform 보존
        return { steer, spin, radius: tire.radius }
      }

      const fl = makePivotsForCorner(selectedByCorner.FL, true)
      const fr = makePivotsForCorner(selectedByCorner.FR, true)
      const rl = makePivotsForCorner(selectedByCorner.RL, false)
      const rr = makePivotsForCorner(selectedByCorner.RR, false)

      this.gltfFrontLeftSteer = fl.steer
      this.gltfFrontLeftSpin = fl.spin
      this.gltfFrontRightSteer = fr.steer
      this.gltfFrontRightSpin = fr.spin
      this.gltfRearLeftSpin = rl.spin
      this.gltfRearRightSpin = rr.spin

      this.gltfFrontWheelAxis = 'x'
      this.gltfRearWheelAxis = 'x'
      this.gltfFrontWheelRadius = Math.max(fl.radius, fr.radius, 0.3)
      this.gltfRearWheelRadius = Math.max(rl.radius, rr.radius, 0.3)

    }, undefined, (_err) => {
    })

    // Build the procedural tractor immediately so it renders before/without GLB.
    this.buildProceduralTractor()

    // Dust particle system
    this.dustPositions = new Float32Array(this.dustCount * 3)
    this.dustVelocities = new Float32Array(this.dustCount * 3)
    const dustGeo = new THREE.BufferGeometry()
    dustGeo.setAttribute('position', new THREE.BufferAttribute(this.dustPositions, 3))
    const dustMat = new THREE.PointsMaterial({
      color: 0x8D6E63,
      size: 0.3,
      transparent: true,
      opacity: 0.6,
      depthWrite: false,
    })
    this.dustParticles = new THREE.Points(dustGeo, dustMat)
    scene.add(this.dustParticles)
  }

  private buildProceduralTractor(): void {
    const root = new THREE.Group()

    // Derived dimensions from TRACTOR_CONFIG
    const frontAxleZ = TRACTOR_CONFIG.wheelbaseM / 2
    const rearAxleZ = -TRACTOR_CONFIG.wheelbaseM / 2
    const frontTrackHalf = TRACTOR_CONFIG.frontTrackM / 2
    const rearTrackHalf = TRACTOR_CONFIG.rearTrackM / 2
    const frontR = TRACTOR_CONFIG.frontTireRadiusM
    const rearR = TRACTOR_CONFIG.rearTireRadiusM
    // Body scale baselines: original hardcoded design was wheelbase=2.6m, rearTrack=1.9m
    const wbScale = TRACTOR_CONFIG.wheelbaseM / 2.6
    const twScale = TRACTOR_CONFIG.rearTrackM / 1.9

    // Raise whole procedural body so the tractor rests on wheel bottoms (~ rear radius).
    root.position.y = rearR
    this.proceduralRoot = root
    this.tractor.add(root)

    // --- Materials ---
    const chassisMat = new THREE.MeshStandardMaterial({ color: 0x2a3a2f, roughness: 0.7, metalness: 0.25 })
    const bodyMat = new THREE.MeshStandardMaterial({ color: 0x2f7a3a, roughness: 0.6, metalness: 0.3 })
    const hoodMat = new THREE.MeshStandardMaterial({ color: 0x276a31, roughness: 0.55, metalness: 0.35 })
    const grillMat = new THREE.MeshStandardMaterial({ color: 0x111111, roughness: 0.8, metalness: 0.4 })
    const headlightMat = new THREE.MeshStandardMaterial({
      color: 0xffe8a0, emissive: 0xffc060, emissiveIntensity: 0.8,
      roughness: 0.3, metalness: 0.1,
    })
    const exhaustMat = new THREE.MeshStandardMaterial({ color: 0x333333, roughness: 0.4, metalness: 0.7 })
    const cabFloorMat = new THREE.MeshStandardMaterial({ color: 0x1a1a1a, roughness: 0.8, metalness: 0.2 })
    const pillarMat = new THREE.MeshStandardMaterial({ color: 0x3a3a3a, roughness: 0.5, metalness: 0.5 })
    const roofMat = new THREE.MeshStandardMaterial({ color: 0xe8d050, roughness: 0.5, metalness: 0.2 })
    const seatMat = new THREE.MeshStandardMaterial({ color: 0x4a3020, roughness: 0.8, metalness: 0.1 })
    const tireMat = new THREE.MeshStandardMaterial({ color: 0x111111, roughness: 0.9, metalness: 0.05 })
    const rimMat = new THREE.MeshStandardMaterial({ color: 0xbbbbbb, roughness: 0.35, metalness: 0.8 })
    const steerMat = new THREE.MeshStandardMaterial({ color: 0x222222, roughness: 0.6, metalness: 0.4 })
    const hitchMat = new THREE.MeshStandardMaterial({ color: 0x555555, roughness: 0.5, metalness: 0.6 })
    this.materials.push(
      chassisMat, bodyMat, hoodMat, grillMat, headlightMat, exhaustMat,
      cabFloorMat, pillarMat, roofMat, seatMat, tireMat, rimMat, steerMat, hitchMat,
    )

    // --- Chassis (low frame) ---
    const chassisGeo = new THREE.BoxGeometry(2.0 * twScale, 0.35, 3.8 * wbScale)
    this.geometries.push(chassisGeo)
    const chassis = new THREE.Mesh(chassisGeo, chassisMat)
    chassis.position.set(0, 0.4, 0)
    chassis.castShadow = true
    chassis.receiveShadow = true
    root.add(chassis)

    // --- Hood lower (wide) ---
    const hoodLowerGeo = new THREE.BoxGeometry(1.5 * twScale, 0.5, 1.9 * wbScale)
    this.geometries.push(hoodLowerGeo)
    const hoodLower = new THREE.Mesh(hoodLowerGeo, bodyMat)
    hoodLower.position.set(0, 0.9, frontAxleZ - 0.35 * wbScale)
    hoodLower.castShadow = true
    hoodLower.receiveShadow = true
    root.add(hoodLower)

    // --- Hood upper (narrower, tapered) ---
    const hoodUpperGeo = new THREE.BoxGeometry(1.1 * twScale, 0.45, 1.7 * wbScale)
    this.geometries.push(hoodUpperGeo)
    const hoodUpper = new THREE.Mesh(hoodUpperGeo, hoodMat)
    hoodUpper.position.set(0, 1.4, frontAxleZ - 0.4 * wbScale)
    hoodUpper.castShadow = true
    hoodUpper.receiveShadow = true
    root.add(hoodUpper)

    // --- Radiator grill (front face, dark) ---
    const grillGeo = new THREE.BoxGeometry(1.0 * twScale, 0.4, 0.04)
    this.geometries.push(grillGeo)
    const grill = new THREE.Mesh(grillGeo, grillMat)
    grill.position.set(0, 0.95, frontAxleZ + 0.55 * wbScale)
    grill.castShadow = true
    root.add(grill)

    // Grill horizontal bars (3 slats).
    const grillBarGeo = new THREE.BoxGeometry(1.0 * twScale, 0.04, 0.06)
    this.geometries.push(grillBarGeo)
    for (let i = 0; i < 3; i++) {
      const bar = new THREE.Mesh(grillBarGeo, chassisMat)
      bar.position.set(0, 0.82 + i * 0.13, frontAxleZ + 0.57 * wbScale)
      bar.castShadow = true
      root.add(bar)
    }

    // --- Headlights x2 ---
    const headlightGeo = new THREE.SphereGeometry(0.1, 12, 8)
    this.geometries.push(headlightGeo)
    const headlightL = new THREE.Mesh(headlightGeo, headlightMat)
    headlightL.position.set(-0.55 * twScale, 1.1, frontAxleZ + 0.52 * wbScale)
    headlightL.castShadow = true
    root.add(headlightL)
    const headlightR = new THREE.Mesh(headlightGeo, headlightMat)
    headlightR.position.set(0.55 * twScale, 1.1, frontAxleZ + 0.52 * wbScale)
    headlightR.castShadow = true
    root.add(headlightR)

    // --- Exhaust pipe (vertical muffler, engine left-front) ---
    const exhaustGeo = new THREE.CylinderGeometry(0.08, 0.08, 1.4, 12)
    this.geometries.push(exhaustGeo)
    const exhaust = new THREE.Mesh(exhaustGeo, exhaustMat)
    exhaust.position.set(-0.65 * twScale, 1.8, frontAxleZ * 0.46)
    exhaust.castShadow = true
    root.add(exhaust)

    // --- Cab floor support ---
    const cabFloorGeo = new THREE.BoxGeometry(1.5 * twScale, 0.1, 1.5 * wbScale)
    this.geometries.push(cabFloorGeo)
    const cabFloor = new THREE.Mesh(cabFloorGeo, cabFloorMat)
    cabFloor.position.set(0, 1.15, rearAxleZ * 0.23)
    cabFloor.castShadow = true
    cabFloor.receiveShadow = true
    root.add(cabFloor)

    // --- Cab pillars (4) ---
    const pillarGeo = new THREE.BoxGeometry(0.08, 1.0, 0.08)
    this.geometries.push(pillarGeo)
    const cabCenterZ = rearAxleZ * 0.23
    const pillarPositions: [number, number][] = [
      [-0.7 * twScale, cabCenterZ + 0.75 * wbScale],
      [0.7 * twScale, cabCenterZ + 0.75 * wbScale],
      [-0.7 * twScale, cabCenterZ - 0.75 * wbScale],
      [0.7 * twScale, cabCenterZ - 0.75 * wbScale],
    ]
    for (const [px, pz] of pillarPositions) {
      const pillar = new THREE.Mesh(pillarGeo, pillarMat)
      pillar.position.set(px, 1.65, pz)
      pillar.castShadow = true
      root.add(pillar)
    }

    // --- Cab roof ---
    const roofGeo = new THREE.BoxGeometry(1.7 * twScale, 0.12, 1.7 * wbScale)
    this.geometries.push(roofGeo)
    const roof = new THREE.Mesh(roofGeo, roofMat)
    roof.position.set(0, 2.2, rearAxleZ * 0.23)
    roof.castShadow = true
    roof.receiveShadow = true
    root.add(roof)

    // --- Seat ---
    const seatGeo = new THREE.BoxGeometry(0.45, 0.35, 0.45)
    this.geometries.push(seatGeo)
    const seat = new THREE.Mesh(seatGeo, seatMat)
    seat.position.set(0, 1.35, -0.55)
    seat.castShadow = true
    root.add(seat)

    const seatBackGeo = new THREE.BoxGeometry(0.45, 0.35, 0.08)
    this.geometries.push(seatBackGeo)
    const seatBack = new THREE.Mesh(seatBackGeo, seatMat)
    seatBack.position.set(0, 1.6, -0.75)
    seatBack.castShadow = true
    root.add(seatBack)

    // --- Front axle + steering pivots ---
    const frontAxle = new THREE.Group()
    frontAxle.position.set(0, 0, frontAxleZ)
    root.add(frontAxle)
    this.frontAxle = frontAxle

    const frontTireGeo = new THREE.CylinderGeometry(frontR, frontR, TRACTOR_CONFIG.frontWheelWidthM, 20)
    const frontRimGeo = new THREE.CylinderGeometry(frontR * 0.55, frontR * 0.55, TRACTOR_CONFIG.frontWheelWidthM * 1.05, 16)
    this.geometries.push(frontTireGeo, frontRimGeo)

    const makeFrontWheel = (sign: number): { pivot: THREE.Group, wheel: THREE.Mesh } => {
      // pivot: positioned at wheel hub (steering axis)
      const pivot = new THREE.Group()
      pivot.position.set(sign * frontTrackHalf, 0, 0)
      frontAxle.add(pivot)
      // wheel mesh rotates around its local X (spin). Cylinder default axis is Y → rotate Z by 90° so axle aligns with X.
      const wheel = new THREE.Mesh(frontTireGeo, tireMat)
      wheel.rotation.z = Math.PI / 2
      wheel.castShadow = true
      wheel.receiveShadow = true
      pivot.add(wheel)
      const rim = new THREE.Mesh(frontRimGeo, rimMat)
      rim.rotation.z = Math.PI / 2
      rim.castShadow = true
      pivot.add(rim)
      return { pivot, wheel }
    }

    const fl = makeFrontWheel(-1)
    const fr = makeFrontWheel(+1)
    this.frontLeftPivot = fl.pivot
    this.frontLeftWheel = fl.wheel
    this.frontRightPivot = fr.pivot
    this.frontRightWheel = fr.wheel

    // --- Front fenders (semi-torus over each front wheel) ---
    const frontFenderGeo = new THREE.TorusGeometry(frontR + 0.08, 0.05, 8, 12, Math.PI)
    this.geometries.push(frontFenderGeo)
    const addFrontFender = (pivot: THREE.Group) => {
      const fender = new THREE.Mesh(frontFenderGeo, hoodMat)
      // Torus default plane is XY; rotate Y by π/2 so the arc lies in the YZ plane over the wheel.
      fender.rotation.y = Math.PI / 2
      fender.castShadow = true
      pivot.add(fender)
    }
    addFrontFender(fl.pivot)
    addFrontFender(fr.pivot)

    // --- Rear wheels ---
    const rearTireGeo = new THREE.CylinderGeometry(rearR, rearR, TRACTOR_CONFIG.rearWheelWidthM, 24)
    const rearRimGeo = new THREE.CylinderGeometry(rearR * 0.55, rearR * 0.55, TRACTOR_CONFIG.rearWheelWidthM * 1.05, 16)
    this.geometries.push(rearTireGeo, rearRimGeo)

    const makeRearWheel = (sign: number): { pivot: THREE.Group, wheel: THREE.Mesh } => {
      const pivot = new THREE.Group()
      pivot.position.set(sign * rearTrackHalf, 0, rearAxleZ)
      root.add(pivot)
      const wheel = new THREE.Mesh(rearTireGeo, tireMat)
      wheel.rotation.z = Math.PI / 2
      wheel.castShadow = true
      wheel.receiveShadow = true
      pivot.add(wheel)
      const rim = new THREE.Mesh(rearRimGeo, rimMat)
      rim.rotation.z = Math.PI / 2
      rim.castShadow = true
      pivot.add(rim)
      return { pivot, wheel }
    }
    const rl = makeRearWheel(-1)
    const rr = makeRearWheel(+1)
    this.rearLeftPivot = rl.pivot
    this.rearLeftWheel = rl.wheel
    this.rearRightPivot = rr.pivot
    this.rearRightWheel = rr.wheel

    // --- Rear fenders (larger semi-torus) ---
    const rearFenderGeo = new THREE.TorusGeometry(rearR + 0.1, 0.06, 8, 16, Math.PI)
    this.geometries.push(rearFenderGeo)
    const addRearFender = (pivot: THREE.Group) => {
      const fender = new THREE.Mesh(rearFenderGeo, hoodMat)
      fender.rotation.y = Math.PI / 2
      fender.castShadow = true
      pivot.add(fender)
    }
    addRearFender(rl.pivot)
    addRearFender(rr.pivot)

    // Capture baseline pivot Y (local to axle/root) for suspension offset math.
    this.frontPivotBaseY = this.frontLeftPivot ? this.frontLeftPivot.position.y : 0
    this.rearPivotBaseY = this.rearLeftPivot ? this.rearLeftPivot.position.y : 0

    // --- Steering column + wheel inside cab ---
    const steeringColumn = new THREE.Group()
    steeringColumn.position.set(0, 1.45, -0.1)
    root.add(steeringColumn)
    this.steeringColumn = steeringColumn

    const columnGeo = new THREE.CylinderGeometry(0.04, 0.04, 0.6, 12)
    this.geometries.push(columnGeo)
    const column = new THREE.Mesh(columnGeo, steerMat)
    column.rotation.x = Math.PI / 6  // tilted toward driver
    column.position.y = 0.2
    column.castShadow = true
    steeringColumn.add(column)

    // Steering wheel as a torus ring (rim) — driver-facing. Parent group handles tilt so
    // child rotation.z (applied in update()) still spins around the wheel axis.
    const steeringWheel = new THREE.Group()
    steeringWheel.rotation.x = Math.PI / 6 + Math.PI / 2
    steeringWheel.position.set(0, 0.5, -0.18)
    steeringColumn.add(steeringWheel)

    const steerRingGeo = new THREE.TorusGeometry(0.18, 0.025, 8, 20)
    this.geometries.push(steerRingGeo)
    const steerRing = new THREE.Mesh(steerRingGeo, steerMat)
    steerRing.castShadow = true
    steeringWheel.add(steerRing)

    // 3 spokes (120° apart), lying in the wheel plane (torus local XY).
    const spokeGeo = new THREE.BoxGeometry(0.35, 0.015, 0.015)
    this.geometries.push(spokeGeo)
    for (let i = 0; i < 3; i++) {
      const spoke = new THREE.Mesh(spokeGeo, steerMat)
      spoke.rotation.z = (i * 2 * Math.PI) / 3
      spoke.castShadow = true
      steeringWheel.add(spoke)
    }
    // steeringWheelMesh typed as THREE.Mesh; cast via unknown to keep the field signature
    // while allowing a Group subtree for the spoked torus (update() only touches rotation.z).
    this.steeringWheelMesh = steeringWheel as unknown as THREE.Mesh

    // --- Rear three-point hitch attach point (with visible link arms) ---
    const hitch = new THREE.Group()
    hitch.position.set(0, 0.3, rearAxleZ - 0.9 * wbScale)
    root.add(hitch)
    this.hitch = hitch

    // Top link (horizontal, high).
    const topLinkGeo = new THREE.CylinderGeometry(0.04, 0.04, 0.8, 10)
    this.geometries.push(topLinkGeo)
    const topLink = new THREE.Mesh(topLinkGeo, hitchMat)
    topLink.rotation.z = Math.PI / 2
    topLink.position.set(0, 0.4, 0.1)
    topLink.castShadow = true
    hitch.add(topLink)

    // Lower links x2 (angled downward).
    const lowerLinkGeo = new THREE.CylinderGeometry(0.05, 0.05, 0.9, 10)
    this.geometries.push(lowerLinkGeo)
    const lowerAngle = -Math.PI / 9  // ~ -20°
    const lowerLinkL = new THREE.Mesh(lowerLinkGeo, hitchMat)
    lowerLinkL.rotation.x = lowerAngle
    lowerLinkL.position.set(-0.35, 0.05, 0.1)
    lowerLinkL.castShadow = true
    hitch.add(lowerLinkL)
    const lowerLinkR = new THREE.Mesh(lowerLinkGeo, hitchMat)
    lowerLinkR.rotation.x = lowerAngle
    lowerLinkR.position.set(0.35, 0.05, 0.1)
    lowerLinkR.castShadow = true
    hitch.add(lowerLinkR)
  }

  update(
    x: number, y: number, z: number,
    headingRad: number, pitch: number, roll: number,
    steerAngle: number, speed: number,
    dt?: number
  ): void {
    // Position and rotation
    this.tractor.position.set(x, y + 0.5, z)
    this.tractor.rotation.set(pitch, -headingRad, roll, 'YXZ')

    // Animate procedural tractor parts (only if procedural build is active).
    if (!this.gltfUsed) {
      const dtReal = dt ?? 1 / 60
      // speed is m/s. Spin = v / r.
      const speedMult = useSessionStore.getState().speedMultiplier
      const frontOmega = speed / TRACTOR_CONFIG.frontTireRadiusM
      const rearOmega = speed / TRACTOR_CONFIG.rearTireRadiusM
      if (this.frontLeftWheel) this.frontLeftWheel.rotation.x += frontOmega * dtReal * speedMult
      if (this.frontRightWheel) this.frontRightWheel.rotation.x += frontOmega * dtReal * speedMult
      if (this.rearLeftWheel) this.rearLeftWheel.rotation.x += rearOmega * dtReal * speedMult
      if (this.rearRightWheel) this.rearRightWheel.rotation.x += rearOmega * dtReal * speedMult
      if (this.frontLeftPivot) this.frontLeftPivot.rotation.y = steerAngle
      if (this.frontRightPivot) this.frontRightPivot.rotation.y = steerAngle
      // Steering wheel rotates around column axis (local Z after column tilt).
      if (this.steeringWheelMesh) this.steeringWheelMesh.rotation.z = steerAngle * 8

      // Suspension visualization: sample terrain at each wheel world XZ and offset
      // pivot.position.y by clamped delta (visual only, physics untouched).
      if (this.terrainSampler) {
        const sinH = Math.sin(headingRad)
        const cosH = Math.cos(headingRad)
        // World XZ for each wheel. Tractor model is rotated Math.PI around Y, so
        // local +Z (front axle) maps to world -Z after rotation. We reconstruct
        // world offsets from axle/track constants relative to (x, z) using the
        // convention used by SimulatorView (world north = -Z, heading 0 faces -Z).
        // forward unit = (-sinH, -cosH); right unit = (cosH, -sinH) in (worldX, worldZ).
        const fwdX = -sinH, fwdZ = -cosH
        const rightX = cosH, rightZ = -sinH
        const halfWB = TRACTOR_CONFIG.wheelbaseM / 2
        // Front axle world center
        const fcx = x + fwdX * halfWB
        const fcz = z + fwdZ * halfWB
        const rcx = x - fwdX * halfWB
        const rcz = z - fwdZ * halfWB
        const frontHalfTrack = TRACTOR_CONFIG.frontTrackM / 2
        const rearHalfTrack = TRACTOR_CONFIG.rearTrackM / 2
        const flx = fcx - rightX * frontHalfTrack, flz = fcz - rightZ * frontHalfTrack
        const frx = fcx + rightX * frontHalfTrack, frz = fcz + rightZ * frontHalfTrack
        const rlx = rcx - rightX * rearHalfTrack, rlz = rcz - rightZ * rearHalfTrack
        const rrx = rcx + rightX * rearHalfTrack, rrz = rcz + rightZ * rearHalfTrack

        const hFL = this.terrainSampler.getHeight(flx, flz)
        const hFR = this.terrainSampler.getHeight(frx, frz)
        const hRL = this.terrainSampler.getHeight(rlx, rlz)
        const hRR = this.terrainSampler.getHeight(rrx, rrz)

        const clamp = (v: number) => Math.max(-SUSPENSION_CLAMP_M, Math.min(SUSPENSION_CLAMP_M, v))
        // Deltas are terrain height minus vehicle baseline y. Positive = bump up.
        const oFL = clamp(hFL - y)
        const oFR = clamp(hFR - y)
        const oRL = clamp(hRL - y)
        const oRR = clamp(hRR - y)

        if (this.frontLeftPivot) this.frontLeftPivot.position.y = this.frontPivotBaseY + oFL
        if (this.frontRightPivot) this.frontRightPivot.position.y = this.frontPivotBaseY + oFR
        if (this.rearLeftPivot) this.rearLeftPivot.position.y = this.rearPivotBaseY + oRL
        if (this.rearRightPivot) this.rearRightPivot.position.y = this.rearPivotBaseY + oRR

        // Dev-only debug hook for QA (exposes last-frame offsets).
        if (typeof window !== 'undefined') {
          ;(window as unknown as { __vehicleRenderer?: { debugWheelOffsets: number[] } }).__vehicleRenderer = {
            debugWheelOffsets: [oFL, oFR, oRL, oRR],
          }
        }
      }
    }

    // Animate GLB tractor pivots (Stage 2). Per-axle radius measured from the
    // aggregate wheel bbox at load time for accurate roll speed.
    if (this.gltfUsed) {
      const dtReal = dt ?? 1 / 60
      const rotateSpin = (spin: THREE.Group | null, axis: 'x' | 'y' | 'z', angle: number) => {
        if (!spin) return
        if (axis === 'x') spin.rotateX(angle)
        else if (axis === 'y') spin.rotateY(angle)
        else spin.rotateZ(angle)
      }
      const speedMult = useSessionStore.getState().speedMultiplier
      const frontAngle = -(speed / this.gltfFrontWheelRadius) * dtReal * speedMult
      const rearAngle = -(speed / this.gltfRearWheelRadius) * dtReal * speedMult
      rotateSpin(this.gltfFrontLeftSpin, this.gltfFrontWheelAxis, frontAngle)
      rotateSpin(this.gltfFrontRightSpin, this.gltfFrontWheelAxis, frontAngle)
      rotateSpin(this.gltfRearLeftSpin, this.gltfRearWheelAxis, rearAngle)
      rotateSpin(this.gltfRearRightSpin, this.gltfRearWheelAxis, rearAngle)
      if (this.gltfFrontLeftSteer) this.gltfFrontLeftSteer.rotation.y = -steerAngle
      if (this.gltfFrontRightSteer) this.gltfFrontRightSteer.rotation.y = -steerAngle
    }

    // Dust particles when moving
    const absSpeed = Math.abs(speed)
    if (absSpeed > 0.5) {
      const sinH = Math.sin(headingRad)
      const cosH = Math.cos(headingRad)
      for (let i = 0; i < this.dustCount; i++) {
        const idx = i * 3
        // Reset dead particles
        if (this.dustPositions[idx + 1] < y - 1 || this.rnd() < 0.02) {
          // Spawn behind vehicle
          this.dustPositions[idx] = x - sinH * 2 + (this.rnd() - 0.5) * 1.5
          this.dustPositions[idx + 1] = y + this.rnd() * 0.5
          this.dustPositions[idx + 2] = z - cosH * 2 + (this.rnd() - 0.5) * 1.5
          this.dustVelocities[idx] = (this.rnd() - 0.5) * 0.5
          this.dustVelocities[idx + 1] = this.rnd() * 2
          this.dustVelocities[idx + 2] = (this.rnd() - 0.5) * 0.5
        }
        // Update positions
        this.dustPositions[idx] += this.dustVelocities[idx] * 0.016
        this.dustPositions[idx + 1] += this.dustVelocities[idx + 1] * 0.016
        this.dustPositions[idx + 2] += this.dustVelocities[idx + 2] * 0.016
        this.dustVelocities[idx + 1] -= 0.5 * 0.016  // gravity
      }
      (this.dustParticles.geometry.attributes.position as THREE.BufferAttribute).needsUpdate = true
      this.dustParticles.visible = true
    } else {
      this.dustParticles.visible = false
    }
  }

  /** Rear three-point hitch attach point (children are anchored to the tractor tail). */
  getHitch(): THREE.Group | null {
    return this.hitch
  }

  /** Front axle pivot group (houses steering pivots and front wheels). */
  getFrontAxle(): THREE.Group | null {
    return this.frontAxle
  }

  /** Steering column pivot group (houses the cab steering wheel). */
  getSteeringColumn(): THREE.Group | null {
    return this.steeringColumn
  }

  /** Materials owned by this renderer. Registered with CSM for cascaded shadows. */
  getMaterials(): THREE.Material[] {
    return this.materials
  }

  setPosition(x: number, y: number, z: number): void {
    this.tractor.position.set(x, y + 0.5, z)
  }

  private disposeObject3D(obj: THREE.Object3D): void {
    obj.traverse((c) => {
      const mesh = c as THREE.Mesh
      if (mesh.isMesh) {
        mesh.geometry?.dispose()
        const mat = mesh.material
        if (Array.isArray(mat)) mat.forEach((m) => m.dispose())
        else if (mat) (mat as THREE.Material).dispose()
      }
    })
  }

  dispose(): void {
    this.dustParticles.geometry.dispose()
    ;(this.dustParticles.material as THREE.Material).dispose()
    for (const g of this.geometries) g.dispose()
    for (const m of this.materials) m.dispose()
    this.gltfFrontLeftSteer = null
    this.gltfFrontLeftSpin = null
    this.gltfFrontRightSteer = null
    this.gltfFrontRightSpin = null
    this.gltfRearLeftSpin = null
    this.gltfRearRightSpin = null
  }
}
