// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 관제용 계기 2종. ADS 에 게이지가 없어 인라인 SVG 로 그리되 색은 토큰만 쓴다.
//
// 방위 로즈: 차량 심볼은 항상 위를 향하고 「방위 링이 회전」한다(rotate(-heading)).
// 관제 중에는 "내가 향한 방향이 위"라는 감각이 유지돼야 실수하지 않는다.
// N/E/S/W 는 삼각함수로 위치만 계산해 정립 텍스트로 찍는다(역회전과 등가).
//
// 조향각 아크: 지령·실측 두 바늘을 같은 스케일에 겹친다 — 두 바늘의 벌어짐이
// 곧 조향 오차라서, 스케일 오차·응답 지연이 한눈에 잡힌다. 스케일 상한은
// 앞바퀴 물리 한계 ±32°: 바늘은 끝에서 멈추되 숫자는 실제값을 보여준다.
//
// Two ops gauges as inline SVG (ADS has no gauge), token colors only.
// Compass rose: the vehicle glyph stays pointing up and the bearing ring
// rotates — "my heading is up" must hold during operation. Cardinal letters
// are placed by trig so they stay upright (equivalent to counter-rotation).
// Steering arc: command and measured needles share one scale, so the gap
// between them IS the steering error. Scale caps at the ±32° physical wheel
// limit; needles clamp, numbers show the true value.
// └────────────────────────────────────────────────────────────────────────────┘
import { Paragraph } from '@seamos/ads'
import { theme } from '../theme'
import { fmt, fmtSigned } from '../format'

const D2R = Math.PI / 180

// ── 방위 로즈 / compass rose ──────────────────────────────────────────────────

export function CompassRose({ headingDeg }: { headingDeg: number | undefined }) {
  const size = 196
  const c = size / 2
  const h = headingDeg ?? 0
  const has = headingDeg !== undefined && Number.isFinite(headingDeg)

  // 방위각 a(0=북) 의 화면 위치 — 링이 -heading 만큼 돌아간 상태.
  // Screen position of bearing a with the ring rotated by -heading.
  const pos = (a: number, r: number) => {
    const t = (a - h) * D2R
    return { x: c + r * Math.sin(t), y: c - r * Math.cos(t) }
  }

  const ticks: { a: number; major: boolean }[] = []
  for (let a = 0; a < 360; a += 30) ticks.push({ a, major: a % 90 === 0 })
  const cardinals = [
    { a: 0, ch: 'N', color: theme.bad },
    { a: 90, ch: 'E', color: theme.textSub },
    { a: 180, ch: 'S', color: theme.textSub },
    { a: 270, ch: 'W', color: theme.textSub },
  ]

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', gap: 4 }}>
      <svg width={size} height={size} aria-hidden="true">
        <circle cx={c} cy={c} r={86} fill="none" stroke={theme.border} strokeWidth={2} />
        {ticks.map(({ a, major }) => {
          const o = pos(a, 86)
          const i = pos(a, major ? 74 : 79)
          return (
            <line
              key={a}
              x1={o.x} y1={o.y} x2={i.x} y2={i.y}
              stroke={has ? theme.grid : theme.border}
              strokeWidth={major ? 2 : 1}
              strokeLinecap="round"
              vectorEffect="non-scaling-stroke"
            />
          )
        })}
        {cardinals.map(({ a, ch, color }) => {
          const p = pos(a, 60)
          return (
            <text
              key={ch}
              x={p.x} y={p.y}
              textAnchor="middle" dominantBaseline="central"
              fontSize={16} fontWeight={700}
              fill={has ? color : theme.textDim}
            >
              {ch}
            </text>
          )
        })}
        {/* 차량 — 항상 위 / vehicle glyph, always up */}
        <polygon
          points={`${c},${c - 22} ${c - 13},${c + 16} ${c},${c + 8} ${c + 13},${c + 16}`}
          fill={has ? theme.text : theme.textDim}
          stroke={theme.cardBg}
          strokeWidth={2}
          strokeLinejoin="round"
        />
      </svg>
      <div style={{ display: 'flex', alignItems: 'baseline', gap: 5 }}>
        <Paragraph typography="2xl" fontWeight="bold" color={has ? theme.text : theme.textDim}>
          {fmt(headingDeg, 1)}
        </Paragraph>
        <Paragraph typography="xs" color={theme.textDim}>° (0=북)</Paragraph>
      </div>
    </div>
  )
}

// ── 조향각 아크 / steering arc ────────────────────────────────────────────────

/** 앞바퀴 물리 한계 / physical front-wheel limit */
export const STEER_LIMIT_DEG = 32
/** ±32° 를 수직 기준 ±60° 호에 사상 / map ±32° onto a ±60° arc from vertical */
const ARC_HALF = 60

const CMD_COLOR = theme.warn
const MEAS_COLOR = theme.text

export function SteerArc({
  targetDeg,
  measDeg,
  measValid,
}: {
  targetDeg: number | undefined
  measDeg: number | undefined
  measValid: boolean | undefined
}) {
  const w = 240
  const hgt = 138
  const cx = w / 2
  const cy = 124
  const rArc = 96

  const pt = (deg: number, r: number) => {
    const t = (Math.max(-STEER_LIMIT_DEG, Math.min(STEER_LIMIT_DEG, deg)) / STEER_LIMIT_DEG) * ARC_HALF * D2R
    return { x: cx + r * Math.sin(t), y: cy - r * Math.cos(t) }
  }
  const a0 = pt(-STEER_LIMIT_DEG, rArc)
  const a1 = pt(STEER_LIMIT_DEG, rArc)

  const measShown = measValid !== false && measDeg !== undefined && Number.isFinite(measDeg)
  const needle = (deg: number, color: string) => {
    const tip = pt(deg, rArc - 10)
    return (
      <line
        x1={cx} y1={cy} x2={tip.x} y2={tip.y}
        stroke={color} strokeWidth={3} strokeLinecap="round"
        vectorEffect="non-scaling-stroke"
      />
    )
  }

  return (
    <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', gap: 6 }}>
      <svg width={w} height={hgt} aria-hidden="true">
        <path
          d={`M ${a0.x} ${a0.y} A ${rArc} ${rArc} 0 0 1 ${a1.x} ${a1.y}`}
          fill="none" stroke={theme.border} strokeWidth={8} strokeLinecap="round"
        />
        {[-32, -16, 0, 16, 32].map((d) => {
          const o = pt(d, rArc + 6)
          const i = pt(d, rArc - 6)
          const l = pt(d, rArc + 18)
          return (
            <g key={d}>
              <line
                x1={o.x} y1={o.y} x2={i.x} y2={i.y}
                stroke={theme.grid} strokeWidth={d === 0 ? 2 : 1}
                strokeLinecap="round" vectorEffect="non-scaling-stroke"
              />
              <text
                x={l.x} y={l.y}
                textAnchor="middle" dominantBaseline="central"
                fontSize={12} fill={theme.textDim}
              >
                {d}
              </text>
            </g>
          )
        })}
        {targetDeg !== undefined && Number.isFinite(targetDeg) && needle(targetDeg, CMD_COLOR)}
        {measShown && needle(measDeg, MEAS_COLOR)}
        <circle cx={cx} cy={cy} r={5} fill={theme.grid} stroke={theme.cardBg} strokeWidth={2} />
      </svg>
      {/* 선 키 — 어느 바늘이 무엇인지 색+라벨+값으로 명시 / legend: swatch + label + value */}
      <div style={{ display: 'flex', gap: 18 }}>
        <LegendItem color={CMD_COLOR} label="지령" value={`${fmtSigned(targetDeg, 1)}°`} />
        <LegendItem
          color={MEAS_COLOR}
          label={measValid === false ? '실측 (무효)' : '실측'}
          value={measValid === false ? '—' : `${fmtSigned(measDeg, 1)}°`}
        />
      </div>
    </div>
  )
}

function LegendItem({ color, label, value }: { color: string; label: string; value: string }) {
  return (
    <div style={{ display: 'flex', alignItems: 'center', gap: 6 }}>
      <span style={{ width: 16, height: 3, background: color, borderRadius: 2, flex: 'none' }} />
      <Paragraph typography="s" color={theme.textSub}>{label}</Paragraph>
      <Paragraph typography="lg" fontWeight="bold" color={theme.text}>{value}</Paragraph>
    </div>
  )
}
