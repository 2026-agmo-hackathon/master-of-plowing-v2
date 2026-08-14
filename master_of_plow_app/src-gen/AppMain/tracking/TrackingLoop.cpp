/*
 * AppMain/tracking/TrackingLoop.cpp
 *
 * 구조 설명은 TrackingLoop.hpp 상단을 참고하세요.
 * See the top of TrackingLoop.hpp for the overall structure.
 */

#include <AppMain/tracking/TrackingLoop.hpp>
#include <AppMain/tracking/ClockTelemetry.hpp>

#include <AppMain/CoordTransform.hpp>
#include <AppMain/MainController.hpp>
#include <AppMain/tracking/IPathTracker.hpp>
#include <AppMain/web/WebSocketEndPoint.hpp>

#include <json/json.h>
#include <nevonex-fcal-platform/log/Logger.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

namespace AppMain {
namespace tracking {

namespace {

// GPS나 경로를 기다리는 동안의 대기 시간. / Sleep while waiting for GPS / a track.
constexpr int IDLE_PERIOD_MS = 100;
// 잘못된 tracker 설정으로 busy-loop가 되지 않게 하는 하한입니다.
// Floor that prevents a misconfigured tracker from busy-looping.
constexpr int MIN_PERIOD_MS = 16;
// dt 상한. 브라우저 탭이 멈췄다 돌아왔을 때 yaw rate가 튀는 것을 막습니다.
// Clamp: a stalled tab must not spike yaw rate.
constexpr double MAX_TICK_DT_S = 0.5;
// 히치 위치 지령. 작업기 내림/올림에 대응하는 퍼센트입니다. 실기에서 작업
// 깊이에 맞춰 조정할 값입니다.
// Hitch position commands for implement down/up, in percent. Tune on the machine
// to match the working depth.
constexpr int HITCH_DOWN_PCT = 10;
constexpr int HITCH_UP_PCT = 90;

// 정지 지령을 놓아도 되는 기준. 시뮬레이터의 stopped 판정보다 넉넉해야
// 제어권을 먼저 놓는 일이 없습니다.
// The standstill the commanded stop waits for. Kept tighter than the
// simulator's own stopped threshold so authority is never released first.
constexpr double STANDSTILL_MPS = 0.05;
constexpr int STANDSTILL_TICKS = 15;
constexpr int COMMANDED_STOP_MAX_TICKS = 300;

constexpr const char* TRACE_PATH = "/tmp/pp_trace.csv";

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// UI의 텔레메트리 검증기가 반드시 요구하는 진단 키들입니다. 원래 Pure Pursuit
// 고유 개념이라서, 그런 개념이 없는 트래커를 위해 기본값 0으로라도 항상
// 채워 보냅니다. 빠뜨리면 UI가 프레임 전체를 버려서 TRACKING 패널이 죽습니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Diagnostics keys the UI's telemetry validator requires. They are Pure Pursuit
// concepts, so they are emitted with a 0 default for trackers that have no such
// notion — otherwise the UI drops the whole frame and the TRACKING panel dies.
// └────────────────────────────────────────────────────────────────────────────┘
const char* const REQUIRED_TELEMETRY_EXTRAS[] = {
    "lookaheadDist", "xLocal", "zLocal", "implementClosestIdx"
};

std::string toCompact(const Json::Value& v)
{
    Json::StreamWriterBuilder wb;
    wb["indentation"] = "";
    return Json::writeString(wb, v);
}

} // namespace

TrackingLoop::TrackingLoop(MainController& controller)
    : controller_(controller)
    , lastTickT_(std::chrono::steady_clock::now())
    , steering_()
    , lastDtS_(0.1)
    , lastSteerDebug_()
    , accumulatedElapsedS_(0.0)
    , elapsedSAtComplete_(-1.0)
    , completeAtT_()
    , traceFile_()
    , traceExtraKeys_()
    , traceT0_()
    , traceInitialised_(false)
{
}

TrackingLoop::~TrackingLoop() = default;

void TrackingLoop::resetRunState()
{
    lastTickT_ = std::chrono::steady_clock::now();
    accumulatedElapsedS_ = 0.0;
    elapsedSAtComplete_ = -1.0;
    completeAtT_.reset();
    // 조향 적분과 엔코더 원점 추정도 함께 버립니다. 지난 주행에서 쌓은 값이
    // 새 주행 첫 주기에 그대로 튀어나오면 안 됩니다.
    // The steering integral and encoder centre estimate go too: a value carried
    // over from the previous run must not appear on the first tick of the next.
    steering_.reset();
    lastDtS_ = 0.1;
    lastSteerDebug_ = SteeringController::Output { };
    lastReportedPoseSampleId_ = -1;
    lastClockLogPoseSampleId_ = -1;
    // A stopped run may be started again without re-uploading its RDDF. Reset
    // the tracker's per-run state as well as the loop's timers; otherwise the
    // longitudinal zero-order hold can carry the final zero-throttle command
    // into the next run indefinitely while the GPS speed remains exactly zero.
    if (auto activeTracker = tracker()) {
        if (activeTracker->hasTrack()) {
            const auto waypoints = activeTracker->getWaypoints();
            activeTracker->setTrack(waypoints);
        }
    }
}

std::shared_ptr<IPathTracker> TrackingLoop::tracker() const
{
    return controller_.getTracker();
}

std::shared_ptr<web::WebSocketEndPoint> TrackingLoop::webSocket() const
{
    return ::AppMain::web::WebSocketEndPoint::getInstance();
}

// ─────────────────────────────────────────────────────────────────────────────
// 루프 본체 / The loop
// ─────────────────────────────────────────────────────────────────────────────

void TrackingLoop::run()
{
    lastTickT_ = std::chrono::steady_clock::now();

    while (true)
    {
        try
        {
            // ┌─ 한국어 ───────────────────────────────────────────────────────┐
            // "달릴 수 있는가"(isReady)와 "달려야 하는가"(주행 상태)를 나눕니다.
            //
            // 예전에는 이 둘이 하나였습니다. 그래서 RDDF·GPS·원점이 갖춰지는
            // 순간 차가 출발했고, 대기 중에는 텔레메트리도 전혀 나가지
            // 않았습니다. 후자 때문에 대시보드가 "준비됐는지"를 알 수 없어
            // 시작 버튼을 만들 수도 없었습니다.
            //
            // 이제 준비만 됐고 시작 지시가 없으면: 안전 지령을 내보내고
            // 텔레메트리는 계속 발행합니다.
            // └────────────────────────────────────────────────────────────────┘
            // ┌─ English ──────────────────────────────────────────────────────┐
            // "Can drive" (isReady) is separated from "should drive" (run state).
            // These used to be one test, so the vehicle pulled away the instant a
            // path, a fix and an origin existed — and while waiting no telemetry
            // went out at all, which is why the dashboard could not tell whether it
            // was ready and a start button was impossible.
            //
            // Armed but not started now means: publish safe commands, keep
            // publishing telemetry.
            // └────────────────────────────────────────────────────────────────┘
            const ::AppMain::RunState runState = controller_.getRunState();

            if (!isReady() || !::AppMain::isDriving(runState))
            {
                holdSafe(runState);
                std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_PERIOD_MS));
                continue;
            }

            if (!machineClock_.calibrated()) {
                holdSafe(runState);
                std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_PERIOD_MS));
                continue;
            }

            auto activeTracker = tracker();

            // ┌─ 한국어 ───────────────────────────────────────────────────────┐
            // 튜닝 파라미터를 매 주기 읽어 트래커와 두 컨트롤러에 밀어넣습니다.
            // 10Hz 에서 작은 구조체 복사는 사실상 무료이므로 "바뀌었을 때만"
            // 같은 세대 카운터 최적화는 하지 않습니다. 종방향 게인은 트래커가
            // setParams() 안에서 자기 SpeedController 로 전달합니다.
            // └────────────────────────────────────────────────────────────────┘
            // ┌─ English ──────────────────────────────────────────────────────┐
            // Read the tuning parameters every tick and push them into the
            // tracker and both controllers. Copying a small struct at 10 Hz is
            // free, so no change-detection counter. The longitudinal gains are
            // forwarded by the tracker's setParams() to its SpeedController.
            // └────────────────────────────────────────────────────────────────┘
            const TrackerParams tuning = controller_.getTrackerParams();
            activeTracker->setParams(tuning);
            const VehicleGeometry geometry = controller_.getRunGeometry();
            activeTracker->setVehicleGeometry(geometry);
            steering_.setGains(tuning.steerKp, tuning.steerKi, tuning.steerKd);
            steering_.setVehicleGeometry(geometry);

            const double wallDtS = consumeTickDt();
            const VehicleState state = sense();
            // Timestamp identity is observed even while the payload freshness
            // gate marks a held gyro value unusable. A slow mux may legitimately
            // hold the same timestamp for about one wall second.
            const ControlTimeStep time = advanceControlTime(machineClock_,
                    state.yawRateSampleId, wallDtS, true);
            controller_.setMachineTimeScale(time.scale);
            if (!time.allowActuation) {
                // Regression, an implausible jump, or bounded timestamp loss
                // invalidates the time domain on this very tick. Do not allow
                // one final stale tracker/actuation cycle before the next gate.
                holdSafe(runState);
                std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_PERIOD_MS));
                continue;
            }
            const double dtS = time.dtS;
            lastDtS_ = dtS;   // machine-time dt used by the sample-driven loops
            const TrackerCommand cmd = activeTracker->update(state, dtS);

            act(cmd);
            report(state, cmd, activeTracker->lastDebug(), dtS);

            if (activeTracker->isTrackComplete())
            {
                onComplete();
            }
            else
            {
                onIncomplete();
            }
        } catch (...)
        {
            // 한 주기에서 예외가 나도 루프는 죽지 않습니다. 참가자가 만든
            // 트래커가 던진 예외로 앱 전체가 멈추면 디버깅이 불가능하니까요.
            // A throwing tick must not kill the loop: an exception from a
            // participant's tracker should not take the whole app down.
            NEVONEX_LOG(::nevonex::log::SeverityLevel::error)
                    << "Exception in TrackingLoop::run:"
                    << boost::current_exception_diagnostic_information();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(periodMs()));
    }
}

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 달리지 않는 주기의 처리. 두 가지를 합니다.
 *
 *   1. 안전 지령을 내보냅니다. 모터 0(조향 해제) · 개도 0 · 변속기 중립 ·
 *      제어권 해제. 특히 중립이 중요합니다 — 개도만 0으로 두고 기어를 전진에
 *      남겨두면 트랙터는 크리프로 계속 굴러갑니다.
 *
 *   2. 텔레메트리는 계속 발행합니다. 대시보드가 "지금 준비됐는가 / 왜 시작할 수
 *      없는가"를 봐야 시작 버튼을 쓸 수 있습니다. 예전에는 이 구간에서 프레임이
 *      아예 안 나가서 화면이 "프레임 대기 중"에 머물렀습니다.
 *
 * 일시정지에서는 조향 PID의 적분을 비우지 않습니다. 게인이 그대로인데 경사에서
 * 잡아 둔 오프셋까지 버리면 재개할 때마다 차가 출렁입니다. 반대로 Idle 로
 * 되돌릴 때는 비웁니다 — 회차가 끝났으므로 지난 오차를 물려받아선 안 됩니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The not-driving tick. It writes the safe command set — motor 0 (steering
 * released), zero accelerator opening, transmission in NEUTRAL, authority released
 * — and keeps publishing telemetry.
 *
 * Neutral matters most: zero opening with the gear still in forward leaves the
 * tractor creeping. And telemetry must continue, or the dashboard cannot tell
 * whether it is ready to start; previously no frames went out at all here and the
 * screen sat on "waiting for frames".
 *
 * A pause keeps the steering integrator (the gains have not changed, and dropping
 * the slope offset would jolt the vehicle on resume); returning to Idle clears it,
 * because the run is over and the next one must not inherit its error.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */
void TrackingLoop::holdSafe(::AppMain::RunState runState)
{
    ::AppMain::MachineOutputs out;
    out.valid = true;
    out.motorCmd = 0;           // 조향 릴리스 / steering released
    out.accelPct = 0;
    out.fnr = 2;                // 중립 / neutral
    out.shift = 1;              // Low
    // 작업기는 올립니다. 멈춘 자리에서 흙에 물려 있으면 재개할 때 부하가 큽니다.
    // Raise the implement: leaving it buried makes the resume load heavy.
    out.hitchPct = HITCH_UP_PCT;
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 여기서 쓰는 개도 0·중립은 제어권을 놓은 채로는 트랙터에 닿지 않습니다.
    // 지령 프레임의 AD_Generic_Mode_Cmd 가 1 이 아니면 수신 측이 프레임을
    // 통째로 버리기 때문에, 주행 마지막 개도가 그대로 남아 바퀴가 계속 돕니다.
    // 그래서 정지를 "지령"한 구간에서는 설 때까지 제어권을 쥔 채 0 을 보내고,
    // 선 것을 확인하면 놓습니다. 마지막으로 수락된 지령이 0 이라 놓아도 서
    // 있습니다. 그냥 대기 중일 때는 쥐지 않습니다 — 사람이 시뮬레이터에서
    // 트랙터를 직접 몰 수 있어야 합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The zero opening and neutral written here never reach the tractor while
    // authority is released: the receiver drops any command frame whose
    // AD_Generic_Mode_Cmd is not 1, so the run's last opening stays applied and
    // the wheels keep turning. During a commanded stop, authority is therefore
    // held with zero output until standstill is observed and released after.
    // The last accepted command is zero, so the machine stays put. Plain idle
    // does not hold it — a human has to be able to drive the tractor in the
    // simulator.
    // └────────────────────────────────────────────────────────────────────────┘
    if (controller_.commandedStopActive()) {
        const auto gps = controller_.getGpsSample();
        if (std::fabs(gps.speed) <= STANDSTILL_MPS) {
            if (++standstillTicks_ >= STANDSTILL_TICKS)
                controller_.endCommandedStop();
        } else {
            standstillTicks_ = 0;
        }
        // 서지 않는 기계를 붙들고 영영 제어권을 쥐고 있지는 않습니다. 0 지령은
        // 이미 수락되어 걸려 있으므로 놓아도 다시 가속하지 않고, 사람이
        // 시뮬레이터에서 트랙터를 빼낼 수 있어야 합니다.
        // Do not hold authority forever over a machine that will not stop: the
        // zero command has already been accepted and latched, so releasing does
        // not let it accelerate, and a human has to be able to free the tractor
        // in the simulator.
        if (++commandedStopTicks_ >= COMMANDED_STOP_MAX_TICKS)
            controller_.endCommandedStop();
    } else {
        standstillTicks_ = 0;
        commandedStopTicks_ = 0;
    }
    out.authority = controller_.commandedStopActive();
    controller_.setMachineOutputs(out);

    if (runState != ::AppMain::RunState::Paused) {
        steering_.reset();
    }

    // 대기 중에도 상태를 계속 보여줍니다. 트래커가 없으면 진단값은 비어 있고,
    // 그래도 GPS 품질·제어권·준비 여부는 유효합니다.
    // Keep reporting while idle: with no tracker the diagnostics are empty, but GPS
    // quality, authority and readiness are still meaningful.
    const double wallDtS = consumeTickDt();
    const VehicleState state = sense();
    const ControlTimeStep time = advanceControlTime(machineClock_,
            state.yawRateSampleId, wallDtS, false);
    controller_.setMachineTimeScale(time.scale);
    auto activeTracker = tracker();
    const TrackerDebug emptyDebug;
    report(state, TrackerCommand { },
           activeTracker ? activeTracker->lastDebug() : emptyDebug, 0.0);
}

double TrackingLoop::consumeTickDt()
{
    const auto now = std::chrono::steady_clock::now();
    double dtS = std::chrono::duration<double>(now - lastTickT_).count();
    if (dtS > MAX_TICK_DT_S) dtS = MAX_TICK_DT_S;
    lastTickT_ = now;
    return dtS;
}

// ─────────────────────────────────────────────────────────────────────────────
// 훅 / Hooks
// ─────────────────────────────────────────────────────────────────────────────

bool TrackingLoop::isReady() const
{
    // 경로, GPS 수신, 좌표 원점, WebSocket이 모두 갖춰져야 한 주기를 돕니다.
    // A tick needs a path, a GPS fix, a coordinate origin and a WebSocket.
    auto activeTracker = tracker();
    return activeTracker && activeTracker->hasTrack()
        && controller_.hasGps() && controller_.hasTrackOrigin()
        && webSocket() != nullptr;
}

int TrackingLoop::periodMs() const
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 트래커가 원하는 주기를 그대로 씁니다. 예전에는 UI가 주는 시뮬레이션 배속
    // 으로 이 값을 나눠 최소 5ms까지 내려갔는데, 실기와 외부 시뮬레이터에는
    // 배속이라는 개념이 없으므로 제거했습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The tracker's preferred period is used as-is. This used to be divided by a
    // UI-supplied simulation speed multiplier (down to a 5 ms floor); neither the
    // real machine nor the external simulator has such a concept.
    // └────────────────────────────────────────────────────────────────────────┘
    auto activeTracker = tracker();
    const int base = activeTracker ? activeTracker->preferredPeriodMs() : IDLE_PERIOD_MS;
    // 하한만 둡니다. 상한을 두면 100ms보다 느린 주기를 원하는 트래커를 조용히
    // 무시하게 됩니다. / Floor only: an upper bound would silently override a
    // tracker that asks for a slower tick than 100 ms.
    return std::max(normalizedPeriodMs(base, machineClock_.scale()), MIN_PERIOD_MS);
}

void TrackingLoop::onIncomplete()
{
    // 트래커는 완주 상태에서 다시 미완주로 돌아올 수 있습니다(세션 중 새 경로
    // 적재). 그때 finalization 재시도 타이머가 살아 있으면 안 됩니다.
    // The tracker can un-complete (a new track is loaded mid-session), so the
    // finalization retry timer must not survive that.
    completeAtT_.reset();
}

// ─────────────────────────────────────────────────────────────────────────────
// 감지 / Sense
// ─────────────────────────────────────────────────────────────────────────────

VehicleState TrackingLoop::sense() const
{
    const auto gps = controller_.getGpsSample();
    const auto utm = ::AppMain::CoordTransform::wgs84ToUtm(
            gps.lat, gps.lon);
    const auto enu = ::AppMain::CoordTransform::utmToEnu(
            utm.easting, utm.northing,
            controller_.trackOriginE(), controller_.trackOriginN());

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // GPS heading은 나침반 방위(0 = 북쪽, 시계방향 +)입니다. 추종 좌표계는
    // +x = 동쪽, +z = 남쪽 기준의 atan2(z, x)를 쓰므로 90도를 빼서 옮깁니다.
    //   0도(북) -> -pi/2      90도(동) -> 0
    //   180도(남) -> +pi/2   270도(서) -> +pi
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // GPS heading is a compass bearing (0 = North, clockwise +). The tracking
    // frame measures atan2(z, x) with +x = East and +z = South, so:
    //   0 deg (N) -> -pi/2      90 deg (E) ->  0
    //   180 deg (S) -> +pi/2   270 deg (W) -> +pi
    // └────────────────────────────────────────────────────────────────────────┘
    VehicleState state;
    state.x = enu.x;
    state.z = enu.z;
    state.headingRad = (gps.heading - 90.0) * M_PI / 180.0;
    state.speedMps = gps.speed;
    state.poseSampleId = gps.sequence;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 나머지는 머신 스냅샷에서 옮겨 담습니다. 인터페이스가 도(deg)로 주는 값은
    // 여기서 한 번만 라디안으로 바꿉니다 — 이후 추종 계층은 전부 라디안입니다.
    //
    // 조향 실측각은 여기서 채우지 않습니다. 엔코더 원값을 앞바퀴각으로 바꾸는
    // 일은 조향비와 원점 추정이 필요하고 그 상태를 SteeringController 가 들고
    // 있으므로, act() 에서 계산해 넣습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The rest is copied from the machine snapshot. Values the interfaces report
    // in degrees are converted to radians here, once — everything above this line
    // is radians from then on.
    //
    // The measured steering angle is not filled in here: turning the raw encoder
    // reading into a wheel angle needs the steering ratio and the centre estimate,
    // and SteeringController owns that state, so act() computes it.
    // └────────────────────────────────────────────────────────────────────────┘
    const ::AppMain::MachineInputs in = controller_.getMachineInputs();

    // Raw GyroZ is counter-clockwise positive, while GPS compass heading and
    // this tracking frame increase clockwise. Normalize the sign exactly once
    // here; SteeringController and every tracker consume the internal convention.
    state.yawRateRadS = internalYawRateFromRawGyroDegS(in.gyroZDegS);
    state.yawRateValid = in.gyroValid;
    state.yawRateSampleId = in.gyroSampleId;
    state.rollRad = in.rollDeg * M_PI / 180.0;
    state.pitchRad = in.pitchDeg * M_PI / 180.0;
    state.attitudeValid = in.attitudeValid;
    state.accelX = in.accelX;
    state.accelY = in.accelY;
    state.accelValid = in.accelValid;
    state.fnrState = in.fnrState;
    state.shiftState = in.shiftState;
    state.hitchPct = in.hitchPct;
    state.hitchValid = in.hitchValid;
    // 네 계통 모두 자율을 수락했을 때만 제어권이 있다고 봅니다. 하나라도
    // 빠지면 그 계통의 지령은 무시되고 있는 것이므로 부분 성공은 성공이 아닙니다.
    // Authority counts only when all four subsystems granted it: a partial grant
    // means some commands are being ignored, which is not success.
    state.controlAuthority = in.accAuto && in.fnrAuto && in.sftAuto && in.hydAuto;
    state.gpsFixType = in.gpsFixType;
    state.gpsSats = in.gpsSats;
    state.gpsHdop = in.gpsHdop;
    state.motorFaultBits = in.motorFaultBits;
    return state;
}

// ─────────────────────────────────────────────────────────────────────────────
// 지시 / Act
// ─────────────────────────────────────────────────────────────────────────────

void TrackingLoop::act(const TrackerCommand& cmd)
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 알고리즘이 낸 TrackerCommand 를 이 기계의 지령으로 번역합니다. 트래커는
    // 기계를 몰라도 되도록 계약을 그대로 두고, 기계별 사정은 전부 여기서 흡수합니다.
    //
    //   steerAngle (rad)  -> SteeringController -> 모터 속도 지령 (±600)
    //   throttle (0..1)   -> 가속 개도 0..100 %
    //   brake (0..1)      -> 제동 채널이 없습니다. 개도 0 + 한 단 다운시프트.
    //   gear (-1|1..3)    -> FNR(1=R,2=N,3=F) + SFT(1=L,2=M,3=H) 로 분리
    //   implement (0|1)   -> 히치 위치 % (내림/올림)
    //
    // 실제 CAN 프레임을 만드는 일은 MachineBridge 가 ProcessTimer 스레드에서
    // 합니다. 여기서는 MachineOutputs 를 채워 넘기기만 합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Translates the algorithm's TrackerCommand into this machine's commands. The
    // tracker contract stays machine-agnostic; every machine-specific quirk is
    // absorbed here.
    //
    // There is no brake channel, so a brake request becomes zero accelerator
    // opening plus a one-stage downshift. `gear` splits into FNR and SFT.
    //
    // Building the actual CAN frames is MachineBridge's job, on the ProcessTimer
    // thread; this only fills in MachineOutputs.
    // └────────────────────────────────────────────────────────────────────────┘
    const ::AppMain::MachineInputs in = controller_.getMachineInputs();
    const VehicleState state = sense();

    ::AppMain::MachineOutputs out;
    out.valid = true;
    // 자율 제어권은 주행 중 계속 요청합니다. 트랙터가 수락하면 *Auto 로 돌아옵니다.
    // Authority is requested continuously while driving; the grant returns as *Auto.
    out.authority = true;

    // ── 조향 / Steering ─────────────────────────────────────────────────────
    const SteeringController::Output steer = steering_.update(cmd.steerAngle,
            state, in.steerEncoderDeg, in.steerEncoderValid,
            in.steerEncoderSampleId, state.yawRateSampleId, lastDtS_);
    out.motorCmd = steer.motorCmd;
    lastSteerDebug_ = steer;
    // 진단용 원값. 융합된 값 하나만으로는 엔코더/자이로/원점보정을 분리할 수
    // 없습니다 — 이 기계의 엔코더가 우측 절반에서 0으로 잘려 있다는 사실도
    // 이 원값을 봐야 드러납니다.
    // Raw inputs for diagnosis: a single fused number cannot be unpicked into
    // encoder, gyro and trim, and this machine's encoder being clipped to zero
    // over its right half is only visible in the raw value.
    lastEncoderDeg_ = in.steerEncoderDeg;
    lastEncoderValid_ = in.steerEncoderValid;

    // ── 종방향 / Longitudinal ───────────────────────────────────────────────
    if (cmd.brake > 0.0)
    {
        // 제동 요청: 개도를 끊고 한 단 내립니다. 엔진 브레이크가 유일한 감속
        // 수단이므로 이것이 할 수 있는 전부입니다.
        // Braking: cut the opening and drop a stage. Engine braking is the only
        // deceleration available, so this is the whole of it.
        out.accelPct = 0;
    }
    else
    {
        out.accelPct = static_cast<int>(std::lround(
                std::clamp(cmd.throttle, 0.0, 1.0) * 100.0));
    }

    // gear 를 방향과 단수로 나눕니다. 후진은 Low 로 고정합니다 — 후진 지령에는
    // 단수 정보가 없고, 작업 중 후진은 저속이 안전합니다.
    // Split gear into direction and stage. Reverse pins to Low: the command
    // carries no stage for reverse, and reversing slowly is the safe default.
    if (cmd.gear < 0)
    {
        out.fnr = 1;    // R
        out.shift = 1;  // Low
    }
    else
    {
        out.fnr = 3;    // F
        out.shift = std::clamp(cmd.gear, 1, 3);
        if (cmd.brake > 0.0)
        {
            out.shift = std::max(1, out.shift - 1);
        }
    }

    // ── 작업기 / Implement ──────────────────────────────────────────────────
    out.hitchPct = (cmd.implement == 1) ? HITCH_DOWN_PCT : HITCH_UP_PCT;

    controller_.setMachineOutputs(out);
}

// ─────────────────────────────────────────────────────────────────────────────
// 보고 / Report
// ─────────────────────────────────────────────────────────────────────────────

void TrackingLoop::report(const VehicleState& state, const TrackerCommand& cmd,
                          const TrackerDebug& debug, double dtS)
{
    auto ws = webSocket();
    const auto telemetryTracker = tracker();
    const int trackingBasePeriodMs = telemetryTracker
            ? telemetryTracker->preferredPeriodMs() : IDLE_PERIOD_MS;
    const auto clockTelemetry = makeClockTelemetry(machineClock_,
            state.poseSampleId, lastReportedPoseSampleId_,
            trackingBasePeriodMs, 100);

    // 실제 경과 시간을 그대로 누적합니다. 예전에는 여기에 시뮬레이션 배속과
    // 농지 이탈 페널티(10배)를 곱했는데, 둘 다 UI가 알려주던 값이라 사라졌습니다.
    //
    // Wall-clock dt accumulates as-is. This used to be scaled by the simulation
    // speed multiplier and the 10x outside-field penalty, both of which were fed
    // by the UI and are gone.
    accumulatedElapsedS_ += dtS;

    // ── 대시보드 텔레메트리 / Telemetry for the dashboard ────────────────────
    if (ws)
    {
        // 메시지 종류 이름("pp_telemetry")은 그대로 둡니다. 어떤 트래커가 돌든
        // 이 이름으로 나갑니다. 대시보드를 붙일 때 조향 PID 항·엔코더 각도·
        // FNR/SFT 실제 상태·모터 고장 비트 등을 여기에 추가하게 됩니다.
        //
        // The message name is kept whichever tracker runs. When the dashboard
        // lands, the steering PID terms, encoder angle, actual FNR/SFT state and
        // motor fault bits get added here.
        Json::Value msg;
        msg["kind"] = "pp_telemetry";
        msg["elapsedS"] = accumulatedElapsedS_;
        msg["ltd"] = debug.lateralDeviationM;   // Lateral Deviation
        msg["hde"] = debug.headingErrorRad;     // Heading Error (rad)
        msg["speedErr"] = debug.speedErrMps;
        msg["targetSpeedKmh"] = debug.targetSpeedKmh;
        msg["targetIdx"] = debug.targetIdx;
        msg["closestIdx"] = debug.closestIdx;
        msg["commandIdx"] = debug.commandIdx;
        // 조향 하위 루프 진단. 대시보드의 조향 화면이 이 값들로 게인을 봅니다.
        // Steering inner-loop diagnostics; the dashboard's steering screen tunes
        // the gains from these.
        msg["dtMs"] = dtS * 1000.0;
        msg["machineTimeScale"] = clockTelemetry.scale;
        msg["machineClockCalibrated"] = clockTelemetry.calibrated;
        msg["machineClockValid"] = clockTelemetry.valid;
        msg["machineClockSourceLoss"] = clockTelemetry.sourceLoss;
        msg["machineClockRegression"] = clockTelemetry.regression;
        msg["machineClockImplausibleJump"] = clockTelemetry.implausibleJump;
        msg["machineClockStatus"] = clockTelemetry.status;
        msg["poseSampleId"] = static_cast<Json::Int64>(clockTelemetry.poseSampleId);
        msg["poseTimestampDeltaMs"] = static_cast<Json::Int64>(
                clockTelemetry.poseTimestampDeltaMs);
        lastReportedPoseSampleId_ = state.poseSampleId;
        msg["trackingPeriodMs"] = clockTelemetry.trackingPeriodMs;
        msg["processTimerPeriodMs"] = clockTelemetry.processTimerPeriodMs;
        msg["steerTargetDeg"] = lastSteerDebug_.targetDeg;
        msg["steerMeasDeg"] = lastSteerDebug_.measuredDeg;
        msg["steerErrDeg"] = lastSteerDebug_.errorDeg;
        msg["steerP"] = lastSteerDebug_.pTerm;
        msg["steerI"] = lastSteerDebug_.iTerm;
        msg["steerD"] = lastSteerDebug_.dTerm;
        msg["steerEncoderWeight"] = lastSteerDebug_.encoderWeight;
        msg["steerCmd"] = lastSteerDebug_.motorCmd;
        msg["steerMeasValid"] = lastSteerDebug_.measuredValid;
        msg["steerBlocked"] = lastSteerDebug_.blocked;
        // 지령(steerCmd, ±600)과 실제 회전수(encRpm)를 나란히 두면 정격속도
        // 스케일이 맞는지 바로 보입니다.
        // Putting the command (steerCmd, ±600) next to the actual rpm shows at a
        // glance whether the rated-speed scaling is right.
        const ::AppMain::MachineInputs machineIn = controller_.getMachineInputs();
        const ::AppMain::MachineOutputs machineOut = controller_.getMachineOutputs();
        msg["encRpm"] = machineIn.steerEncoderSpeedRpm;
        msg["encDeg"] = machineIn.steerEncoderDeg;
        msg["accV1"] = machineIn.accSig1V;
        msg["accV2"] = machineIn.accSig2V;
        msg["controlAuthority"] = state.controlAuthority;
        msg["outputAuthority"] = machineOut.authority;
        msg["fnrState"] = state.fnrState;
        msg["shiftState"] = state.shiftState;
        msg["hitchPct"] = state.hitchPct;
        msg["gpsFix"] = state.gpsFixType;
        msg["gpsSats"] = state.gpsSats;
        msg["gpsHdop"] = state.gpsHdop;
        msg["motorFaultBits"] = static_cast<int>(state.motorFaultBits);

        // 주행 상태. tuning_state 에도 실려 나가지만, 대시보드의 상시 표시는
        // 텔레메트리 주기(10Hz)를 따라야 즉시 반응합니다 — tuning_state 는
        // 지시가 있을 때만 나갑니다.
        // Run state. It also rides in tuning_state, but the always-visible readout
        // must follow the 10 Hz telemetry to react immediately; tuning_state is
        // only emitted on a request.
        msg["runState"] = ::AppMain::runStateName(controller_.getRunState());
        msg["readyToStart"] = controller_.isReadyToStart();
        msg["waypointCount"] = static_cast<int>(
                controller_.getTrackedWaypoints().size());

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 차량 자체의 상태. 오차만 보내면 "지금 차가 어떤 자세로 어디를 향해
        // 가고 있는가"를 알 수 없습니다. 대시보드가 방위·자세 계기를 그리는 데
        // 쓰고, 실측 속도는 목표에서 역산하지 않고 그대로 표시합니다.
        //
        // 각도는 전부 도(deg)로 변환해 보냅니다 — 표시 단위가 도이므로 변환을
        // 한 곳에서 끝내는 편이 낫습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // The vehicle's own state. Errors alone cannot say what attitude the
        // machine is in or where it is heading. The dashboard draws its heading and
        // attitude instruments from these, and shows the measured speed directly
        // instead of deriving it from the target.
        //
        // Angles are converted to degrees here, since that is the display unit and
        // one conversion site is better than several.
        // └────────────────────────────────────────────────────────────────────┘
        constexpr double RAD_TO_DEG = 180.0 / M_PI;
        msg["speedMps"] = state.speedMps;
        msg["headingDeg"] = controller_.getLastGpsHeading();
        msg["enuX"] = state.x;
        msg["enuZ"] = state.z;
        msg["rollDeg"] = state.rollRad * RAD_TO_DEG;
        msg["pitchDeg"] = state.pitchRad * RAD_TO_DEG;
        msg["attitudeValid"] = state.attitudeValid;
        msg["yawRateDegS"] = state.yawRateRadS * RAD_TO_DEG;
        msg["yawRateValid"] = state.yawRateValid;
        msg["accelX"] = state.accelX;
        msg["accelY"] = state.accelY;
        // 먼저 필수 키를 기본값으로 깔고, 그 위에 트래커가 실제로 발행한
        // extras를 덮어씁니다. / Lay down the required keys first, then let the
        // tracker's actual extras overwrite them.
        for (const char* key : REQUIRED_TELEMETRY_EXTRAS) {
            msg[key] = debug.get(key);
        }
        for (const auto& kv : debug.extras) {
            msg[kv.first] = kv.second;
        }
        ws->publishMessage(toCompact(msg));
    }

    writeClockLine(state, debug, makeClockTelemetry(machineClock_,
            state.poseSampleId, lastClockLogPoseSampleId_,
            trackingBasePeriodMs, 100));
    writeDiagnosticLine(state, cmd, debug);
    writeTraceRow(state, cmd, debug);
}

void TrackingLoop::writeClockLine(const VehicleState& state,
                                  const TrackerDebug& debug,
                                  const ClockTelemetry& telemetry)
{
    if (!clockLogRateLimiter_.shouldLog(std::chrono::steady_clock::now())) {
        return;
    }
    const ::AppMain::MachineOutputs machineOut = controller_.getMachineOutputs();
    ClockLogValues values;
    values.clock = telemetry;
    values.outputAuthority = machineOut.authority;
    values.runState = ::AppMain::runStateName(controller_.getRunState());
    values.speedMps = state.speedMps;
    values.waypointIndex = debug.closestIdx;
    values.steerTargetDeg = lastSteerDebug_.targetDeg;
    values.outputMotorCommand = machineOut.motorCmd;
    std::cerr << formatClockLog(values) << std::endl;
    lastClockLogPoseSampleId_ = state.poseSampleId;
}

/**
 * 주기마다 한 줄씩 표준에러로 나가는 진단 로그입니다. 주행 중 터미널에서
 * 눈으로 따라가기 위한 것으로, 자세한 분석은 CSV 트레이스를 쓰세요.
 *
 * One diagnostic line per tick on stderr, for watching a run live in a
 * terminal. Use the CSV trace for detailed analysis.
 */
void TrackingLoop::writeDiagnosticLine(const VehicleState& state,
                                       const TrackerCommand& cmd,
                                       const TrackerDebug& debug) const
{
    // 예전에는 조향각에 상수를 곱해 "rpm"을 만들어 찍었습니다. 이제는 조향
    // 하위 루프가 낸 실제 모터 지령과 실측각을 그대로 보여줍니다.
    // This used to print a "rpm" derived by scaling the steering angle. It now
    // shows the actual motor command from the steering loop and the measurement.
    const ::AppMain::MachineOutputs machineOut = controller_.getMachineOutputs();
    std::cerr << std::fixed << std::setprecision(2)
        << "PP_LOG"
        << " enu=(" << state.x << "," << state.z << ")"
        << " hdg_deg=" << controller_.getLastGpsHeading()
        << " h_pp_rad=" << std::setprecision(4) << state.headingRad
        << std::setprecision(2)
        << " ci=" << debug.closestIdx
        << " cmd=" << debug.commandIdx
        << " ti=" << debug.targetIdx
        << " ltd=" << debug.lateralDeviationM
        << " hde=" << std::setprecision(4) << debug.headingErrorRad
        << " steer_tgt_deg=" << lastSteerDebug_.targetDeg
        << " steer_meas_deg=" << lastSteerDebug_.measuredDeg
        << " steer_cmd=" << lastSteerDebug_.motorCmd
        << " enc_w=" << std::setprecision(3) << lastSteerDebug_.encoderWeight
        // 융합된 값 하나만으로는 내루프를 되짚을 수 없습니다. 원시 엔코더와
        // yaw rate 를 같이 찍어야 엔코더/자이로/원점보정 셋을 분리할 수 있습니다.
        // A single fused number cannot be unpicked into its three sources; the raw
        // encoder and the yaw rate are needed to separate encoder, gyro and trim.
        << " enc_raw=" << lastEncoderDeg_
        << " enc_ok=" << (lastEncoderValid_ ? 1 : 0)
        << " yaw_rads=" << std::setprecision(4) << state.yawRateRadS
        << std::setprecision(2)
        << " yaw_ok=" << (state.yawRateValid ? 1 : 0)
        << std::setprecision(3)
        << " thr=" << cmd.throttle
        << " brk=" << cmd.brake
        << " gear=" << cmd.gear
        << " out_valid=" << (machineOut.valid ? 1 : 0)
        << " out_auth=" << (machineOut.authority ? 1 : 0)
        << " out_acc=" << machineOut.accelPct
        << " out_fnr=" << machineOut.fnr
        << " out_sft=" << machineOut.shift
        << std::setprecision(1)
        << " tV_kmh=" << debug.targetSpeedKmh
        << std::setprecision(2)
        << " currV_mps=" << state.speedMps
        << " dV_mps=" << debug.speedErrMps
        << std::endl;
}

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * /tmp/pp_trace.csv 에 주기마다 한 줄을 기록합니다. 주행이 끝난 뒤 파이썬이나
 * 스프레드시트로 궤적과 경로를 비교하며 튜닝할 때 쓰는 파일입니다.
 * 매 실행마다 새로 씁니다(덮어쓰기).
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Appends one row per tick to /tmp/pp_trace.csv, for offline
 * trajectory-versus-path analysis after a run. Truncated on each run.
 * └────────────────────────────────────────────────────────────────────────────┘
 */
void TrackingLoop::writeTraceRow(const VehicleState& state,
                                 const TrackerCommand& cmd,
                                 const TrackerDebug& debug)
{
    if (!traceInitialised_)
    {
        traceInitialised_ = true;
        traceT0_ = std::chrono::steady_clock::now();
        traceFile_.open(TRACE_PATH, std::ios::out | std::ios::trunc);
        if (traceFile_.is_open())
        {
            // 트래커의 extras를 포함해 컬럼 구성을 여기서 확정합니다. 이후
            // 행들이 어긋나지 않게 하기 위한 조치입니다.
            // Column layout is frozen here, including the tracker's extras, so
            // that later rows cannot drift out of alignment.
            traceFile_ << "t_ms,veh_x,veh_z,hdg_rad,speed_mps,"
                << "closest_idx,command_idx,target_idx,target_speed_kmh,"
                << "steer_rad,throttle,brake,gear,implement,ltd,hde,speed_err";
            for (const auto& kv : debug.extras) {
                traceExtraKeys_.push_back(kv.first);
                traceFile_ << "," << kv.first;
            }
            traceFile_ << "\n";
        }
    }
    if (!traceFile_.is_open()) return;

    std::ofstream& csv = traceFile_;
    const long tMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - traceT0_).count();

    csv << std::fixed << std::setprecision(4);
    csv << tMs << ","
        << state.x << "," << state.z << ","
        << state.headingRad << "," << state.speedMps << ","
        << debug.closestIdx << "," << debug.commandIdx << "," << debug.targetIdx << ","
        << debug.targetSpeedKmh << ","
        << cmd.steerAngle << "," << cmd.throttle << "," << cmd.brake << ","
        << cmd.gear << "," << cmd.implement << ","
        << debug.lateralDeviationM << "," << debug.headingErrorRad << "," << debug.speedErrMps;
    // 헤더에 고정한 키 순서대로만 씁니다. 없는 값은 0으로 채워집니다.
    // Written in the exact key order frozen into the header; missing values become 0.
    for (const auto& key : traceExtraKeys_) {
        csv << "," << debug.get(key);
    }
    csv << "\n";
    csv.flush();
}

// ─────────────────────────────────────────────────────────────────────────────
// 종료 시퀀스 / Finish sequence
// ─────────────────────────────────────────────────────────────────────────────

void TrackingLoop::onComplete()
{
    if (controller_.wasTrackCompleteSent()) {
        const auto state=controller_.getOrchestrationSnapshot();
        const auto now=std::chrono::steady_clock::now();
        if(state.finalization==orchestration::FinalizationState::RetryableError
            && (!completeAtT_ || now-*completeAtT_>=std::chrono::seconds(1))) {
            completeAtT_=now;
            controller_.requestOrchestrationFinish(true);
        }
        return;
    }
    completeAtT_ = std::chrono::steady_clock::now();
    elapsedSAtComplete_ = accumulatedElapsedS_;
    controller_.markTrackCompleteSent();
    // Claim finalization on the very first completed tick. request...Finish
    // synchronously releases authority and commands before its managed worker
    // performs recorder/network I/O. Finished is entered only after the exact
    // CSV is stopped, fetched and durably persisted.
    controller_.requestOrchestrationFinish(true);

    auto ws = webSocket();
    if (!ws) return;

    // 채점용 RunSummary 확정·디스크 저장과 리더보드 finished 전송이 여기 있었습니다.
    // 남은 것은 "경로 주행을 마쳤다"는 사실 하나이고, 이건 대시보드가 알아야 하는
    // 자율주행 상태이므로 그대로 발행합니다.
    //
    // Finalising the scored RunSummary, persisting it to disk and the leaderboard
    // "finished" upload all lived here. What remains is the bare fact that the
    // path is done — an autonomous-driving state the dashboard needs — so that is
    // still published.
    {
        Json::Value done;
        done["kind"] = "track_complete";
        done["elapsedS"] = elapsedSAtComplete_;
        ws->publishMessage(toCompact(done));
    }

}

} // namespace tracking
} // namespace AppMain
