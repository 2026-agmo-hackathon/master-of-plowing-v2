/*
 * AppMain/RddfValidator.cpp
 */

#include "RddfValidator.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace AppMain {

double RddfValidator::haversineMeters(double lat1, double lon1,
                                      double lat2, double lon2)
{
    constexpr double R = 6371000.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    return 2.0 * R * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
}

// Format a double to one decimal place as string
static std::string fmt1(double v)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << v;
    return oss.str();
}

static std::string fmt2(double v)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << v;
    return oss.str();
}

ValidationResult RddfValidator::validate(const std::vector<RddfWaypoint>& waypoints)
{
    ValidationResult r;
    r.accepted = true;
    std::size_t belowFloorCount = 0;
    std::size_t firstBelowFloorIdx = 0;
    double lowestNonzeroSpeedKmh = 0.0;

    // Rule 4: empty waypoints
    if (waypoints.empty()) {
        r.accepted = false;
        r.reason = "empty waypoints";
        return r;
    }

    // Only the two documented implement states are safe to forward to the
    // machine. Reject malformed flags before any tracker state is installed.
    for (std::size_t i = 0; i < waypoints.size(); ++i) {
        const RddfWaypoint& wp = waypoints[i];
        if (!std::isfinite(wp.lat) || !std::isfinite(wp.lon) ||
            !std::isfinite(wp.speed)) {
            r.accepted = false;
            r.reason = "non-finite waypoint value at waypoint " +
                       std::to_string(static_cast<int>(i));
            return r;
        }
        if (wp.implementFlag != 0 && wp.implementFlag != 1) {
            r.accepted = false;
            r.reason = "invalid implement flag " +
                       std::to_string(wp.implementFlag) + " at waypoint " +
                       std::to_string(static_cast<int>(i)) +
                       " (expected 0 or 1)";
            return r;
        }
        const double speedMagnitude = std::fabs(wp.speed);
        if (speedMagnitude > RddfValidator::MAX_MACHINE_SPEED_KMH) {
            r.accepted = false;
            r.reason = "speed outside machine envelope at waypoint " +
                       std::to_string(static_cast<int>(i)) + ": got " +
                       fmt1(wp.speed) + " km/h, allowed -" +
                       fmt1(RddfValidator::MAX_MACHINE_SPEED_KMH) + ".." +
                       fmt1(RddfValidator::MAX_MACHINE_SPEED_KMH) + " km/h";
            return r;
        }
        if (speedMagnitude > 0.0 &&
            speedMagnitude < RddfValidator::MIN_MACHINE_SPEED_KMH) {
            if (belowFloorCount == 0) {
                firstBelowFloorIdx = i;
                lowestNonzeroSpeedKmh = speedMagnitude;
            } else if (speedMagnitude < lowestNonzeroSpeedKmh) {
                lowestNonzeroSpeedKmh = speedMagnitude;
            }
            ++belowFloorCount;
        }
    }

    if (belowFloorCount > 0) {
        r.warnings.push_back(
            std::to_string(static_cast<unsigned long long>(belowFloorCount)) +
            " waypoint(s) request a nonzero speed below the machine's " +
            "sustainable minimum: first at waypoint " +
            std::to_string(static_cast<unsigned long long>(firstBelowFloorIdx)) +
            ", lowest magnitude " + fmt1(lowestNonzeroSpeedKmh) +
            " km/h versus " + fmt2(RddfValidator::MIN_MACHINE_SPEED_KMH) +
            " km/h. The RDDF is loaded unchanged; the speed controller may " +
            "raise an unreachable target to its creep floor and reports that " +
            "condition in telemetry.");
    }

    // Rule 3: invalid spacing. This blocks loading because interpolating across
    // an unrepresented gap (or through duplicate points) is not physically
    // well-defined for the tracker.
    for (std::size_t i = 1; i < waypoints.size(); ++i) {
        double d = haversineMeters(waypoints[i - 1].lat, waypoints[i - 1].lon,
                                   waypoints[i].lat, waypoints[i].lon);
        if (d < MIN_WAYPOINT_SPACING_M || d > MAX_WAYPOINT_SPACING_M) {
            r.accepted = false;
            r.reason = "invalid waypoint spacing " +
                       std::to_string(static_cast<int>(i - 1)) + "->" +
                       std::to_string(static_cast<int>(i)) + " (" + fmt1(d) +
                       " m, required " + fmt1(MIN_WAYPOINT_SPACING_M) + ".." +
                       fmt1(MAX_WAYPOINT_SPACING_M) + " m)";
            return r;
        }
    }

    // Rule 6: 기계가 못 도는 코너 경고. 연속 3점의 방위 변화로 곡률을 재고,
    // 자전거 모델로 필요한 앞바퀴각을 환산해 물리 한계와 비교합니다.
    // Rule 6: undrivable-corner warning. Curvature from the heading change over
    // three consecutive points is converted to the wheel angle the bicycle model
    // needs and compared against the physical limit. Participant geometry remains
    // unchanged and loadable; this warning explains expected tracking error.
    {
        const double minR = minTurnRadiusM();
        const double wheelbase = tracking::PurePursuitTracker::WHEELBASE_M;
        std::size_t tight = 0;
        double tightestR = 0.0;
        std::size_t tightestIdx = 0;

        for (std::size_t i = 1; i + 1 < waypoints.size(); ++i) {
            const RddfWaypoint& a = waypoints[i - 1];
            const RddfWaypoint& b = waypoints[i];
            const RddfWaypoint& c = waypoints[i + 1];

            // 전진/후진이 바뀌는 지점은 곡률이 의미가 없습니다(방향이 뒤집히므로).
            // Curvature is meaningless across an F/R reversal.
            if (a.speed * b.speed < 0.0 || b.speed * c.speed < 0.0) {
                continue;
            }

            const double d1 = haversineMeters(a.lat, a.lon, b.lat, b.lon);
            const double d2 = haversineMeters(b.lat, b.lon, c.lat, c.lon);
            if (d1 < 0.05 || d2 < 0.05) {
                continue;
            }

            // 위경도 -> 국소 평면(m). 짧은 구간이라 이 근사로 충분합니다.
            // Lat/lon to a local plane in metres; the segments are short enough.
            const double mPerDegLat = 111132.0;
            const double mPerDegLon = 111320.0 * std::cos(b.lat * M_PI / 180.0);
            const double ux = (b.lon - a.lon) * mPerDegLon;
            const double uy = (b.lat - a.lat) * mPerDegLat;
            const double vx = (c.lon - b.lon) * mPerDegLon;
            const double vy = (c.lat - b.lat) * mPerDegLat;

            double dTheta = std::atan2(vy, vx) - std::atan2(uy, ux);
            while (dTheta >  M_PI) dTheta -= 2.0 * M_PI;
            while (dTheta < -M_PI) dTheta += 2.0 * M_PI;

            const double ds = 0.5 * (d1 + d2);
            const double curvature = std::fabs(dTheta) / ds;
            if (curvature < 1e-9) {
                continue;
            }

            const double radius = 1.0 / curvature;
            if (radius < minR) {
                if (tight == 0 || radius < tightestR) {
                    tightestR = radius;
                    tightestIdx = i;
                }
                ++tight;
            }
        }

        if (tight > 0) {
            const double needDeg = std::atan(wheelbase / tightestR) * 180.0 / M_PI;
            const double maxDeg =
                    tracking::PurePursuitTracker::WHEEL_MAX_RAD * 180.0 / M_PI;
            r.warnings.push_back(
                "turn tighter than the machine can drive at " +
                std::to_string(static_cast<int>(tight)) + " waypoint(s): "
                "tightest radius " + fmt1(tightestR) + " m at waypoint " +
                std::to_string(static_cast<int>(tightestIdx)) +
                " needs " + fmt1(needDeg) + " deg of steer but the wheels stop at " +
                fmt1(maxDeg) + " deg (minimum radius " + fmt1(minR) + " m). "
                "The tractor will run wide through these corners and carry the "
                "cross-track error into the next pass. Open the turn out in the "
                "path; no controller gain can turn tighter than the machine.");
        }
    }

    return r;
}

} // namespace AppMain
