// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 조향 탭 — PID 튜닝 화면. 「지령 ↔ 실제」를 쌍으로 붙여 스케일 오차와 응답
// 지연을 눈으로 잡는다: 조향각(지령/실측), 모터(지령 ±600 / 엔코더 rpm).
// 지령 계열의 끝점은 경고색(warn), 실측 계열은 흰색 — 조향각 아크의 바늘 색과
// 같은 부호화를 유지해 화면 간 감각이 끊기지 않게 한다.
//
// Steering tab — the PID tuning screen. Command↔actual pairs sit side by side
// so scale errors and lag are visible: steer angle (cmd/meas) and motor
// (cmd ±600 / encoder rpm). Command endpoints are warn-colored, measured are
// white — the same encoding as the steering-arc needles, so the mapping holds
// across screens.
// └────────────────────────────────────────────────────────────────────────────┘
import type { PpTelemetry } from '../types'
import type { Histories } from '../useTelemetry'
import { theme, magnitudeColor } from '../theme'
import { fmt, fmtSigned, fmtInt, fmtBool } from '../format'
import { Card, StatTile, KVRow } from '../components/primitives'
import { Hero } from '../components/Hero'
import { Sparkline } from '../components/Sparkline'

const CMD = theme.warn
const MEAS = theme.text

export function SteerTab({ f, h }: { f: PpTelemetry; h: Histories }) {
  const errColor = magnitudeColor(f.steerErrDeg, 2, 5)
  const measInvalid = f.steerMeasValid === false

  return (
    <>
      <Hero
        label="조향 오차 · Steer Error"
        keyName="steerErrDeg"
        value={fmtSigned(f.steerErrDeg, 2)}
        unit="°"
        color={errColor}
        series={h.steerErrDeg}
      />

      <Card title="조향각 지령 ↔ 실측" span={2}>
        <KVRow
          label="지령"
          keyName="steerTargetDeg"
          value={fmtSigned(f.steerTargetDeg, 1)}
          unit="°"
          trailing={<Sparkline values={h.steerTargetDeg} width={150} height={40} accent={CMD} />}
        />
        <KVRow
          label={measInvalid ? '실측 (무효)' : '실측'}
          keyName="steerMeasDeg"
          value={measInvalid ? '—' : fmtSigned(f.steerMeasDeg, 1)}
          unit="°"
          color={measInvalid ? theme.textDim : theme.text}
          trailing={<Sparkline values={h.steerMeasDeg} width={150} height={40} accent={MEAS} />}
        />
      </Card>
      <Card title="모터 지령 ↔ 엔코더" span={2}>
        <KVRow
          label="지령 (±600)"
          keyName="steerCmd"
          value={fmtInt(f.steerCmd)}
          trailing={<Sparkline values={h.steerCmd} width={150} height={40} accent={CMD} />}
        />
        <KVRow
          label="엔코더 회전"
          keyName="encRpm"
          value={fmtInt(f.encRpm)}
          unit="rpm"
          trailing={<Sparkline values={h.encRpm} width={150} height={40} accent={MEAS} />}
        />
      </Card>

      <StatTile label="P 항" value={fmt(f.steerP, 2)} sub="steerP" />
      <StatTile label="I 항" value={fmt(f.steerI, 2)} sub="steerI" />
      <StatTile label="D 항" value={fmt(f.steerD, 2)} sub="steerD" />
      <StatTile label="엔코더 위치" value={fmt(f.encDeg, 1)} unit="°" sub="encDeg" />

      <Card title="폐루프 상태" span={2}>
        <KVRow
          label="제어권"
          keyName="controlAuthority"
          value={fmtBool(f.controlAuthority, 'AUTO', 'MANUAL')}
          color={
            f.controlAuthority === undefined
              ? theme.textDim
              : f.controlAuthority
                ? theme.ok
                : theme.warn
          }
        />
        <KVRow
          label="실측 유효"
          keyName="steerMeasValid"
          value={fmtBool(f.steerMeasValid, '유효', '무효')}
          color={
            f.steerMeasValid === undefined
              ? theme.textDim
              : f.steerMeasValid
                ? theme.ok
                : theme.bad
          }
        />
        <KVRow
          label="조향 차단"
          keyName="steerBlocked"
          value={fmtBool(f.steerBlocked, '차단', '정상')}
          color={
            f.steerBlocked === undefined
              ? theme.textDim
              : f.steerBlocked
                ? theme.bad
                : theme.ok
          }
        />
      </Card>
      <Card title="트래커 조향 항" span={2}>
        <KVRow label="곡률 앞먹임" keyName="cfTerm" value={fmtSigned(f.cfTerm, 4)} />
        <KVRow label="HDE 보정" keyName="hdeTerm" value={fmtSigned(f.hdeTerm, 4)} />
        <KVRow label="LTD 보정" keyName="ltdTerm" value={fmtSigned(f.ltdTerm, 4)} />
        <KVRow label="순수 추종" keyName="ppTerm" value={fmtSigned(f.ppTerm, 4)} />
        <KVRow label="요 감쇠" keyName="yawDampTerm" value={fmtSigned(f.yawDampTerm, 4)} />
      </Card>
    </>
  )
}
