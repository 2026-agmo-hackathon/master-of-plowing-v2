/*
 * AppMain/MainController.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_MAINCONTROLLER_HPP
#define APPMAIN_MAINCONTROLLER_HPP

#include <ecorecpp/mapping_forward.hpp>

#include <AppMain_forward.hpp>

#include <nevonex/can_agmo_customized_tractor_forward.hpp>
#include <nevonex/can_agmo_mtlt305_forward.hpp>
#include <nevonex/can_agmo_steermotor_forward.hpp>
#include <nevonex/isopgn_forward.hpp>
#include <nevonex/serial_ext_gps_nmea0183_forward.hpp>
#include <AppMain/ApplicationInputData.hpp>
#include <AppMain/IController.hpp>

#include <nevonex.hpp>

using namespace ::ecore;
using namespace ::ecorecpp::mapping;

using namespace ::nevonex;
using namespace ::nevonex::fcal;
using namespace ::nevonex::common;
using namespace ::nevonex::types;

using namespace ::nevonex::can_agmo_customized_tractor;

using namespace ::nevonex::can_agmo_mtlt305;

using namespace ::nevonex::can_agmo_steermotor;

using namespace ::nevonex::isopgn;

using namespace ::nevonex::serial_ext_gps_nmea0183;

/*PROTECTED REGION ID(MainController_pre) ENABLED START*/
// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 머신 I/O 스냅샷 구조체를 여기 둡니다. 별도 헤더(MachineIO.hpp)에 있었는데,
// 생성되지 않는 파일은 마이그레이션에서 따라오지 않으므로 보존되는 자리로
// 옮겼습니다. 손으로 쓴 코드는 전부 PROTECTED REGION 안에 있어야 합니다.
//
// ── 왜 스냅샷인가 ──
// 두 스레드가 있습니다. ProcessTimer 스레드(MainController::run(), 100ms)만
// SDK 머신 객체를 만지고, TrackingLoop 스레드는 스냅샷만 읽고 씁니다. SDK
// 객체의 스레드 안전성이 문서로 보장되지 않고, 참조 구현(agmo_solution)도
// CAN 입출력을 ProcessTimer 스레드에만 두기 때문입니다.
//
// ── 값이 없는 것과 0인 것을 구분합니다 ──
// Adhoc 신호는 질의 응답이 오기 전까지 값이 없습니다. 특히 조향 엔코더가
// 그런데, 응답이 없는 것을 0도로 착각하면 조향 PID 가 "지금 직진 중"이라고
// 믿고 최대 조향을 걸어버립니다. 그래서 *Valid 플래그를 함께 둡니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The machine I/O snapshots live here. They used to sit in a separate
// MachineIO.hpp, but a file the generator does not emit is not carried through a
// migration, so they moved somewhere that is preserved: all hand-written code
// belongs inside a PROTECTED REGION.
//
// WHY SNAPSHOTS: only the ProcessTimer thread (MainController::run(), 100 ms)
// touches SDK machine objects; the TrackingLoop thread only reads and writes
// these structs. The SDK objects carry no documented thread-safety guarantee, and
// the reference implementation keeps all CAN I/O on the ProcessTimer thread too.
//
// MISSING IS NOT ZERO: Adhoc signals have no value until a response arrives. The
// steering encoder especially — mistaking "no response" for 0 degrees would make
// the steering PID believe the wheels are straight and command full lock. Hence
// the paired *Valid flags.
// └────────────────────────────────────────────────────────────────────────────┘
#include <memory>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>
#include <AppMain/RddfParser.hpp>
// TrackerParams 보관과 trackerName_ 기본값(defaultTrackerName())에 필요합니다.
// Needed for the TrackerParams storage and the trackerName_ default.
#include <AppMain/tracking/TrackerTypes.hpp>
#include <AppMain/tracking/TrackerFactory.hpp>
#include <AppMain/tracking/GpsSampleStore.hpp>
#include <AppMain/tracking/ControlTimeGate.hpp>
#include <AppMain/tracking/SignalFreshness.hpp>
#include <AppMain/orchestration/RunOrchestrator.hpp>
#include <AppMain/orchestration/LaunchLifecycle.hpp>
#include <atomic>
#include <functional>
#include <deque>
#include <future>
#include <condition_variable>

namespace AppMain
{
struct MachineInputs
{
    // ── 조향 모터 (CAN_AGMO_SteerMotor) ─────────────────────────────────────
    // 엔코더 위치. 단위는 인터페이스 정의상 도(deg)이고 모터축 기준입니다.
    // 앞바퀴각으로 쓰려면 조향비로 나눠야 합니다.
    // Encoder position in degrees (per the interface definition), on the motor
    // shaft — divide by the steering ratio for a wheel angle.
    double steerEncoderDeg = 0.0;
    bool steerEncoderValid = false;
    // SDK interface timestamp: the sample identity used by the steering PID.
    // A repeated timestamp is a held frame even when the retained value changes
    // slightly elsewhere in the fusion pipeline.
    std::int64_t steerEncoderSampleId = -1;
    // 엔코더 속도, rpm. 지령과 비교하면 정격속도 스케일을 확인할 수 있습니다.
    // Encoder speed in rpm; comparing it against the command reveals the rated
    // speed scaling.
    int steerEncoderSpeedRpm = 0;
    bool steerEncoderSpeedValid = false;
    // Motor_Heartbeat 10비트를 접은 값. TrackerTypes.hpp 의 MotorFaultBit 참고.
    // The ten Motor_Heartbeat flags folded into one word; see MotorFaultBit.
    unsigned int motorFaultBits = 0;
    bool motorHeartbeatValid = false;

    // ── 자세·관성 (CAN_AGMO_MTLT305) ────────────────────────────────────────
    // 인터페이스가 도(deg)와 deg/s 로 주므로 여기까지는 도 단위로 들고 옵니다.
    // 라디안 변환은 VehicleState 를 만들 때 한 번만 합니다.
    // The interface reports degrees and deg/s, so degrees are carried this far;
    // the conversion to radians happens once, when VehicleState is built.
    double rollDeg = 0.0;
    double pitchDeg = 0.0;
    bool attitudeValid = false;
    double gyroZDegS = 0.0;
    bool gyroValid = false;
    std::int64_t gyroSampleId = -1;
    double accelX = 0.0;  // m/s^2
    double accelY = 0.0;  // m/s^2
    bool accelValid = false;

    // ── 트랙터 (CAN_AGMO_Customized_Tractor) ────────────────────────────────
    // 실제 체결 상태. 0 = 모름.
    //   fnrState   : 1 = R, 2 = N, 3 = F
    //   shiftState : 1 = Low, 2 = Middle, 3 = High
    // Actual engaged state; 0 means unknown.
    int fnrState = 0;
    int shiftState = 0;
    // 자율 제어권을 수락했는가. 우리가 보낸 mode 비트에 대한 응답입니다.
    // Whether autonomous authority was granted — the reply to our mode bit.
    bool accAuto = false;
    bool fnrAuto = false;
    bool sftAuto = false;
    bool hydAuto = false;
    // 가속 페달 전압 2채널(이중화). 개도 지령의 실제 반영을 확인하는 값입니다.
    // Accelerator pedal voltage, two redundant channels; used to confirm that the
    // opening command actually landed.
    double accSig1V = 0.0;
    double accSig2V = 0.0;
    // 진단 코드. 0이 아니면 해당 계통에 이상이 있습니다.
    // Diagnostic codes; non-zero means that subsystem has a fault.
    int accDiag = 0;
    int fnrDiag = 0;
    int sftDiag = 0;
    int hydDiag = 0;

    // ── 히치 (ISOPGN.HitchPosPerRe) ─────────────────────────────────────────
    double hitchPct = 0.0;
    bool hitchValid = false;

    // ── GPS 품질 (Serial_Ext_GPS_NMEA0183_Data) ─────────────────────────────
    // 위치·속도·방위 자체는 기존 MainController::setCurrentGps() 경로로 갑니다.
    // 여기 담는 것은 그 값을 믿어도 되는지 판단하는 품질 지표뿐입니다.
    // Position/speed/heading go through the existing setCurrentGps() path; what is
    // carried here is only the quality needed to decide whether to trust them.
    int gpsFixType = 0;  // NMEA GGA quality indicator (4 = RTK fixed, 5 = float)
    int gpsSats = 0;
    double gpsHdop = 0.0;

    // 한 번이라도 머신에서 값을 읽었는가. / True once anything was read at all.
    bool valid = false;
};

/**
 * 머신으로 내보낼 지령. 제어 루프가 채우고 ProcessTimer 스레드가 씁니다.
 * Commands to send: filled by the control loop, written by the ProcessTimer thread.
 */
struct MachineOutputs
{
    // 조향 모터 속도 지령. 정격속도의 천분율이고 ±600으로 제한됩니다.
    // 0은 "정지 유지"가 아니라 Disable(조향 릴리스)입니다 — KY170 프로토콜에서
    // 속도 0 지령과 Disable을 구분할 수단이 없어 참조 구현도 그렇게 씁니다.
    //
    // Steering motor speed in per-mille of rated speed, limited to ±600. Zero
    // means Disable (release steering), not "hold": the KY170 protocol gives no
    // way to distinguish a zero-speed command from disabling, and the reference
    // implementation treats it the same way.
    int motorCmd = 0;

    // 가속 개도. 0..100 %.  -> Send_ACC_CMD.AD_Generic_Cmd
    int accelPct = 0;
    // 전후진. 1 = R, 2 = N, 3 = F.  -> Send_FNR_CMD.AD_Generic_Cmd
    int fnr = 2;
    // 변속 단수. 1 = Low, 2 = Middle, 3 = High.  -> Send_SFT_CMD.AD_Generic_Cmd
    int shift = 1;
    // 히치 위치. 0..100 %.  -> Send_HYD_CMD.AD_Generic_Cmd
    int hitchPct = 0;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 자율 제어권 요청. 네 개 CMD 의 AD_Generic_Mode_Cmd 에 함께 실려 나갑니다
    // (1 = 요청, 0 = 해제). 별도 핸드셰이크가 아니라 매 주기 지령과 같이 보내는
    // 방식이고, 수락 여부는 MachineInputs 의 *Auto 로 돌아옵니다.
    //
    // false면 모든 지령을 0으로 보냅니다 — 제어권을 놓는 동안 마지막 지령이
    // 남아 있으면 안 되기 때문입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Autonomous authority request, carried on AD_Generic_Mode_Cmd of all four
    // commands (1 = request, 0 = release). It is not a separate handshake but a
    // bit sent alongside every command; the grant comes back as the *Auto fields.
    //
    // When false, every command goes out as zero — a released authority must not
    // leave the last command standing.
    // └────────────────────────────────────────────────────────────────────────┘
    bool authority = false;

    // 제어 루프가 한 번이라도 계산했는가. false면 run()은 아무것도 쓰지 않고
    // 안전 상태(전부 0, 제어권 해제)를 내보냅니다.
    // True once the control loop has produced anything. While false, run() writes
    // the safe state (all zero, authority released) instead.
    bool valid = false;
};

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 주행 상태. 자율주행을 시작·일시정지·종료하는 상태 기계입니다.
 *
 * ── 왜 필요한가 ──
 *
 * 이게 없을 때는 RDDF 를 받는 순간 차가 움직였습니다. 경로·GPS·원점이 갖춰지면
 * TrackingLoop 이 바로 지령을 쏘기 시작했기 때문입니다. 파일이 도착하는 시점을
 * 사람이 통제할 수 없으므로, 작업자가 준비되기 전에 트랙터가 출발할 수 있었습니다.
 *
 * 이제 적재는 "준비"까지만이고, 움직이는 것은 명시적인 시작 지시가 있을 때뿐입니다.
 *
 *   Idle      RDDF 가 없거나, 있어도 아직 시작하지 않음. 지령은 전부 안전값.
 *   Running   정상 제어.
 *   Paused    주행 중 멈춤. 개도 0 · 중립 · 조향 해제. 제어권은 유지해 즉시 재개.
 *   Finished  경로를 끝까지 주행함. 다시 시작하려면 Idle 로 되돌립니다.
 *
 * Paused 에서 적분 상태를 버리지 않는 이유: 게인은 그대로인데 경사에서 잡아 둔
 * 오프셋까지 버리면 재개할 때마다 차가 출렁입니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Run state: the machine that starts, pauses and ends autonomous driving.
 *
 * WHY: without it, the vehicle moved the moment an RDDF arrived — TrackingLoop
 * began issuing commands as soon as a path, a GPS fix and an origin existed. Nobody
 * controls when the file lands, so the tractor could pull away before the operator
 * was ready. Loading now only *arms*; motion requires an explicit start.
 *
 * Paused keeps the integrator state: the gains have not changed, and discarding the
 * slope offset would jolt the vehicle on every resume.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */
enum class RunState : int
{
    Idle = 0,
    Running,
    Paused,
    Finished
};

inline const char* runStateName(RunState s)
{
    switch (s) {
        case RunState::Running:  return "running";
        case RunState::Paused:   return "paused";
        case RunState::Finished: return "finished";
        case RunState::Idle:
        default:                 return "idle";
    }
}

/**
 * 주행 중인가 (지령을 실제로 내보내는 상태인가).
 * Whether commands are actually being issued.
 */
inline bool isDriving(RunState s)
{
    return s == RunState::Running;
}

/**
 * 주행 회차가 진행 중인가. 이때는 파라미터를 잠급니다 — 게인이나 알고리즘을
 * 주행 도중 바꾸면 거동이 갑자기 달라지고, 어떤 설정으로 달린 결과인지도
 * 알 수 없게 됩니다. Paused 도 포함합니다: 잠시 멈춘 것이지 회차가 끝난 게
 * 아니기 때문입니다.
 *
 * Whether a run is in progress, which locks the parameters: changing a gain or the
 * algorithm mid-run changes the behaviour abruptly and destroys the record of what
 * setting produced the result. Paused counts — it is a pause, not an end.
 */
inline bool isRunLocked(RunState s)
{
    return s == RunState::Running || s == RunState::Paused;
}

    namespace tracking
    {
        class IPathTracker;
        // forward declaration
        class TrackingLoop;
    // forward declaration
    }// namespace tracking
} // namespace AppMain
/*PROTECTED REGION END*/

namespace AppMain
{

    class MainController: public virtual ::AppMain::ApplicationInputData,
            public virtual ::AppMain::IController
    {
        /*PROTECTED REGION ID(MainController_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        MainController();

        virtual ~MainController();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
    public:

        /*PROTECTED REGION ID(MainController) ENABLED START*/
            public:
        using GpsSample = tracking::GpsSample;
        void setCurrentGps(double lat, double lon, double heading, double speed,
                double alt, std::int64_t interfaceTimestamp = -1)
        {
            gpsSamples_.update(lat, lon, heading, speed, alt,
                    interfaceTimestamp);
        }
        void setMachineTimeScale(double scale)
        {
            machineTimeScale_.store(std::clamp(scale, 1.0, 6.0));
        }
        double getMachineTimeScale() const { return machineTimeScale_.load(); }
        GpsSample getGpsSample() const
        {
            return gpsSamples_.snapshot();
        }
        // The app only ever refers to the tracking interface; the concrete
        // algorithm is chosen by tracking::createTracker(). See
        // AppMain/tracking/TrackerFactory.hpp.
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // tracker_ 는 이제 런타임에도 교체됩니다(WS 스레드의 set_tracker).
        // shared_ptr 자체의 대입/복사는 같은 객체에 대해 스레드 안전하지
        // 않으므로 tuningMutex_ 로 보호합니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // tracker_ is now also swapped at runtime (set_tracker on the WS
        // thread). Assigning/copying the same shared_ptr object is not
        // thread-safe, so both accessors lock tuningMutex_.
        // └────────────────────────────────────────────────────────────────────┘
        void setTracker(std::shared_ptr< tracking::IPathTracker > tracker)
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            tracker_ = tracker;
        }
        std::shared_ptr< tracking::IPathTracker > getTracker() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return tracker_;
        }

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 런타임 튜닝 상태. MachineInputs 스냅샷과 같은 패턴입니다:
        //   쓰기 = WebSocket 콜백 스레드 (set_params / set_tracker)
        //   읽기 = TrackingLoop 스레드 (매 tick), WS 스레드 (tuning_state 방송)
        // 구조체가 작으므로 값 복사가 락 구간을 짧게 유지하는 가장 단순한
        // 방법입니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Runtime tuning state, same pattern as the MachineInputs snapshots:
        // written by the WebSocket callback thread, read by the TrackingLoop
        // thread every tick. The structs are small, so copying by value keeps
        // the critical section short.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 주행 상태. 파라미터와 같은 뮤텍스로 보호합니다 — 둘을 함께 판단해야
        // 하기 때문입니다(주행 중이면 파라미터 변경 거부). 별 뮤텍스로 나누면
        // "상태를 읽고 나서 잠금 여부를 판단하는 사이에 상태가 바뀌는" 창이
        // 생깁니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Run state shares the tuning mutex because the two are decided together
        // (a running machine refuses parameter changes). Separate mutexes would
        // open a window where the state changes between reading it and deciding
        // whether the parameters are locked.
        // └────────────────────────────────────────────────────────────────────┘
        ::AppMain::RunState getRunState() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return runState_;
        }

            private:
        int nextProcessIntervalMs(const int configuredIntervalMs) const override
        {
            return tracking::normalizedPeriodMs(configuredIntervalMs,
                    getMachineTimeScale());
        }
        /**
         * isReadyToStart() 의 잠금-없는 버전입니다. 호출자가 이미 tuningMutex_
         * 를 쥐고 있을 때만 불러야 합니다(예: setRunState() 내부). tracker_ 를
         * getTracker() 로 다시 읽으면 같은 뮤텍스를 재귀적으로 잠그려 해
         * 교착합니다 — 그래서 tracker_ 를 직접 읽습니다.
         *
         * Lock-free counterpart of isReadyToStart(). Callers must already hold
         * tuningMutex_ (e.g. from inside setRunState()). Re-fetching the
         * tracker via getTracker() would re-lock the same mutex and deadlock,
         * so this reads tracker_ directly instead.
         */
        bool isReadyToStartLocked() const
        {
            return tracker_ != nullptr && hasGps() && hasTrackOrigin();
        }

            public:
        /**
         * 시작할 수 있는 상태인가. 경로·GPS 수신·좌표 원점이 모두 있어야 합니다.
         * 조향 모터 고장은 여기서 보지 않습니다 — 고장 인터록은
         * SteeringController 가 매 주기 판단하며, 고장 중에도 "시작을 눌렀다"는
         * 의도 자체는 유효합니다(고장이 풀리면 이어서 달립니다).
         *
         * Whether a start is possible: a path, a GPS fix and an origin must all
         * exist. Motor faults are not checked here — SteeringController enforces
         * that interlock every tick, and the intent to start stays valid across a
         * transient fault.
         */
        bool isReadyToStart() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return isReadyToStartLocked();
        }

        /**
         * ┌─ 한국어 ───────────────────────────────────────────────────────────┐
         * 주행 상태를 바꿉니다. 허용되지 않는 전환은 false 를 돌려주고 상태를
         * 그대로 둡니다 — UI 가 보낸 지시를 무조건 믿지 않습니다.
         *
         *   Idle/Finished -> Running   준비돼 있어야만 (isReadyToStart)
         *   Running       -> Paused
         *   Paused        -> Running
         *   무엇이든      -> Idle      (종료는 항상 허용 — 안전한 방향입니다)
         *   Running       -> Finished  (완주. TrackingLoop 이 부릅니다)
         *
         * 종료(Idle)로 가는 전환만 무조건 허용하는 이유는 그것이 유일하게
         * "차를 멈추는" 방향이기 때문입니다. 안전한 방향의 지시를 검증으로
         * 막아서는 안 됩니다.
         * └────────────────────────────────────────────────────────────────────┘
         * ┌─ English ──────────────────────────────────────────────────────────┐
         * Changes the run state, returning false and leaving it untouched for a
         * disallowed transition — a UI request is not taken on trust.
         *
         * Only the transition to Idle is unconditional, because it is the one that
         * stops the vehicle. A request in the safe direction must never be blocked
         * by validation.
         * └────────────────────────────────────────────────────────────────────┘
         */
        bool setRunState(::AppMain::RunState desired)
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            if (desired == runState_) {
                return true;
            }
            switch (desired) {
                case ::AppMain::RunState::Idle:
                    // 종료는 언제나 허용. / Stopping is always allowed.
                    break;
                case ::AppMain::RunState::Running:
                    if (runState_ == ::AppMain::RunState::Paused) {
                        break;  // 재개 / resume
                    }
                    // 새 회차 시작: 준비 상태를 직접 확인합니다. 이 지점은 이미
                    // tuningMutex_ 를 쥐고 있으므로 잠금-없는 버전을 써야 합니다
                    // — 공개 isReadyToStart() 는 스스로 tuningMutex_ 를 다시
                    // 잠가 자기-교착합니다.
                    // Starting a new run checks readiness directly. tuningMutex_
                    // is already held here, so the lock-free variant must be
                    // used — the public isReadyToStart() locks tuningMutex_
                    // itself (via its own guard) and would self-deadlock.
                    if (!isReadyToStartLocked()) {
                        return false;
                    }
                    break;
                case ::AppMain::RunState::Paused:
                    if (runState_ != ::AppMain::RunState::Running) {
                        return false;  // 달리고 있지 않으면 멈출 것도 없습니다
                    }
                    break;
                case ::AppMain::RunState::Finished:
                    if (runState_ != ::AppMain::RunState::Running) {
                        return false;
                    }
                    break;
            }
            runState_ = desired;
            return true;
        }

        void markRunFinished()
        {
            std::lock_guard<std::mutex> guard(tuningMutex_);
            runState_ = ::AppMain::RunState::Finished;
        }

        void setRunGeometry(const ::AppMain::tracking::VehicleGeometry& geometry)
        {
            if (!geometry.valid()) return;
            std::lock_guard<std::mutex> guard(tuningMutex_);
            if (runGeometryFrozen_) return;
            runGeometry_ = geometry;
            runGeometryFrozen_ = true;
        }
        ::AppMain::tracking::VehicleGeometry getRunGeometry() const
        {
            std::lock_guard<std::mutex> guard(tuningMutex_);
            return runGeometry_;
        }

        void requestOrchestrationRefresh();
        void requestOrchestrationSetup(const ::AppMain::orchestration::StartRequest& request);
        void requestOrchestrationStart(const ::AppMain::orchestration::StartRequest& request);
        bool acknowledgeSealedCapture(const std::string& runId);
        bool beginSealedCapture(const std::string& runId);
        bool failSealedCapture(const std::string& runId);
        bool markLeaderboardSubmitted(const std::string& runId,const std::string& verdict);
        bool beginResetCleanup(const std::string& runId);
        bool beginPurgeAllCleanup();
        bool noteResetCleanupPrepared(unsigned long long generation);
        bool failResetCleanup(const std::string& runId);
        bool acknowledgeResetCleanup(const std::string& runId);
        void requestOrchestrationFinish(bool automatic);
        void requestOrchestrationReset();
        bool isOrchestrationFinishRequested() const
        {
            return orchestrationFinishRequested_.load();
        }
        ::AppMain::orchestration::Snapshot getOrchestrationSnapshot() const;
        bool loadRecording(const std::string& id, std::string& bytes) const;
        std::vector< ::AppMain::orchestration::UnsubmittedRecord > listUnsubmittedRecordings() const;
        bool purgeUnsubmittedRecordings();

        /** 주행 중이라 파라미터를 잠가야 하는가. / Whether a run locks the parameters. */
        bool areParamsLocked() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return ::AppMain::isRunLocked(runState_);
        }

        void setTrackerParams(const ::AppMain::tracking::TrackerParams &params)
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            trackerParams_ = params;
        }
        ::AppMain::tracking::TrackerParams getTrackerParams() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return trackerParams_;
        }
        void setTrackerName(const std::string &name)
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            trackerName_ = name;
        }
        std::string getTrackerName() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return trackerName_;
        }
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // RddfLoader 가 만든 ENU 웨이포인트 사본. 알고리즘 전환 시 RDDF 파일을
        // 다시 읽지 않고 새 트래커에 setTrack() 하기 위해 보관합니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Copy of the ENU waypoints RddfLoader produced, kept so an algorithm
        // switch can setTrack() the new tracker without re-reading the file.
        // └────────────────────────────────────────────────────────────────────┘
        void setTrackedWaypoints(
                const std::vector< ::AppMain::tracking::TrackerWaypoint > &wps)
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            trackedWaypoints_ = wps;
        }
        std::vector< ::AppMain::tracking::TrackerWaypoint > getTrackedWaypoints() const
        {
            std::lock_guard< std::mutex > guard(tuningMutex_);
            return trackedWaypoints_;
        }
        bool hasGps() const
        {
            return getGpsSample().valid;
        }
        double getLastGpsLat() const
        {
            return getGpsSample().lat;
        }
        double getLastGpsLon() const
        {
            return getGpsSample().lon;
        }
        double getLastGpsHeading() const
        {
            return getGpsSample().heading;
        }
        double getLastGpsSpeed() const
        {
            return getGpsSample().speed;
        }
        double getLastGpsAlt() const
        {
            return getGpsSample().alt;
        }

        void setTrackOrigin(double utmE, double utmN)
        {
            trackOriginE_ = utmE;
            trackOriginN_ = utmN;
            trackOriginValid_ = true;
        }
        void setTrackOriginGps(double lat, double lon)
        {
            trackOriginLat_ = lat;
            trackOriginLon_ = lon;
            trackOriginGpsValid_ = true;
        }
        bool hasTrackOrigin() const
        {
            return trackOriginValid_;
        }
        double trackOriginE() const
        {
            return trackOriginE_;
        }
        double trackOriginN() const
        {
            return trackOriginN_;
        }
        bool hasTrackOriginGps() const
        {
            return trackOriginGpsValid_;
        }
        double trackOriginLat() const
        {
            return trackOriginLat_;
        }
        double trackOriginLon() const
        {
            return trackOriginLon_;
        }
        bool wasTrackCompleteSent() const
        {
            return trackCompleteSent_;
        }
        void markTrackCompleteSent()
        {
            trackCompleteSent_ = true;
        }
        void resetTrackCompleteSent()
        {
            trackCompleteSent_ = false;
        }
        // Raw GPS waypoints (lat/lon, not ENU), set alongside setTracker().
        // Kept so an observability dashboard can draw the planned path without
        // redoing the UTM conversion.
        void setRawWaypoints(const std::vector< ::AppMain::RddfWaypoint > &wps)
        {
            rawWaypoints_ = wps;
        }
        const std::vector< ::AppMain::RddfWaypoint >& getRawWaypoints() const
        {
            return rawWaypoints_;
        }

        /**
         * ┌─ 한국어 ───────────────────────────────────────────────────────────┐
         * 새 경로를 적재하기 직전에 주행 상태를 지웁니다. RddfLoader 가 부릅니다.
         *
         * 이게 없으면 두 번째 RDDF 를 받았을 때 이렇게 됩니다.
         *   - trackCompleteSent_ 가 지난 주행에서 true 로 남아 있어
         *     TrackingLoop::onComplete() 가 조기 반환하고 완주 방송이 안 나갑니다
         *   - accumulatedElapsedS_ 가 이전 주행에서 이어져 경과 시간이 틀립니다
         *   - 지난 지령이 머신으로 계속 나갑니다
         *
         * 예전에는 UI 의 리셋 메시지가 부르는 coldStartReset() 이 이 일을 했는데,
         * UI 프로토콜을 걷어내면서 호출자가 사라졌습니다. 트리거를 UI 가 아니라
         * "새 경로 적재" 로 옮긴 것이 이 함수입니다.
         *
         * GPS 상태는 지우지 않습니다 — 위치는 계속 흘러 들어오고, 지우면 다음
         * 프레임까지 루프가 멈춥니다. 트래커도 지우지 않습니다 — 호출 직후
         * setTracker() 로 덮어쓰기 때문입니다.
         * └────────────────────────────────────────────────────────────────────┘
         * ┌─ English ──────────────────────────────────────────────────────────┐
         * Clears per-run state just before a new path is installed; called by
         * RddfLoader.
         *
         * Without it, a second RDDF would leave trackCompleteSent_ latched from the
         * previous run (so onComplete() returns early and never broadcasts),
         * continue accumulating elapsed time from that run, and keep the previous
         * command going out to the machine.
         *
         * This work used to live in coldStartReset(), triggered by a UI reset
         * message; removing the UI protocol removed its only caller, so the trigger
         * moved to "a new path was loaded".
         *
         * GPS state is deliberately not cleared — positions keep arriving, and
         * clearing it would stall the loop until the next frame. Neither is the
         * tracker, which setTracker() overwrites immediately after this call.
         * └────────────────────────────────────────────────────────────────────┘
         */
        void prepareForNewTrack();

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 머신 I/O 스냅샷 교환. 배경은 AppMain/MachineIO.hpp 상단을 보세요.
        //   쓰기 setMachineInputs  / 읽기 getMachineInputs   : run() -> 제어 루프
        //   쓰기 setMachineOutputs / 읽기 getMachineOutputs  : 제어 루프 -> run()
        // 두 스레드가 접근하므로 뮤텍스로 보호합니다. 구조체는 작아서 값 복사가
        // 락 구간을 짧게 유지하는 가장 단순한 방법입니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Machine I/O snapshot exchange; see the top of AppMain/MachineIO.hpp.
        // Two threads touch these, so they are mutex-protected. The structs are
        // small, so copying by value is the simplest way to keep the critical
        // section short.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 입력은 scratch 에 모아 두고 한 번에 발행합니다.
        //
        // run() 의 provider 블록 다섯 개가 각자 자기 계통을 읽는데, 그때마다
        // 공유 스냅샷을 갱신하면 TrackingLoop 이 반쯤 갱신된 상태를 볼 수
        // 있습니다 — 새 엔코더값과 지난 주기 IMU 가 섞이면 조향각 융합이
        // 어긋납니다. 그래서 scratch(단일 스레드 전용, 락 없음)에 모으고
        // publishMachineInputs() 로 원자적으로 넘깁니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Inputs accumulate in a scratch copy and are published once.
        //
        // The five provider blocks in run() each read their own subsystem. If each
        // updated the shared snapshot directly, TrackingLoop could observe a
        // half-updated one — a fresh encoder reading paired with last tick's IMU
        // skews the steering-angle fusion. So they fill the scratch (single-thread,
        // no lock) and publishMachineInputs() hands it over atomically.
        // └────────────────────────────────────────────────────────────────────┘
        ::AppMain::MachineInputs& machineInputScratch()
        {
            return machineInputScratch_;
        }
        void publishMachineInputs()
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            machineInputs_ = machineInputScratch_;
        }

        void setMachineInputs(const ::AppMain::MachineInputs &in)
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            machineInputs_ = in;
        }
        ::AppMain::MachineInputs getMachineInputs() const
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            return machineInputs_;
        }
        void setMachineOutputs(const ::AppMain::MachineOutputs &out)
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            machineOutputs_ = out;
        }
        ::AppMain::MachineOutputs getMachineOutputs() const
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            return machineOutputs_;
        }
        /**
         * 지령을 안전 상태로 되돌립니다. 경로가 사라지거나 콜드스타트할 때
         * 마지막 지령이 남아 계속 나가는 것을 막습니다.
         *
         * Resets the command to the safe state so a stale command cannot keep
         * going out after the path is dropped or on a cold start.
         */
        void releaseMachineOutputs()
        {
            std::lock_guard< std::mutex > guard(machineMutex_);
            machineOutputs_ = ::AppMain::MachineOutputs { };
        }

        /**
         * ┌─ 한국어 ───────────────────────────────────────────────────────────┐
         * 정지를 "지령"하는 구간입니다. 트랙터는 AD_Generic_Mode_Cmd 가 1 인
         * 지령만 받아들이므로, 제어권을 놓은 채 내보내는 개도 0·중립은 통째로
         * 버려지고 주행 마지막 개도가 그대로 남습니다. 그래서 Finish 를 눌러도
         * 바퀴가 계속 돌았고, 시뮬레이터의 stopped 는 영영 false 였습니다.
         *
         * 서는 것이 확인될 때까지는 제어권을 쥔 채 0 을 계속 보내고, 선 뒤에
         * 놓습니다. 마지막으로 수락된 지령이 0 이므로 놓아도 그대로 서 있습니다.
         * 대기 중에 늘 제어권을 쥐지는 않습니다 — 그러면 사람이 시뮬레이터에서
         * 트랙터를 직접 몰 수 없습니다.
         * └────────────────────────────────────────────────────────────────────┘
         * ┌─ English ──────────────────────────────────────────────────────────┐
         * A commanded stop. The tractor only accepts commands carrying
         * AD_Generic_Mode_Cmd = 1, so the zero opening and neutral written after
         * authority is released are discarded outright and the run's last
         * opening stays applied. That is why the wheels kept turning after
         * Finish and the simulator's stopped never became true.
         *
         * Authority is held with zero output until standstill is observed, then
         * released — the last accepted command is zero, so the machine stays
         * put. Authority is not held throughout idle, because that would stop a
         * human from driving the tractor in the simulator.
         * └────────────────────────────────────────────────────────────────────┘
         */
        void beginCommandedStop() { commandedStop_ = true; }
        void endCommandedStop() { commandedStop_ = false; }
        bool commandedStopActive() const { return commandedStop_.load(); }

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 여기 있던 것들을 걷어냈습니다.
        //   RunSummary 일체 (onPoseTick / applyProgressMirror / getRunSummary /
        //   serializeRunSummaryJson / persistRunSummaryToDisk / loadRunSummaryByRun)
        //     — 해커톤 채점과 리더보드 전송을 위한 집계였습니다.
        //   setOutsideField / isOutsideField
        //     — UI의 update_transform 이 알려주던 농지 이탈 여부. 경과시간에
        //       10배 페널티를 주는 데 썼습니다.
        //   setMapStartPosition / getMapStartPosition / clearMapStartPositions
        //     — 외부 서버 맵 리스트가 준 맵별 시작 좌표. 현재 정적 RDDF loader에는
        //       이 실행 시점 데이터가 없고 simulator/session start gate가 검사합니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Removed from here: the whole RunSummary accumulator (hackathon scoring
        // and leaderboard uploads), the outside-field flag the UI used to supply
        // for the 10x elapsed-time penalty, and the per-map start positions that
        // came from the external map list. The simulator/session start gate now
        // owns live pose proximity; the static RDDF loader does not fabricate it.
        // └────────────────────────────────────────────────────────────────────┘

            private:
        // resetGpsState() 와 resetTrackState() 가 여기 있었습니다. 둘 다
        // coldStartReset() 만이 호출자였고, 그 함수는 UI 리셋 메시지가 사라지면서
        // 호출자를 잃었습니다. GPS 는 계속 흘러 들어오므로 지울 이유가 없고,
        // 트래커는 새 경로를 적재할 때 setTracker() 가 덮어씁니다.
        //
        // resetGpsState() and resetTrackState() were here. coldStartReset() was
        // their only caller, and it lost its own caller when the UI reset message
        // went away. GPS keeps streaming so there is nothing to clear, and the
        // tracker is overwritten by setTracker() when a new path loads.

        std::shared_ptr< tracking::IPathTracker > tracker_;
        std::shared_ptr< tracking::TrackingLoop > trackingLoop_;
        std::vector< ::AppMain::RddfWaypoint > rawWaypoints_;

        // ── 런타임 튜닝 상태 / Runtime tuning state ─────────────────────────
        // WS 스레드와 TrackingLoop 스레드가 공유합니다. machineMutex_ 와 분리한
        // 이유: 웨이포인트 벡터 복사가 머신 I/O 스냅샷 교환을 막지 않게 하기
        // 위해서입니다.
        // Shared between the WS and TrackingLoop threads. Separate from
        // machineMutex_ so copying the waypoint vector never blocks the
        // machine I/O snapshot exchange.
        mutable std::mutex tuningMutex_;
        tracking::GpsSampleStore gpsSamples_;
        std::atomic<double> machineTimeScale_{1.0};
        // 주행 상태. 기동 직후는 Idle — RDDF 가 도착해도 시작 지시 없이는
        // 움직이지 않습니다. / Idle at boot: an arriving RDDF arms but never moves.
        ::AppMain::RunState runState_ = ::AppMain::RunState::Idle;
        ::AppMain::tracking::TrackerParams trackerParams_;
        ::AppMain::tracking::VehicleGeometry runGeometry_;
        std::string trackerName_ = ::AppMain::tracking::defaultTrackerName();
        std::vector< ::AppMain::tracking::TrackerWaypoint > trackedWaypoints_;
        double trackOriginE_ = 0.0;
        double trackOriginN_ = 0.0;
        bool trackOriginValid_ = false;
        double trackOriginLat_ = 0.0;
        double trackOriginLon_ = 0.0;
        bool trackOriginGpsValid_ = false;
        bool trackCompleteSent_ = false;
        bool runGeometryFrozen_ = false;

        // 머신 I/O 스냅샷. mutable 은 const getter 안에서 잠그기 위한 것입니다.
        // Machine I/O snapshots; mutable so the const getters can lock.
        mutable std::mutex machineMutex_;
        ::AppMain::MachineInputs machineInputs_;
        ::AppMain::MachineOutputs machineOutputs_;
        // 정지 지령이 아직 걸려 있는가. 제어 루프와 오케스트레이터가 서로 다른
        // 스레드에서 읽고 쓰므로 원자적으로 둡니다.
        // Whether a commanded stop is still in force; the control loop and the
        // orchestrator touch it from different threads.
        std::atomic< bool > commandedStop_ { false };
        // ProcessTimer 스레드 전용. 락이 필요 없습니다.
        // ProcessTimer thread only; needs no lock.
        ::AppMain::MachineInputs machineInputScratch_;

        // The generated SDK marks a numeric zero encoder payload invalid, even
        // though its interface timestamp advances on every received CAN frame.
        // Track timestamp freshness so zero remains a valid physical angle, but
        // expire it promptly if encoder frames stop arriving.
        ::AppMain::tracking::SignalFreshness steerEncoderFreshness_;
        ::AppMain::tracking::SignalFreshness gyroFreshness_;

        mutable ::AppMain::orchestration::LaunchLifecycle orchestrationLifecycle_;
        mutable std::mutex orchestrationMutex_;
        std::shared_ptr< ::AppMain::orchestration::ISimulatorApi > simulatorApi_;
        std::shared_ptr< ::AppMain::orchestration::IRecordingStore > recordingStore_;
        std::shared_ptr< ::AppMain::orchestration::IRunControl > runControlAdapter_;
        std::shared_ptr< ::AppMain::orchestration::RunOrchestrator > orchestrator_;
        std::atomic<bool> orchestrationTaskActive_{false};
        std::future<void> orchestrationWorker_;
        std::atomic<bool> orchestrationFinishRequested_{false};
        std::atomic<bool> orchestrationShutdown_{false};
        std::atomic<unsigned long long> orchestrationSetupGeneration_{0};
        std::string browserSealedCapturedRunId_;
        // Refresh is a pure read of simulator state, so a second queued refresh
        // can never observe anything the first one will not. The UI polls it,
        // and each refresh costs four simulator round trips — without this
        // coalescing latch a slow simulator grows the queue without bound.
        bool orchestrationRefreshQueued_ = false;
        // 큐에 떠 있는 *변경성* 작업 수. Start 승인은 이것만 봅니다.
        //
        // 이전에는 큐가 비어 있어야만 Start 를 받았는데, UI 가 2500ms 마다 보내는
        // 폴링이 매번 refresh 를 큐에 넣으므로 refresh 가 도는 동안 누른 Start 는
        // 무조건 거부됐습니다 — 버튼이 동전 던지기가 됩니다. refresh 는 읽기
        // 전용이고 큐는 워커 하나가 순차 처리하므로, Start 는 refresh 뒤에 줄을
        // 서면 그만입니다. 막아야 하는 건 Start/Reset/Finish 끼리의 충돌뿐입니다.
        //
        // Count of *mutating* tasks queued or running; Start admission consults
        // only this. Previously Start required a completely empty queue, but the
        // UI's 2500 ms poll enqueues a refresh every time, so a Start pressed
        // while one was in flight was always rejected — the button became a coin
        // flip. A refresh is read-only and the queue drains serially on one
        // worker, so Start can simply queue behind it; only Start/Reset/Finish
        // may not overlap each other.
        int orchestrationMutatingPending_ = 0;
        struct OrchestrationTask {
            std::function<void(::AppMain::orchestration::RunOrchestrator&)> work;
            // Default-initialised: the worker declares one before popping, and a
            // stray true would leak a permanent Start block on an empty queue.
            bool mutating = false;
        };
        std::deque<OrchestrationTask> orchestrationTasks_;
        void ensureOrchestrator();
        enum class OrchestrationAdmission { Refresh, Queue, Start, Finish };
        bool launchOrchestration(
            std::function<void(::AppMain::orchestration::RunOrchestrator&)> work,
            OrchestrationAdmission admission=OrchestrationAdmission::Queue);

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 조향 모터 Enable 래치. 지령이 0이 아닐 때 한 번만 Enable 을 보내기
        // 위한 것입니다. 매 주기 Enable 을 보내는 구현도 있지만(테스트 앱),
        // 참조 구현(agmo_solution)은 래치를 씁니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Steering motor Enable latch, so Enable is sent once when the command
        // becomes non-zero. Some implementations resend it every tick; the
        // reference implementation latches.
        // └────────────────────────────────────────────────────────────────────┘
        bool steerMotorEnabled_ = false;
        // 재사용하는 CAN 프레임 객체. 매 주기 new 하지 않기 위한 것입니다.
        // Reused CAN frame object, so a new one is not allocated every tick.
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr steerFrame_;
        /*PROTECTED REGION END*/

    protected:
        virtual void start(const int intervalInMilliSecond) override;
        void run();
    public:
        void setWebSocketEndPoint(
                std::shared_ptr< web::WebSocketEndPoint > &webSocketEndPoint);
        std::shared_ptr< web::WebSocketEndPoint > getWebSocketEndPoint() const;
    private:
        std::shared_ptr< web::WebSocketEndPoint > webSocketEndPoint;

    protected:
        MainController_ptr _this()
        {
            return MainController_ptr(this);
        }

    private:
        // Attributes

        // References

        /*PROTECTED REGION ID(MainController_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_MAINCONTROLLER_HPP
