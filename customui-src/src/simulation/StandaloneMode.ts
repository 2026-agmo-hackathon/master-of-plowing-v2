// Cached result — computed once on first call per module load
let cached: boolean | null = null;

function compute(): boolean {
  // env first — inlined at build time
  const envFlag = import.meta.env?.VITE_STANDALONE;
  if (envFlag === '1' || envFlag === 'true' || (envFlag as unknown) === true) return true;

  // URL query (browser environment only)
  if (typeof window !== 'undefined' && window.location?.search) {
    const params = new URLSearchParams(window.location.search);
    const q = params.get('standalone');
    if (q === '1' || q === 'true') return true;
  }
  return false;
}

export function isStandaloneMode(): boolean {
  if (cached === null) cached = compute();
  return cached;
}

// Test-only cache reset helper (unused in production)
export function __resetStandaloneCacheForTest(): void {
  cached = null;
}
