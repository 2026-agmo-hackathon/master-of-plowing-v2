import { useEffect, useRef } from 'react'
import type { JSX, RefObject } from 'react'
import { useFrame, useThree } from '@react-three/fiber'
import { OrbitControls } from '@react-three/drei'
import type { OrbitControls as OrbitControlsImpl } from 'three-stdlib'
import * as THREE from 'three'
import type { VehicleState } from '../../simulation/VehicleSystem'
import { getOriginLat, getOriginLon, getMetersPerDegLat, getMetersPerDegLon } from '../../simulation/GpsOrigin'

export interface CameraRigProps {
  targetRef: RefObject<VehicleState>
  mode: 'follow' | 'free'
}

const DEG2RAD = Math.PI / 180

const FOLLOW_BEHIND = 16
const FOLLOW_ABOVE = 7
const FOLLOW_AHEAD = 2
const FOLLOW_LERP = 0.1
const LOOK_LERP = 0.15
const INITIAL_FREE_DISTANCE = 9
const INITIAL_FREE_HEIGHT = 4.5

function vehicleWorldXZ(s: VehicleState): { x: number; z: number } {
  const x = (s.longitude - getOriginLon()) * getMetersPerDegLon()
  const z = (s.latitude - getOriginLat()) * getMetersPerDegLat()
  return { x, z }
}

export default function CameraRig(props: CameraRigProps): JSX.Element | null {
  const { targetRef, mode } = props
  const { camera } = useThree()

  const orbitRef = useRef<OrbitControlsImpl | null>(null)
  const prevModeRef = useRef<'follow' | 'free'>(mode)
  const justSwitchedRef = useRef(true)

  // Reusable temporaries to avoid per-frame allocations.
  const desiredPos = useRef(new THREE.Vector3()).current
  const desiredLook = useRef(new THREE.Vector3()).current
  const currentLook = useRef(new THREE.Vector3()).current
  const tmpQuat = useRef(new THREE.Quaternion()).current
  const tmpMatrix = useRef(new THREE.Matrix4()).current
  const upVec = useRef(new THREE.Vector3(0, 1, 0)).current

  useEffect(() => {
    if (prevModeRef.current !== mode) {
      justSwitchedRef.current = true
      prevModeRef.current = mode
    }
  }, [mode])

  useFrame(() => {
    const s = targetRef.current
    if (!s) return
    const { x, z } = vehicleWorldXZ(s)
    const headingRad = s.heading * DEG2RAD

    if (mode === 'follow') {
      // Forward direction in world (matches Tractor heading convention).
      const fx = Math.sin(headingRad)
      const fz = Math.cos(headingRad)

      desiredPos.set(x - fx * FOLLOW_BEHIND, FOLLOW_ABOVE, z - fz * FOLLOW_BEHIND)
      desiredLook.set(x + fx * FOLLOW_AHEAD, 0, z + fz * FOLLOW_AHEAD)

      if (justSwitchedRef.current) {
        // Snap-ish on first frame after switching to follow to avoid a jump.
        camera.position.copy(desiredPos)
        currentLook.copy(desiredLook)
        tmpMatrix.lookAt(camera.position, currentLook, upVec)
        camera.quaternion.setFromRotationMatrix(tmpMatrix)
        justSwitchedRef.current = false
      } else {
        camera.position.lerp(desiredPos, FOLLOW_LERP)
        currentLook.lerp(desiredLook, LOOK_LERP)
        tmpMatrix.lookAt(camera.position, currentLook, upVec)
        tmpQuat.setFromRotationMatrix(tmpMatrix)
        camera.quaternion.slerp(tmpQuat, LOOK_LERP)
      }
    } else {
      // free mode — OrbitControls drives the camera orientation.
      const controls = orbitRef.current
      if (controls) {
        if (justSwitchedRef.current) {
          const fx = Math.sin(headingRad)
          const fz = Math.cos(headingRad)
          camera.position.set(x - fx * INITIAL_FREE_DISTANCE, INITIAL_FREE_HEIGHT, z - fz * INITIAL_FREE_DISTANCE)
          controls.target.set(x, 0, z)
          controls.update()
          justSwitchedRef.current = false
        } else {
          const dx = x - controls.target.x
          const dz = z - controls.target.z
          if (dx !== 0 || dz !== 0) {
            controls.target.x = x
            controls.target.z = z
            camera.position.x += dx
            camera.position.z += dz
            controls.update()
          }
        }
      }
      // Clamp camera Y position to prevent going below ground
      const MIN_CAMERA_Y = 0.3
      if (camera.position.y < MIN_CAMERA_Y) {
        camera.position.y = MIN_CAMERA_Y
        if (controls) controls.update()
      }
    }
  })

  if (mode === 'free') {
    return (
      <OrbitControls
        ref={orbitRef}
        makeDefault
        enableDamping
        maxPolarAngle={Math.PI / 2 - 0.02}
        minDistance={1.5}
        maxDistance={100}
      />
    )
  }
  return null
}
