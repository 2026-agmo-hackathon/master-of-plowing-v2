#include <AppMain/orchestration/RunOrchestrator.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <iostream>
#include <thread>

namespace AppMain {
namespace orchestration {

bool RunOrchestrator::sealedCaptureRequired(const Snapshot& snapshot,
                                            const std::string& acknowledgedRunId)
{
    return snapshot.finalization==FinalizationState::Completed
        && !snapshot.recordingId.empty()
        && snapshot.recordingId!=acknowledgedRunId;
}

namespace {
bool idSafe(const std::string& value)
{
    return !value.empty() && value.size() <= 96 &&
           std::all_of(value.begin(), value.end(), [](unsigned char c) {
               return std::isalnum(c) || c == '-' || c == '_';
           });
}

long long effectiveAgeMs(const Snapshot& snapshot)
{
    const long long reported=snapshot.catalog.selection.snapshotAgeMs;
    if(reported<0 || snapshot.selectionObservedAt.time_since_epoch().count()==0)
        return reported;
    const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now()-snapshot.selectionObservedAt).count();
    return reported+std::max<long long>(0,elapsed);
}

// Every published copy must carry the age as of the moment it is published.
// Publishing snapshot_ verbatim ships the age recorded at the last refresh, so
// frames emitted from the same poll disagree by hundreds of milliseconds and
// the UI freshness gate (<=1000 ms) flickers, disabling controls mid-interaction.
Snapshot publishable(const Snapshot& snapshot)
{
    Snapshot copy=snapshot;
    copy.catalog.selection.snapshotAgeMs=effectiveAgeMs(snapshot);
    return copy;
}

std::string physicalStopIssue(const Selection& selection,
                              const StartRequest& expected)
{
    if(!selection.live)return "simulator selection is not live";
    if(selection.snapshotAgeMs<0)return "simulator selection age is invalid: "+
        std::to_string(selection.snapshotAgeMs)+" ms";
    if(selection.snapshotAgeMs>RunOrchestrator::MAX_SNAPSHOT_AGE_MS)
        return "simulator selection is stale: "+
            std::to_string(selection.snapshotAgeMs)+" ms (maximum "+
            std::to_string(RunOrchestrator::MAX_SNAPSHOT_AGE_MS)+" ms)";
    if(!selection.stopped)return "simulator selection stopped=false";
    if(selection.reactRunning)return "simulator selection reactRunning=true";
    if(selection.mapId!=expected.mapId || selection.tractorId!=expected.tractorId
        || selection.implementId!=expected.implementId) {
        return "selected IDs mismatch: expected "+expected.mapId+"/"+
            expected.tractorId+"/"+expected.implementId+", observed "+
            selection.mapId+"/"+selection.tractorId+"/"+selection.implementId;
    }
    // Keep this helper tied to the single authoritative predicate.
    return RunOrchestrator::physicalStopSampleAcceptable(selection,expected)
        ? std::string{} : "simulator physical-stop state is invalid";
}

std::string httpStatusCategory(const ApiResult& result)
{
    if(result.status<=0)return "transport unavailable";
    if(result.status==409)return "HTTP 409 conflict";
    if(result.status==503)return "HTTP 503 simulator not ready";
    return "HTTP "+std::to_string(result.status);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
// 지금까지 오케스트레이션은 로그를 한 줄도 남기지 않아서, 모든 장애를 라이브
// WebSocket 프로브로만 진단할 수 있었다. 전이·거절·종결 이벤트를 fcal.log 로
// 내보내 사후 진단이 가능하게 한다. 한 번의 스트림 연산으로 써서 다중 스레드
// 로그 줄이 서로 찢기지 않게 한다.
// └────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────┐
// Orchestration used to emit no log lines at all, so every incident could
// only be diagnosed with a live WebSocket probe. Transition, refusal and
// finalization events now reach fcal.log. Each line is a single stream write
// so concurrent threads cannot tear it apart.
// └────────────────────────────────────────────────────────────────────────┘
void logLine(const std::string& text)
{
    std::cerr<<("[Orchestrator] "+text+"\n");
}
}

const char* phaseName(Phase phase)
{
    switch (phase) {
        case Phase::Refreshing: return "refreshing";
        case Phase::ApplyingSetup: return "applying_setup";
        case Phase::Starting: return "starting";
        case Phase::Running: return "running";
        case Phase::Finishing: return "finishing";
        case Phase::Failed: return "failed";
        case Phase::Idle:
        default: return "idle";
    }
}

const char* finalizationStateName(FinalizationState state)
{
    switch(state) {
        case FinalizationState::InProgress: return "in_progress";
        case FinalizationState::Completed: return "completed";
        case FinalizationState::RetryableError: return "retryable_error";
        case FinalizationState::Idle:
        default: return "idle";
    }
}

const char* postRunStateName(PostRunState state)
{
    switch(state){case PostRunState::None:return "none";case PostRunState::AwaitingAction:return "awaiting_action";case PostRunState::CaptureInProgress:return "capture_in_progress";case PostRunState::CaptureDurable:return "capture_durable";case PostRunState::Submitted:return "submitted";case PostRunState::ResetRetryable:return "reset_retryable";case PostRunState::ResetCleanupPreparing:return "reset_cleanup_preparing";case PostRunState::ResetCleanupPending:return "reset_cleanup_pending";}return "none";
}

RunOrchestrator::RunOrchestrator(ISimulatorApi& api, IRecordingStore& store,
                                 IRunControl& runControl,
                                 std::function<void(const Snapshot&)> publish,
                                 std::function<void(const std::string&,const std::string&)> finalized,
                                 std::function<void(const std::string&)> notify)
    : api_(api), store_(store), runControl_(runControl), publish_(std::move(publish)),
      finalized_(std::move(finalized)), notify_(std::move(notify))
{
}

const CatalogItem* RunOrchestrator::find(const std::vector<CatalogItem>& items,
                                         const std::string& id)
{
    const auto it = std::find_if(items.begin(), items.end(),
            [&](const CatalogItem& item) { return item.id == id; });
    return it == items.end() ? nullptr : &*it;
}

bool RunOrchestrator::fresh(const Selection& s)
{
    return s.live && s.snapshotAgeMs >= 0 && s.snapshotAgeMs <= MAX_SNAPSHOT_AGE_MS;
}

bool RunOrchestrator::physicalStopSampleAcceptable(
        const Selection& selection, const StartRequest& expected)
{
    return vehicleAtRestSampleAcceptable(selection,expected)
        && !selection.reactRunning;
}

bool RunOrchestrator::vehicleAtRestSampleAcceptable(
        const Selection& selection, const StartRequest& expected)
{
    return fresh(selection) && selection.stopped
        && selection.mapId==expected.mapId
        && selection.tractorId==expected.tractorId
        && selection.implementId==expected.implementId;
}

bool RunOrchestrator::simulatorLoopDown(long long snapshotAgeMs)
{
    return snapshotAgeMs>=SIMULATOR_DOWN_AGE_MS;
}

void RunOrchestrator::setPhase(Phase phase, const std::string& error)
{
    Snapshot copy;
    bool logged=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.phase = phase;
        snapshot_.busy = phase == Phase::Refreshing || phase == Phase::Starting ||
                         phase == Phase::ApplyingSetup || phase == Phase::Finishing;
        snapshot_.error = error;
        copy = publishable(snapshot_);
        // Refreshing 은 리셋 경로만 지나는 통과 상태라 로그에서 뺀다(리셋은
        // 자체 로그가 있다). 그 외 전이는 (phase, error) 변화마다 한 번 남긴다.
        // Refreshing is now only a reset-path pass-through with its own log
        // lines; every other transition logs once per (phase, error) change.
        if(phase!=Phase::Refreshing
            && (phase!=loggedPhase_ || error!=loggedError_)) {
            loggedPhase_=phase; loggedError_=error;
            logged=true;
        }
    }
    if(logged)
        logLine(std::string("phase=")+phaseName(phase)
            +(error.empty()?std::string{}:" error=\""+error+"\""));
    if (publish_) publish_(copy);
}

Snapshot RunOrchestrator::snapshot() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    Snapshot copy=publishable(snapshot_);
    // The age is now current, so the observation instant must move with it —
    // otherwise a consumer that ages the copy again double-counts the elapsed.
    copy.selectionObservedAt=std::chrono::steady_clock::now();
    return copy;
}

void RunOrchestrator::requestShutdown()
{
    shutdownRequested_=true;
}

bool RunOrchestrator::shuttingDown() const
{
    return shutdownRequested_.load();
}

bool RunOrchestrator::refresh()
{
    if(shuttingDown()) return false;
    // 읽기 전용 통과 상태는 방송하지 않는다. 매 2.5초 폴마다 Refreshing/busy
    // 프레임이 나가면 UI 의 편집 게이트가 폴마다 퍼덕여 설정 셀렉트가 회색으로
    // 깜빡인다 (실측: 폴 한 번에 idle→refreshing→idle 3연발). 아래에서 정착된
    // 스냅샷만 발행한다.
    // Do not broadcast the read-only pass-through: a Refreshing/busy frame on
    // every 2.5 s poll flaps the UI's edit gate and visibly blinks the setup
    // selects (measured: an idle→refreshing→idle triple per poll). Only the
    // settled snapshot below is published.
    Catalog catalog;
    const ApiResult result = api_.catalog(catalog);
    if (!result.ok || !fresh(catalog.selection)) {
        bool postRunQuiet=false;
        bool debouncing=false;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            postRunQuiet=snapshot_.postRun!=PostRunState::None
                || finalizationState_==FinalizationState::Completed;
            // ┌─ 한국어 ────────────────────────────────────────┐
            // 서버에 닿지도 못한 요청은 지금 곧장 말해야 하는 사실이다.
            // 디바운스는 시뮬레이터가 답을 주기는 한 모든 경우를 덮는다:
            // 200 안의 스냅샷이 낡은 경우, 그리고 클라이언트가 실패로 옮겨
            // 담는 "not live" / "catalog is stale" 판정(그래서
            // simulatorQuiet 이 필요하다). 실측 2026-08-18: 두 번째를 빼놨더니
            // 9 분 주행 내내 배너가 2~5 초마다 running -> failed -> running
            // 으로 깜빡였다.
            // └────────────────────────────────────────────┘
            // ┌─ English ───────────────────────────────────────┐
            // A catalog request that never reached the server is a fact to
            // report now. The debounce covers every answer the simulator did
            // give: a stale snapshot inside a 200, and the client's own
            // "not live" / "catalog is stale" verdicts, which are 200 responses
            // the client folds into failures (hence result.simulatorQuiet).
            // Measured 2026-08-18: without that second case the banner flapped
            // running -> failed -> running every two to five seconds for the
            // whole of a nine-minute run.
            // └────────────────────────────────────────────┘
            if(result.ok || result.simulatorQuiet)
                debouncing=++consecutiveStaleCatalogPolls_
                    <STALE_CATALOG_POLLS_BEFORE_FAILED;
        }
        if(!postRunQuiet && !debouncing) {
            setPhase(Phase::Failed, result.ok ? "simulator catalog is stale" : result.error);
            return false;
        }
        // ┌─ 한국어 ───────────────────────────────────────────────────────┐
        // 주행이 종결되면 finishImpl 이 엔진을 끄고, 엔진이 꺼진 시뮬레이터는
        // 물리 루프가 멈춰 스냅샷을 띄엄띄엄만 발행한다 (실측: live=false
        // age=9154ms 와 live=true age=255ms 가 번갈아 나옴). 그 상태에서 매
        // 새로고침이 "maps catalog is not live" 로 phase 를 Failed 로 꺾으면,
        // 방금 완주·제출까지 끝난 화면에 오류 배너가 깜빡인다. 종결 후의
        // 카탈로그 유실은 기대된 상태이므로 조용히 넘기고, 선택 스냅샷만이라도
        // 갱신을 시도해 캐시가 한없이 낡는 것을 막는다.
        // └────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────┐
        // Finalization cuts the engine, and an engine-off simulator stops its
        // physics loop and publishes snapshots only sporadically (measured:
        // live=false age=9154ms alternating with live=true age=255ms). If every
        // refresh then flips the phase to Failed with "maps catalog is not
        // live", the screen of a run that just completed and submitted blinks
        // an error banner. A post-run catalog outage is the expected state, so
        // stay quiet and still try to update the cached selection so it does
        // not age without bound.
        // └────────────────────────────────────────────────────────────────┘
        Selection current;
        if(api_.selection(current).ok && fresh(current)) {
            std::lock_guard<std::mutex> lock(mutex_);
            snapshot_.catalog.selection=current;
            snapshot_.selectionObservedAt=std::chrono::steady_clock::now();
        }
        setPhase(runControl_.runActive()?Phase::Running:Phase::Idle);
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        consecutiveStaleCatalogPolls_=0;
        snapshot_.catalog = std::move(catalog);
        snapshot_.selectionObservedAt=std::chrono::steady_clock::now();
        const Selection& selected=snapshot_.catalog.selection;
        snapshot_.confirmedSetup.mapId=selected.mapId;
        snapshot_.confirmedSetup.tractorId=selected.tractorId;
        snapshot_.confirmedSetup.implementId=selected.implementId;
        if(snapshot_.desiredSetup.mapId.empty())
            snapshot_.desiredSetup=snapshot_.confirmedSetup;
    }
    setPhase(runControl_.runActive()?Phase::Running:Phase::Idle);
    return true;
}

void RunOrchestrator::noteSetupIntent(const StartRequest& request,
                                      unsigned long long generation)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(generation<snapshot_.setupGeneration) return;
        snapshot_.setupGeneration=generation;
        snapshot_.desiredSetup=request;
        snapshot_.teamName=request.teamName;
        snapshot_.setupApplying=true;
        snapshot_.busy=true;
        snapshot_.error.clear();
        copy=publishable(snapshot_);
    }
    if(publish_) publish_(copy);
}

bool RunOrchestrator::applySetup(const StartRequest& request,
                                 unsigned long long generation)
{
    if(shuttingDown()) return false;
    Snapshot immediate;
    bool teamOnly=false;
    bool admissionRejected=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(generation!=snapshot_.setupGeneration) return false;
        if(runControl_.runActive() || snapshot_.phase==Phase::Running
            || snapshot_.phase==Phase::Starting || snapshot_.phase==Phase::Finishing) {
            snapshot_.setupApplying=false;
            snapshot_.busy=false;
            snapshot_.error="setup changes require a stopped idle controller";
            immediate=publishable(snapshot_);
            admissionRejected=true;
        }
        const StartRequest& confirmed=snapshot_.confirmedSetup;
        teamOnly=!confirmed.mapId.empty()
            && confirmed.mapId==request.mapId
            && confirmed.tractorId==request.tractorId
            && confirmed.implementId==request.implementId;
        if(teamOnly) {
            snapshot_.confirmedSetup.teamName=request.teamName;
            snapshot_.setupApplying=false;
            snapshot_.busy=false;
            snapshot_.phase=Phase::Idle;
            immediate=publishable(snapshot_);
        }
    }
    if(admissionRejected) { if(publish_) publish_(immediate); return false; }
    if(teamOnly) { if(publish_) publish_(immediate); return true; }
    auto failSetup=[&](const std::string& error) {
        logLine("setup failed: "+error);
        Snapshot failed;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(generation!=snapshot_.setupGeneration) return false;
            snapshot_.setupApplying=false;
            snapshot_.busy=false;
            snapshot_.phase=Phase::Failed;
            snapshot_.error=error;
            failed=publishable(snapshot_);
        }
        if(publish_) publish_(failed);
        return false;
    };
    if(!idSafe(request.mapId) || !idSafe(request.tractorId)
        || !idSafe(request.implementId) || request.teamName.size()>80) {
        return failSetup("invalid team or setup identifier");
    }
    setPhase(Phase::ApplyingSetup);
    Catalog catalog;
    ApiResult result=api_.catalog(catalog);
    if(!result.ok) return failSetup("simulator catalog unavailable: "+result.error);
    if(!catalog.selection.live) return failSetup("simulator selection is not live");
    if(catalog.selection.snapshotAgeMs<0
        || catalog.selection.snapshotAgeMs>MAX_SNAPSHOT_AGE_MS)
        return failSetup("simulator selection is stale");
    if(!catalog.selection.stopped) return failSetup("simulator is not stopped");
    if(!find(catalog.maps,request.mapId)) return failSetup("unknown map: "+request.mapId);
    if(!find(catalog.tractors,request.tractorId))
        return failSetup("unknown tractor: "+request.tractorId);
    if(!find(catalog.implements,request.implementId))
        return failSetup("unknown implement: "+request.implementId);
    Selection selected=catalog.selection;
    auto applyOne=[&](const std::string& wanted,
                      const std::function<ApiResult()>& apply,
                      const std::function<bool(const Selection&)>& exact,
                      const char* label) {
        // 409 "tractor must be stopped" 는 재시도로 풀리는 경쟁 상태다. 선택
        // 스냅샷은 1 Hz 라, 맵을 바꾼 직후 차량이 다시 자리를 잡는 동안에도
        // 앱에는 직전의 stopped=true 샘플이 그대로 보인다. 그래서 map -> tractor
        // -> implement 를 잇달아 보내면 두 번째 요청이 409 로 튕기고, 화면에는
        // "tractor selection: tractor must be stopped" 만 남아 셋업이 멈췄다.
        //
        // A 409 "tractor must be stopped" is a race that retrying resolves: the
        // selection snapshot is 1 Hz, so while the vehicle is still settling
        // after a map change the app still sees the previous stopped=true
        // sample. Sending map -> tractor -> implement back to back therefore
        // bounced the second request and left the setup stuck behind
        // "tractor selection: tractor must be stopped".
        constexpr int MAX_CONFLICT_RETRIES=20;
        for(int attempt=0;;++attempt) {
            const ApiResult applied=apply();
            if(applied.ok) break;
            if(applied.status!=409 || attempt>=MAX_CONFLICT_RETRIES)
                return failSetup(std::string(label)+": "+applied.error);
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        for(int attempt=0;attempt<30;++attempt) {
            Selection current;
            const ApiResult state=api_.selection(current);
            if(!state.ok) return failSetup(std::string(label)+" confirmation: "+state.error);
            if(fresh(current) && current.stopped && exact(current)) { selected=current; return true; }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return failSetup(std::string(label)+" confirmation timed out for "+wanted);
    };
    bool ok=true;
    if(selected.mapId!=request.mapId)
        ok=applyOne(request.mapId,[&]{return api_.selectMap(request.mapId);},
            [&](const Selection& s){return s.mapId==request.mapId;},"map selection");
    if(ok && selected.tractorId!=request.tractorId)
        ok=applyOne(request.tractorId,[&]{return api_.selectTractor(request.tractorId);},
            [&](const Selection& s){return s.mapId==request.mapId && s.tractorId==request.tractorId;},"tractor selection");
    if(ok && selected.implementId!=request.implementId)
        ok=applyOne(request.implementId,[&]{return api_.selectImplement(request.implementId);},
            [&](const Selection& s){return s.mapId==request.mapId && s.tractorId==request.tractorId && s.implementId==request.implementId;},"implement selection");
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(generation==snapshot_.setupGeneration) {
            snapshot_.catalog=catalog;
            snapshot_.catalog.selection=selected;
            snapshot_.selectionObservedAt=std::chrono::steady_clock::now();
            if(ok) snapshot_.confirmedSetup=request;
            snapshot_.setupApplying=false;
            snapshot_.busy=false;
            snapshot_.phase=ok?Phase::Idle:Phase::Failed;
            copy=publishable(snapshot_);
        } else return false;
    }
    if(publish_) publish_(copy);
    return ok;
}

bool RunOrchestrator::start(const StartRequest& request, std::string* refusal)
{
    auto refuse=[&](const std::string& reason) {
        logLine("start refused: "+reason);
        if(refusal) *refusal=reason;
        return false;
    };
    if(shuttingDown()) return refuse("the app is shutting down");
    {
        std::lock_guard<std::mutex> lock(mutex_);
        // 이 가드가 왜 걸렸는지 이름을 붙인다.
        //
        // 예전에는 조건 하나로 묶어 그냥 false 를 돌려줬다. phase 도 안 바뀌고
        // 에러도 안 나가서, 브라우저 입장에서는 Start 를 눌러도 말 그대로 아무
        // 일도 일어나지 않았다. 특히 finalization 이 RetryableError 로 굳으면
        // (직전 주행 마무리 실패) 앱을 다시 띄우기 전까지 모든 Start 가 조용히
        // 거절된다 — 화면에는 단서가 하나도 없다. 무엇을 해야 풀리는지까지
        // 문장에 담는다.
        //
        // Name whichever guard tripped. This used to be one condition returning
        // a bare false: no phase change, no error, so pressing Start did
        // literally nothing from the browser's point of view. A finalization
        // stuck at RetryableError (the previous run failed to finalise) refuses
        // every Start until the app is restarted, with nothing on screen to say
        // so. Each message names the way out.
        if(runControl_.runActive() || snapshot_.phase==Phase::Running
            || snapshot_.phase==Phase::Starting)
            return refuse("a run is already in progress");
        if(snapshot_.setupApplying)
            return refuse("simulator setup is still being applied");
        if(snapshot_.postRun!=PostRunState::None)
            return refuse("the previous recording is still open —"
                          " submit or reset it before starting a new run");
        if(finalizationState_==FinalizationState::InProgress)
            return refuse("the previous run is still being finalised");
        if(finalizationState_==FinalizationState::Completed)
            return refuse("the previous run is finalised but not cleared —"
                          " submit or reset it before starting a new run");
        if(finalizationState_==FinalizationState::RetryableError)
            return refuse("finalising the previous run failed —"
                          " press Finish to retry it, then start again");
    }
    if (!idSafe(request.mapId) || !idSafe(request.tractorId) ||
        !idSafe(request.implementId) || request.teamName.empty() ||
        request.teamName.size() > 80) {
        setPhase(Phase::Failed, "invalid team or selection identifier");
        return refuse("invalid team or selection identifier");
    }
    setPhase(Phase::Starting);
    Catalog catalog;
    ApiResult r = api_.catalog(catalog);
    const CatalogItem* tractor = find(catalog.tractors, request.tractorId);
    // 프리플라이트 실패도 스냅샷에만 적으면 화면에 도달하지 못한다. 2.5초 주기
    // 새로고침이 setPhase(Idle) 로 error 를 지우는데, 실측하면 Failed 프레임
    // 두 장 뒤 바로 err="" 가 온다 — 사람 눈에는 아무 일도 없었던 것과 같다.
    // 그래서 스냅샷과 별개로, 사용자가 닫을 때까지 남는 채널로도 내보낸다.
    //
    // A preflight failure written only into the snapshot never reaches the
    // screen: the 2.5 s refresh calls setPhase(Idle), which clears error, and
    // measured on the device err="" arrives two frames after Failed — which
    // looks exactly like nothing happened. Report it on the channel the UI keeps
    // until dismissed as well as in the snapshot.
    auto reject=[&](const std::string& reason) {
        setPhase(Phase::Failed,"start preflight: "+reason);
        return refuse("start preflight: "+reason);
    };
    bool aligned=false;
    if(r.ok && (catalog.selection.mapId!=request.mapId
        || catalog.selection.tractorId!=request.tractorId
        || catalog.selection.implementId!=request.implementId)) {
        // ┌─ 한국어 ───────────────────────────────────────────────────────┐
        // 라이브 선택은 앱 몰래 흘러내린다: 시뮬레이터 페이지를 새로 열면
        // 작업기가 해제되고, refresh 도 규약상 작업기를 내려놓는다 (Issue #9
        // §2.19). 예전에는 이 상태에서 Start 가 "selected IDs mismatch" 로
        // 거절됐고, 빠져나가는 길은 드롭다운을 두 번 토글해 set_sim_setup 을
        // 다시 보내는 것뿐이었는데 화면에는 그런 안내가 없다. 사용자는 방금
        // 이 Start 요청으로 원하는 설정을 정확히 명명했으므로, applySetup 과
        // 같은 방식(선택 적용 + 신선한 표본으로 확인)으로 여기서 직접 다시
        // 맞춘다. 정렬이 끝나도 아래의 프리플라이트 전체가 다시 검증한다.
        // └────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────┐
        // The live selection drifts behind the app's back: reopening the
        // simulator page drops the implement, and refresh drops it by contract
        // (Issue #9 §2.19). Start used to refuse this state with "selected IDs
        // mismatch", and the only way out was toggling a dropdown twice to
        // resend set_sim_setup — which nothing on screen suggests. The operator
        // has just named the exact setup in this Start request, so realign it
        // here the same way applySetup does (apply, then confirm on fresh
        // samples). The full preflight below still re-verifies the result.
        // └────────────────────────────────────────────────────────────────┘
        if(!find(catalog.maps,request.mapId))
            return reject("selected map is absent from live catalog: "+request.mapId);
        if(!tractor)
            return reject("selected tractor is absent from live catalog: "+request.tractorId);
        if(!find(catalog.implements,request.implementId))
            return reject("selected implement is absent from live catalog: "+request.implementId);
        logLine("start: realigning simulator selection to "+request.mapId+"/"
            +request.tractorId+"/"+request.implementId+" (observed "
            +catalog.selection.mapId+"/"+catalog.selection.tractorId+"/"
            +catalog.selection.implementId+")");
        auto alignOne=[&](const std::string& wanted,
                          const std::function<ApiResult()>& apply,
                          const std::function<bool(const Selection&)>& exact,
                          const char* label)->bool {
            constexpr int MAX_CONFLICT_RETRIES=20;
            for(int attempt=0;;++attempt) {
                const ApiResult applied=apply();
                if(applied.ok) break;
                if(applied.status!=409 || attempt>=MAX_CONFLICT_RETRIES) {
                    reject(std::string("start setup alignment: ")+label+": "
                        +applied.error);
                    return false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            for(int attempt=0;attempt<30;++attempt) {
                Selection current;
                const ApiResult state=api_.selection(current);
                if(!state.ok) {
                    reject(std::string("start setup alignment: ")+label
                        +" confirmation: "+state.error);
                    return false;
                }
                if(fresh(current) && current.stopped && exact(current)) {
                    catalog.selection=current;
                    return true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            reject(std::string("start setup alignment: ")+label
                +" confirmation timed out for "+wanted);
            return false;
        };
        if(catalog.selection.mapId!=request.mapId) {
            if(cancelled("start setup alignment")) return false;
            if(!alignOne(request.mapId,[&]{return api_.selectMap(request.mapId);},
                [&](const Selection& s){return s.mapId==request.mapId;},
                "map selection")) return false;
        }
        if(catalog.selection.tractorId!=request.tractorId) {
            if(cancelled("start setup alignment")) return false;
            if(!alignOne(request.tractorId,
                [&]{return api_.selectTractor(request.tractorId);},
                [&](const Selection& s){return s.mapId==request.mapId
                    && s.tractorId==request.tractorId;},
                "tractor selection")) return false;
        }
        if(catalog.selection.implementId!=request.implementId) {
            if(cancelled("start setup alignment")) return false;
            if(!alignOne(request.implementId,
                [&]{return api_.selectImplement(request.implementId);},
                [&](const Selection& s){return s.mapId==request.mapId
                    && s.tractorId==request.tractorId
                    && s.implementId==request.implementId;},
                "implement selection")) return false;
        }
        aligned=true;
    }
    bool setupMatches=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        const bool simulatorAgrees=r.ok
            && catalog.selection.mapId==request.mapId
            && catalog.selection.tractorId==request.tractorId
            && catalog.selection.implementId==request.implementId;
        if(snapshot_.confirmedSetup.mapId.empty() && simulatorAgrees) {
            snapshot_.confirmedSetup=request;
            snapshot_.desiredSetup=request;
        }
        // 반영이 끝난 뒤 남은 낡은 "의도" 때문에 Start 를 영영 막지 않는다.
        //
        // desiredSetup 은 요청한 설정, confirmedSetup 은 반영된 설정이다.
        // set_sim_setup 이 중간에 실패하면 desired 만 앞서 나간 채 굳고, 그
        // 뒤로는 confirmed 와 시뮬레이터가 둘 다 요청과 같아도 Start 가 계속
        // 거절된다(실측: desired=plow, confirmed=none, 시뮬레이터=none, UI=none).
        // 빠져나갈 길은 드롭다운을 두 번 토글해 set_sim_setup 을 다시 보내는
        // 것뿐인데 화면에는 그런 안내가 없다. confirmed 와 **라이브 시뮬레이터**가
        // 모두 요청과 일치하면 그 설정은 이미 반영된 것이므로 — 바로 위에서 빈
        // confirmedSetup 을 채울 때 믿는 근거와 같다 — 의도를 현재 상태로 맞춘다.
        // 둘 중 하나라도 어긋나면 종전대로 거절한다.
        //
        // Do not let stale intent block Start forever. desiredSetup is what was
        // asked for, confirmedSetup is what was applied. A set_sim_setup that
        // fails part-way leaves desired running ahead, and from then on Start is
        // refused even though confirmed and the simulator both equal the request
        // (measured: desired=plow, confirmed=none, simulator=none, UI=none). The
        // only way out was toggling a dropdown twice to resend set_sim_setup,
        // which nothing on screen suggests. When confirmed *and the live
        // simulator* both match the request the setup is applied — the same
        // evidence trusted just above to fill an empty confirmedSetup — so
        // realign the intent. If either disagrees, refuse exactly as before.
        if(simulatorAgrees
            && snapshot_.confirmedSetup.mapId==request.mapId
            && snapshot_.confirmedSetup.tractorId==request.tractorId
            && snapshot_.confirmedSetup.implementId==request.implementId) {
            snapshot_.desiredSetup=request;
        }
        // 위의 정렬이 바로 이 요청을 적용하고 신선한 표본으로 확인까지 끝냈다 —
        // applySetup 이 신뢰하는 것과 같은 증거이므로 의도와 반영 기록을 모두
        // 요청에 맞춘다.
        // The alignment above applied and confirmed exactly this request on
        // fresh samples — the same evidence applySetup trusts — so both the
        // intent and the applied record follow it.
        if(aligned && simulatorAgrees) {
            snapshot_.confirmedSetup=request;
            snapshot_.desiredSetup=request;
        }
        const StartRequest& desired=snapshot_.desiredSetup;
        const StartRequest& confirmed=snapshot_.confirmedSetup;
        setupMatches=desired.mapId==request.mapId
            && desired.tractorId==request.tractorId
            && desired.implementId==request.implementId
            && confirmed.mapId==request.mapId
            && confirmed.tractorId==request.tractorId
            && confirmed.implementId==request.implementId;
    }
    if(!r.ok)return reject("simulator catalog unavailable: "+r.error);
    const std::string stopIssue=physicalStopIssue(catalog.selection,request);
    if(!stopIssue.empty())return reject(stopIssue);
    if(!setupMatches)return reject("desired and confirmed simulator setup do not match Start request");
    if(!find(catalog.maps,request.mapId))return reject("selected map is absent from live catalog: "+request.mapId);
    if(!tractor)return reject("selected tractor is absent from live catalog: "+request.tractorId);
    if(!tractor->geometry.valid())return reject("selected tractor geometry is invalid: "+request.tractorId);
    if(!find(catalog.implements,request.implementId))
        return reject("selected implement is absent from live catalog: "+request.implementId);
    if(!runControl_.readyToStart())
        return reject("controller is not ready: require loaded path, GPS sample, and track origin");
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.catalog = catalog;
        snapshot_.selectionObservedAt=std::chrono::steady_clock::now();
        snapshot_.teamName = request.teamName;
        snapshot_.recordingId.clear();
        snapshot_.recordingSha256.clear();
        ++runGeneration_;
        snapshot_.runGeneration=runGeneration_;
        const auto epochMs=std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        archiveRunId_=std::to_string(epochMs)+"-"+std::to_string(runGeneration_);
        recordingStartIssued_=false;
        recordingStartGeneration_=0;
        recordingOwned_=false;
        recordingGeneration_=0;
        finalizationState_ = FinalizationState::Idle;
        snapshot_.finalization = finalizationState_;
        finalizationShouldFinish_=false;
    }

    auto waitSelection = [&](const std::function<bool(const Selection&)>& accepted,
                             const char* stage) {
        for (int attempt=0;attempt<30;++attempt) {
            if (cancelled(stage)) return false;
            Selection state;
            const ApiResult result=api_.selection(state);
            if (!result.ok) {
                compensate(); setPhase(Phase::Failed,std::string(stage)+": "+result.error);
                return false;
            }
            if (fresh(state) && accepted(state)) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        compensate(); setPhase(Phase::Failed,std::string(stage)+": confirmation timed out");
        return false;
    };
    runControl_.resetRun();
    runControl_.installRunGeometry(tractor->geometry);
    if (cancelled("recording reset")) return false;
    r = api_.recordingReset();
    if (!r.ok || r.status!=200) {
        compensate();
        setPhase(Phase::Failed,"recording reset requires HTTP 200");
        return false;
    }
    if (cancelled("recording start")) return false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        // Every POST is ambiguous until reconciled. Claim the cleanup
        // obligation before entering the HTTP call so timeouts and late server
        // application cannot escape the generation owner's Stop.
        recordingStartIssued_=true;
        recordingStartGeneration_=runGeneration_;
    }
    r = api_.recordingStart();
    if (!r.ok && r.status!=409) {
        compensate(); setPhase(Phase::Failed, "recording start: " + r.error); return false;
    }
    bool recorderConfirmed=false;
    for (int attempt=0;attempt<30;++attempt) {
        if (cancelled("recording confirmation")) return false;
        RecorderState recorder;
        r=api_.recordingState(recorder);
        if (!r.ok) {
            compensate();
            setPhase(Phase::Failed,"recording confirmation: "+r.error);
            return false;
        }
        if (cancelled("recording confirmation")) return false;
        if (!recorder.present) {
            // The recorder we just started has not surfaced yet. Keep waiting
            // for it — here the absence is transient, unlike at Finish where it
            // is the answer.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        if (recorder.live && recorder.snapshotAgeMs>=0
            && recorder.snapshotAgeMs<=MAX_SNAPSHOT_AGE_MS
            && recorder.recording) {
            recorderConfirmed=true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!recorderConfirmed) {
        compensate();
        setPhase(Phase::Failed,
            "recording confirmation timed out: no fresh recording state");
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        // Reset alone cannot identify which recorder buffer would be fetched,
        // and a successful Start response still is not live-state evidence.
        // This generation owns the recording only after a fresh confirmation.
        recordingOwned_=true;
        recordingGeneration_=runGeneration_;
    }
    Selection preEngine;
    r=api_.selection(preEngine);
    const std::string preEngineIssue=r.ok
        ? physicalStopIssue(preEngine,request) : std::string{};
    if(!r.ok || !preEngineIssue.empty()) {
        const std::string reason=!r.ok ? "selection unavailable: "+r.error
            : preEngineIssue;
        compensate();setPhase(Phase::Failed,"pre-engine physical-stop confirmation: "+reason);
        return false;
    }
    const long long before = preEngine.simElapsedMs;
    if (cancelled("engine start")) return false;
    r=api_.engine(true);
    if(!r.ok) {
        compensate(); setPhase(Phase::Failed,"engine start: "+r.error); return false;
    }
    if (!waitSelection([&](const Selection& state) {
            return state.reactRunning && state.simElapsedMs>before;
        },"engine confirmation")) return false;
    if (!runControl_.enterRunning()) {
        compensate(); setPhase(Phase::Failed, "controller refused Running"); return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        logLine("run started: team=\""+snapshot_.teamName+"\" runId="+archiveRunId_
            +" setup="+request.mapId+"/"+request.tractorId+"/"+request.implementId);
    }
    setPhase(Phase::Running);
    return true;
}

bool RunOrchestrator::cancelled(const char* stage)
{
    if(shuttingDown()) {
        (void)finishImpl(false,true);
        setPhase(Phase::Failed,std::string(stage)+": Start cancelled by shutdown");
        return true;
    }
    if (!runControl_.startCancelled()) return false;
    // The Start worker owns its generation until compensation and optional
    // partial archive complete. A queued Finish later observes Completed or
    // RetryableError and cannot create a second archive.
    (void)finish(false);
    setPhase(Phase::Failed,std::string(stage)+": Start cancelled by Finish");
    return true;
}

bool RunOrchestrator::recAcceptable(const Recording& rec, std::string& reason)
{
    // 나이는 페이지의 발행 최신성일 뿐, 서버가 들고 있는 CSV 의 완결성과는
    // 무관하다. 루프가 죽은 것이 증명되면 그 바이트는 더 늘어날 수 없으므로
    // 최종본이다. 예전에는 여기서 "recording snapshot is stale" 로 되돌아가
    // 이미 정지시킨 레코더를 아카이브하지 못했다.
    //
    // The age reports the page's publish recency, not the integrity of the CSV
    // the server holds. Once the loop is provably down those bytes can no
    // longer grow, so they are final. This used to bounce back as "recording
    // snapshot is stale" and leave an already-stopped recorder unarchived.
    const bool freshSnapshot = rec.live && rec.snapshotAgeMs >= 0
        && rec.snapshotAgeMs <= MAX_SNAPSHOT_AGE_MS;
    const bool loopDown = simulatorLoopDown(rec.snapshotAgeMs);
    if (!freshSnapshot && !loopDown) {
        reason = "recording snapshot is stale"; return false;
    }
    // 같은 이유로 recording 플래그도 루프가 죽었으면 믿을 수 없다. 얼어붙은
    // true 하나 때문에 이미 정지시킨 기록을 "recorder still running" 으로
    // 되돌려 보관에 실패하던 자리다. 서 있는 루프는 행을 더할 수 없다.
    //
    // For the same reason the recording flag is worthless once the loop is
    // down. One frozen true used to bounce an already-stopped recording back as
    // "recorder still running" and fail the archive. A stopped loop cannot
    // append a row.
    if (rec.recording && !loopDown) { reason = "recorder still running"; return false; }
    if (rec.bytes.empty() || rec.bytes.size() > MAX_RECORDING_BYTES) {
        reason = "recording size is invalid"; return false;
    }
    std::string type = rec.contentType;
    std::transform(type.begin(), type.end(), type.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (type.find("text/csv") != 0) { reason = "recording is not text/csv"; return false; }
    if (rec.disposition.find("attachment") == std::string::npos) {
        reason = "recording disposition is missing"; return false;
    }
    return true;
}

RunOrchestrator::PersistOutcome RunOrchestrator::persistRecording()
{
    Recording rec;
    const ApiResult get = api_.recording(rec);
    // 404 는 "기록이 아예 없다"는 시뮬레이터의 확답이다 (Issue #9 §2.10).
    // 재시도 가능한 실패로 올리면 Finish 재시도가 같은 404 만 영원히 다시
    // 받는다 — 호출자가 종결 처리한다.
    // A 404 is the simulator's definite answer that no recording exists
    // (Issue #9 §2.10). Reported as retryable, every Finish retry just reads
    // the same 404 forever — the caller handles it terminally.
    if (!get.ok && get.status==404) return PersistOutcome::Lost;
    std::string reason;
    if (!get.ok || !recAcceptable(rec, reason)) {
        setPhase(Phase::Failed, get.ok ? reason : get.error);
        return PersistOutcome::Retryable;
    }
    std::string id, sha;
    std::string team,runId;
    { std::lock_guard<std::mutex> lock(mutex_);
      team = snapshot_.teamName; runId=archiveRunId_; }
    const ApiResult stored = store_.persist(team,runId,rec.bytes,id,sha);
    if (!stored.ok) { setPhase(Phase::Failed, stored.error); return PersistOutcome::Retryable; }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.recordingId = id;
        snapshot_.recordingSha256 = sha;
    }
    logLine("recording archived: "+id+" sha256="+sha
        +" bytes="+std::to_string(rec.bytes.size()));
    return PersistOutcome::Persisted;
}

void RunOrchestrator::compensate()
{
    bool ownsRecording=false;
    bool cleanupRequired=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ownsRecording=recordingOwned_ && recordingGeneration_==runGeneration_;
        cleanupRequired=recordingStartIssued_
            && recordingStartGeneration_==runGeneration_;
    }
    if(ownsRecording || cleanupRequired) {
        // 성공한 finishImpl 은 엔진까지 내린다. 실패했다면 성립하지 않은 주행이
        // 시동만 걸린 채 남으므로 여기서 확실히 끈다.
        // A successful finishImpl cuts the engine itself. If it failed, a run
        // that never came into being would be left idling, so cut it here.
        if(!finishImpl(false,shuttingDown())) (void)api_.engine(false);
        return;
    }
    runControl_.enterSafeIdle();
    (void)api_.engine(false);
}

bool RunOrchestrator::finalizationFailed(const std::string& error)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        finalizationState_=FinalizationState::RetryableError;
        snapshot_.finalization=finalizationState_;
    }
    logLine("finalize failed (retryable): "+error);
    setPhase(Phase::Failed,error);
    return false;
}

bool RunOrchestrator::finish(bool automatic)
{
    return finishImpl(automatic,false);
}

bool RunOrchestrator::confirmVehicleAtRest(bool duringShutdown)
{
    StartRequest expected;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        expected=snapshot_.confirmedSetup;
    }
    int consecutiveStopped=0;
    int consecutiveLoopDown=0;
    constexpr int REQUIRED_STOPPED_SAMPLES=3;
    // 루프 사망은 나이 자체가 이미 15 초짜리 증거다. 연속 요구는 한 번 튄 값을
    // 거르는 용도이므로 짧게 둔다.
    // The age alone is already fifteen seconds of evidence that the loop is
    // down; the consecutive requirement only filters a single garbage reading.
    constexpr int REQUIRED_LOOP_DOWN_SAMPLES=5;
    // 지령을 놓은 차량이 굴러 멈추기까지 기다린다. 예전 3 초 예산은 작업기를
    // 끌고 4.5 km/h 로 달리던 차량에게는 감속이 끝나기 전에 소진됐다.
    // Give the released vehicle time to roll to a stop. The former 3 s budget
    // expired before a 4.5 km/h run with an implement had finished decelerating.
    constexpr int MAX_ATTEMPTS=150;
    for(int attempt=0;attempt<MAX_ATTEMPTS;++attempt) {
        if(shuttingDown() && !duringShutdown)
            return finalizationFailed("vehicle stop confirmation cancelled by shutdown");
        Selection selection;
        const ApiResult state=api_.selection(selection);
        consecutiveStopped=state.ok
            && vehicleAtRestSampleAcceptable(selection,expected)
            ? consecutiveStopped+1 : 0;
        if(consecutiveStopped>=REQUIRED_STOPPED_SAMPLES) return true;
        // ┌─ 한국어 ────────────────────────────────────────┐
        // 시뮬레이터 페이지가 죽으면 신선한 샘플은 영영 오지 않는다. 예전에는
        // 여기서 15 초를 채우고 "physical stop confirmation timed out" 으로
        // 끝났고, 그 뒤 단계인 레코더 정지에 아예 도달하지 못해 주행이 끝난
        // 뒤에도 기록이 계속 돌았다. API 서버가 응답했는데 그 응답이 루프가
        // 서 있다고 말한다면, 서 있는 루프는 차량을 움직일 수 없다 — 그것을
        // 정지 확인으로 받고 남은 종결 단계(레코더 정지, 아카이브)를 진행한다.
        // 이것은 confirmEngineOff 가 이미 쓰는 논리와 같다.
        // └────────────────────────────────────────────┘
        // ┌─ English ───────────────────────────────────────┐
        // A dead simulator page never delivers another fresh sample. This loop
        // used to burn its 15 s and fail with "physical stop confirmation timed
        // out", never reaching the recorder stop that follows, which left the
        // recording running long after the run was over. If the API server does
        // answer and its answer says the loop is down, then a stopped loop
        // cannot be moving the vehicle - take that as confirmation and carry on
        // with the remaining steps (stop the recorder, archive). This is the
        // same reasoning confirmEngineOff already relies on.
        // └────────────────────────────────────────────┘
        consecutiveLoopDown=state.ok && simulatorLoopDown(selection.snapshotAgeMs)
            ? consecutiveLoopDown+1 : 0;
        if(consecutiveLoopDown>=REQUIRED_LOOP_DOWN_SAMPLES) {
            logLine("vehicle stop unobservable: simulator snapshot is "
                +std::to_string(selection.snapshotAgeMs)
                +" ms old, so the physics loop is down and cannot move the "
                "vehicle; continuing finalization");
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return finalizationFailed(
        "vehicle stop confirmation timed out: require three consecutive fresh "
        "selection samples with stopped=true and exact setup IDs");
}

bool RunOrchestrator::confirmEngineOff(bool duringShutdown)
{
    StartRequest expected;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        expected=snapshot_.confirmedSetup;
    }
    // POST /api/simulator/engine 은 왕복하지 않는다 (Issue #9 §2.18) — 200 은
    // 명령을 전달했다는 뜻뿐이라 selection 으로 다시 확인해야 한다. 다만 엔진을
    // 끄면 시뮬레이터의 물리 루프가 함께 멈추고(loopDiag.stopFrom 이
    // onBackendCommand -> stop() 을 가리킨다), 화면 탭이 가려져 있으면 스냅샷
    // 발행까지 멈춰 관측 자체가 끊긴다. 스냅샷이 끊기는 것은 루프가 섰다는
    // 증거이므로 정지 확인으로 받는다 — 서 있는 루프는 차량을 움직일 수 없다.
    //
    // The engine POST does not round-trip: 200 only means the command was
    // delivered, so the result has to be read back from selection. But cutting
    // the engine also stops the simulator's physics loop, and with the page
    // hidden that silences the snapshot publisher too. A simulator that stops
    // reporting is itself proof the loop is down, and a stopped loop cannot
    // move the vehicle, so that counts as confirmation.
    int unobservable=0;
    constexpr int REQUIRED_UNOBSERVABLE_SAMPLES=5;
    constexpr int MAX_ATTEMPTS=50;
    for(int attempt=0;attempt<MAX_ATTEMPTS;++attempt) {
        if(shuttingDown() && !duringShutdown)
            return finalizationFailed("engine off confirmation cancelled by shutdown");
        Selection selection;
        const ApiResult state=api_.selection(selection);
        if(state.ok && fresh(selection)) {
            if(!selection.reactRunning) return true;
            unobservable=0;
            const ApiResult repeated=api_.engine(false);
            if(!repeated.ok && repeated.status!=409)
                return finalizationFailed("repeated engine off: "+repeated.error);
        } else if(++unobservable>=REQUIRED_UNOBSERVABLE_SAMPLES) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return finalizationFailed(
        "engine off confirmation timed out: the simulator kept reporting "
        "reactRunning=true after repeated engine off requests");
}

bool RunOrchestrator::finishImpl(bool, bool duringShutdown)
{
    if(shuttingDown() && !duringShutdown) return false;
    bool retrying=false;
    bool wasRunning=false;
    bool ownsRecording=false;
    bool cleanupRequired=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(finalizationState_==FinalizationState::Completed) return true;
        if(finalizationState_==FinalizationState::InProgress) return false;
        ownsRecording=recordingOwned_ && recordingGeneration_==runGeneration_;
        cleanupRequired=recordingStartIssued_
            && recordingStartGeneration_==runGeneration_;
        if(!ownsRecording && !cleanupRequired) {
            // A Start cancelled before fresh recorder confirmation has no
            // generation-owned bytes. Do not stop or fetch an ambiguous
            // pre-existing/partially-started recorder buffer.
            archiveRunId_.clear();
            finalizationState_=FinalizationState::Idle;
            snapshot_.finalization=finalizationState_;
        } else {
            retrying=finalizationState_==FinalizationState::RetryableError;
            wasRunning=snapshot_.phase==Phase::Running || runControl_.runActive();
            if(!retrying) finalizationShouldFinish_=wasRunning;
            else wasRunning=finalizationShouldFinish_;
            finalizationState_=FinalizationState::InProgress;
            finalizationGeneration_=runGeneration_;
            snapshot_.finalization=finalizationState_;
        }
    }
    if(!ownsRecording && !cleanupRequired) {
        runControl_.enterSafeIdle();
        (void)api_.engine(false);
        setPhase(Phase::Idle);
        return true;
    }
    logLine(std::string("finalize begin")+(retrying?" (retry)":"")
        +(ownsRecording?"":" (cleanup only, no owned recording)"));
    setPhase(Phase::Finishing);
    // Safe control authority and zero/default outputs precede every external
    // finalization request. The SDK setters provide no actuator CAN ACK, so
    // physical stop is confirmed independently through Issue #9 selection.
    runControl_.enterSafeIdle();
    // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
    // 엔진을 먼저 끄면 안 된다. 시뮬레이터는 엔진 정지를 물리 루프 정지로
    // 구현하고, 스냅샷도 그 루프가 발행한다. 그래서 엔진을 끈 직후부터
    // selection 은 마지막 값(달리던 중이라면 stopped=false)에 얼어붙고, 앱은
    // 자기가 방금 명령한 정지를 영영 관측할 수 없다. 실제로 이 순서 때문에
    // Finish 가 "physical stop confirmation timed out" 으로 끝났다.
    // 정지 확인 -> 기록 정지 -> 엔진 정지 순서로 두면 관측이 필요한 두 단계가
    // 모두 루프가 살아 있는 동안 끝난다.
    // └────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────┐
    // The engine must not go off first. The simulator implements engine-off as
    // stopping the physics loop, and that same loop publishes the snapshots, so
    // from the moment the engine is cut selection freezes at its last value
    // (stopped=false if the vehicle was moving) and the app can never observe
    // the stop it just commanded. This ordering is what made Finish end in
    // "physical stop confirmation timed out". Confirming the stop, then the
    // recorder, and only then cutting the engine keeps both observations inside
    // the window where the loop is still alive.
    // └────────────────────────────────────────────────────────────────────┘
    if(!confirmVehicleAtRest(duringShutdown)) return false;
    ApiResult stop = api_.recordingStop();
    if (!stop.ok && stop.status != 409)
        return finalizationFailed("recording stop: "+stop.error);
    RecorderState recorder;
    // ┌─ 한국어 ────────────────────────────────────────┐
    // 409 {"error":"not recording"} 는 API 서버 자신의 확답이다: 지금 돌고 있는
    // 기록이 없다. 예전에는 이것을 "무해하니 무시" 정도로만 다뤘고, 확인은
    // 아래 헤더에만 맡겼다. 그런데 헤더는 서버 상태가 아니다 (아래 참고) —
    // 페이지가 얼면 서버가 409 로 "안 돌고 있다" 고 답하는 동안 헤더는
    // X-Rec-Recording: true 를 계속 내놓는다. 실측된 이 모순 때문에 종결이
    // stop 을 30 번 재발행하고 "recording stop confirmation timed out" 으로
    // 끝났다. 답을 들었으면 그것이 확인이다.
    // └────────────────────────────────────────────┘
    // ┌─ English ───────────────────────────────────────┐
    // A 409 {"error":"not recording"} is the API server's own answer: nothing is
    // recording. This used to be merely tolerated as harmless, leaving the
    // confirmation to the headers below - but those headers are not server state
    // (see below). With the page frozen the server answers 409 "not recording"
    // while the header keeps reporting X-Rec-Recording: true. That measured
    // contradiction made finalization re-issue the stop thirty times and end in
    // "recording stop confirmation timed out". An answer received is a
    // confirmation.
    // └────────────────────────────────────────────┘
    bool stoppedConfirmed=stop.status==409;
    bool absentNow=false;
    int consecutiveFreshStopped=0;
    constexpr int REQUIRED_STOPPED_SAMPLES=3;
    // 확답을 이미 받았으면 폴링하지 않는다.
    // No polling once the answer is already in hand.
    for(int attempt=0;attempt<30 && !stoppedConfirmed;++attempt) {
        if(shuttingDown() && !duringShutdown)
            return finalizationFailed("finalization cancelled by shutdown");
        const ApiResult stateResult=api_.recordingState(recorder);
        if(!stateResult.ok)
            return finalizationFailed("recording stop confirmation: "+stateResult.error);
        if(!recorder.present) {
            // 기록이 아예 없다는 것은 확답이지만, 응답이 늦은 rec/start 가 아직
            // 도착하지 않았을 수도 있다. 그래서 여기서 곧장 확정하지 않고 창을
            // 끝까지 돌려 늦게 뜬 기록을 잡아 세운다. 창이 끝날 때까지도 없으면
            // 그때 정지로 인정한다 — 없는 기록은 돌고 있지 않다.
            //
            // "No recording" is a definite answer, but a rec/start whose response
            // was lost may still be about to land. So the window is run to the end
            // to catch and stop a late recorder instead of concluding here; if it
            // is still absent when the window closes, that counts as stopped —
            // a recording that does not exist is not running.
            absentNow=true;
            consecutiveFreshStopped=0;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        absentNow=false;
        const bool fresh=recorder.live && recorder.snapshotAgeMs>=0
            && recorder.snapshotAgeMs<=MAX_SNAPSHOT_AGE_MS;
        // ┌─ 한국어 ────────────────────────────────────────┐
        // X-Rec-Recording 은 페이지가 발행하는 스냅샷을 타고 온다. 서버 상태가
        // 아니다 — X-Rec-Elapsed-Ms 가 주행이 끝난 값에 얼어붙은 채 함께
        // 나오는 것이 그 증거다. 그러므로 루프가 죽었다는 것이 증명된 순간
        // 이 플래그는 아무것도 말해 주지 않는다. 그때의 진실은 rec/stop 이
        // 돌려주는 상태 코드뿐이다.
        // └────────────────────────────────────────────┘
        // ┌─ English ───────────────────────────────────────┐
        // X-Rec-Recording rides the snapshot the page publishes; it is not
        // server state - the proof is X-Rec-Elapsed-Ms arriving frozen at the
        // value it held when the run ended. So once the loop is provably down
        // this flag says nothing at all, and the only truth left is the status
        // code rec/stop returns.
        // └────────────────────────────────────────────┘
        const bool loopDown=simulatorLoopDown(recorder.snapshotAgeMs);
        const bool usable=fresh || loopDown;
        if(usable && recorder.recording) {
            consecutiveFreshStopped=0;
            const ApiResult repeatedStop=api_.recordingStop();
            if(!repeatedStop.ok && repeatedStop.status!=409)
                return finalizationFailed("repeated recording stop: "+repeatedStop.error);
            if(repeatedStop.status==409) { stoppedConfirmed=true; break; }
        } else if(usable) {
            ++consecutiveFreshStopped;
            if(consecutiveFreshStopped>=REQUIRED_STOPPED_SAMPLES) {
                stoppedConfirmed=true;
                break;
            }
        } else {
            consecutiveFreshStopped=0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(!stoppedConfirmed && absentNow) stoppedConfirmed=true;
    if(!stoppedConfirmed) {
        return finalizationFailed("recording stop confirmation timed out");
    }
    // 엔진 정지는 관측이 필요한 모든 단계가 끝난 뒤에만 보낸다. 아카이브까지
    // 마친 다음이라, 실패해 재시도하더라도 시뮬레이터는 여전히 살아 있다.
    // Cut the engine only after every step that needs to observe the simulator
    // has finished. Doing it after the archive also means a failed finalization
    // can be retried against a simulator that is still alive.
    const auto stopEngine=[&]() {
        const ApiResult engine=api_.engine(false);
        if(!engine.ok && engine.status!=409)
            return finalizationFailed("engine off: "+engine.error);
        return confirmEngineOff(duringShutdown);
    };
    if(!ownsRecording) {
        if(!stopEngine()) return false;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            recordingStartIssued_=false;
            recordingStartGeneration_=0;
            archiveRunId_.clear();
            finalizationState_=FinalizationState::Idle;
            snapshot_.finalization=finalizationState_;
        }
        setPhase(Phase::Idle);
        return true;
    }
    const PersistOutcome persisted=persistRecording();
    if(persisted==PersistOutcome::Lost) {
        // ┌─ 한국어 ───────────────────────────────────────────────────────┐
        // 시작 때 확인까지 마친 기록이 사라졌다: 주행 중 시뮬레이터 페이지가
        // 다시 로드됐거나 다른 클라이언트가 레코더를 리셋한 경우다. 바이트는
        // 이미 없으므로 어떤 재시도도 복구할 수 없다. 예전에는 이걸
        // RetryableError 로 두어 Finish 재시도가 영원히 같은 404 를 받았고,
        // 앱을 재시작해야만 풀렸다 (그리고 재시작은 RDDF 재전송까지 불렀다).
        // 종결로 선언한다: 소유권과 종결 상태를 지워 다음 Start 를 허용하고,
        // 엔진을 최선껏 내리고, 스냅샷 밖 채널로도 알린다.
        // └────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────┐
        // A recording this generation confirmed at Start has vanished: the
        // simulator page reloaded mid-run or another client reset the
        // recorder. The bytes are gone, so no retry can recover them. This
        // used to sit in RetryableError, every Finish retry re-reading the
        // same 404 until an app restart (which then demanded RDDF
        // re-delivery). Declare it terminal instead: clear ownership and the
        // finalization state so the next Start is admitted, cut the engine
        // best-effort, and report on the durable channel too.
        // └────────────────────────────────────────────────────────────────┘
        {
            std::lock_guard<std::mutex> lock(mutex_);
            recordingOwned_=false;
            recordingGeneration_=0;
            recordingStartIssued_=false;
            recordingStartGeneration_=0;
            archiveRunId_.clear();
            finalizationShouldFinish_=false;
            finalizationState_=FinalizationState::Idle;
            snapshot_.finalization=finalizationState_;
            snapshot_.recordingId.clear();
            snapshot_.recordingSha256.clear();
        }
        (void)api_.engine(false);
        for(int attempt=0;attempt<20;++attempt) {
            Selection state;
            const ApiResult read=api_.selection(state);
            if(read.ok && fresh(state)) {
                if(!state.reactRunning) break;
                (void)api_.engine(false);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        const std::string message=
            "the simulator recorder no longer holds this run (no recording"
            " exists — the simulator page reloaded or the recorder was reset"
            " mid-run), so there is nothing to archive and no retry can"
            " recover it; the run has been cleared — start a new run";
        logLine("finalize terminal: recording lost");
        setPhase(Phase::Failed,message);
        if(notify_) notify_("Finish: "+message);
        return false;
    }
    if(persisted!=PersistOutcome::Persisted) {
        const std::string error=snapshot().error;
        return finalizationFailed(error.empty()?"recording persistence failed":error);
    }
    if(!stopEngine()) return false;
    bool generationChanged=false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        generationChanged=finalizationGeneration_!=runGeneration_;
        if(!generationChanged) {
        finalizationState_=FinalizationState::Completed;
        snapshot_.finalization=finalizationState_;
        snapshot_.postRun=PostRunState::AwaitingAction;
        recordingOwned_=false;
        recordingStartIssued_=false;
        }
    }
    if(generationChanged)
        return finalizationFailed("run generation changed during finalization");
    if(wasRunning) runControl_.enterFinished();
    logLine("finalize completed; awaiting submit or reset");
    setPhase(Phase::Idle);
    if(finalized_) {
        std::string team,runId;
        { std::lock_guard<std::mutex> lock(mutex_);
          team=snapshot_.teamName; runId=archiveRunId_; }
        finalized_(team,runId);
    }
    return true;
}

bool RunOrchestrator::beginResetCleanup(const std::string& runId)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(runControl_.runActive() || snapshot_.busy
            || snapshot_.recordingId!=runId
            || (finalizationState_!=FinalizationState::Completed
                && finalizationState_!=FinalizationState::RetryableError)
            || snapshot_.postRun==PostRunState::CaptureInProgress
            || snapshot_.postRun==PostRunState::ResetCleanupPending)
            return false;
        snapshot_.busy=true;
        snapshot_.error.clear();
        snapshot_.postRun=PostRunState::ResetCleanupPreparing;
        ++snapshot_.resetGeneration;
        resetPreparedGeneration_=0;
        purgeAllCleanup_=false;
        copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::beginPurgeAllCleanup()
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(runControl_.runActive() || snapshot_.busy
            || !snapshot_.recordingId.empty()
            || snapshot_.postRun==PostRunState::CaptureInProgress
            || snapshot_.postRun==PostRunState::ResetCleanupPreparing
            || snapshot_.postRun==PostRunState::ResetCleanupPending)
            return false;
        snapshot_.busy=true;
        snapshot_.error.clear();
        snapshot_.postRun=PostRunState::ResetCleanupPreparing;
        ++snapshot_.resetGeneration;
        purgeAllCleanup_=true;
        copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::noteResetCleanupPrepared(unsigned long long generation)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(snapshot_.postRun!=PostRunState::ResetCleanupPreparing
        || snapshot_.resetGeneration!=generation || generation==0)return false;
    resetPreparedGeneration_=generation;
    return true;
}

bool RunOrchestrator::failResetCleanup(const std::string& runId)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(snapshot_.recordingId!=runId
            || snapshot_.postRun!=PostRunState::ResetCleanupPreparing)return false;
        // 이미 다른 클라이언트가 아웃박스를 비웠다고 알려 왔고 리셋이 진행 중이면
        // 여기서 취소하지 않는다. 대시보드는 여러 개가 붙을 수 있고, 실제로 낡은
        // 탭 하나가 배수에 실패하는 바람에 정상 탭이 성립시킨 리셋이 통째로
        // 취소돼 다음 주행을 아예 시작할 수 없었다.
        //
        // Do not cancel a reset another client already admitted by reporting its
        // outbox drained. Several dashboards can be attached, and one stale tab
        // failing its drain was enough to cancel the reset a healthy tab had
        // just started — which left the next run impossible to begin.
        if(resetPreparedGeneration_!=0
            && resetPreparedGeneration_==snapshot_.resetGeneration)return true;
        snapshot_.busy=false;
        snapshot_.phase=Phase::Failed;
        snapshot_.postRun=PostRunState::ResetRetryable;
        snapshot_.error="browser reset cleanup timed out";
        copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::resetCompletedRun()
{
    if(shuttingDown()) return false;
    // 이 예산 하나로 절차 전체를 덮는다: 정지 확인 3 샘플 -> refresh -> 다시
    // 3 샘플 -> recordingReset -> 안정 확인. 시뮬레이터가 1 초에 한 번 보고하니
    // "연속 3 회" 하나만도 3 초가 넘게 걸릴 수 있어, 3.5 초로는 refresh 뒤 첫
    // 단계에서 반드시 시간이 끊겼다.
    //
    // One budget covers the whole procedure: three stop samples, refresh, three
    // more, recordingReset, then the settled check. The simulator reports once a
    // second, so a single "three consecutive" alone can take over three seconds
    // and the former 3.5 s always ran out at the first post-refresh step.
    const auto resetDeadline=std::chrono::steady_clock::now()
        +std::chrono::seconds(25);
    StartRequest expected;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(runControl_.runActive()
            || snapshot_.postRun!=PostRunState::ResetCleanupPreparing
            || (!purgeAllCleanup_
                && finalizationState_!=FinalizationState::Completed
                && finalizationState_!=FinalizationState::RetryableError)) {
            return false;
        }
        expected=snapshot_.confirmedSetup;
        if(expected.mapId.empty())expected.mapId=snapshot_.catalog.selection.mapId;
        if(expected.tractorId.empty())expected.tractorId=snapshot_.catalog.selection.tractorId;
        if(expected.implementId.empty())expected.implementId=snapshot_.catalog.selection.implementId;
        snapshot_.busy=true;
        snapshot_.error.clear();
    }
    setPhase(Phase::Refreshing);
    const auto fail=[&](const std::string& error) {
        logLine("run reset failed: "+error);
        Snapshot copy;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            snapshot_.busy=false;snapshot_.phase=Phase::Failed;snapshot_.error=error;
            snapshot_.postRun=PostRunState::ResetRetryable;
            copy=publishable(snapshot_);
        }
        if(publish_)publish_(copy);
        return false;
    };
    runControl_.enterSafeIdle();
    // Require safe authoritative state before destructive recorder reset.
    int safeSamples=0;
    while(std::chrono::steady_clock::now()<resetDeadline && safeSamples<3){Selection s;const ApiResult r=api_.selection(s);safeSamples=r.ok&&physicalStopSampleAcceptable(s,expected)?safeSamples+1:0;if(safeSamples<3)std::this_thread::sleep_for(std::chrono::milliseconds(100));}
    if(safeSamples<3)return fail("run reset requires a fresh stopped simulator");
    // Issue #9 reset contract: refresh simulator-owned state first, then reset
    // the recorder. Neither call starts the engine or controller.
    ApiResult result=api_.simulatorRefresh();
    if(!result.ok)return fail("simulator refresh failed: "+httpStatusCategory(result));

    // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
    // refresh 는 작업기를 내려놓는다 (Issue #9 §2.19 초기화 범위: 키 입력,
    // 히치, 작업기, 경작 자국). 그래서 refresh 뒤에도 주행 때 쓰던 작업기
    // 식별자를 계속 요구하면 그 조건은 영영 만족되지 않는다 — 리셋이 매번
    // "run reset confirmation timed out" 으로 끝났다. 맵과 트랙터는 그대로
    // 남으므로 그 둘을 확인하고, 작업기는 원래 값이든 해제든 모두 받는다.
    // └────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────┐
    // A refresh drops the implement (Issue #9 §2.19 resets key input, hitch,
    // implement and tilled ground). Continuing to require the run's implement
    // identifier afterwards is a condition that can never be met, and the reset
    // ended in "run reset confirmation timed out" every time. Map and tractor do
    // survive, so those are checked and either implement state is accepted.
    // └────────────────────────────────────────────────────────────────────┘
    const auto settledAfterRefresh=[&](const ApiResult& state,const Selection& s) {
        return state.ok && fresh(s) && s.stopped && !s.reactRunning
            && s.mapId==expected.mapId && s.tractorId==expected.tractorId
            && (s.implementId==expected.implementId || s.implementId=="none"
                || s.implementId.empty());
    };
    safeSamples=0;
    while(std::chrono::steady_clock::now()<resetDeadline && safeSamples<3) {
        Selection refreshed;
        const ApiResult state=api_.selection(refreshed);
        safeSamples=settledAfterRefresh(state,refreshed)?safeSamples+1:0;
        if(safeSamples<3)std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(safeSamples<3)
        return fail("post-refresh simulator selection did not become fresh and stopped");

    // A 503 is the documented transient while refreshed recorder state is
    // coming online. Retry it boundedly; every other non-200 is actionable and
    // must not be treated as a completed reset.
    for(;;) {
        result=api_.recordingReset();
        if(result.ok && result.status==200)break;
        const bool retryable=result.status==0 || result.status==503;
        if(!retryable || std::chrono::steady_clock::now()>=resetDeadline)
            return fail("recording reset requires HTTP 200; observed "+
                httpStatusCategory(result));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int stableSelection=0;
    while(std::chrono::steady_clock::now()<resetDeadline) {
        Selection selection;
        const ApiResult selected=api_.selection(selection);
        stableSelection=settledAfterRefresh(selected,selection)
            ? stableSelection+1:0;
        RecorderState recorder;
        const ApiResult rec=api_.recordingState(recorder);
        const bool resetConfirmed=rec.ok && !recorder.present;
        if(stableSelection>=3 && resetConfirmed) {
            const ApiResult purged=store_.purgeUnsubmitted();
            if(!purged.ok)return fail("unsubmitted recording purge: "+purged.error);
            logLine("run reset confirmed; awaiting browser cleanup acknowledgement");
            Snapshot copy;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                snapshot_.postRun=PostRunState::ResetCleanupPending;
                snapshot_.busy=true;snapshot_.phase=Phase::Refreshing;snapshot_.error.clear();
                snapshot_.catalog.selection=selection;
                snapshot_.selectionObservedAt=std::chrono::steady_clock::now();copy=publishable(snapshot_);
            }
            if(publish_)publish_(copy);
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.postRun=PostRunState::ResetRetryable;
    }
    return fail("run reset confirmation timed out");
}

bool RunOrchestrator::beginSealedCapture(const std::string& runId)
{
    // 봉인본을 받아 리더보드로 보내는 일은 전부 브라우저가 한다. 여기서 하는 일은
    // 그 사실을 화면과 재접속 복구에 알리는 상태 전이뿐이므로, 정확히 이 주행이
    // 맞기만 하면 승인한다. 예전에는 postRun 이 awaiting_action 일 때만
    // 받아들여서, 리셋이 한 번 실패해 reset_retryable 로 굳거나 캡처가 이미
    // 지나간 뒤에는 Submit 을 누를 때마다 거절돼 화면에 "sealed capture is not
    // authorized" 만 반복해서 떴다 — 제출할 방법이 아예 없었다.
    //
    // The browser does the whole submission: it fetches the sealed envelope and
    // posts it to the leaderboard. All this does is mirror that in the snapshot
    // for the screen and for reload-resume, so naming the exact run is enough to
    // be granted. Accepting only from awaiting_action meant that once a failed
    // reset had stuck postRun at reset_retryable — or once capture had moved on —
    // every Submit was refused with "sealed capture is not authorized" and there
    // was no path left to submit at all.
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(finalizationState_!=FinalizationState::Completed
            || snapshot_.recordingId!=runId)return false;
        if(snapshot_.postRun!=PostRunState::AwaitingAction
            && snapshot_.postRun!=PostRunState::ResetRetryable)return true;
        snapshot_.postRun=PostRunState::CaptureInProgress;copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::acknowledgeSealedCapture(const std::string& runId)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(snapshot_.recordingId!=runId)return false;
        // Browser acknowledgement frames are deliberately replayed after a
        // reconnect until the authoritative snapshot catches up.  Accept an
        // acknowledgement that has already been applied without publishing a
        // second transition or weakening the exact-run check.
        if(snapshot_.postRun==PostRunState::CaptureDurable
            || snapshot_.postRun==PostRunState::Submitted)return true;
        // 캡처는 UI 소유라 승인 전이 없이도 끝나 있을 수 있다(승인 프레임이
        // 유실되거나 백엔드가 그 사이 재시작한 경우). 봉인본을 들고 있다는
        // 보고는 그 자체로 사실이므로 awaiting_action 에서도 받는다.
        // Capture is UI-owned and can finish without the grant transition (a lost
        // frame, or a backend restart in between). Holding the envelope is a fact
        // on its own, so accept the report from awaiting_action too.
        if(snapshot_.postRun!=PostRunState::CaptureInProgress
            && snapshot_.postRun!=PostRunState::AwaitingAction)return false;
        snapshot_.postRun=PostRunState::CaptureDurable;copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::failSealedCapture(const std::string& runId)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(snapshot_.recordingId!=runId
            || snapshot_.postRun!=PostRunState::CaptureInProgress)return false;
        snapshot_.postRun=PostRunState::AwaitingAction;copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::markLeaderboardSubmitted(const std::string& runId,const std::string& verdict)
{
    if(verdict!="SCORED"&&verdict!="INVALID")return false;
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        // 현재 주행이 아닌 runId 는 보관된 과거 주행이다. 브라우저 아웃박스는
        // 봉인본을 들고 있다가 주행이 끝난 한참 뒤에 제출을 끝낼 수 있는데,
        // 그 보고를 "현재 주행이 아님"으로 거부하면 원장이 갱신되지 않아 CSV 가
        // 영원히 미제출로 남고 대시보드에는 오류 프레임까지 떴다. 주행 후 상태
        // 기계는 현재 주행만 다루므로, 과거 주행은 원장 기록만 하고 전이는
        // 만들지 않는다(원장은 집합 삽입이라 멱등하다).
        // A runId other than the current run names an archived one. The browser
        // outbox can finish submitting a stored envelope long after the run
        // ended; rejecting that report as "not the current run" left the ledger
        // stale — the CSV stayed listed as unsubmitted forever and the
        // dashboard received an error frame. The post-run state machine covers
        // only the current run, so archived runs get the ledger write and no
        // transition (the ledger is a set insert, hence idempotent).
        if(snapshot_.recordingId!=runId) {
            logLine("leaderboard verdict "+verdict+" recorded for archived run "+runId);
            return store_.markSubmitted(runId,verdict).ok;
        }
        // The browser persists a terminal result before sending this frame and
        // replays it until the backend snapshot confirms Submitted.  A lost
        // reply must therefore be an idempotent success, not an error.
        if(snapshot_.postRun==PostRunState::Submitted)return true;
        // 제출 판정은 브라우저가 리더보드에서 직접 받아온 결과다. capture_durable
        // 을 거쳐 왔는지는 우리가 알 바가 아니며, 요구했다가는 승인 전이가 없던
        // 제출이 원장에 영영 기록되지 않는다. 리셋이 도는 중에만 거절해 제출된
        // 보관본이 그 틈에 삭제되는 일을 막는다.
        // The verdict comes from the browser's own leaderboard exchange. Whether
        // it passed through capture_durable is not ours to require — demanding it
        // left every submission without a grant transition missing from the
        // ledger. Only a running reset refuses, so a submitted archive cannot be
        // purged in the gap.
        if(snapshot_.postRun==PostRunState::None
            || snapshot_.postRun==PostRunState::ResetCleanupPreparing
            || snapshot_.postRun==PostRunState::ResetCleanupPending)return false;
        // Keep the post-run transition and its durable ledger write mutually
        // exclusive with Reset so a submitted archive cannot be purged in the
        // gap between the two operations.
        if(!store_.markSubmitted(runId,verdict).ok)return false;
        logLine("leaderboard verdict "+verdict+" recorded for current run "+runId);
        snapshot_.postRun=PostRunState::Submitted;
        copy=publishable(snapshot_);
    }
    if(publish_)publish_(copy);
    return true;
}

bool RunOrchestrator::acknowledgeResetCleanup(const std::string& runId)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(snapshot_.recordingId!=runId||snapshot_.postRun!=PostRunState::ResetCleanupPending)return false;
        snapshot_.recordingId.clear();snapshot_.recordingSha256.clear();archiveRunId_.clear();
        recordingStartIssued_=false;recordingStartGeneration_=0;recordingOwned_=false;
        recordingGeneration_=0;finalizationShouldFinish_=false;
        purgeAllCleanup_=false;
        finalizationState_=FinalizationState::Idle;snapshot_.finalization=finalizationState_;
        snapshot_.postRun=PostRunState::None;snapshot_.busy=false;snapshot_.phase=Phase::Idle;snapshot_.error.clear();copy=publishable(snapshot_);
    }
    logLine("run reset cleanup acknowledged for "+runId+"; back to idle");
    if(publish_)publish_(copy);
    return true;
}

} // namespace orchestration
} // namespace AppMain
