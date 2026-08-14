import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js'
import { readFileSync } from 'node:fs'
import path from 'node:path'
import * as THREE from 'three'

globalThis.self = globalThis
const arg = process.argv[2]
const fullPath = path.resolve(arg)
const buf = readFileSync(fullPath)
const loader = new GLTFLoader()
loader.parse(buf.buffer.slice(buf.byteOffset, buf.byteOffset + buf.byteLength), '', (gltf) => {
  const parts = []
  gltf.scene.updateMatrixWorld(true)
  gltf.scene.traverse((n) => {
    if (!n.isMesh) return
    const bbox = new THREE.Box3().setFromObject(n)
    const c = bbox.getCenter(new THREE.Vector3())
    const s = bbox.getSize(new THREE.Vector3())
    parts.push({ name: n.name, mat: n.material.name, cx: c.x, cy: c.y, cz: c.z, sx: s.x, sy: s.y, sz: s.z })
  })
  // Global bbox
  const global = new THREE.Box3().setFromObject(gltf.scene)
  const gc = global.getCenter(new THREE.Vector3())
  const gs = global.getSize(new THREE.Vector3())
  console.log(`# Global bbox center=(${gc.x.toFixed(2)},${gc.y.toFixed(2)},${gc.z.toFixed(2)}) size=(${gs.x.toFixed(2)},${gs.y.toFixed(2)},${gs.z.toFixed(2)})`)
  console.log(`# Total parts: ${parts.length}`)
  // Classify by position heuristics (x: left/right, z: front/back, y: low/mid/high)
  const gmin = global.min, gmax = global.max
  const yLow = gmin.y + (gmax.y - gmin.y) * 0.25
  const yHigh = gmin.y + (gmax.y - gmin.y) * 0.7
  const zFront = gmin.z + (gmax.z - gmin.z) * 0.7
  const zBack = gmin.z + (gmax.z - gmin.z) * 0.3
  const xLeftMid = gmin.x + (gmax.x - gmin.x) * 0.35
  const xRightMid = gmin.x + (gmax.x - gmin.x) * 0.65

  const categorize = (p) => {
    const side = p.cx < xLeftMid ? 'L' : p.cx > xRightMid ? 'R' : 'C'
    const height = p.cy < yLow ? 'low' : p.cy > yHigh ? 'high' : 'mid'
    const depth = p.cz < zBack ? 'back' : p.cz > zFront ? 'front' : 'mid'
    // Specific heuristics
    const isRoundWheel = Math.abs(p.sy / p.sz - 1) < 0.3 && p.sx < 0.8 // cylinder-like flat
    if (height === 'low' && isRoundWheel) return `wheel(${side}-${depth})`
    if (height === 'high' && depth === 'mid' && side === 'C') return 'cab/roof'
    if (depth === 'front' && height !== 'low') return `hood/front(${side})`
    if (depth === 'back' && height === 'low' && side === 'C') return 'hitch'
    return `${depth}-${height}-${side}`
  }

  // Group by category
  const groups = {}
  for (const p of parts) {
    const cat = categorize(p)
    if (!groups[cat]) groups[cat] = []
    groups[cat].push(p)
  }
  console.log('\n## 카테고리별 파츠 수')
  for (const [cat, arr] of Object.entries(groups).sort((a, b) => b[1].length - a[1].length)) {
    console.log(`  ${cat}: ${arr.length}개`)
  }
  console.log('\n## 주요 파츠 상세 (bbox 기준 카테고리)')
  const interesting = ['wheel', 'cab', 'hood', 'hitch']
  for (const [cat, arr] of Object.entries(groups)) {
    if (!interesting.some(k => cat.includes(k))) continue
    console.log(`\n### ${cat}`)
    for (const p of arr.slice(0, 5)) {
      console.log(`  - ${p.name}  mat=${p.mat}  c=(${p.cx.toFixed(2)},${p.cy.toFixed(2)},${p.cz.toFixed(2)})  size=(${p.sx.toFixed(2)},${p.sy.toFixed(2)},${p.sz.toFixed(2)})`)
    }
    if (arr.length > 5) console.log(`  ... (+${arr.length - 5}개)`)
  }
}, (err) => { console.error(err); process.exit(1) })
