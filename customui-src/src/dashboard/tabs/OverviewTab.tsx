// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 관제 탭 — 경로 추종의 최상위 질문 "지금 경로에서 얼마나 벗어났나"에 답한다.
// Hero 는 LTD 하나뿐(화면당 하나 규칙). 계기(방위·조향각)로 상황 감각을,
// stat 4개와 자세/진행 표로 나머지 상태를 요약한다.
//
// Overview tab: answers the top question of path tracking — "how far off the
// path am I right now". The single hero is LTD; the two gauges give situational
// feel, four stats plus attitude/progress tables summarize the rest.
// └────────────────────────────────────────────────────────────────────────────┘
import { Paragraph } from '@seamos/ads'
import type { PpTelemetry } from '../types'
import type { Histories } from '../useTelemetry'
import { theme, magnitudeColor } from '../theme'
import { fmt, fmtSigned, fmtInt, radToDeg } from '../format'
import { Card, StatTile, KVRow } from '../components/primitives'
import { Hero } from '../components/Hero'
import { DeviationMeter } from '../components/DeviationMeter'
import { Sparkline } from '../components/Sparkline'
import { CompassRose, SteerArc } from '../components/gauges'

// LTD 상태 임계 (m) — 경작 폭 대비 허용 오차 감각값 / LTD thresholds in metres
const LTD_WARN = 0.1
const LTD_BAD = 0.3

export function OverviewTab({ f, h }: { f: PpTelemetry; h: Histories }) {
  const ltdColor = magnitudeColor(f.ltd, LTD_WARN, LTD_BAD)
  const hdeDeg = radToDeg(f.hde)
  const attInvalid = f.attitudeValid === false
  const attColor = (v: number | undefined) =>
    attInvalid ? theme.textDim : magnitudeColor(v, 7, 15)

  return (
    <>
      <Hero
        label="LTD · Lateral Deviation"
        keyName="ltd"
        value={fmtSigned(f.ltd, 2)}
        unit="m"
        color={ltdColor}
        series={h.ltd}
      >
        <DeviationMeter
          value={f.ltd}
          range={0.5}
          warnAt={LTD_WARN}
          badAt={LTD_BAD}
          unit="m"
        />
      </Hero>

      <Card title="방위 · Heading" span={2}>
        <div style={{ display: 'flex', justifyContent: 'center' }}>
          <CompassRose headingDeg={f.headingDeg} />
        </div>
      </Card>
      <Card title="조향각 지령 ↔ 실측" span={2}>
        <div style={{ display: 'flex', justifyContent: 'center' }}>
          <SteerArc
            targetDeg={f.steerTargetDeg}
            measDeg={f.steerMeasDeg}
            measValid={f.steerMeasValid}
          />
        </div>
      </Card>

      {/* 값과 목표를 같은 단위로 둡니다. 값은 m/s, 목표는 km/h 로 섞여 있으면
          두 수치를 눈으로 비교할 수 없습니다 — 목표속도가 km/h 로 오므로
          실측을 km/h 로 환산해 맞춥니다.
          Value and target share a unit: with m/s beside km/h the two numbers
          cannot be compared at a glance. The waypoint target arrives in km/h, so
          the measurement is converted to match. */}
      <StatTile
        label="속도"
        value={fmt(f.speedMps === undefined ? undefined : f.speedMps * 3.6, 1)}
        unit="km/h"
        sub={`목표 ${fmt(f.targetSpeedKmh, 1)} km/h`}
      />
      <StatTile
        label="HDE · Heading Error"
        value={fmtSigned(hdeDeg, 2)}
        unit="°"
        color={magnitudeColor(hdeDeg, 2, 6)}
        sub="hde (rad→°)"
      />
      <StatTile
        label="요레이트"
        value={fmtSigned(f.yawRateDegS, 1)}
        unit="°/s"
        color={f.yawRateValid === false ? theme.textDim : theme.text}
        sub={f.yawRateValid === false ? 'yawRateDegS · 무효' : 'yawRateDegS'}
      />
      <StatTile label="히치" value={fmt(f.hitchPct, 0)} unit="%" sub="hitchPct" />

      <Card title="차체 자세 (롤·피치)" span={2}>
        <KVRow
          label={attInvalid ? '롤 (무효)' : '롤'}
          keyName="rollDeg"
          value={fmtSigned(f.rollDeg, 1)}
          unit="°"
          color={attColor(f.rollDeg)}
          trailing={<Sparkline values={h.rollDeg} width={150} height={40} accent={attColor(f.rollDeg)} />}
        />
        <KVRow
          label={attInvalid ? '피치 (무효)' : '피치'}
          keyName="pitchDeg"
          value={fmtSigned(f.pitchDeg, 1)}
          unit="°"
          color={attColor(f.pitchDeg)}
          trailing={<Sparkline values={h.pitchDeg} width={150} height={40} accent={attColor(f.pitchDeg)} />}
        />
        {attInvalid && (
          <Paragraph typography="xs" color={theme.warn}>
            자세 무효 (attitudeValid=false) — 값을 신뢰하지 마세요
          </Paragraph>
        )}
      </Card>
      <Card title="경로 진행" span={2}>
        <KVRow label="목표점" keyName="targetIdx" value={fmtInt(f.targetIdx)} />
        <KVRow label="최근접점" keyName="closestIdx" value={fmtInt(f.closestIdx)} />
        <KVRow label="지령점" keyName="commandIdx" value={fmtInt(f.commandIdx)} />
      </Card>
    </>
  )
}
