// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 화면당 정확히 하나뿐인 이끄는 수치. 60px(5xl) — 진동 위에서도 곁눈으로 읽힌다.
// tabular-nums 를 쓰지 않는다: 60px 독립 수치에 등폭을 걸면 "121" 이 헐렁해진다.
// 추세는 오른쪽 스파크라인, 가운데 슬롯(children)에는 편차 미터 등이 들어간다.
//
// The single leading figure per screen, 60px — readable at a glance on a
// vibrating machine. No tabular-nums: monospaced digits look loose at 60px on
// a standalone number. Trend goes to the right sparkline; the middle slot
// hosts e.g. the deviation meter.
// └────────────────────────────────────────────────────────────────────────────┘
import type { ReactNode } from 'react'
import { Paragraph } from '@seamos/ads'
import { Card } from './primitives'
import { Sparkline } from './Sparkline'
import { theme } from '../theme'

export function Hero({
  label,
  keyName,
  value,
  unit,
  color = theme.text,
  series,
  children,
}: {
  label: string
  /** 백엔드 JSON 키 병기 / backend key shown verbatim */
  keyName?: string
  value: string
  unit?: string
  color?: string
  series?: number[]
  children?: ReactNode
}) {
  return (
    <Card span={4}>
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          gap: 28,
          flexWrap: 'wrap',
        }}
      >
        <div style={{ minWidth: 250 }}>
          <Paragraph typography="m" color={theme.textSub}>{label}</Paragraph>
          {keyName !== undefined && (
            <Paragraph typography="2xs" color={theme.textDim}>{keyName}</Paragraph>
          )}
          <div style={{ display: 'flex', alignItems: 'baseline', gap: 10, marginTop: 4 }}>
            <Paragraph typography="5xl" fontWeight="bold" color={color}>
              {value}
            </Paragraph>
            {unit !== undefined && (
              <Paragraph typography="xl" color={theme.textDim}>{unit}</Paragraph>
            )}
          </div>
        </div>
        {children !== undefined && (
          <div style={{ flex: 1, minWidth: 240 }}>{children}</div>
        )}
        {series !== undefined && (
          <div style={{ display: 'flex', flexDirection: 'column', gap: 2 }}>
            <Paragraph typography="2xs" color={theme.textDim}>최근 5초 추세</Paragraph>
            <Sparkline values={series} width={340} height={80} accent={color} />
          </div>
        )}
      </div>
    </Card>
  )
}
