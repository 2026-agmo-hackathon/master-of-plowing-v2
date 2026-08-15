#ifndef APPMAIN_ORCHESTRATION_RUNORCHESTRATOR_HPP
#define APPMAIN_ORCHESTRATION_RUNORCHESTRATOR_HPP

#include <AppMain/tracking/TrackerTypes.hpp>

#include <cstddef>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace AppMain {
namespace orchestration {

struct CatalogItem {
    std::string id;
    std::string label;
    double widthM = 0.0;
    tracking::VehicleGeometry geometry;
};

struct Selection {
    std::string mapId;
    std::string tractorId;
    std::string implementId;
    bool stopped = false;
    bool reactRunning = false;
    bool live = false;
    long long snapshotAgeMs = -1;
    long long simElapsedMs = -1;
    // SimWorld 물리는 브라우저 페이지 안에서 돈다 — 창이 가려지거나(hidden)
    // 절전에서 못 깨어나면(schedule=timer) 모든 API 는 살아 있는데 세계만
    // 멈춘다. loopDiag 가 그 유일한 증거이므로 화면까지 그대로 나른다.
    // SimWorld physics runs inside a browser page: occlusion (hidden) or an
    // unrecovered throttle (schedule=timer) stops the world while every API
    // stays up. loopDiag is the only evidence, so it rides through to the UI.
    bool pageHidden = false;
    std::string schedule;
    long long droppedMs = -1;
    long long ticks = -1;
    std::string stopFrom;
};

struct RecorderState {
    bool live = false;
    bool recording = false;
    // Issue #9 §2.10 answers "is there a recording?" with the status code:
    // 404 {"error":"no recording yet"} means no recorder buffer exists at all.
    // That is an answer, not a failure, so it is carried here instead of being
    // reported as a transport error the caller has to guess about.
    bool present = true;
    long long snapshotAgeMs = -1;
};

struct Catalog {
    std::vector<CatalogItem> maps;
    std::vector<CatalogItem> tractors;
    std::vector<CatalogItem> implements;
    Selection selection;
};

struct Recording {
    std::string bytes;
    std::string contentType;
    std::string disposition;
    bool live = false;
    bool recording = false;
    long long snapshotAgeMs = -1;
};

struct ApiResult {
    bool ok = false;
    int status = 0;
    std::string error;
};
struct UnsubmittedRecord { std::string runId,teamName,createdAt,error; std::size_t bytes=0; bool csvAvailable=false; };

class ISimulatorApi {
public:
    virtual ~ISimulatorApi() = default;
    virtual ApiResult catalog(Catalog& out) = 0;
    virtual ApiResult selection(Selection& out) = 0;
    virtual ApiResult selectMap(const std::string& id) = 0;
    virtual ApiResult selectTractor(const std::string& id) = 0;
    virtual ApiResult selectImplement(const std::string& id) = 0;
    virtual ApiResult simulatorRefresh() = 0;
    virtual ApiResult recordingReset() = 0;
    virtual ApiResult recordingStart() = 0;
    virtual ApiResult recordingStop() = 0;
    virtual ApiResult recordingState(RecorderState& out) = 0;
    virtual ApiResult engine(bool on) = 0;
    virtual ApiResult recording(Recording& out) = 0;
};

class IRecordingStore {
public:
    virtual ~IRecordingStore() = default;
    virtual ApiResult persist(const std::string& teamName,
                              const std::string& stableRunId,
                              const std::string& rawCsv,
                              std::string& downloadId,
                              std::string& sha256) = 0;
    virtual ApiResult markSubmitted(const std::string& recordingId,
                                    const std::string& verdict) = 0;
    virtual ApiResult purgeUnsubmitted() = 0;
    virtual std::vector<UnsubmittedRecord> listUnsubmitted() const = 0;
};

class IRunControl {
public:
    virtual ~IRunControl() = default;
    virtual bool readyToStart() const = 0;
    virtual bool runActive() const = 0;
    virtual bool startCancelled() const { return false; }
    virtual void installRunGeometry(const tracking::VehicleGeometry& geometry) = 0;
    virtual void resetRun() = 0;
    virtual bool enterRunning() = 0;
    virtual void enterSafeIdle() = 0;
    virtual void enterFinished() = 0;
};

enum class Phase {
    Idle, Refreshing, ApplyingSetup, Starting, Running, Finishing, Failed
};

enum class FinalizationState {
    Idle, InProgress, Completed, RetryableError
};
enum class PostRunState { None, AwaitingAction, CaptureInProgress, CaptureDurable, Submitted, ResetRetryable, ResetCleanupPreparing, ResetCleanupPending };

struct StartRequest {
    std::string teamName;
    std::string mapId;
    std::string tractorId;
    std::string implementId;
};

struct Snapshot {
    Phase phase = Phase::Idle;
    Catalog catalog;
    // Monotonic receipt time for catalog.selection. Cached snapshots add the
    // elapsed time to the simulator-reported age instead of freezing freshness.
    std::chrono::steady_clock::time_point selectionObservedAt;
    std::string teamName;
    std::string error;
    std::string recordingId;
    std::string recordingSha256;
    bool busy = false;
    FinalizationState finalization = FinalizationState::Idle;
    PostRunState postRun = PostRunState::None;
    unsigned long long runGeneration = 0;
    unsigned long long resetGeneration = 0;
    bool setupApplying = false;
    unsigned long long setupGeneration = 0;
    StartRequest desiredSetup;
    StartRequest confirmedSetup;
};

class RunOrchestrator {
public:
    // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
    // Issue #9 의 선택·기록 스냅샷은 1 초 주기로 발행된다. 그래서 관측 나이는
    // 0~1000 ms 를 오르내리는 톱니이고, 예산을 발행 주기와 같은 1000 ms 로
    // 두면 정상적으로 도착한 샘플의 꼭짓점이 매 주기 예산 밖으로 나간다.
    // 실측에서도 발행 간격이 1.06 초까지 벌어지며 1508 ms 짜리 샘플이 나왔다.
    // "연속 3 회 신선" 을 요구하는 확인 루프들은 그 한 번에 카운터가 초기화돼
    // 제한 시간을 넘겼다. 예산을 한 주기만큼 넉넉히 둬서, 실제로 발행이
    // 걸렀을 때만 낡음으로 판정되게 한다.
    // └────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────┐
    // Issue #9's selection and recorder snapshots are published once a second,
    // so an observed age is a sawtooth from 0 to about 1000 ms. A budget equal
    // to the publish period puts the peak of a perfectly normal sample outside
    // it once per period; measurements showed the interval stretching to 1.06 s
    // and produced a 1508 ms sample. The confirmation loops that require three
    // consecutive fresh samples had their counter reset by that one reading and
    // ran out of time. One full period of slack means a sample is only stale
    // when a publish was genuinely missed.
    // └────────────────────────────────────────────────────────────────────┘
    static constexpr long long MAX_SNAPSHOT_AGE_MS = 2000;
    static constexpr long long MAX_CATALOG_AGE_MS = 10000;
    static constexpr std::size_t MAX_RECORDING_BYTES = 64u * 1024u * 1024u;

    RunOrchestrator(ISimulatorApi& api, IRecordingStore& store,
                    IRunControl& runControl,
                    std::function<void(const Snapshot&)> publish = {},
                    std::function<void(const std::string&,const std::string&)> finalized = {});

    bool refresh();
    void noteSetupIntent(const StartRequest& request, unsigned long long generation);
    bool applySetup(const StartRequest& request, unsigned long long generation);
    // refusal 이 주어지면, 시작을 거절한 이유를 사람이 읽을 문장으로 채운다.
    // 거절은 스냅샷에 남지 않는다 — phase 는 Running 이면 Running 그대로여야
    // 하고, Failed 로 적어 봐야 2.5초 뒤 새로고침이 Idle 로 덮어써 화면에서
    // 사라진다. 그래서 호출자가 orchestration_request_error 로 따로 알린다.
    //
    // When refusal is given, it receives a human-readable reason the start was
    // refused. A refusal must not be recorded in the snapshot: the phase has to
    // stay whatever it is (Running stays Running), and writing Failed there is
    // erased by the next 2.5 s refresh before anyone can read it. The caller
    // reports it over orchestration_request_error instead.
    bool start(const StartRequest& request, std::string* refusal = nullptr);
    bool finish(bool automatic);
    bool beginResetCleanup(const std::string& runId);
    bool beginPurgeAllCleanup();
    // 어떤 클라이언트가 아웃박스를 비웠다고 알려 왔음을 기록한다. 대시보드는
    // 여러 개가 붙을 수 있고, 낡은 탭 하나가 배수에 실패했다고 해서 이미
    // 성립한 리셋을 취소해서는 안 된다.
    //
    // Records that some client reported its outbox drained. Several dashboards
    // can be attached, and one stale tab failing its drain must not cancel a
    // reset another client has already admitted.
    bool noteResetCleanupPrepared(unsigned long long generation);
    bool failResetCleanup(const std::string& runId);
    bool resetCompletedRun();
    bool beginSealedCapture(const std::string& runId);
    bool acknowledgeSealedCapture(const std::string& runId);
    bool failSealedCapture(const std::string& runId);
    bool markLeaderboardSubmitted(const std::string& runId,const std::string& verdict);
    bool acknowledgeResetCleanup(const std::string& runId);
    void requestShutdown();
    Snapshot snapshot() const;
    static bool recAcceptable(const Recording& rec, std::string& reason);
    static bool sealedCaptureRequired(const Snapshot& snapshot,
                                      const std::string& acknowledgedRunId);
    static bool physicalStopSampleAcceptable(const Selection& selection,
                                             const StartRequest& expected);
    // 엔진이 켜져 있는 동안 "차량이 실제로 서 있는가"만 본다. 엔진 정지 확인은
    // 별도 단계다 — 엔진을 끄는 순간 시뮬레이터 물리 루프가 멈춰서 그 뒤로는
    // 어떤 것도 관측할 수 없기 때문이다.
    //
    // Whether the vehicle itself is at rest, with the engine still on. Engine
    // shutdown is confirmed separately: cutting the engine stops the simulator
    // loop, after which nothing can be observed at all.
    static bool vehicleAtRestSampleAcceptable(const Selection& selection,
                                              const StartRequest& expected);

private:
    static const CatalogItem* find(const std::vector<CatalogItem>& items,
                                   const std::string& id);
    static bool fresh(const Selection& selection);
    void compensate();
    void setPhase(Phase phase, const std::string& error = {});
    bool persistRecording();
    bool cancelled(const char* stage);
    bool shuttingDown() const;
    bool finalizationFailed(const std::string& error);
    bool finishImpl(bool automatic, bool duringShutdown);
    bool confirmVehicleAtRest(bool duringShutdown);
    bool confirmEngineOff(bool duringShutdown);

    ISimulatorApi& api_;
    IRecordingStore& store_;
    IRunControl& runControl_;
    std::function<void(const Snapshot&)> publish_;
    std::function<void(const std::string&,const std::string&)> finalized_;
    mutable std::mutex mutex_;
    Snapshot snapshot_;
    FinalizationState finalizationState_ = FinalizationState::Idle;
    unsigned long long runGeneration_ = 0;
    unsigned long long finalizationGeneration_ = 0;
    unsigned long long resetPreparedGeneration_ = 0;
    bool finalizationShouldFinish_ = false;
    std::string archiveRunId_;
    bool recordingStartIssued_ = false;
    unsigned long long recordingStartGeneration_ = 0;
    bool recordingOwned_ = false;
    unsigned long long recordingGeneration_ = 0;
    bool purgeAllCleanup_ = false;
    std::atomic<bool> shutdownRequested_{false};
};

const char* phaseName(Phase phase);
const char* finalizationStateName(FinalizationState state);
const char* postRunStateName(PostRunState state);

} // namespace orchestration
} // namespace AppMain

#endif
