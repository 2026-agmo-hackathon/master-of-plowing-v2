import * as THREE from 'three'

/**
 * Visual-only implement (plow / rotary) attached to the rear three-point hitch.
 * Does not affect physics or ImplementSystem state — SimulatorView drives
 * setType() and setLift() based on plow/rotary UI toggles.
 */
export class ImplementRenderer {
  private parent: THREE.Group
  private plow: THREE.Group
  private rotary: THREE.Group
  private rotaryDrum: THREE.Group | null = null
  private currentType: 'plow' | 'rotary' | null = null
  private materials: THREE.Material[] = []
  private geometries: THREE.BufferGeometry[] = []
  // Lift range: Y offset = -0.3 (lowered) .. 0.0 (raised, 0.3m up from lowered).
  // setLift(value=0) → fully raised (Y = 0), setLift(value=1) → fully lowered (Y = -0.3).
  private liftValue = 0

  constructor(parentGroup: THREE.Group) {
    this.parent = parentGroup

    this.plow = new THREE.Group()
    this.plow.visible = false
    this.buildPlow(this.plow)
    this.parent.add(this.plow)

    this.rotary = new THREE.Group()
    this.rotary.visible = false
    this.buildRotary(this.rotary)
    this.parent.add(this.rotary)

    this.applyLift()
  }

  private buildPlow(group: THREE.Group): void {
    const frameMat = new THREE.MeshStandardMaterial({ color: 0x4a4a4a, metalness: 0.7, roughness: 0.5 })
    const shareMat = new THREE.MeshStandardMaterial({ color: 0x606060, metalness: 0.75, roughness: 0.45 })
    this.materials.push(frameMat, shareMat)

    // Frame: horizontal bar behind hitch.
    const frameGeo = new THREE.BoxGeometry(1.4, 0.15, 0.8)
    this.geometries.push(frameGeo)
    const frame = new THREE.Mesh(frameGeo, frameMat)
    frame.position.set(0, 0, -0.5)
    frame.castShadow = true
    frame.receiveShadow = true
    group.add(frame)

    // Three plow shares (cutting blades), spaced 0.35m apart, tilted forward.
    const shareGeo = new THREE.BoxGeometry(0.15, 0.5, 0.3)
    this.geometries.push(shareGeo)
    for (let i = 0; i < 3; i++) {
      const offsetX = (i - 1) * 0.35
      const share = new THREE.Mesh(shareGeo, shareMat)
      share.position.set(offsetX, -0.35, -0.7)
      share.rotation.x = -0.3
      share.castShadow = true
      share.receiveShadow = true
      group.add(share)
    }
  }

  private buildRotary(group: THREE.Group): void {
    const drumMat = new THREE.MeshStandardMaterial({ color: 0x8a3030, metalness: 0.6, roughness: 0.55 })
    const bladeMat = new THREE.MeshStandardMaterial({ color: 0x2a2a2a, metalness: 0.75, roughness: 0.45 })
    const housingMat = new THREE.MeshStandardMaterial({ color: 0xd4c41a, metalness: 0.4, roughness: 0.6 })
    const shaftMat = new THREE.MeshStandardMaterial({ color: 0x2a2a2a, metalness: 0.85, roughness: 0.35 })
    this.materials.push(drumMat, bladeMat, housingMat, shaftMat)

    // Pentagon "house shape" side end-cap panels.
    // Vertices in (z, y) world coords:
    //   bottom-front  z=-1.05, y=-0.70
    //   bottom-back   z=-0.35, y=-0.70
    //   shoulder-back z=-0.35, y=-0.10
    //   peak          z=-0.70, y=+0.15  (rounded apex)
    //   shoulder-front z=-1.05, y=-0.10
    // Shape is defined in the XY plane of ExtrudeGeometry; we map: shape-X→world-Z, shape-Y→world-Y.
    // After extrusion along local Z (thickness 0.04), we rotate 90° around Y so local-Z becomes world-X.

    const pentShape = new THREE.Shape()
    pentShape.moveTo(-1.05, -0.70)  // bottom-front
    pentShape.lineTo(-0.35, -0.70)  // bottom-back
    pentShape.lineTo(-0.35, -0.10)  // shoulder-back

    // Rounded peak: stop 0.06 before apex on back slope, quadratic through apex, continue on front slope.
    const peakZ = -0.70, peakY = 0.15
    const backShoulderZ = -0.35, backShoulderY = -0.10
    const frontShoulderZ = -1.05, frontShoulderY = -0.10

    const backDZ = peakZ - backShoulderZ   // -0.35
    const backDY = peakY - backShoulderY   // +0.25
    const backLen = Math.sqrt(backDZ * backDZ + backDY * backDY)
    const r = 0.06
    const backT = 1 - r / backLen
    const preApexZ = backShoulderZ + backDZ * backT
    const preApexY = backShoulderY + backDY * backT

    const frontDZ = peakZ - frontShoulderZ  // +0.35
    const frontDY = peakY - frontShoulderY  // +0.25
    const frontLen = Math.sqrt(frontDZ * frontDZ + frontDY * frontDY)
    const frontT = r / frontLen
    const postApexZ = frontShoulderZ + frontDZ * frontT
    const postApexY = frontShoulderY + frontDY * frontT

    pentShape.lineTo(preApexZ, preApexY)
    pentShape.quadraticCurveTo(peakZ, peakY, postApexZ, postApexY)
    pentShape.lineTo(frontShoulderZ, frontShoulderY)  // shoulder-front
    pentShape.closePath()

    const extrudeSettings = { depth: 0.04, bevelEnabled: false }
    const pentGeo = new THREE.ExtrudeGeometry(pentShape, extrudeSettings)
    this.geometries.push(pentGeo)

    // Left side panel at x = -0.85: extrusion depth goes from local-Z=0 to local-Z=0.04.
    // After rotateY(π/2), local-Z becomes world-X. Center at x=-0.85 → offset = -0.85 - 0.02 = -0.87 before rotation.
    // We rotate mesh 90° around Y so extrusion (local +Z) aligns with world +X.
    const leftPanel = new THREE.Mesh(pentGeo, housingMat)
    leftPanel.rotation.y = Math.PI / 2
    // After rotation.y=+π/2: local-X→world-Z, local-Z→world-X (inverted sign).
    // Extrusion spans local-Z 0..0.04 → world-X 0..-0.04.
    // To center on x=-0.85 we need the slab midpoint at x=-0.85: offset +0.02 along local-Z direction (world +X).
    leftPanel.position.set(-0.85 + 0.02, 0, 0)
    leftPanel.castShadow = true
    leftPanel.receiveShadow = true
    group.add(leftPanel)

    // Right side panel at x = +0.85: rotate -π/2 so extrusion goes toward world -X from face.
    const rightPanel = new THREE.Mesh(pentGeo, housingMat)
    rightPanel.rotation.y = -Math.PI / 2
    // After rotation.y=-π/2: local-Z→world -X direction.
    // Extrusion spans local-Z 0..0.04 → world-X 0..+0.04.
    // Center slab at x=+0.85: position at x=0.85 - 0.02.
    rightPanel.position.set(0.85 - 0.02, 0, 0)
    rightPanel.castShadow = true
    rightPanel.receiveShadow = true
    group.add(rightPanel)

    // Front roof slope (상판): plate along the front slope from shoulder-front to peak.
    // Slope vector: from (z=-1.05, y=-0.10) to (z=-0.70, y=+0.15).
    const frontSlopeLen = Math.sqrt(((-0.70) - (-1.05)) ** 2 + (0.15 - (-0.10)) ** 2)  // ≈0.4301
    const frontSlopeMidZ = (-1.05 + -0.70) / 2  // -0.875
    const frontSlopeMidY = (-0.10 + 0.15) / 2   // +0.025
    const frontSlopeAngle = Math.atan2(0.15 - (-0.10), (-0.70) - (-1.05))  // atan2(dY, dZ)
    const frontRoofGeo = new THREE.BoxGeometry(1.7, 0.04, frontSlopeLen)
    this.geometries.push(frontRoofGeo)
    const frontRoof = new THREE.Mesh(frontRoofGeo, housingMat)
    frontRoof.position.set(0, frontSlopeMidY, frontSlopeMidZ)
    // Rotate around X so the plate lies flat along the front slope (tilts in the YZ plane).
    frontRoof.rotation.x = -frontSlopeAngle
    frontRoof.castShadow = true
    frontRoof.receiveShadow = true
    group.add(frontRoof)

    // Rear roof slope (후면 사선 덮개): plate along back slope from shoulder-back to peak.
    // Slope vector: from (z=-0.35, y=-0.10) to (z=-0.70, y=+0.15).
    const rearSlopeLen = Math.sqrt(((-0.70) - (-0.35)) ** 2 + (0.15 - (-0.10)) ** 2)  // ≈0.4301
    const rearSlopeMidZ = (-0.35 + -0.70) / 2   // -0.525
    const rearSlopeMidY = (-0.10 + 0.15) / 2    // +0.025
    const rearSlopeAngle = Math.atan2(0.15 - (-0.10), (-0.70) - (-0.35))  // atan2(dY, dZ) — negative dZ
    const rearRoofGeo = new THREE.BoxGeometry(1.7, 0.04, rearSlopeLen)
    this.geometries.push(rearRoofGeo)
    const rearRoof = new THREE.Mesh(rearRoofGeo, housingMat)
    rearRoof.position.set(0, rearSlopeMidY, rearSlopeMidZ)
    rearRoof.rotation.x = -rearSlopeAngle
    rearRoof.castShadow = true
    rearRoof.receiveShadow = true
    group.add(rearRoof)

    // Front vertical wall at z = -1.05, from y=-0.70 to y=-0.10 (height 0.60).
    const frontWallGeo = new THREE.BoxGeometry(1.7, 0.60, 0.04)
    this.geometries.push(frontWallGeo)
    const frontWall = new THREE.Mesh(frontWallGeo, housingMat)
    frontWall.position.set(0, -0.40, -1.05)
    frontWall.castShadow = true
    frontWall.receiveShadow = true
    group.add(frontWall)

    // Rear vertical wall at z = -0.35, from y=-0.70 to y=-0.10 (height 0.60).
    const rearWallGeo = new THREE.BoxGeometry(1.7, 0.60, 0.04)
    this.geometries.push(rearWallGeo)
    const rearWall = new THREE.Mesh(rearWallGeo, housingMat)
    rearWall.position.set(0, -0.40, -0.35)
    rearWall.castShadow = true
    rearWall.receiveShadow = true
    group.add(rearWall)

    // Top horizontal shaft along the peak ridge (world X axis).
    const shaftGeo = new THREE.CylinderGeometry(0.05, 0.05, 1.9, 20)
    this.geometries.push(shaftGeo)
    const shaft = new THREE.Mesh(shaftGeo, shaftMat)
    shaft.rotation.z = Math.PI / 2
    shaft.position.set(0, 0.15, -0.70)
    shaft.castShadow = true
    group.add(shaft)

    // Drum group — rotated so its cylinder axis is horizontal (cross-track).
    const drumGroup = new THREE.Group()
    drumGroup.position.set(0, -0.35, -0.7)
    drumGroup.rotation.z = Math.PI / 2
    group.add(drumGroup)
    this.rotaryDrum = drumGroup

    const drumGeo = new THREE.CylinderGeometry(0.25, 0.25, 1.5, 16)
    this.geometries.push(drumGeo)
    const drum = new THREE.Mesh(drumGeo, drumMat)
    drum.castShadow = true
    drum.receiveShadow = true
    drumGroup.add(drum)

    // 8 blades around drum circumference.
    const bladeGeo = new THREE.BoxGeometry(0.08, 0.15, 0.15)
    this.geometries.push(bladeGeo)
    const bladeCount = 8
    for (let i = 0; i < bladeCount; i++) {
      const angle = (i / bladeCount) * Math.PI * 2
      // Drum local axis: cylinder's Y (now world X after rotation.z=π/2) runs cross-track.
      // Place blades around the cylinder's radial plane (XZ in drum-local space).
      const blade = new THREE.Mesh(bladeGeo, bladeMat)
      const bladeR = 0.28
      blade.position.set(0, Math.cos(angle) * bladeR, Math.sin(angle) * bladeR)
      blade.rotation.x = angle
      blade.castShadow = true
      drumGroup.add(blade)
    }
  }

  /** Select active implement visual. Pass null to hide both. */
  setType(type: 'plow' | 'rotary' | null): void {
    this.currentType = type
    this.plow.visible = type === 'plow'
    this.rotary.visible = type === 'rotary'
  }

  /** 0 = raised, 1 = lowered. Maps to Y offset -0.3 + value * 0.3 (lower = closer to ground). */
  setLift(value: number): void {
    const v = Math.max(0, Math.min(1, value))
    this.liftValue = v
    this.applyLift()
  }

  private applyLift(): void {
    // value=0 (raised) → Y = 0; value=1 (lowered) → Y = -0.3.
    const y = -0.3 * this.liftValue
    this.plow.position.y = y
    this.rotary.position.y = y
  }

  /** Advance animations. vehicleSpeed in m/s; drum spins proportional to speed. */
  update(dt: number, vehicleSpeed: number): void {
    if (this.currentType === 'rotary' && this.rotaryDrum) {
      // Drum radius ~0.25m; spin rate = speed / radius, scaled up for visual effect.
      const omega = (Math.abs(vehicleSpeed) / 0.25) * 3
      // Local rotation axis is the drum's local Y (cylinder axis).
      this.rotaryDrum.rotation.y += omega * dt
    }
  }

  getMaterials(): THREE.Material[] {
    return this.materials
  }

  dispose(): void {
    this.parent.remove(this.plow)
    this.parent.remove(this.rotary)
    for (const g of this.geometries) g.dispose()
    for (const m of this.materials) m.dispose()
  }
}
