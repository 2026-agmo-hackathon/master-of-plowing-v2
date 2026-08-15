#include <AppMain/MainController.hpp>
#include <AppMain/orchestration/SimulatorApiClient.hpp>
#include <AppMain/web/OrchestrationProtocol.hpp>
#include <AppMain/web/WebSocketEndPoint.hpp>

#include <json/json.h>
#include <thread>

namespace AppMain {
namespace {

class MainRunControl final : public orchestration::IRunControl {
public:
    explicit MainRunControl(MainController& controller) : controller_(controller) {}
    bool readyToStart() const override { return controller_.isReadyToStart(); }
    bool runActive() const override {
        const RunState state=controller_.getRunState();
        return state==RunState::Running || state==RunState::Paused;
    }
    bool startCancelled() const override { return controller_.isOrchestrationFinishRequested(); }
    void installRunGeometry(const tracking::VehicleGeometry& g) override { controller_.setRunGeometry(g); }
    void resetRun() override { controller_.prepareForNewTrack(); }
    bool enterRunning() override {
        controller_.endCommandedStop();
        return !controller_.isOrchestrationFinishRequested()
            && controller_.setRunState(RunState::Running);
    }
    void enterSafeIdle() override {
        // 정지는 "지령"해야 한다. 제어권을 놓기만 하면 트랙터는 주행 마지막
        // 개도를 그대로 유지한다 (MainController::beginCommandedStop 주석 참고).
        // Releasing authority alone leaves the tractor at the run's last
        // opening; the stop has to be commanded — see beginCommandedStop.
        controller_.beginCommandedStop();
        controller_.setRunState(RunState::Idle);
        controller_.releaseMachineOutputs();
    }
    void enterFinished() override { controller_.markRunFinished(); }
private:
    MainController& controller_;
};

// An admission rejection changes no snapshot field, so the only way the UI can
// tell "refused" from "still working" is this frame.
void publishOrchestrationRejection(const std::string& error)
{
    Json::Value message;
    message["kind"]="orchestration_request_error";
    message["error"]=error;
    if(auto endpoint=web::WebSocketEndPoint::getInstance())
        endpoint->publishMessage(message);
}

Json::Value catalogJson(const std::vector<orchestration::CatalogItem>& items)
{
    Json::Value out(Json::arrayValue);
    for (const auto& item : items) {
        Json::Value row;
        row["id"] = item.id; row["label"] = item.label;
        if (item.widthM > 0.0) row["widthM"] = item.widthM;
        if (item.geometry.valid()) {
            row["wheelbaseM"] = item.geometry.wheelbaseM;
            row["maxWheelAngleRad"] = item.geometry.maxWheelAngleRad;
            row["steeringRatio"] = item.geometry.steeringRatio;
            row["maxForwardGear"] = item.geometry.maxForwardGear;
        }
        out.append(row);
    }
    return out;
}

}

std::string orchestrationSnapshotJson(const orchestration::Snapshot& s)
{
    Json::Value root;
    root["kind"] = "sim_setup_state";
    root["phase"] = orchestration::phaseName(s.phase);
    root["finalization"] = orchestration::finalizationStateName(s.finalization);
    root["postRun"] = orchestration::postRunStateName(s.postRun);
    root["runGeneration"] = Json::UInt64(s.runGeneration);
    root["resetGeneration"] = Json::UInt64(s.resetGeneration);
    root["setupApplying"] = s.setupApplying;
    root["setupGeneration"] = Json::UInt64(s.setupGeneration);
    root["busy"] = s.busy; root["error"] = s.error; root["teamName"] = s.teamName;
    root["recordingId"] = s.recordingId; root["recordingSha256"] = s.recordingSha256;
    root["maps"] = catalogJson(s.catalog.maps);
    root["tractors"] = catalogJson(s.catalog.tractors);
    root["implements"] = catalogJson(s.catalog.implements);
    const auto& selected=s.catalog.selection;
    root["selection"]["mapId"] = selected.mapId;
    root["selection"]["tractorId"] = selected.tractorId;
    root["selection"]["implementId"] = selected.implementId;
    root["selection"]["stopped"] = selected.stopped;
    root["selection"]["reactRunning"] = selected.reactRunning;
    root["selection"]["live"] = selected.live;
    root["selection"]["snapshotAgeMs"] = Json::Int64(selected.snapshotAgeMs);
    Json::Value& diag = root["selection"]["loopDiag"];
    diag["hidden"] = selected.pageHidden;
    diag["schedule"] = selected.schedule;
    diag["droppedMs"] = Json::Int64(selected.droppedMs);
    diag["ticks"] = Json::Int64(selected.ticks);
    diag["stopFrom"] = selected.stopFrom;
    root["desiredSetup"]["mapId"] = s.desiredSetup.mapId;
    root["desiredSetup"]["tractorId"] = s.desiredSetup.tractorId;
    root["desiredSetup"]["implementId"] = s.desiredSetup.implementId;
    root["confirmedSetup"]["mapId"] = s.confirmedSetup.mapId;
    root["confirmedSetup"]["tractorId"] = s.confirmedSetup.tractorId;
    root["confirmedSetup"]["implementId"] = s.confirmedSetup.implementId;
    Json::StreamWriterBuilder writer; writer["indentation"]="";
    return Json::writeString(writer,root);
}

void MainController::ensureOrchestrator()
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    if (orchestrator_) return;
    simulatorApi_ = std::make_shared<orchestration::SimulatorApiClient>();
    recordingStore_ = std::make_shared<orchestration::FileRecordingStore>();
    runControlAdapter_ = std::make_shared<MainRunControl>(*this);
    orchestrator_ = std::make_shared<orchestration::RunOrchestrator>(
        *simulatorApi_, *recordingStore_, *runControlAdapter_,
        [this](const orchestration::Snapshot& state) {
            if (auto ws=getWebSocketEndPoint()) ws->publishMessage(orchestrationSnapshotJson(state));
        },[this](const std::string& team,const std::string& runId){
            Json::Value event;event["kind"]="run_finalized";
            event["teamName"]=team;event["runId"]=runId;
            if(auto ws=getWebSocketEndPoint())ws->publishMessage(event);
        });
}

bool MainController::launchOrchestration(
    std::function<void(orchestration::RunOrchestrator&)> work,
    OrchestrationAdmission admission)
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();
    if(!lifecycleToken) return false;
    ensureOrchestrator();
    std::shared_ptr<orchestration::RunOrchestrator> orchestrator;
    std::future<void> previousWorker;
    {
        std::lock_guard<std::mutex> lock(orchestrationMutex_);
        if(orchestrationShutdown_.load()) return false;
        // Start admission and Finish cancellation are serialized with the
        // queue. Thus a concurrent Finish either observes and cancels the
        // accepted Start, or is fully queued before a later Start is rejected.
        if(admission==OrchestrationAdmission::Start) {
            // Only a mutating peer blocks a Start. A background refresh does
            // not: it is read-only, and start() re-reads the catalog itself.
            if(orchestrationMutatingPending_>0)
                return false;
            orchestrationFinishRequested_=false;
        } else if(admission==OrchestrationAdmission::Finish) {
            orchestrationFinishRequested_=true;
        }
        if(admission!=OrchestrationAdmission::Refresh) ++orchestrationMutatingPending_;
        orchestrationTasks_.push_back(
            OrchestrationTask{std::move(work),
                              admission!=OrchestrationAdmission::Refresh});
        if (orchestrationTaskActive_.exchange(true)) return true;
        orchestrator = orchestrator_;
        previousWorker=std::move(orchestrationWorker_);
    }
    if(previousWorker.valid()) {
        try { previousWorker.get(); }
        catch(...) {
            std::lock_guard<std::mutex> lock(orchestrationMutex_);
            orchestrationTaskActive_=false;
            orchestrationTasks_.clear();
            orchestrationMutatingPending_=0;
            return false;
        }
    }
    try {
        std::future<void> worker=std::async(std::launch::async,[this,orchestrator] {
            for (;;) {
                OrchestrationTask next;
                {
                    std::lock_guard<std::mutex> lock(orchestrationMutex_);
                    if(orchestrationShutdown_.load()) {
                        orchestrationTasks_.clear();
                        orchestrationMutatingPending_=0;
                        orchestrationTaskActive_=false;
                        break;
                    }
                    if (orchestrationTasks_.empty()) {
                        orchestrationTaskActive_=false;
                        break;
                    }
                    next=std::move(orchestrationTasks_.front());
                    orchestrationTasks_.pop_front();
                }
                // Held until the task *finishes*, not merely until it is
                // dequeued: a second Start must not be admitted while the first
                // one is still talking to the simulator.
                struct Release {
                    MainController* self; bool mutating;
                    ~Release() {
                        if(!mutating) return;
                        std::lock_guard<std::mutex> lock(self->orchestrationMutex_);
                        if(self->orchestrationMutatingPending_>0)
                            --self->orchestrationMutatingPending_;
                    }
                } release{this,next.mutating};
                next.work(*orchestrator);
            }
        });
        {
            std::lock_guard<std::mutex> lock(orchestrationMutex_);
            orchestrationWorker_=std::move(worker);
        }
    } catch(...) {
        {
            std::lock_guard<std::mutex> lock(orchestrationMutex_);
            orchestrationTaskActive_=false;
            orchestrationTasks_.clear();
            orchestrationMutatingPending_=0;
        }
        return false;
    }
    return true;
}

void MainController::requestOrchestrationRefresh()
{
    // At most one refresh may be queued behind the running one. The latch is
    // released as the task is consumed, not when it completes, so a later poll
    // still queues a fresh read while the current one is in flight — but the
    // queue can never grow past one and starve Start/Reset admission.
    {
        std::lock_guard<std::mutex> lock(orchestrationMutex_);
        if(orchestrationRefreshQueued_) return;
        orchestrationRefreshQueued_=true;
    }
    const bool queued=launchOrchestration([this](orchestration::RunOrchestrator& o) {
        {
            std::lock_guard<std::mutex> lock(orchestrationMutex_);
            orchestrationRefreshQueued_=false;
        }
        o.refresh();
    },OrchestrationAdmission::Refresh);
    if(!queued) {
        std::lock_guard<std::mutex> lock(orchestrationMutex_);
        orchestrationRefreshQueued_=false;
    }
}
void MainController::requestOrchestrationSetup(const orchestration::StartRequest& request)
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();
    if(!lifecycleToken || orchestrationShutdown_.load()) return;
    ensureOrchestrator();
    const unsigned long long generation=++orchestrationSetupGeneration_;
    std::shared_ptr<orchestration::RunOrchestrator> orchestrator;
    {
        std::lock_guard<std::mutex> lock(orchestrationMutex_);
        orchestrator=orchestrator_;
    }
    orchestrator->noteSetupIntent(request,generation);
    (void)launchOrchestration(
        [request,generation](orchestration::RunOrchestrator& o) {
            o.applySetup(request,generation);
        });
}
void MainController::requestOrchestrationStart(const orchestration::StartRequest& request)
{
    // Admission is backend-owned: never queue a second Start behind refresh,
    // Start, or Finish, and never clear an in-flight Finish cancellation.
    // A rejection is invisible in the snapshot (no phase changes), so it must
    // be reported explicitly — otherwise the button appears to do nothing.
    if(!launchOrchestration(
        [this,request](orchestration::RunOrchestrator& o) {
            // 워커까지 들어간 뒤의 거절도 반드시 화면에 띄운다. 스냅샷에 적으면
            // 2.5초 뒤 새로고침이 지워 버리므로, 사용자가 닫을 때까지 남는
            // orchestration_request_error 로 보낸다.
            //
            // A refusal that happens once the worker is running must reach the
            // screen too. Recording it in the snapshot is erased by the next
            // 2.5 s refresh, so send it over orchestration_request_error, which
            // the UI keeps until the operator dismisses it.
            std::string refusal;
            if(!o.start(request,&refusal) && !refusal.empty())
                publishOrchestrationRejection("Start rejected: "+refusal);
        },
        OrchestrationAdmission::Start)) {
        publishOrchestrationRejection(
            "Start rejected: another orchestration request is still in flight");
    }
}
void MainController::requestOrchestrationReset()
{
    const auto before=getOrchestrationSnapshot();
    if(before.postRun!=orchestration::PostRunState::ResetCleanupPreparing)return;
    if(!launchOrchestration(
        [this,runId=before.recordingId,resetGeneration=before.resetGeneration](orchestration::RunOrchestrator& o) {
            if(!o.resetCompletedRun())return;
            Json::Value event;event["kind"]="run_reset_ready";event["runId"]=runId;
            event["resetGeneration"]=Json::UInt64(resetGeneration);
            if(auto ws=getWebSocketEndPoint())ws->publishMessage(event);
        },OrchestrationAdmission::Start)) {
        // The browser already prepared its cleanup and is waiting. Leaving it
        // silent strands the run in ResetCleanupPreparing with no retry cue.
        (void)failResetCleanup(before.recordingId);
        publishOrchestrationRejection(
            "Reset rejected: another orchestration request is still in flight");
    }
}
bool MainController::beginResetCleanup(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->beginResetCleanup(runId);
}
bool MainController::beginPurgeAllCleanup()
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->beginPurgeAllCleanup();
}
bool MainController::noteResetCleanupPrepared(unsigned long long generation)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->noteResetCleanupPrepared(generation);
}
bool MainController::failResetCleanup(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->failResetCleanup(runId);
}
bool MainController::acknowledgeSealedCapture(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    if(!orchestrator_)return false;
    if(!orchestrator_->acknowledgeSealedCapture(runId))return false;
    browserSealedCapturedRunId_=runId;return true;
}
bool MainController::beginSealedCapture(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->beginSealedCapture(runId);
}
bool MainController::failSealedCapture(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->failSealedCapture(runId);
}
bool MainController::markLeaderboardSubmitted(const std::string& runId,const std::string& verdict)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->markLeaderboardSubmitted(runId,verdict);
}
bool MainController::acknowledgeResetCleanup(const std::string& runId)
{
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ && orchestrator_->acknowledgeResetCleanup(runId);
}
void MainController::requestOrchestrationFinish(bool automatic)
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();
    if(!lifecycleToken) return;
    if(orchestrationShutdown_.load()) return;
    const auto state=getOrchestrationSnapshot();
    const bool retry=state.finalization==orchestration::FinalizationState::RetryableError;
    // The first Finish releases authority synchronously. A retry resumes only
    // recorder/network finalization and must not repeat controller transitions.
    if(!retry) { beginCommandedStop(); setRunState(RunState::Idle); releaseMachineOutputs(); }
    (void)launchOrchestration(
        [automatic](orchestration::RunOrchestrator& o){o.finish(automatic);},
        OrchestrationAdmission::Finish);
}
orchestration::Snapshot MainController::getOrchestrationSnapshot() const
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();
    if(!lifecycleToken) return {};
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    return orchestrator_ ? orchestrator_->snapshot() : orchestration::Snapshot{};
}
bool MainController::loadRecording(const std::string& id,std::string& bytes) const
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();
    if(!lifecycleToken) return false;
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    auto store=std::dynamic_pointer_cast<orchestration::FileRecordingStore>(recordingStore_);
    return store && store->load(id,bytes);
}
std::vector<orchestration::UnsubmittedRecord> MainController::listUnsubmittedRecordings() const
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();if(!lifecycleToken)return {};
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    auto store=std::dynamic_pointer_cast<orchestration::FileRecordingStore>(recordingStore_);
    return store?store->listUnsubmitted():std::vector<orchestration::UnsubmittedRecord>{};
}
bool MainController::purgeUnsubmittedRecordings()
{
    auto lifecycleToken=orchestrationLifecycle_.tryEnter();if(!lifecycleToken)return false;
    std::lock_guard<std::mutex> lock(orchestrationMutex_);
    auto store=std::dynamic_pointer_cast<orchestration::FileRecordingStore>(recordingStore_);
    return store&&store->purgeUnsubmitted().ok;
}
} // namespace AppMain
