// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 인라인 SVG 스파크라인. 추세선은 비강조 회색(grey-600, 2px 마크 전용 대비),
// 현재 값 끝점만 강조색 — 시선이 "지금"에 꽂히게 한다.
//
// 평평한 계열 처리에 주의: 정지 중(값 불변)에는 min==max 라서 순진하게 정규화하면
// 선이 컨테이너 위/아래 끝에 붙어 잘려 보인다(실제로 겪은 버그). 평평하면 인위적
// 범위를 만들어 가운데에 그리고, 모든 계열에 위아래 패딩을 둔다. 0 기준선은
// 0 이 표시 범위 안에 있을 때만 1px 실선으로 그린다.
//
// Inline SVG sparkline. Trend stroke is de-emphasized grey; only the current
// endpoint gets the accent color. Flat-series guard: when min==max (vehicle at
// rest) naive normalization pins the line to the container edge and it looks
// clipped — a bug we actually hit. Flat series render centered; every series
// gets vertical padding; the 0 baseline is drawn only when 0 is in range.
// └────────────────────────────────────────────────────────────────────────────┘
import { theme } from '../theme'

export function Sparkline({
  values,
  width,
  height,
  accent = theme.text,
}: {
  values: number[] | undefined
  width: number
  height: number
  /** 끝점 마커 색 / endpoint marker color */
  accent?: string
}) {
  const w = width
  const h = height
  if (!values || values.length < 2) {
    // 데이터 부족 — 빈 트랙만 그려 자리를 유지 / not enough data: keep the slot
    return (
      <svg width={w} height={h} aria-hidden="true" style={{ flex: 'none' }}>
        <line
          x1={0} y1={h / 2} x2={w} y2={h / 2}
          stroke={theme.border} strokeWidth={1}
        />
      </svg>
    )
  }

  let min = Math.min(...values)
  let max = Math.max(...values)
  if (min === max) {
    // 평평한 계열은 가운데로 / flat series: draw centered
    const span = Math.max(Math.abs(min), 1)
    min -= span * 0.5
    max += span * 0.5
  }
  const pad = (max - min) * 0.15
  min -= pad
  max += pad

  const px = 5 // 끝점 마커가 잘리지 않도록 좌우 여유 / margin so the dot is not clipped
  const x = (i: number) => px + (i / (values.length - 1)) * (w - px * 2)
  const y = (v: number) => px + (1 - (v - min) / (max - min)) * (h - px * 2)
  const points = values.map((v, i) => `${x(i).toFixed(1)},${y(v).toFixed(1)}`).join(' ')
  const lastV = values[values.length - 1]
  const zeroVisible = min <= 0 && 0 <= max

  return (
    <svg width={w} height={h} aria-hidden="true" style={{ flex: 'none' }}>
      {zeroVisible && (
        <line
          x1={0} y1={y(0)} x2={w} y2={y(0)}
          stroke={theme.grid} strokeWidth={1}
        />
      )}
      <polyline
        points={points}
        fill="none"
        stroke={theme.trend}
        strokeWidth={2}
        strokeLinejoin="round"
        strokeLinecap="round"
        vectorEffect="non-scaling-stroke"
      />
      {/* 표면색 2px 링 — 선과 겹쳐도 마커가 분리돼 보이게 / surface ring keeps the dot legible over the line */}
      <circle
        cx={x(values.length - 1)} cy={y(lastV)} r={4.5}
        fill={accent} stroke={theme.cardBg} strokeWidth={2}
      />
    </svg>
  )
}
