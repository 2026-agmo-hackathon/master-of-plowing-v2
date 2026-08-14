import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js'
import { readFileSync } from 'node:fs'
import path from 'node:path'
import * as THREE from 'three'

globalThis.self = globalThis
const fullPath = path.resolve(process.argv[2])
const buf = readFileSync(fullPath)
const loader = new GLTFLoader()
loader.parse(buf.buffer.slice(buf.byteOffset, buf.byteOffset + buf.byteLength), '', (gltf) => {
  gltf.scene.updateMatrixWorld(true)
  // Apply same transform as VehicleRenderer: Y축 -π/2 회전
  gltf.scene.rotation.y = -Math.PI / 2
  gltf.scene.updateMatrixWorld(true)
  const full = new THREE.Box3().setFromObject(gltf.scene)
  const fc = full.getCenter(new THREE.Vector3())
  const fs = full.getSize(new THREE.Vector3())
  gltf.scene.position.x -= fc.x
  gltf.scene.position.z -= fc.z
  gltf.scene.position.y -= full.min.y
  gltf.scene.updateMatrixWorld(true)

  console.log(`# Full bbox after transform: size(${fs.x.toFixed(2)},${fs.y.toFixed(2)},${fs.z.toFixed(2)})`)
  console.log(`\n# Material_#4 계열 mesh 위치 (bbox center):`)
  const tires = []
  gltf.scene.traverse((n) => {
    if (!n.isMesh) return
    const matName = n.material.name || ''
    if (matName.includes('_4') || matName.includes('Material_4')) {
      const bb = new THREE.Box3().setFromObject(n)
      const c = bb.getCenter(new THREE.Vector3())
      const s = bb.getSize(new THREE.Vector3())
      tires.push({ name: n.name, mat: matName, cx: c.x, cy: c.y, cz: c.z, sx: s.x, sy: s.y, sz: s.z })
    }
  })
  tires.sort((a, b) => a.cx - b.cx || a.cz - b.cz)
  for (const t of tires) {
    console.log(`  ${t.name}  [${t.mat}]  c=(${t.cx.toFixed(2)},${t.cy.toFixed(2)},${t.cz.toFixed(2)}) size=(${t.sx.toFixed(2)},${t.sy.toFixed(2)},${t.sz.toFixed(2)})`)
  }
}, (err) => { console.error(err); process.exit(1) })
