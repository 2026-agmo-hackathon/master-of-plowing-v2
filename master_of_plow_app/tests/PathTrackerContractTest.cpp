// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
//
// 등록된 모든 트래커가 반드시 만족해야 하는 "계약 테스트"입니다.
//
// 이 파일은 일부러 IPathTracker와 팩토리만 사용합니다(구체 클래스 이름이 한 번도
// 나오지 않습니다). 새 알고리즘이 여기를 통과하면 TrackingLoop이 그 알고리즘을
// 문제없이 굴릴 수 있다는 뜻입니다.
//
// ★ TrackerFactory.cpp에 구현을 한 줄 추가하면, 이 테스트들이 자동으로 그
//   구현까지 검사합니다. 테스트를 따로 작성할 필요가 없습니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
//
// ┌─ English ──────────────────────────────────────────────────────────────────┐
//
// Contract tests that every registered tracker has to satisfy.
//
// These deliberately touch nothing but IPathTracker and the factory: if a new
// algorithm passes here, TrackingLoop can drive it. Add an implementation to
// TrackerFactory.cpp and it is covered automatically.
//
// └────────────────────────────────────────────────────────────────────────────┘

#include <gtest/gtest.h>

#include <AppMain/tracking/TrackerFactory.hpp>

#include <cmath>
#include <set>
#include <vector>

using namespace AppMain::tracking;

namespace {

constexpr double WHEELBASE_M = 2.05;
constexpr double DT = 0.1;

std::vector<TrackerWaypoint> straightAlongX(int count)
{
    std::vector<TrackerWaypoint> wps;
    for (int i = 0; i < count; ++i) {
        wps.push_back({static_cast<double>(i), 0.0, 4.0, 1});
    }
    return wps;
}

VehicleState poseAt(double x, double z, double headingRad, double speedMps)
{
    VehicleState s;
    s.x = x;
    s.z = z;
    s.headingRad = headingRad;
    s.speedMps = speedMps;
    return s;
}

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * 자전거 운동학 모델을 플랜트로 써서 폐루프로 주행시켜 봅니다. 트래커가 낸
 * 명령으로 차량을 실제로 움직이므로, 한 주기만 보는 테스트와 달리 인덱스 관리와
 * 완주 판정까지 통째로 검사됩니다.
 *
 * 완주까지 걸린 주기 수를 돌려주고, 끝까지 완주를 알리지 않으면 -1을 돌려줍니다.
 *
 * heading 적분에 마이너스가 붙은 것에 주의하세요. 조향 양수는 좌회전이고, 이
 * 좌표계에서 좌회전은 atan2(z, x)를 감소시킵니다.
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Closed-loop drive with a kinematic bicycle plant. Returns the tick count, or
 * -1 if the tracker never reported completion.
 *
 * Note the minus on the heading integration: positive steer is a left turn,
 * which decreases atan2(z, x) in this frame.
 * └────────────────────────────────────────────────────────────────────────────┘
 */
int driveToCompletion(IPathTracker& tracker,
                      const std::vector<TrackerWaypoint>& track,
                      int maxTicks = 4000)
{
    tracker.setTrack(track);
    double x = track.front().x;
    double z = track.front().z;
    double heading = 0.0;
    double speed = 0.0;

    for (int tick = 0; tick < maxTicks; ++tick) {
        const TrackerCommand cmd =
            tracker.update(poseAt(x, z, heading, speed), DT);
        if (tracker.isTrackComplete()) {
            return tick;
        }
        // 플랜트의 조향 한계. 이 파일은 트래커 구현에 의존하지 않는 계약
        // 테스트라 특정 트래커의 상수를 참조하지 않습니다. 실측 물리 한계는
        // ±0.44 rad 이며 여기서는 여유를 두고 잘라 어떤 트래커든 통과합니다.
        // The plant's steering limit. This is a tracker-agnostic contract test,
        // so it does not reach into any one tracker's constants; the measured
        // physical limit is +/-0.44 rad and this clips loosely so that any
        // tracker can satisfy the contract.
        constexpr double PLANT_STEER_LIMIT_RAD = 0.56;
        const double steer = std::fmax(-PLANT_STEER_LIMIT_RAD,
                             std::fmin(PLANT_STEER_LIMIT_RAD, cmd.steerAngle));
        const int dir = (cmd.gear < 0) ? -1 : 1;
        speed += (3.0 * cmd.throttle - 4.0 * cmd.brake - 0.2) * DT;
        speed = std::fmax(0.0, std::fmin(4.0, speed));
        const double ds = dir * speed * DT;
        heading -= (ds / WHEELBASE_M) * std::tan(steer);
        x += ds * std::cos(heading);
        z += ds * std::sin(heading);
    }
    return -1;
}

} // namespace

TEST(PathTrackerContractTest, RegistryIsNotEmpty) {
    EXPECT_FALSE(availableTrackerNames().empty());
}

TEST(PathTrackerContractTest, UnknownNameFallsBackInsteadOfReturningNull) {
    auto tracker = createTracker("no_such_tracker");
    ASSERT_TRUE(tracker != nullptr);
    EXPECT_STREQ(tracker->name(), "pure_pursuit");
}

TEST(PathTrackerContractTest, FactoryNameMatchesInstanceName) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        EXPECT_EQ(registered, std::string(tracker->name()));
    }
}

TEST(PathTrackerContractTest, EmptyTrackIsInertRatherThanCrashing) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        tracker->setTrack({});

        EXPECT_FALSE(tracker->hasTrack()) << registered;
        EXPECT_EQ(tracker->waypointCount(), 0u) << registered;

        const TrackerCommand cmd = tracker->update(poseAt(0, 0, 0, 0), DT);
        EXPECT_NEAR(cmd.throttle, 0.0, 1e-9) << registered;
        EXPECT_EQ(cmd.implement, 0) << registered;
    }
}

TEST(PathTrackerContractTest, SetTrackResetsProgress) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        const auto track = straightAlongX(30);

        // 완주까지 돌린 뒤 같은 경로를 다시 적재합니다. 트래커는 "이미 끝났다"
        // 상태로 남아 있으면 안 되고, 다시 주행할 준비가 되어 있어야 합니다.
        // (onTrackSet()에서 상태 초기화를 빼먹으면 여기서 걸립니다.)
        //
        // Run to completion, then reload the same path: the tracker must be
        // ready to drive it again rather than staying finished. Forgetting to
        // reset state in onTrackSet() is caught here.
        driveToCompletion(*tracker, track);
        tracker->setTrack(track);
        EXPECT_FALSE(tracker->isTrackComplete()) << registered;
    }
}

TEST(PathTrackerContractTest, StraightPathIsFollowedWithSmallSteer) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        const auto track = straightAlongX(30);
        tracker->setTrack(track);

        // 경로 위에 있고 방향도 맞은 상태입니다. 어떤 트래커든 큰 조향을 요구해선
        // 안 됩니다. 조향 부호를 반대로 잡으면 이 테스트가 잡아냅니다.
        // On the path, aligned with it: no tracker should demand much steer.
        // A reversed steering sign shows up here.
        const TrackerCommand cmd = tracker->update(poseAt(0.0, 0.0, 0.0, 1.1), DT);
        EXPECT_LT(std::abs(cmd.steerAngle), 0.15) << registered;
        EXPECT_EQ(cmd.implement, 1) << registered;
    }
}

TEST(PathTrackerContractTest, EveryTrackerCompletesAStraightPath) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        const int ticks = driveToCompletion(*tracker, straightAlongX(40));
        EXPECT_GT(ticks, 0) << registered << " never reported completion";
    }
}

TEST(PathTrackerContractTest, StillCorrectsWhileNearestToTheLastWaypoint) {
    // 마지막 웨이포인트가 최근접이 된 뒤에도 조향 보정과 오차 보고가 살아
    // 있어야 합니다. 접선 방향을 "현재 웨이포인트에서 나가는 구간"으로만 잡으면
    // 마지막에서는 그 구간이 없어 오차가 0으로 접히고, 최종 접근 구간이 통째로
    // 무보정이 되어 목표를 스쳐 지나갑니다.
    //
    // A tracker must still correct — and still report the error — once the last
    // waypoint is the nearest one. Deriving the path tangent only from the
    // segment *leaving* the current waypoint collapses both to zero there, which
    // leaves the entire final approach uncorrected.
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        const auto track = straightAlongX(5);
        tracker->setTrack(track);

        // Level with the last waypoint but 3m off to +z. Whatever the tracker
        // uses as its reference point, the last waypoint is the nearest one.
        const TrackerCommand cmd = tracker->update(poseAt(4.0, 3.0, 0.0, 1.5), DT);

        EXPECT_GT(std::abs(cmd.steerAngle), 0.05)
            << registered << " commanded no steer with 3m of lateral error";

        // 진단값도 함께 검사합니다. lateralDeviationM(LTD)은 텔레메트리 `ltd`
        // 키와 CSV 트레이스 `ltd` 컬럼으로 그대로 나가고, 대시보드의 Hero 수치와
        // 편차 미터가 이 값을 그립니다. 조향은 맞게 내면서 진단값만 0을 보고하면
        // 화면에는 "경로 위에 있다"고 표시되어 튜닝을 오판하게 됩니다.
        //
        // (예전에는 이 값이 RunSummary::deviationM 으로 누적되어 채점 패널티에
        // 반영됐습니다. RunSummary 는 제거됐고 지금 소비자는 대시보드입니다.)
        //
        // The diagnostic matters too: lateralDeviationM (LTD) goes out verbatim as
        // the telemetry `ltd` key and the CSV trace's `ltd` column, and the
        // dashboard's hero figure and deviation meter draw from it. A tracker that
        // steers correctly but reports 0 here makes the screen claim the vehicle is
        // on the path, which misleads tuning.
        //
        // (This used to accumulate into RunSummary::deviationM for the hackathon
        // score penalty; RunSummary is gone and the dashboard is the consumer now.)
        EXPECT_GT(std::abs(tracker->lastDebug().lateralDeviationM), 0.5)
            << registered << " reported ~zero lateral-deviation error with 3m offset";
    }
}

TEST(PathTrackerContractTest, DiagnosticsAreFilledAfterATick) {
    for (const std::string& registered : availableTrackerNames()) {
        auto tracker = createTracker(registered);
        ASSERT_TRUE(tracker != nullptr) << registered;
        tracker->setTrack(straightAlongX(30));
        tracker->update(poseAt(0.0, 1.0, 0.0, 1.1), DT);

        const TrackerDebug& dbg = tracker->lastDebug();
        // 계약 필드는 반드시 채워야 하며 -1 초기값으로 남아 있으면 안 됩니다.
        // Contract fields must be populated, not left at their -1 sentinel.
        EXPECT_GE(dbg.closestIdx, 0) << registered;
        EXPECT_GE(dbg.commandIdx, 0) << registered;
        EXPECT_GE(dbg.targetIdx, 0) << registered;
        // 차량이 +z쪽으로 1m 벗어나 있으므로 LTD(횡방향 편차)는 양수여야 합니다.
        // Vehicle is 1m to the +z side, so lateral-deviation must be signed positive.
        EXPECT_GT(dbg.lateralDeviationM, 0.0) << registered;
    }
}
