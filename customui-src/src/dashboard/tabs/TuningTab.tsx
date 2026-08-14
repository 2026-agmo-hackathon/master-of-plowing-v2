// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 튜닝 탭 — 이 화면의 목적은 「설정」 하나뿐이다(모니터링과 섞지 않는다).
//
// - 입력은 전부 스테퍼(− / +): 진동하는 기계 위에서 자유 텍스트·미세 슬라이더는
//   금지. 버튼은 size="lg"(≥48px 터치 타깃), 값은 24px bold 로 크게.
// - 알고리즘 버튼의 활성 표시는 「백엔드가 보고한 tracker」만 따른다(낙관적
//   업데이트 금지) — 이 화면의 목적이 "실제로 적용됐는가" 확인이기 때문.
// - 파라미터 값은 로컬 draft: 스테퍼 연타가 즉시 보이되, 전송은 300ms 디바운스
//   로 모아 set_params 부분 프레임 하나로 나간다. 백엔드가 tuning_state 로
//   clamp 된 실제값을 돌려주면 (편집 중이 아닌 키만) draft 를 덮어쓴다.
// - 선택되지 않은 알고리즘 그룹은 흐리게만 하고 숨기지 않는다(비교하며 튜닝).
// - 기본값과 다른 값은 경고색 + "기본 x.x" 병기(색만으로 전달 금지).
//
// Tuning tab — one purpose: settings (never mixed with monitoring). All input
// is steppers (no free text / fine sliders on a vibrating machine; lg buttons,
// ≥48px). The active algorithm shows only what the backend reported — no
// optimistic update, because confirming actual application is the point.
// Param values live in a local draft: taps show immediately, sends are
// debounced 300ms into one partial set_params frame; the tuning_state echo
// (backend clamped truth) overwrites non-pending keys. The unselected
// algorithm group is dimmed, never hidden. Modified values get warn color plus
// a "기본 x.x" caption (color never carries meaning alone).
// └────────────────────────────────────────────────────────────────────────────┘
import { useEffect, useRef, useState, type CSSProperties } from 'react'
import { Button, Paragraph } from '@seamos/ads'
import type { TuningState } from '../types'
import { theme } from '../theme'
import { Card } from '../components/primitives'
import { PARAM_GROUPS, TRACKER_LABELS, type ParamSpec } from '../tuning'

const DEBOUNCE_MS = 300

/** 부동소수 스텝 누적 오차 제거 / kill float drift from repeated steps */
function roundStep(v: number): number {
  return Math.round(v * 10000) / 10000
}

export function TuningTab({
  tuning,
  send,
}: {
  tuning: TuningState | null
  send: (payload: Record<string, unknown>) => boolean
}) {
  // 로컬 draft — 스테퍼의 즉각 반응용. 진실은 백엔드 tuning_state.
  // Local draft for instant stepper feedback; the backend state is the truth.
  const [draft, setDraft] = useState<Record<string, number>>({})
  // 디바운스 창 안에서 편집돼 아직 전송되지 않은 키들.
  // Keys edited inside the debounce window, not yet flushed.
  const pendingRef = useRef<Record<string, number>>({})
  const timerRef = useRef<number | null>(null)

  useEffect(() => {
    const params = tuning?.params
    if (!params) return
    setDraft((cur) => {
      const next = { ...cur }
      for (const [k, v] of Object.entries(params)) {
        // 편집 중(미전송)인 키는 사용자의 입력을 지키고, 나머지는 백엔드
        // 실제값으로 동기화한다. / Keep in-flight edits; sync the rest.
        if (typeof v === 'number' && !(k in pendingRef.current)) next[k] = v
      }
      return next
    })
  }, [tuning])

  useEffect(
    () => () => {
      if (timerRef.current !== null) window.clearTimeout(timerRef.current)
    },
    [],
  )

  const scheduleFlush = () => {
    if (timerRef.current !== null) window.clearTimeout(timerRef.current)
    timerRef.current = window.setTimeout(() => {
      timerRef.current = null
      const changed = pendingRef.current
      pendingRef.current = {}
      // 바뀐 필드만 부분 전송. 실패(소켓 닫힘)하면 백엔드 echo 가 없으므로
      // draft 가 남는다 — 재접속 시 get_tuning_state 가 다시 맞춘다.
      // Partial send of changed fields only. On failure there is no echo; the
      // reconnect's get_tuning_state resynchronises.
      send({ kind: 'set_params', ...changed })
    }, DEBOUNCE_MS)
  }

  const bump = (spec: ParamSpec, dir: 1 | -1) => {
    setDraft((cur) => {
      const base = cur[spec.key]
      if (base === undefined) return cur // 백엔드 상태 수신 전 / no state yet
      // 음수 게인은 보정 부호가 뒤집혀 폭주한다 — 클라이언트에서도 0 미만을
      // 만들지 않는다(백엔드가 최종 clamp). / Never go below 0 client-side;
      // the backend still clamps authoritatively.
      const next = Math.max(0, roundStep(base + dir * spec.step))
      pendingRef.current[spec.key] = next
      scheduleFlush()
      return { ...cur, [spec.key]: next }
    })
  }

  const resetGroup = (params: ParamSpec[]) => {
    setDraft((cur) => {
      const next = { ...cur }
      for (const spec of params) {
        next[spec.key] = spec.defaultValue
        pendingRef.current[spec.key] = spec.defaultValue
      }
      scheduleFlush()
      return next
    })
  }

  const activeTracker = tuning?.tracker
  const available = tuning?.available ?? Object.keys(TRACKER_LABELS)
  // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
  // 주행 중(Running/Paused)에는 파라미터와 알고리즘을 잠급니다. 백엔드가
  // 거부하는 것이 권위이고(TuningProtocol), 여기서 비활성화하는 것은 "눌러도
  // 안 되는 버튼을 누르게 하지 않는" 배려입니다. UI 만으로 막으면 낡은 상태를
  // 든 클라이언트가 우회할 수 있으므로 둘 다 필요합니다.
  // └────────────────────────────────────────────────────────────────────────┘
  // ┌─ English ──────────────────────────────────────────────────────────────┐
  // A run in progress (Running/Paused) locks the parameters and the algorithm. The
  // backend's refusal is the authority; disabling here only spares the user from
  // pressing a button that cannot work. Both are needed — UI-only gating is
  // bypassable by a client holding stale state.
  // └────────────────────────────────────────────────────────────────────────┘
  const locked = tuning?.paramsLocked === true
  const ready = tuning !== null && !locked

  return (
    <>
      {locked && (
        <section
          style={{
            gridColumn: 'span 4',
            background: theme.cardBg,
            border: `1px solid ${theme.border}`,
            borderLeft: `8px solid ${theme.warn}`,
            borderRadius: 16,
            padding: '14px 18px',
          }}
        >
          <Paragraph typography="lg" fontWeight="semibold" color={theme.warn}>
            주행 중 — 파라미터 잠김
          </Paragraph>
          <Paragraph typography="xs" color={theme.textSub}>
            {`현재 상태: ${tuning?.runState ?? '—'} · 게인과 알고리즘을 바꾸려면 관제 탭에서 주행을 종료하세요. `}
            {'주행 도중 값을 바꾸면 거동이 갑자기 달라지고, 어떤 설정으로 달린 결과인지도 알 수 없게 됩니다.'}
          </Paragraph>
        </section>
      )}
      <Card title="경로추종 알고리즘" span={4}>
        <div style={{ display: 'flex', gap: 10 }}>
          {available.map((name) => {
            const active = name === activeTracker
            return (
              <Button
                key={name}
                variant="solid"
                tone="brand"
                size="lg"
                fullWidth
                disabled={!ready}
                onClick={() => send({ kind: 'set_tracker', name })}
                // 활성 = 백엔드 보고값만. ads@1.0.0 번들에는 --button-* 변수
                // 후킹이 없어 비활성은 인라인 토큰으로 칠한다(탭 바와 동일).
                // Active follows the backend report only; inactive painted via
                // inline tokens (no --button-* hooks in ads@1.0.0).
                style={
                  active
                    ? undefined
                    : ({
                        backgroundColor: theme.cardBg,
                        color: theme.textSub,
                        borderColor: theme.border,
                      } as CSSProperties)
                }
              >
                {`${TRACKER_LABELS[name] ?? name}${active ? ' ✓ 적용 중' : ''}`}
              </Button>
            )
          })}
        </div>
        <Paragraph typography="2xs" color={theme.textDim}>
          {`백엔드 보고값: ${activeTracker ?? '—'}`}
        </Paragraph>
        {/* 주행 중 전환 시 pathStarted 가드가 리셋돼 완주 판정이 걸리지 않는다.
            Switching mid-run resets the pathStarted guard: completion never latches. */}
        <div
          style={{
            borderLeft: `6px solid ${theme.warn}`,
            background: theme.pageBg,
            borderRadius: 8,
            padding: '10px 14px',
          }}
          role="note"
        >
          <Paragraph typography="s" fontWeight="semibold" color={theme.warn}>
            정지 상태에서 전환 권장
          </Paragraph>
          <Paragraph typography="xs" color={theme.textSub}>
            주행 중에 알고리즘을 바꾸면 새 알고리즘이 시작점(웨이포인트 0) 접근
            이력을 잃어, 경로 끝까지 가도 완주로 판정되지 않습니다.
          </Paragraph>
        </div>
      </Card>

      {PARAM_GROUPS.map((group) => {
        // 알고리즘 소속 그룹은 미선택 시 흐리게만 — 숨기면 비교 튜닝이 안 된다.
        // Dim (never hide) the group of the unselected algorithm.
        const dimmed =
          group.tracker !== null && ready && group.tracker !== activeTracker
        return (
          <Card
            key={group.title}
            title={
              group.tracker !== null
                ? `${group.title}${dimmed ? ' (미선택)' : ''}`
                : group.title
            }
            span={2}
            style={dimmed ? { opacity: 0.45 } : undefined}
          >
            {group.note !== undefined && (
              <Paragraph typography="xs" color={theme.warn}>
                {`⚠ ${group.note}`}
              </Paragraph>
            )}
            {group.params.map((spec) => (
              <StepperRow
                key={spec.key}
                spec={spec}
                value={draft[spec.key]}
                disabled={!ready}
                onBump={(dir) => bump(spec, dir)}
              />
            ))}
            <Button
              variant="solid"
              tone="neutral"
              size="lg"
              fullWidth
              disabled={!ready}
              onClick={() => resetGroup(group.params)}
              style={
                {
                  backgroundColor: theme.pageBg,
                  color: theme.textSub,
                  borderColor: theme.border,
                } as CSSProperties
              }
            >
              기본값 복귀
            </Button>
          </Card>
        )
      })}
    </>
  )
}

// ── 스테퍼 행 / stepper row ──────────────────────────────────────────────────
// 자유 텍스트 입력 금지 규칙에 따라 − / + 버튼만 제공한다. 값이 기본값과 다르면
// 경고색 + "기본 x.x" 병기로 구분한다(색 단독 전달 금지).
// Steppers only (no free text). Modified values: warn color plus a default
// caption, never color alone.
function StepperRow({
  spec,
  value,
  disabled,
  onBump,
}: {
  spec: ParamSpec
  value: number | undefined
  disabled: boolean
  onBump: (dir: 1 | -1) => void
}) {
  const modified =
    value !== undefined &&
    Math.abs(value - spec.defaultValue) > spec.step * 1e-6 + 1e-9
  return (
    <div
      style={{
        display: 'flex',
        alignItems: 'center',
        gap: 10,
        borderBottom: `1px solid ${theme.border}`,
        paddingBottom: 8,
        minHeight: 56,
      }}
    >
      <div style={{ flex: 1, minWidth: 0 }}>
        <Paragraph typography="m" color={theme.textSub}>
          {spec.label}
          {spec.unit !== undefined ? ` (${spec.unit})` : ''}
        </Paragraph>
        {/* 백엔드 키 병기 — 키는 계약 / backend key shown verbatim */}
        <Paragraph typography="2xs" color={theme.textDim}>
          {spec.key}
          {modified ? ` · 기본 ${spec.defaultValue.toFixed(spec.digits)}` : ''}
        </Paragraph>
      </div>
      <Button
        variant="solid"
        tone="neutral"
        size="lg"
        disabled={disabled || value === undefined}
        onClick={() => onBump(-1)}
        aria-label={`${spec.label} 감소`}
        style={
          {
            backgroundColor: theme.pageBg,
            color: theme.text,
            borderColor: theme.border,
            minWidth: 56,
          } as CSSProperties
        }
      >
        −
      </Button>
      <Paragraph
        typography="2xl"
        fontWeight="bold"
        color={modified ? theme.warn : theme.text}
        style={{
          fontVariantNumeric: 'tabular-nums',
          minWidth: 88,
          textAlign: 'center',
        }}
      >
        {value === undefined ? '—' : value.toFixed(spec.digits)}
      </Paragraph>
      <Button
        variant="solid"
        tone="neutral"
        size="lg"
        disabled={disabled || value === undefined}
        onClick={() => onBump(1)}
        aria-label={`${spec.label} 증가`}
        style={
          {
            backgroundColor: theme.pageBg,
            color: theme.text,
            borderColor: theme.border,
            minWidth: 56,
          } as CSSProperties
        }
      >
        +
      </Button>
    </div>
  )
}
