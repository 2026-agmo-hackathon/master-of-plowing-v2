// ┌─ 설계 의도 / Design intent ────────────────────────────────────────────────┐
// 텔레메트리 훅. 포트 발견/재접속 규칙은 @seamos/connect 에 이미 들어
// 있으므로(런타임 배정 포트, get_assigned_ports 상대 경로) 직접 구현하지 않는다.
//
// 원래 수신 전용이었으나 튜닝 제어 채널(get_tuning_state / set_tracker /
// set_params)을 위해 send() 를 노출한다. 소켓은 앱 재시작 시 조용히 닫히므로
// 전송 전 readyState === OPEN 을 반드시 확인하고, 실패를 boolean 으로 알린다.
// open 시 get_tuning_state 를 보내 현재 상태로 동기화한다(재접속 포함).
//
// 이력 버퍼는 48표본(10Hz ≈ 5초)으로 제한한다 — 더 길면 방금 바꾼 게인의 효과가
// 옛 데이터에 묻혀 튜닝 피드백 루프가 느려진다. 값이 없는 주기는 이전 값으로
// 메우지 않고 건너뛴다(계열별 독립 버퍼인 이유).
//
// 링크 상태는 별도 상태 기계로 둔다: 「연결 끊김 / 프레임 대기 / 프레임 정지」는
// 화면의 다른 모든 값의 의미를 바꾸는 정보라서 프레임 데이터와 분리해 관리한다.
//
// Telemetry hook. Port discovery / reconnect rules live inside
// @seamos/connect (runtime-assigned port, relative get_assigned_ports), so we
// do not reimplement them. Originally receive-only; send() was added for the
// tuning control channel — it checks readyState === OPEN because the socket
// closes silently on app restart, and get_tuning_state is sent on every open
// (including reconnects). History buffers hold 48 samples (≈5 s at 10 Hz) —
// longer would bury the effect of a gain change under stale data. Absent values
// are skipped, never forward-filled (hence independent per-series buffers).
// Link state is a separate machine because it changes the meaning of every
// other value on screen.
// └────────────────────────────────────────────────────────────────────────────┘
import { useCallback, useEffect, useRef, useState } from 'react'
import {
  initPorts,
  createWebSocketClient,
  closeWebSocketSafe,
  type WebSocketClientResult,
} from '@seamos/connect'
import type { LeaderboardState, OrchestrationState, PpTelemetry, TuningState,UnsubmittedRecord } from './types'
import {appendRecording,beginRecording,completeRecording,type RecordingTransfer} from './recordingTransfer'
import {BrowserLeaderboardService,IndexedDbOutboxStore} from './browserLeaderboard'

export type LinkState = 'connecting' | 'waiting' | 'live' | 'stalled' | 'closed'

export const HISTORY_LEN = 48
/** 프레임 정지 판정 — 10Hz 프레임이 15주기 이상 끊기면 정지로 본다.
 *  Stall threshold: >15 nominal periods without a frame. */
const STALL_MS = 1500

/** 오케스트레이션 폴링 주기. 이 요청 1회는 백엔드에서 시뮬레이터 HTTP 4회
 *  (maps/tractors/implements/selection)로 증폭되므로 싸지 않다. 카탈로그는
 *  주행 사이에만 바뀌므로 초 단위면 충분하다.
 *  Orchestration poll period. One request fans out to four simulator round
 *  trips in the backend, and the catalog only changes between runs. */
export const ORCHESTRATION_POLL_MS = 2500
/** 응답이 이만큼 안 오면 유실로 보고 다시 보낸다 — 그 전까지는 중복 발사 금지.
 *  Treat an unanswered poll as lost after this; never fire a second meanwhile. */
const ORCHESTRATION_POLL_TIMEOUT_MS = 10_000

/** 시뮬레이터가 자체 상태를 갱신하는 주기. `/api/simulator/selection` 의
 *  simElapsedMs 가 정확히 1000ms 씩 뛰고 snapshotAgeMs 는 그 사이에서
 *  0→1000 톱니로 움직인다. 즉 전송 지연이 0이어도 관측 나이는 최대 1초다.
 *  Simulator self-refresh period: simElapsedMs advances in exact 1000 ms steps
 *  and snapshotAgeMs sawtooths 0→1000 between them, so the observed age reaches
 *  one second even with zero transport delay. */
export const SIMULATOR_TICK_MS = 1000

/** 하트비트 주기. 앱의 발행 펌프는 클라이언트 침묵 약 15초 뒤 멎으므로
 *  넉넉한 여유를 두고 그보다 훨씬 짧게 잡는다.
 *  Heartbeat period. The app's publish pump stalls after ~15 s of client
 *  silence, so stay well inside that with room to spare. */
export const HEARTBEAT_MS = 2000

/** 이력을 쌓는 계열. 파생 계열 targetMps 는 push 시점에 계산한다.
 *  Series with history; derived targetMps is computed at push time. */
const HISTORY_KEYS = [
  'ltd', 'hde', 'speedErr',
  'steerErrDeg', 'steerTargetDeg', 'steerMeasDeg', 'steerCmd', 'encRpm',
  'speedMps', 'yawRateDegS', 'rollDeg', 'pitchDeg',
  'accV1', 'accV2', 'accelX', 'accelY', 'dtMs',
] as const satisfies readonly (keyof PpTelemetry)[]

export type HistoryKey = (typeof HISTORY_KEYS)[number] | 'targetMps'
export type Histories = Partial<Record<HistoryKey, number[]>>

export interface Telemetry {
  frame: PpTelemetry | null
  link: LinkState
  /** 수신 프레임 수 / received frame count */
  frames: number
  histories: Histories
  trackComplete: { elapsedS?: number } | null
  dismissTrackComplete: () => void
  /** 마지막으로 수신한 튜닝 상태 — 백엔드가 보고한 값이 진실.
   *  Last received tuning state; the backend's report is the truth. */
  tuning: TuningState | null
  orchestration: OrchestrationState | null
  /** 백엔드가 요청 자체를 거부했을 때의 사유. 스냅샷에는 아무 흔적이 남지
   *  않으므로 별도 채널로 보관한다. / Reason the backend refused a request
   *  outright; the snapshot records no trace of it. */
  requestError: string | null
  dismissRequestError: () => void
  leaderboard: LeaderboardState | null
  retryLeaderboard: (runId:string) => void
  submitLeaderboard: (runId:string) => void
  resetRun: (runId:string,discard:boolean) => void
  purgeAllUnsubmitted: () => void
  unsubmittedRecords: UnsubmittedRecord[]
  requestUnsubmittedRecords: () => void
  recordingDownload: { id: string; url?: string; error?: string } | null
  /** 소켓이 열려 있으면 JSON 을 보내고 true. 아니면 아무것도 안 하고 false.
   *  Sends JSON if the socket is open; returns false otherwise. */
  send: (payload: Record<string, unknown>) => boolean
}

function push(histories: Histories, key: HistoryKey, v: unknown): void {
  if (typeof v !== 'number' || !Number.isFinite(v)) return // 값 없음은 건너뜀 / skip absent
  const buf = histories[key] ?? (histories[key] = [])
  buf.push(v)
  if (buf.length > HISTORY_LEN) buf.shift()
}

export function useTelemetry(): Telemetry {
  const [frame, setFrame] = useState<PpTelemetry | null>(null)
  const [link, setLink] = useState<LinkState>('connecting')
  const [frames, setFrames] = useState(0)
  const [trackComplete, setTrackComplete] =
    useState<{ elapsedS?: number } | null>(null)
  const [tuning, setTuning] = useState<TuningState | null>(null)
  const [orchestration, setOrchestration] = useState<OrchestrationState | null>(null)
  const [authoritySnapshotSeq,setAuthoritySnapshotSeq]=useState(0)
  const [requestError,setRequestError]=useState<string | null>(null)
  /** 미응답 폴링의 발사 시각(ms). 0 이면 대기 중인 폴링이 없다.
   *  Send time of the outstanding poll; 0 means none is in flight. */
  const pollSentAtRef=useRef(0)
  /** 백엔드가 작업 중인 동안은 폴링을 멈춘다 — 백엔드가 단계마다 스스로
   *  스냅샷을 publish 하므로 폴링은 시뮬레이터 왕복만 추가로 만든다.
   *  Pause polling while the backend is working: it publishes a snapshot on
   *  every phase change anyway, so a poll only adds simulator round trips. */
  const backendBusyRef=useRef(false)
  const [leaderboard, setLeaderboard] = useState<LeaderboardState | null>(null)
  const [unsubmittedRecords,setUnsubmittedRecords]=useState<UnsubmittedRecord[]>([])
  const [recordingDownload, setRecordingDownload] =
    useState<{ id: string; url?: string; error?: string } | null>(null)
  const recordingRef = useRef(new Map<string, RecordingTransfer>())
  const downloadUrlRef=useRef<string | null>(null)
  const leaderboardServiceRef=useRef<BrowserLeaderboardService | null>(null)
  // 이력은 ref 로 — 프레임마다 setFrame 이 리렌더를 유발하므로 버퍼 자체는
  // 상태로 복제할 필요가 없다. / setFrame already re-renders every frame, so
  // the buffers themselves need no state copy.
  const historiesRef = useRef<Histories>({})
  const lastFrameAtRef = useRef(0)
  // client.socket 은 재접속 후의 현재 소켓을 돌려주는 live getter 다.
  // client.socket is a live getter returning the post-reconnect socket.
  const clientRef = useRef<WebSocketClientResult | null>(null)

  const send = useCallback((payload: Record<string, unknown>): boolean => {
    const sock = clientRef.current?.socket
    // 소켓은 앱 재시작 시 조용히 닫힌다 — OPEN 확인 없이 send 하면 예외.
    // The socket closes silently on app restart; sending unchecked throws.
    if (!sock || sock.readyState !== WebSocket.OPEN) return false
    sock.send(JSON.stringify(payload))
    return true
  }, [])

  useEffect(() => {
    let alive = true
    let client: WebSocketClientResult | null = null
    try {
      const leaderboardService=new BrowserLeaderboardService({store:new IndexedDbOutboxStore(),fetch:window.fetch.bind(window),publish:(value)=>{if(alive)setLeaderboard(value)},captured:(runId)=>send({kind:'sealed_capture_durable',runId}),captureFailed:(runId)=>send({kind:'sealed_capture_failed',runId}),submitted:(runId,verdict)=>send({kind:'leaderboard_submitted',runId,verdict})})
      leaderboardServiceRef.current=leaderboardService
    } catch {
      setLeaderboard({kind:'leaderboard_state',status:'capture_error',attempt:0,error:'Leaderboard outbox is unavailable',retryAllowed:false})
    }

    void (async () => {
      try {
        await initPorts()
      } catch {
        if (alive) setLink('closed')
        return
      }
      if (!alive) return
      client = createWebSocketClient('/socket', {
        autoReconnect: true,
        reconnectInterval: 3000,
        events: {
          open: () => {
            if (!alive) return
            setLink('waiting')
            // 접속(재접속 포함)마다 현재 튜닝 상태를 요청해 동기화한다.
            // Request the current tuning state on every (re)connect.
            clientRef.current?.socket?.send(
              JSON.stringify({ kind: 'get_tuning_state' }),
            )
            // 재접속 직후의 첫 스냅샷도 폴링과 같은 in-flight 규칙을 따른다.
            // The reconnect snapshot obeys the same in-flight rule as a poll.
            pollSentAtRef.current=Date.now()
            backendBusyRef.current=false
            clientRef.current?.socket?.send(
              JSON.stringify({ kind: 'get_orchestration_state' }),
            )
          },
          close: () => {
            if (!alive) return
            pollSentAtRef.current=0
            backendBusyRef.current=false
            setLink('closed')
          },
          error: () => {},
          message: (ev: MessageEvent) => {
            if (!alive || typeof ev.data !== 'string') return
            let msg: unknown
            try {
              msg = JSON.parse(ev.data)
            } catch {
              return
            }
            if (typeof msg !== 'object' || msg === null) return
            const kind = (msg as { kind?: unknown }).kind
            if (kind === 'track_complete') {
              setTrackComplete({ elapsedS: (msg as { elapsedS?: number }).elapsedS })
              return
            }
            if (kind === 'tuning_state') {
              // 백엔드가 clamp 까지 마친 실제 반영값 — UI 상태의 진실이다.
              // The backend's post-clamp truth; the UI state follows it.
              setTuning(msg as TuningState)
              return
            }
            if (kind === 'orchestration_state' || kind === 'sim_setup_state') {
              const state=msg as OrchestrationState
              pollSentAtRef.current=0
              backendBusyRef.current=Boolean(state.busy || state.setupApplying)
              setOrchestration({...state,selection:{...state.selection,
                receivedAtMonoMs:performance.now()}})
              setAuthoritySnapshotSeq((n)=>n+1)
              return
            }
            if (kind === 'orchestration_request_error') {
              const m=msg as {error?:unknown}
              setRequestError(typeof m.error==='string' && m.error
                ? m.error : 'orchestration request was rejected')
              return
            }
            if(kind==='leaderboard_state') {
              // Old FIFs may still emit this frame; external submission is
              // browser-owned, so never let a backend HTTPS state replace it.
              return
            }
            if(kind==='run_finalized') return
            if(kind==='run_reset') {
              const m=msg as {runId?:unknown;discard?:unknown;resetGeneration?:unknown}
              if(typeof m.runId==='string'&&m.discard===true)void leaderboardServiceRef.current?.discard(m.runId)
              if(typeof m.resetGeneration==='number')setUnsubmittedRecords([])
              return
            }
            if(kind==='run_cleanup_prepare') {
              const m=msg as {runId?:unknown;resetGeneration?:unknown}
              if(typeof m.runId==='string'&&typeof m.resetGeneration==='number')void leaderboardServiceRef.current?.discardAllUnsubmitted(m.resetGeneration)
                .then(ok=>send({kind:ok?'reset_cleanup_prepared':'reset_cleanup_failed',runId:m.runId,resetGeneration:m.resetGeneration}))
              return
            }
            if(kind==='run_reset_ready') {
              const m=msg as {runId?:unknown;resetGeneration?:unknown}
              if(typeof m.runId==='string'&&typeof m.resetGeneration==='number')send({kind:'reset_cleanup_durable',runId:m.runId,resetGeneration:m.resetGeneration})
              return
            }
            if(kind==='unsubmitted_records') {
              const rows=(msg as {records?:unknown}).records
              if(Array.isArray(rows))void leaderboardServiceRef.current?.outboxStates().then(states=>{
                setUnsubmittedRecords(mergeUnsubmittedRecords(
                  rows.filter((r):r is UnsubmittedRecord=>Boolean(r&&typeof r==='object'&&typeof (r as UnsubmittedRecord).runId==='string')),states))
              })
              return
            }
            if(kind==='unsubmitted_purge_result') {
              // Retained only for compatibility with older installed FIFs.
              return
            }
            if (kind === 'recording_begin') {
              const m=msg as { id: string; sha256?: string; totalBytes:number }
              const transfer=beginRecording(m.id,m.totalBytes,m.sha256)
              if (!transfer) {
                setRecordingDownload({id:m.id,error:'Invalid recording size'}); return
              }
              recordingRef.current.set(m.id,transfer)
              setRecordingDownload({ id: m.id })
              return
            }
            if (kind === 'recording_chunk') {
              const m=msg as { id: string; base64: string; index:number }
              const pending=recordingRef.current.get(m.id)
              if(pending) {
                const error=appendRecording(pending,m.index,m.base64)
                if(!error)return
                recordingRef.current.delete(m.id)
                setRecordingDownload({id:m.id,error})
              }
              return
            }
            if (kind === 'recording_error') {
              const m=msg as { id: string; error: string }
              setRecordingDownload({ id:m.id,error:m.error })
              return
            }
            if (kind === 'recording_end') {
              const m=msg as { id: string; chunks:number }
              const pending=recordingRef.current.get(m.id)
              if (!pending || !completeRecording(pending,m.chunks)) return
              recordingRef.current.delete(m.id)
              const blob=new Blob(pending.chunks as BlobPart[],{type:'text/csv'})
              if(blob.size!==pending.total) { setRecordingDownload({id:m.id,error:'Recording size mismatch'}); return }
              void blob.arrayBuffer().then(async (buffer) => {
                const subtle=globalThis.crypto?.subtle
                if (subtle && pending.sha) {
                  const digest=Array.from(new Uint8Array(await subtle.digest('SHA-256',buffer)))
                    .map((v)=>v.toString(16).padStart(2,'0')).join('')
                  if (digest !== pending.sha) {
                    setRecordingDownload({id:m.id,error:'Recording checksum mismatch'})
                    return
                  }
                }
                if(downloadUrlRef.current)URL.revokeObjectURL(downloadUrlRef.current)
                downloadUrlRef.current=URL.createObjectURL(blob)
                setRecordingDownload({id:m.id,url:downloadUrlRef.current})
              })
              return
            }
            if (kind !== 'pp_telemetry') return
            const f = msg as PpTelemetry
            const h = historiesRef.current
            for (const k of HISTORY_KEYS) push(h, k, f[k])
            // 종방향 목표는 longTargetMps 우선, 없으면 km/h 를 환산 —
            // 목표↔실측을 같은 단위 이력으로 겹쳐 보기 위한 파생 계열.
            // Derived series so target/actual share one unit in history.
            push(h, 'targetMps',
              f.longTargetMps ?? (f.targetSpeedKmh !== undefined ? f.targetSpeedKmh / 3.6 : undefined))
            lastFrameAtRef.current = Date.now()
            setFrame(f)
            setFrames((n) => n + 1)
            setLink('live')
          },
        },
      })
      clientRef.current = client
    })()

    // 프레임 정지 감시 — 소켓이 열려 있어도 프레임이 멎으면 화면의 값은 과거다.
    // Stall watchdog: an open socket with no frames means every value is stale.
    const watchdog = window.setInterval(() => {
      if (!alive) return
      setLink((cur) =>
        cur === 'live' && Date.now() - lastFrameAtRef.current > STALL_MS
          ? 'stalled'
          : cur,
      )
    }, 500)
    // ┌─ 설계 의도 / Design intent ──────────────────────────────────────────┐
    // 하트비트. 앱의 웹소켓 큐는 클라이언트가 뭔가 보낼 때만 플러시된다 —
    // 측정 결과 클라이언트가 조용하면 약 15초 뒤 발행이 통째로 멎고, 아무거나
    // 한 프레임 보내면 즉시 되살아난다. 그래서 절대 건너뛰지 않는다: 화면이
    // 숨겨져 있든, 백엔드가 바쁘든, 폴링이 이미 떠 있든 무조건 보낸다.
    //
    // 이게 없으면 Start 를 누른 직후가 최악이다. Start 는 busy=true 를 만들고
    // 아래 폴링은 busy 면 쉬므로 클라이언트가 침묵한다 → 펌프가 멎는다 →
    // 주행이 실제로 시작됐는지, 거부됐는지 프레임이 영영 안 온다. 화면은
    // "눌렀는데 아무 반응 없음" 으로 굳는다.
    //
    // kind 는 백엔드의 어느 핸들러에도 걸리지 않는 값이어야 한다(무시된다).
    // 부수효과 없이 펌프만 돌리는 것이 목적이다.
    //
    // Heartbeat. The app flushes its websocket queue only when the client sends
    // something: measured, publishing stops entirely after ~15 s of client
    // silence and resumes the moment any frame arrives. So this never skips —
    // not when hidden, not when the backend is busy, not when a poll is already
    // in flight. Without it the worst moment is right after Start: Start sets
    // busy=true, the poll below pauses while busy, the client falls silent, the
    // pump stalls, and no frame ever reports whether the run started or was
    // rejected — the screen freezes as "pressed it and nothing happened".
    // The kind must match no backend handler (it is ignored); the point is to
    // drive the pump with no side effect.
    // └──────────────────────────────────────────────────────────────────────┘
    const heartbeat = window.setInterval(() => {
      if (!alive) return
      const sock = clientRef.current?.socket
      if (sock?.readyState !== WebSocket.OPEN) return
      sock.send(JSON.stringify({ kind: 'ui_heartbeat' }))
    }, HEARTBEAT_MS)

    const orchestrationRefresh = window.setInterval(() => {
      if (!alive || document.visibilityState !== 'visible') return
      const sock=clientRef.current?.socket
      if(sock?.readyState!==WebSocket.OPEN) return
      // 백엔드가 일하는 중이면 스냅샷은 알아서 온다.
      if(backendBusyRef.current) return
      const now=Date.now()
      const pending=pollSentAtRef.current
      if(pending!==0 && now-pending<ORCHESTRATION_POLL_TIMEOUT_MS) return
      pollSentAtRef.current=now
      sock.send(JSON.stringify({kind:'get_orchestration_state'}))
    },ORCHESTRATION_POLL_MS)

    return () => {
      alive = false
      leaderboardServiceRef.current?.shutdown();leaderboardServiceRef.current=null
      window.clearInterval(watchdog)
      window.clearInterval(heartbeat)
      window.clearInterval(orchestrationRefresh)
      clientRef.current = null
      if(downloadUrlRef.current)URL.revokeObjectURL(downloadUrlRef.current)
      if (client) {
        closeWebSocketSafe(client.socket)
        client.close() // 재접속 루프까지 중지 / also stops the reconnect loop
      }
    }
  }, [])

  // Capture begins only after an explicit Submit has made the backend the
  // authority for this exact run. Reconnect can safely resume that granted
  // capture, but a merely completed run never causes a sealed GET.
  useEffect(()=>{
    if(orchestration?.postRun==='capture_in_progress'&&orchestration.recordingId&&orchestration.teamName)
      void leaderboardServiceRef.current?.enqueue(orchestration.teamName,orchestration.recordingId)
  },[orchestration?.postRun,orchestration?.recordingId,orchestration?.teamName])

  // Never process durable browser work before an authoritative backend
  // snapshot. Reconnect snapshots repeat reconciliation; each generation first
  // retires unmatched historical 404s and only then resumes eligible work.
  useEffect(()=>{
    if(!orchestration||authoritySnapshotSeq===0)return
    void leaderboardServiceRef.current?.reconcileAuthority(orchestration)
  },[authoritySnapshotSeq,orchestration])

  const retryLeaderboard=useCallback((runId:string)=>{void leaderboardServiceRef.current?.retry(runId)},[])
  const submitLeaderboard=useCallback((runId:string)=>{send(buildSubmitIntent(runId))},[send])
  const resetRun=useCallback((runId:string,_discard:boolean)=>{
    send(buildResetIntent(runId))
  },[send])
  const purgeAllUnsubmitted=useCallback(()=>{
    send({kind:'prepare_purge_all'})
  },[send])
  const requestUnsubmittedRecords=useCallback(()=>{send({kind:'list_unsubmitted_records'})},[send])
  const dismissRequestError=useCallback(()=>{setRequestError(null)},[])
  return {
    frame,
    link,
    frames,
    histories: historiesRef.current,
    trackComplete,
    dismissTrackComplete: () => setTrackComplete(null),
    tuning,
    orchestration,
    requestError,dismissRequestError,
    leaderboard,retryLeaderboard,submitLeaderboard,resetRun,purgeAllUnsubmitted,unsubmittedRecords,requestUnsubmittedRecords,
    recordingDownload,
    send,
  }
}

export const buildSubmitIntent=(runId:string)=>({kind:'begin_sealed_capture',runId})
export const buildResetIntent=(runId:string)=>({kind:'reset_run',runId})

export type OutboxStateRow={runId?:string;status:LeaderboardState['status'];teamName?:string;createdAt?:string;error?:string;retryAllowed?:boolean;hasEnvelope?:boolean}

/** 백엔드 목록(디스크의 CSV)과 브라우저 아웃박스를 한 목록으로 합친다.
 *
 *  아웃박스에만 있는 행은 종결 상태(submitted/invalid/failed_terminal)를
 *  거른다. 특히 failed_terminal 은 「모두 제거」가 남기는 폐기 표식이라 —
 *  드레인은 IndexedDB 행을 지우지 않고 이 상태로 바꿔만 둔다 — 거르지 않으면
 *  전체 삭제 뒤에도 목록에 영원히 되살아났다. 디스크에 CSV 가 실존하는 행은
 *  항상 실리고, 아웃박스 상태는 주석으로만 붙는다.
 *
 *  Merges the backend list (CSV files on disk) with the browser outbox.
 *  Outbox-only rows in a terminal state (submitted/invalid/failed_terminal)
 *  are dropped: failed_terminal is the marker purge-all leaves behind — the
 *  drain never deletes IndexedDB rows, it only flips them to this state — so
 *  without this filter every purged record resurrected in the list forever.
 *  Rows whose CSV really exists on disk always show, with the outbox state
 *  attached as an annotation only. */
export function mergeUnsubmittedRecords(rows:UnsubmittedRecord[],states:OutboxStateRow[]):UnsubmittedRecord[]{
  const merged=new Map<string,UnsubmittedRecord>()
  for(const state of states){
    if(!state.runId||state.status==='submitted'||state.status==='invalid'||state.status==='failed_terminal')continue
    merged.set(state.runId,{runId:state.runId,teamName:state.teamName??'',createdAt:state.createdAt??'',bytes:0,csvAvailable:false,outboxStatus:state.status,outboxError:state.error,outboxRetryAllowed:state.retryAllowed,outboxHasEnvelope:state.hasEnvelope})
  }
  for(const record of rows){
    const outbox=states.find(state=>state.runId===record.runId)
    merged.set(record.runId,{...record,outboxStatus:outbox?.status,outboxError:outbox?.error,outboxRetryAllowed:outbox?.retryAllowed,outboxHasEnvelope:outbox?.hasEnvelope})
  }
  return [...merged.values()].sort((a,b)=>a.runId.localeCompare(b.runId))
}
