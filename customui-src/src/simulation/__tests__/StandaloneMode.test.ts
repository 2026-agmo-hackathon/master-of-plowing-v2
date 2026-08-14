import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { __resetStandaloneCacheForTest, isStandaloneMode } from '../StandaloneMode'

describe('StandaloneMode', () => {
  beforeEach(() => {
    __resetStandaloneCacheForTest()
  })

  afterEach(() => {
    vi.unstubAllEnvs()
    vi.unstubAllGlobals()
    __resetStandaloneCacheForTest()
  })

  it('returns true when VITE_STANDALONE env flag is "1"', () => {
    vi.stubEnv('VITE_STANDALONE', '1')
    vi.stubGlobal('window', { location: { search: '' } })
    expect(isStandaloneMode()).toBe(true)
  })

  it('returns true when URL query standalone=1 is present', () => {
    vi.stubEnv('VITE_STANDALONE', '')
    vi.stubGlobal('window', { location: { search: '?standalone=1' } })
    expect(isStandaloneMode()).toBe(true)
  })

  it('returns false when neither env flag nor URL query is set', () => {
    vi.stubEnv('VITE_STANDALONE', '')
    vi.stubGlobal('window', { location: { search: '' } })
    expect(isStandaloneMode()).toBe(false)
  })

  it('returns true when VITE_STANDALONE is "true"', () => {
    vi.stubEnv('VITE_STANDALONE', 'true')
    vi.stubGlobal('window', { location: { search: '' } })
    expect(isStandaloneMode()).toBe(true)
  })

  it('returns true when URL query standalone=true is present', () => {
    vi.stubEnv('VITE_STANDALONE', '')
    vi.stubGlobal('window', { location: { search: '?foo=bar&standalone=true' } })
    expect(isStandaloneMode()).toBe(true)
  })

  it('caches the computed value across calls', () => {
    vi.stubEnv('VITE_STANDALONE', '1')
    vi.stubGlobal('window', { location: { search: '' } })
    expect(isStandaloneMode()).toBe(true)

    // Change env after first call — cache should hold the previous value
    vi.stubEnv('VITE_STANDALONE', '')
    expect(isStandaloneMode()).toBe(true)
  })
})
