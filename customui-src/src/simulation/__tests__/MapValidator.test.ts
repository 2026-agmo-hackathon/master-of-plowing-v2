import { describe, expect, it } from 'vitest'
import { validateMap } from '../MapValidator'

function minimalMap(): Record<string, unknown> {
  return {
    schemaVersion: 1,
    id: 'map_min',
    label: 'Minimal Map',
    size: { width: 100, height: 100 },
    heightmap: { type: 'fn', fn: 'flat' },
    textureLayers: [
      { splatChannel: 'r', diffuse: 'dirt', tileSize: 4 },
    ],
  }
}

describe('validateMap', () => {
  it('accepts a valid minimal map and returns typed MapDefinition', () => {
    const m = validateMap(minimalMap())
    expect(m.schemaVersion).toBe(1)
    expect(m.id).toBe('map_min')
    expect(m.label).toBe('Minimal Map')
    expect(m.size.width).toBe(100)
    expect(m.size.height).toBe(100)
    expect(m.heightmap.type).toBe('fn')
    expect(m.textureLayers).toHaveLength(1)
    expect(m.textureLayers[0].splatChannel).toBe('r')
  })

  it('rejects missing id', () => {
    const raw = minimalMap()
    delete raw.id
    expect(() => validateMap(raw)).toThrow(/id/)
  })

  it('rejects polygon with fewer than 3 points', () => {
    const raw = minimalMap()
    raw.fields = [
      {
        id: 'field_bad',
        polygon: [
          [0, 0],
          [10, 0],
        ],
      },
    ]
    expect(() => validateMap(raw)).toThrow(/polygon/)
  })

  it('rejects unknown heightmap type', () => {
    const raw = minimalMap()
    raw.heightmap = { type: 'unknown' }
    expect(() => validateMap(raw)).toThrow(/heightmap/)
  })

  it('rejects invalid decor type', () => {
    const raw = minimalMap()
    raw.decor = [{ type: 'invalid', position: [0, 0] }]
    expect(() => validateMap(raw)).toThrow(/decor/)
  })

  it('rejects schemaVersion mismatch', () => {
    const raw = minimalMap()
    raw.schemaVersion = 2
    expect(() => validateMap(raw)).toThrow(/schemaVersion/)
  })
})
