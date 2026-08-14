import { describe, expect, it, vi, beforeEach, afterEach } from 'vitest'
import { loadMap } from '../MapLoader'
import { useSessionStore } from '../../state/sessionStore'

// Minimal backend polygon in [lat, lon], matching sessionStore.mapList.
function makeFakePolygon(): [number, number][] {
  return [
    [37.5665, 126.9780],
    [37.5665, 126.9790],
    [37.5672, 126.9790],
    [37.5675, 126.9785],
    [37.5672, 126.9780],
  ]
}

vi.mock('../../state/sessionStore', () => {
  const subscribers: Array<(state: { mapList: { id: number; label: string; polygonLL?: [number, number][] }[] }) => void> = []
  let currentState = { mapList: [] as { id: number; label: string; polygonLL?: [number, number][] }[] }

  const useSessionStore = {
    getState: vi.fn(() => currentState),
    subscribe: vi.fn((cb: (state: typeof currentState) => void) => {
      subscribers.push(cb)
      return () => {
        const idx = subscribers.indexOf(cb)
        if (idx >= 0) subscribers.splice(idx, 1)
      }
    }),
    // Test helper to push new state
    __setState: (state: typeof currentState) => {
      currentState = state
      for (const sub of subscribers) sub(state)
    },
  }

  return { useSessionStore }
})

function setMapList(entries: { id: number; label: string; polygonLL?: [number, number][] }[]) {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  ;(useSessionStore as any).__setState({ mapList: entries })
}

beforeEach(() => {
  vi.useFakeTimers({ shouldAdvanceTime: true })
  // Reset to empty
  setMapList([])
})

afterEach(() => {
  vi.useRealTimers()
  vi.restoreAllMocks()
})

describe('loadMap — store-driven', () => {
  it('resolves with def matching requested id when entry is already in store', async () => {
    setMapList([{ id: 1, label: 'Field One', polygonLL: makeFakePolygon() }])
    const { def } = await loadMap('1')
    expect(def.id).toBe('1')
  })

  it('returns label from store entry', async () => {
    setMapList([{ id: 2, label: 'Test Field', polygonLL: makeFakePolygon() }])
    const { def } = await loadMap('2')
    expect(def.label).toBe('Test Field')
  })

  it('returns at least one field with >= 3 polygon points', async () => {
    setMapList([{ id: 3, label: 'F3', polygonLL: makeFakePolygon() }])
    const { def } = await loadMap('3')
    expect(def.fields!.length).toBeGreaterThanOrEqual(1)
    expect(def.fields![0].polygon.length).toBeGreaterThanOrEqual(3)
  })

  it('throws when polygon is missing', async () => {
    setMapList([{ id: 4, label: 'No poly', polygonLL: [] }])
    await expect(loadMap('4')).rejects.toThrow(/no polygon/)
  })

  it('waits for map list to arrive then resolves', async () => {
    // Entry arrives after a delay
    const promise = loadMap('5')
    // Simulate delayed arrival
    Promise.resolve().then(() => {
      setMapList([{ id: 5, label: 'Delayed', polygonLL: makeFakePolygon() }])
    })
    const { def } = await promise
    expect(def.id).toBe('5')
  })

  it('times out if entry never arrives', async () => {
    const promise = loadMap('99')
    // Advance past the 10s timeout
    vi.advanceTimersByTime(11_000)
    await expect(promise).rejects.toThrow(/timeout/)
  })
})
