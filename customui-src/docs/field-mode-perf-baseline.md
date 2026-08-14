# Field Mode Performance Baseline

## Goal
- FPS ≥ 55 sustained at 1080p
- Draw calls ≤ 30
- GPU memory < 200 MB

## How to measure
1. Run dev server: `npm run dev`
2. Open `http://localhost:3002/?mode=field` in Chrome at 1920×1080 viewport.
3. Press the HUD start button, drive forward for 30 seconds with plow active (`P`).
4. Observe the dev perf panel (top-right) and the drei Stats panel (top-left).
5. Record values below.

## Measurements
| Date | Build | FPS (idle) | FPS (driving + plow) | Draw calls | GPU mem (MB) | Notes |
|------|-------|------------|----------------------|------------|--------------|-------|
|      |       |            |                      |            |              |       |

## Regression policy
- If FPS drops below 55 in driving+plow scenario, investigate before merging.
- If draw calls exceed 30, check for accidental new render passes or per-frame allocations.
- Update this file with each significant change.
