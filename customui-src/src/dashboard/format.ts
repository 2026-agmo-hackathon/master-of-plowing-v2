// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 값 표시 규칙의 단일 구현. 값 없음은 0 이 아니라 '—' 로 — 튜닝 중에 "CAN 이
// 안 붙었다"와 "값이 0 이다"를 혼동하면 배선 문제를 게인 문제로 오진한다.
//
// Single implementation of value formatting. Absent values render as '—', never
// 0 — during tuning, confusing "CAN not wired" with "value is zero" leads to
// misdiagnosing a wiring fault as a gain problem.
// └────────────────────────────────────────────────────────────────────────────┘

/** undefined / NaN → '—' */
export function fmt(v: number | undefined, digits = 2): string {
  if (v === undefined || !Number.isFinite(v)) return '—'
  return v.toFixed(digits)
}

/** 부호를 항상 표시 — 편차량은 방향이 정보다 / always show sign: for deviations the direction is information */
export function fmtSigned(v: number | undefined, digits = 2): string {
  if (v === undefined || !Number.isFinite(v)) return '—'
  const s = v.toFixed(digits)
  return v > 0 && !s.startsWith('+') ? `+${s}` : s
}

export function fmtInt(v: number | undefined): string {
  if (v === undefined || !Number.isFinite(v)) return '—'
  return Math.round(v).toString()
}

export function fmtBool(
  v: boolean | undefined,
  yes: string,
  no: string,
): string {
  if (v === undefined) return '—'
  return v ? yes : no
}

export function radToDeg(v: number | undefined): number | undefined {
  return v === undefined ? undefined : (v * 180) / Math.PI
}
