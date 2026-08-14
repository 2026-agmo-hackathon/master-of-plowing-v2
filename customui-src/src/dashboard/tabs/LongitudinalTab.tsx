// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 종방향 탭 — 속도 루프 튜닝과 구동계 배선 확인. 목표↔실측 속도를 같은 단위
// (m/s)로 나란히 두고, 가속페달 2채널(accV1/accV2)을 쌍으로 보여 채널 간 스케일
// 불일치(배선·캘리브레이션 문제)를 바로 잡아낸다.
//
// Longitudinal tab — speed-loop tuning and drivetrain wiring checks. Target and
// measured speed share one unit (m/s) side by side; the two accelerator
// channels are paired so a scale mismatch between them (wiring/calibration)
// shows up immediately.
// └────────────────────────────────────────────────────────────────────────────┘
import type { PpTelemetry } from '../types'
import type { Histories } from '../useTelemetry'
import { theme, magnitudeColor } from '../theme'
import { fmt, fmtSigned } from '../format'
import { fnrLabel, shiftLabel } from '../types'
import { Card, StatTile, KVRow } from '../components/primitives'
import { Hero } from '../components/Hero'
import { Sparkline } from '../components/Sparkline'

const CMD = theme.warn
const MEAS = theme.text

export function LongitudinalTab({ f, h }: { f: PpTelemetry; h: Histories }) {
  const errColor = magnitudeColor(f.speedErr, 0.15, 0.4)
  // 목표 속도는 longTargetMps 우선, 없으면 km/h 환산 — 이력 계열(targetMps)과
  // 같은 규칙. / Target speed: longTargetMps first, else converted km/h — same
  // rule as the derived history series.
  const targetMps =
    f.longTargetMps ??
    (f.targetSpeedKmh !== undefined ? f.targetSpeedKmh / 3.6 : undefined)
  const reversing =
    f.longReversing === undefined ? undefined : Boolean(f.longReversing)

  return (
    <>
      <Hero
        label="속도 오차 · Speed Error"
        keyName="speedErr"
        value={fmtSigned(f.speedErr, 2)}
        unit="m/s"
        color={errColor}
        series={h.speedErr}
      />

      <Card title="속도 목표 ↔ 실측" span={2}>
        <KVRow
          label="목표"
          keyName="longTargetMps"
          value={fmt(targetMps, 2)}
          unit="m/s"
          trailing={<Sparkline values={h.targetMps} width={150} height={40} accent={CMD} />}
        />
        <KVRow
          label="실측"
          keyName="speedMps"
          value={fmt(f.speedMps, 2)}
          unit="m/s"
          trailing={<Sparkline values={h.speedMps} width={150} height={40} accent={MEAS} />}
        />
        <KVRow
          label="목표 (원지령)"
          keyName="targetSpeedKmh"
          value={fmt(f.targetSpeedKmh, 1)}
          unit="km/h"
        />
      </Card>
      <Card title="구동계 실제" span={2}>
        <KVRow
          label="전후진 (FNR)"
          keyName="fnrState"
          value={fnrLabel(f.fnrState)}
          color={f.fnrState === undefined || f.fnrState === 0 ? theme.textDim : theme.text}
        />
        <KVRow
          label="변속"
          keyName="shiftState"
          value={shiftLabel(f.shiftState)}
          color={f.shiftState === undefined || f.shiftState === 0 ? theme.textDim : theme.text}
        />
        <KVRow label="히치" keyName="hitchPct" value={fmt(f.hitchPct, 0)} unit="%" />
      </Card>

      <StatTile label="P 항" value={fmt(f.longP, 3)} sub="longP" />
      <StatTile label="I 항" value={fmt(f.longI, 3)} sub="longI" />
      <StatTile label="D 항" value={fmt(f.longD, 3)} sub="longD" />
      <StatTile
        label="후진 대기"
        value={reversing === undefined ? '—' : reversing ? '예' : '아니오'}
        color={reversing ? theme.warn : theme.text}
        sub="longReversing"
      />

      <Card title="가속페달 2채널" span={2}>
        <KVRow
          label="채널 1"
          keyName="accV1"
          value={fmt(f.accV1, 2)}
          unit="V"
          trailing={<Sparkline values={h.accV1} width={150} height={40} accent={MEAS} />}
        />
        <KVRow
          label="채널 2"
          keyName="accV2"
          value={fmt(f.accV2, 2)}
          unit="V"
          trailing={<Sparkline values={h.accV2} width={150} height={40} accent={MEAS} />}
        />
      </Card>
      <Card title="관성 (IMU)" span={2}>
        <KVRow
          label="가속 X"
          keyName="accelX"
          value={fmtSigned(f.accelX, 2)}
          unit="m/s²"
          trailing={<Sparkline values={h.accelX} width={150} height={40} accent={MEAS} />}
        />
        <KVRow
          label="가속 Y"
          keyName="accelY"
          value={fmtSigned(f.accelY, 2)}
          unit="m/s²"
          trailing={<Sparkline values={h.accelY} width={150} height={40} accent={MEAS} />}
        />
        <KVRow
          label={f.yawRateValid === false ? '요레이트 (무효)' : '요레이트'}
          keyName="yawRateDegS"
          value={fmtSigned(f.yawRateDegS, 1)}
          unit="°/s"
          color={f.yawRateValid === false ? theme.textDim : theme.text}
        />
      </Card>
    </>
  )
}
