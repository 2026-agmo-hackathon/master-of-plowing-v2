// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 카드·stat 타일·표 행 프리미티브. ADS 에 stat/카드 컴포넌트가 없으므로 여기서
// 조합하되, 텍스트는 전부 ADS Paragraph, 색은 전부 theme 토큰만 쓴다.
//
// 타이포 크기는 진동하는 기계 위에서 읽는 전제: stat 값 30px(4xl), 표 값
// 24px(2xl) bold, 라벨 18px(m). tabular-nums 는 세로 정렬이 필요한 표 행 값에만
// 적용한다 — 큰 독립 수치에 쓰면 자간이 헐렁해진다.
//
// Card / stat tile / table-row primitives. ADS has no stat or card component,
// so we compose them here — every text node is ADS Paragraph, every color a
// theme token. Type sizes assume reading on a vibrating machine. tabular-nums
// only on table-row values that need vertical alignment.
// └────────────────────────────────────────────────────────────────────────────┘
import type { CSSProperties, ReactNode } from 'react'
import { Paragraph } from '@seamos/ads'
import { theme } from '../theme'

const TABULAR: CSSProperties = { fontVariantNumeric: 'tabular-nums' }

export function Card({
  title,
  span,
  children,
  style,
}: {
  title?: string
  span: 1 | 2 | 4
  children: ReactNode
  style?: CSSProperties
}) {
  return (
    <section
      style={{
        gridColumn: `span ${span}`,
        background: theme.cardBg,
        border: `1px solid ${theme.border}`,
        borderRadius: 12,
        padding: 16,
        display: 'flex',
        flexDirection: 'column',
        gap: 10,
        minWidth: 0,
        ...style,
      }}
    >
      {title !== undefined && (
        <Paragraph typography="m" fontWeight="semibold" color={theme.textSub}>
          {title}
        </Paragraph>
      )}
      {children}
    </section>
  )
}

/** 1열 stat 타일 — 라벨 18px, 값 30px bold / one-column stat tile */
export function StatTile({
  label,
  value,
  unit,
  color = theme.text,
  sub,
}: {
  label: string
  value: string
  unit?: string
  color?: string
  sub?: string
}) {
  return (
    <Card span={1}>
      <Paragraph typography="m" color={theme.textSub}>{label}</Paragraph>
      <div style={{ display: 'flex', alignItems: 'baseline', gap: 6 }}>
        <Paragraph typography="4xl" fontWeight="bold" color={color}>
          {value}
        </Paragraph>
        {unit !== undefined && (
          <Paragraph typography="s" color={theme.textDim}>{unit}</Paragraph>
        )}
      </div>
      {sub !== undefined && (
        <Paragraph typography="xs" color={theme.textDim}>{sub}</Paragraph>
      )}
    </Card>
  )
}

/** 표 행 — 라벨 좌, 값 우(24px bold, tabular). 우측에 스파크라인 등 부착 가능.
 *  Table row: label left, 24px bold tabular value right, optional trailing node. */
export function KVRow({
  label,
  value,
  unit,
  color = theme.text,
  keyName,
  trailing,
}: {
  label: string
  value: string
  unit?: string
  color?: string
  /** 백엔드 JSON 키 병기 — 용어는 바꿔도 키는 계약 / backend key shown verbatim */
  keyName?: string
  trailing?: ReactNode
}) {
  return (
    <div
      style={{
        display: 'flex',
        alignItems: 'center',
        gap: 12,
        borderBottom: `1px solid ${theme.border}`,
        paddingBottom: 8,
        minHeight: 40,
      }}
    >
      <div style={{ flex: 1, minWidth: 0 }}>
        <Paragraph typography="m" color={theme.textSub}>{label}</Paragraph>
        {keyName !== undefined && (
          <Paragraph typography="2xs" color={theme.textDim}>{keyName}</Paragraph>
        )}
      </div>
      {trailing}
      <div style={{ display: 'flex', alignItems: 'baseline', gap: 5 }}>
        <Paragraph typography="2xl" fontWeight="bold" color={color} style={TABULAR}>
          {value}
        </Paragraph>
        {unit !== undefined && (
          <Paragraph typography="xs" color={theme.textDim}>{unit}</Paragraph>
        )}
      </div>
    </div>
  )
}

/** 상태 칩 — 색만으로 전달 금지 규칙에 따라 항상 라벨+텍스트 동반.
 *  Status chip: color never carries meaning alone — always label + text. */
export function StatusChip({
  label,
  value,
  color,
}: {
  label: string
  value: string
  color: string
}) {
  return (
    <div style={{ display: 'flex', flexDirection: 'column', gap: 2, minWidth: 0 }}>
      <Paragraph typography="2xs" color={theme.textDim}>{label}</Paragraph>
      <Paragraph typography="lg" fontWeight="bold" color={color}>{value}</Paragraph>
    </div>
  )
}
