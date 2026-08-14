#include <gtest/gtest.h>

#include <AppMain/tracking/impl/PurePursuitTracker.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace AppMain::tracking;

// ┌─ 한국어: 이 파일이 무엇을 지키는가 ────────────────────────────────────────┐
//
// 나머지 트래커 테스트는 한 틱씩 열어 보는 단위 테스트입니다. 그것만으로는
// "조향이 계속 물리 한계에 붙어 릴레이처럼 동작한다"를 잡을 수 없습니다. 한 틱만
// 보면 각 항은 전부 정상 범위이고, 문제는 폐루프를 여러 바퀴 돌린 뒤에야
// 드러나기 때문입니다.
//
// 그래서 여기서는 자전거 모델 플랜트를 붙여 폐루프로 돌립니다. 에뮬레이터의
// 까다로운 조건 세 가지를 일부러 그대로 재현합니다.
//
//   (1) 수정 전 mux 조건을 보수적으로 재현해 10Hz 제어 루프에서 자세를 1Hz로
//       홀드합니다. 현재 수정된 에뮬레이터 인터페이스는 약 10Hz입니다.
//   (2) 방위에 잡음이 있습니다. NMEA course-over-ground는 저속에서 특히 지저분합니다.
//   (3) 조향 액추에이터는 슬루율이 제한돼 있습니다 — 큰 조향일수록 오래 걸립니다.
//
// ★ 핵심 지표는 "조향 지령이 앞바퀴 물리 한계에 붙어 있는 시간 비율"입니다.
// 이 값이 높다는 것은 제어기가 낼 수 없는 각도를 계속 요구하고 있다는 뜻이고,
// 그러면 조향은 사실상 on/off 릴레이가 됩니다. 릴레이의 리밋 사이클이 곧 S자
// 사행입니다. 에뮬레이터 실측에서 이 값이 주행 GPS 샘플의 92%였습니다.
//
// 게인을 바꾸거나 조향 항을 추가할 때 이 테스트가 깨지면, 새 설정이 액추에이터가
// 낼 수 있는 범위를 넘어섰다는 뜻입니다. 횡편차만 보고 판단하지 마세요 —
// 포화 상태에서도 잠깐은 잘 따라가는 것처럼 보입니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
//
// ┌─ English: what this file protects ─────────────────────────────────────────┐
//
// The other tracker tests are single-tick unit tests. They cannot catch "the
// steering demand sits at the physical limit and behaves as a relay", because on
// any single tick every term looks reasonable; the failure only appears after the
// closed loop has gone around several times.
//
// So this file attaches a bicycle-model plant and closes the loop, deliberately
// reproducing the three awkward properties of the emulator:
//
//   (1) pose is deliberately held at the harsher pre-fix 1 Hz cadence while the
//       control loop ticks at 10 Hz; corrected emulator interfaces are about 10 Hz;
//   (2) heading is noisy — NMEA course-over-ground is poor at low speed;
//   (3) the steering actuator is rate-limited, so a bigger move takes longer.
//
// The headline metric is the fraction of ticks whose steering demand sits at the
// physical front-wheel limit. A high value means the controller keeps
// asking for angles the wheels cannot make, which turns steering into an on/off
// relay; that relay's limit cycle is the S-weave. Measured on the emulator, this
// was 92% of moving GPS samples.
//
// If this test breaks after a gain change or a new steering term, the new setup
// demands more than the actuator can deliver. Do not judge by cross-track error
// alone — a saturated loop still tracks acceptably for short stretches.
//
// └────────────────────────────────────────────────────────────────────────────┘

namespace {

// 플랜트의 물리 한계는 트래커와 같은 상수를 씁니다. 따로 적어 두면 트래커의
// 한계를 고쳤을 때 플랜트만 예전 값으로 남아, 지령이 도달 불가능해도 테스트는
// 통과해 버립니다.
// The plant's limit comes from the tracker's own constant. Writing it out again
// would leave the plant on the old value after the tracker's limit changes, and
// the test would pass while the demand is unreachable.
constexpr double WHEEL_MAX_RAD = PurePursuitTracker::WHEEL_MAX_RAD;
constexpr double WHEELBASE_M = PurePursuitTracker::WHEELBASE_M;
constexpr double TICK_S = 0.1;           // IPathTracker::preferredPeriodMs()
constexpr double GPS_PERIOD_S = 1.0;     // 자세의 유일한 출처 / the only pose source

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 조향 액추에이터는 1차 지연이 아니라 슬루율 제한입니다. 이 구분이 중요합니다.
//
// 1차 지연(예전 모델, tau=0.3s)은 걸린 시간이 이동량과 무관합니다 — 5도를 가든
// 50도를 가든 0.9초면 95%에 도달합니다. 실제 유압 조향은 모터가 최대로 물려도
// 초당 정해진 각도밖에 못 갑니다. 큰 조향일수록 오래 걸립니다.
//
// 이 차이가 예측을 통째로 틀리게 만듭니다. 1차 지연 모델에서는 지령이 포화해도
// 바퀴가 금방 따라잡으니 폐루프가 멀쩡해 보입니다. 실제로는 지령이 부호를 뒤집을
// 때마다 바퀴가 4초 넘게 이동 중이고, 그동안 차량은 4m 이상 진행하며 경로를
// 가로질러 반대편으로 넘어갑니다 — S자 사행 그 자체입니다.
// 실제로 예전 모델은 새 게인에서 포화 2% 미만을 예측했지만 에뮬레이터 실측은
// 58%였습니다.
//
// 실측(1,127초 주행): 모터가 ±600에 물린 채 바퀴가 실제로 움직이는 1초 구간
// 4,504개에서 p90 = 14.2°/s, p99 = 17.7°/s. 15°/s = 0.26 rad/s 를 씁니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The steering actuator is rate-limited, not first-order-lagged, and the
// difference matters.
//
// A first-order lag (the previous model, tau = 0.3 s) takes the same time
// whatever the distance — 5 deg or 50 deg both reach 95% in 0.9 s. Real
// hydrostatic steering covers a fixed number of degrees per second at full motor
// command, so a big move takes proportionally longer.
//
// That difference invalidates the prediction entirely. Under a lag model the
// wheels catch up quickly even when the demand saturates, so the closed loop
// looks healthy. In reality every sign flip of the demand leaves the wheels in
// transit for over 4 s, and the vehicle covers more than 4 m in that time,
// crossing the path and coming out the other side — the S-weave itself. The old
// model predicted under 2% saturation for the current gains where the emulator
// measured 58%.
//
// Measured over a 1,127 s run: across 4,504 one-second windows with the motor
// pinned at +/-600 and the wheel actually moving, p90 = 14.2 deg/s and
// p99 = 17.7 deg/s. 15 deg/s = 0.26 rad/s is used here.
// └────────────────────────────────────────────────────────────────────────────┘
constexpr double WHEEL_SLEW_RAD_S = 0.26;

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 조향에는 불감시간이 있습니다. 지령이 10도 넘게 어긋난 뒤 바퀴가 실제로 움직이기
// 시작할 때까지 중앙값 2초입니다 (에뮬레이터 두 주행 모두: n=9 / n=103, 중앙 2s,
// p90 2~3s). 10Hz 루프에서 20틱이므로 우리 PID가 아니라 유압/모터 특성입니다.
//
// ★ 이 항이 빠지면 이 파일은 예측력이 없습니다. 실제로 슬루율만 넣고 불감시간을
//   빼면 같은 게인에서 포화 6.5%/사행 0회가 나오는데, 에뮬레이터 실측은
//   포화 72.5%/좌우 풀락 반전 8.8회/100초였습니다. 불감시간을 넣자마자
//   67.8% / 8.8회로 맞았습니다. 지연 없는 플랜트는 어떤 게인도 안정해 보입니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The steering has a dead time: a median 2 s from a >10 deg demand error to the
// wheel actually starting to move (both emulator runs, n=9 and n=103, median 2 s,
// p90 2-3 s). That is 20 ticks of a 10 Hz loop, so it is the hydraulics rather
// than our PID.
//
// Without this term the file has no predictive power. With the slew limit but no
// dead time the same gains score 6.5% saturation and zero weave, where the
// emulator measured 72.5% and 8.8 lock-to-lock reversals per 100 s; adding the
// dead time reproduced it at 67.8% and 8.8. A lag-free plant makes every gain
// setting look stable.
// └────────────────────────────────────────────────────────────────────────────┘
constexpr double STEER_DEAD_TIME_S = 2.0;

constexpr double HEADING_NOISE_RAD = 2.0 * M_PI / 180.0;
constexpr double POSITION_NOISE_M = 0.05;

// 툴체인이 달라도 같은 숫자가 나와야 하므로 난수를 직접 굴립니다. <random>의
// 분포 구현은 표준이 결과를 규정하지 않아 stdlib마다 다른 값이 나옵니다.
// Rolled by hand so every toolchain sees identical numbers: the standard does not
// pin down the <random> distribution implementations, so they differ per stdlib.
class Noise
{
public:
    explicit Noise(std::uint64_t seed) : state_(seed) {}

    double gaussian(double sigma)
    {
        // Box-Muller. 두 번째 표본은 버립니다 — 짧은 코드가 낫습니다.
        // Box-Muller; the second sample is discarded in favour of shorter code.
        const double u1 = std::max(uniform(), 1e-12);
        const double u2 = uniform();
        return sigma * std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    }

private:
    double uniform()
    {
        // splitmix64
        state_ += 0x9E3779B97F4A7C15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        z = z ^ (z >> 31);
        return static_cast<double>(z >> 11) / 9007199254740992.0;  // [0,1)
    }

    std::uint64_t state_;
};

struct LoopResult
{
    double saturatedFraction = 0.0;  // |조향 지령| 이 물리 한계인 틱의 비율
    double meanAbsCrossTrack = 0.0;  // 정착 구간 / settled window
    double maxAbsCrossTrack = 0.0;
};

// +x 방향 직선 경로. 부호 규약은 PurePursuitTest.cpp 상단 주석과 같습니다.
// Straight path along +x; sign conventions as documented atop PurePursuitTest.cpp.
std::vector<TrackerWaypoint> straightLane(int count, double speedKmh)
{
    std::vector<TrackerWaypoint> wps;
    wps.reserve(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        wps.push_back({static_cast<double>(i), 0.0, speedKmh, 1});
    }
    return wps;
}

/**
 * 자전거 모델 플랜트로 트래커를 폐루프 주행시킵니다.
 *
 * 부호 규약: 조향 > 0 은 좌회전이고, 좌회전은 heading 을 "감소"시킵니다
 * (+x = 동, +z = 남, heading = atan2(z, x) 인 왼손 좌표계).
 *
 * Drives the tracker closed-loop against a bicycle-model plant. Sign convention:
 * a positive steer is a LEFT turn and a left turn DECREASES heading, in the
 * left-handed frame where +x = East, +z = South and heading = atan2(z, x).
 */
LoopResult driveStraightLane(PurePursuitTracker& tracker, double speedMps,
                             double startOffsetM, double startHeadingRad,
                             double durationS, std::uint64_t seed = 12345)
{
    const double speedKmh = speedMps * 3.6;
    const auto waypoints = straightLane(400, speedKmh);
    tracker.setTrack(waypoints);

    Noise noise(seed);

    double x = 0.0;
    double z = startOffsetM;
    double heading = startHeadingRad;
    double wheelAngle = 0.0;  // 액추에이터가 실제로 도달한 각 / where the wheels are

    // 트래커가 보는 자세는 1Hz로만 갱신됩니다.
    // The pose the tracker sees only refreshes at 1 Hz.
    double sampledX = x;
    double sampledZ = z;
    double sampledHeading = heading;
    double sinceSample = GPS_PERIOD_S;

    const int ticks = static_cast<int>(durationS / TICK_S);
    int saturated = 0;
    std::vector<double> crossTrack;
    crossTrack.reserve(static_cast<std::size_t>(ticks));

    // 조향 불감시간을 담는 지연선 / delay line carrying the steering dead time
    std::vector<double> deadTime(
            static_cast<std::size_t>(STEER_DEAD_TIME_S / TICK_S), 0.0);
    std::size_t deadHead = 0;

    for (int i = 0; i < ticks; ++i) {
        sinceSample += TICK_S;
        if (sinceSample >= GPS_PERIOD_S - 1e-9) {
            sampledX = x + noise.gaussian(POSITION_NOISE_M);
            sampledZ = z + noise.gaussian(POSITION_NOISE_M);
            sampledHeading = heading + noise.gaussian(HEADING_NOISE_RAD);
            sinceSample = 0.0;
        }

        VehicleState state;
        state.x = sampledX;
        state.z = sampledZ;
        state.headingRad = sampledHeading;
        state.speedMps = speedMps;

        const TrackerCommand cmd = tracker.update(state, TICK_S);

        if (std::fabs(cmd.steerAngle) >= WHEEL_MAX_RAD - 1e-9) {
            ++saturated;
        }

        // 불감시간: 지령은 이만큼 지난 뒤에야 액추에이터에 도달합니다.
        // Dead time: the demand only reaches the actuator this much later.
        const double delayed = deadTime[deadHead];
        deadTime[deadHead] = cmd.steerAngle;
        deadHead = (deadHead + 1) % deadTime.size();

        // 슬루율 제한 액추에이터. 한 틱에 갈 수 있는 각도가 정해져 있습니다.
        // Rate-limited actuator: a fixed angle per tick, no more.
        const double step = std::clamp(delayed - wheelAngle,
                                       -WHEEL_SLEW_RAD_S * TICK_S,
                                        WHEEL_SLEW_RAD_S * TICK_S);
        wheelAngle = std::clamp(wheelAngle + step, -WHEEL_MAX_RAD, WHEEL_MAX_RAD);

        const double yawRate = -speedMps * std::tan(wheelAngle) / WHEELBASE_M;
        heading += yawRate * TICK_S;
        x += speedMps * std::cos(heading) * TICK_S;
        z += speedMps * std::sin(heading) * TICK_S;

        // 경로가 z = 0 위의 직선이므로 횡편차는 |z| 입니다.
        // The path is the line z = 0, so cross-track error is |z|.
        crossTrack.push_back(std::fabs(z));
    }

    LoopResult r;
    r.saturatedFraction = static_cast<double>(saturated) / static_cast<double>(ticks);

    // 정착 구간만 봅니다. 앞부분은 초기 오차를 되돌리는 과도 구간입니다.
    // Only the settled window; the first half is the transient that pulls the
    // deliberate initial error out.
    const std::size_t half = crossTrack.size() / 2;
    double sum = 0.0;
    for (std::size_t i = half; i < crossTrack.size(); ++i) {
        sum += crossTrack[i];
        r.maxAbsCrossTrack = std::max(r.maxAbsCrossTrack, crossTrack[i]);
    }
    r.meanAbsCrossTrack = sum / static_cast<double>(crossTrack.size() - half);
    return r;
}

} // namespace

// ── 회귀 방지 / The regression guards ────────────────────────────────────────

TEST(ClosedLoopTrackingTest, SteeringDemandDoesNotSitAtThePhysicalLimit)
{
    // 2m 벗어난 채 20도 틀어진 상태에서 출발합니다. 되돌리는 동안 잠깐 한계를
    // 치는 것은 정상이지만, 계속 붙어 있으면 조향이 릴레이가 된 것입니다.
    // Starts 2 m off the path and 20 deg misaligned. Touching the limit while
    // recovering is fine; staying there means steering has become a relay.
    for (double speedMps : {0.556, 1.111, 2.222}) {  // 2, 4, 8 km/h
        PurePursuitTracker tracker;
        const LoopResult r = driveStraightLane(tracker, speedMps, 2.0,
                                               20.0 * M_PI / 180.0, 180.0);
        EXPECT_LT(r.saturatedFraction, 0.10)
                << "steering demand pinned at the wheel limit on "
                << (100.0 * r.saturatedFraction) << "% of ticks at "
                << (speedMps * 3.6) << " km/h. The controller is asking for "
                   "angles the wheels cannot make, so steering is a relay and "
                   "the vehicle will weave.";
    }
}

TEST(ClosedLoopTrackingTest, ConvergesOntoTheLaneAndStaysThere)
{
    PurePursuitTracker tracker;
    const LoopResult r = driveStraightLane(tracker, 1.111, 2.0,
                                           20.0 * M_PI / 180.0, 180.0);
    EXPECT_LT(r.meanAbsCrossTrack, 0.50)
            << "mean settled cross-track error " << r.meanAbsCrossTrack << " m";
    EXPECT_LT(r.maxAbsCrossTrack, 1.00)
            << "worst settled cross-track error " << r.maxAbsCrossTrack << " m";
}

TEST(ClosedLoopTrackingTest, TrackingDoesNotCollapseAsSpeedRises)
{
    // 속도를 두 배로 올려도 추종이 무너지면 안 됩니다. 예전 ltdTerm 은 속도
    // 정규화가 없어서 2 -> 4km/h 에서 횡편차 평균이 0.78m -> 3.49m 가 됐습니다.
    // Doubling speed must not collapse tracking. The old ltdTerm had no speed
    // normalisation, and 2 -> 4 km/h took mean cross-track from 0.78 to 3.49 m.
    PurePursuitTracker slow;
    PurePursuitTracker fast;
    const LoopResult a = driveStraightLane(slow, 1.111, 2.0, 20.0 * M_PI / 180.0, 180.0);
    const LoopResult b = driveStraightLane(fast, 2.222, 2.0, 20.0 * M_PI / 180.0, 180.0);
    EXPECT_LT(b.meanAbsCrossTrack, 4.0 * std::max(a.meanAbsCrossTrack, 0.02))
            << "cross-track went from " << a.meanAbsCrossTrack << " m at 4 km/h to "
            << b.meanAbsCrossTrack << " m at 8 km/h";
}

TEST(ClosedLoopTrackingTest, YawDampingUsesTheMeasurementIntervalNotTheTick)
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 수정 전 mux 조건을 재현해 방위를 1Hz로 홀드하고 제어 루프는 10Hz로 둡니다.
    // 제어 주기(0.1s)로 나누면 1초치 변화량을 0.1초로 나누게
    // 되어 정확히 10배가 됩니다.
    //
    // 감쇠항만 떼어 재려고 같은 입력을 두 트래커에 흘립니다. 하나는
    // ppKYawDamp = 0, 다른 하나는 K. 두 지령의 차가 곧 -K * yawRate 이므로,
    // 거기서 트래커가 실제로 본 yaw rate 를 역산할 수 있습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // This regression holds heading at the pre-fix 1 Hz cadence while the loop
    // ticks at 10 Hz, so dividing by the control period
    // spreads a second of change over 0.1 s — exactly 10x too much.
    //
    // To isolate the damping term, the same inputs go to two trackers, one with
    // ppKYawDamp = 0 and one with K. The difference between their demands is
    // -K * yawRate, which recovers the yaw rate the tracker actually saw.
    // └────────────────────────────────────────────────────────────────────────┘
    constexpr double K = 0.3;
    constexpr double STEP_RAD = 5.0 * M_PI / 180.0;
    constexpr int HOLD_TICKS = 10;  // 방위가 1초 동안 그대로 / heading held 1 s

    auto demandAfterHeadingStep = [](double yawDampGain) {
        PurePursuitTracker tracker;
        TrackerParams params;           // 나머지는 전부 기본값 / everything else default
        params.ppKYawDamp = yawDampGain;
        tracker.setParams(params);
        tracker.setTrack(straightLane(400, 4.0));

        VehicleState state;
        state.x = 5.0;
        state.z = 0.0;
        state.headingRad = 0.0;
        state.speedMps = 1.111;

        // 방위를 1초 동안 고정한 뒤 한 번에 STEP_RAD 만큼 움직입니다. 실제 회전
        // 각속도는 STEP_RAD / 1초 입니다.
        // Hold the heading for a second, then move it by STEP_RAD in one go. The
        // true turn rate is STEP_RAD per second.
        for (int i = 0; i < HOLD_TICKS; ++i) {
            tracker.update(state, TICK_S);
        }
        state.headingRad = STEP_RAD;
        return tracker.update(state, TICK_S).steerAngle;
    };

    const double undamped = demandAfterHeadingStep(0.0);
    const double damped = demandAfterHeadingStep(K);

    // damped - undamped == -K * yawRate  =>  yawRate == (undamped - damped) / K
    const double impliedYawRate = (undamped - damped) / K;
    const double trueYawRate = STEP_RAD / (HOLD_TICKS * TICK_S);

    EXPECT_NEAR(std::fabs(impliedYawRate), std::fabs(trueYawRate),
                0.1 * std::fabs(trueYawRate))
            << "the damping term implies a yaw rate of " << impliedYawRate
            << " rad/s where the heading actually moved at " << trueYawRate
            << " rad/s. Dividing the heading change by the control period instead "
               "of the time since the last GPS fix overstates it "
            << HOLD_TICKS << "x, which slams the demand to full lock once per second.";
}

TEST(ClosedLoopTrackingTest, YawDampingOpposesTheTurnInsteadOfFeedingIt)
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 감쇠항의 부호 회귀 테스트. 이 좌표계는 왼손계라서 조향 양수 = 좌회전이고
    // 좌회전은 방위를 "줄입니다". 교과서(오른손계)의 -k*yawRate 를 그대로 옮겨
    // 오면 부호가 뒤집혀, 이미 돌고 있는 쪽으로 더 꺾는 양의 피드백이 됩니다.
    //
    // 재는 법: 차량이 좌회전 중(방위 감소, yawRate < 0)일 때 감쇠항은 우조향
    // (음수)을 더해야 합니다. ppKYawDamp 만 다른 두 트래커에 같은 입력을 흘려
    // 감쇠항만 떼어 냅니다.
    //
    // 에뮬레이터 실측: 뒤집힌 부호에서 감쇠항은 이동 중 GPS 1,060샘플 전부에서
    // 이미 걸려 있던 조향각과 같은 방향이었습니다 — 감쇠기가 아니라 증폭기.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Sign regression for the damping term. This frame is left-handed: a positive
    // steer is a LEFT turn and a left turn DECREASES heading, so the textbook
    // (right-handed) -k*yawRate arrives with its sign inverted and becomes
    // positive feedback that steers further into the turn already happening.
    //
    // The measurement: while the vehicle turns left (heading falling, yawRate
    // negative) the damper must contribute RIGHT steer, i.e. a negative angle.
    // Two trackers differing only in ppKYawDamp isolate the term.
    //
    // Measured on the emulator, the inverted sign pointed the same way as the
    // angle already on the wheels on all 1,060 moving GPS samples — an amplifier,
    // not a damper.
    // └────────────────────────────────────────────────────────────────────────┘
    constexpr double K = 0.3;
    constexpr double STEP_RAD = 5.0 * M_PI / 180.0;
    constexpr int HOLD_TICKS = 10;

    // headingDelta < 0 이면 좌회전, > 0 이면 우회전.
    // A negative headingDelta is a left turn, a positive one a right turn.
    auto dampingContribution = [](double headingDelta) {
        auto demand = [headingDelta](double yawDampGain) {
            PurePursuitTracker tracker;
            TrackerParams params;
            params.ppKYawDamp = yawDampGain;
            tracker.setParams(params);
            tracker.setTrack(straightLane(400, 4.0));

            VehicleState state;
            state.x = 5.0;
            state.z = 0.0;
            state.headingRad = 0.0;
            state.speedMps = 1.111;

            for (int i = 0; i < HOLD_TICKS; ++i) {
                tracker.update(state, TICK_S);
            }
            state.headingRad = headingDelta;
            return tracker.update(state, TICK_S).steerAngle;
        };
        return demand(K) - demand(0.0);
    };

    const double turningLeft = dampingContribution(-STEP_RAD);
    const double turningRight = dampingContribution(STEP_RAD);

    EXPECT_LT(turningLeft, 0.0)
            << "the vehicle is turning LEFT and the damping term contributes "
            << turningLeft << " rad. A damper has to push the other way, so this "
               "must be negative (right steer). A positive value means the sign "
               "was copied from the right-handed textbook form and the term is "
               "feeding the turn instead of damping it.";
    EXPECT_GT(turningRight, 0.0)
            << "the vehicle is turning RIGHT and the damping term contributes "
            << turningRight << " rad, which must be positive (left steer).";
}
