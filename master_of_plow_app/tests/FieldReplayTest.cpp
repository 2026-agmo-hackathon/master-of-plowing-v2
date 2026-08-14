// TEMPORARY tuning harness — not part of the skeleton deliverable.
// Replays a real field path (ENU csv via HL24_CSV) closed-loop and prints the
// same metrics measured on the emulator, so a gain choice can be validated
// against reality instead of guessed.
#include <gtest/gtest.h>

#include <AppMain/tracking/impl/PurePursuitTracker.hpp>
#include <AppMain/tracking/SampleClock.hpp>
#include <AppMain/tracking/SteeringController.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace AppMain::tracking;

namespace {

constexpr double TICK_S = 0.1;
// Conservative held-sample stress matching the pre-fix mux, not the current
// emulator contract (about 10 Hz per interface after the cadence fix).
constexpr double GPS_PERIOD_S = 1.0;
constexpr double WHEEL_SLEW_RAD_S = 0.26;
// 실측: 지령 오차가 10도를 넘고 나서 바퀴가 실제로 움직이기까지 중앙값 2초
// (run_v5 n=9, run_full n=103, 둘 다 중앙값 2s / p90 2-3s). 10Hz 루프에 20틱이므로
// 우리 PID가 아니라 플랜트(유압/모터) 특성입니다.
// Measured: 2 s median from a >10 deg demand error to the wheel actually moving,
// on both runs. That is 20 ticks of a 10 Hz loop, so it is the plant, not our PID.
constexpr double DEAD_TIME_S = 2.0;
constexpr double STEERING_RATIO = 24.2;
constexpr double HEADING_NOISE_RAD = 2.0 * M_PI / 180.0;
constexpr double POSITION_NOISE_M = 0.05;

class Noise
{
public:
    explicit Noise(std::uint64_t seed) : state_(seed) {}
    double gaussian(double sigma)
    {
        const double u1 = std::max(uniform(), 1e-12);
        const double u2 = uniform();
        return sigma * std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    }

private:
    double uniform()
    {
        state_ += 0x9E3779B97F4A7C15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        z = z ^ (z >> 31);
        return static_cast<double>(z >> 11) / 9007199254740992.0;
    }
    std::uint64_t state_;
};

std::vector<TrackerWaypoint> loadEnu(const std::string& path)
{
    std::vector<TrackerWaypoint> wps;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        for (char& c : line) {
            if (c == ',') {
                c = ' ';
            }
        }
        std::istringstream ss(line);
        double x = 0.0;
        double z = 0.0;
        double kmh = 0.0;
        int flag = 1;
        if (ss >> x >> z >> kmh >> flag) {
            wps.push_back({x, z, kmh, flag});
        }
    }
    return wps;
}

double percentile(std::vector<double> v, double p)
{
    if (v.empty()) {
        return 0.0;
    }
    std::sort(v.begin(), v.end());
    const std::size_t i = static_cast<std::size_t>(p * static_cast<double>(v.size() - 1));
    return v[i];
}

struct Report
{
    double satFrac = 0.0;
    double ltdMed = 0.0;
    double ltdP90 = 0.0;
    double ltdMax = 0.0;
    int signFlips = 0;
    double seconds = 0.0;
    double flipsPer100s = 0.0;
    double straightMed = 0.0;   // 경로가 곧은 곳에서의 오차 = 안정성 지표
    double straightP90 = 0.0;
    double cornerP90 = 0.0;     // 코너에서의 오차 = 코너 컷 정도
    int lastIdx = 0;
};

std::vector<TrackerWaypoint> constantRadiusArc(double radiusM, double spacingM,
                                                int implementFlag);
std::vector<TrackerWaypoint> genericSCurve(double spacingM);
std::vector<TrackerWaypoint> straightArcStraight(double spacingM);

// 경로 자체가 곧은 구간인지 판정합니다. 안정성(직선에서 흔들리는가)과
// 코너 컷(코너에서 벌어지는가)은 완전히 다른 문제이므로 나눠서 봅니다.
// Whether the path itself is straight here. Stability (weaving on a straight)
// and corner-cutting are different failures and must be scored separately.
std::vector<bool> straightMask(const std::vector<TrackerWaypoint>& w)
{
    std::vector<bool> s(w.size(), true);
    for (std::size_t i = 1; i + 1 < w.size(); ++i) {
        const double h0 = std::atan2(w[i].z - w[i - 1].z, w[i].x - w[i - 1].x);
        const double h1 = std::atan2(w[i + 1].z - w[i].z, w[i + 1].x - w[i].x);
        double d = h1 - h0;
        while (d > M_PI) {
            d -= 2.0 * M_PI;
        }
        while (d < -M_PI) {
            d += 2.0 * M_PI;
        }
        if (std::fabs(d) > 3.0 * M_PI / 180.0) {
            // 코너와 그 앞뒤 6개 점을 코너로 취급합니다 (선행거리만큼의 영향권).
            // Mark the corner and six waypoints either side — the lookahead reach.
            for (std::size_t k = (i > 6 ? i - 6 : 0);
                 k < std::min(w.size(), i + 7); ++k) {
                s[k] = false;
            }
        }
    }
    return s;
}

Report replay(const std::vector<TrackerWaypoint>& wps, const TrackerParams& params,
              double speedMps, double seconds, int timeScale = 1)
{
    PurePursuitTracker tracker;
    tracker.setParams(params);
    tracker.setTrack(wps);

    Noise noise(20260811);

    // 첫 두 점의 방향으로 정렬해서 시작합니다 — 에뮬레이터의 시작 상태와 같습니다.
    // Start aligned with the first segment, as the emulator run did.
    double x = wps[0].x;
    double z = wps[0].z;
    double heading = std::atan2(wps[1].z - wps[0].z, wps[1].x - wps[0].x);
    double wheelAngle = 0.0;

    double sampledX = x;
    double sampledZ = z;
    double sampledHeading = heading;
    double sinceSample = GPS_PERIOD_S;
    std::int64_t poseSampleId = 0;

    int saturated = 0;
    int ticks = 0;
    int flips = 0;
    double prevDemand = 0.0;
    std::vector<double> ltds;
    std::vector<double> straights;
    std::vector<double> corners;
    int lastIdx = 0;
    const std::vector<bool> mask = straightMask(wps);
    SteeringController steer;
    double reportedWheelRad = 0.0;
    double encoderAgeS = 1.0;
    std::int64_t encoderSampleId = 0;
    SampleClock sampleClock;
    std::int64_t imuTimestampMs = 10000;
    double imuWallAgeS = 0.0;
    double elapsedMachineS = 0.0;

    // Production observes the same interface while stopped and does not admit
    // motion until two consistent timestamp epochs establish the time domain.
    sampleClock.advance(imuTimestampMs, TICK_S);
    for (int epoch = 0; epoch < 2; ++epoch) {
        for (int held = 0; held < 9; ++held) {
            sampleClock.advance(imuTimestampMs, TICK_S);
        }
        imuTimestampMs += 1000 * timeScale;
        sampleClock.advance(imuTimestampMs, TICK_S);
    }
    EXPECT_TRUE(sampleClock.calibrated());
    EXPECT_NEAR(sampleClock.scale(), static_cast<double>(timeScale), 0.05);

    while (elapsedMachineS < seconds) {
        const double wallTickS = std::max(0.016,
                TICK_S / sampleClock.scale());
        // Exercise the production clock path with a slow (~1 Hz wall) FCAL
        // interface: nine held timestamps followed by one advancing frame.
        // Accelerated physics advances the authoritative timestamp by the same
        // multiplier; controller dt is never injected directly by the test.
        imuWallAgeS += wallTickS;
        if (imuWallAgeS >= 1.0 - 1e-9) {
            imuTimestampMs += 1000 * timeScale;
            imuWallAgeS -= 1.0;
        }
        const double machineTickS = sampleClock.advance(imuTimestampMs, wallTickS);
        elapsedMachineS += wallTickS * static_cast<double>(timeScale);
        ++ticks;
        sinceSample += machineTickS;
        if (sinceSample >= GPS_PERIOD_S - 1e-9) {
            sampledX = x + noise.gaussian(POSITION_NOISE_M);
            sampledZ = z + noise.gaussian(POSITION_NOISE_M);
            sampledHeading = heading + noise.gaussian(HEADING_NOISE_RAD);
            ++poseSampleId;
            sinceSample = 0.0;
        }

        VehicleState state;
        state.x = sampledX;
        state.z = sampledZ;
        state.headingRad = sampledHeading;
        state.speedMps = speedMps;
        state.poseSampleId = poseSampleId;

        const TrackerCommand cmd = tracker.update(state, machineTickS);

        if (std::fabs(cmd.steerAngle) >= PurePursuitTracker::WHEEL_MAX_RAD - 1e-9) {
            ++saturated;
        }
        // 지령이 한계에서 한계로 넘어간 횟수 = 릴레이 동작 횟수.
        // Limit-to-limit transitions of the demand = how often it acts as a relay.
        const double lim = PurePursuitTracker::WHEEL_MAX_RAD - 1e-9;
        if (cmd.steerAngle >= lim && prevDemand <= -lim) {
            ++flips;
        }
        if (cmd.steerAngle <= -lim && prevDemand >= lim) {
            ++flips;
        }
        if (std::fabs(cmd.steerAngle) >= lim) {
            prevDemand = cmd.steerAngle;
        }

        const double e = std::fabs(tracker.lastDebug().lateralDeviationM);
        const int ci = tracker.lastDebug().closestIdx;
        ltds.push_back(e);
        if (ci >= 0 && static_cast<std::size_t>(ci) < mask.size()) {
            (mask[static_cast<std::size_t>(ci)] ? straights : corners).push_back(e);
            lastIdx = std::max(lastIdx, ci);
        }

        // ── 실제 내루프 / the REAL inner loop ────────────────────────────
        // 이전 판은 여기서 이상화된 rate limiter 를 썼습니다. 그래서 같은
        // 파라미터로 직선 오차 0.08m 를 예측했지만 실기는 1.54m 였습니다.
        // 실제 SteeringController 를 넣고, 엔코더는 보수적인 수정 전 1Hz 조건으로 보고합니다.
        // The earlier version put an idealised rate limiter here and predicted
        // 0.08 m of straight-line error where the machine produced 1.54 m. This
        // runs the real SteeringController with the encoder held at the
        // conservative pre-fix 1 Hz cadence.
        if (encoderAgeS >= GPS_PERIOD_S - 1e-9) {
            reportedWheelRad = wheelAngle;
            ++encoderSampleId;
            encoderAgeS = 0.0;
        }
        encoderAgeS += machineTickS;

        VehicleState sst;
        sst.speedMps = speedMps;
        sst.yawRateValid = false;
        const SteeringController::Output so = steer.update(
                cmd.steerAngle, sst,
                reportedWheelRad * (180.0 / M_PI) * STEERING_RATIO, true,
                encoderSampleId, -1, machineTickS);

        // 모터 지령은 속도 지령입니다: 600 에서 약 15도/s (실측 p90 14.2-15.7).
        // The motor command is a velocity command: 600 gives about 15 deg/s.
        for (int substep = 0; substep < timeScale; ++substep) {
            wheelAngle += so.motorCmd * (15.0 / 600.0) * (M_PI / 180.0) * wallTickS;
            wheelAngle = std::clamp(wheelAngle,
                                    -PurePursuitTracker::WHEEL_MAX_RAD,
                                    PurePursuitTracker::WHEEL_MAX_RAD);
            const double yawRate = -speedMps * std::tan(wheelAngle)
                                 / PurePursuitTracker::WHEELBASE_M;
            heading += yawRate * wallTickS;
            x += speedMps * std::cos(heading) * wallTickS;
            z += speedMps * std::sin(heading) * wallTickS;
        }
    }

    Report r;
    r.satFrac = static_cast<double>(saturated) / static_cast<double>(ticks);
    r.ltdMed = percentile(ltds, 0.50);
    r.ltdP90 = percentile(ltds, 0.90);
    r.ltdMax = percentile(ltds, 1.00);
    r.signFlips = flips;
    r.seconds = seconds;
    r.flipsPer100s = 100.0 * static_cast<double>(flips) / seconds;
    r.straightMed = percentile(straights, 0.50);
    r.straightP90 = percentile(straights, 0.90);
    r.cornerP90 = percentile(corners, 0.90);
    r.lastIdx = lastIdx;
    return r;
}

TEST(FieldReplayTest, MachineTimeControlIsStableAcrossOneTwoFourAndSixTimes)
{
    TrackerParams params;
    for (const int scale : {1, 2, 4, 6}) {
        const std::vector<std::vector<TrackerWaypoint>> routes = {
            constantRadiusArc(5.0, 1.0, 0),
            constantRadiusArc(6.0, 1.0, 1),
            constantRadiusArc(10.0, 1.0, 0),
            constantRadiusArc(20.0, 1.0, 1),
            genericSCurve(1.0),
            straightArcStraight(1.0)
        };
        for (const auto& route : routes) {
            const Report r = replay(route, params, 2.4 / 3.6, 45.0, scale);
            EXPECT_LT(r.satFrac, 0.20) << "scale=" << scale;
            EXPECT_LE(r.signFlips, 1) << "scale=" << scale;
            EXPECT_LT(r.ltdP90, 1.2) << "scale=" << scale;
        }
    }
}

const char* csvPath()
{
    const char* p = std::getenv("HL24_CSV");
    return (p != nullptr && p[0] != '\0') ? p : nullptr;
}

std::vector<TrackerWaypoint> constantRadiusArc(double radiusM, double spacingM,
                                                int implementFlag)
{
    std::vector<TrackerWaypoint> waypoints;
    const double endAngle = 1.8;
    const int count = static_cast<int>(std::ceil(radiusM * endAngle / spacingM));
    for (int i = 0; i <= count; ++i) {
        const double angle = endAngle * static_cast<double>(i) / count;
        waypoints.push_back({radiusM * std::sin(angle),
                             radiusM * (1.0 - std::cos(angle)),
                             2.4, implementFlag});
    }
    return waypoints;
}

std::vector<TrackerWaypoint> genericSCurve(double spacingM)
{
    std::vector<TrackerWaypoint> waypoints;
    for (double x = 0.0; x <= 70.0; x += spacingM) {
        waypoints.push_back({x, 2.0 * std::sin(x / 12.0), 2.4, 1});
    }
    return waypoints;
}

std::vector<TrackerWaypoint> straightArcStraight(double spacingM)
{
    std::vector<TrackerWaypoint> waypoints;
    for (double x = 0.0; x < 15.0; x += spacingM) {
        waypoints.push_back({x, 0.0, 2.4, 1});
    }
    constexpr double radius = 10.0;
    const int arcCount = static_cast<int>(std::ceil(radius * (M_PI / 2.0) / spacingM));
    for (int i = 0; i <= arcCount; ++i) {
        const double angle = (M_PI / 2.0) * static_cast<double>(i) / arcCount;
        waypoints.push_back({15.0 + radius * std::sin(angle),
                             radius * (1.0 - std::cos(angle)), 2.4, 1});
    }
    for (double z = radius + spacingM; z <= radius + 20.0; z += spacingM) {
        waypoints.push_back({15.0 + radius, z, 2.4, 1});
    }
    return waypoints;
}

}  // namespace

TEST(FieldReplayTest, GenericValidGeometryMatrixHasNoLimitCycle)
{
    TrackerParams params;
    for (double spacingM : {0.5, 1.0, 1.5}) {
        for (double radiusM : {5.0, 6.0, 10.0, 20.0}) {
            const auto path = constantRadiusArc(radiusM, spacingM, 1);
            const double seconds = 0.75 * radiusM * 1.8 / (2.4 / 3.6);
            const Report r = replay(path, params, 2.4 / 3.6, seconds);
            EXPECT_LT(r.satFrac, 0.10) << "radius=" << radiusM
                                       << " spacing=" << spacingM;
            EXPECT_EQ(r.signFlips, 0) << "radius=" << radiusM
                                      << " spacing=" << spacingM;
            EXPECT_LT(r.cornerP90, 0.8) << "radius=" << radiusM
                                        << " spacing=" << spacingM;
        }
    }

    const Report s = replay(genericSCurve(1.0), params, 2.4 / 3.6, 75.0);
    EXPECT_LT(s.satFrac, 0.05);
    EXPECT_EQ(s.signFlips, 0);
    EXPECT_LT(s.ltdP90, 0.6);

    const Report transition = replay(
            straightArcStraight(1.0), params, 2.4 / 3.6, 55.0);
    EXPECT_LT(transition.satFrac, 0.05);
    EXPECT_EQ(transition.signFlips, 0);
    // With the deliberately harsh 2 s steering dead time and 1 Hz pose/encoder
    // samples, the entry transient is expected; it must remain bounded below 1 m.
    EXPECT_LT(transition.ltdP90, 1.0);
}

// 현재 기본값이 에뮬레이터 실측(포화 72.5%, 릴레이 반전 8.8회/100s, |ltd| 중앙 1.90 m,
// p90 3.12 m)을 재현하는지 확인합니다. 재현되지 않으면 이 하네스로 튜닝하면 안 됩니다.
TEST(FieldReplayTest, DISABLED_Baseline)
{
    const char* p = csvPath();
    if (p == nullptr) {
#ifdef GTEST_SKIP
        GTEST_SKIP() << "set HL24_CSV to an ENU csv (x,z,speedKmh,flag) to run";
#else
        return;  // GoogleTest 1.8 on the target-compatible Buster toolchain.
#endif
    }
    const auto wps = loadEnu(p);
    ASSERT_GT(wps.size(), 100u);

    TrackerParams params;
    const Report r = replay(wps, params, 1.08, 102.0);
    std::printf("BASELINE sat=%.1f%% flips/100s=%.1f ltd med=%.2f p90=%.2f max=%.2f\n",
                100.0 * r.satFrac, r.flipsPer100s, r.ltdMed, r.ltdP90, r.ltdMax);
}

TEST(FieldReplayTest, DISABLED_Sweep)
{
    const char* p = csvPath();
    if (p == nullptr) {
#ifdef GTEST_SKIP
        GTEST_SKIP() << "set HL24_CSV to an ENU csv (x,z,speedKmh,flag) to run";
#else
        return;  // GoogleTest 1.8 on the target-compatible Buster toolchain.
#endif
    }
    const auto wps = loadEnu(p);
    ASSERT_GT(wps.size(), 100u);

    // 전체 경로(478점, 약 500m)를 두 속도로 완주시킵니다. 기계가 실제로 내는
    // 속도는 {0.57, 1.08} m/s 두 개뿐이라 그 둘만 봅니다.
    // Drive the whole 478-waypoint path at the only two speeds the machine
    // actually achieves.
    std::printf("%6s %5s %5s %5s %5s | %6s %7s %8s %8s %8s %8s\n",
                "lkMin", "wPp", "kLtd", "kHde", "kYaw",
                "sat%", "flip100", "strMed", "strP90", "crnP90", "max");
    for (double v : {1.08, 0.57}) {
        const double secs = 1.15 * 500.0 / v;
        std::printf("--- %.2f m/s ---\n", v);
        // 선행"시간"을 루프 총 지연(GPS 0.5s + 불감 2s + 슬루 ~2s ≈ 4.5s)보다
        // 충분히 크게 잡는 것이 원리입니다. 그러면 선행거리가 속도에 따라 저절로
        // 늘고 줄어, 저속 코너와 고속 직선을 한 설정으로 모두 커버합니다.
        // The lookahead TIME must exceed the loop's total lag; then the distance
        // scales with speed on its own and one setting covers both cases.
        for (double lkT : {2.5, 6.0, 8.0, 9.0, 11.0}) {
            for (double wPp : {1.0, 0.7, 0.5}) {
                TrackerParams params;
                params.ppLookaheadTimeS = lkT;
                params.ppLookaheadMinM = 5.0;
                params.ppLookaheadMaxM = 12.0;
                params.ppWeightPp = wPp;
                const Report r = replay(wps, params, v, secs);
                std::printf("lkT=%4.1f (Ld=%4.1fm) %5.2f %5.2f %5.2f %5.2f | %6.1f %7.1f"
                            " %8.2f %8.2f %8.2f %8.2f  reached=%d/%d\n",
                            lkT, std::clamp(v * lkT, 5.0, 12.0), wPp, params.ppKLtd,
                            params.ppKHde, params.ppKYawDamp,
                            100.0 * r.satFrac, r.flipsPer100s,
                            r.straightMed, r.straightP90, r.cornerP90,
                            r.ltdMax, r.lastIdx, static_cast<int>(wps.size()));
            }
        }
    }
}
