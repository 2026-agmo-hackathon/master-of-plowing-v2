/*
 * AppMain/tracking/TrackingLoop.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * IPathTracker를 실제로 굴리는 제어 루프입니다. 전용 스레드 하나를 차지합니다.
 *
 * 한 주기(tick)는 다음 순서로 진행됩니다.
 *
 *     isReady()  ->  sense()  ->  tracker->update()  ->  act()  ->  report()
 *
 *   isReady()  주행할 준비가 됐는지 (경로·GPS·원점이 모두 있는지)
 *   sense()    GPS 좌표를 ENU 자세로 변환
 *   update()   여러분의 알고리즘 (여기가 유일하게 알고리즘이 바뀌는 지점)
 *   act()      명령을 시뮬레이터 메시지로 발행
 *   report()   텔레메트리·기록 집계·리더보드·CSV 트레이스
 *
 * sense()와 act()는 "미션 프로토콜"입니다. GPS를 어떻게 ENU로 바꾸는지,
 * TrackerCommand를 어떤 메시지로 내보내는지는 모든 알고리즘에서 동일하고,
 * 이걸 바꾸면 주행 실력이 좋아지는 게 아니라 시뮬레이터의 동작 자체가
 * 달라집니다. 그래서 virtual이 아닙니다.
 *
 * 실제로 달라져야 하는 부분은 파일 아래쪽의 virtual 훅과
 * IPathTracker::preferredPeriodMs()입니다. 제어 주기만 바꾸고 싶다면 이
 * 클래스는 건드릴 필요가 전혀 없습니다 — 트래커에서 preferredPeriodMs()만
 * 재정의하면 됩니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The control loop that drives an IPathTracker. Owns a dedicated thread.
 *
 * One tick is:
 *
 *     isReady()  ->  sense()  ->  tracker->update()  ->  act()  ->  report()
 *
 * sense() and act() are the mission protocol: how GPS becomes an ENU pose, and
 * how a TrackerCommand becomes simulator messages. They are the same for every
 * algorithm, and changing them changes what the simulator does rather than how
 * well the vehicle drives — so they are not virtual.
 *
 * What *is* meant to vary lives behind the virtual hooks at the bottom, plus
 * IPathTracker::preferredPeriodMs(). A tracker that only wants a faster tick
 * does not need to touch this class at all.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_TRACKINGLOOP_HPP
#define APPMAIN_TRACKING_TRACKINGLOOP_HPP

#include <AppMain/tracking/SteeringController.hpp>
#include <AppMain/tracking/ClockLog.hpp>
#include <AppMain/tracking/SampleClock.hpp>
#include <AppMain/tracking/ControlTimeGate.hpp>
#include <AppMain/tracking/TrackerTypes.hpp>

#include <chrono>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace AppMain
{
class MainController;

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// RunState 는 MainController.hpp 의 보호 리전 안에 정의돼 있습니다(재생성에서
// 보존되어야 하므로). 이 헤더는 MainController.hpp 를 include 하지 않으니
// 불투명 enum 선언으로 씁니다 — 기반 타입을 명시하면 크기가 확정되므로 값
// 전달 인자로 쓸 수 있습니다. 정의와 기반 타입이 반드시 일치해야 합니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// RunState is defined inside MainController.hpp's protected region (it has to
// survive regeneration). This header does not include MainController.hpp, so an
// opaque enum declaration is used: naming the underlying type completes the type,
// which makes by-value parameters legal. The underlying type must match the
// definition.
// └────────────────────────────────────────────────────────────────────────────┘
enum class RunState : int;

namespace web
{
class WebSocketEndPoint;
}

namespace tracking
{

class IPathTracker;

class TrackingLoop
{
public:
    explicit TrackingLoop(MainController& controller);
    virtual ~TrackingLoop();

    /**
     * 블로킹 방식의 주기 루프. 전용 스레드에서 실행하도록 만들어졌습니다.
     * Blocking tick loop. Intended to own a dedicated thread.
     */
    void run();

    /**
     * 주행 1회분의 타이밍·완주 상태를 초기화합니다. 세션 리셋 시 호출됩니다.
     * Clears per-run timing and completion state. Called on session reset.
     */
    void resetRunState();

protected:
    // ── 미션 프로토콜: 모든 알고리즘에서 동일 ────────────────────────────────
    // ── Mission protocol: identical for every algorithm ─────────────────────

    /**
     * GPS(WGS84) -> UTM -> ENU 변환과, 나침반 방위 -> 수학 각도 변환.
     * GPS (WGS84) -> UTM -> ENU, plus the compass-to-math heading conversion.
     */
    VehicleState sense() const;

    /**
     * 조향 / 스로틀 / 브레이크 / 기어 / 작업기를 시뮬레이터로 발행합니다.
     * Publishes steer / throttle / brake / gear / implement to the simulator.
     */
    void act(const TrackerCommand& cmd);

    /**
     * 텔레메트리, 주행 기록 집계, 리더보드 전송, CSV 트레이스.
     * Telemetry, run summary accumulation, leaderboard ticks, CSV trace.
     */
    void report(const VehicleState& state, const TrackerCommand& cmd,
                const TrackerDebug& debug, double dtS);

    // ── 훅: 루프 동작을 바꾸려면 여기를 재정의 ───────────────────────────────
    // ── Hooks: override to change loop behaviour ────────────────────────────

    /**
     * 한 주기를 돌릴 만큼 상태가 갖춰졌는지.
     * True when there is enough state to run a tick.
     */
    virtual bool isReady() const;

    /**
     * 주기 사이의 대기 시간. 트래커의 희망 주기와 시뮬레이션 배속에서 계산합니다.
     * Sleep between ticks, derived from the tracker's requested hardware cadence.
     */
    virtual int periodMs() const;

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * 트래커가 완주를 알린 주기마다 호출됩니다. 첫 완주 주기에는 즉시 안전
     * Idle/출력 해제를 요청하고 track_complete를 한 번 방송합니다. 이후 비동기
     * finalization은 물리 정지와 recorder archive를 확인합니다.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Called on every tick where the tracker reports completion. The first
     * completed tick immediately requests safe Idle/output release and emits
     * track_complete once. Async finalization then confirms physical stop and
     * the recorder archive.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual void onComplete();

    /**
     * 트래커가 아직 완주하지 않은 주기마다 호출됩니다.
     * Called on every tick where the tracker is NOT complete.
     */
    virtual void onIncomplete();

    std::shared_ptr<IPathTracker> tracker() const;
    std::shared_ptr<web::WebSocketEndPoint> webSocket() const;

    MainController& controller_;

private:
    /**
     * 이전 주기로부터의 경과 초. 긴 정지(탭 전환 등)에 대비해 상한을 둡니다.
     * Seconds since the previous tick, clamped against long stalls.
     */
    double consumeTickDt();

    /**
     * 달리지 않는 주기: 안전 지령을 쓰고 텔레메트리만 계속 발행합니다.
     * 자세한 이유는 .cpp 의 정의 위 주석을 보세요.
     *
     * The not-driving tick: writes the safe command set and keeps telemetry
     * flowing. See the comment above the definition in the .cpp.
     */
    void holdSafe(::AppMain::RunState runState);

    void writeTraceRow(const VehicleState& state, const TrackerCommand& cmd,
                       const TrackerDebug& debug);
    void writeDiagnosticLine(const VehicleState& state, const TrackerCommand& cmd,
                             const TrackerDebug& debug) const;
    void writeClockLine(const VehicleState& state, const TrackerDebug& debug,
                        const ClockTelemetry& telemetry);

    std::chrono::steady_clock::time_point lastTickT_;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 조향 하위 루프. 목표 조향각을 모터 속도 지령으로 바꾸는 폐루프이고,
    // 엔코더 원점 추정 같은 상태를 들고 있으므로 주행 내내 같은 객체여야 합니다.
    //
    // lastDtS_ 는 act()가 쓸 제어 주기입니다. run()이 consumeTickDt()로 구한
    // 값을 act()까지 인자로 넘기지 않고 여기 담아 둡니다 — act()의 시그니처가
    // 미션 프로토콜의 일부라 바꾸지 않으려는 것입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The steering inner loop. It must be the same object for the whole run
    // because it carries state such as the encoder centre estimate.
    //
    // lastDtS_ is the control period act() needs. run() stashes what
    // consumeTickDt() measured rather than widening act()'s signature, which is
    // part of the mission protocol.
    // └────────────────────────────────────────────────────────────────────────┘
    SteeringController steering_;
    SampleClock machineClock_;
    double lastDtS_;
    SteeringController::Output lastSteerDebug_;
    // PP_LOG 진단용 원시 엔코더 입력 / raw encoder input, for PP_LOG
    double lastEncoderDeg_ = 0.0;
    bool lastEncoderValid_ = false;
    std::int64_t lastReportedPoseSampleId_ = -1;
    std::int64_t lastClockLogPoseSampleId_ = -1;
    // 정지 지령 중 연속으로 관측한 정지 샘플 수 / consecutive standstill samples
    // observed during a commanded stop.
    int standstillTicks_ = 0;
    int commandedStopTicks_ = 0;
    ClockLogRateLimiter clockLogRateLimiter_;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 시뮬레이션 경과 시간: 실제 경과 시간(dt)에 시뮬레이션 배속과 농지 이탈
    // 페널티 배율을 곱해 누적한 값입니다. 주행 기록에 보고되는 시간입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Machine elapsed time derived from validated sensor timestamps. This is
    // what the run summary and completion hold report.
    // └────────────────────────────────────────────────────────────────────────┘
    double accumulatedElapsedS_;
    double elapsedSAtComplete_;

    // 완주를 처음 감지하거나 retryable finalization을 마지막 재시도한 시각.
    // First completion or most recent retryable-finalization attempt.
    std::optional<std::chrono::steady_clock::time_point> completeAtT_;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // CSV 트레이스. 컬럼 구성은 첫 주기에 확정합니다. 그래야 extras가 도중에
    // 바뀌는 트래커가 있어도 컬럼이 어긋나지 않습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // CSV trace. The column layout is fixed on the first tick so that a tracker
    // whose extras vary cannot misalign the columns.
    // └────────────────────────────────────────────────────────────────────────┘
    std::ofstream traceFile_;
    std::vector<std::string> traceExtraKeys_;
    std::chrono::steady_clock::time_point traceT0_;
    bool traceInitialised_;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_TRACKINGLOOP_HPP
