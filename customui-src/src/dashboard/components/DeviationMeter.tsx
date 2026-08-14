// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 부호 있는 편차 미터 — 중앙이 0, 좌우로 자란다. 색은 부호가 아니라 「크기」에
// 붙인다: 왼쪽 이탈과 오른쪽 이탈은 똑같이 나쁘므로 한쪽만 빨갛게 칠하면
// 거짓말이다. 중앙 기준선은 중립 회색(발산 표현의 중간값에 색 금지).
// SVG 대신 div 로 그려 카드 폭에 자연스럽게 늘어난다.
//
// Signed deviation meter: zero at center, growing sideways. Color encodes
// magnitude, never sign — left and right departure are equally bad, so
// coloring one side red would lie. The center baseline stays neutral grey.
// Built with divs (not SVG) so it stretches with the card width.
// └────────────────────────────────────────────────────────────────────────────┘
import { Paragraph } from '@seamos/ads'
import { theme, magnitudeColor } from '../theme'
import { fmtSigned } from '../format'

export function DeviationMeter({
  value,
  range,
  warnAt,
  badAt,
  unit,
}: {
  value: number | undefined
  /** 풀스케일 ±range / full scale ±range */
  range: number
  warnAt: number
  badAt: number
  unit: string
}) {
  const has = value !== undefined && Number.isFinite(value)
  const clamped = has ? Math.max(-range, Math.min(range, value)) : 0
  const halfPct = (Math.abs(clamped) / range) * 50
  const color = magnitudeColor(value, warnAt, badAt)

  return (
    <div style={{ width: '100%' }}>
      <div
        style={{
          position: 'relative',
          height: 14,
          borderRadius: 7,
          background: theme.border,
          overflow: 'hidden',
        }}
      >
        {has && (
          <div
            style={{
              position: 'absolute',
              top: 0,
              bottom: 0,
              // 0 에서 값 방향으로만 채운다 / fill only from zero toward the value
              left: clamped < 0 ? `${50 - halfPct}%` : '50%',
              width: `${halfPct}%`,
              background: color,
              borderRadius: 7,
            }}
          />
        )}
        {/* 중앙 0 기준선 — 중립 회색 / neutral center baseline */}
        <div
          style={{
            position: 'absolute',
            top: 0,
            bottom: 0,
            left: 'calc(50% - 1px)',
            width: 2,
            background: theme.grid,
          }}
        />
      </div>
      <div style={{ display: 'flex', justifyContent: 'space-between', marginTop: 4 }}>
        <Paragraph typography="xs" color={theme.textDim}>{fmtSigned(-range, 1)} {unit}</Paragraph>
        <Paragraph typography="xs" color={theme.textDim}>0</Paragraph>
        <Paragraph typography="xs" color={theme.textDim}>{fmtSigned(range, 1)} {unit}</Paragraph>
      </div>
    </div>
  )
}
