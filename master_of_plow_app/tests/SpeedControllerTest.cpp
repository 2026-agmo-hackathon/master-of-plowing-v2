// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
//
// SpeedController가 고쳤던 버그를 실제로 잡아내는 회귀 테스트입니다.
//
// 예전 코드는 불감대 안에서도, 초과속 분기에서도 throttle을 강제로 0으로
// 만들었습니다. 그러면 정상상태에서 열려 있는 개도가 없어져 스로틀이
// 100%(가속) 아니면 0%(브레이크) 사이만 오가는 bang-bang 릴레이가 되고,
// 가속<->정지를 반복하는 리밋 사이클이 됩니다. 지금은 단일 연속 PID
// 출력이므로 정상상태에서 throttle이 0보다 큰 값을 계속 유지합니다.
//
// PathTrackerContractTest.cpp의 차량 모델(코스팅 감속 0.2 m/s^2)은 이 버그를
// 잡지 못한다는 것을 실측으로 확인했습니다: 그 모델에서는 pre-fix 코드도 5개
// 테스트를 전부 통과합니다. 예전 P 제어기가 그 완만한 감속에서 0.049 m/s
// 흔들림 정도로만 정착해 버리기 때문입니다. 그래서 여기서는 실기의 로터리
// 하강(견인부하 + 엔진 브레이크) 조건을 대표하는 DRAG = 1.0 m/s^2를 씁니다.
//
// 두 버전은 처음 약 55초 동안(둘 다 아직 가속 중이라 오차가 불감대 밖에
// 있는 동안) 완전히 동일하게 움직입니다. 차이는 차량이 처음 불감대에
// 들어온 뒤에야 드러납니다. 그래서 180초를 돌리고, 마지막 60초(마지막
// 600틱)만 "정착된 구간"으로 판정합니다 — 짧게 돌리면 두 버전을 구분하지
// 못합니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
//
// ┌─ English ──────────────────────────────────────────────────────────────────┐
//
// Regression tests that actually catch the bug SpeedController fixed.
//
// The old code forced throttle to 0 both inside the deadband and on the
// overspeed branch, leaving no steady-state opening at all: throttle only
// ever sat at 100% (accelerate) or 0% (brake) — a bang-bang relay that
// repeats an accelerate/stop limit cycle. The fixed controller emits a single
// continuous PID output, so throttle stays above 0 once settled.
//
// The vehicle model in PathTrackerContractTest.cpp (0.2 m/s^2 coast
// deceleration) was verified by exact simulation NOT to expose this bug: the
// pre-fix code passes all 5 tests under that model, because the old
// proportional controller settles there with only a 0.049 m/s wobble. So
// this file uses DRAG = 1.0 m/s^2 instead, representing the plough-down
// condition on the real machine (draft load + engine braking).
//
// Both versions behave identically for roughly the first 55 s, while the
// error is still outside the deadband and both are accelerating. The
// divergence only appears after the vehicle first reaches the deadband, so
// the run must be 180 s with the settled window being the last 60 s (last
// 600 ticks) — a short run will not discriminate.
//
// └────────────────────────────────────────────────────────────────────────────┘

#include <gtest/gtest.h>

#include <AppMain/tracking/SpeedController.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

using namespace AppMain::tracking;

namespace {

constexpr double DT = 0.1;
constexpr double DRAG = 1.0;        // m/s^2, plough-down draft load + rolling resistance
constexpr int TOTAL_TICKS = 1800;   // 180 s at DT = 0.1
constexpr int SETTLED_TICKS = 600;  // last 60 s of the run

double step(double speed, double throttle, double brake)
{
    speed += (3.0 * throttle - 4.0 * brake - DRAG) * DT;
    return std::max(0.0, std::min(4.0, speed));
}

struct SettledStats
{
    double finalSpeed = 0.0;
    double minSpeed = std::numeric_limits<double>::infinity();
    double maxSpeed = -std::numeric_limits<double>::infinity();
    double minThrottle = std::numeric_limits<double>::infinity();
};

SettledStats runAndCollectSettledStats(double targetKmh)
{
    SpeedController ctrl;
    double speed = 0.0;
    SettledStats stats;
    for (int i = 0; i < TOTAL_TICKS; ++i) {
        const SpeedController::Output out = ctrl.update(targetKmh, speed, DT);
        speed = step(speed, out.throttle, out.brake);
        if (i >= TOTAL_TICKS - SETTLED_TICKS) {
            stats.minSpeed = std::min(stats.minSpeed, speed);
            stats.maxSpeed = std::max(stats.maxSpeed, speed);
            stats.minThrottle = std::min(stats.minThrottle, out.throttle);
        }
    }
    stats.finalSpeed = speed;
    return stats;
}

} // namespace

// 목표속도는 기계 하한(MIN_MACHINE_SPEED_KMH = 2.05) 위에서 골랐습니다. 그 아래는
// raiseToMachineFloor 가 끌어올리므로 PID 수렴이 아니라 클램프를 시험하게 됩니다.
// Targets sit above the machine floor (MIN_MACHINE_SPEED_KMH = 2.05); below it
// raiseToMachineFloor takes over and these would test the clamp, not the PID.

TEST(SpeedControllerTest, ConvergesToTargetWithin180sAndHoldsIt) {
    const SettledStats stats = runAndCollectSettledStats(4.0);
    EXPECT_NEAR(stats.finalSpeed, 4.0 / 3.6, 0.05);
}

TEST(SpeedControllerTest, SelectedTransmissionMaximumIsRespected)
{
    SpeedController controller;
    VehicleGeometry geometry;
    geometry.maxForwardGear=1;
    controller.setVehicleGeometry(geometry);
    for (int i=0;i<30;++i) {
        EXPECT_EQ(controller.update(7.0,0.5,DT).gear,1);
    }
}

TEST(SpeedControllerTest, HoldsNonZeroThrottleAtSteadyState) {
    // This is the test that directly captures the old bug: the pre-fix
    // controller forced throttle to 0 both inside the deadband and on the
    // overspeed branch, so it could never hold a steady-state opening.
    const SettledStats stats = runAndCollectSettledStats(4.0);
    EXPECT_GT(stats.minThrottle, 0.0);
}

TEST(SpeedControllerTest, DoesNotOscillateOnceSettled) {
    const SettledStats stats = runAndCollectSettledStats(4.0);
    EXPECT_LT(stats.maxSpeed - stats.minSpeed, 0.02);
}

// The following two pass against BOTH the pre-fix and post-fix controller —
// they guard behaviour the bug never touched, not the bug itself.

TEST(SpeedControllerTest, CommandsBrakeWhenGenuinelyOverspeed) {
    SpeedController ctrl;
    // Target 4.0 km/h (~1.111 m/s), current 3.0 m/s.
    const SpeedController::Output out = ctrl.update(4.0, 3.0, DT);
    EXPECT_GT(out.brake, 0.0);
    EXPECT_EQ(out.throttle, 0.0);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 수정 전 mux에서 이 기계의 지면속도는 약 1Hz였고 제어 루프는 10Hz(preferredPeriodMs)
// 입니다. 같은 샘플을 PID가 10번 소비하며 매 틱 적분하면 실효 적분 게인이
// 10배가 되므로, 새 측정값이 왔을 때만 한 스텝 전진해야 합니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Before the mux correction, ground speed arrived at roughly 1 Hz while the
// control loop ran at 10 Hz (preferredPeriodMs). Consuming the same sample ten times and
// integrating on every tick makes the effective integral gain 10x, so the PID
// must advance one step per genuine measurement.
// └────────────────────────────────────────────────────────────────────────────┘

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 이 기계에는 비례 브레이크가 없습니다. TrackingLoop::act() 가 brake > 0 을
// "ACC 개도 0 + 다운시프트"로 바꾸므로, 0.13 이든 0.23 이든 결과는 완전 정지입니다
// (쟁기 부하 때문에 1.5~2초). 그래서 4.0 -> 2.4 km/h 같은 통상 감속에 브레이크를
// 걸면 안 됩니다 — 감속은 기어가 이미 해내고 있습니다.
//
// 에뮬레이터에서 이 지점마다 정지가 재현됐고, 그것이 사용자가 보고한
// "선회할 때 간헐적으로 멈추는 현상"이었습니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// There is no proportional brake. TrackingLoop::act() turns any brake > 0 into
// "opening 0 plus a downshift", so 0.13 and 0.23 alike mean a full stop within
// 1.5-2 s under plough draft. A routine 4.0 -> 2.4 km/h step must therefore not
// brake; the gear already delivers that reduction. This reproduced at every such
// step on the emulator and was the reported "intermittent stopping when turning".
// └────────────────────────────────────────────────────────────────────────────┘
TEST(SpeedControllerTest, RoutineSlowdownCoastsInsteadOfBraking)
{
    SpeedController ctrl;
    // 4.0 -> 2.4 km/h 목표 하강 직후의 실제 상태: 목표 0.667 m/s, 실제 1.08 m/s.
    // The state right after a 4.0 -> 2.4 km/h step: target 0.667, actual 1.08 m/s.
    const SpeedController::Output out = ctrl.update(2.4, 1.08, DT);
    EXPECT_DOUBLE_EQ(out.brake, 0.0)
        << "braking for a routine slowdown stops this machine outright";
    EXPECT_GT(out.throttle, 0.0)
        << "a zero opening is a stop, not a slowdown";
}

// 위와 반대: 기어로는 못 잡는 진짜 폭주에는 브레이크가 나와야 합니다. 두 테스트가
// 함께 있어야 문턱이 어느 쪽으로도 무너지지 않습니다.
// The converse: a runaway a downshift cannot catch must still brake. Both tests
// together pin the threshold from either side.
TEST(SpeedControllerTest, RunawayBeyondOneGearStepStillBrakes)
{
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(4.0, 3.0, DT);
    EXPECT_GT(out.brake, 0.0);
}

TEST(SpeedControllerTest, HoldsCommandBetweenMeasurementUpdates) {
    // GPS 샘플 하나를 10번 소비하며 매 틱 적분하면 실효 적분 게인이 10배가 됩니다.
    // 새 정보가 없으면 지령도 그대로여야 합니다(영차 홀드).
    //
    // Consuming one GPS sample ten times while integrating on every tick makes the
    // effective integral gain 10x. With no new information the command must not
    // change (zero-order hold).
    SpeedController ctrl;
    const SpeedController::Output first = ctrl.update(4.0, 0.5, DT);

    for (int i = 0; i < 8; ++i) {  // 0.8 s, below MAX_MEAS_HOLD_S
        const SpeedController::Output held = ctrl.update(4.0, 0.5, DT);
        EXPECT_DOUBLE_EQ(held.throttle, first.throttle);
        EXPECT_DOUBLE_EQ(held.brake, first.brake);
    }

    // 새 측정값이 들어오면 반응해야 합니다. / A genuine new sample is acted on.
    const SpeedController::Output fresh = ctrl.update(4.0, 0.9, DT);
    EXPECT_NE(fresh.throttle, first.throttle);
}

TEST(SpeedControllerTest, GearDoesNotHuntOnAChatteringTarget) {
    // 실측 RDDF의 속도 열은 두렁 경계에서 매초 4.0 <-> 1.2 로 뒤집힙니다. 그 값이
    // gearForSpeed 의 3.0km/h 임계값을 넘나들면 변속기가 매초 단을 바꿉니다 —
    // 에뮬레이터에서 기어 변경 40회가 목표 플립 40회와 1:1로 일치했습니다.
    //
    // A real RDDF speed column flips 4.0 <-> 1.2 every second at a headland
    // boundary. Straddling the 3.0 km/h threshold in gearForSpeed makes the
    // transmission change stage every second — in the emulator 40 gear changes
    // matched 40 target flips one for one.
    SpeedController ctrl;
    int prev = ctrl.update(4.0, 1.0, DT).gear;
    int changes = 0;

    for (int i = 0; i < 200; ++i) {  // 20 s, target flipping once per second
        const double targetKmh = ((i / 10) % 2 == 0) ? 4.0 : 1.2;
        const int gear = ctrl.update(targetKmh, 1.0, DT).gear;
        if (gear != prev) {
            ++changes;
        }
        prev = gear;
    }

    EXPECT_LE(changes, 2);
}

TEST(SpeedControllerTest, SustainedGearChangeIsStillAdopted) {
    // 히스테리시스가 변속을 막아버리면 안 됩니다. 목표가 계속 유지되면 바뀌어야
    // 합니다. / Hysteresis must not block shifting outright: a target that holds
    // has to be adopted.
    SpeedController ctrl;
    const int fast = ctrl.update(4.0, 1.0, DT).gear;
    int slow = fast;
    for (int i = 0; i < 60; ++i) {  // 6 s, comfortably past GEAR_DWELL_S
        slow = ctrl.update(1.2, 0.3, DT).gear;
    }
    EXPECT_NE(slow, fast);
}

TEST(SpeedControllerTest, ReversalLatchesFullBrakeUntilStopped) {
    SpeedController ctrl;
    double speed = 1.0;  // rolling forward

    SpeedController::Output out = ctrl.update(2.0, speed, DT);
    ASSERT_FALSE(out.brakingForReversal);

    // Sign flip while still rolling forward: latch full brake.
    out = ctrl.update(-2.0, speed, DT);
    EXPECT_TRUE(out.brakingForReversal);
    EXPECT_EQ(out.throttle, 0.0);
    EXPECT_EQ(out.brake, 1.0);

    // Stays latched while coasting down toward a stop.
    while (speed > 0.05) {
        speed = step(speed, out.throttle, out.brake);
        out = ctrl.update(-2.0, speed, DT);
        if (speed > 0.05) {
            EXPECT_TRUE(out.brakingForReversal);
            EXPECT_EQ(out.brake, 1.0);
        }
    }

    EXPECT_LE(speed, 0.05);
    EXPECT_FALSE(out.brakingForReversal);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 기계 하한 아래 목표속도. 참가자가 RDDF를 직접 만들어 제출하므로 이건 예외가
// 아니라 정상 입력입니다. 낼 수 없는 속도를 그대로 추종하려 들면 크리프 하한과
// 정지를 오가며 평균만 맞추게 되고, 그게 "전진했다 멈췄다"의 정체입니다.
// 하한으로 올려 연속 주행하는 쪽을 택했고, 못 지킨 사실은 텔레메트리로 알립니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Below-floor target speeds. Participants author their own RDDFs, so this is a
// normal input rather than an exceptional one. Chasing an unreachable speed makes
// the machine surge between the creep floor and a standstill to average it out —
// that is what "drives, stops, drives, stops" actually is. The controller raises
// it to the floor and keeps moving, and says so through telemetry.
// └────────────────────────────────────────────────────────────────────────────┘

TEST(SpeedControllerTest, TargetBelowMachineFloorIsRaisedAndFlagged) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(1.2, 0.5, DT);

    EXPECT_TRUE(out.targetRaisedToFloor);
    EXPECT_NEAR(out.targetMps, SpeedController::MIN_MACHINE_SPEED_KMH / 3.6, 1e-9);
    EXPECT_NEAR(out.requestedMps, 1.2 / 3.6, 1e-9);
}

TEST(SpeedControllerTest, ReachableTargetIsLeftAlone) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(4.0, 1.0, DT);

    EXPECT_FALSE(out.targetRaisedToFloor);
    EXPECT_NEAR(out.targetMps, 4.0 / 3.6, 1e-9);
    EXPECT_NEAR(out.requestedMps, 4.0 / 3.6, 1e-9);
}

TEST(SpeedControllerTest, ZeroTargetStillMeansStop) {
    // 0은 "낼 수 없는 느린 속도"가 아니라 "여기서 정지"입니다. 하한으로 끌어올리면
    // 정지해야 할 곳에서 기어 나갑니다.
    // Zero means "stop here", not "a speed too slow to hold": raising it to the
    // floor would make the machine creep away from a point it must stop at.
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(0.0, 0.0, DT);

    EXPECT_FALSE(out.targetRaisedToFloor);
    EXPECT_EQ(out.targetMps, 0.0);
}

TEST(SpeedControllerTest, BelowFloorReverseKeepsItsDirection) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(-1.0, 0.0, DT);

    EXPECT_TRUE(out.targetRaisedToFloor);
    EXPECT_NEAR(out.targetMps, SpeedController::MIN_MACHINE_SPEED_KMH / 3.6, 1e-9);
    EXPECT_EQ(out.gear, -1);
}

TEST(SpeedControllerTest, BelowFloorTargetIsHeldContinuouslyNotPulsed) {
    // 클램프의 존재 이유입니다. 하한 아래를 그대로 좇으면 스로틀이 0까지 떨어지는
    // 구간이 생깁니다(= 정지). 클램프가 있으면 계속 굴러가야 합니다.
    // This is why the clamp exists. Chasing a below-floor target drives the
    // throttle to zero at some point (= a stop); with the clamp it keeps rolling.
    SpeedController ctrl;
    double speed = 0.0;
    double minSettledThrottle = 1.0;

    for (int i = 0; i < TOTAL_TICKS; ++i) {
        const SpeedController::Output out = ctrl.update(1.2, speed, DT);
        speed = step(speed, out.throttle, out.brake);
        if (i >= TOTAL_TICKS - SETTLED_TICKS) {
            minSettledThrottle = std::min(minSettledThrottle, out.throttle);
        }
    }

    EXPECT_NEAR(speed, SpeedController::MIN_MACHINE_SPEED_KMH / 3.6, 0.05);
    EXPECT_GT(minSettledThrottle, 0.0);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 앞먹임이 없으면 정상상태 개도를 적분만으로 만들어야 합니다. 실측상 이 기계는
// 어떤 속도든 0.45 정도를 열어야 굴러가는데, 목표 근처에서 오차는 0.03 m/s 라
// 적분은 0.0015/s 로 자랍니다 — 0.45 를 쌓는 데 5분입니다. 그동안은 목표에
// 닿을 때마다 개도가 무너지고 부하에 눌려 정지했다가 다시 튀어 오릅니다.
// 에뮬레이터에서 실제로 4초 주기로 갔다 섰다 했습니다.
//
// 아래 플랜트는 그 상황을 재현합니다: 부하(DRAFT)를 이기려면 개도가 THR_HOLD
// 이상이어야 하고, 그 아래면 감속합니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Without feedforward the steady-state opening has to come from the integral
// alone. Measured, this machine needs ~0.45 open to move at any speed, but near
// the target the error is 0.03 m/s so the integral grows at 0.0015/s — five
// minutes to build 0.45. Until then the opening collapses every time the speed
// reaches target, the load stops the machine, and it surges back: a four-second
// drive/stop limit cycle, exactly as seen on the emulator.
//
// The plant below reproduces that: an opening under THR_HOLD cannot hold speed.
// └────────────────────────────────────────────────────────────────────────────┘

TEST(SpeedControllerTest, ReachesAndHoldsSpeedAgainstADraftLoadWithoutStalling) {
    constexpr double THR_HOLD = 0.45;   // 실측 정상상태 개도 / measured steady-state opening
    constexpr double TARGET_KMH = 4.0;
    constexpr double TARGET_MPS = TARGET_KMH / 3.6;

    SpeedController ctrl;
    double speed = 0.0;      // 실제 속도 / true speed
    double reported = 0.0;   // 제어기가 보는 값 / what the controller sees
    int settledStalls = 0;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // ★ 두 가지를 반드시 재현해야 이 버그가 나타납니다.
    //   (1) 수정 전 GPS 1Hz — 제어는 10Hz 인데 속도 측정은 초당 한 번뿐입니다.
    //   (2) 속도 양자화 — 이 기계가 보고하는 속도는 0.00 / 0.57 / 0.62 / 1.03 /
    //       1.08 m/s 뿐입니다(GPS 샘플 3,865개 실측). 목표 1.111 m/s 는 그 사이에
    //       없으므로 오차가 0.03 m/s 에 "고정"되고, 적분은 ki*0.03 = 0.0015/s 로
    //       기어갑니다. 연속 속도로 모델링하면 적분이 금방 자라서 버그가 사라집니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Two properties are required for this bug to appear at all:
    //   (1) pre-fix 1 Hz GPS — control ticks at 10 Hz but speed arrives once per second;
    //   (2) speed quantisation — this machine only ever reports 0.00 / 0.57 /
    //       0.62 / 1.03 / 1.08 m/s (measured over 3,865 GPS samples). The target
    //       1.111 m/s falls between levels, so the error PINS at 0.03 m/s and the
    //       integral crawls at ki*0.03 = 0.0015/s. Model the speed as continuous
    //       and the integral grows quickly enough to hide the bug.
    // └────────────────────────────────────────────────────────────────────────┘
    const double levels[] = {0.0, 0.57, 0.62, 1.03, 1.08};
    auto quantise = [&levels](double v) {
        double best = levels[0];
        for (double l : levels) {
            if (std::fabs(l - v) < std::fabs(best - v)) { best = l; }
        }
        return best;
    };

    for (int i = 0; i < 600; ++i) {  // 60 s
        if (i % 10 == 0) { reported = quantise(speed); }   // pre-fix 1 Hz stress case
        const SpeedController::Output out = ctrl.update(TARGET_KMH, reported, DT);
        // 개도가 THR_HOLD 를 넘는 만큼만 가속, 모자라면 그만큼 감속.
        // Accelerate on the excess over THR_HOLD, decelerate on the shortfall.
        speed += (3.0 * (out.throttle - THR_HOLD) - 4.0 * out.brake) * DT;
        speed = std::max(0.0, std::min(4.0, speed));
        if (i >= 300 && speed < 0.05) { ++settledStalls; }
    }

    EXPECT_GT(speed, 0.9 * quantise(TARGET_MPS))
            << "final speed " << speed << " m/s vs target " << TARGET_MPS;
    EXPECT_EQ(settledStalls, 0)
            << "came to a standstill on " << settledStalls
            << " ticks in the last 30 s while a positive speed was commanded — "
               "the drive/stop limit cycle is back";
}

TEST(SpeedControllerTest, FeedforwardCarriesTheSteadyStateOpening) {
    // 첫 지령부터 부하를 이길 만큼 열려 있어야 합니다. 적분이 자라기를
    // 기다리면 그 사이에 멈춥니다.
    // The very first command must already clear the load; waiting for the
    // integral to grow means stalling in the meantime.
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(4.0, 0.0, DT);
    EXPECT_GT(out.ffTerm, 0.40);
    EXPECT_LT(out.ffTerm, 0.60);
}

TEST(SpeedControllerTest, StopCommandGetsNoFeedforward) {
    // 목표 0은 "정지"입니다. 앞먹임이 붙으면 세워야 할 곳에서 기어 나갑니다.
    // A zero target means stop; a feedforward opening would creep away from it.
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(0.0, 0.0, DT);
    EXPECT_EQ(out.ffTerm, 0.0);
    EXPECT_EQ(out.throttle, 0.0);
}

TEST(SpeedControllerTest, IdenticalFreshPoseSamplesAdvanceExactlyOnce)
{
    SpeedController ctrl;
    const auto first = ctrl.update(4.0, 1.0, 0.1, 10);
    const auto held = ctrl.update(4.0, 1.0, 0.1, 10);
    EXPECT_DOUBLE_EQ(held.throttle, first.throttle);
    const auto freshSameValue = ctrl.update(4.0, 1.0, 0.1, 11);
    EXPECT_GT(freshSameValue.iTerm, held.iTerm)
        << "sample identity, not floating-value inequality, advances the PID";
    const auto heldAgain = ctrl.update(4.0, 1.0, 0.1, 11);
    EXPECT_DOUBLE_EQ(heldAgain.throttle, freshSameValue.throttle);
}
