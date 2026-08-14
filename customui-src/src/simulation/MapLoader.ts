import type { MapDefinition, MapField } from './MapDefinition'
import { flatTerrainNoise } from './MapTypes'
import { useSessionStore } from '../state/sessionStore'
import { gpsToEnu, setOrigin } from './GpsOrigin'

/**
 * Loaded map: validated definition + height sampling function derived from heightmap source.
 */
export interface LoadedMap {
  def: MapDefinition
  heightFn: (x: number, z: number) => number
  spawn?: { lat: number; lon: number; headingDeg: number }
}

// Radius of Earth in metres (WGS84 mean)
const EARTH_RADIUS_M = 6_371_000

const MAP_LIST_TIMEOUT_MS = 10_000

/**
 * Convert WGS84 [lon, lat] coordinates to ENU [x, z] metres relative to an origin.
 * East = +x, North = +z (y is up but unused here).
 */
function wgs84ToEnu(
  lon: number,
  lat: number,
  originLon: number,
  originLat: number,
): [number, number] {
  const dLat = (lat - originLat) * (Math.PI / 180)
  const dLon = (lon - originLon) * (Math.PI / 180)
  const originLatRad = originLat * (Math.PI / 180)
  const east = dLon * Math.cos(originLatRad) * EARTH_RADIUS_M
  const north = dLat * EARTH_RADIUS_M
  return [east, north]
}

/**
 * Convert a backend boundary polygon ([lon, lat] outer ring) into a MapField
 * with ENU polygon coordinates relative to the given origin.
 * If no origin is supplied, the polygon's first vertex is used as origin.
 */
function polygonToMapField(
  outerRing: [number, number][],
  fieldId: string,
  origin?: { lat: number; lon: number },
): MapField {
  if (!Array.isArray(outerRing) || outerRing.length < 3) {
    throw new Error(`[MapLoader] boundary polygon for "${fieldId}" needs at least 3 points`)
  }
  const originLon = origin ? origin.lon : outerRing[0][0]
  const originLat = origin ? origin.lat : outerRing[0][1]
  const polygon: [number, number][] = outerRing.map(([lon, lat]) =>
    wgs84ToEnu(lon, lat, originLon, originLat),
  )
  return { id: fieldId, polygon }
}

/**
 * Re-project cached raw [lon, lat] polygons to ENU using a shared origin.
 * Returns MapField array ready to pass to GameLoop.setFields().
 */
export function reprojectPolygons(
  rawPolygonsLonLat: [number, number][][],
  origin: { lat: number; lon: number },
): MapField[] {
  return rawPolygonsLonLat.map((ring, i) => {
    try {
      return polygonToMapField(ring, `field_${i}`, origin)
    } catch {
      return null
    }
  }).filter((f): f is MapField => f !== null)
}

type MapListEntry = ReturnType<typeof useSessionStore.getState>['mapList'][number]

function findMapEntry(id: string): MapListEntry | undefined {
  const numericId = Number(id)
  const mapList = useSessionStore.getState().mapList
  return mapList.find(e =>
    Number.isFinite(numericId) && e.id === numericId
  )
}

async function waitForMapEntry(id: string, timeoutMs = MAP_LIST_TIMEOUT_MS): Promise<MapListEntry> {
  // Return immediately if already in mapList
  const existing = findMapEntry(id)
  if (existing) return existing
  // Subscribe to store changes with timeout
  return new Promise<MapListEntry>((resolve, reject) => {
    let settled = false
    const unsubscribe = useSessionStore.subscribe((state) => {
      if (settled) return
      const numericId = Number(id)
      const entry = state.mapList.find(e =>
        Number.isFinite(numericId) && e.id === numericId
      )
      if (entry) {
        settled = true
        clearTimeout(timer)
        unsubscribe()
        resolve(entry)
      }
    })
    const timer = setTimeout(() => {
      if (settled) return
      settled = true
      unsubscribe()
      reject(new Error(`[MapLoader] timeout waiting for map_list entry "${id}"`))
    }, timeoutMs)
  })
}

export async function loadMap(id: string): Promise<LoadedMap> {
  const entry = await waitForMapEntry(id)

  // Anchor the scene's GpsOrigin to this map's origin BEFORE any ENU conversion.
  // This guarantees the polygon, vehicle, and RDDF (when it later arrives) all
  // share the same reference frame. If RDDF later sets a slightly different
  // origin, the version bump there will retrigger consumers.
  const polygonLL = entry.polygonLL ?? []
  if (polygonLL.length < 3) {
    throw new Error(`[MapLoader] map "${id}" has no polygon`)
  }

  // Always set GpsOrigin. Prefer the map's declared origin; fall back to the
  // first polygon vertex so we never proceed with the stale Seoul default.
  let originLat: number, originLon: number
  if (entry.origin) {
    originLat = entry.origin.lat
    originLon = entry.origin.lon
  } else {
    const [fallbackLat, fallbackLon] = polygonLL[0]
    originLat = fallbackLat
    originLon = fallbackLon
    console.warn(
      `[MapLoader] map "${id}" missing 'origin' field; falling back to polygonLL[0] = (${fallbackLat}, ${fallbackLon})`
    )
  }
  setOrigin(originLat, originLon)
  // Convert raw [lat, lon] to ENU [x, z] using the current GpsOrigin.
  const fieldPolygon: [number, number][] = polygonLL.map(([lat, lon]) => {
    const { x, z } = gpsToEnu(lat, lon)
    return [x, z]
  })

  const xs = fieldPolygon.map(([x]) => x)
  const zs = fieldPolygon.map(([, z]) => z)
  const minX = Math.min(...xs), maxX = Math.max(...xs)
  const minZ = Math.min(...zs), maxZ = Math.max(...zs)
  const paddingM = 100
  const width = Math.ceil((maxX - minX) + paddingM * 2)
  const height = Math.ceil((maxZ - minZ) + paddingM * 2)
  const centerX = (minX + maxX) / 2
  const centerZ = (minZ + maxZ) / 2

  const def: MapDefinition = {
    schemaVersion: 1,
    id,
    label: entry.label ?? id,
    size: { width, height, centerX, centerZ },
    heightmap: { type: 'fn', fn: 'flat' },
    textureLayers: [
      { splatChannel: 'r', diffuse: 'grass', tileSize: 4 },
    ],
    fields: [{ id: `field_0`, polygon: fieldPolygon }],
  }

  const spawn = (entry.has_start && entry.start_lat != null && entry.start_lon != null)
    ? { lat: entry.start_lat, lon: entry.start_lon, headingDeg: entry.start_heading_deg ?? 0 }
    : undefined

  return { def, heightFn: flatTerrainNoise, spawn }
}
