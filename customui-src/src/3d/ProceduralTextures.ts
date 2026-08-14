import * as THREE from 'three'

/**
 * Seeded noise texture generator — deterministic for regression stability.
 */
export function seededRandom(seed: number): () => number {
  let s = seed
  return () => {
    s = (s * 9301 + 49297) % 233280
    return s / 233280
  }
}

export function makeDirtTexture(size = 256): THREE.DataTexture {
  const rnd = seededRandom(1337)
  const data = new Uint8Array(size * size * 4)
  for (let i = 0; i < size * size; i++) {
    // Brown tones (shade 90~140) with speckle variation
    const shade = 90 + Math.floor(rnd() * 50)
    data[i * 4 + 0] = shade + 30
    data[i * 4 + 1] = shade - 10
    data[i * 4 + 2] = shade - 40
    data[i * 4 + 3] = 255
  }
  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  tex.colorSpace = THREE.SRGBColorSpace
  tex.wrapS = tex.wrapT = THREE.RepeatWrapping
  tex.needsUpdate = true
  return tex
}

export function makeGrassTexture(size = 256): THREE.DataTexture {
  const rnd = seededRandom(7331)
  const data = new Uint8Array(size * size * 4)
  for (let i = 0; i < size * size; i++) {
    // Green tones (shade 50~120) with speckle variation
    const shade = 50 + Math.floor(rnd() * 70)
    data[i * 4 + 0] = shade - 20
    data[i * 4 + 1] = shade + 40
    data[i * 4 + 2] = shade - 30
    data[i * 4 + 3] = 255
  }
  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  tex.colorSpace = THREE.SRGBColorSpace
  tex.wrapS = tex.wrapT = THREE.RepeatWrapping
  tex.needsUpdate = true
  return tex
}

export function makeCultivatedTexture(size = 256): THREE.DataTexture {
  const rnd = seededRandom(5150)
  const data = new Uint8Array(size * size * 4)
  for (let y = 0; y < size; y++) {
    for (let x = 0; x < size; x++) {
      const i = (y * size + x) * 4
      // Ridge pattern (stripe) + brown jitter
      const stripe = Math.sin(x * 0.4) * 15
      const shade = 75 + Math.floor(rnd() * 30) + stripe
      data[i + 0] = Math.max(0, Math.min(255, shade + 25))
      data[i + 1] = Math.max(0, Math.min(255, shade - 20))
      data[i + 2] = Math.max(0, Math.min(255, shade - 45))
      data[i + 3] = 255
    }
  }
  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  tex.colorSpace = THREE.SRGBColorSpace
  tex.wrapS = tex.wrapT = THREE.RepeatWrapping
  tex.needsUpdate = true
  return tex
}

export function makeRoadTexture(size = 256): THREE.DataTexture {
  const rnd = seededRandom(3003)
  const data = new Uint8Array(size * size * 4)
  for (let i = 0; i < size * size; i++) {
    const shade = 60 + Math.floor(rnd() * 25)
    data[i * 4 + 0] = shade
    data[i * 4 + 1] = shade
    data[i * 4 + 2] = shade + 5
    data[i * 4 + 3] = 255
  }
  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  tex.colorSpace = THREE.SRGBColorSpace
  tex.wrapS = tex.wrapT = THREE.RepeatWrapping
  tex.needsUpdate = true
  return tex
}

/**
 * Default splatmap: central cultivated field (B), surrounding grass (G),
 * edge dirt (R), central vertical road (A).
 * size x size RGBA with (R + G + B + A) ~ 255 per pixel.
 */
export function makeDefaultSplatmap(size = 256): THREE.DataTexture {
  const data = new Uint8Array(size * size * 4)
  const cx = size / 2
  const cz = size / 2
  const fieldHalfX = size * 0.17 // field width (central 34%)
  const fieldHalfZ = size * 0.33 // field length (central 66%)
  const roadHalf = size * 0.015

  for (let z = 0; z < size; z++) {
    for (let x = 0; x < size; x++) {
      const i = (z * size + x) * 4
      const dx = Math.abs(x - cx)
      const dz = Math.abs(z - cz)

      // Default: grass
      let r = 0, g = 220, b = 0, a = 0

      // Field interior: cultivated
      if (dx < fieldHalfX && dz < fieldHalfZ) {
        r = 0; g = 0; b = 220; a = 0
      }

      // Central vertical road (outside field only)
      if (dx < roadHalf && !(dx < fieldHalfX && dz < fieldHalfZ)) {
        r = 0; g = 0; b = 0; a = 220
      }

      // Far edge: dirt (blurry boundary)
      const edgeFade = Math.max(dx / cx, dz / cz)
      if (edgeFade > 0.85) {
        const t = (edgeFade - 0.85) / 0.15
        r = Math.floor(220 * t)
        g = Math.max(0, g - Math.floor(220 * t))
      }

      data[i + 0] = r
      data[i + 1] = g
      data[i + 2] = b
      data[i + 3] = a
    }
  }

  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  // Splatmap: no color space conversion (weights are linear)
  tex.colorSpace = THREE.NoColorSpace
  tex.wrapS = tex.wrapT = THREE.ClampToEdgeWrapping
  tex.needsUpdate = true
  return tex
}

/**
 * Composite-field splatmap: 3 field plots (B channel) + horizontal road (A channel)
 * + edge dirt (R) + grass elsewhere (G).
 */
export function makeCompositeFieldSplatmap(size = 256): THREE.DataTexture {
  const data = new Uint8Array(size * size * 4)
  // polygon coords are world units [-600, 600] (width 1200), splatmap [0, size] mapping
  // field-north:  world z in [100, 300],   x in [-80, 80]
  // field-center: world z in [-100, 80],   x in [-80, 80]
  // field-south:  world z in [-300, -120], x in [-80, 80]
  // road:         world x in [-300, 300],  z in [-3, 3] (width 6)
  const mapWidth = 1200
  const mapHeight = 2200

  const inField = (wx: number, wz: number) =>
    Math.abs(wx) <= 80 && (
      (wz >= 100 && wz <= 300) ||
      (wz >= -100 && wz <= 80) ||
      (wz >= -300 && wz <= -120)
    )

  for (let z = 0; z < size; z++) {
    for (let x = 0; x < size; x++) {
      const i = (z * size + x) * 4
      // splat pixel -> world coord (centered)
      const wx = (x / size - 0.5) * mapWidth
      const wz = (z / size - 0.5) * mapHeight

      let r = 0, g = 200, b = 0, a = 0

      if (inField(wx, wz)) { r = 0; g = 0; b = 220; a = 0 }
      if (Math.abs(wz) < 3 && Math.abs(wx) < 300 && !inField(wx, wz)) {
        r = 0; g = 0; b = 0; a = 220
      }

      // Edge dirt fade
      const edgeFade = Math.max(Math.abs(wx) / (mapWidth / 2), Math.abs(wz) / (mapHeight / 2))
      if (edgeFade > 0.85) {
        const t = (edgeFade - 0.85) / 0.15
        r = Math.floor(220 * t)
        g = Math.max(0, g - Math.floor(220 * t))
      }

      data[i + 0] = r
      data[i + 1] = g
      data[i + 2] = b
      data[i + 3] = a
    }
  }

  const tex = new THREE.DataTexture(data, size, size, THREE.RGBAFormat)
  tex.colorSpace = THREE.NoColorSpace
  tex.wrapS = tex.wrapT = THREE.ClampToEdgeWrapping
  tex.needsUpdate = true
  return tex
}
