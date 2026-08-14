#include <gtest/gtest.h>
#include <cmath>
#include <string>
#include <vector>
#include "RddfParser.hpp"
#include "RddfValidator.hpp"

using namespace AppMain;

namespace {

RddfWaypoint makeWp(int idx, double lat, double lon, double speed, int impl = 1) {
    RddfWaypoint wp;
    wp.lineNo = idx;
    wp.index = idx;
    wp.lat = lat;
    wp.lon = lon;
    wp.speed = speed;
    wp.implementFlag = impl;
    return wp;
}

}  // anonymous namespace

TEST(RddfValidatorTest, AllValidIsAccepted) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.56651, 126.978, 4.0),  // ~1.1m
        makeWp(3, 37.56652, 126.978, 4.0),
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_TRUE(result.accepted);
    EXPECT_TRUE(result.reason.empty());
}


TEST(RddfValidatorTest, ValidationIsIndependentOfRuntimeSpawn) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5670, 126.9800, 4.0),  // first waypoint
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_TRUE(result.accepted)
        << "runtime spawn proximity is a session start-gate concern, not an "
           "intrinsic property of the submitted RDDF";
}

TEST(RddfValidatorTest, SparseSpacingIsRejected) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.5670, 126.978, 4.0),  // ~55m gap (sparse > 2m)
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_FALSE(result.accepted);
    EXPECT_NE(result.reason.find("spacing"), std::string::npos);
}

TEST(RddfValidatorTest, DuplicateSpacingIsRejected) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.5665, 126.978, 4.0),
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_FALSE(result.accepted);
    EXPECT_NE(result.reason.find("spacing"), std::string::npos);
}

TEST(RddfValidatorTest, InvalidImplementFlagIsRejected) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0, 1),
        makeWp(2, 37.56651, 126.978, 4.0, 2),
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_FALSE(result.accepted);
    EXPECT_NE(result.reason.find("implement flag"), std::string::npos);
}

TEST(RddfValidatorTest, HighSpeedIsRejectedWithoutMutation) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 50.0),
    };
    auto result = RddfValidator::validate(waypoints);
    EXPECT_FALSE(result.accepted);
    EXPECT_NE(result.reason.find("outside machine envelope"), std::string::npos);
}

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 기계 최저 지속속도(약 2.05 km/h) 미만을 요구하는 경로도 참가자 입력으로
// 그대로 적재합니다. 실제 기계가 그 속도를 유지하지 못할 수 있다는 사실은
// 경고와 속도 제어 텔레메트리로 노출합니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// A nonzero path speed below the machine's sustainable floor (~2.05 km/h) is
// still loaded unchanged. The physical limitation is exposed through a warning
// and speed-controller telemetry rather than disabling the entire route.
// └────────────────────────────────────────────────────────────────────────────┘

TEST(RddfValidatorTest, SpeedBelowMachineMinimumIsAcceptedWithWarning) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.56651, 126.978, 1.2),  // headland speed, below the floor
        makeWp(3, 37.56652, 126.978, 4.0),
    };
    auto result = RddfValidator::validate(waypoints);

    EXPECT_TRUE(result.accepted);
    ASSERT_EQ(result.warnings.size(), 1u);
    EXPECT_NE(result.warnings.front().find("below the machine's sustainable minimum"),
              std::string::npos);
    EXPECT_NE(result.warnings.front().find("first at waypoint 1"),
              std::string::npos);
}

TEST(RddfValidatorTest, SpeedAtOrAboveMachineMinimumIsSilent) {
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.56651, 126.978, 2.4),  // above the ~2.05 km/h floor
    };
    auto result = RddfValidator::validate(waypoints);

    for (const std::string& w : result.warnings) {
        EXPECT_EQ(w.find("machine minimum"), std::string::npos);
    }
}

TEST(RddfValidatorTest, ZeroSpeedIsNotFlaggedAsTooSlow) {
    // 0은 "정지 지시"이지 "너무 느린 주행"이 아닙니다.
    // Zero means "stop here", not "drive too slowly".
    std::vector<RddfWaypoint> waypoints = {
        makeWp(1, 37.5665, 126.978, 4.0),
        makeWp(2, 37.56651, 126.978, 0.0),
    };
    auto result = RddfValidator::validate(waypoints);

    EXPECT_TRUE(result.accepted);
}

namespace {

// 반지름 radiusM 인 원호 위에 1m 간격으로 웨이포인트를 놓습니다.
// Waypoints 1 m apart along an arc of the given radius.
std::vector<RddfWaypoint> arcPath(double radiusM, int count) {
    constexpr double LAT0 = 37.5665;
    constexpr double LON0 = 126.978;
    constexpr double M_PER_DEG_LAT = 111132.0;
    const double mPerDegLon = 111320.0 * std::cos(LAT0 * M_PI / 180.0);

    std::vector<RddfWaypoint> wps;
    const double dTheta = 1.0 / radiusM;  // 1 m of arc
    for (int i = 0; i < count; ++i) {
        const double th = dTheta * i;
        const double east = radiusM * std::sin(th);
        const double north = radiusM * (1.0 - std::cos(th));
        wps.push_back(makeWp(i + 1, LAT0 + north / M_PER_DEG_LAT,
                             LON0 + east / mPerDegLon, 4.0));
    }
    return wps;
}

bool hasTightTurnWarning(const ValidationResult& r) {
    for (const auto& warning : r.warnings) {
        if (warning.find("tighter than the machine") != std::string::npos) {
            return true;
        }
    }
    return false;
}

}  // anonymous namespace

TEST(RddfValidatorTest, TurnTighterThanTheMachineIsAcceptedWithWarning) {
    // 기계 최소 회전반경보다 확실히 급한 원호. 조향을 끝까지 물려도 못 따라갑니다.
    // An arc comfortably tighter than the machine's minimum radius: it cannot be
    // followed even at full lock.
    const double tooTight = RddfValidator::minTurnRadiusM() * 0.6;
    auto waypoints = arcPath(tooTight, 12);
    auto result = RddfValidator::validate(waypoints);

    EXPECT_TRUE(result.accepted);
    EXPECT_TRUE(hasTightTurnWarning(result))
            << "an arc of radius " << tooTight << " m needs more steer than the "
               "wheels can reach (minimum radius "
            << RddfValidator::minTurnRadiusM()
            << " m) and must be reported, otherwise the path looks like a "
               "controller bug when the tractor runs wide through every corner.";
}

TEST(RddfValidatorTest, TurnTheMachineCanDriveIsSilent) {
    const double comfortable = RddfValidator::minTurnRadiusM() * 2.0;
    auto waypoints = arcPath(comfortable, 12);
    auto result = RddfValidator::validate(waypoints);

    EXPECT_FALSE(hasTightTurnWarning(result))
            << "radius " << comfortable << " m is twice the machine minimum and "
               "must not be flagged";
}

TEST(RddfValidatorTest, StraightPathHasNoTightTurnWarning) {
    // 위경도 눈금(1e-5도 = 약 1.1m)에 걸린 직선. 양자화 잡음이 곡률로 잘못
    // 읽히면 모든 경로가 경고를 뱉게 됩니다.
    // A straight line on the lat/lon grid (1e-5 deg ~ 1.1 m). If quantisation
    // noise reads as curvature then every path warns.
    std::vector<RddfWaypoint> waypoints;
    for (int i = 0; i < 20; ++i) {
        waypoints.push_back(makeWp(i + 1, 37.5665 + i * 1e-5, 126.978, 4.0));
    }
    auto result = RddfValidator::validate(waypoints);

    EXPECT_FALSE(hasTightTurnWarning(result));
}
