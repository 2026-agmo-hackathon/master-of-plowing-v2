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

TEST(SpeedControllerTest, GenuineOverspeedCutsTheOpeningToZero) {
    // 개도가 릴레이인 기계에서 초과속을 되돌리는 방법은 개도를 0으로 두는 것
    // 하나뿐입니다. 예전 코드는 여기서 개도를 0.47 로 "낮췄는데", 이 기계에서
    // 0.47 은 100% 와 완전히 같은 지령이라 감속이 전혀 일어나지 않았습니다.
    //
    // On a machine whose opening is a relay, zero is the only command that undoes
    // an overspeed. The old code "reduced" the opening to 0.47 here — on this
    // machine 0.47 is the same command as 100%, so nothing decelerated.
    SpeedController ctrl;
    // Target 4.0 km/h (~1.111 m/s), current 3.0 m/s.
    const SpeedController::Output out = ctrl.update(4.0, 3.0, DT);
    EXPECT_EQ(out.throttle, 0.0);
    EXPECT_TRUE(out.throttleGated);
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
        << "braking for a routine slowdown downshifts and stops this machine";
    EXPECT_EQ(out.throttle, 0.0)
        << "coasting is the whole of the deceleration this machine has";
}

// 위와 반대 방향의 구속: 0 이 아닌 속도가 지시된 동안에는 아무리 초과속이어도
// 브레이크를 내보내면 안 됩니다. TrackingLoop 이 brake > 0 을 다운시프트로도
// 읽기 때문에, 목표를 낼 수 있는 유일한 단을 스스로 버리게 됩니다.
// The constraint from the other side: however large the overspeed, a non-zero
// commanded speed must never emit a brake. TrackingLoop also reads brake > 0 as a
// downshift, which throws away the only stage that can make the target.
TEST(SpeedControllerTest, OverspeedNeverBrakesWhileMovingIsCommanded)
{
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(4.0, 3.0, DT);
    EXPECT_DOUBLE_EQ(out.brake, 0.0);
}

TEST(SpeedControllerTest, ZeroTargetStillBrakes)
{
    // 브레이크가 남아 있어야 하는 유일한 경우입니다. 목표 0 은 감속이 아니라
    // 정지이고, 거기서는 다운시프트도 옳습니다.
    // The one case the brake is still for: a zero target is a stop, not a
    // slowdown, and a downshift is right there.
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(0.0, 1.0, DT);
    EXPECT_GT(out.brake, 0.0);
    EXPECT_EQ(out.throttle, 0.0);
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
// 아니라 정상 입력입니다.
//
// 예전에는 목표를 하한으로 올려 버렸습니다. 그러면 두렁에서 요청보다 빠르게
// 달리고(compact 는 1.20 요청에 2.78 로 달렸습니다 — 132% 초과), 무엇보다 RDDF에
// 적힌 숫자가 트랙터 조합에 따라 다른 뜻이 됩니다.
//
// 지금은 올리지 않습니다. 가장 낮은 단을 물고 개도를 끊었다 열었다 해서 평균이
// 정확히 요청 속도가 되게 합니다. 요청이 가장 느린 단보다 느리다는 사실은
// belowSlowestStage 로 알리되, 목표 자체는 건드리지 않습니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Below-floor target speeds. Participants author their own RDDFs, so this is a
// normal input rather than an exceptional one.
//
// This used to raise the target to the floor. That overspeeds the headland — the
// compact ran 2.78 against a 1.20 request, 132% over — and, worse, makes the
// number in the RDDF mean something different on each tractor.
//
// It is no longer raised. The slowest stage is engaged and the opening is pulsed
// so the mean is exactly the request. That the request is below the slowest
// stage is reported through belowSlowestStage; the target itself is untouched.
// └────────────────────────────────────────────────────────────────────────────┘

TEST(SpeedControllerTest, TargetBelowSlowestStageIsFlaggedButNotChanged) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(1.2, 0.5, DT);

    EXPECT_TRUE(out.belowSlowestStage);
    EXPECT_NEAR(out.targetMps, 1.2 / 3.6, 1e-9)
        << "the request is followed as written, not raised";
    EXPECT_NEAR(out.requestedMps, 1.2 / 3.6, 1e-9);
}

TEST(SpeedControllerTest, ReachableTargetIsLeftAlone) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(4.0, 1.0, DT);

    EXPECT_FALSE(out.belowSlowestStage);
    EXPECT_NEAR(out.targetMps, 4.0 / 3.6, 1e-9);
    EXPECT_NEAR(out.requestedMps, 4.0 / 3.6, 1e-9);
}

TEST(SpeedControllerTest, ZeroTargetStillMeansStop) {
    // 0은 "낼 수 없는 느린 속도"가 아니라 "여기서 정지"입니다.
    // Zero means "stop here", not "a speed too slow to hold".
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(0.0, 0.0, DT);

    EXPECT_FALSE(out.belowSlowestStage);
    EXPECT_EQ(out.targetMps, 0.0);
}

TEST(SpeedControllerTest, BelowFloorReverseKeepsItsDirection) {
    SpeedController ctrl;
    const SpeedController::Output out = ctrl.update(-1.0, 0.0, DT);

    EXPECT_TRUE(out.belowSlowestStage);
    EXPECT_NEAR(out.targetMps, 1.0 / 3.6, 1e-9);
    EXPECT_EQ(out.gear, -1);
}

TEST(SpeedControllerTest, BelowFloorTargetIsHeldContinuouslyOnAProportionalMachine) {
    // 개도에 진짜 권한이 있는 기계(위쪽 DRAG 플랜트)에서는 변조기가 일할 일이
    // 없습니다. PID 가 1.2 km/h 를 그대로 잡아내고, 개도는 끊기지 않습니다.
    // On a machine where the opening has authority (the DRAG plant above) the
    // modulator has nothing to do: the PID holds 1.2 km/h and the opening is
    // never cut.
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

    EXPECT_NEAR(speed, 1.2 / 3.6, 0.05);
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

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
//
// 단 선택 폐루프.
//
// 아래 플랜트는 에뮬레이터 실측을 그대로 옮긴 것입니다. 같은 경로를 조합을
// 바꿔가며 네 번 주행하고 10Hz 텔레메트리를 받아 보면, 한 단 안에서는 개도를
// 얼마로 주든 정상상태 속도가 같습니다(개도 1.5V 와 4.0V 가 같은 km/h). 속도를
// 정하는 것은 단이고, 단별 속도는 트랙터/작업기 조합마다 다릅니다.
//
// 여기 쓰는 값은 large 4WD + 3.6m 쟁기의 실측치입니다: 1단 1.48, 2단 2.78,
// 3단 4.07 km/h. 이 조합에서 4.00 km/h 경로는 예전 코드에서 개도가 99.1% 시간
// 포화된 채 2.78 km/h 밖에 못 냈습니다 — 3단이면 4.07 이 나오는데도요.
//
// 위쪽 DRAG 플랜트와 둘 다 있어야 의미가 있습니다. 저쪽은 개도에 권한이 있는
// 실기를 대표하고, 이쪽은 권한이 없는 시뮬레이터를 대표합니다. 한쪽만 보고
// 만든 제어기는 반드시 다른 쪽에서 틀립니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
//
// Closed-loop stage selection.
//
// The plant below is the emulator, measured. Across four runs of one route with
// different pairings, logged at 10 Hz, the steady-state speed within a stage is
// the same whatever the opening (1.5 V and 4.0 V settle at the same km/h). The
// stage sets the speed, and what each stage delivers depends on the pairing.
//
// These numbers are the large 4WD with a 3.6 m plough: 1.48 / 2.78 / 4.07 km/h.
// On that pairing a 4.00 km/h route used to run at 2.78 km/h with the opening
// saturated 99.1% of the time — while stage 3 delivers 4.07.
//
// This plant and the DRAG plant above are only meaningful together: that one
// represents the real machine, where the opening has authority, and this one the
// simulator, where it has none. A controller tuned against either alone is wrong
// on the other.
//
// └────────────────────────────────────────────────────────────────────────────┘

namespace {

constexpr double STAGE_KMH[4] = {0.0, 1.48, 2.78, 4.07};
constexpr double STAGE_TAU_S = 0.5;  // measured settling after a shift
// 개도 0 에서의 감속. 실측: 5.19 -> 1.67 km/h 를 2.1초에.
// Deceleration at zero opening, measured: 5.19 -> 1.67 km/h in 2.1 s.
constexpr double COAST_DECEL_MPS2 = 0.47;

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 개도는 릴레이입니다. 0보다 크기만 하면 값과 무관하게 그 단의 속도로 수렴하고
// (실측: 개도 8% 와 100% 가 같은 km/h), 0이면 선형으로 감속합니다. 예전 플랜트는
// 개도 0 도 1차 지연으로 0에 수렴시켰는데, 그러면 실제보다 훨씬 빨리 느려져서
// 저속에서 정지하는 실패 모드가 가려집니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The opening is a relay: any value above zero converges to that stage's speed
// (measured: 8% and 100% give the same km/h) and zero decelerates linearly. The
// previous plant let zero decay to zero with the same first-order lag, which is
// far faster than the real machine and hides the low-speed stall mode.
// └────────────────────────────────────────────────────────────────────────────┘
double stageStep(const double (&stages)[4], double speed, int gear,
                 double throttle, double brake)
{
    if (brake > 0.0 || throttle <= 0.0) {
        return std::max(0.0, speed - COAST_DECEL_MPS2 * DT);
    }
    const int stage = std::clamp(gear, 1, 3);
    const double demandMps = stages[stage] / 3.6;
    return speed + (demandMps - speed) * (DT / STAGE_TAU_S);
}

double stageQuantizedStep(double speed, int gear, double throttle, double brake)
{
    return stageStep(STAGE_KMH, speed, gear, throttle, brake);
}

struct StageRun
{
    int gear = 0;
    double speedMps = 0.0;       // 마지막 순간속도 / instantaneous speed at the end
    double meanSettledKmh = 0.0;  // 마지막 1/3 구간의 평균 / mean over the last third
    double minSettledKmh = 0.0;
    double maxSettledKmh = 0.0;
    int highestGear = 0;
    int lowestGear = 3;
    int settledStalls = 0;
};

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// samplePeriodTicks 는 이 기계의 속도 측정 주기입니다. 실측 텔레메트리에서
// 보고 속도가 바뀌는 간격이 약 0.3초라 기본값 3(=0.3s)을 씁니다. 변조기의
// 스위칭 속도는 제어 주기가 아니라 이 값이 정하므로, 10Hz 로 두고 시험하면
// 실제보다 훨씬 곱게 나옵니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// samplePeriodTicks is how often this machine reports a speed. The measured
// telemetry changes value about every 0.3 s, hence the default of 3 ticks. The
// modulator's switching rate is set by that, not by the control period, so
// testing at a full 10 Hz measurement would flatter it.
// └────────────────────────────────────────────────────────────────────────────┘
StageRun runRelayPlant(const double (&stages)[4], double targetKmh, int ticks,
                       int samplePeriodTicks = 3)
{
    SpeedController ctrl;
    StageRun run;
    double speed = 0.0;
    double reported = 0.0;
    const int settleFrom = ticks - ticks / 3;
    double sum = 0.0;
    int n = 0;
    run.minSettledKmh = 1e9;
    for (int i = 0; i < ticks; ++i) {
        if (i % samplePeriodTicks == 0) { reported = speed; }
        const SpeedController::Output out =
                ctrl.update(targetKmh, reported, DT, i / samplePeriodTicks);
        run.gear = out.gear;
        run.highestGear = std::max(run.highestGear, out.gear);
        run.lowestGear = std::min(run.lowestGear, out.gear);
        speed = stageStep(stages, speed, out.gear, out.throttle, out.brake);
        if (i >= settleFrom) {
            sum += speed * 3.6;
            ++n;
            run.minSettledKmh = std::min(run.minSettledKmh, speed * 3.6);
            run.maxSettledKmh = std::max(run.maxSettledKmh, speed * 3.6);
            if (speed < 0.05) { ++run.settledStalls; }
        }
    }
    run.speedMps = speed;
    run.meanSettledKmh = (n > 0) ? sum / n : 0.0;
    return run;
}

StageRun runStageQuantized(double targetKmh, int ticks)
{
    // 매 틱 새 샘플. 영차 홀드가 아니라 단 선택 자체를 봅니다.
    // A fresh sample every tick: this exercises stage selection, not the hold.
    return runRelayPlant(STAGE_KMH, targetKmh, ticks, 1);
}

} // namespace

TEST(SpeedControllerTest, UpshiftsUntilTheTargetIsReachable) {
    // 이것이 보고된 버그입니다. 예전 gearForSpeed 는 4.00 km/h 를 무조건 2단으로
    // 보냈고, 2단은 이 조합에서 2.78 밖에 못 냅니다. 목표를 낼 수 있는 단이
    // 있는데도 요청한 적이 없었습니다.
    // The reported bug: the old gearForSpeed sent 4.00 km/h to stage 2, which on
    // this pairing tops out at 2.78. A stage that can do it existed and was never
    // asked for.
    const StageRun run = runStageQuantized(4.0, 600);  // 60 s

    EXPECT_EQ(run.gear, 3);
    EXPECT_NEAR(run.speedMps * 3.6, STAGE_KMH[3], 0.05)
        << "the machine can reach the route speed and must end up doing so";
}

TEST(SpeedControllerTest, StopsAtTheLowestStageThatMeetsTheTarget) {
    // 위로 올라가되 필요 이상으로는 올라가지 않아야 합니다. 2.5 km/h 는 2단
    // (2.78) 이 낼 수 있으므로 3단(4.07)까지 갈 이유가 없습니다 — 과속은
    // 브레이크가 없는 이 기계에서 되돌릴 수 없습니다.
    // Climb, but no further than necessary: 2.5 km/h is within stage 2 (2.78), so
    // stage 3 (4.07) is not warranted — and with no brake, overspeed cannot be
    // taken back.
    const StageRun run = runStageQuantized(2.5, 900);  // 90 s

    EXPECT_EQ(run.gear, 2);
    EXPECT_EQ(run.highestGear, 2);
}

TEST(SpeedControllerTest, BelowFloorTargetStaysInTheSlowestStage) {
    // 경로가 기계 하한보다 느린 속도를 요구하면 가장 낮은 단이 정답입니다.
    // 폐루프가 여기서 위로 기어오르면 두렁에서 과속합니다.
    // When the route asks for less than the machine's floor, the slowest stage is
    // the right answer; a closed loop that climbs here overspeeds the headland.
    const StageRun run = runStageQuantized(1.2, 900);  // 90 s

    EXPECT_EQ(run.gear, 1);
    EXPECT_EQ(run.highestGear, 1);
}

TEST(SpeedControllerTest, SlowAccelerationIsNotMistakenForAStageCeiling) {
    // 부하가 큰 기계는 목표 도달까지 SHORTFALL_HOLD_S 보다 오래 걸릴 수 있고,
    // 그동안 개도는 당연히 끝까지 열려 있습니다. 그것을 "이 단의 천장"으로 읽고
    // 변속해 버리면, 개도에 권한이 있는 실기에서 근거 없이 단을 올리게 됩니다.
    // 여기서는 목표 도달에 9.3초가 걸리므로 정착(2s)+확인(2s) 창을 훌쩍 넘깁니다.
    //
    // A heavily loaded machine can take longer than SHORTFALL_HOLD_S to reach the
    // target with the opening legitimately wide open throughout. Reading that as a
    // stage ceiling would upshift a real machine — where the opening does have
    // authority — for no reason. This plant needs 9.3 s, well past the 2 s settle
    // plus 2 s confirmation window.
    SpeedController ctrl;
    double speed = 0.0;
    int highestGear = 0;
    for (int i = 0; i < 1200; ++i) {  // 120 s
        const SpeedController::Output out = ctrl.update(4.0, speed, DT, i);
        highestGear = std::max(highestGear, out.gear);
        speed += (0.4 * out.throttle - 0.28) * DT;
        speed = std::max(0.0, speed);
    }

    EXPECT_EQ(highestGear, 2) << "a slow climb to the target is not a ceiling";
    EXPECT_NEAR(speed, 4.0 / 3.6, 0.05);
}

TEST(SpeedControllerTest, LearnedFloorReplacesTheCompactTractorConstant) {
    // 하한 상수 2.05 km/h 는 compact 트랙터에서 잰 값입니다. 1단이 실제로 내는
    // 속도를 알고 나면 그것이 이 조합의 진짜 하한이고, 텔레메트리의
    // belowSlowestStage 도 그 값을 기준으로 판정해야 합니다.
    // The 2.05 km/h floor was measured on the compact tractor. Once stage 1's real
    // speed is known that is this pairing's floor, and belowSlowestStage has to be
    // judged against it rather than against the constant.
    // 2.5 km/h 는 1단(1.48)이 낼 수 없는 속도라 개도가 끝까지 열린 채 부족한
    // 상태가 이어집니다. 단의 천장이 드러나는 것은 이때뿐입니다 — 변조 중인
    // 단은 듀티비가 속도를 정하므로 아무것도 알려주지 않습니다.
    // 2.5 km/h is beyond stage 1 (1.48), so the opening pins wide open and stays
    // short. That is the only condition under which a stage's ceiling shows: a
    // modulated stage's speed is set by the duty cycle and says nothing.
    SpeedController ctrl;
    double speed = 0.0;
    SpeedController::Output out;
    for (int i = 0; i < 1200; ++i) {  // 120 s
        out = ctrl.update(2.5, speed, DT, i);
        speed = stageQuantizedStep(speed, out.gear, out.throttle, out.brake);
    }

    EXPECT_LT(out.machineFloorKmh, SpeedController::MIN_MACHINE_SPEED_KMH)
        << "the learned floor must replace the compact tractor's constant";
    EXPECT_NEAR(out.machineFloorKmh, STAGE_KMH[1], 0.05);
    EXPECT_NEAR(out.targetMps * 3.6, 2.5, 1e-9)
        << "the request is followed as written whatever the stage can do";
}

TEST(SpeedControllerTest, StageTableIsDroppedWhenTheMachineChanges) {
    // 학습값은 그 트랙터/작업기 조합에서만 유효합니다. 다음 주행이 이전 조합의
    // 값을 물려받으면 처음부터 틀린 단을 고릅니다.
    // What was learned holds only for that tractor/implement pair; inheriting it
    // into the next run starts the next machine in the wrong stage.
    SpeedController ctrl;
    double speed = 0.0;
    SpeedController::Output out;
    for (int i = 0; i < 600; ++i) {  // 60 s at 4.0: learns its way up to stage 3
        out = ctrl.update(4.0, speed, DT, i);
        speed = stageQuantizedStep(speed, out.gear, out.throttle, out.brake);
    }
    ASSERT_EQ(out.gear, 3) << "the learning has moved selection off the prior";
    for (int i = 600; i < 1500; ++i) {  // 90 s at 2.5: stage 1 is short, so it learns
        out = ctrl.update(2.5, speed, DT, i);
        speed = stageQuantizedStep(speed, out.gear, out.throttle, out.brake);
    }
    ASSERT_LT(out.machineFloorKmh, SpeedController::MIN_MACHINE_SPEED_KMH);

    ctrl.reset();
    const SpeedController::Output afterReset = ctrl.update(4.0, 0.0, DT, 9000);
    EXPECT_EQ(afterReset.gear, 2) << "reset must restore the prior, not the learning";
    EXPECT_FALSE(afterReset.stageMeasured);
    EXPECT_DOUBLE_EQ(afterReset.machineFloorKmh,
                     SpeedController::MIN_MACHINE_SPEED_KMH);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
//
// 개도 변조기: RDDF에 적힌 속도를 조합과 무관하게 지키는가.
//
// 아래 세 표는 에뮬레이터에서 잰 실측치입니다. 세 조합 모두 4.00 km/h 를 낼 수
// 있는 단이 정확히 4.00 인 경우가 없습니다 — 3.33 이거나 4.82 이거나 5.37 입니다.
// 단만으로는 어느 조합에서도 4.00 이 나오지 않는다는 뜻이고, 그게 이 변조기가
// 있는 이유입니다.
//
// 각 시험은 평균만이 아니라 정지 횟수도 봅니다. 평균을 맞추겠다고 멈췄다
// 달렸다 하면 쟁기를 박고 있는 트랙터에서는 실패입니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
//
// The opening modulator: is the RDDF's speed honoured whatever the pairing?
//
// The three tables below are measured on the emulator. On none of the three does
// a stage deliver 4.00 km/h — it is 3.33, or 4.82, or 5.37. Stages alone cannot
// produce 4.00 on any pairing, which is the whole reason this modulator exists.
//
// Every case checks standstills as well as the mean: averaging the right speed by
// stopping and starting is a failure for a tractor with a plough in the ground.
//
// └────────────────────────────────────────────────────────────────────────────┘

namespace {
// 실측 / measured
constexpr double LARGE_PLOUGH_KMH[4]  = {0.0, 1.48, 2.78, 4.07};
constexpr double MEDIUM_PLOUGH_KMH[4] = {0.0, 1.85, 3.33, 4.82};
constexpr double COMPACT_BARE_KMH[4]  = {0.0, 2.78, 5.37, 5.37};
}  // namespace

TEST(SpeedControllerTest, RouteSpeedIsHeldOnTheMediumPairingWhereNoStageMatches) {
    // 2단 3.33 은 모자라고 3단 4.82 는 20% 넘칩니다. v1.1.52 는 이 조합에서
    // 3단과 2단 사이를 오가며 평균 3.33 에 머물렀습니다.
    // Stage 2 (3.33) is short and stage 3 (4.82) is 20% over. v1.1.52 shuttled
    // between them on this pairing and stayed at a 3.33 mean.
    const StageRun run = runRelayPlant(MEDIUM_PLOUGH_KMH, 4.0, 1800);  // 180 s

    EXPECT_NEAR(run.meanSettledKmh, 4.0, 0.15);
    EXPECT_EQ(run.gear, 3) << "the stage that can reach it must stay engaged";
    EXPECT_EQ(run.settledStalls, 0) << "the mean must not be averaged over stops";
}

TEST(SpeedControllerTest, RouteSpeedIsHeldOnTheCompactPairingThatOvershootsBadly) {
    // compact 는 2단이 5.37 입니다. 4.00 요청에 34% 초과입니다.
    // The compact's stage 2 is 5.37 — 34% over a 4.00 request.
    const StageRun run = runRelayPlant(COMPACT_BARE_KMH, 4.0, 1800);

    EXPECT_NEAR(run.meanSettledKmh, 4.0, 0.15);
    EXPECT_EQ(run.settledStalls, 0);
}

TEST(SpeedControllerTest, RouteSpeedIsHeldOnTheLargePairingThatCannotOvershoot) {
    // large + 3.6m 쟁기의 3단은 4.07 로 요청과 거의 같습니다. 여기서는 변조가
    // 거의 필요 없어야 하고, 불필요한 개도 끊김이 생기면 안 됩니다.
    // Stage 3 on the large pairing is 4.07, all but equal to the request. Almost
    // no modulation should be needed and none should be manufactured.
    const StageRun run = runRelayPlant(LARGE_PLOUGH_KMH, 4.0, 1800);

    EXPECT_NEAR(run.meanSettledKmh, 4.07, 0.10);
    EXPECT_EQ(run.gear, 3);
    EXPECT_EQ(run.settledStalls, 0);
}

TEST(SpeedControllerTest, HeadlandSpeedIsHeldWithoutComingToAStandstill) {
    // 두렁 1.20 km/h 요청. compact 의 1단은 2.78 로 132% 초과입니다. 예전 코드는
    // 목표를 하한으로 올려 2.78 그대로 달렸습니다.
    //
    // 순수 델타-시그마(장부 부호만으로 스위칭)를 쓰면 여기서 반드시 멈춥니다:
    // 코스팅 동안 쌓인 장부를 저속에서 갚는 데 시간이 오래 걸려 그 사이에 차가
    // 0까지 내려갑니다. 이 시험이 그 설계를 배제합니다.
    //
    // A 1.20 km/h headland request. The compact's stage 1 is 2.78 — 132% over —
    // and the old code raised the target to the floor and simply ran 2.78.
    //
    // A pure delta-sigma (switching on the ledger's sign alone) always stalls
    // here: repaying the ledger accumulated during a coast takes long enough at
    // low speed for the machine to reach zero. This test rules that design out.
    const StageRun run = runRelayPlant(COMPACT_BARE_KMH, 1.2, 1800);

    EXPECT_EQ(run.gear, 1);
    EXPECT_NEAR(run.meanSettledKmh, 1.2, 0.15);
    EXPECT_EQ(run.settledStalls, 0)
        << "min " << run.minSettledKmh << " km/h — the machine stopped in the turn";
}

TEST(SpeedControllerTest, ProportionalMachineNeverEngagesTheModulator) {
    // 개도에 진짜 권한이 있는 기계에서는 PID 가 목표를 잡아내므로 장부가 불감대
    // 안에 머물고 변조기는 한 번도 켜지지 않아야 합니다. 켜지면 그 기계에서
    // 예전의 가속/정지 리밋 사이클을 새로 만들어 내는 셈입니다.
    // On a machine where the opening has authority the PID holds the target, the
    // ledger stays inside the deadband and the modulator must never fire —
    // firing there would recreate the old accelerate/stop limit cycle.
    SpeedController ctrl;
    double speed = 0.0;
    int gatedTicks = 0;
    for (int i = 0; i < TOTAL_TICKS; ++i) {
        const SpeedController::Output out = ctrl.update(4.0, speed, DT);
        speed = step(speed, out.throttle, out.brake);
        if (i >= TOTAL_TICKS - SETTLED_TICKS && out.throttleGated) { ++gatedTicks; }
    }
    EXPECT_EQ(gatedTicks, 0);
    EXPECT_NEAR(speed, 4.0 / 3.6, 0.05);
}

TEST(SpeedControllerTest, HeadlandDoesNotStallWhenCoastingIsFasterThanTheSampleRate) {
    // 실측 실패 사례입니다. 쟁기를 박은 medium 조합의 코스팅 감속은 1.7 m/s^2 이고
    // 속도 측정은 약 0.3초마다 옵니다. 지령을 측정 주기에 묶어 두면 한 번 개도를
    // 끊을 때 최소 0.51 m/s 가 지워지는데, 두렁 목표 1.20 km/h 는 0.33 m/s 이므로
    // 한 번만 끊어도 무조건 정지입니다. 에뮬레이터에서 두렁 시간의 25%가
    // 정지였습니다.
    //
    // The measured failure. Coast deceleration for the medium tractor with its
    // plough down is 1.7 m/s^2 and speed arrives about every 0.3 s. Tying the
    // command to the sample period makes the shortest cut 0.51 m/s, and a
    // 1.20 km/h headland target is 0.33 m/s — one cut is always a stop. A quarter
    // of the headland was spent at a standstill on the emulator.
    constexpr double STAGES[4] = {0.0, 1.85, 3.33, 4.82};
    constexpr double HARD_COAST = 1.7;   // m/s^2, plough in the ground
    constexpr int SAMPLE_TICKS = 3;      // speed reported every 0.3 s

    SpeedController ctrl;
    double speed = 0.0;
    double reported = 0.0;
    int settledStalls = 0;
    double sum = 0.0;
    int n = 0;
    for (int i = 0; i < 1800; ++i) {     // 180 s
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 샘플 id 는 매 틱(10Hz) 새로 오지만 속도 "값"은 0.3초마다 바뀝니다.
        // 에뮬레이터가 실제로 그렇습니다 — poseTimestampDeltaMs 는 약 102 ms 인데
        // 보고 속도는 0.185 km/h 단위로 양자화돼 있어 서너 틱에 한 번 움직입니다.
        // 신선도를 샘플 id 로만 판단하면 이 지연이 보이지 않습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // A fresh sample id every tick (10 Hz) but a speed *value* that only
        // moves every 0.3 s — which is what the emulator does:
        // poseTimestampDeltaMs is about 102 ms while the reported speed is
        // quantised to 0.185 km/h and therefore changes every third or fourth
        // tick. Judging freshness by sample id alone hides that lag.
        // └────────────────────────────────────────────────────────────────────┘
        if (i % SAMPLE_TICKS == 0) { reported = speed; }
        const SpeedController::Output out = ctrl.update(1.2, reported, DT, i);
        if (out.brake > 0.0 || out.throttle <= 0.0) {
            speed = std::max(0.0, speed - HARD_COAST * DT);
        } else {
            speed += (STAGES[std::clamp(out.gear, 1, 3)] / 3.6 - speed)
                   * (DT / STAGE_TAU_S);
        }
        if (i >= 1200) {
            sum += speed * 3.6;
            ++n;
            if (speed < 0.05) { ++settledStalls; }
        }
    }
    EXPECT_EQ(settledStalls, 0)
        << "stalled on " << settledStalls << " of " << n
        << " ticks — the opening must be restored between measurements";
    EXPECT_NEAR(sum / n, 1.2, 0.25);
}

TEST(SpeedControllerTest, LedgerDoesNotBankDebtWhileStopIsCommanded) {
    // 정지 지시 동안 뒤처짐을 쌓아 두면 다시 출발할 때 그만큼 과속으로 갚습니다.
    // Debt banked while a stop is commanded would be repaid as overspeed later.
    SpeedController ctrl;
    for (int i = 0; i < 300; ++i) { ctrl.update(0.0, 0.0, DT, i); }
    const SpeedController::Output out = ctrl.update(0.0, 0.0, DT, 999);
    EXPECT_DOUBLE_EQ(out.debtM, 0.0);
}
