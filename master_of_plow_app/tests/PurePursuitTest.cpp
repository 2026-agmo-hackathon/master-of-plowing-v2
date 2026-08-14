#include <gtest/gtest.h>

#include <AppMain/tracking/impl/PurePursuitTracker.hpp>

#include <cmath>
#include <vector>

using namespace AppMain::tracking;

// ┌─ 한국어: 이 파일이 검증하는 좌표계와 부호 규약 ────────────────────────────┐
//
// 위치는 (x, z)이고 +x = 동쪽, +z = 남쪽입니다. 방향은 atan2(z, x)로 측정합니다.
// 따라서 이 좌표계는 왼손 좌표계이고, 트래커의 모든 부호 실수는 여기서 나옵니다.
//
//   heading 0    -> 동쪽을 봄        heading +pi/2 -> 남쪽을 봄
//   조향 > 0     -> 좌회전           그리고 좌회전은 heading을 "감소"시킵니다
//
// 트래커가 전방 목표점을 차량 좌표계로 옮기는 식은 다음과 같습니다.
//
//   x_local = dx*cos(h) + dz*sin(h)     전방 성분, 양수 = 차량 앞쪽
//   z_local = dx*sin(h) - dz*cos(h)     횡방향 성분, 양수 = 차량의 "왼쪽"
//
// 그래서 동쪽으로 가는 경로의 +z쪽(남쪽)에 있는 차량은 목표점을 왼쪽에서 보게
// 되고, z_local > 0 이 되며, 경로로 돌아오기 위해 양수(좌회전 = 북쪽으로 틀기)
// 조향을 냅니다. 이 셋은 하나의 규약에서 같이 따라 나옵니다. 둘 중 하나만
// 기대하는 테스트는 존재하지 않는 좌표계를 검사하는 것입니다.
//
// 부호 테스트용 웨이포인트는 z = 0 위를 +x 방향으로 1m 간격으로 놓았습니다.
// 그래야 최소 lookahead 거리 5.5m가 차량보다 충분히 앞에 떨어집니다.
//
// └────────────────────────────────────────────────────────────────────────────┘
//
// ┌─ English: frame and sign conventions under test ───────────────────────────┐
//
// Position is (x, z) with +x = East and +z = South, and heading is measured as
// atan2(z, x). That makes the frame left-handed, which is where every sign
// mistake in a tracker comes from:
//
//   heading  0     -> facing East       heading +pi/2 -> facing South
//   steer    > 0   -> LEFT turn         and a left turn DECREASES heading
//
// The tracker's vehicle-frame projection of the lookahead target is
//
//   x_local = dx*cos(h) + dz*sin(h)     forward, + = ahead of the vehicle
//   z_local = dx*sin(h) - dz*cos(h)     lateral, + = to the vehicle's LEFT
//
// So a vehicle sitting on the +z (South) side of an eastbound path sees its
// target to the left, gets z_local > 0, and steers positive (left, i.e. toward
// North) to rejoin. Both of those follow from the same convention; a test that
// expects one without the other is testing a frame that does not exist.
//
// Waypoints for the sign tests run straight along +x at z = 0, spaced 1m, so
// the 5.5m minimum lookahead lands well ahead of the vehicle.
//
// └────────────────────────────────────────────────────────────────────────────┘

namespace {

std::vector<TrackerWaypoint> straightAlongX(int count, double speedKmh = 4.0,
                                            int implementFlag = 1)
{
    std::vector<TrackerWaypoint> wps;
    for (int i = 0; i < count; ++i) {
        wps.push_back({static_cast<double>(i), 0.0, speedKmh, implementFlag});
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

std::vector<TrackerWaypoint> arc(double radiusM, int count = 40)
{
    std::vector<TrackerWaypoint> wps;
    for (int i = 0; i < count; ++i) {
        const double a = static_cast<double>(i) / radiusM; // about 1 m spacing
        wps.push_back({radiusM * std::sin(a),
                       radiusM * (1.0 - std::cos(a)), 4.0, 1});
    }
    return wps;
}

} // namespace

TEST(PurePursuitTest, StraightPathHasNearZeroSteer) {
    // +Z 방향 직선 경로, 차량은 (0,0)에서 +Z를 향함(heading = PI/2).
    // LTD(횡방향 편차)가 0이면 곡률 요구도 0이어야 합니다.
    // Waypoints along +Z; vehicle at (0,0) facing +Z (heading = PI/2).
    // Zero lateral offset means zero curvature demand.
    std::vector<TrackerWaypoint> waypoints;
    for (int i = 0; i < 20; ++i) {
        waypoints.push_back({0.0, static_cast<double>(i), 4.0, 1});
    }
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 0.0, M_PI / 2.0, 1.1), 0.1);
    EXPECT_LT(std::abs(cmd.steerAngle), 0.087);  // < 5 degrees
}

TEST(PurePursuitTest, SameTrackRestartReleasesZeroOrderHoldFromStop) {
    const auto waypoints = straightAlongX(40, 2.4);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    const VehicleState stopped = poseAt(0.0, 0.0, 0.0, 0.0);
    EXPECT_GT(tracker.update(stopped, 0.1).throttle, 0.0);

    // A stop/start does not necessarily upload a new RDDF. Re-applying the
    // unchanged track is the per-run reset contract used by TrackingLoop.
    tracker.setTrack(waypoints);
    const TrackerCommand restarted = tracker.update(stopped, 0.1);
    EXPECT_GT(restarted.throttle, 0.0);
    EXPECT_DOUBLE_EQ(restarted.brake, 0.0);
    EXPECT_EQ(restarted.gear, 1);
}

TEST(PurePursuitTest, RightCurveProducesNonZeroSteer) {
    // 경로가 +Z에서 +X로 휘므로 의미 있는 크기의 조향이 필요합니다.
    // Path bends from +Z toward +X, so a non-trivial steer is required.
    std::vector<TrackerWaypoint> waypoints = {
        {0, 0, 4.0, 1}, {0, 1, 4.0, 1}, {0, 2, 4.0, 1},
        {0.5, 3, 4.0, 1}, {1, 3.5, 4.0, 1}, {2, 4, 4.0, 1}, {3, 4, 4.0, 1},
        {5, 4, 4.0, 1}, {7, 4, 4.0, 1},
    };
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 1.5, M_PI / 2.0, 1.1), 0.1);
    EXPECT_GT(std::abs(cmd.steerAngle), 0.05);
}

TEST(PurePursuitTest, CurveAwareLookaheadPreservesStraightAndShortensR5)
{
    PurePursuitTracker straight;
    straight.setTrack(straightAlongX(40));
    straight.update(poseAt(1.0, 0.0, 0.0, 1.0), 0.1);
    const double straightLookahead = straight.lastDebug().get("lookaheadDist");

    PurePursuitTracker curved;
    curved.setTrack(arc(5.0));
    curved.update(poseAt(std::sin(0.2) * 5.0,
                         (1.0 - std::cos(0.2)) * 5.0, 0.2, 1.0), 0.1);
    const double curveLookahead = curved.lastDebug().get("lookaheadDist");

    EXPECT_GE(straightLookahead, 5.4)
            << "straight delay margin must remain conservative";
    EXPECT_GE(curveLookahead, 3.5)
            << "valid near-minimum-radius curves retain a bounded preview";
    EXPECT_LT(curveLookahead, straightLookahead - 0.5)
            << "R5 preview must shorten enough to avoid cutting across the arc";
}

TEST(PurePursuitTest, SelectedGeometryLimitsAngleAndAccountsForSteeringDelay)
{
    PurePursuitTracker limited;
    VehicleGeometry compact;
    compact.maxWheelAngleRad=0.18;
    limited.setVehicleGeometry(compact);
    limited.setTrack(arc(5.0,80));
    const auto command=limited.update(poseAt(1.0,1.0,0.2,3.0),0.1);
    EXPECT_LE(std::abs(command.steerAngle),0.18+1e-9);

    PurePursuitTracker normal;
    normal.setTrack(arc(5.0,80));
    normal.update(poseAt(1.0,1.0,0.2,3.0),0.1);
    PurePursuitTracker sluggish;
    VehicleGeometry slow;
    slow.steeringWheelMaxRateDegS=40.0;
    slow.steeringWheelTauS=1.0;
    slow.frontWheelTauS=0.8;
    sluggish.setVehicleGeometry(slow);
    sluggish.setTrack(arc(5.0,80));
    sluggish.update(poseAt(1.0,1.0,0.2,3.0),0.1);
    EXPECT_GT(sluggish.lastDebug().targetIdx,normal.lastDebug().targetIdx)
        << "a slower steering plant must preview farther along the same arc";
}

TEST(PurePursuitTest, ImplementFlagDoesNotChangeLateralControl) {
    std::vector<TrackerWaypoint> up;
    std::vector<TrackerWaypoint> down;
    constexpr double radius = 10.0;
    for (int i = 0; i <= 24; ++i) {
        const double angle = (M_PI / 2.0) * static_cast<double>(i) / 24.0;
        const double x = radius * std::sin(angle);
        const double z = radius * (1.0 - std::cos(angle));
        up.push_back({x, z, 2.4, 0});
        down.push_back({x, z, 2.4, 1});
    }

    PurePursuitTracker upTracker;
    PurePursuitTracker downTracker;
    upTracker.setTrack(up);
    downTracker.setTrack(down);
    const VehicleState pose = poseAt(0.0, 0.5, 0.08, 0.77);
    const TrackerCommand upCommand = upTracker.update(pose, 0.1);
    const TrackerCommand downCommand = downTracker.update(pose, 0.1);

    EXPECT_DOUBLE_EQ(upCommand.steerAngle, downCommand.steerAngle);
    EXPECT_NE(upCommand.implement, downCommand.implement);
    const TrackerDebug& upDebug = upTracker.lastDebug();
    const TrackerDebug& downDebug = downTracker.lastDebug();
    EXPECT_DOUBLE_EQ(upDebug.lateralDeviationM, downDebug.lateralDeviationM);
    EXPECT_DOUBLE_EQ(upDebug.headingErrorRad, downDebug.headingErrorRad);
    EXPECT_DOUBLE_EQ(upDebug.get("cfTerm"), downDebug.get("cfTerm"));
    EXPECT_DOUBLE_EQ(upDebug.get("ppTerm"), downDebug.get("ppTerm"));
    EXPECT_DOUBLE_EQ(upDebug.get("ltdTerm"), downDebug.get("ltdTerm"));
    EXPECT_DOUBLE_EQ(upDebug.get("feedbackApplied"), downDebug.get("feedbackApplied"));
}

TEST(PurePursuitTest, SteerNeverExceedsPhysicalWheelLimit) {
    // 예전 상한은 1.0 rad(57.3°)로 실제 물리 한계의 2배가 넘었습니다. 도달할 수
    // 없는 각을 계속 요구하면 하위 조향 루프가 영구히 포화되고, 부호가 뒤집힐
    // 때마다 풀락에서 풀락까지 되감느라 S자 주행이 됩니다. 큰 횡편차와 큰 방향
    // 오차를 동시에 주어 모든 항을 같은 방향으로 몰아넣습니다.
    //
    // The old cap was 1.0 rad (57.3 deg), over twice the real physical limit.
    // Demanding an angle the wheels cannot reach saturates the inner steering
    // loop permanently, and every sign flip then has to slew lock to lock — the
    // S-weave. Drive every term the same way at once with a large lateral offset
    // and a large heading error.
    PurePursuitTracker tracker;
    tracker.setTrack(straightAlongX(40));

    // 경로에서 6m 벗어난 채 경로 반대 방향을 보고 있음.
    // 6 m off the path and facing the opposite way along it.
    auto cmd = tracker.update(poseAt(5.0, 6.0, M_PI, 1.1), 0.1);
    EXPECT_LE(std::abs(cmd.steerAngle), 0.56 + 1e-9);
}

TEST(PurePursuitTest, LateralTermSoftensAsSpeedRises) {
    // ltdTerm에 속도 항이 없으면 같은 편차에 같은 조향을 요구하는데, 빠를수록
    // 반응할 시간만 짧아져 오버슈트가 커집니다. 에뮬레이터에서 2 -> 4km/h로
    // 올렸을 때 횡편차 평균이 0.78m -> 3.49m로 악화된 원인입니다.
    //
    // Without a speed term, ltdTerm demands the same angle at any speed while
    // leaving less time to act, so overshoot grows. That is why the emulator run
    // moved mean cross-track error from 0.78 m to 3.49 m going 2 -> 4 km/h.
    //
    // 두 속도 모두 lookahead 는 최소값 5.5m로 고정되므로, 차이는 ltdTerm 에서만
    // 나옵니다. / Lookahead pins to its 5.5 m minimum at both speeds, so the only
    // difference comes from ltdTerm.
    const double offsetM = 0.5;

    PurePursuitTracker slow;
    slow.setTrack(straightAlongX(40));
    const double steerSlow =
            std::abs(slow.update(poseAt(5.0, offsetM, 0.0, 1.0), 0.1).steerAngle);

    PurePursuitTracker fast;
    fast.setTrack(straightAlongX(40));
    const double steerFast =
            std::abs(fast.update(poseAt(5.0, offsetM, 0.0, 2.0), 0.1).steerAngle);

    EXPECT_GT(steerSlow, steerFast);
}

TEST(PurePursuitTest, GoalReachedMarksTrackCompleteAndHoldsLastCommand) {
    // 마지막 웨이포인트로부터 GOAL_TOLERANCE_M(1m) 안에 있고, 0번 웨이포인트에도
    // 충분히 가까워 주행 시작 가드가 이미 풀린 상태입니다.
    // Within GOAL_TOLERANCE_M (1m) of the final waypoint, and close enough to
    // waypoint 0 that the path-start guard has released.
    std::vector<TrackerWaypoint> waypoints = {
        {0, 0, 4.0, 1}, {0, 1, 4.0, 1}, {0, 2, 4.0, 1},
    };
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 2.0, M_PI / 2.0, 1.1), 0.1);

    EXPECT_TRUE(tracker.isTrackComplete());
    // ★ 주의: 트래커 단독으로는 완주 시 마지막 명령을 반복합니다. 실제 제품
    //   루프는 첫 완주 tick에서 동기적으로 safe Idle/출력 해제를 요청하고,
    //   simulator selection이 물리 정지를 확인한 뒤 recorder를 finalize합니다.
    //
    // NOTE: on completion the tracker repeats whatever it last commanded rather
    // than braking. Here that is the initial idle command, so throttle is 0 —
    // but mid-run it would be the last driving command, throttle included. The
    // product loop synchronously requests safe Idle/output release on the first
    // completed tick, then waits for authoritative physical-stop confirmation
    // before recorder finalization.
    EXPECT_EQ(cmd.gear, 1);
    EXPECT_LT(cmd.throttle, 0.1);
}

TEST(PurePursuitTest, NegativeSpeedYieldsReverseGear) {
    std::vector<TrackerWaypoint> waypoints = {
        {0, 0, -3.0, 0}, {0, -1, -3.0, 0}, {0, -2, -3.0, 0}, {0, -3, -3.0, 0},
    };
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 0.0, M_PI / 2.0, 0.0), 0.1);
    EXPECT_EQ(cmd.gear, -1);
}

TEST(PurePursuitTest, VehicleSouthOfPathSteersLeft) {
    // 차량이 z = +2, 즉 동쪽으로 가는 경로의 남쪽에 있고 heading은 0입니다.
    // 목표점이 차량의 왼쪽(북쪽)에 있으므로 z_local > 0 이 되고, 조향 명령은
    // 양수 — 경로로 되돌아가는 좌회전입니다.
    //
    // Vehicle at z = +2 (South of an eastbound path) with heading 0.
    // The target lies to the vehicle's left (North), so z_local > 0 and the
    // steer command is positive — a left turn back onto the path.
    const auto waypoints = straightAlongX(20);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 2.0, 0.0, 1.1), 0.1);
    const auto& dbg = tracker.lastDebug();

    EXPECT_GT(dbg.get("zLocal"), 0.0);
    EXPECT_GT(cmd.steerAngle, 0.0);
    // LTD(횡방향 편차)는 경로의 +z쪽에서 양수입니다.
    // Cross-track error is positive on the +z side of the path.
    EXPECT_GT(dbg.lateralDeviationM, 0.0);
}

TEST(PurePursuitTest, VehicleNorthOfPathSteersRight) {
    // 앞 테스트의 거울상입니다. 차량이 z = -2(경로의 북쪽)에 있으면 목표점을
    // 오른쪽에서 보게 되므로 z_local < 0, 조향은 음수가 됩니다.
    //
    // Mirror image of the previous test: vehicle at z = -2 (North of the path)
    // sees the target to its right, so z_local < 0 and the steer is negative.
    const auto waypoints = straightAlongX(20);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, -2.0, 0.0, 1.1), 0.1);
    const auto& dbg = tracker.lastDebug();

    EXPECT_LT(dbg.get("zLocal"), 0.0);
    EXPECT_LT(cmd.steerAngle, 0.0);
    EXPECT_LT(dbg.lateralDeviationM, 0.0);
}

TEST(PurePursuitTest, OnPathProducesZeroSteer) {
    const auto waypoints = straightAlongX(20);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    auto cmd = tracker.update(poseAt(0.0, 0.0, 0.0, 1.1), 0.1);
    const auto& dbg = tracker.lastDebug();

    EXPECT_NEAR(dbg.get("zLocal"), 0.0, 1e-6);
    EXPECT_NEAR(cmd.steerAngle, 0.0, 0.01);
}

TEST(PurePursuitTest, ForwardTargetHasPositiveXLocal) {
    const auto waypoints = straightAlongX(20);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    tracker.update(poseAt(0.0, 0.0, 0.0, 1.1), 0.1);
    EXPECT_GT(tracker.lastDebug().get("xLocal"), 0.0);
}

TEST(PurePursuitTest, HeadingRotatesTargetIntoVehicleFrame) {
    // +Z 방향 경로에 차량도 +Z를 향합니다. 목표점은 순수하게 전방으로만 투영되어야
    // 합니다: x_local > 0, z_local == 0.
    // Waypoints along +Z, vehicle facing +Z. The target must project as purely
    // forward: x_local > 0 and z_local == 0.
    std::vector<TrackerWaypoint> waypoints;
    for (int i = 0; i < 20; ++i) {
        waypoints.push_back({0.0, static_cast<double>(i), 4.0, 1});
    }
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    tracker.update(poseAt(0.0, 0.0, M_PI / 2.0, 1.1), 0.1);
    const auto& dbg = tracker.lastDebug();

    EXPECT_GT(dbg.get("xLocal"), 0.0);
    EXPECT_NEAR(dbg.get("zLocal"), 0.0, 1e-6);
}

TEST(PurePursuitTest, ReverseTargetFlipsSteerAngle) {
    // VehicleNorthOfPathSteersRight 와 기하는 같지만 후진 구간에서 테스트합니다.
    // 후진 중에는 같은 방향으로 붙으려면 바퀴를 반대로 꺾어야 하므로, 후진 목표점
    // 에서는 명령 전체의 부호가 뒤집힙니다.
    //
    // Same geometry as VehicleNorthOfPathSteersRight but on a reverse segment.
    // Steering into a corner while backing up requires the opposite wheel
    // direction, so the whole command is negated for reverse targets.
    const auto forwardWaypoints = straightAlongX(20, 3.0, 0);
    const auto reverseWaypoints = straightAlongX(20, -3.0, 0);

    PurePursuitTracker forwardTracker;
    forwardTracker.setTrack(forwardWaypoints);
    const double forwardSteer =
        forwardTracker.update(poseAt(0.0, -2.0, 0.0, 0.0), 0.1).steerAngle;

    PurePursuitTracker reverseTracker;
    reverseTracker.setTrack(reverseWaypoints);
    const double reverseSteer =
        reverseTracker.update(poseAt(0.0, -2.0, 0.0, 0.0), 0.1).steerAngle;

    EXPECT_LT(forwardSteer, 0.0);
    EXPECT_GT(reverseSteer, 0.0);
}

TEST(PurePursuitTest, ImplementIndexTrailsVehicleOnStraightPath)
{
    // 작업기는 차량보다 3m 뒤에 있으므로 웨이포인트 인덱스도 차량보다 뒤처져야
    // 합니다. 실제 주행처럼 평면 통과로 명령 인덱스가 전진하도록 0.5m씩 움직입니다.
    //
    // The implement sits 3m behind the vehicle, so its waypoint index must lag
    // the vehicle's. Drive in 0.5m steps so plane-crossing advances the command
    // index the way it does in a real run.
    const auto waypoints = straightAlongX(20);
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    for (double x = 0.5; x <= 10.0; x += 0.5) {
        tracker.update(poseAt(x, 0.0, 0.0, 1.1), 0.1);
    }

    const auto& dbg = tracker.lastDebug();
    const int implementClosestIdx = static_cast<int>(dbg.get("implementClosestIdx", -1.0));

    EXPECT_GE(dbg.closestIdx, 7);
    EXPECT_LE(implementClosestIdx, dbg.closestIdx);
    EXPECT_GE(implementClosestIdx, dbg.commandIdx);
    EXPECT_LE(implementClosestIdx, dbg.commandIdx + 3);
}

TEST(PurePursuitTest, CommandIndexNeverRegressesWhenCuttingAKTurn)
{
    // 전진 구간 wp[0..9]는 z=0, 후진 구간 wp[10..19]는 z=3에 둡니다. 그래서 F/R
    // 경계가 wp[9]와 wp[10] 사이에 있고 경로가 자기 자신을 되짚어 옵니다 —
    // 단순한 최근접 웨이포인트 탐색이 틀리는 바로 그 상황입니다.
    //
    // Forward pass wp[0..9] at z=0, reverse pass wp[10..19] at z=3, so the F/R
    // boundary sits between wp[9] and wp[10] and the path doubles back on
    // itself — the case a plain nearest-waypoint search gets wrong.
    std::vector<TrackerWaypoint> waypoints;
    for (int i = 0; i < 10; ++i) {
        waypoints.push_back({static_cast<double>(i), 0.0, 4.0, 1});
    }
    for (int i = 0; i < 10; ++i) {
        waypoints.push_back({static_cast<double>(9 - i), 3.0, -3.0, 0});
    }
    PurePursuitTracker tracker;
    tracker.setTrack(waypoints);

    int finalCmdIdx = -1;
    for (double x = 0.0; x <= 9.5; x += 0.5) {
        tracker.update(poseAt(x, 0.0, 0.0, 1.1), 0.1);
        finalCmdIdx = tracker.lastDebug().commandIdx;
        if (finalCmdIdx >= 8) break;
    }
    ASSERT_GE(finalCmdIdx, 5) << "forward driving failed to advance commandIdx";

    // 코너를 잘라 지나가는 상황: 여기서는 평면 통과만으로는 wp[9]를 넘어갈 수
    // 없습니다. 최근접 인덱스 하한 보정이 명령 인덱스를 끌고 넘어가야 합니다.
    //
    // Cutting the corner: plane-crossing alone cannot advance past wp[9] here,
    // so the closest-index floor has to carry the command index through.
    tracker.update(poseAt(9.5, 0.0, 0.0, 1.1), 0.1);
    const auto& dbg = tracker.lastDebug();
    const int implementClosestIdx = static_cast<int>(dbg.get("implementClosestIdx", -1.0));

    EXPECT_GE(dbg.commandIdx, finalCmdIdx) << "commandIdx must not regress";
    EXPECT_GE(implementClosestIdx, dbg.commandIdx);
    EXPECT_LE(implementClosestIdx, dbg.commandIdx + 5);
}
