#include <gtest/gtest.h>

#include <AppMain/tracking/SteeringController.hpp>

#include <algorithm>
#include <cmath>
#include <vector>

using namespace AppMain::tracking;

// ┌─ 한국어: 이 파일이 무엇을 지키는가 ────────────────────────────────────────┐
//
// SteeringController 는 오랫동안 테스트가 하나도 없었고, 그래서 이 코드베이스에서
// 이미 두 번 잡았던 버그가 여기서만 살아남았습니다: 수정 전 mux에서 1Hz 로 들어오던 측정에
// 10Hz PID 를 거는 것.
//
// 에뮬레이터 실측(107초 주행):
//   - 엔코더 갱신 73회 중 61회가 정확히 10틱(=1초) 간격
//   - 틱당 변화 중앙값 0.006°, p99 12.9° — 계단 신호
//   - 새 측정이 전혀 없는 구간에서 모터 지령 변동폭이 p90 255, 최대 1200(풀스케일)
//   - 모터 지령 부호 반전 107초에 21회
//
// 결과는 조향이 지령이 아니라 자기 노이즈를 쫓는 발진이었고, 지령이 ±10° 안에
// 머무는 직선 구간에서도 실제 앞바퀴는 ±25° 를 풀스윙했습니다. 그것이 사용자가
// 본 S자 사행입니다.
//
// 같은 실수를 SpeedController(영차 홀드)와 PurePursuitTracker(yaw 감쇠항)에서
// 이미 고쳤습니다. 대응하는 테스트가 각각 있고, 이 파일이 세 번째입니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English: what this file protects ─────────────────────────────────────────┐
//
// SteeringController had no tests at all, which is why a bug already fixed twice
// in this codebase survived only here: running a 10 Hz PID on the pre-fix 1 Hz
// mux measurement.
//
// Measured over a 107 s emulator run, the encoder updated every ten ticks, the
// motor command swung by up to the full 1200 scale inside a single measurement
// interval where no new information existed, and the command reversed sign 21
// times. Steering chased its own noise: the wheels swung the full +/-25 deg while
// the demand stayed inside +/-10 deg on a straight stretch — the S-weave.
//
// SpeedController (zero-order hold) and PurePursuitTracker (yaw damping over the
// measurement interval) already have their own guards. This is the third.
// └────────────────────────────────────────────────────────────────────────────┘

namespace {

constexpr double TICK_S = 0.1;            // IPathTracker::preferredPeriodMs()
// Deliberately retain the harsher pre-fix cadence as a held-sample regression;
// the corrected seven-way mux delivers each encoder interface at about 10 Hz.
constexpr int TICKS_PER_ENCODER = 10;
constexpr double STEERING_RATIO = 24.2;   // SteeringController 와 같은 값
constexpr double DEG_PER_RAD = 57.29577951308232;

VehicleState movingState()
{
    VehicleState s;
    s.speedMps = 1.08;      // 이 기계가 실제로 내는 속도 / a speed the machine reaches
    s.yawRateValid = false; // 엔코더만으로 시험합니다 / encoder-only, to isolate the PID
    return s;
}

// 앞바퀴각(도) -> 엔코더 원값(모터축 도)
// Front-wheel degrees -> raw encoder reading in motor-shaft degrees.
double encoderFor(double wheelDeg)
{
    return wheelDeg * STEERING_RATIO;
}

}  // namespace

// ── 핵심 회귀 방지 / The headline guard ──────────────────────────────────────

TEST(SteeringControllerTest, HoldsCommandBetweenEncoderUpdates)
{
    // 수정 전 조건처럼 엔코더를 1Hz 로 갱신하고 루프는 10Hz 로 둡니다. 새 정보가 없는 9틱 동안
    // 지령이 움직이면, 제어기는 측정이 아니라 자기 내부 상태를 쫓고 있는 것입니다.
    // This regression holds the encoder at the pre-fix 1 Hz while the loop ticks
    // at 10 Hz. If the command
    // moves during the nine ticks that carry no new information, the controller
    // is chasing its own state rather than a measurement.
    // ★ 측정값은 틱마다 아주 조금씩 움직입니다. 자이로 지분과 원점 보정이
    // 10Hz로 돌기 때문이며, 실측 중앙값이 앞바퀴 0.006°/틱 입니다. 이 창(creep)을
    // 빼고 완전히 같은 값을 먹이면 테스트가 버그를 못 잡습니다 — 실제로 그렇게
    // 썼다가 수정을 되돌려도 통과하는 것을 확인했습니다. 창을 반드시 넣으세요.
    // The reading creeps every tick because the gyro share and the offset trim
    // run at 10 Hz — a measured median of 0.006 wheel-deg per tick. A test that
    // feeds a perfectly constant value does NOT catch the bug: written that way,
    // it passed with the fix reverted. The creep has to be here.
    constexpr double CREEP_WHEEL_DEG_PER_TICK = 0.006;

    // ★ 오차가 크면 P항만으로 모터가 포화해서(50 x 30도 = 1500 -> 600) 버그가
    // 있든 없든 지령이 600으로 같습니다. 반드시 포화하지 않는 작은 오차로
    // 시험해야 합니다 — 큰 오차로 썼다가 수정을 되돌려도 통과하는 것을
    // 확인했습니다. 그리고 D항이 실제로 발동하려면 진짜 계단이 한 번 와야 합니다.
    // A large error saturates on the P term alone (50 x 30 deg = 1500 -> 600) and
    // the command reads 600 either way. The error has to stay inside the linear
    // range — written with a large error, this passed with the fix reverted. A
    // genuine step also has to arrive for the derivative to fire at all.
    SteeringController ctrl;
    const VehicleState state = movingState();
    const double targetRad = 0.0;

    double wheelDeg = -10.0;   // 오차 10도 -> 지령 500, 아직 포화 아님
    ctrl.update(targetRad, state, encoderFor(wheelDeg), true, 1, -1, TICK_S);
    for (int i = 1; i < TICKS_PER_ENCODER; ++i) {
        ctrl.update(targetRad, state,
                    encoderFor(wheelDeg + CREEP_WHEEL_DEG_PER_TICK * i), true,
                    1, -1, TICK_S);
    }

    // 1초 뒤 진짜 갱신이 도착합니다 (앞바퀴 6도 계단, 실측 규모).
    // A genuine update lands a second later: a 6 deg step, the measured size.
    wheelDeg = -4.0;
    const SteeringController::Output onUpdate =
            ctrl.update(targetRad, state, encoderFor(wheelDeg), true, 2, -1, TICK_S);

    for (int i = 1; i < TICKS_PER_ENCODER; ++i) {
        const double crept = wheelDeg + CREEP_WHEEL_DEG_PER_TICK * i;
        const SteeringController::Output held =
                ctrl.update(targetRad, state, encoderFor(crept), true, 2, -1, TICK_S);
        EXPECT_EQ(held.motorCmd, onUpdate.motorCmd)
                << "motor command moved from " << onUpdate.motorCmd << " to "
                << held.motorCmd << " on tick " << i
                << " although no genuine measurement had arrived. A 10 Hz PID on "
                   "a 1 Hz measurement divides the derivative by the tick, so "
                   "every arrival spikes the command and it decays over the next "
                   "nine ticks on stale data — measured on the emulator as a swing "
                   "of up to the full 1200 scale inside one interval.";
    }
}

TEST(SteeringControllerTest, ActsOnAGenuineEncoderUpdate)
{
    // 위 테스트의 반대쪽: 홀드가 지나쳐서 새 측정까지 무시하면 안 됩니다.
    // The converse: holding must not go so far as to ignore a real update.
    SteeringController ctrl;
    const VehicleState state = movingState();
    const double targetRad = 20.0 / DEG_PER_RAD;

    const SteeringController::Output before =
            ctrl.update(targetRad, state, encoderFor(-10.0), true, TICK_S);
    const SteeringController::Output after =
            ctrl.update(targetRad, state, encoderFor(18.0), true, TICK_S);

    EXPECT_NE(after.motorCmd, before.motorCmd)
            << "a genuine encoder update must change the command";
    EXPECT_LT(std::fabs(after.errorDeg), std::fabs(before.errorDeg))
            << "the wheel moved toward the target, so the error must shrink";
}

TEST(SteeringControllerTest, DerivativeUsesTheMeasurementIntervalNotTheTick)
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 오차를 제어 주기(0.1s)로 미분하면, 1초치 변화량을 0.1초로 나누게 되어
    // 정확히 10배가 됩니다. 같은 계단 입력을 두 방식이 어떻게 다루는지 보려면
    // D 항만 남기고(kp=ki=0) 계단을 한 번 먹이면 됩니다.
    //
    // 앞바퀴 5도에 해당하는 변화가 1초에 걸쳐 들어왔으므로 올바른 미분값은
    // 5deg/s 근처여야 합니다. 틱으로 나누면 50deg/s 가 됩니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // Differentiating by the 0.1 s tick spreads a second of change over a tenth
    // of a second — exactly 10x. With only the D term alive, a 5 deg step that
    // arrived over 1 s must read as roughly 5 deg/s, not 50.
    SteeringController ctrl;
    ctrl.setGains(0.0, 0.0, 1.0);
    const VehicleState state = movingState();
    const double targetRad = 0.0;

    // 첫 측정으로 기준을 잡고, 1초를 흘려보낸 뒤 5도 움직인 측정을 넣습니다.
    // Establish a baseline, let a second pass, then deliver a 5 deg step.
    ctrl.update(targetRad, state, encoderFor(0.0), true, 1, -1, TICK_S);
    for (int i = 1; i < TICKS_PER_ENCODER; ++i) {
        ctrl.update(targetRad, state, encoderFor(0.0), true, 1, -1, TICK_S);
    }
    const SteeringController::Output out =
            ctrl.update(targetRad, state, encoderFor(5.0), true, 2, -1, TICK_S);

    // 목표가 0이므로 오차는 -5도, 1초에 걸친 변화이니 -5deg/s 근처입니다.
    // 저역통과(D_TAU_S = 0.2s)가 걸려 있어 크기는 줄지만, 틱으로 나눈 값(-50)
    // 근처가 나오면 안 됩니다.
    // The target is 0 so the error stepped by -5 deg over 1 s. The low-pass
    // shrinks the magnitude, but it must not land anywhere near the -50 that
    // dividing by the tick would give.
    EXPECT_LT(std::fabs(out.dTerm), 20.0)
            << "dTerm " << out.dTerm << " is far larger than a 5 deg/s change "
               "warrants — the derivative is dividing by the tick, not by the "
               "measurement interval";
}

// ── 폐루프: 실제로 발진하지 않는가 / Closed loop: does it actually settle ────

namespace {

struct SteerLoopResult
{
    double settledErrorDeg = 0.0;   // 마지막 구간 평균 |오차|
    double overshootDeg = 0.0;      // 목표를 넘어간 최대량
    int commandReversals = 0;       // 모터 지령 부호 반전 횟수
    double pinnedFraction = 0.0;    // 모터가 한계에 물려 있던 비율
};

/**
 * 조향 액추에이터 플랜트. 모터 지령은 속도 지령이고, 엔코더는 1Hz 로만
 * 보고됩니다 — 실측된 두 가지 성질 그대로입니다.
 *
 * The steering plant: the motor command is a velocity command and the encoder
 * only reports at 1 Hz, both as measured on the emulator.
 */
SteerLoopResult driveSteering(SteeringController& ctrl, double targetDeg,
                              double startDeg, double durationS)
{
    // 모터 지령 600 에서 앞바퀴가 약 15도/s 로 움직입니다 (실측 p90 14.2-15.7).
    // At a command of 600 the wheel moves about 15 deg/s (measured p90 14.2-15.7).
    constexpr double DEG_PER_S_PER_CMD = 15.0 / 600.0;
    constexpr double WHEEL_LIMIT_DEG = 25.2;

    VehicleState state = movingState();
    double wheelDeg = startDeg;
    double reportedDeg = startDeg;   // 엔코더가 마지막으로 보고한 값
    int prevCmd = 0;
    int reversals = 0;
    int pinned = 0;
    double maxBeyond = 0.0;
    std::vector<double> tailError;

    const int ticks = static_cast<int>(durationS / TICK_S);
    for (int i = 0; i < ticks; ++i) {
        if (i % TICKS_PER_ENCODER == 0) {
            reportedDeg = wheelDeg;
        }

        const SteeringController::Output out = ctrl.update(
                targetDeg / DEG_PER_RAD, state, encoderFor(reportedDeg), true,
                i / TICKS_PER_ENCODER, -1, TICK_S);

        if (std::abs(out.motorCmd) >= 595) {
            ++pinned;
        }
        if (prevCmd != 0 && out.motorCmd != 0
            && (prevCmd > 0) != (out.motorCmd > 0)) {
            ++reversals;
        }
        prevCmd = out.motorCmd;

        wheelDeg += out.motorCmd * DEG_PER_S_PER_CMD * TICK_S;
        wheelDeg = std::clamp(wheelDeg, -WHEEL_LIMIT_DEG, WHEEL_LIMIT_DEG);

        // 목표를 지나쳐 간 양 / how far past the target it went
        if (targetDeg >= 0.0) {
            maxBeyond = std::max(maxBeyond, wheelDeg - targetDeg);
        } else {
            maxBeyond = std::max(maxBeyond, targetDeg - wheelDeg);
        }

        if (i >= ticks / 2) {
            tailError.push_back(std::fabs(wheelDeg - targetDeg));
        }
    }

    SteerLoopResult r;
    r.overshootDeg = maxBeyond;
    r.commandReversals = reversals;
    r.pinnedFraction = static_cast<double>(pinned) / static_cast<double>(ticks);
    double sum = 0.0;
    for (double e : tailError) {
        sum += e;
    }
    r.settledErrorDeg = tailError.empty() ? 0.0 : sum / static_cast<double>(tailError.size());
    return r;
}

}  // namespace

TEST(SteeringControllerTest, SettlesOnTheDemandInsteadOfOscillating)
{
    // 우측 풀락에서 좌측 20도로 가는, 실제 주행에서 흔한 큰 이동입니다.
    // A large move from right lock to 20 deg left, common in the real run.
    SteeringController ctrl;
    const SteerLoopResult r = driveSteering(ctrl, 20.0, -25.0, 20.0);

    EXPECT_LT(r.settledErrorDeg, 3.0)
            << "the wheel never settles on the demand: mean |error| "
            << r.settledErrorDeg << " deg over the second half";
    EXPECT_LT(r.overshootDeg, 8.0)
            << "overshot the demand by " << r.overshootDeg
            << " deg; a saturating loop on a 1 Hz measurement runs past the "
               "target and has to come back, which is the weave";
    EXPECT_LE(r.commandReversals, 4)
            << "motor command reversed sign " << r.commandReversals
            << " times settling a single step — the loop is oscillating";
}

TEST(SteeringControllerTest, DoesNotSitPinnedAtTheMotorLimit)
{
    // 작은 조향 요구에는 모터가 한계에 물리면 안 됩니다. 물린다면 제어기가
    // 비례 제어가 아니라 릴레이로 동작하고 있는 것입니다.
    // A small demand must not pin the motor; if it does, the controller is acting
    // as a relay rather than a proportional loop.
    SteeringController ctrl;
    const SteerLoopResult r = driveSteering(ctrl, 5.0, 0.0, 20.0);
    EXPECT_LT(r.pinnedFraction, 0.35)
            << "motor pinned at the limit on " << (100.0 * r.pinnedFraction)
            << "% of ticks for a 5 deg demand";
}

// ── 실측각의 출처 / where the measured angle comes from ─────────────────────

TEST(SteeringControllerTest, HealthySignedEncoderIsPrimaryDuringRightTurn)
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 이 에뮬레이터의 엔코더는 우측 절반에서 0 으로 잘려 있습니다. 실측 169초
    // 주행에서 바퀴가 실제로 -2.8 ~ -25.9도에 있는 동안 엔코더는 -1.0 ~ 6.5
    // 모터도(사실상 0)만 보고했고, 1,568틱 내내 -1.0 아래로 내려간 적이 없습니다.
    //
    // 그 신호를 크게 신뢰하면 우회전 내내 제어기가 "바퀴는 거의 직진"이라고 믿게
    // 되고, 좌회전만 정상인 비대칭 고장이 됩니다. 그것이 S자 사행의 원인이었습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // The encoder on this emulator is clipped to zero over its right half: across
    // 169 s it never read below -1.0 motor-deg while the wheel was genuinely at
    // -2.8 to -25.9 deg. Trusting it leaves the controller believing the wheels
    // are near straight throughout every right turn — the asymmetric failure that
    // sustained the S-weave. That defect is fixed at the simulator boundary;
    // participant code now verifies the normal, correctly signed interface.
    VehicleState state;
    state.speedMps = 1.08;
    state.yawRateValid = true;
    // 좌수 좌표계에서 우회전은 heading 을 증가시키고, 앞바퀴각은 음수입니다.
    // delta = atan(L*yawRate/v) 로 -20도가 나오도록 yaw rate 를 잡습니다.
    const double wantWheelRad = -20.0 / DEG_PER_RAD;
    state.yawRateRadS = -std::tan(wantWheelRad) * state.speedMps / 2.05;

    SteeringController ctrl;
    const SteeringController::Output out =
            ctrl.update(0.0, state, encoderFor(-20.0), true, TICK_S);

    ASSERT_TRUE(out.measuredValid);
    EXPECT_NEAR(out.measuredDeg, -20.0, 0.5);
}

TEST(SteeringControllerTest, YawOnlyUsesLeftHandedWheelSign)
{
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    state.yawRateRadS = 0.12;  // increasing heading is a right turn

    SteeringController ctrl;
    const auto out = ctrl.update(0.0, state, 0.0, false, -1, 1, TICK_S);

    ASSERT_TRUE(out.measuredValid);
    EXPECT_LT(out.measuredDeg, 0.0)
            << "positive yaw rate must reconstruct negative/right wheel angle";
}

TEST(SteeringControllerTest, LosesValidityWhenBothSourcesDisappear)
{
    SteeringController ctrl;
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    state.yawRateRadS = 0.1;
    ASSERT_TRUE(ctrl.update(0.0, state, encoderFor(-10.0), true,
                            1, 1, TICK_S).measuredValid);

    state.yawRateValid = false;
    const auto out = ctrl.update(0.0, state, encoderFor(-10.0), false,
                                 1, 1, TICK_S);
    EXPECT_FALSE(out.measuredValid);
    EXPECT_EQ(out.motorCmd, 0);
}

TEST(SteeringControllerTest, ValidBitsWithoutSampleIdentityFailClosed)
{
    SteeringController ctrl;
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    state.yawRateRadS = 0.1;
    const auto out = ctrl.update(10.0 / DEG_PER_RAD, state,
                                 encoderFor(0.0), true, -1, -1, TICK_S);
    EXPECT_FALSE(out.measuredValid);
    EXPECT_EQ(out.motorCmd, 0);
}

TEST(SteeringControllerTest, HeldTimestampHoldsPidDespiteValueCreep)
{
    SteeringController ctrl;
    const VehicleState state = movingState();
    const auto first = ctrl.update(0.0, state, encoderFor(-5.0), true,
                                   10, -1, TICK_S);
    for (int i = 1; i <= 20; ++i) {
        const auto held = ctrl.update(0.0, state,
                encoderFor(-5.0 + 0.1 * i), true, 10, -1, TICK_S);
        EXPECT_EQ(held.motorCmd, first.motorCmd);
    }
}

TEST(SteeringControllerTest, YawOnlyFallbackIsSampleAware)
{
    SteeringController ctrl;
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    state.yawRateRadS = 0.08;
    const auto first = ctrl.update(0.0, state, 0.0, false,
                                   -1, 40, TICK_S);
    for (int i = 0; i < 10; ++i) {
        state.yawRateRadS += 0.002; // retained/filtered value may drift
        const auto held = ctrl.update(0.0, state, 0.0, false,
                                      -1, 40, TICK_S);
        EXPECT_EQ(held.motorCmd, first.motorCmd);
    }
    state.yawRateRadS = 0.02;
    const auto next = ctrl.update(0.0, state, 0.0, false,
                                  -1, 41, TICK_S);
    EXPECT_NE(next.motorCmd, first.motorCmd);
}

TEST(SteeringControllerTest, OppositeSignEncoderDownweightsAndRecovers)
{
    SteeringController ctrl;
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    // Actual/yaw-inferred wheel is +10 deg left.
    state.yawRateRadS = -std::tan(10.0 / DEG_PER_RAD) * state.speedMps / 2.05;

    SteeringController::Output out;
    for (int i = 1; i <= 25; ++i) {
        out = ctrl.update(0.0, state, encoderFor(-10.0), true,
                          i, i, TICK_S);
    }
    EXPECT_LT(out.encoderWeight, 0.25);
    EXPECT_GT(out.measuredDeg, 5.0);

    for (int i = 26; i <= 90; ++i) {
        out = ctrl.update(0.0, state, encoderFor(10.0), true,
                          i, i, TICK_S);
    }
    EXPECT_GT(out.encoderWeight, 0.35)
            << "healthy agreement must recover encoder confidence with hysteresis";
}

TEST(SteeringControllerTest, CapturedRawGyroBoundaryAgreesWithGpsAndEncoder)
{
    // Captured v1.0.8 regression sample: GPS heading decreased while raw GyroZ
    // was +0.139 rad/s and the encoder reported +640.1 motor-deg. Raw GyroZ is
    // CCW-positive; GPS/tracking heading is clockwise-positive. After the one
    // boundary conversion, both independent sources describe a left wheel angle.
    constexpr double RAW_GYRO_RAD_S = 0.139;
    constexpr double RAW_GYRO_DEG_S = RAW_GYRO_RAD_S * DEG_PER_RAD;
    constexpr double GPS_HEADING_DERIVATIVE_RAD_S = -0.139;
    constexpr double ENCODER_MOTOR_DEG = 640.1;

    VehicleState state;
    state.speedMps = 0.57;
    state.yawRateValid = true;
    state.yawRateRadS = internalYawRateFromRawGyroDegS(RAW_GYRO_DEG_S);
    EXPECT_NEAR(state.yawRateRadS, GPS_HEADING_DERIVATIVE_RAD_S, 1e-6);

    SteeringController ctrl;
    SteeringController::Output out;
    for (int i = 1; i <= 30; ++i) {
        out = ctrl.update(20.0 / DEG_PER_RAD, state, ENCODER_MOTOR_DEG, true,
                          i, i, TICK_S);
    }

    EXPECT_GT(out.measuredDeg, 20.0);
    EXPECT_GT(out.encoderWeight, 0.65)
            << "the normalized yaw and healthy encoder agree; confidence must stay high";
    EXPECT_GT(out.motorCmd, -590)
            << "agreement must not suppress the encoder and sustain negative full-lock";
}

TEST(SteeringControllerTest, StuckEncoderDownweightsWhenYawAngleMoves)
{
    SteeringController ctrl;
    VehicleState state;
    state.speedMps = 1.0;
    state.yawRateValid = true;
    SteeringController::Output out;
    for (int i = 1; i <= 25; ++i) {
        const double wheelDeg = 2.0 + 0.8 * i;
        state.yawRateRadS = -std::tan(wheelDeg / DEG_PER_RAD) *
                            state.speedMps / 2.05;
        out = ctrl.update(0.0, state, encoderFor(2.0), true,
                          i, i, TICK_S);
    }
    EXPECT_LT(out.encoderWeight, 0.25);
    EXPECT_GT(out.measuredDeg, 10.0);
}

TEST(SteeringControllerTest, OffsetTrimCannotDriftWithoutBound)
{
    // 원점 보정은 오프셋 적분기인데, 스케일이나 부호가 어긋난 신호를 만나면
    // 수렴할 수 없어 영영 표류합니다. 실측에서 169초에 6.6도까지 밀렸습니다.
    // The trim is an offset integrator; against a scale or sign error it cannot
    // converge and simply drifts — measured at 6.6 deg over 169 s.
    VehicleState state;
    state.speedMps = 1.08;
    state.yawRateValid = true;
    state.yawRateRadS = std::tan(-20.0 / DEG_PER_RAD) * state.speedMps / 2.05;

    SteeringController ctrl;
    // 엔코더가 계속 0 을 주장하는 상태로 5분을 돌립니다.
    // Five minutes with the encoder insisting on zero.
    SteeringController::Output out;
    for (int i = 0; i < 3000; ++i) {
        out = ctrl.update(0.0, state, encoderFor(0.0), true, TICK_S);
    }
    EXPECT_LT(std::fabs(out.measuredDeg), 90.0)
            << "measured angle ran away to " << out.measuredDeg
            << " deg — the offset trim has no bound";
}

TEST(SteeringControllerTest, FaultCutsTheCommandAndDoesNotLeaveItHeld)
{
    // 고장으로 지령을 끊은 뒤, 영차 홀드가 예전 지령을 되살리면 안 됩니다.
    // After a fault cuts the command, the zero-order hold must not resurrect it.
    SteeringController ctrl;
    VehicleState state = movingState();
    ctrl.update(20.0 / DEG_PER_RAD, state, encoderFor(-10.0), true, TICK_S);

    state.motorFaultBits = 0xFFFFFFFFu;
    const SteeringController::Output faulted =
            ctrl.update(20.0 / DEG_PER_RAD, state, encoderFor(-10.0), true, TICK_S);
    EXPECT_TRUE(faulted.blocked);
    EXPECT_EQ(faulted.motorCmd, 0);

    state.motorFaultBits = 0u;
    const SteeringController::Output afterClear =
            ctrl.update(0.0, state, encoderFor(0.0), true, TICK_S);
    EXPECT_EQ(afterClear.motorCmd, 0)
            << "the command from before the fault came back through the hold";
}
