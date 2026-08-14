// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 다크 관제 테마의 유일한 근원. ADS 토큰(hex 하드코딩 금지)을 역할별로 매핑한다.
// 대비는 배경 #2C2E31 기준으로 이미 검증된 값이므로 여기서 단계를 바꾸지 말 것:
// 카드를 grey-800 로 올리면 success 가 2.9:1 로 WCAG 미달이 된다. 카드는 grey-900,
// 페이지만 black 을 28% 섞어 한 단 어둡게 유지한다.
//
// Single source of truth for the dark ops theme. Maps ADS tokens (never raw hex)
// to roles. Contrast ratios were verified against #2C2E31 — do not shift steps:
// cards on grey-800 drop success to 2.9:1. Cards stay grey-900; only the page
// mixes 28% black underneath.
// └────────────────────────────────────────────────────────────────────────────┘
export const theme = {
  /** 페이지 배경 — 카드보다 한 단 어둡게 / page bg, one step darker than cards */
  pageBg: 'color-mix(in srgb, var(--ads-color-grey-900) 72%, var(--ads-color-black))',
  /** 카드 배경 / card surface */
  cardBg: 'var(--ads-color-grey-900)',
  /** 카드 테두리·미터 트랙 / card border, meter track */
  border: 'var(--ads-color-grey-800)',
  /** 본문 텍스트 / primary text */
  text: 'var(--ads-color-text-inverse)',
  /** 보조 텍스트 (8.9:1) / secondary text */
  textSub: 'var(--ads-color-white-900)',
  /** 3차 텍스트 (7.9:1) / tertiary text */
  textDim: 'var(--ads-color-grey-500)',
  /** 추세선 전용, 2px 마크만 — 텍스트 금지 (3.4:1) / trend strokes only, never text */
  trend: 'var(--ads-color-grey-600)',
  /** 격자·기준선 (1px 실선) / gridlines, baselines — 1px solid */
  grid: 'var(--ads-color-grey-700)',
  /** 정상 (4.8:1) */ ok: 'var(--ads-color-success)',
  /** 경고 (6.2:1) */ warn: 'var(--ads-color-warning)',
  /** 위험 (4.0:1) */ bad: 'var(--ads-color-critical)',
} as const

/** 크기(절대값) 기준 상태색 — 편차는 부호가 아니라 크기가 나쁨의 척도다.
 *  Status color by magnitude: for deviations, badness is |v|, not sign. */
export function magnitudeColor(
  v: number | undefined,
  warnAt: number,
  badAt: number,
): string {
  if (v === undefined || !Number.isFinite(v)) return theme.textDim
  const m = Math.abs(v)
  if (m >= badAt) return theme.bad
  if (m >= warnAt) return theme.warn
  return theme.ok
}
