import type { JSX } from 'react'

// Rendered below the main terrain (y = -0.05) to avoid z-fighting,
// and uses renderOrder=-1 so it draws first and the main terrain overrides it.
export default function FarGround(): JSX.Element {
  return (
    <mesh
      rotation-x={-Math.PI / 2}
      position={[0, -0.05, 0]}
      renderOrder={-1}
      receiveShadow={false}
      castShadow={false}
    >
      <planeGeometry args={[5000, 5000, 1, 1]} />
      <meshLambertMaterial color="#8a7556" />
    </mesh>
  )
}
