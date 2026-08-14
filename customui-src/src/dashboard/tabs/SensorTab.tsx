// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 센서 탭 — 측위 품질과 조향 모터 건강 상태. Hero 는 GPS Fix: RTK 가 풀리면
// 다른 모든 지표의 신뢰도가 무너지므로 이 화면의 이끄는 값이다.
// 고장 비트는 10개 전부 이름과 함께 나열한다 — 비트마스크 정수를 머리로
// 풀게 하지 않는다. bit3(Disabled)은 정상 동작 중에도 켜지므로 위험색을 쓰지
// 않고 라벨로만 구분한다.
//
// Sensor tab — positioning quality and steer-motor health. The hero is GPS Fix:
// when RTK drops, every other number loses trust. All 10 fault bits are listed
// by name — nobody should decode a bitmask in their head. bit3 (Disabled) also
// turns on in normal operation, so it never gets the critical color.
// └────────────────────────────────────────────────────────────────────────────┘
import { Paragraph } from '@seamos/ads'
import type { PpTelemetry } from '../types'
import type { Histories } from '../useTelemetry'
import { gpsFixLabel, MOTOR_FAULT_NAMES } from '../types'
import { theme, magnitudeColor } from '../theme'
import { fmt, fmtInt } from '../format'
import { Card, StatTile, KVRow } from '../components/primitives'
import { Hero } from '../components/Hero'
import { Sparkline } from '../components/Sparkline'

export function gpsFixColor(fix: number | undefined): string {
  if (fix === undefined) return theme.textDim
  if (fix === 4) return theme.ok // RTK Fixed
  if (fix === 5 || fix === 2) return theme.warn // RTK Float / DGPS
  return theme.bad // Single / No Fix
}

export function SensorTab({ f, h }: { f: PpTelemetry; h: Histories }) {
  const faults = f.motorFaultBits

  return (
    <>
      <Hero
        label="GPS Fix · NMEA GGA"
        keyName="gpsFix"
        value={gpsFixLabel(f.gpsFix)}
        unit={f.gpsFix !== undefined ? `(${f.gpsFix})` : undefined}
        color={gpsFixColor(f.gpsFix)}
      >
        <Paragraph typography="xs" color={theme.textDim}>
          4=RTK Fixed · 5=RTK Float · 2=DGPS · 1=Single · 0=No Fix
        </Paragraph>
      </Hero>

      <StatTile label="위성 수" value={fmtInt(f.gpsSats)} sub="gpsSats" />
      <StatTile
        label="HDOP"
        value={fmt(f.gpsHdop, 2)}
        color={magnitudeColor(f.gpsHdop, 1.5, 3)}
        sub="gpsHdop"
      />
      <StatTile
        label="제어 주기"
        value={fmt(f.dtMs, 1)}
        unit="ms"
        color={magnitudeColor(f.dtMs, 150, 300)}
        sub="dtMs"
      />
      <StatTile label="경과 시간" value={fmt(f.elapsedS, 1)} unit="s" sub="elapsedS" />

      <Card title="조향 모터" span={2}>
        <KVRow
          label="엔코더 회전"
          keyName="encRpm"
          value={fmtInt(f.encRpm)}
          unit="rpm"
          trailing={<Sparkline values={h.encRpm} width={150} height={40} accent={theme.text} />}
        />
        <KVRow label="엔코더 위치" keyName="encDeg" value={fmt(f.encDeg, 1)} unit="°" />
        <Paragraph typography="s" fontWeight="semibold" color={theme.textSub}>
          고장 비트 (motorFaultBits {faults === undefined ? '—' : `0b${faults.toString(2).padStart(10, '0')}`})
        </Paragraph>
        <div
          style={{
            display: 'grid',
            gridTemplateColumns: 'repeat(2, minmax(0, 1fr))',
            gap: '6px 16px',
          }}
        >
          {MOTOR_FAULT_NAMES.map((name, bit) => {
            const active = faults !== undefined && (faults & (1 << bit)) !== 0
            const isDisabledBit = bit === 3
            const label =
              faults === undefined
                ? '—'
                : active
                  ? isDisabledBit
                    ? '켜짐 (정상 Disable)'
                    : '고장'
                  : '정상'
            const color =
              faults === undefined
                ? theme.textDim
                : active
                  ? isDisabledBit
                    ? theme.textSub
                    : theme.bad
                  : theme.textDim
            return (
              <div
                key={name}
                style={{ display: 'flex', justifyContent: 'space-between', gap: 8 }}
              >
                <Paragraph typography="xs" color={theme.textSub}>
                  b{bit} {name}
                </Paragraph>
                <Paragraph typography="xs" fontWeight="bold" color={color}>
                  {label}
                </Paragraph>
              </div>
            )
          })}
        </div>
      </Card>
      <Card title="위치 (ENU)" span={2}>
        <KVRow label="동쪽 E" keyName="enuX" value={fmt(f.enuX, 2)} unit="m" />
        <KVRow label="북쪽 N" keyName="enuZ" value={fmt(f.enuZ, 2)} unit="m" />
        <KVRow label="경로계 X" keyName="xLocal" value={fmt(f.xLocal, 2)} unit="m" />
        <KVRow label="경로계 Z" keyName="zLocal" value={fmt(f.zLocal, 2)} unit="m" />
        <KVRow label="전방 주시 거리" keyName="lookaheadDist" value={fmt(f.lookaheadDist, 2)} unit="m" />
      </Card>
    </>
  )
}
