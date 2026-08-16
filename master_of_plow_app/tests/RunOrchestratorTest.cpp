#include <gtest/gtest.h>
#include <AppMain/orchestration/RunOrchestrator.hpp>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <sstream>
#include <thread>

using namespace AppMain::orchestration;

namespace {
Catalog goodCatalog()
{
    Catalog c;
    CatalogItem map; map.id="map-a"; map.label="Map A"; c.maps.push_back(map);
    CatalogItem tractor; tractor.id="tractor-a"; tractor.label="Tractor A";
    tractor.geometry.wheelbaseM = 3.1;
    tractor.geometry.steeringRatio = 20.0;
    tractor.geometry.maxForwardGear = 6;
    c.tractors.push_back(tractor);
    CatalogItem implement; implement.id="plow-a"; implement.label="Plow A";
    implement.widthM=2.2; c.implements.push_back(implement);
    c.selection = {"map-a", "tractor-a", "plow-a", true, false, true, 10, 100};
    return c;
}

// SimulatorApiClient 는 HTTP 404 "no recording yet" 을 실패가 아니라
// present=false 인 정상 응답으로 옮긴다. 가짜 API 도 같은 계약을 흉내낸다.
//
// SimulatorApiClient reports HTTP 404 "no recording yet" as a successful read
// with present=false, not as a failure. The fakes model the same contract.
ApiResult noRecording(RecorderState& out)
{
    out.live=true; out.recording=false; out.present=false; out.snapshotAgeMs=10;
    return {true,404,{}};
}

struct FakeApi : ISimulatorApi {
    Catalog data = goodCatalog();
    Selection selected = data.selection;
    RecorderState recorder{true,false,true,10};
    std::string failStage;
    int engineOff = 0, engineOn = 0, stopCalls = 0, resetCalls = 0, refreshCalls = 0, startCalls = 0, selectionCalls=0;
    int mapCalls=0,tractorCalls=0,implementCalls=0;
    int catalogCalls=0;
    std::vector<std::string> selectionOrder;
    ApiResult result(const char* stage) {
        return failStage == stage ? ApiResult{false, 503, stage} : ApiResult{true, 200, {}};
    }
    ApiResult catalog(Catalog& out) override { ++catalogCalls;out=data; out.selection=selected; return result("catalog"); }
    ApiResult selection(Selection& out) override {
        out=selected;
        ++selectionCalls;
        if (failStage=="engine_confirm" && selectionCalls==1) return {false,503,"engine_confirm"};
        return result("selection");
    }
    ApiResult selectMap(const std::string& id) override { ++mapCalls;selectionOrder.push_back("map");selected.mapId=id; return result("map"); }
    ApiResult selectTractor(const std::string& id) override { ++tractorCalls;selectionOrder.push_back("tractor");selected.tractorId=id; return result("tractor"); }
    ApiResult selectImplement(const std::string& id) override { ++implementCalls;selectionOrder.push_back("implement");selected.implementId=id; return result("implement"); }
    ApiResult simulatorRefresh() override { ++refreshCalls; return result("sim_refresh"); }
    ApiResult recordingReset() override { ++resetCalls; recorder.recording=false; return result("rec_reset"); }
    ApiResult recordingStart() override { ++startCalls; recorder.recording=true; return result("rec_start"); }
    ApiResult recordingStop() override { ++stopCalls; recorder.recording=false; return result("rec_stop"); }
    ApiResult recordingState(RecorderState& out) override {
        out=recorder;
        return result("rec_confirm");
    }
    ApiResult engine(bool on) override { if (on) ++engineOn; else ++engineOff; selected.reactRunning=on; selected.simElapsedMs += on ? 1 : 0; return result(on?"engine_on":"engine_off"); }
    ApiResult recording(Recording& out) override { out={"a,b\n1,2\n", "text/csv; charset=utf-8", "attachment; filename=\"rec.csv\"", true, false, 10}; return result("rec_get"); }
};
struct FakeStore : IRecordingStore {
    int calls=0, failures=0,purgeCalls=0,submittedCalls=0; std::string bytes;
    ApiResult persist(const std::string&, const std::string&, const std::string& b, std::string& id, std::string& sha) override {
        ++calls; bytes=b;
        if(failures>0){--failures;return {false,500,"disk failed"};}
        id="run.csv"; sha="abc"; return {true,200,{}};
    }
    ApiResult markSubmitted(const std::string&,const std::string&) override{++submittedCalls;return {true,200,{}};}
    ApiResult purgeUnsubmitted() override{++purgeCalls;return {true,200,{}};}
    std::vector<UnsubmittedRecord> listUnsubmitted() const override{return {};}
};

TEST(RunOrchestratorPolicyTest, BlocksNextStartUntilBrowserDurablyCapturedSealedRun)
{
    Snapshot state;state.finalization=FinalizationState::Completed;state.recordingId="stable-run";
    EXPECT_TRUE(RunOrchestrator::sealedCaptureRequired(state,""));
    EXPECT_TRUE(RunOrchestrator::sealedCaptureRequired(state,"other-run"));
    EXPECT_FALSE(RunOrchestrator::sealedCaptureRequired(state,"stable-run"));
    state.finalization=FinalizationState::RetryableError;
    EXPECT_FALSE(RunOrchestrator::sealedCaptureRequired(state,""));
}

TEST(RunOrchestratorPolicyTest, PhysicalStopUsesFreshIssue9SelectionContract)
{
    const StartRequest expected{"Team","map-a","tractor-a","plow-a"};
    Selection sample{"map-a","tractor-a","plow-a",true,false,true,100,10};
    EXPECT_TRUE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
    sample.snapshotAgeMs=RunOrchestrator::MAX_SNAPSHOT_AGE_MS+1;
    EXPECT_FALSE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
    sample.snapshotAgeMs=100; sample.live=false;
    EXPECT_FALSE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
    sample.live=true; sample.reactRunning=true;
    EXPECT_FALSE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
    sample.reactRunning=false; sample.stopped=false;
    EXPECT_FALSE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
    sample.stopped=true; sample.mapId="other";
    EXPECT_FALSE(RunOrchestrator::physicalStopSampleAcceptable(sample,expected));
}

TEST(SimulatorApiSourceContractTest, RecordingResetPostHasZeroLengthBody)
{
    const auto source=std::filesystem::path(__FILE__).parent_path().parent_path()/
        "src-gen/AppMain/orchestration/SimulatorApiClient.cpp";
    std::ifstream input(source);
    ASSERT_TRUE(input.good()) << source;
    std::ostringstream bytes;
    bytes << input.rdbuf();
    EXPECT_NE(bytes.str().find(
        "recordingReset(){return apiResult(request(\"POST\",\"/api/simulator/rec/reset\"));}"),
        std::string::npos);
    EXPECT_EQ(bytes.str().find(
        "recordingReset(){return apiResult(request(\"POST\",\"/api/simulator/rec/reset\",\"{}\"));}"),
        std::string::npos);
}
struct FakeRun : IRunControl {
    bool ready=true, running=false, finished=false, refuseRunning=false; std::atomic<bool> cancel{false}; int safe=0, reset=0; AppMain::tracking::VehicleGeometry geometry;
    bool readyToStart() const override { return ready; }
    bool runActive() const override { return running; }
    bool startCancelled() const override { return cancel.load(); }
    void installRunGeometry(const AppMain::tracking::VehicleGeometry& g) override { geometry=g; }
    void resetRun() override { ++reset; }
    bool enterRunning() override { running=!refuseRunning; return !refuseRunning; }
    void enterSafeIdle() override { running=false; ++safe; }
    void enterFinished() override { finished=true; }
};
StartRequest request() { return {"Team Alpha", "map-a", "tractor-a", "plow-a"}; }
void addAlternateSetup(FakeApi& api)
{
    CatalogItem map; map.id="map-b"; map.label="Map B"; api.data.maps.push_back(map);
    CatalogItem tractor; tractor.id="tractor-b"; tractor.label="Tractor B";
    tractor.geometry.wheelbaseM=2.8; tractor.geometry.steeringRatio=18.0;
    tractor.geometry.maxForwardGear=5; api.data.tractors.push_back(tractor);
    CatalogItem implement; implement.id="plow-b"; implement.label="Plow B";
    implement.widthM=3.0; api.data.implements.push_back(implement);
}
}

TEST(RunOrchestratorTest, SetupAppliesChangedFieldsInOrderAndConfirmsWithoutRunSideEffects)
{
    FakeApi api; addAlternateSetup(api); FakeStore store; FakeRun run;
    RunOrchestrator orchestrator(api,store,run);
    const StartRequest setup{"Team B","map-b","tractor-b","plow-b"};
    orchestrator.noteSetupIntent(setup,1);
    ASSERT_TRUE(orchestrator.applySetup(setup,1));
    EXPECT_EQ(api.selectionOrder,(std::vector<std::string>{"map","tractor","implement"}));
    EXPECT_EQ(api.resetCalls,0); EXPECT_EQ(api.startCalls,0); EXPECT_EQ(api.stopCalls,0);
    EXPECT_EQ(api.engineOff,0); EXPECT_EQ(run.reset,0); EXPECT_FALSE(run.running);
    EXPECT_EQ(orchestrator.snapshot().confirmedSetup.mapId,"map-b");
}

// 맵을 바꾸면 차량이 다시 자리를 잡는데, 선택 스냅샷은 1 Hz 라 그 사이에도
// 앱에는 stopped=true 로 보인다. 그래서 이어지는 트랙터 선택이 409 로 튕기고
// 셋업이 "tractor must be stopped" 에서 멈춰 버렸다. 409 는 재시도로 풀린다.
//
// A map change resettles the vehicle, but the 1 Hz selection snapshot still
// shows stopped=true meanwhile, so the following tractor request bounced with
// 409 and the setup stalled on "tractor must be stopped". A 409 is a race that
// retrying resolves.
TEST(RunOrchestratorTest, SetupRetriesTheStoppedConflictInsteadOfFailingTheWholeSetup)
{
    struct ConflictApi : FakeApi {
        int tractorConflicts=2;
        ApiResult selectTractor(const std::string& id) override {
            if(tractorConflicts-->0) return {false,409,"tractor must be stopped"};
            return FakeApi::selectTractor(id);
        }
    } api;
    addAlternateSetup(api);
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    const StartRequest setup{"Team B","map-b","tractor-b","plow-b"};
    orchestrator.noteSetupIntent(setup,1);
    ASSERT_TRUE(orchestrator.applySetup(setup,1));
    EXPECT_EQ(orchestrator.snapshot().confirmedSetup.tractorId,"tractor-b");
    EXPECT_EQ(orchestrator.snapshot().error,"");
    EXPECT_EQ(api.selectionOrder,(std::vector<std::string>{"map","tractor","implement"}));
}

TEST(RunOrchestratorTest, SetupStillFailsWhenTheConflictNeverClears)
{
    struct StuckApi : FakeApi {
        ApiResult selectTractor(const std::string&) override {
            return {false,409,"tractor must be stopped"};
        }
    } api;
    addAlternateSetup(api);
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    const StartRequest setup{"Team B","map-b","tractor-b","plow-b"};
    orchestrator.noteSetupIntent(setup,1);
    EXPECT_FALSE(orchestrator.applySetup(setup,1));
    EXPECT_NE(orchestrator.snapshot().error.find("tractor must be stopped"),
              std::string::npos);
}

TEST(RunOrchestratorTest, StartPreflightReportsFirstFailedPredicateWithoutSideEffects)
{
    struct Case { std::string expected; std::function<void(FakeApi&,FakeRun&)> breakGate; };
    const std::vector<Case> cases={
        {"catalog unavailable",[](FakeApi& a,FakeRun&){a.failStage="catalog";}},
        {"not live",[](FakeApi& a,FakeRun&){a.selected.live=false;}},
        {"age is invalid: -1 ms",[](FakeApi& a,FakeRun&){a.selected.snapshotAgeMs=-1;}},
        {"is stale: "+std::to_string(RunOrchestrator::MAX_SNAPSHOT_AGE_MS+1)+" ms",
            [](FakeApi& a,FakeRun&){a.selected.snapshotAgeMs=RunOrchestrator::MAX_SNAPSHOT_AGE_MS+1;}},
        {"stopped=false",[](FakeApi& a,FakeRun&){a.selected.stopped=false;}},
        {"reactRunning=true",[](FakeApi& a,FakeRun&){a.selected.reactRunning=true;}},
        // 선택이 흘러내린 경우 Start 는 이제 직접 재정렬을 시도한다. 재정렬이
        // 성공하는 경로는 별도 테스트가 고정하고, 여기서는 적용 자체가 계속
        // 실패할 때 부작용 없이 거절되는 것을 본다.
        // A drifted selection is now realigned by Start itself; the success
        // path is pinned by its own test. This case pins that a persistently
        // failing apply is refused without side effects.
        {"start setup alignment: map selection",
            [](FakeApi& a,FakeRun&){a.selected.mapId="map-b";a.failStage="map";}},
        {"selected map is absent",[](FakeApi& a,FakeRun&){a.data.maps.clear();}},
        {"selected tractor is absent",[](FakeApi& a,FakeRun&){a.data.tractors.clear();}},
        {"tractor geometry is invalid",[](FakeApi& a,FakeRun&){a.data.tractors.front().geometry.wheelbaseM=0.0;}},
        {"selected implement is absent",[](FakeApi& a,FakeRun&){a.data.implements.clear();}},
        {"controller is not ready",[](FakeApi&,FakeRun& r){r.ready=false;}},
    };
    for(const auto& test:cases) {
        FakeApi api;FakeStore store;FakeRun run;test.breakGate(api,run);
        RunOrchestrator orchestrator(api,store,run);
        EXPECT_FALSE(orchestrator.start(request()))<<test.expected;
        EXPECT_NE(orchestrator.snapshot().error.find(test.expected),std::string::npos)<<orchestrator.snapshot().error;
        EXPECT_EQ(api.resetCalls,0);EXPECT_EQ(api.startCalls,0);EXPECT_EQ(api.engineOn,0);
        EXPECT_EQ(run.reset,0);EXPECT_FALSE(run.running);
    }

    FakeApi api;addAlternateSetup(api);FakeStore store;FakeRun run;RunOrchestrator orchestrator(api,store,run);
    const StartRequest other{"Team Alpha","map-b","tractor-a","plow-a"};
    orchestrator.noteSetupIntent(other,1);ASSERT_TRUE(orchestrator.applySetup(other,1));
    api.selected=goodCatalog().selection;
    EXPECT_FALSE(orchestrator.start(request()));
    EXPECT_NE(orchestrator.snapshot().error.find("desired and confirmed"),std::string::npos);
    EXPECT_EQ(api.resetCalls,0);EXPECT_EQ(api.startCalls,0);
}

TEST(RunOrchestratorTest, StartRechecksPhysicalStopAfterRecorderConfirmationBeforeEngine)
{
    struct ChangedSelectionApi : FakeApi {
        bool stale=false;
        ApiResult selection(Selection& out) override {
            ++selectionCalls;out=selected;
            if(selectionCalls==1) {
                if(stale)out.snapshotAgeMs=RunOrchestrator::MAX_SNAPSHOT_AGE_MS+1;
                else out.reactRunning=true;
            }
            return {true,200,{}};
        }
    };
    for(bool stale:{false,true}) {
        ChangedSelectionApi api;api.stale=stale;
        FakeStore store;FakeRun run;RunOrchestrator orchestrator(api,store,run);
        EXPECT_FALSE(orchestrator.start(request()));
        EXPECT_NE(orchestrator.snapshot().error.find("pre-engine physical-stop confirmation"),std::string::npos);
        EXPECT_EQ(api.engineOn,0);EXPECT_GE(api.stopCalls,1);EXPECT_FALSE(run.running);
    }
}

TEST(RunOrchestratorTest, CachedSelectionAgeAdvancesMonotonically)
{
    FakeApi api;FakeStore store;FakeRun run;RunOrchestrator orchestrator(api,store,run);
    ASSERT_TRUE(orchestrator.refresh());
    const auto first=orchestrator.snapshot().catalog.selection.snapshotAgeMs;
    std::this_thread::sleep_for(std::chrono::milliseconds(12));
    const auto later=orchestrator.snapshot().catalog.selection.snapshotAgeMs;
    EXPECT_GE(later,first+8);
}

TEST(RunOrchestratorTest, SetupRejectsStaleUnknownMovingAndUnconfirmedWithoutCompensation)
{
    for(const int mode : {0,1,2,3}) {
        struct UnconfirmedApi : FakeApi {
            bool withhold=false;
            ApiResult selectMap(const std::string& id) override {
                if(withhold){++mapCalls;return {true,200,{}};}
                return FakeApi::selectMap(id);
            }
        } api;
        addAlternateSetup(api); FakeStore store; FakeRun run;
        StartRequest setup{"Team","map-b","tractor-b","plow-b"};
        if(mode==0) api.selected.snapshotAgeMs=4001;
        if(mode==1) setup.mapId="not-listed";
        if(mode==2) {api.selected.stopped=false;run.running=true;}
        if(mode==3) api.withhold=true;
        RunOrchestrator orchestrator(api,store,run);
        orchestrator.noteSetupIntent(setup,1);
        EXPECT_FALSE(orchestrator.applySetup(setup,1));
        EXPECT_EQ(api.resetCalls+api.startCalls+api.stopCalls,0);
        EXPECT_EQ(api.engineOff,0);
    }
}

TEST(RunOrchestratorTest, SetupGateReportsSpecificSelectionAndCatalogErrors)
{
    struct Case { int mode; const char* error; };
    for(const Case expected : std::vector<Case>{{0,"not live"},{1,"stale"},
            {2,"not stopped"},{3,"unknown map"},{4,"unknown tractor"},
            {5,"unknown implement"}}) {
        FakeApi api; addAlternateSetup(api); FakeStore store; FakeRun run;
        StartRequest setup{"Team","map-b","tractor-b","plow-b"};
        if(expected.mode==0) api.selected.live=false;
        if(expected.mode==1) api.selected.snapshotAgeMs=4001;
        if(expected.mode==2) api.selected.stopped=false;
        if(expected.mode==3) setup.mapId="missing-map";
        if(expected.mode==4) setup.tractorId="missing-tractor";
        if(expected.mode==5) setup.implementId="missing-implement";
        RunOrchestrator o(api,store,run);
        o.noteSetupIntent(setup,1);
        EXPECT_FALSE(o.applySetup(setup,1));
        EXPECT_NE(o.snapshot().error.find(expected.error),std::string::npos)
            << o.snapshot().error;
        EXPECT_EQ(api.resetCalls+api.startCalls+api.stopCalls,0);
        EXPECT_EQ(api.engineOff,0);
    }
}

TEST(RunOrchestratorTest, NoneImplementIsAllowlistedAndAppliedWithoutRunSideEffects)
{
    FakeApi api; FakeStore store; FakeRun run;
    CatalogItem none; none.id="none"; none.label="None";
    api.data.implements.insert(api.data.implements.begin(),none);
    const StartRequest setup{"Team","map-a","tractor-a","none"};
    RunOrchestrator o(api,store,run);
    o.noteSetupIntent(setup,1);
    ASSERT_TRUE(o.applySetup(setup,1));
    EXPECT_EQ(api.selected.implementId,"none");
    EXPECT_EQ(api.implementCalls,1);
    EXPECT_EQ(api.resetCalls+api.startCalls+api.stopCalls,0);
    EXPECT_EQ(api.engineOff,0);
    EXPECT_FALSE(run.running);
}

TEST(RunOrchestratorTest, LatestSetupGenerationWinsAndStartRejectsPending)
{
    FakeApi api; addAlternateSetup(api); FakeStore store; FakeRun run;
    RunOrchestrator orchestrator(api,store,run);
    const StartRequest first{"A","map-b","tractor-b","plow-b"};
    const StartRequest latest{"B","map-a","tractor-a","plow-a"};
    orchestrator.noteSetupIntent(first,1);
    orchestrator.noteSetupIntent(latest,2);
    EXPECT_FALSE(orchestrator.applySetup(first,1));
    EXPECT_FALSE(orchestrator.start(latest));
    EXPECT_EQ(api.mapCalls+api.tractorCalls+api.implementCalls,0);
    EXPECT_TRUE(orchestrator.applySetup(latest,2));
    EXPECT_EQ(orchestrator.snapshot().confirmedSetup.teamName,"B");
}

TEST(RunOrchestratorTest, TeamOnlySetupPersistsWithoutSimulatorCall)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    ASSERT_TRUE(orchestrator.refresh());
    const int beforeCatalog=api.catalogCalls;
    StartRequest setup{"Renamed Team","map-a","tractor-a","plow-a"};
    orchestrator.noteSetupIntent(setup,1);
    ASSERT_TRUE(orchestrator.applySetup(setup,1));
    EXPECT_EQ(api.catalogCalls,beforeCatalog);
    EXPECT_EQ(api.mapCalls+api.tractorCalls+api.implementCalls,0);
    EXPECT_EQ(orchestrator.snapshot().teamName,"Renamed Team");
}

TEST(RunOrchestratorTest, StartUsesAlreadyConfirmedSetupWithoutSelectionWrites)
{
    FakeApi api; FakeStore store; FakeRun run;
    RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    EXPECT_TRUE(run.running);
    EXPECT_EQ(run.geometry.wheelbaseM,3.1);
    EXPECT_EQ(run.geometry.maxForwardGear,6);
    EXPECT_EQ(o.snapshot().phase,Phase::Running);
    EXPECT_EQ(api.mapCalls+api.tractorCalls+api.implementCalls,0);
}

class StartRollbackTest : public testing::TestWithParam<const char*> {};
TEST_P(StartRollbackTest, EveryFailureCompensatesToSafeIdle)
{
    FakeApi api; FakeStore store; FakeRun run; api.failStage=GetParam();
    RunOrchestrator o(api,store,run);
    EXPECT_FALSE(o.start(request()));
    EXPECT_FALSE(run.running);
    EXPECT_GE(run.safe,1);
    EXPECT_GE(api.engineOff,1);
    const std::string stage=GetParam();
    const bool recordingStartMayHaveTakenEffect=stage=="rec_start"
        || stage=="rec_confirm" || stage=="engine_on" || stage=="engine_confirm";
    EXPECT_EQ(api.stopCalls,recordingStartMayHaveTakenEffect?1:0);
    EXPECT_EQ(o.snapshot().phase,Phase::Failed);
}
INSTANTIATE_TEST_SUITE_P(Stages,StartRollbackTest,
    testing::Values("rec_start","rec_confirm","engine_on","engine_confirm"));

TEST(RunOrchestratorTest, ReadOnlyPreflightFailureDoesNotCompensate)
{
    FakeApi api; api.failStage="catalog"; FakeStore store; FakeRun run;
    RunOrchestrator o(api,store,run);
    EXPECT_FALSE(o.start(request()));
    EXPECT_EQ(run.safe,0);
    EXPECT_EQ(api.engineOff,0);
    EXPECT_EQ(api.stopCalls,0);
}

TEST(RunOrchestratorTest, ControllerRefusalCompensates)
{
    FakeApi api; FakeStore store; FakeRun run; run.refuseRunning=true;
    RunOrchestrator o(api,store,run);
    EXPECT_FALSE(o.start(request()));
    EXPECT_GE(run.safe,1);
    EXPECT_EQ(o.snapshot().phase,Phase::Failed);
}

TEST(RunOrchestratorTest, IdempotentRecorderConflictsAreAcceptedWhenConfirmed)
{
    struct ConflictApi : FakeApi {
        ApiResult recordingStart() override { recorder.recording=true; return {false,409,"already recording"}; }
        ApiResult recordingStop() override { ++stopCalls; recorder.recording=false; return {false,409,"already stopped"}; }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    EXPECT_TRUE(o.finish(false));
}

TEST(RunOrchestratorTest, ResetConflictWithOldRecordingCannotStart)
{
    struct OldRecordingApi : FakeApi {
        ApiResult recordingReset() override {
            recorder.recording=true;
            return {false,409,"old recording still active"};
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    EXPECT_FALSE(o.start(request()));
    EXPECT_FALSE(run.running);
    EXPECT_EQ(api.startCalls,0);
    EXPECT_EQ(o.snapshot().phase,Phase::Failed);
}

TEST(RunOrchestratorTest, EveryNon2xxResetFailsEvenWhenRecorderLooksStopped)
{
    for(const int status : {400,409,500}) {
        struct ResetFailureApi : FakeApi {
            int resetStatus=500;
            ApiResult recordingReset() override {
                recorder.recording=false;
                return {false,resetStatus,"reset rejected"};
            }
        } api;
        api.resetStatus=status;
        FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
        EXPECT_FALSE(o.start(request()));
        EXPECT_EQ(api.startCalls,0);
        EXPECT_FALSE(run.running);
        EXPECT_GE(run.safe,1);
        EXPECT_EQ(o.snapshot().phase,Phase::Failed);
    }
}

TEST(RunOrchestratorTest, DoubleStartDoesNotCompensateOrStopActiveRun)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    const int offBefore=api.engineOff;
    const int stopBefore=api.stopCalls;
    EXPECT_FALSE(o.start(request()));
    EXPECT_TRUE(run.running);
    EXPECT_EQ(api.engineOff,offBefore);
    EXPECT_EQ(api.stopCalls,stopBefore);
    EXPECT_EQ(o.snapshot().phase,Phase::Running);
}

TEST(RunOrchestratorTest, RecorderConfirmationRequiresFreshLiveRecHeaders)
{
    for (const RecorderState bad : {RecorderState{false,true,true,10},
                                    RecorderState{true,true,true,4000}}) {
        struct FrozenRecApi : FakeApi {
            RecorderState frozen;
            ApiResult recordingStart() override { ++startCalls; return {true,200,{}}; }
            ApiResult recordingState(RecorderState& out) override { out=frozen; return {true,200,{}}; }
        } frozen;
        frozen.frozen=bad;
        FakeStore store; FakeRun run; RunOrchestrator o(frozen,store,run);
        EXPECT_FALSE(o.start(request()));
        EXPECT_EQ(o.snapshot().phase,Phase::Failed);
        EXPECT_GE(run.safe,1);
    }
}

TEST(RunOrchestratorTest, RecorderConfirmationTreatsInitial404AsPending)
{
    struct PendingApi : FakeApi {
        int pending=3;
        ApiResult recordingState(RecorderState& out) override {
            if(pending-->0) return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    EXPECT_TRUE(o.start(request()));
    EXPECT_TRUE(run.running);
    EXPECT_EQ(api.startCalls,1);
    EXPECT_EQ(api.stopCalls,0);
}

TEST(RunOrchestratorTest, PersistentRecorder404FailsBoundedAndCleansWithoutArchive)
{
    // SimulatorApiClient maps HTTP 404 "no recording yet" to a successful
    // read with present=false, so the fake reports the absence the same way.
    struct MissingApi : FakeApi {
        ApiResult recordingState(RecorderState& out) override {
            return noRecording(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    EXPECT_FALSE(o.start(request()));
    EXPECT_EQ(o.snapshot().phase,Phase::Failed);
    EXPECT_NE(o.snapshot().error.find("timed out"),std::string::npos);
    EXPECT_GE(api.stopCalls,1);
    EXPECT_EQ(store.calls,0);
    EXPECT_FALSE(run.running);
}

TEST(RunOrchestratorTest, Recorder404ThenShutdownPerformsSafeStopOnlyCleanup)
{
    struct PendingApi : FakeApi {
        std::atomic<bool> observed{false};
        ApiResult recordingState(RecorderState& out) override {
            observed=true;
            return noRecording(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    auto start=std::async(std::launch::async,[&]{return o.start(request());});
    for(int i=0;i<100 && !api.observed.load();++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    ASSERT_TRUE(api.observed.load());
    o.requestShutdown();
    EXPECT_FALSE(start.get());
    EXPECT_GE(api.stopCalls,1);
    EXPECT_EQ(store.calls,0);
    EXPECT_FALSE(run.running);
}

TEST(RunOrchestratorTest, FinishIsExactOnceAndPreservesRawCsv)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    ASSERT_TRUE(o.finish(false));
    ASSERT_TRUE(o.finish(true));
    EXPECT_EQ(store.calls,1);
    EXPECT_EQ(store.bytes,"a,b\n1,2\n");
    EXPECT_EQ(o.snapshot().recordingSha256,"abc");
    EXPECT_EQ(o.snapshot().postRun,PostRunState::AwaitingAction);
    EXPECT_FALSE(o.start(request()));
}

TEST(RunOrchestratorTest, ExplicitCaptureGrantAndDurableAckAreExactRunBound)
{
    FakeApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
    const std::string id=o.snapshot().recordingId;
    EXPECT_FALSE(o.beginSealedCapture("other"));
    ASSERT_TRUE(o.beginSealedCapture(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::CaptureInProgress);
    EXPECT_FALSE(o.beginSealedCapture(id));
    ASSERT_TRUE(o.acknowledgeSealedCapture(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::CaptureDurable);
    EXPECT_TRUE(o.acknowledgeSealedCapture(id));
}

TEST(RunOrchestratorTest, ResetRequiresRefreshThenExactResetAnd404Proof)
{
    struct ResetApi:FakeApi{
        std::vector<std::string> order;
        bool afterReset=false;
        ApiResult simulatorRefresh() override{++refreshCalls;order.push_back("refresh");return result("sim_refresh");}
        ApiResult recordingReset() override{++resetCalls;order.push_back("reset");afterReset=true;return result("rec_reset");}
        ApiResult recordingStart() override{
            afterReset=false;
            return FakeApi::recordingStart();
        }
        ApiResult recordingState(RecorderState& out) override{
            if(afterReset)return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
    api.order.clear();
    const std::string id=o.snapshot().recordingId;
    ASSERT_TRUE(o.beginResetCleanup(id));
    const auto resetGeneration=o.snapshot().resetGeneration;
    EXPECT_GT(resetGeneration,0u);
    ASSERT_TRUE(o.resetCompletedRun());
    EXPECT_EQ(api.order,(std::vector<std::string>{"refresh","reset"}));
    EXPECT_EQ(store.purgeCalls,1);
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPending);
    EXPECT_FALSE(o.start(request()));
    ASSERT_TRUE(o.acknowledgeResetCleanup(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::None);
    EXPECT_EQ(o.snapshot().resetGeneration,resetGeneration);
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Idle);
    EXPECT_TRUE(o.snapshot().recordingId.empty());
    EXPECT_TRUE(o.start(request()));
}

TEST(RunOrchestratorTest, ResetWaitsForPostRefreshStoppedSelectionAndRetriesOnly503)
{
    struct ResetApi:FakeApi {
        bool afterRefresh=false,afterReset=false,keepUnready=false;
        int transient503=0;
        ApiResult simulatorRefresh() override {++refreshCalls;afterRefresh=true;return {true,200,{}};}
        ApiResult selection(Selection& out) override {
            out=selected;++selectionCalls;
            if(afterRefresh && keepUnready)out.live=false;
            return {true,200,{}};
        }
        ApiResult recordingReset() override {
            ++resetCalls;
            if(!afterRefresh){recorder.recording=false;return {true,200,{}};}
            if(transient503-->0)return {false,503,"simulator not ready"};
            afterReset=true;recorder.recording=false;return {true,200,{}};
        }
        ApiResult recordingState(RecorderState& out) override {
            if(afterRefresh && afterReset)return noRecording(out);
            return FakeApi::recordingState(out);
        }
    };
    {
        ResetApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
        ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
        ASSERT_TRUE(o.beginResetCleanup(o.snapshot().recordingId));api.transient503=2;
        EXPECT_TRUE(o.resetCompletedRun());EXPECT_EQ(api.resetCalls,4); // Start reset + 2 transient + success.
    }
    {
        ResetApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
        ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
        ASSERT_TRUE(o.beginResetCleanup(o.snapshot().recordingId));api.keepUnready=true;
        EXPECT_FALSE(o.resetCompletedRun());EXPECT_EQ(api.resetCalls,1); // Start reset only.
        EXPECT_NE(o.snapshot().error.find("post-refresh"),std::string::npos);
    }
}

TEST(RunOrchestratorTest, ResetNon503FailureReportsSafeStatusAndNeverCompletes)
{
    struct ConflictApi:FakeApi {
        bool resetPhase=false;
        ApiResult simulatorRefresh() override {++refreshCalls;resetPhase=true;return {true,200,{}};}
        ApiResult recordingReset() override {
            ++resetCalls;
            if(resetPhase)return {false,409,"sensitive upstream body"};
            recorder.recording=false;return {true,200,{}};
        }
    } api;
    FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
    ASSERT_TRUE(o.beginResetCleanup(o.snapshot().recordingId));
    EXPECT_FALSE(o.resetCompletedRun());
    EXPECT_NE(o.snapshot().error.find("HTTP 409 conflict"),std::string::npos);
    EXPECT_EQ(o.snapshot().error.find("sensitive"),std::string::npos);
    EXPECT_EQ(store.purgeCalls,0);EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetRetryable);
}

TEST(RunOrchestratorTest, ResetCannotTouchSimulatorBeforeBrowserDrainAndTimeoutIsRetryable)
{
    FakeApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
    const std::string id=o.snapshot().recordingId;
    EXPECT_FALSE(o.resetCompletedRun());
    EXPECT_EQ(api.refreshCalls,0);EXPECT_EQ(api.resetCalls,1); // Start reset only.
    ASSERT_TRUE(o.beginResetCleanup(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPreparing);
    ASSERT_TRUE(o.failResetCleanup(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetRetryable);
    EXPECT_FALSE(o.snapshot().busy);
    EXPECT_EQ(api.refreshCalls,0);
    ASSERT_TRUE(o.beginResetCleanup(id));
    EXPECT_EQ(o.snapshot().resetGeneration,2u);
}

// 대시보드는 여러 탭이 동시에 붙을 수 있다. 낡은 탭 하나가 아웃박스를 비우지
// 못했다고 보고하는 바람에, 정상 탭이 이미 성립시킨 리셋이 통째로 취소돼 다음
// 주행을 시작할 수 없었다. 성립한 리셋은 뒤늦은 실패 보고로 무를 수 없다.
//
// Several dashboard tabs can be attached at once. One stale tab reporting that
// it could not drain its outbox cancelled the reset a healthy tab had already
// admitted, leaving the next run impossible to start. An admitted reset cannot
// be undone by a late failure report.
// refresh 는 작업기를 내려놓는다(Issue #9 §2.19). 그런데 리셋 확인이 주행 때의
// 작업기 식별자를 계속 요구하는 바람에, 그 조건이 영영 만족되지 않아 리셋이
// 매번 "run reset confirmation timed out" 으로 끝나고 다음 주행을 시작할 수
// 없었다.
//
// A refresh drops the implement (Issue #9 §2.19), but the reset confirmation
// kept requiring the run's implement identifier — a condition that could never
// be met, so every reset ended in "run reset confirmation timed out" and no
// further run could be started.
TEST(RunOrchestratorTest, ResetAcceptsTheImplementThatRefreshDropped)
{
    struct RefreshApi : FakeApi {
        bool afterReset=false;
        ApiResult simulatorRefresh() override {
            ++refreshCalls;
            selected.implementId="none";   // Issue #9 §2.19
            return result("sim_refresh");
        }
        ApiResult recordingReset() override {++resetCalls;afterReset=true;return result("rec_reset");}
        ApiResult recordingStart() override {afterReset=false;return FakeApi::recordingStart();}
        ApiResult recordingState(RecorderState& out) override {
            if(afterReset)return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); ASSERT_TRUE(o.finish(false));
    ASSERT_TRUE(o.beginResetCleanup(o.snapshot().recordingId));
    ASSERT_TRUE(o.noteResetCleanupPrepared(o.snapshot().resetGeneration));
    EXPECT_TRUE(o.resetCompletedRun());
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPending);
    EXPECT_EQ(o.snapshot().error,"");
}

TEST(RunOrchestratorTest, StaleClientDrainFailureCannotCancelAnAdmittedReset)
{
    struct ResetApi : FakeApi {
        bool afterReset=false;
        ApiResult recordingReset() override {++resetCalls;afterReset=true;return result("rec_reset");}
        ApiResult recordingStart() override {afterReset=false;return FakeApi::recordingStart();}
        ApiResult recordingState(RecorderState& out) override {
            if(afterReset)return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); ASSERT_TRUE(o.finish(false));
    const std::string id=o.snapshot().recordingId;
    ASSERT_TRUE(o.beginResetCleanup(id));
    const auto generation=o.snapshot().resetGeneration;
    ASSERT_TRUE(o.noteResetCleanupPrepared(generation));
    EXPECT_TRUE(o.failResetCleanup(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPreparing);
    EXPECT_EQ(o.snapshot().error,"");
    ASSERT_TRUE(o.resetCompletedRun());
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPending);
}

TEST(RunOrchestratorTest, DrainFailureStillStopsAResetNoClientAdmitted)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); ASSERT_TRUE(o.finish(false));
    const std::string id=o.snapshot().recordingId;
    ASSERT_TRUE(o.beginResetCleanup(id));
    // 이전 세대의 성립 기록이 다음 리셋까지 넘어와서는 안 된다.
    // An admission from an earlier generation must not carry into the next reset.
    EXPECT_FALSE(o.noteResetCleanupPrepared(o.snapshot().resetGeneration+1));
    EXPECT_TRUE(o.failResetCleanup(id));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetRetryable);
    EXPECT_EQ(api.refreshCalls,0);
}

TEST(RunOrchestratorTest, StandalonePurgeGetsPositiveGenerationAndUsesFullResetLifecycle)
{
    struct PurgeApi:FakeApi {
        bool afterReset=false;
        ApiResult recordingReset() override {++resetCalls;afterReset=true;return result("rec_reset");}
        ApiResult recordingState(RecorderState& out) override {
            if(afterReset)return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.refresh());
    EXPECT_TRUE(o.snapshot().recordingId.empty());
    ASSERT_TRUE(o.beginPurgeAllCleanup());
    const auto generation=o.snapshot().resetGeneration;
    EXPECT_GT(generation,0u);
    ASSERT_TRUE(o.resetCompletedRun());
    EXPECT_EQ(api.refreshCalls,1);EXPECT_EQ(api.resetCalls,1);EXPECT_EQ(store.purgeCalls,1);
    EXPECT_EQ(o.snapshot().postRun,PostRunState::ResetCleanupPending);
    ASSERT_TRUE(o.acknowledgeResetCleanup(""));
    EXPECT_EQ(o.snapshot().postRun,PostRunState::None);
    EXPECT_EQ(o.snapshot().resetGeneration,generation);
}

TEST(RunOrchestratorTest, SubmittedLedgerAcceptsDurableCurrentRunAndArchivedRuns)
{
    FakeApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));const auto id=o.snapshot().recordingId;
    EXPECT_FALSE(o.markLeaderboardSubmitted(id,"SCORED"));
    ASSERT_TRUE(o.beginSealedCapture(id));ASSERT_TRUE(o.acknowledgeSealedCapture(id));
    EXPECT_FALSE(o.markLeaderboardSubmitted(id,"BAD"));
    // 과거(보관) 주행의 뒤늦은 제출 보고: 원장에만 기록되고 현재 주행의 주행 후
    // 상태 전이는 일어나지 않는다. / A late report for an archived run writes
    // the ledger only and never transitions the current run's post-run state.
    EXPECT_TRUE(o.markLeaderboardSubmitted("other","SCORED"));
    EXPECT_EQ(store.submittedCalls,1);
    EXPECT_EQ(o.snapshot().postRun,PostRunState::CaptureDurable);
    EXPECT_FALSE(o.markLeaderboardSubmitted("other","BAD"));
    EXPECT_TRUE(o.markLeaderboardSubmitted(id,"INVALID"));EXPECT_EQ(store.submittedCalls,2);
    EXPECT_EQ(o.snapshot().postRun,PostRunState::Submitted);
    EXPECT_TRUE(o.acknowledgeSealedCapture(id));
    EXPECT_TRUE(o.markLeaderboardSubmitted(id,"INVALID"));
    EXPECT_EQ(store.submittedCalls,2);
}

TEST(RunOrchestratorTest, ResetFailureKeepsCompletedRunAndCaptureInProgressRejectsReset)
{
    FakeApi api;FakeStore store;FakeRun run;RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));ASSERT_TRUE(o.finish(false));
    const std::string id=o.snapshot().recordingId;
    ASSERT_TRUE(o.beginSealedCapture(id));
    EXPECT_FALSE(o.beginResetCleanup(id));
    EXPECT_FALSE(o.resetCompletedRun());EXPECT_EQ(api.refreshCalls,0);
    ASSERT_TRUE(o.acknowledgeSealedCapture(id));api.failStage="sim_refresh";
    ASSERT_TRUE(o.beginResetCleanup(id));
    EXPECT_FALSE(o.resetCompletedRun());
    EXPECT_EQ(o.snapshot().recordingId,id);
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Completed);
}

// 엔진 정지는 관측이 끝난 뒤에 와야 한다. 시뮬레이터는 엔진 정지를 물리 루프
// 정지로 구현하고 스냅샷도 그 루프가 내보내므로, 먼저 끄면 방금 명령한 정지를
// 확인할 방법이 사라진다 — 실제로 Finish 가 여기서 타임아웃으로 끝났다.
//
// The engine must be cut last. The simulator implements engine-off by stopping
// the physics loop that also publishes the snapshots, so cutting it first
// removes any way to confirm the stop just commanded — which is exactly how
// Finish used to time out.
TEST(RunOrchestratorTest, FinishConfirmsStopAndRecorderBeforeCuttingTheEngine)
{
    struct CoastingApi : FakeApi {
        bool finishing=false;
        bool safeAtEngineOff=false;
        FakeRun* observedRun=nullptr;
        std::size_t stopIndex=0;
        std::vector<Selection> stopSamples;
        std::vector<std::string> events;
        ApiResult engine(bool on) override {
            events.push_back(on?"engine_on":"engine_off");
            if(!on) {
                safeAtEngineOff=observedRun && observedRun->safe>0
                    && !observedRun->running;
            }
            return FakeApi::engine(on);
        }
        ApiResult selection(Selection& out) override {
            if(finishing && !stopSamples.empty()) {
                out=stopSamples[std::min(stopIndex,stopSamples.size()-1)];
                ++stopIndex; ++selectionCalls; events.push_back("selection");
                return {true,200,{}};
            }
            return FakeApi::selection(out);
        }
        ApiResult recordingStop() override {
            events.push_back("rec_stop");
            return FakeApi::recordingStop();
        }
    } api;
    FakeStore store; FakeRun run; api.observedRun=&run;
    RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    api.events.clear();
    Selection moving=api.selected;
    moving.reactRunning=true; moving.stopped=false;
    Selection coast=moving; coast.reactRunning=false;
    Selection stopped=coast; stopped.stopped=true;
    // Two acceptable samples are deliberately broken by another coasting
    // sample: finalization needs three consecutive confirmations.
    api.stopSamples={moving,stopped,stopped,coast,stopped,stopped,stopped};
    const int safeBefore=run.safe;
    api.finishing=true;
    ASSERT_TRUE(o.finish(true));
    EXPECT_TRUE(api.safeAtEngineOff);
    EXPECT_GT(run.safe,safeBefore);
    const auto recStop=std::find(api.events.begin(),api.events.end(),"rec_stop");
    const auto engineOff=std::find(api.events.begin(),api.events.end(),"engine_off");
    ASSERT_NE(recStop,api.events.end());
    ASSERT_NE(engineOff,api.events.end());
    EXPECT_LT(recStop,engineOff);
    // Seven samples: three consecutive stops, broken once by a coasting sample.
    EXPECT_EQ(std::count(api.events.begin(),recStop,"selection"),7);
    EXPECT_EQ(api.engineOff,1);
    EXPECT_EQ(store.calls,1);
    EXPECT_TRUE(run.finished);
}

// 엔진을 끄면 시뮬레이터의 물리 루프가 서고, 화면 탭이 가려져 있으면 스냅샷
// 발행까지 멈춘다. 관측이 끊긴 것 자체가 루프가 섰다는 증거다 — 여기서 실패로
// 돌아서면 이미 아카이브까지 끝난 주행이 retryable_error 에 갇힌다.
//
// Cutting the engine stops the simulator's physics loop, and with the page
// hidden the snapshot publisher goes with it. Losing the observation is itself
// proof the loop is down; failing here would strand an already-archived run in
// retryable_error.
TEST(RunOrchestratorTest, EngineOffIsConfirmedWhenTheSimulatorStopsReporting)
{
    struct FreezingApi : FakeApi {
        bool frozen=false;
        ApiResult engine(bool on) override {
            const ApiResult result=FakeApi::engine(on);
            if(!on) frozen=true;
            return result;
        }
        ApiResult selection(Selection& out) override {
            const ApiResult result=FakeApi::selection(out);
            if(frozen) { out.live=false; out.snapshotAgeMs=32000; out.stopped=false; }
            return result;
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    ASSERT_TRUE(o.finish(false));
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Completed);
    EXPECT_EQ(store.calls,1);
    EXPECT_TRUE(run.finished);
}

// 기록이 없다는 404 는 확답이다. 이것을 통신 오류로 취급하는 동안 Finish 는
// "recording stop confirmation timed out" 으로만 끝났고, 그 뒤 모든 Start 가
// 조용히 거절됐다.
//
// A 404 saying there is no recording is an answer. While it was treated as a
// transport error, Finish could only end in "recording stop confirmation timed
// out", after which every Start was silently refused.
TEST(RunOrchestratorTest, FinishTreatsAnAbsentRecorderAsAConfirmedStop)
{
    struct VanishingApi : FakeApi {
        bool vanished=false;
        ApiResult recordingStop() override {
            vanished=true;
            return FakeApi::recordingStop();
        }
        ApiResult recordingState(RecorderState& out) override {
            if(vanished) return noRecording(out);
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    ASSERT_TRUE(o.finish(false));
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Completed);
    EXPECT_EQ(o.snapshot().error,"");
    EXPECT_EQ(store.calls,1);
}

// 주행 중 시뮬레이터 페이지가 다시 로드되면 레코더 버퍼가 사라지고, 그 뒤의
// 모든 기록 조회는 404 다. 예전에는 이것이 RetryableError 로 남아 Finish 재시도가
// 영원히 같은 404 를 받았고 앱 재시작만이 출구였다. 이제는 종결이다: 다음 Start
// 가 앱 재시작 없이 허용되고, 지속 알림 채널로 이유가 나간다.
//
// A mid-run simulator page reload destroys the recorder buffer and every read
// after that is a 404. This used to sit in RetryableError with every Finish
// retry reading the same 404 forever, app restart being the only exit. It is
// terminal now: the next Start is admitted without a restart and the reason
// goes out on the durable notice channel.
TEST(RunOrchestratorTest, VanishedRecordingIsTerminalAndFreesTheNextStart)
{
    struct LostApi : FakeApi {
        bool lost=false;
        ApiResult recordingState(RecorderState& out) override {
            if(lost) return noRecording(out);
            return FakeApi::recordingState(out);
        }
        ApiResult recording(Recording& out) override {
            if(lost) return {false,404,"no recording yet"};
            return FakeApi::recording(out);
        }
    } api;
    FakeStore store; FakeRun run;
    std::vector<std::string> notices;
    RunOrchestrator o(api,store,run,{},{},
        [&](const std::string& notice){notices.push_back(notice);});
    ASSERT_TRUE(o.start(request()));
    api.lost=true;
    EXPECT_FALSE(o.finish(false));
    const auto snap=o.snapshot();
    EXPECT_EQ(snap.finalization,FinalizationState::Idle);
    EXPECT_EQ(snap.postRun,PostRunState::None);
    EXPECT_TRUE(snap.recordingId.empty());
    EXPECT_NE(snap.error.find("recorder no longer holds"),std::string::npos)
        << snap.error;
    EXPECT_EQ(store.calls,0);
    ASSERT_EQ(notices.size(),1u);
    EXPECT_NE(notices.front().find("no retry can recover"),std::string::npos);
    EXPECT_GE(api.engineOff,1);
    // 앱 재시작 없이 다음 주행이 시작돼야 한다.
    // The next run must start without an app restart.
    api.lost=false;
    std::string refusal;
    EXPECT_TRUE(o.start(request(),&refusal)) << refusal;
    EXPECT_EQ(o.snapshot().phase,Phase::Running);
}

TEST(RunOrchestratorTest, PhysicalStopTimeoutIsRetryableWithoutArchiveOrFinalEvent)
{
    struct StopGateApi : FakeApi {
        bool finishing=false;
        bool allowStop=false;
        ApiResult selection(Selection& out) override {
            ApiResult result=FakeApi::selection(out);
            if(finishing) {
                out.reactRunning=false;
                out.stopped=allowStop;
                out.live=true;
                out.snapshotAgeMs=10;
            }
            return result;
        }
    } api;
    FakeStore store; FakeRun run; std::atomic<int> finalized{0};
    RunOrchestrator o(api,store,run,{},[&](const std::string&,const std::string&){++finalized;});
    ASSERT_TRUE(o.start(request()));
    api.finishing=true;
    EXPECT_FALSE(o.finish(true));
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::RetryableError);
    EXPECT_EQ(api.stopCalls,0);
    EXPECT_EQ(store.calls,0);
    EXPECT_EQ(finalized.load(),0);
    EXPECT_FALSE(run.running);
    EXPECT_FALSE(run.finished);
    api.allowStop=true;
    EXPECT_TRUE(o.finish(false));
    EXPECT_EQ(api.stopCalls,1);
    EXPECT_EQ(store.calls,1);
    EXPECT_EQ(finalized.load(),1);
    EXPECT_TRUE(run.finished);
}

// 거절은 이유를 말해야 한다. 이 가드가 조용히 false 만 돌려주던 동안 Start 는
// 앱을 다시 띄우기 전까지 화면에 아무 흔적도 남기지 않고 계속 무시됐다.
//
// A refusal has to say why. While this guard returned a bare false, Start went
// on being ignored with no trace on screen until the app was restarted.
TEST(RunOrchestratorTest, StartRefusalNamesTheGuardThatBlockedIt)
{
    struct RefusalApi : FakeApi {
        bool failFinishState=false;
        ApiResult recordingState(RecorderState& out) override {
            if(failFinishState){failFinishState=false;return {false,503,"rec unavailable"};}
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));

    std::string refusal;
    EXPECT_FALSE(o.start(request(),&refusal));
    EXPECT_NE(refusal.find("already in progress"),std::string::npos) << refusal;

    api.failFinishState=true;
    ASSERT_FALSE(o.finish(true));
    ASSERT_EQ(o.snapshot().finalization,FinalizationState::RetryableError);

    // 이게 사용자가 갇혔던 상태다: 무엇을 해야 하는지가 문장에 있어야 한다.
    // This is the state operators got stuck in; the way out must be in the text.
    refusal.clear();
    EXPECT_FALSE(o.start(request(),&refusal));
    EXPECT_NE(refusal.find("Finish"),std::string::npos) << refusal;

    // 이유를 원하지 않는 호출자도 그대로 동작해야 한다.
    // Callers that do not want a reason keep working unchanged.
    EXPECT_FALSE(o.start(request()));
}

// 낡은 desiredSetup 이 Start 를 영구히 막던 함정. confirmed 와 시뮬레이터가
// 모두 요청과 같으면 설정은 이미 반영된 것이다.
//
// The trap where a stale desiredSetup blocked Start forever. When confirmed and
// the simulator both equal the request, the setup is already applied.
TEST(RunOrchestratorTest, StaleDesiredSetupDoesNotBlockAnAppliedStart)
{
    // 작업기 선택만 실패하고 시뮬레이터는 그대로인 상황 — set_sim_setup 이
    // 반쯤 실패하면 의도만 앞서 나간다.
    // Only the implement selection fails and the simulator is left untouched:
    // a half-failed set_sim_setup leaves intent running ahead.
    struct AheadApi : FakeApi {
        bool refuseImplement=false;
        ApiResult selectImplement(const std::string& id) override {
            if(refuseImplement) return {false,503,"implement"};
            return FakeApi::selectImplement(id);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    const StartRequest applied=request();
    o.noteSetupIntent(applied,1);
    ASSERT_TRUE(o.applySetup(applied,1));

    StartRequest ahead=applied; ahead.implementId="implement-other";
    api.refuseImplement=true;
    o.noteSetupIntent(ahead,2);
    EXPECT_FALSE(o.applySetup(ahead,2));
    api.refuseImplement=false;
    ASSERT_EQ(o.snapshot().desiredSetup.implementId,ahead.implementId);
    ASSERT_EQ(o.snapshot().confirmedSetup.implementId,applied.implementId);

    std::string refusal;
    EXPECT_TRUE(o.start(applied,&refusal)) << refusal;
    EXPECT_TRUE(refusal.empty()) << refusal;
    EXPECT_EQ(o.snapshot().desiredSetup.implementId,applied.implementId);
    EXPECT_EQ(o.snapshot().confirmedSetup.implementId,applied.implementId);
}

// 시뮬레이터가 요청과 다르면 종전대로 거절해야 한다 — 위 완화가 안전장치를
// 통째로 없앤 것이 아님을 고정한다.
//
// A simulator that disagrees must still be refused: pin that the relaxation
// above did not remove the guard.
TEST(RunOrchestratorTest, StartStillRefusedWhenTheSimulatorDisagrees)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    StartRequest elsewhere=request(); elsewhere.implementId="implement-other";
    std::string refusal;
    EXPECT_FALSE(o.start(elsewhere,&refusal));
    EXPECT_FALSE(refusal.empty());
}

// 시뮬레이터 페이지를 다시 열면 작업기 선택이 해제된다. 예전에는 그 상태의
// Start 가 "selected IDs mismatch" 로 거절됐고, 화면에는 빠져나갈 안내가 없어
// 드롭다운을 두 번 토글해야만 풀렸다. Start 요청이 곧 사용자가 명명한 설정이므로
// Start 가 직접 재정렬한다.
//
// Reopening the simulator page drops the implement selection. Start used to
// refuse that state with "selected IDs mismatch" and nothing on screen said the
// way out was toggling a dropdown twice. The Start request names the exact
// setup, so Start realigns it itself.
TEST(RunOrchestratorTest, StartRealignsDriftedSimulatorSelectionInsteadOfRefusing)
{
    FakeApi api; FakeStore store; FakeRun run;
    api.selected.implementId="none";
    RunOrchestrator o(api,store,run);
    std::string refusal;
    EXPECT_TRUE(o.start(request(),&refusal)) << refusal;
    EXPECT_EQ(api.implementCalls,1);
    EXPECT_EQ(api.selected.implementId,"plow-a");
    const auto snap=o.snapshot();
    EXPECT_EQ(snap.phase,Phase::Running);
    EXPECT_EQ(snap.confirmedSetup.implementId,"plow-a");
    EXPECT_EQ(snap.desiredSetup.implementId,"plow-a");
}

TEST(RunOrchestratorTest, RecorderStateFailureCanBeRetriedToOneSuccessfulArchive)
{
    struct RetryApi : FakeApi {
        bool failFinishState=false;
        ApiResult recordingState(RecorderState& out) override {
            if(failFinishState){failFinishState=false;return {false,503,"rec unavailable"};}
            return FakeApi::recordingState(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); api.failFinishState=true;
    EXPECT_FALSE(o.finish(true));
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::RetryableError);
    EXPECT_TRUE(o.finish(false));
    EXPECT_EQ(store.calls,1);
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Completed);
}

TEST(RunOrchestratorTest, RecorderStop503CanBeRetried)
{
    struct StopRetryApi : FakeApi {
        bool failStop=false;
        ApiResult recordingStop() override {
            ++stopCalls;
            if(failStop){failStop=false;return {false,503,"rec stop unavailable"};}
            recorder.recording=false;
            return {true,200,{}};
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); api.failStop=true;
    EXPECT_FALSE(o.finish(true));
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::RetryableError);
    EXPECT_TRUE(o.finish(false));
    EXPECT_EQ(store.calls,1);
}

TEST(RunOrchestratorTest, StaleStopTimeoutCanBeRetried)
{
    struct StaleApi : FakeApi {
        bool stale=false;
        ApiResult recordingState(RecorderState& out) override {
            out=recorder;
            if(stale)out.snapshotAgeMs=4001;
            return {true,200,{}};
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); api.stale=true;
    EXPECT_FALSE(o.finish(true));
    api.stale=false;
    EXPECT_TRUE(o.finish(false));
    EXPECT_EQ(store.calls,1);
}

TEST(RunOrchestratorTest, CsvFetchAndDiskFailuresAreRetryable)
{
    struct FetchApi : FakeApi {
        int fetchFailures=1;
        ApiResult recording(Recording& out) override {
            if(fetchFailures>0){--fetchFailures;return {false,503,"csv unavailable"};}
            return FakeApi::recording(out);
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    EXPECT_FALSE(o.finish(true));
    EXPECT_TRUE(o.finish(false));
    EXPECT_EQ(store.calls,1);

    FakeApi api2; FakeStore store2; store2.failures=1; FakeRun run2;
    RunOrchestrator o2(api2,store2,run2);
    ASSERT_TRUE(o2.start(request()));
    EXPECT_FALSE(o2.finish(true));
    EXPECT_TRUE(o2.finish(false));
    EXPECT_EQ(store2.calls,2);
    EXPECT_EQ(o2.snapshot().finalization,FinalizationState::Completed);
}

TEST(RunOrchestratorTest, ConcurrentAutoAndManualFinishArchiveExactlyOnce)
{
    struct SlowFetchApi : FakeApi {
        ApiResult recording(Recording& out) override {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return FakeApi::recording(out);
        }
    } api;
    FakeStore store; FakeRun run; std::atomic<int> finalized{0};
    std::atomic<bool> callbackSawArchive{false},callbackSawFinished{false};
    std::string finalTeam,finalRun;
    RunOrchestrator o(api,store,run,{},[&](const std::string& team,const std::string& id){
        callbackSawArchive=store.calls==1;
        callbackSawFinished=run.finished;
        ++finalized;finalTeam=team;finalRun=id;
    });
    ASSERT_TRUE(o.start(request()));
    std::atomic<bool> first{false},second{false};
    std::thread automatic([&]{first=o.finish(true);});
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::thread manual([&]{second=o.finish(false);});
    automatic.join(); manual.join();
    EXPECT_NE(first.load(),second.load());
    EXPECT_EQ(store.calls,1);
    EXPECT_EQ(finalized.load(),1);EXPECT_EQ(finalTeam,"Team Alpha");EXPECT_FALSE(finalRun.empty());
    EXPECT_TRUE(callbackSawArchive.load());
    EXPECT_TRUE(callbackSawFinished.load());
    EXPECT_EQ(o.snapshot().finalization,FinalizationState::Completed);
}

TEST(RunOrchestratorTest, ShutdownInterruptsRecorderPollingPromptly)
{
    struct NeverFreshApi : FakeApi {
        bool stale=false;
        ApiResult recordingState(RecorderState& out) override {
            out=recorder;
            if(stale)out.snapshotAgeMs=4001;
            return {true,200,{}};
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request())); api.stale=true;
    const auto begin=std::chrono::steady_clock::now();
    std::thread worker([&]{EXPECT_FALSE(o.finish(true));});
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    o.requestShutdown(); worker.join();
    const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now()-begin).count();
    EXPECT_LT(elapsed,600);
    EXPECT_FALSE(run.running);
}

enum class StartBarrierStage {
    MapApply,
    TractorApply,
    ImplementApply,
    BeforeReset,
    AfterResetBeforeStartRequest,
    AfterStartResponse,
    DuringConfirmation,
    AfterRecordingOwnership
};

struct StartBarrierApi : FakeApi {
    explicit StartBarrierApi(StartBarrierStage value,
                             std::shared_future<void> releaseFuture)
        : stage(value), release(std::move(releaseFuture)) {}

    void hold(StartBarrierStage expected)
    {
        if(stage!=expected) return;
        reached.set_value();
        release.wait();
    }

    ApiResult selectMap(const std::string& id) override
    { hold(StartBarrierStage::MapApply); return FakeApi::selectMap(id); }
    ApiResult selectTractor(const std::string& id) override
    { hold(StartBarrierStage::TractorApply); return FakeApi::selectTractor(id); }
    ApiResult selectImplement(const std::string& id) override
    { hold(StartBarrierStage::ImplementApply); return FakeApi::selectImplement(id); }
    ApiResult recordingReset() override
    {
        hold(StartBarrierStage::BeforeReset);
        const ApiResult result=FakeApi::recordingReset();
        hold(StartBarrierStage::AfterResetBeforeStartRequest);
        return result;
    }
    ApiResult engine(bool on) override
    {
        if(on) hold(StartBarrierStage::AfterRecordingOwnership);
        return FakeApi::engine(on);
    }
    ApiResult recordingState(RecorderState& out) override
    {
        ++confirmationCalls;
        if(stage==StartBarrierStage::AfterStartResponse && confirmationCalls==1)
            hold(stage);
        if(stage==StartBarrierStage::DuringConfirmation) {
            if(confirmationCalls==1) {
                out=recorder;
                out.recording=false;
                return {true,200,{}};
            }
            if(confirmationCalls==2) hold(stage);
        }
        return FakeApi::recordingState(out);
    }
    ApiResult recording(Recording& out) override
    { ++fetchCalls; return FakeApi::recording(out); }

    StartBarrierStage stage;
    std::promise<void> reached;
    std::shared_future<void> release;
    int fetchCalls=0;
    int confirmationCalls=0;
};

class PreOwnershipFinishTest : public testing::TestWithParam<StartBarrierStage> {};

TEST_P(PreOwnershipFinishTest, CancellationNeverTouchesAmbiguousRecorderBytes)
{
    std::promise<void> releasePromise;
    StartBarrierApi api(GetParam(),releasePromise.get_future().share());
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    std::atomic<bool> startResult{true};
    std::thread worker([&]{startResult=orchestrator.start(request());});
    api.reached.get_future().wait();
    run.cancel=true;
    releasePromise.set_value();
    worker.join();

    EXPECT_FALSE(startResult.load());
    EXPECT_TRUE(orchestrator.finish(false));
    EXPECT_FALSE(run.running);
    EXPECT_GE(run.safe,1);
    EXPECT_GE(api.engineOff,1);
    EXPECT_EQ(api.stopCalls,0);
    EXPECT_EQ(api.fetchCalls,0);
    EXPECT_EQ(store.calls,0);
    EXPECT_EQ(orchestrator.snapshot().finalization,FinalizationState::Idle);
}

INSTANTIATE_TEST_SUITE_P(BeforeRecorderOwnership,PreOwnershipFinishTest,
    testing::Values(StartBarrierStage::BeforeReset,
                    StartBarrierStage::AfterResetBeforeStartRequest));

class CleanupObligationFinishTest : public testing::TestWithParam<StartBarrierStage> {};

TEST_P(CleanupObligationFinishTest, CancellationStopsButNeverArchivesUnconfirmedRecording)
{
    std::promise<void> releasePromise;
    StartBarrierApi api(GetParam(),releasePromise.get_future().share());
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    std::atomic<bool> startResult{true};
    std::thread worker([&]{startResult=orchestrator.start(request());});
    api.reached.get_future().wait();
    run.cancel=true;
    releasePromise.set_value();
    worker.join();

    EXPECT_FALSE(startResult.load());
    EXPECT_TRUE(orchestrator.finish(false));
    EXPECT_FALSE(run.running);
    EXPECT_EQ(api.stopCalls,1);
    EXPECT_EQ(api.fetchCalls,0);
    EXPECT_EQ(store.calls,0);
    EXPECT_EQ(orchestrator.snapshot().finalization,FinalizationState::Idle);
}

INSTANTIATE_TEST_SUITE_P(AfterStartBeforeOwnership,CleanupObligationFinishTest,
    testing::Values(StartBarrierStage::AfterStartResponse,
                    StartBarrierStage::DuringConfirmation));

TEST(RunOrchestratorTest, EveryAmbiguousStartResponseHasBoundedStopReconciliation)
{
    struct LateAppliedApi : FakeApi {
        int fetchCalls=0;
        int reconcileCalls=0;
        ApiResult recordingStart() override {
            ++startCalls;
            recorder.recording=false;
            return {false,504,"start response timed out"};
        }
        ApiResult recordingStop() override {
            ++stopCalls;
            if(stopCalls>=2) recorder.recording=false;
            return {true,200,{}};
        }
        ApiResult recordingState(RecorderState& out) override {
            ++reconcileCalls;
            // The initial Stop sees the recorder stopped. The timed-out Start
            // applies after that first poll and persists until Stop is reissued.
            if(reconcileCalls==2) recorder.recording=true;
            out=recorder;
            return {true,200,{}};
        }
        ApiResult recording(Recording& out) override
        { ++fetchCalls; return FakeApi::recording(out); }
    } late;

    FakeStore store1; FakeRun run1; RunOrchestrator first(late,store1,run1);
    EXPECT_FALSE(first.start(request()));
    EXPECT_GE(late.stopCalls,2);
    EXPECT_GE(late.reconcileCalls,5);
    EXPECT_EQ(late.fetchCalls,0);
    EXPECT_EQ(store1.calls,0);

    struct DefinitelyRejectedApi : FakeApi {
        ApiResult recordingStart() override {
            ++startCalls;
            recorder.recording=false;
            return {false,400,"rejected"};
        }
    } rejected;
    FakeStore store2; FakeRun run2; RunOrchestrator second(rejected,store2,run2);
    EXPECT_FALSE(second.start(request()));
    EXPECT_EQ(rejected.stopCalls,1);
    EXPECT_EQ(store2.calls,0);
}

TEST(RunOrchestratorTest, LateStartAtReconciliationDeadlineIsRetryable)
{
    struct DeadlineApi : FakeApi {
        int stateCalls=0;
        bool retryPhase=false;
        ApiResult recordingStart() override {
            ++startCalls;
            recorder.recording=false;
            return {false,504,"start response timed out"};
        }
        ApiResult recordingStop() override {
            ++stopCalls;
            if(retryPhase) recorder.recording=false;
            return {true,200,{}};
        }
        ApiResult recordingState(RecorderState& out) override {
            ++stateCalls;
            out=recorder;
            if(!retryPhase) {
                out.live=false;
                if(stateCalls==30) {
                    recorder.recording=true;
                    out={true,true,true,10};
                }
            }
            return {true,200,{}};
        }
        ApiResult recording(Recording& out) override {
            ++fetchCalls;
            return FakeApi::recording(out);
        }
        int fetchCalls=0;
    } api;
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);

    EXPECT_FALSE(orchestrator.start(request()));
    EXPECT_EQ(orchestrator.snapshot().finalization,FinalizationState::RetryableError);
    EXPECT_GE(api.stopCalls,2);
    EXPECT_EQ(api.fetchCalls,0);
    EXPECT_EQ(store.calls,0);

    api.retryPhase=true;
    EXPECT_TRUE(orchestrator.finish(false));
    EXPECT_FALSE(api.recorder.recording);
    EXPECT_EQ(api.fetchCalls,0);
    EXPECT_EQ(store.calls,0);
    EXPECT_EQ(orchestrator.snapshot().finalization,FinalizationState::Idle);
}

TEST(RunOrchestratorTest, CancellationAfterConfirmedRecorderOwnershipArchivesExactlyOnce)
{
    std::promise<void> releasePromise;
    StartBarrierApi api(StartBarrierStage::AfterRecordingOwnership,
                        releasePromise.get_future().share());
    FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
    std::atomic<bool> startResult{true};
    std::thread worker([&]{startResult=orchestrator.start(request());});
    api.reached.get_future().wait();
    run.cancel=true;
    releasePromise.set_value();
    worker.join();

    EXPECT_FALSE(startResult.load());
    EXPECT_TRUE(orchestrator.finish(false));
    EXPECT_FALSE(run.running);
    EXPECT_GE(run.safe,1);
    EXPECT_EQ(api.stopCalls,1);
    EXPECT_EQ(api.fetchCalls,1);
    EXPECT_EQ(store.calls,1);
    EXPECT_EQ(orchestrator.snapshot().finalization,FinalizationState::Completed);
}

TEST(RunOrchestratorTest, ShutdownDuringStartHonorsRecorderOwnershipBoundaries)
{
    for(const StartBarrierStage stage : {
            StartBarrierStage::BeforeReset,
            StartBarrierStage::AfterStartResponse,
            StartBarrierStage::AfterRecordingOwnership}) {
        std::promise<void> releasePromise;
        StartBarrierApi api(stage,releasePromise.get_future().share());
        FakeStore store; FakeRun run; RunOrchestrator orchestrator(api,store,run);
        std::atomic<bool> startResult{true};
        std::thread owner([&]{startResult=orchestrator.start(request());});
        api.reached.get_future().wait();
        orchestrator.requestShutdown();
        releasePromise.set_value();
        owner.join();

        EXPECT_FALSE(startResult.load());
        EXPECT_FALSE(run.running);
        EXPECT_GE(run.safe,1);
        EXPECT_GE(api.engineOff,1);
        if(stage==StartBarrierStage::BeforeReset) {
            EXPECT_EQ(api.stopCalls,0);
            EXPECT_EQ(api.fetchCalls,0);
            EXPECT_EQ(store.calls,0);
        } else if(stage==StartBarrierStage::AfterStartResponse) {
            EXPECT_EQ(api.stopCalls,1);
            EXPECT_EQ(api.fetchCalls,0);
            EXPECT_EQ(store.calls,0);
        } else {
            EXPECT_EQ(api.stopCalls,1);
            EXPECT_EQ(api.fetchCalls,1);
            EXPECT_EQ(store.calls,1);
        }
    }
}

TEST(RunOrchestratorTest, FinishWaitsForLiveFreshRecorderToActuallyStop)
{
    struct DelayedStopApi : FakeApi {
        int stateCalls=0;
        ApiResult recordingStop() override { ++stopCalls; return {true,200,{}}; }
        ApiResult recordingState(RecorderState& out) override {
            ++stateCalls;
            if(stateCalls>=4) recorder.recording=false;
            out=recorder;
            return {true,200,{}};
        }
    } api;
    FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    const int before=api.stateCalls;
    ASSERT_TRUE(o.finish(false));
    EXPECT_GE(api.stateCalls-before,3);
    EXPECT_EQ(store.calls,1);
}

TEST(RunOrchestratorTest, RefreshRejectsAStaleSelectionSnapshot)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    api.selected.snapshotAgeMs=RunOrchestrator::MAX_SNAPSHOT_AGE_MS+1;
    EXPECT_FALSE(o.refresh());
    EXPECT_EQ(o.snapshot().phase,Phase::Failed);
}

// 종결이 엔진을 끄면 시뮬레이터는 물리 루프를 멈추고 스냅샷을 띄엄띄엄만
// 발행한다. 그 상태의 카탈로그 유실은 기대된 것이므로, 완주·제출 대기 화면에
// "maps catalog is not live" 오류가 깜빡여서는 안 된다. 주행 전의 유실은
// 종전대로 요란하게 실패한다.
//
// Finalization cuts the engine, the simulator stops its physics loop and
// publishes snapshots only sporadically. A catalog outage in that state is
// expected, so the screen of a completed run awaiting submit/reset must not
// blink "maps catalog is not live". A pre-run outage still fails loudly.
TEST(RunOrchestratorTest, PostRunRefreshOutageStaysQuietWhileAwaitingAction)
{
    FakeApi api; FakeStore store; FakeRun run; RunOrchestrator o(api,store,run);
    ASSERT_TRUE(o.start(request()));
    ASSERT_TRUE(o.finish(false));
    ASSERT_EQ(o.snapshot().postRun,PostRunState::AwaitingAction);
    api.failStage="catalog";
    EXPECT_FALSE(o.refresh());
    const auto snap=o.snapshot();
    EXPECT_EQ(snap.phase,Phase::Idle);
    EXPECT_TRUE(snap.error.empty()) << snap.error;
    EXPECT_EQ(snap.postRun,PostRunState::AwaitingAction);
    EXPECT_EQ(snap.finalization,FinalizationState::Completed);

    FakeApi preRun; preRun.failStage="catalog";
    FakeStore store2; FakeRun run2; RunOrchestrator idle(preRun,store2,run2);
    EXPECT_FALSE(idle.refresh());
    EXPECT_EQ(idle.snapshot().phase,Phase::Failed);
}

TEST(RunOrchestratorTest, RejectsStaleOrWrongContentTypeRecording)
{
    Recording r{"x", "application/json", "attachment; filename=x", true, false, 10};
    std::string reason;
    EXPECT_FALSE(RunOrchestrator::recAcceptable(r,reason));
    r={"x","text/csv","attachment; filename=x",false,false,10};
    EXPECT_FALSE(RunOrchestrator::recAcceptable(r,reason));
    r={"x","text/csv","inline",true,false,10};
    EXPECT_FALSE(RunOrchestrator::recAcceptable(r,reason));
}
