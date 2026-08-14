// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 관제 대시보드 루트. 모니터링 4탭은 수신 전용이고, 튜닝 탭만 튜닝 제어 채널
// (get_tuning_state / set_tracker / set_params)으로 송신한다 — 화면당 목적
// 하나: 모니터링과 설정을 섞지 않는다.
//
// - 링크 배너: 「연결 끊김/프레임 대기/프레임 정지」는 화면의 다른 모든 값의
//   의미를 바꾸는 상태라서 20px + 컬러 바로 크게 노출한다.
// - 상시 상태 바: 제어권·GPS·기어·LTD·주기는 어느 탭에서도 보여야 한다.
// - 탭 바는 ADS Tabs 대신 Button 조합: Tabs 는 내용 크기로만 렌더돼 전체 너비가
//   불가하고, outline variant 는 다크 배경에서 --button-background-color 를
//   무시하므로 활성/비활성 모두 solid 로 두고 비활성만 CSS 변수로 칠한다.
// - 고장 모달: 차단성 고장(bit3 제외)은 토스트가 아니라 차단성 Popup.
//   고장이 사라지면 확인 상태를 되돌려 재발 시 다시 뜬다.
//
// Ops dashboard root. The four monitoring tabs are receive-only; only the
// tuning tab sends, over the tuning control channel — one purpose per screen,
// monitoring and settings never mix. The link banner is
// large because it changes the meaning of every other value. The persistent
// status bar shows authority/GPS/gear/LTD/period on every tab. The tab bar is
// composed from Buttons (ADS Tabs cannot fill width; outline ignores the
// background CSS var on dark). Blocking faults (bit3 excluded) raise a modal
// Popup, re-armed when the fault clears.
// └────────────────────────────────────────────────────────────────────────────┘
import { useEffect, useState, type CSSProperties, type MouseEvent } from 'react'
import { Button, Paragraph, Popup } from '@seamos/ads'
import { useTelemetry, ORCHESTRATION_POLL_MS, SIMULATOR_TICK_MS, type LinkState } from './useTelemetry'
import type { OrchestrationState, PpTelemetry } from './types'
import {
  BLOCKING_FAULT_MASK,
  MOTOR_FAULT_NAMES,
  fnrLabel,
  gpsFixLabel,
  shiftLabel,
} from './types'
import { theme, magnitudeColor } from './theme'
import { fmt, fmtBool, fmtSigned } from './format'
import { StatusChip } from './components/primitives'
import { OverviewTab } from './tabs/OverviewTab'
import { SteerTab } from './tabs/SteerTab'
import { LongitudinalTab } from './tabs/LongitudinalTab'
import { SensorTab } from './tabs/SensorTab'
import { TuningTab } from './tabs/TuningTab'
import { gpsFixColor } from './tabs/SensorTab'
import './dashboard.css'

const TABS = ['관제', '조향', '종방향', '센서', '튜닝'] as const

const EMPTY_FRAME: PpTelemetry = { kind: 'pp_telemetry' }

export default function Dashboard() {
  const {
    frame,
    link,
    frames,
    histories,
    trackComplete,
    dismissTrackComplete,
    tuning,
    orchestration,
    leaderboard,
    retryLeaderboard,
    submitLeaderboard,
    resetRun,
    purgeAllUnsubmitted,
    unsubmittedRecords,
    requestUnsubmittedRecords,
    requestError,
    dismissRequestError,
    recordingDownload,
    send,
  } = useTelemetry()
  const [tab, setTab] = useState(0)
  // 확인한 차단성 고장 조합 — 조합이 바뀌면(새 고장) 모달이 다시 뜬다.
  // Acknowledged blocking-fault set; a new combination re-raises the modal.
  const [ackedFaults, setAckedFaults] = useState(0)

  const f = frame ?? EMPTY_FRAME
  const blockingFaults = (f.motorFaultBits ?? 0) & BLOCKING_FAULT_MASK

  // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
  // 주행 상태는 텔레메트리(10Hz)를 우선합니다. tuning_state 는 지시가 있을 때만
  // 오므로 그것만 보면 완주 같은 자발적 전환이 늦게 반영됩니다. 텔레메트리가
  // 아직 없을 때만 tuning_state 로 대체합니다.
  // └────────────────────────────────────────────────────────────────────────┘
  // ┌─ English ──────────────────────────────────────────────────────────────┐
  // The run state comes from telemetry (10 Hz) first: tuning_state only arrives on
  // request, so relying on it would lag a spontaneous transition such as
  // completion. It is the fallback only until telemetry exists.
  // └────────────────────────────────────────────────────────────────────────┘
  const runState = f.runState ?? tuning?.runState

  useEffect(() => {
    // 고장이 사라지면 확인 상태를 되돌려 재발 시 다시 차단한다.
    // Re-arm the modal once the fault clears.
    if (blockingFaults === 0) setAckedFaults(0)
  }, [blockingFaults])

  const faultOpen = blockingFaults !== 0 && blockingFaults !== ackedFaults
  const faultNames = MOTOR_FAULT_NAMES.filter(
    (_, bit) => (blockingFaults & (1 << bit)) !== 0,
  )

  // 튜닝 탭은 텔레메트리(f/h)가 아니라 튜닝 상태·송신 채널을 받으므로 별도
  // 분기. / The tuning tab takes tuning state + send, not telemetry, so it
  // branches separately.
  const MonitorView = [OverviewTab, SteerTab, LongitudinalTab, SensorTab][tab]

  return (
    <div
      style={{
        minHeight: '100vh',
        background: theme.pageBg,
        // 링크 상태에 따라 값 신뢰도가 다르다 — 배너가 항상 콘텐츠 위에 오도록
        // 단일 스크롤 문서로 둔다. / single scrolling document keeps the banner
        // above the content it qualifies.
      }}
    >
      <div
        style={{
          maxWidth: 1280,
          margin: '0 auto',
          padding: 'clamp(12px, 2vw, 24px)',
          display: 'flex',
          flexDirection: 'column',
          gap: 14,
        }}
      >
        <header style={{ display: 'flex', alignItems: 'baseline', justifyContent: 'space-between' }}>
          {/* 접미사를 고정 "관제" 로 두면 탭 이름 중 하나와 충돌해, 조향 탭에서도
              헤더가 "관제" 라고 적혀 어느 화면인지 헷갈립니다. 활성 탭을 따라가게
              합니다.
              A fixed "관제" suffix collided with one of the tab names, so the header
              still read "관제" while the 조향 tab was open. It now follows the active
              tab instead. */}
          <Paragraph typography="xl" fontWeight="bold" color={theme.text} as="h1">
            {`Master of Plow · ${TABS[tab]}`}
          </Paragraph>
          {/* frames 카운터는 스크린샷 하네스의 수신 검증에도 쓰인다 / also read by the shot harness */}
          <Paragraph typography="2xs" color={theme.textDim}>
            frames {frames}
          </Paragraph>
        </header>

        <LinkBanner link={link} />
        <RunSetup
          orchestration={orchestration}
          leaderboard={leaderboard}
          link={link}
          runState={runState}
          recordingDownload={recordingDownload}
          send={send}
          retryLeaderboard={retryLeaderboard}
          submitLeaderboard={submitLeaderboard}
          resetRun={resetRun}
          purgeAllUnsubmitted={purgeAllUnsubmitted}
          unsubmittedRecords={unsubmittedRecords}
          requestUnsubmittedRecords={requestUnsubmittedRecords}
          requestError={requestError}
          dismissRequestError={dismissRequestError}
        />
        <StatusBar f={f} />

        <nav style={{ display: 'flex', gap: 8, overflowX: 'auto' }}>
          {TABS.map((label, i) => (
            <Button
              key={label}
              variant="solid"
              tone="brand"
              size="lg"
              fullWidth
              onClick={() => setTab(i)}
              // 설치된 ads@1.0.0 번들 CSS 에는 --button-* 변수 후킹이 없어
              // (변형·톤별 토큰 직참조) 비활성만 인라인으로 토큰을 덮어쓴다.
              // ads@1.0.0's bundled CSS has no --button-* hooks, so inactive
              // tabs override inline — still ADS tokens only.
              style={
                i === tab
                  ? undefined
                  : ({
                      backgroundColor: theme.cardBg,
                      color: theme.textSub,
                      borderColor: theme.border,
                    } as CSSProperties)
              }
            >
              {label}
            </Button>
          ))}
        </nav>

        <main
          style={{
            display: 'grid',
            gridTemplateColumns: 'repeat(auto-fit, minmax(min(100%, 250px), 1fr))',
            gap: 14,
          }}
        >
          {tab === 4 ? (
            <TuningTab tuning={tuning} send={send} />
          ) : (
            <MonitorView f={f} h={histories} />
          )}
        </main>
      </div>

      {/* 차단성 고장 — 명시적 확인 필수, dimmer 클릭으로 닫히지 않음.
          Blocking fault: explicit acknowledgement, no dimmer dismiss. */}
      <Popup
        open={faultOpen}
        onClose={() => setAckedFaults(blockingFaults)}
        closeOnDimmerClick={false}
        title="조향 모터 고장"
        content={
          <Popup.Description>
            {`차단성 고장 비트가 활성입니다:\n${faultNames.join('\n')}\n\n모터 지령이 무시될 수 있습니다. 배선과 전원을 점검하세요.`}
          </Popup.Description>
        }
        buttonRight={
          <Button
            variant="solid"
            tone="critical"
            size="lg"
            fullWidth
            onClick={() => setAckedFaults(blockingFaults)}
          >
            확인
          </Button>
        }
      />

      <Popup
        open={trackComplete !== null}
        onClose={dismissTrackComplete}
        title="경로 완주"
        content={
          <Popup.Description>
            {`트랙 주행을 완료했습니다.\n경과 시간: ${fmt(trackComplete?.elapsedS, 1)} s`}
          </Popup.Description>
        }
        buttonRight={
          <Button variant="solid" tone="brand" size="lg" fullWidth onClick={dismissTrackComplete}>
            확인
          </Button>
        }
      />
    </div>
  )
}

type RunSetupProps = {
  orchestration: ReturnType<typeof useTelemetry>['orchestration']
  leaderboard?: ReturnType<typeof useTelemetry>['leaderboard']
  recordingDownload: ReturnType<typeof useTelemetry>['recordingDownload']
  link: LinkState
  runState: PpTelemetry['runState']
  send: (payload: Record<string, unknown>) => boolean
  retryLeaderboard?: (runId:string) => void
  submitLeaderboard?: (runId:string) => void
  resetRun?: (runId:string,discard:boolean) => void
  purgeAllUnsubmitted?:()=>void
  unsubmittedRecords?: ReturnType<typeof useTelemetry>['unsubmittedRecords']
  requestUnsubmittedRecords?:()=>void
  requestError?: string | null
  dismissRequestError?: () => void
}

export function RunSetup({ orchestration: o, leaderboard, recordingDownload, link, runState, send, retryLeaderboard,submitLeaderboard,resetRun,purgeAllUnsubmitted,unsubmittedRecords=[],requestUnsubmittedRecords,requestError=null,dismissRequestError }: RunSetupProps) {
  const [recordsOpen,setRecordsOpen]=useState(false)
  const [teamName,setTeamName]=useState(()=>typeof localStorage==='undefined'
    ? o?.teamName ?? '' : localStorage.getItem('master_of_plow_teamName') ?? o?.teamName ?? '')
  const [mapId,setMapId]=useState(()=>o?.confirmedSetup?.mapId || o?.selection.mapId || '')
  const [tractorId,setTractorId]=useState(()=>o?.confirmedSetup?.tractorId || o?.selection.tractorId || '')
  const [implementId,setImplementId]=useState(()=>o?.confirmedSetup?.implementId || o?.selection.implementId || '')
  // 권위값을 먼저 스칼라로 뽑아 의존성에 넣는다. 스냅샷 객체는 폴링마다 새
  // 참조가 되므로 [o] 로 두면 선택이 확정되기도 전에 계속 되감긴다.
  // Derive the authoritative scalars first: the snapshot is a fresh reference on
  // every poll, so depending on [o] rewinds the user's selection mid-edit.
  const authoritative=o?.setupApplying ? o.desiredSetup : o?.confirmedSetup
  const authMapId=authoritative?.mapId || o?.selection.mapId || ''
  const authTractorId=authoritative?.tractorId || o?.selection.tractorId || ''
  const authImplementId=authoritative?.implementId || o?.selection.implementId || ''
  const authTeamName=o?.teamName ?? ''
  const hasOrchestration=Boolean(o)
  useEffect(() => {
    if (!hasOrchestration) return
    setTeamName((v)=>v || authTeamName)
    setMapId(authMapId)
    setTractorId(authTractorId)
    setImplementId(authImplementId)
  },[hasOrchestration,authTeamName,authMapId,authTractorId,authImplementId])
  const effectiveAge=effectiveSelectionAgeMs(o?.selection,performance.now())
  const fresh=Boolean(o?.selection.live && effectiveAge >= 0
    && effectiveAge <= SELECTION_STALE_BUDGET_MS)
  const physicallyStopped=Boolean(o?.selection.stopped && !o?.selection.reactRunning)
  const teamEditable=link === 'live' && fresh
    && physicallyStopped && runState !== 'running'
    && (!o?.busy || Boolean(o?.setupApplying))
  const editable=link === 'live' && fresh && physicallyStopped
    && !o?.busy && !o?.setupApplying && runState !== 'running'
  const complete=Boolean(teamName.trim() && mapId && tractorId && implementId)
  const leaderboardCaptureBlocking=isCurrentRunCaptureBlocking(o,leaderboard)
  const postRun=Boolean(o?.recordingId && o.finalization==='completed')
  const finishRetry=o?.finalization==='retryable_error'
  const sendSetup=(next:{teamName:string;mapId:string;tractorId:string;implementId:string}) => {
    if(next.mapId && next.tractorId && next.implementId)
      send(buildSetupIntent(next))
  }
  const changeTeam=(value:string) => {
    setTeamName(value)
    persistTeamName(value)
  }
  const changeSelection=(field:'mapId'|'tractorId'|'implementId',value:string) => {
    const next={teamName,mapId,tractorId,implementId,[field]:value}
    sendSetup(next)
  }
  const start=() => {
    if (!editable || !complete || leaderboardCaptureBlocking || postRun) return
    if (!window.confirm(`Start run for team "${teamName.trim()}"?`)) return
    dismissRequestError?.()
    send({kind:'start_run',teamName:teamName.trim(),mapId,tractorId,implementId})
  }
  const finish=() => send({kind:'finish_run'})
  const submit=()=>{if(o?.recordingId)submitLeaderboard?.(o.recordingId)}
  const reset=()=>{
    if(!o?.recordingId)return
    // 리셋이 한 번 실패하면 postRun 은 reset_retryable 이 되어 "제출됨"이라는
    // 사실을 잃는다. 그러면 이미 리더보드에 올라간 주행에도 "영구히 버립니다"
    // 경고가 뜬다. 아웃박스가 그 사실을 들고 있으므로 함께 본다.
    // A failed reset turns postRun into reset_retryable and loses the fact that
    // the run was submitted, so an already-submitted run still raised the
    // "permanently discard" warning. The outbox still knows, so ask it too.
    const durable=o.postRun==='submitted'
      ||(leaderboard?.runId===o.recordingId&&leaderboard.status==='submitted')
    if(!durable&&!window.confirm('Reset will permanently discard this recording and any pending submission. Continue?'))return
    dismissRequestError?.()
    resetRun?.(o.recordingId,!durable)
  }
  const requestDownload=(event: MouseEvent<HTMLAnchorElement>) => {
    if (!recordingDownload?.url) {
      event.preventDefault()
      if (o?.recordingId) send({kind:'get_recording',id:o.recordingId})
    }
  }
  return <section style={{background:theme.cardBg,border:`1px solid ${theme.border}`,borderRadius:8,padding:14}}>
    <Paragraph typography="lg" fontWeight="bold" color={theme.text}>Run setup</Paragraph>
    <div data-testid="run-fields" className="run-setup-fields">
      <label style={{color:theme.textSub}}>Team name<input aria-label="Team name" value={teamName} disabled={!teamEditable} onChange={(e)=>changeTeam(e.target.value)} style={controlStyle}/></label>
      <CatalogSelect label="Map" value={mapId} disabled={!editable} items={o?.maps ?? []} onChange={(v)=>changeSelection('mapId',v)}/>
      <CatalogSelect label="Tractor" value={tractorId} disabled={!editable} items={o?.tractors ?? []} onChange={(v)=>changeSelection('tractorId',v)}/>
      <CatalogSelect label="Implement" value={implementId} disabled={!editable} items={o?.implements ?? []} onChange={(v)=>changeSelection('implementId',v)} showWidth/>
    </div>
    {o?.setupApplying && <Paragraph typography="xs" color={theme.warn}>Applying simulator setup…</Paragraph>}
    <div data-testid="run-actions" className="run-setup-actions">
      <Button variant="solid" tone="brand" size="lg" fullWidth disabled={!editable || !complete || leaderboardCaptureBlocking || postRun || finishRetry} onClick={start}>Start</Button>
      <Button variant="solid" tone="critical" size="lg" fullWidth disabled={!finishRetry && runState !== 'running' && o?.phase !== 'starting' && o?.phase !== 'running'} onClick={finish}>Finish</Button>
      {postRun&&<Button variant="solid" tone="brand" size="lg" fullWidth disabled={o?.postRun==='capture_in_progress'} onClick={submit}>Submit</Button>}
      {postRun&&<Button variant="solid" tone="neutral" size="lg" fullWidth disabled={o?.postRun==='capture_in_progress'||Boolean(o?.busy)} onClick={reset}>Reset</Button>}
    </div>
    {!fresh && <Paragraph typography="xs" color={theme.warn}>Catalog or simulator status is stale. <a href="#retry" onClick={(e)=>{e.preventDefault();send({kind:'get_orchestration_state'})}}>Retry</a></Paragraph>}
    {/* 직전 주행 마무리가 실패하면 백엔드는 모든 Start 를 거절한다. 그 사실이
        스냅샷에 finalization='retryable_error' 로만 들어 있어서, 예전에는
        Finish 버튼이 조용히 활성화되는 것 말고는 아무 표시가 없었다 — 사용자는
        Start 를 누르고 아무 반응이 없는 이유를 알 방법이 없었다. 누르기 전에
        말해 준다.
        A failed finalisation makes the backend refuse every Start. That only
        showed up as finalization='retryable_error' in the snapshot, which
        silently enabled the Finish button and nothing else, so there was no way
        to learn why Start did nothing. Say it before it is pressed. */}
    {finishRetry && <div data-testid="finish-retry" role="alert">
      <Paragraph typography="s" fontWeight="bold" color={theme.warn}>
        직전 주행 마무리가 실패했습니다. Finish 를 눌러 재시도한 뒤 다시 시작하세요.
      </Paragraph>
    </div>}
    {/* 실패 사유는 이 화면의 유일한 진단 통로다 — 백엔드는 거부를 로그에 남기지
        않고(setPhase 는 publish 만 한다) WS 프레임으로만 알린다. xs 본문으로
        다른 안내문 사이에 끼워 두면 "눌렀는데 아무 일도 없다"로 읽힌다.
        The only diagnostic channel on this screen: the backend never logs a
        rejection (setPhase only publishes), it reports it in this frame alone.
        Rendered as xs body text among other notices it reads as "nothing
        happened", so both failure kinds share one prominent alert. */}
    {(o?.error || requestError) && <div className="run-setup-rejection" role="alert">
      <Paragraph typography="s" fontWeight="bold" color={theme.bad}>
        {requestError ?? o?.error}
      </Paragraph>
      {requestError && <Button variant="ghost" tone="neutral" size="sm"
        onClick={()=>dismissRequestError?.()}>닫기</Button>}
    </div>}
    {o?.recordingId && <Paragraph typography="xs" color={theme.textSub}>
      <a href={recordingDownload?.url ?? '#download'} download={o.recordingId} onClick={requestDownload}>Download recording</a>
      {recordingDownload?.error ? ` — ${recordingDownload.error}` : ''}
    </Paragraph>}
    {leaderboard && leaderboard.status!=='idle' && isPrimaryLeaderboardRelevant(o,leaderboard) && <div data-testid="leaderboard-state">
      <Paragraph typography="xs" color={leaderboard.status==='submitted'?theme.ok:leaderboard.status==='invalid'||leaderboard.status==='failed_terminal'?theme.bad:theme.warn}>
        Leaderboard: {leaderboard.status}{leaderboard.attempt?` (attempt ${leaderboard.attempt})`:''}
        {leaderboard.backlog?` — backlog ${leaderboard.backlog}`:''}
        {leaderboard.verdict?` — ${leaderboard.verdict}`:''}
        {leaderboard.score!==undefined?` — score ${leaderboard.score}`:''}
        {leaderboard.reason?` — ${leaderboard.reason}`:leaderboard.error?` — ${leaderboard.error}`:''}
      </Paragraph>
      {canRetryLeaderboard(leaderboard) && <Button variant="solid" tone="neutral" size="sm"
        onClick={()=>retryLeaderboard?.(leaderboard.runId)}>Retry leaderboard</Button>}
    </div>}
    <div className="run-setup-records">
      <Button variant="outline" tone="neutral" size="sm" onClick={()=>{setRecordsOpen(true);requestUnsubmittedRecords?.()}}>
        미제출 기록 ({unsubmittedRecords.length})
      </Button>
    </div>
    <Popup
      open={recordsOpen}
      onClose={()=>setRecordsOpen(false)}
      title="미제출 기록"
      content={
        <div className="unsubmitted-body">
          <Paragraph typography="xs" color={theme.textDim}>
            제출이 끝나지 않은 주행 기록입니다. CSV 는 개별로 내려받을 수 있고, 제거는 전체 단위로만 가능합니다.
          </Paragraph>
          {unsubmittedRecords.length===0
            ? <div className="unsubmitted-empty">
                <Paragraph typography="s" color={theme.textSub}>미제출 기록이 없습니다.</Paragraph>
              </div>
            : <ul className="unsubmitted-list">
                {unsubmittedRecords.map(r=>
                  <UnsubmittedRow key={r.runId} record={r}
                    onDownload={()=>send({kind:'get_recording',id:r.runId})}/>)}
              </ul>}
        </div>
      }
      buttonLeft={
        <Button variant="outline" tone="critical" size="lg" fullWidth
          disabled={o?.postRun==='capture_in_progress'}
          onClick={()=>{if(window.confirm('모든 미제출 기록을 제거하고 시뮬레이터 기록을 초기화합니다. 계속할까요?')){if(o?.recordingId)resetRun?.(o.recordingId,true);else purgeAllUnsubmitted?.()}}}>
          모두 제거
        </Button>
      }
      buttonRight={
        <Button variant="solid" tone="brand" size="lg" fullWidth onClick={()=>setRecordsOpen(false)}>
          닫기
        </Button>
      }
    />
  </section>
}

/** 아웃박스 상태 → 한국어 라벨과 색. 색만으로 뜻을 전달하지 않기 위해 항상 짝이다.
 *  Outbox status to label + color; never color alone. */
const OUTBOX_STATUS: Record<string,{label:string;color:string}> = {
  capture_pending: {label:'캡처 대기', color:theme.warn},
  capture_error:   {label:'캡처 실패', color:theme.bad},
  pending:         {label:'제출 대기', color:theme.warn},
  submitting:      {label:'제출 중',   color:theme.warn},
  retry_wait:      {label:'재시도 대기',color:theme.warn},
  submitted:       {label:'제출 완료', color:theme.ok},
  invalid:         {label:'무효 판정', color:theme.bad},
  failed_terminal: {label:'종료 실패', color:theme.bad},
  idle:            {label:'대기',      color:theme.textDim},
}

export function formatBytes(bytes:number):string {
  if (!Number.isFinite(bytes) || bytes < 0) return '—'
  if (bytes < 1024) return `${bytes} B`
  const units=['KiB','MiB','GiB']
  let value=bytes/1024, unit=0
  while (value >= 1024 && unit < units.length-1) { value/=1024; unit+=1 }
  return `${value.toFixed(1)} ${units[unit]}`
}

function UnsubmittedRow({record,onDownload}:{
  record: NonNullable<RunSetupProps['unsubmittedRecords']>[number]
  onDownload: () => void
}) {
  const status=record.outboxStatus ? OUTBOX_STATUS[record.outboxStatus] : undefined
  const detail=record.outboxError ?? record.error
  return (
    <li className="unsubmitted-row" style={{borderLeftColor: status?.color ?? theme.grid}}>
      <div className="unsubmitted-row-head">
        <Paragraph typography="m" fontWeight="semibold" color={theme.text}>
          {record.teamName || '—'}
        </Paragraph>
        {status && <Paragraph typography="2xs" fontWeight="bold" color={status.color}>
          {status.label}
        </Paragraph>}
      </div>
      <Paragraph typography="2xs" color={theme.textDim}>{record.runId}</Paragraph>
      <div className="unsubmitted-row-meta">
        <Paragraph typography="2xs" color={theme.textDim}>{record.createdAt || '—'}</Paragraph>
        <Paragraph typography="2xs" color={theme.textDim}>{formatBytes(record.bytes)}</Paragraph>
      </div>
      {detail && <Paragraph typography="2xs" color={theme.bad}>{detail}</Paragraph>}
      {record.csvAvailable &&
        <div className="unsubmitted-row-action">
          <Button variant="ghost" tone="neutral" size="sm" onClick={onDownload}>CSV 다운로드</Button>
        </div>}
    </li>
  )
}

export function canRetryLeaderboard(state: NonNullable<RunSetupProps['leaderboard']>): state is NonNullable<RunSetupProps['leaderboard']> & {runId:string} {
  return (state.status==='retry_wait'||state.status==='capture_error') && state.retryAllowed===true && Boolean(state.runId)
}

export function isCurrentRunCaptureBlocking(
  orchestration: RunSetupProps['orchestration'],
  leaderboard: RunSetupProps['leaderboard'],
): boolean {
  return orchestration?.finalization==='completed'
    && Boolean(orchestration.recordingId)
    && leaderboard?.runId===orchestration.recordingId
    && (leaderboard?.status==='capture_pending'||leaderboard?.status==='capture_error')
}

/** 화면이 「낡음」을 선언하기까지 허용하는 관측 나이.
 *
 *  관측 나이는 세 구간이 더해진 값이다: 시뮬레이터가 자기 상태를 들고 있던
 *  시간(0~1000ms 톱니) + 백엔드가 조회해서 발행하기까지 + 브라우저가 받은 뒤
 *  다음 폴링까지(최대 ORCHESTRATION_POLL_MS). 그래서 예산을 백엔드의
 *  MAX_SNAPSHOT_AGE_MS(1000)와 같게 두면 전송 지연 몫이 0이 되어 매 폴링 주기마다
 *  반드시 낡음으로 넘어간다 — 배너가 깜빡이고 조작이 잠긴다.
 *  예산은 폴링 주기에 묶어 둔다: 정상 폴링이 한 번 통째로 유실돼야 낡음이 뜬다.
 *
 *  이건 안전 게이트가 아니다. 안전 게이트는 백엔드에 있다 —
 *  RunOrchestrator::start() 는 카탈로그를 그 자리에서 다시 조회해
 *  physicalStopIssue() 로 1000ms 규칙을 직접 검증하므로, 화면의 캐시 나이는
 *  Start 판정에 관여하지 않는다.
 *
 *  Budget for declaring the view stale. Observed age is the sum of three spans:
 *  the simulator holding its own sample (a 0–1000 ms sawtooth), the backend
 *  fetching and publishing it, and the browser holding it until the next poll
 *  (up to ORCHESTRATION_POLL_MS). Setting the budget equal to the backend's
 *  MAX_SNAPSHOT_AGE_MS (1000) leaves zero room for the latter two, so the view
 *  necessarily goes stale once per poll period — the banner flaps and the
 *  controls lock. Tying it to the poll period means staleness reports a genuinely
 *  missed refresh instead. This is not the safety gate: RunOrchestrator::start()
 *  re-fetches the catalog and re-checks the 1000 ms rule via physicalStopIssue(),
 *  so the browser's cached age never decides whether a run may begin. */
export const SELECTION_STALE_BUDGET_MS = SIMULATOR_TICK_MS + 2*ORCHESTRATION_POLL_MS

export function effectiveSelectionAgeMs(
  selection: OrchestrationState['selection']|undefined,
  nowMonoMs: number,
): number {
  if(!selection)return -1
  const elapsed=selection.receivedAtMonoMs===undefined?0:Math.max(0,nowMonoMs-selection.receivedAtMonoMs)
  return selection.snapshotAgeMs+elapsed
}

export function isPrimaryLeaderboardRelevant(
  orchestration: RunSetupProps['orchestration'],
  leaderboard: RunSetupProps['leaderboard'],
): boolean {
  return Boolean(orchestration?.recordingId && orchestration.finalization==='completed'
    && leaderboard?.runId===orchestration.recordingId)
}

export function persistTeamName(value:string): void {
  if(typeof localStorage!=='undefined') localStorage.setItem('master_of_plow_teamName',value)
}

export function buildSetupIntent(next:{teamName:string;mapId:string;tractorId:string;implementId:string}): Record<string,string> {
  return {kind:'set_sim_setup',...next}
}

const controlStyle: CSSProperties={display:'block',width:'100%',marginTop:4,minHeight:40,boxSizing:'border-box',background:theme.pageBg,color:theme.text,border:`1px solid ${theme.border}`,borderRadius:4,padding:'6px 8px'}

function CatalogSelect({label,value,disabled,items,onChange,showWidth=false}:{label:string,value:string,disabled:boolean,items:{id:string,label:string,widthM?:number}[],onChange:(id:string)=>void,showWidth?:boolean}) {
  return <label style={{color:theme.textSub}}>{label}<select aria-label={label} value={value} disabled={disabled} onChange={(e)=>onChange(e.target.value)} style={controlStyle}>
    <option value="">Select…</option>
    {items.map((item)=><option key={item.id} value={item.id}>{item.label}{showWidth && item.widthM ? ` · ${item.widthM.toFixed(2)} m` : ''}</option>)}
  </select></label>
}

// ── 링크 배너 / link banner ───────────────────────────────────────────────────

const BANNERS: Record<
  Exclude<LinkState, 'live'>,
  { color: string; title: string; desc: string }
> = {
  connecting: {
    color: theme.warn,
    title: '프레임 대기 중',
    desc: '포트 발견 및 WebSocket 연결을 시도하고 있습니다.',
  },
  waiting: {
    color: theme.warn,
    title: '프레임 대기 중',
    desc: '연결됨 — 아직 텔레메트리 프레임이 도착하지 않았습니다.',
  },
  stalled: {
    color: theme.bad,
    title: '프레임 정지',
    desc: '링크는 열려 있으나 프레임이 멎었습니다. 아래 값은 과거 데이터입니다.',
  },
  closed: {
    color: theme.bad,
    title: '연결 끊김',
    desc: 'WebSocket 이 닫혔습니다. 자동 재접속을 시도합니다.',
  },
}

function LinkBanner({ link }: { link: LinkState }) {
  if (link === 'live') return null
  const b = BANNERS[link]
  return (
    <div
      style={{
        display: 'flex',
        alignItems: 'baseline',
        gap: 12,
        background: theme.cardBg,
        border: `1px solid ${theme.border}`,
        borderLeft: `6px solid ${b.color}`,
        borderRadius: 8,
        padding: '12px 16px',
      }}
      role="status"
    >
      <Paragraph typography="lg" fontWeight="bold" color={b.color}>
        {b.title}
      </Paragraph>
      <Paragraph typography="s" color={theme.textSub}>{b.desc}</Paragraph>
    </div>
  )
}

// ── 상시 상태 바 / persistent status bar ─────────────────────────────────────

function StatusBar({ f }: { f: PpTelemetry }) {
  const gear =
    f.fnrState === undefined && f.shiftState === undefined
      ? '—'
      : `${fnrLabel(f.fnrState)}·${shiftLabel(f.shiftState)}`
  return (
    <div
      style={{
        display: 'flex',
        gap: 'clamp(16px, 3vw, 40px)',
        background: theme.cardBg,
        border: `1px solid ${theme.border}`,
        borderRadius: 8,
        padding: '10px 16px',
        flexWrap: 'wrap',
      }}
    >
      <StatusChip
        label="제어권"
        value={fmtBool(f.controlAuthority, 'AUTO', 'MANUAL')}
        color={
          f.controlAuthority === undefined
            ? theme.textDim
            : f.controlAuthority
              ? theme.ok
              : theme.warn
        }
      />
      <StatusChip label="GPS" value={gpsFixLabel(f.gpsFix)} color={gpsFixColor(f.gpsFix)} />
      <StatusChip label="기어" value={gear} color={theme.text} />
      <StatusChip
        label="LTD · Lateral Deviation"
        value={`${fmtSigned(f.ltd, 2)} m`}
        color={magnitudeColor(f.ltd, 0.1, 0.3)}
      />
      <StatusChip
        label="주기"
        value={`${fmt(f.dtMs, 0)} ms`}
        color={magnitudeColor(f.dtMs, 150, 300)}
      />
    </div>
  )
}
