import { renderToStaticMarkup } from 'react-dom/server'
import { describe, expect, it, vi } from 'vitest'
import { buildSetupIntent, canRetryLeaderboard, effectiveSelectionAgeMs, formatBytes, isCurrentRunCaptureBlocking, isPrimaryLeaderboardRelevant, RunSetup, SELECTION_STALE_BUDGET_MS } from './Dashboard'
import {buildResetIntent,buildSubmitIntent,ORCHESTRATION_POLL_MS,SIMULATOR_TICK_MS} from './useTelemetry'
import type { LeaderboardState, OrchestrationState } from './types'

const state: OrchestrationState={
  kind:'sim_setup_state',phase:'idle',busy:false,
  maps:[{id:'map-live-7',label:'Live Map'}],
  tractors:[{id:'tractor-live-4',label:'Live Tractor',wheelbaseM:2.4}],
  implements:[{id:'implement-live-2',label:'Live Plow',widthM:2.75}],
  selection:{mapId:'map-live-7',tractorId:'tractor-live-4',implementId:'implement-live-2',stopped:true,live:true,snapshotAgeMs:20},
}

describe('RunSetup',()=>{
  it('builds the typed immediate setup intent',()=>{
    expect(buildSetupIntent({teamName:'Team',mapId:'m',tractorId:'t',implementId:'i'}))
      .toEqual({kind:'set_sim_setup',teamName:'Team',mapId:'m',tractorId:'t',implementId:'i'})
  })
  it('builds exact run-bound manual Submit and Reset intents',()=>{
    expect(buildSubmitIntent('run-7')).toEqual({kind:'begin_sealed_capture',runId:'run-7'})
    expect(buildResetIntent('run-7')).toEqual({kind:'reset_run',runId:'run-7'})
  })
  it('renders live IDs and implement width with only Start and Finish run actions',()=>{
    const html=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html).toContain('value="map-live-7"')
    expect(html).toContain('2.75 m')
    const action=html.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    expect(action.match(/<button/g)).toHaveLength(2)
    expect(action).toContain('Start')
    expect(action).toContain('Finish')
    expect(action).not.toMatch(/Pause|Resume|일시정지|재개/)
  })

  it('uses authoritative none as the default implement selection',()=>{
    const noImplement={...state,
      implements:[{id:'none',label:'None',widthM:0},...state.implements],
      selection:{...state.selection,implementId:'none'}}
    const html=renderToStaticMarkup(<RunSetup orchestration={noImplement} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html).toContain('value="none"')
    expect(html).toContain('None')
  })

  it('uses viewport-responsive classes and remains selection-disabled when stale',()=>{
    const stale={...state,selection:{...state.selection,snapshotAgeMs:SELECTION_STALE_BUDGET_MS+1000}}
    const html=renderToStaticMarkup(<RunSetup orchestration={stale} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html).toContain('class="run-setup-fields"')
    expect(html).toContain('class="run-setup-actions"')
    expect((html.match(/disabled=""/g) ?? []).length).toBeGreaterThanOrEqual(5)
    expect(html).toContain('Catalog or simulator status is stale')
  })

  it('keeps the single Finish action enabled for retryable finalization',()=>{
    const retry={...state,phase:'failed' as const,finalization:'retryable_error' as const}
    const html=renderToStaticMarkup(<RunSetup orchestration={retry} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    const action=html.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    expect(action.match(/<button/g)).toHaveLength(2)
    expect(action).toContain('Finish')
    expect(action.match(/disabled=""/g) ?? []).toHaveLength(1)
  })

  it('renders only backend desired values while applying and rolls back to confirmed',()=>{
    const applying={...state,maps:[...state.maps,{id:'map-pending',label:'Pending Map'}],setupApplying:true,busy:true,
      desiredSetup:{mapId:'map-pending',tractorId:'tractor-live-4',implementId:'implement-live-2'},
      confirmedSetup:{mapId:'map-live-7',tractorId:'tractor-live-4',implementId:'implement-live-2'}}
    const pendingHtml=renderToStaticMarkup(<RunSetup orchestration={applying} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(pendingHtml).toContain('Applying simulator setup')
    expect(pendingHtml).toContain('value="map-pending"')
    expect((pendingHtml.match(/disabled=""/g) ?? [])).toHaveLength(5)
    expect(pendingHtml).not.toMatch(/aria-label="Team name"[^>]*disabled/)

    const failed={...state,phase:'failed' as const,error:'map selection confirmation timed out',
      setupApplying:false,confirmedSetup:{mapId:'map-live-7',tractorId:'tractor-live-4',implementId:'implement-live-2'}}
    const failedHtml=renderToStaticMarkup(<RunSetup orchestration={failed} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(failedHtml).toContain('value="map-live-7"')
    expect(failedHtml).toContain('map selection confirmation timed out')
  })

  it('shows leaderboard results and only authorizes retryable current runs',()=>{
    const submitted: LeaderboardState={kind:'leaderboard_state',status:'submitted',attempt:1,
      runId:'run-1',verdict:'SCORED',score:42,verified:true,retryAllowed:false}
    const current={...state,finalization:'completed' as const,recordingId:'run-1'}
    const done=renderToStaticMarkup(<RunSetup orchestration={current} leaderboard={submitted} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(done).toContain('SCORED');expect(done).toContain('score 42');expect(done).not.toContain('Retry leaderboard')
    const retry: LeaderboardState={kind:'leaderboard_state',status:'retry_wait',attempt:5,
      runId:'run-1',error:'network unavailable',retryAllowed:true}
    const waiting=renderToStaticMarkup(<RunSetup orchestration={current} leaderboard={retry} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(waiting).toContain('network unavailable');expect(waiting).toContain('Retry leaderboard')
    expect(canRetryLeaderboard(retry)).toBe(true)
    expect(canRetryLeaderboard({...retry,status:'failed_terminal'})).toBe(false)
    const capture={...retry,status:'capture_error' as const,backlog:2,error:'capture journal persistence failed'}
    const captureHtml=renderToStaticMarkup(<RunSetup orchestration={current} leaderboard={capture} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(captureHtml).toContain('backlog 2');expect(captureHtml).toContain('capture journal persistence failed');expect(canRetryLeaderboard(capture)).toBe(true)
  })
  // 회귀: 신선도 예산이 폴링 주기보다 짧으면 매 주기마다 반드시 낡음으로 넘어가
  // 배너가 깜빡이고 조작이 잠긴다. 실제로 예산 1000ms + 폴링 2500ms 조합에서
  // 앱이 사용 불가능해졌다. 시뮬레이터 자체 나이(0~1000ms 톱니)까지 더해도
  // 정상 폴링 한 주기는 반드시 신선해야 한다.
  // Regression: a budget shorter than the poll period goes stale once per period
  // by construction — the banner flaps and the controls lock. A budget of 1000 ms
  // against a 2500 ms poll made the app unusable. One normal poll period must stay
  // fresh even after the simulator's own 0–1000 ms sawtooth is added.
  it('keeps a snapshot fresh for a full poll period plus the simulator tick',()=>{
    expect(SELECTION_STALE_BUDGET_MS).toBeGreaterThan(SIMULATOR_TICK_MS+ORCHESTRATION_POLL_MS)
    const worstCase={...state,teamName:'Team',selection:{...state.selection,
      snapshotAgeMs:SIMULATOR_TICK_MS,receivedAtMonoMs:performance.now()-ORCHESTRATION_POLL_MS}}
    const html=renderToStaticMarkup(<RunSetup orchestration={worstCase} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html).not.toContain('Catalog or simulator status is stale')
    // 낡음이면 select 3개 + 팀 입력이 잠긴다. 신선하면 Start 까지 눌러야 한다.
    // Stale locks the three selects and the team input; fresh must reach Start.
    const fields=html.match(/data-testid="run-fields"[\s\S]*?data-testid="run-actions"/)?.[0] ?? ''
    expect(fields).toContain('<select')
    expect(fields).not.toContain('disabled=""')
    const actions=html.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    expect(actions).toContain('Start')
    expect(actions.match(/disabled=""/g) ?? []).toHaveLength(1)
  })

  it('expires cached selection age and requires stopped non-running live link',()=>{
    expect(effectiveSelectionAgeMs({...state.selection,snapshotAgeMs:200,receivedAtMonoMs:1000},1901)).toBe(1101)
    const overBudget=SELECTION_STALE_BUDGET_MS-200+1
    const cached={...state,selection:{...state.selection,snapshotAgeMs:200,receivedAtMonoMs:performance.now()-overBudget}}
    const stale=renderToStaticMarkup(<RunSetup orchestration={cached} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(stale).toContain('Catalog or simulator status is stale')
    const running={...state,selection:{...state.selection,reactRunning:true}}
    const html=renderToStaticMarkup(<RunSetup orchestration={running} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0]).toContain('disabled=""')
    const waiting=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="waiting" runState="idle" send={vi.fn()}/>)
    expect(waiting.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0]).toContain('disabled=""')
  })
  it('hides unrelated retired terminal state from primary banner',()=>{
    const retired:LeaderboardState={kind:'leaderboard_state',status:'failed_terminal',attempt:0,
      runId:'old-run',retryAllowed:false,error:'retired stale capture: sealed recording HTTP 404 (application/json)'}
    expect(isPrimaryLeaderboardRelevant(state,retired)).toBe(false)
    const hidden=renderToStaticMarkup(<RunSetup orchestration={state} leaderboard={retired} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(hidden).not.toContain('Leaderboard:')
    const current={...state,finalization:'completed' as const,recordingId:'old-run'}
    expect(isPrimaryLeaderboardRelevant(current,retired)).toBe(true)
    expect(renderToStaticMarkup(<RunSetup orchestration={current} leaderboard={retired} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)).toContain('retired stale capture')
  })
  it('blocks Start only for capture work belonging to the current completed run',()=>{
    const completed={...state,teamName:'Team',finalization:'completed' as const,recordingId:'current-run'}
    const captureError:LeaderboardState={kind:'leaderboard_state',status:'capture_error',attempt:0,runId:'old-run',retryAllowed:true,error:'sealed recording HTTP 404'}
    expect(isCurrentRunCaptureBlocking(completed,captureError)).toBe(false)
    const staleHtml=renderToStaticMarkup(<RunSetup orchestration={completed} leaderboard={captureError} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    const staleActions=staleHtml.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    expect(staleActions).toContain('Submit');expect(staleActions).toContain('Reset')
    expect(staleActions.match(/disabled=""/g) ?? []).toHaveLength(2)

    const currentCapture={...captureError,runId:'current-run'}
    expect(isCurrentRunCaptureBlocking(completed,currentCapture)).toBe(true)
    const currentHtml=renderToStaticMarkup(<RunSetup orchestration={completed} leaderboard={currentCapture} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    const currentActions=currentHtml.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    expect(currentActions.match(/disabled=""/g) ?? []).toHaveLength(2)

    expect(isCurrentRunCaptureBlocking(completed,{...currentCapture,status:'submitted',retryAllowed:false})).toBe(false)
    expect(isCurrentRunCaptureBlocking({...state,recordingId:undefined},captureError)).toBe(false)
  })

  it('surfaces a backend admission rejection separately from the snapshot error',()=>{
    const rejected='Start rejected: another orchestration request is still in flight'
    const html=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="live" runState="idle" send={vi.fn()} requestError={rejected}/>)
    expect(html).toContain(rejected)
    expect(html).toContain('run-setup-rejection')
    const silent=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(silent).not.toContain('run-setup-rejection')
  })

  it('explains a failed finalisation and disables Start, which the backend would refuse',()=>{
    const stuck={...state,teamName:'Team',finalization:'retryable_error' as const}
    const html=renderToStaticMarkup(<RunSetup orchestration={stuck} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(html).toContain('finish-retry')
    expect(html).toContain('Finish')
    const actions=html.match(/data-testid="run-actions"[\s\S]*?<\/div>/)?.[0] ?? ''
    // Start disabled (the backend refuses it), Finish enabled (it is the way out).
    expect(actions.match(/disabled=""/g) ?? []).toHaveLength(1)
    expect(actions.split('Finish')[0]).toContain('disabled=""')

    const idle=renderToStaticMarkup(<RunSetup orchestration={{...state,teamName:'Team'}} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(idle).not.toContain('finish-retry')
  })

  // 회귀: 게이트(신선도·busy)가 폴링마다 퍼덕이면 input 이 disabled 로 바뀌고
  // 브라우저는 disabled 컨트롤에서 포커스를 뺏는다 — "입력 중에 자꾸 멈춤".
  // 팀 이름은 로컬 문자열이므로 잠글 이유가 주행 중 하나뿐이고, 그때도
  // readOnly 로 잠근다(포커스 유지, 복사 가능).
  // Regression: flapping gates (freshness/busy) flipped the input to disabled,
  // and the browser steals focus from a disabled control — the "keeps
  // interrupting my typing" bug. The name is local, so the only lock is an
  // active run, and even that is readOnly, which never steals focus.
  it('keeps the team input focusable: never disabled, readOnly only while a run is active',()=>{
    const stale={...state,selection:{...state.selection,snapshotAgeMs:SELECTION_STALE_BUDGET_MS+1000}}
    const staleHtml=renderToStaticMarkup(<RunSetup orchestration={stale} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(staleHtml).not.toMatch(/aria-label="Team name"[^>]*disabled/)
    expect(staleHtml).not.toMatch(/aria-label="Team name"[^>]*readonly/i)
    const busyHtml=renderToStaticMarkup(<RunSetup orchestration={{...state,busy:true}} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(busyHtml).not.toMatch(/aria-label="Team name"[^>]*(disabled|readonly)/i)
    const offlineHtml=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="closed" runState="idle" send={vi.fn()}/>)
    expect(offlineHtml).not.toMatch(/aria-label="Team name"[^>]*(disabled|readonly)/i)
    const runningHtml=renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="live" runState="running" send={vi.fn()}/>)
    expect(runningHtml).toMatch(/aria-label="Team name"[^>]*readonly/i)
    expect(runningHtml).not.toMatch(/aria-label="Team name"[^>]*disabled/)
  })

  // SimWorld 물리는 브라우저 페이지 안에서 돈다 — 창이 가려지거나 절전에서 못
  // 깨어나면 Start 가 조용히 무의미해진다. loopDiag 가 그 유일한 증거다.
  // SimWorld physics lives in a browser page: occlusion or an unrecovered
  // throttle makes Start silently meaningless, and loopDiag is the only evidence.
  it('raises the SimWorld health alert for a hidden or throttle-wedged sim page',()=>{
    const hidden={...state,selection:{...state.selection,
      loopDiag:{hidden:true,schedule:'timer',droppedMs:182000,ticks:649}}}
    const hiddenHtml=renderToStaticMarkup(<RunSetup orchestration={hidden} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(hiddenHtml).toContain('simworld-health')
    expect(hiddenHtml).toContain('가려져')
    expect(hiddenHtml).toContain('182초')

    const wedged={...state,selection:{...state.selection,
      loopDiag:{hidden:false,schedule:'timer',droppedMs:300000,ticks:12}}}
    const wedgedHtml=renderToStaticMarkup(<RunSetup orchestration={wedged} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>)
    expect(wedgedHtml).toContain('simworld-health')
    expect(wedgedHtml).toContain('새로고침')

    const healthy={...state,selection:{...state.selection,
      loopDiag:{hidden:false,schedule:'raf',droppedMs:0,ticks:16239}}}
    expect(renderToStaticMarkup(<RunSetup orchestration={healthy} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>))
      .not.toContain('simworld-health')
    // loopDiag 를 모르는 옛 FIF 는 침묵해야 한다 / an old FIF without loopDiag stays silent
    expect(renderToStaticMarkup(<RunSetup orchestration={state} recordingDownload={null} link="live" runState="idle" send={vi.fn()}/>))
      .not.toContain('simworld-health')
  })

  it('formats recording sizes in binary units and rejects absent values',()=>{
    expect(formatBytes(0)).toBe('0 B')
    expect(formatBytes(1023)).toBe('1023 B')
    expect(formatBytes(1536)).toBe('1.5 KiB')
    expect(formatBytes(5*1024*1024)).toBe('5.0 MiB')
    expect(formatBytes(Number.NaN)).toBe('—')
    expect(formatBytes(-1)).toBe('—')
  })
})
