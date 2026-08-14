import type { JSX } from 'react'

interface SkyAndLightsProps {
  shadowEnabled?: boolean
  shadowMapSize?: number
  fogDensity?: number
}

export default function SkyAndLights({
  shadowEnabled = true,
  shadowMapSize = 2048,
  fogDensity = 0,
}: SkyAndLightsProps): JSX.Element {
  return (
    <>
      {/* overcast sky tone */}
      <ambientLight color="#c8d0dc" intensity={0.85} />
      <hemisphereLight args={['#9aa3b0', '#6b6558', 0.75]} />
      {shadowEnabled === false ? (
        <directionalLight
          position={[50, 80, 30]}
          color="#dde2ea"
          intensity={0.7}
          castShadow={false}
        />
      ) : (
        <directionalLight
          position={[50, 80, 30]}
          color="#dde2ea"
          intensity={0.7}
          castShadow
          shadow-mapSize-width={shadowMapSize}
          shadow-mapSize-height={shadowMapSize}
          shadow-camera-left={-50}
          shadow-camera-right={50}
          shadow-camera-top={50}
          shadow-camera-bottom={-50}
        />
      )}
      <color attach="background" args={['#7a8290']} />
      {fogDensity > 0 && <fogExp2 attach="fog" args={['#8a92a0', fogDensity]} />}
    </>
  )
}
