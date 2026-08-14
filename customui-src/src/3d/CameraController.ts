import * as THREE from 'three'

export type CameraMode = 'third-person' | 'top-down' | 'free'

export class CameraController {
  private camera: THREE.PerspectiveCamera
  mode: CameraMode = 'third-person'

  // Free camera state
  private freePhi = 0  // pitch
  private freeTheta = 0  // yaw
  private freeSpeed = 20
  private keys = new Set<string>()
  private mouseDown = false
  private container: HTMLElement | null = null
  private boundKeyDown: ((e: KeyboardEvent) => void) | null = null
  private boundKeyUp: ((e: KeyboardEvent) => void) | null = null
  private boundMouseDown: ((e: MouseEvent) => void) | null = null
  private boundMouseUp: ((e: MouseEvent) => void) | null = null
  private boundMouseMove: ((e: MouseEvent) => void) | null = null

  constructor(camera: THREE.PerspectiveCamera) {
    this.camera = camera
  }

  switchMode(): CameraMode {
    const modes: CameraMode[] = ['third-person', 'top-down', 'free']
    const idx = modes.indexOf(this.mode)
    this.mode = modes[(idx + 1) % modes.length]
    if (this.mode === 'free') {
      // Initialize free camera angles from current camera direction
      const dir = new THREE.Vector3()
      this.camera.getWorldDirection(dir)
      this.freePhi = Math.asin(-dir.y)
      this.freeTheta = Math.atan2(dir.x, dir.z)
    }
    return this.mode
  }

  enableFreeFlight(container: HTMLElement): void {
    this.container = container
    this.boundKeyDown = (e: KeyboardEvent) => this.keys.add(e.key.toLowerCase())
    this.boundKeyUp = (e: KeyboardEvent) => this.keys.delete(e.key.toLowerCase())
    this.boundMouseDown = (e: MouseEvent) => { if (e.button === 2) this.mouseDown = true }
    this.boundMouseUp = () => { this.mouseDown = false }
    this.boundMouseMove = (e: MouseEvent) => {
      if (!this.mouseDown || this.mode !== 'free') return
      this.freeTheta += e.movementX * 0.003
      this.freePhi = Math.max(-Math.PI / 2 + 0.1, Math.min(Math.PI / 2 - 0.1, this.freePhi + e.movementY * 0.003))
    }
    window.addEventListener('keydown', this.boundKeyDown)
    window.addEventListener('keyup', this.boundKeyUp)
    container.addEventListener('mousedown', this.boundMouseDown)
    window.addEventListener('mouseup', this.boundMouseUp)
    container.addEventListener('mousemove', this.boundMouseMove)
    container.addEventListener('contextmenu', (e) => e.preventDefault())
  }

  update(targetX: number, targetY: number, targetZ: number, headingRad: number, dt = 1 / 60): void {
    if (this.mode === 'third-person') {
      const camX = targetX - Math.sin(headingRad) * 12
      const camZ = targetZ - Math.cos(headingRad) * 12
      this.camera.position.lerp(new THREE.Vector3(camX, targetY + 8, camZ), 0.05)
      this.camera.lookAt(targetX, targetY + 1, targetZ)

    } else if (this.mode === 'top-down') {
      this.camera.position.lerp(new THREE.Vector3(targetX, targetY + 50, targetZ + 0.1), 0.05)
      this.camera.lookAt(targetX, targetY, targetZ)

    } else if (this.mode === 'free') {
      // WASD/IJKL movement in free mode
      const moveDir = new THREE.Vector3()
      const forward = new THREE.Vector3(
        Math.sin(this.freeTheta) * Math.cos(this.freePhi),
        -Math.sin(this.freePhi),
        Math.cos(this.freeTheta) * Math.cos(this.freePhi)
      )
      const right = new THREE.Vector3(Math.cos(this.freeTheta), 0, -Math.sin(this.freeTheta))

      if (this.keys.has('i') || this.keys.has('arrowup')) moveDir.add(forward)
      if (this.keys.has('k') || this.keys.has('arrowdown')) moveDir.sub(forward)
      if (this.keys.has('j') || this.keys.has('arrowleft')) moveDir.sub(right)
      if (this.keys.has('l') || this.keys.has('arrowright')) moveDir.add(right)
      if (this.keys.has(' ')) moveDir.y += 1
      if (this.keys.has('shift')) moveDir.y -= 1

      if (moveDir.length() > 0) {
        moveDir.normalize().multiplyScalar(this.freeSpeed * dt)
        this.camera.position.add(moveDir)
      }

      // Look direction from angles
      const lookTarget = this.camera.position.clone().add(forward)
      this.camera.lookAt(lookTarget)
    }
  }

  dispose(): void {
    if (this.boundKeyDown) window.removeEventListener('keydown', this.boundKeyDown)
    if (this.boundKeyUp) window.removeEventListener('keyup', this.boundKeyUp)
    if (this.container && this.boundMouseDown) this.container.removeEventListener('mousedown', this.boundMouseDown)
    if (this.boundMouseUp) window.removeEventListener('mouseup', this.boundMouseUp)
    if (this.container && this.boundMouseMove) this.container.removeEventListener('mousemove', this.boundMouseMove)
  }
}
