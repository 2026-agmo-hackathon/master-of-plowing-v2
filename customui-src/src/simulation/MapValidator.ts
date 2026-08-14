import type {
  HeightmapRoughness,
  HeightmapSource,
  MapDecorItem,
  MapDefinition,
  MapField,
  MapRoad,
  SplatmapSource,
  TerrainRegion,
  TextureLayer,
} from './MapDefinition'

function isPlainObject(v: unknown): v is Record<string, unknown> {
  return typeof v === 'object' && v !== null && !Array.isArray(v)
}

function isPointTuple(v: unknown): v is [number, number] {
  return (
    Array.isArray(v) &&
    v.length === 2 &&
    typeof v[0] === 'number' &&
    typeof v[1] === 'number' &&
    Number.isFinite(v[0]) &&
    Number.isFinite(v[1])
  )
}

function validateRoughness(raw: unknown, ctx: string): HeightmapRoughness | undefined {
  if (raw === undefined) return undefined
  if (!isPlainObject(raw)) throw new Error(`${ctx}: roughness must be a plain object`)
  const { amplitude, wavelengthM, seed } = raw
  if (typeof amplitude !== 'number' || !Number.isFinite(amplitude) || amplitude < 0 || amplitude > 5) {
    throw new Error(`${ctx}: roughness.amplitude must be a finite number in [0, 5]`)
  }
  if (typeof wavelengthM !== 'number' || !Number.isFinite(wavelengthM) || wavelengthM < 0.1 || wavelengthM > 1000) {
    throw new Error(`${ctx}: roughness.wavelengthM must be a finite number in [0.1, 1000]`)
  }
  const out: HeightmapRoughness = { amplitude, wavelengthM }
  if (seed !== undefined) {
    if (typeof seed !== 'number' || !Number.isFinite(seed)) {
      throw new Error(`${ctx}: roughness.seed must be a finite number when provided`)
    }
    out.seed = seed
  }
  return out
}

function validateHeightmap(raw: unknown, ctx: string): HeightmapSource {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: heightmap must be an object`)
  const t = raw.type
  if (t === 'fn') {
    const fn = raw.fn
    if (fn === 'tilted-plane') {
      const axis = raw.axis
      if (axis !== 'x' && axis !== '-x' && axis !== 'z' && axis !== '-z') {
        throw new Error(`${ctx}: heightmap.axis must be one of 'x'|'-x'|'z'|'-z' for tilted-plane`)
      }
      const slopeRad = raw.slopeRad
      if (typeof slopeRad !== 'number' || !Number.isFinite(slopeRad) || slopeRad < -1.0 || slopeRad > 1.0) {
        throw new Error(`${ctx}: heightmap.slopeRad must be a finite number in [-1.0, 1.0]`)
      }
      const roughness = validateRoughness(raw.roughness, ctx)
      const out: HeightmapSource = { type: 'fn', fn: 'tilted-plane', axis, slopeRad }
      if (roughness !== undefined) out.roughness = roughness
      return out
    }
    if (fn !== 'flat' && fn !== 'single-hill') {
      throw new Error(`${ctx}: heightmap.fn must be one of 'flat'|'single-hill'|'tilted-plane'`)
    }
    const roughness = validateRoughness(raw.roughness, ctx)
    const out: HeightmapSource = roughness !== undefined
      ? { type: 'fn', fn, roughness }
      : { type: 'fn', fn }
    return out
  }
  if (t === 'png') {
    if (typeof raw.url !== 'string' || raw.url.length === 0) {
      throw new Error(`${ctx}: heightmap.url must be a non-empty string`)
    }
    if (raw.scale !== undefined && (typeof raw.scale !== 'number' || !Number.isFinite(raw.scale))) {
      throw new Error(`${ctx}: heightmap.scale must be a number when provided`)
    }
    const out: HeightmapSource = { type: 'png', url: raw.url }
    if (raw.scale !== undefined) out.scale = raw.scale as number
    return out
  }
  throw new Error(`${ctx}: heightmap.type must be 'fn' or 'png' (got ${String(t)})`)
}

function validateSplatmap(raw: unknown, ctx: string): SplatmapSource {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: splatmap must be an object`)
  const t = raw.type
  if (t === 'procedural') {
    const kind = raw.kind
    if (kind !== 'default' && kind !== 'composite-field') {
      throw new Error(`${ctx}: splatmap.kind must be one of 'default'|'composite-field'`)
    }
    return { type: 'procedural', kind }
  }
  if (t === 'png') {
    if (typeof raw.url !== 'string' || raw.url.length === 0) {
      throw new Error(`${ctx}: splatmap.url must be a non-empty string`)
    }
    return { type: 'png', url: raw.url }
  }
  throw new Error(`${ctx}: splatmap.type must be 'procedural' or 'png' (got ${String(t)})`)
}

function validateTextureLayer(raw: unknown, ctx: string): TextureLayer {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: textureLayer must be an object`)
  const ch = raw.splatChannel
  if (ch !== 'r' && ch !== 'g' && ch !== 'b' && ch !== 'a') {
    throw new Error(`${ctx}: textureLayer.splatChannel must be one of 'r'|'g'|'b'|'a'`)
  }
  if (typeof raw.diffuse !== 'string' || raw.diffuse.length === 0) {
    throw new Error(`${ctx}: textureLayer.diffuse must be a non-empty string`)
  }
  if (typeof raw.tileSize !== 'number' || !Number.isFinite(raw.tileSize) || raw.tileSize <= 0) {
    throw new Error(`${ctx}: textureLayer.tileSize must be a positive number`)
  }
  if (raw.normal !== undefined && (typeof raw.normal !== 'string' || raw.normal.length === 0)) {
    throw new Error(`${ctx}: textureLayer.normal must be a non-empty string when provided`)
  }
  const out: TextureLayer = {
    splatChannel: ch,
    diffuse: raw.diffuse,
    tileSize: raw.tileSize,
  }
  if (raw.normal !== undefined) out.normal = raw.normal as string
  return out
}

function validateField(raw: unknown, ctx: string): MapField {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: field must be an object`)
  if (typeof raw.id !== 'string' || raw.id.length === 0) {
    throw new Error(`${ctx}.id must be a non-empty string`)
  }
  if (!Array.isArray(raw.polygon)) throw new Error(`${ctx}.polygon must be an array`)
  if (raw.polygon.length < 3) throw new Error(`${ctx}.polygon must have at least 3 points`)
  const polygon: [number, number][] = []
  for (let i = 0; i < raw.polygon.length; i++) {
    const p = raw.polygon[i]
    if (!isPointTuple(p)) {
      throw new Error(`${ctx}.polygon[${i}] must be a [number, number] tuple`)
    }
    polygon.push(p)
  }
  const crop = raw.crop
  if (crop !== undefined && crop !== 'rice' && crop !== 'wheat' && crop !== 'corn') {
    throw new Error(`${ctx}.crop must be one of 'rice'|'wheat'|'corn' when provided`)
  }
  const out: MapField = { id: raw.id, polygon }
  if (crop !== undefined) out.crop = crop
  return out
}

function validateRoad(raw: unknown, ctx: string): MapRoad {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: road must be an object`)
  if (!Array.isArray(raw.polyline)) throw new Error(`${ctx}.polyline must be an array`)
  if (raw.polyline.length < 2) throw new Error(`${ctx}.polyline must have at least 2 points`)
  const polyline: [number, number][] = []
  for (let i = 0; i < raw.polyline.length; i++) {
    const p = raw.polyline[i]
    if (!isPointTuple(p)) {
      throw new Error(`${ctx}.polyline[${i}] must be a [number, number] tuple`)
    }
    polyline.push(p)
  }
  if (typeof raw.width !== 'number' || !Number.isFinite(raw.width) || raw.width <= 0) {
    throw new Error(`${ctx}.width must be a positive number`)
  }
  return { polyline, width: raw.width }
}

function validateDecor(raw: unknown, ctx: string): MapDecorItem {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: decor must be an object`)
  const t = raw.type
  if (t !== 'tree' && t !== 'fence' && t !== 'barn' && t !== 'silo') {
    throw new Error(`${ctx}.type must be one of 'tree'|'fence'|'barn'|'silo' (got ${String(t)})`)
  }
  if (!isPointTuple(raw.position)) {
    throw new Error(`${ctx}.position must be a [number, number] tuple`)
  }
  if (raw.rotation !== undefined && (typeof raw.rotation !== 'number' || !Number.isFinite(raw.rotation))) {
    throw new Error(`${ctx}.rotation must be a number when provided`)
  }
  if (raw.scale !== undefined && (typeof raw.scale !== 'number' || !Number.isFinite(raw.scale))) {
    throw new Error(`${ctx}.scale must be a number when provided`)
  }
  const out: MapDecorItem = { type: t, position: raw.position }
  if (raw.rotation !== undefined) out.rotation = raw.rotation as number
  if (raw.scale !== undefined) out.scale = raw.scale as number
  return out
}

function validateRegion(raw: unknown, ctx: string): TerrainRegion {
  if (!isPlainObject(raw)) throw new Error(`${ctx}: region must be an object`)
  const t = raw.type
  if (t !== 'ramp' && t !== 'ridge') {
    throw new Error(`${ctx}.type must be one of 'ramp'|'ridge' (got ${String(t)})`)
  }
  if (!Array.isArray(raw.polygon)) throw new Error(`${ctx}.polygon must be an array`)
  if (raw.polygon.length < 3) throw new Error(`${ctx}.polygon must have at least 3 points`)
  const polygon: [number, number][] = []
  for (let i = 0; i < raw.polygon.length; i++) {
    const p = raw.polygon[i]
    if (!isPointTuple(p)) {
      throw new Error(`${ctx}.polygon[${i}] must be a [number, number] tuple`)
    }
    polygon.push(p)
  }
  if (typeof raw.featherM !== 'number' || !Number.isFinite(raw.featherM) || raw.featherM < 0) {
    throw new Error(`${ctx}.featherM must be a non-negative number`)
  }
  if (t === 'ramp') {
    const axis = raw.axis
    if (axis !== 'x' && axis !== '-x' && axis !== 'z' && axis !== '-z') {
      throw new Error(`${ctx}.axis must be one of 'x'|'-x'|'z'|'-z' for ramp`)
    }
    if (typeof raw.slopeRad !== 'number' || !Number.isFinite(raw.slopeRad)) {
      throw new Error(`${ctx}.slopeRad must be a number`)
    }
    return {
      type: 'ramp',
      polygon,
      slopeRad: raw.slopeRad,
      axis,
      featherM: raw.featherM,
    }
  }
  const axis = raw.axis
  if (axis !== 'x' && axis !== 'z') {
    throw new Error(`${ctx}.axis must be one of 'x'|'z' for ridge`)
  }
  if (typeof raw.periodM !== 'number' || !Number.isFinite(raw.periodM) || raw.periodM <= 0) {
    throw new Error(`${ctx}.periodM must be a positive number`)
  }
  if (typeof raw.heightM !== 'number' || !Number.isFinite(raw.heightM)) {
    throw new Error(`${ctx}.heightM must be a number`)
  }
  return {
    type: 'ridge',
    polygon,
    periodM: raw.periodM,
    heightM: raw.heightM,
    axis,
    featherM: raw.featherM,
  }
}

function validateRegions(value: unknown): TerrainRegion[] | undefined {
  if (value === undefined) return undefined
  if (!Array.isArray(value)) throw new Error('MapValidator: regions must be an array when provided')
  return value.map((r, i) => validateRegion(r, `MapValidator: regions[${i}]`))
}

export function validateMap(raw: unknown): MapDefinition {
  if (!isPlainObject(raw)) throw new Error('MapValidator: map must be an object')

  if (raw.schemaVersion !== 1) {
    throw new Error(`MapValidator: schemaVersion must be 1 (got ${String(raw.schemaVersion)})`)
  }
  if (typeof raw.id !== 'string' || raw.id.length === 0) {
    throw new Error('MapValidator: id must be a non-empty string')
  }
  if (typeof raw.label !== 'string' || raw.label.length === 0) {
    throw new Error('MapValidator: label must be a non-empty string')
  }
  if (!isPlainObject(raw.size)) throw new Error('MapValidator: size must be an object')
  if (typeof raw.size.width !== 'number' || !Number.isFinite(raw.size.width) || raw.size.width <= 0) {
    throw new Error('MapValidator: size.width must be a positive number')
  }
  if (typeof raw.size.height !== 'number' || !Number.isFinite(raw.size.height) || raw.size.height <= 0) {
    throw new Error('MapValidator: size.height must be a positive number')
  }

  const heightmap = validateHeightmap(raw.heightmap, 'MapValidator: heightmap')

  if (!Array.isArray(raw.textureLayers) || raw.textureLayers.length < 1) {
    throw new Error('MapValidator: textureLayers must be a non-empty array')
  }
  const textureLayers: TextureLayer[] = raw.textureLayers.map((layer, i) =>
    validateTextureLayer(layer, `MapValidator: textureLayers[${i}]`)
  )

  const out: MapDefinition = {
    schemaVersion: 1,
    id: raw.id,
    label: raw.label,
    size: { width: raw.size.width, height: raw.size.height },
    heightmap,
    textureLayers,
  }

  if (raw.splatmap !== undefined) {
    out.splatmap = validateSplatmap(raw.splatmap, 'MapValidator: splatmap')
  }

  if (raw.fields !== undefined) {
    if (!Array.isArray(raw.fields)) throw new Error('MapValidator: fields must be an array when provided')
    out.fields = raw.fields.map((f, i) => validateField(f, `MapValidator: fields[${i}]`))
  }

  if (raw.roads !== undefined) {
    if (!Array.isArray(raw.roads)) throw new Error('MapValidator: roads must be an array when provided')
    out.roads = raw.roads.map((r, i) => validateRoad(r, `MapValidator: roads[${i}]`))
  }

  if (raw.decor !== undefined) {
    if (!Array.isArray(raw.decor)) throw new Error('MapValidator: decor must be an array when provided')
    out.decor = raw.decor.map((d, i) => validateDecor(d, `MapValidator: decor[${i}]`))
  }

  const regions = validateRegions(raw.regions)
  if (regions !== undefined) {
    out.regions = regions
  }

  if (raw.soilType !== undefined) {
    const st = raw.soilType
    if (st !== 'sandy' && st !== 'loam' && st !== 'clay' && st !== 'wet' && st !== 'hardpack' && st !== 'weedy') {
      throw new Error(`MapValidator: soilType must be one of 'sandy'|'loam'|'clay'|'wet'|'hardpack'|'weedy' (got ${String(st)})`)
    }
    out.soilType = st
  }

  if (raw.originLatLng !== undefined) {
    if (!isPointTuple(raw.originLatLng)) {
      throw new Error('MapValidator: originLatLng must be a [lat, lon] tuple')
    }
    const [lat, lon] = raw.originLatLng
    if (lat < -90 || lat > 90) {
      throw new Error('MapValidator: originLatLng[0] (lat) must be in [-90, 90]')
    }
    if (lon < -180 || lon > 180) {
      throw new Error('MapValidator: originLatLng[1] (lon) must be in [-180, 180]')
    }
    out.originLatLng = [lat, lon]
  }

  return out
}
