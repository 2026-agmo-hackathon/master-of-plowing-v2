#include <AppMain/orchestration/RunOrchestrator.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
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
                                 std::function<void(const std::string&,const std::string&)> finalized)
    : api_(api), store_(store), runControl_(runControl), publish_(std::move(publish)),
      finalized_(std::move(finalized))
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

void RunOrchestrator::setPhase(Phase phase, const std::string& error)
{
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.phase = phase;
        snapshot_.busy = phase == Phase::Refreshing || phase == Phase::Starting ||
                         phase == Phase::ApplyingSetup || phase == Phase::Finishing;
        snapshot_.error = error;
        copy = publishable(snapshot_);
    }
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
    setPhase(Phase::Refreshing);
    Catalog catalog;
    const ApiResult result = api_.catalog(catalog);
    if (!result.ok || !fresh(catalog.selection)) {
        setPhase(Phase::Failed, result.ok ? "simulator catalog is stale" : result.error);
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
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
        const StartRequest& desired=snapshot_.desiredSetup;
        const StartRequest& confirmed=snapshot_.confirmedSetup;
        setupMatches=desired.mapId==request.mapId
            && desired.tractorId==request.tractorId
            && desired.implementId==request.implementId
            && confirmed.mapId==request.mapId
            && confirmed.tractorId==request.tractorId
            && confirmed.implementId==request.implementId;
    }
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
    if (!rec.live || rec.snapshotAgeMs < 0 || rec.snapshotAgeMs > MAX_SNAPSHOT_AGE_MS) {
        reason = "recording snapshot is stale"; return false;
    }
    if (rec.recording) { reason = "recorder still running"; return false; }
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

bool RunOrchestrator::persistRecording()
{
    Recording rec;
    const ApiResult get = api_.recording(rec);
    std::string reason;
    if (!get.ok || !recAcceptable(rec, reason)) {
        setPhase(Phase::Failed, get.ok ? reason : get.error);
        return false;
    }
    std::string id, sha;
    std::string team,runId;
    { std::lock_guard<std::mutex> lock(mutex_);
      team = snapshot_.teamName; runId=archiveRunId_; }
    const ApiResult stored = store_.persist(team,runId,rec.bytes,id,sha);
    if (!stored.ok) { setPhase(Phase::Failed, stored.error); return false; }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        snapshot_.recordingId = id;
        snapshot_.recordingSha256 = sha;
    }
    return true;
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
    constexpr int REQUIRED_STOPPED_SAMPLES=3;
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
    bool stoppedConfirmed=false;
    bool absentNow=false;
    int consecutiveFreshStopped=0;
    constexpr int REQUIRED_STOPPED_SAMPLES=3;
    for(int attempt=0;attempt<30;++attempt) {
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
        if(fresh && recorder.recording) {
            consecutiveFreshStopped=0;
            const ApiResult repeatedStop=api_.recordingStop();
            if(!repeatedStop.ok && repeatedStop.status!=409)
                return finalizationFailed("repeated recording stop: "+repeatedStop.error);
        } else if(fresh) {
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
    if (!persistRecording()) {
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
    Snapshot copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(finalizationState_!=FinalizationState::Completed
            || snapshot_.recordingId!=runId
            || snapshot_.postRun!=PostRunState::AwaitingAction)return false;
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
        if(snapshot_.postRun!=PostRunState::CaptureInProgress)return false;
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
        if(snapshot_.recordingId!=runId)return false;
        // The browser persists a terminal result before sending this frame and
        // replays it until the backend snapshot confirms Submitted.  A lost
        // reply must therefore be an idempotent success, not an error.
        if(snapshot_.postRun==PostRunState::Submitted)return true;
        if(snapshot_.postRun!=PostRunState::CaptureDurable)return false;
        // Keep the post-run transition and its durable ledger write mutually
        // exclusive with Reset so a submitted archive cannot be purged in the
        // gap between the two operations.
        if(!store_.markSubmitted(runId,verdict).ok)return false;
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
    if(publish_)publish_(copy);
    return true;
}

} // namespace orchestration
} // namespace AppMain
