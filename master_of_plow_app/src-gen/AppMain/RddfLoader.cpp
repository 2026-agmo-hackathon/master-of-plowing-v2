/*
 * AppMain/RddfLoader.cpp
 *
 * 설계 배경은 RddfLoader.hpp 상단을 참고하세요.
 * See the top of RddfLoader.hpp for the rationale.
 */

#include <AppMain/RddfLoader.hpp>

#include <AppMain/CoordTransform.hpp>
#include <AppMain/MainController.hpp>
#include <AppMain/RddfParser.hpp>
#include <AppMain/RddfValidator.hpp>
#include <AppMain/tracking/IPathTracker.hpp>
#include <AppMain/tracking/TrackerFactory.hpp>
#include <AppMain/tracking/TrackerTypes.hpp>

#include <iostream>
#include <vector>

namespace AppMain
{

bool RddfLoader::loadFromFile(const std::string& filePath,
                              MainController& controller)
{
    try
    {
        // ── 파싱 / Parse ────────────────────────────────────────────────────
        auto parseResult = ::AppMain::RddfParser::parse(filePath);
        if (!parseResult.waypoints.has_value())
        {
            std::cerr << "[RddfLoader] parse failed: "
                    << parseResult.errorMessage << "\n";
            return false;
        }
        const auto& waypoints = parseResult.waypoints.value();
        if (waypoints.empty())
        {
            std::cerr << "[RddfLoader] parse failed: file contains no waypoints\n";
            return false;
        }

        // ── 검증 / Validate ─────────────────────────────────────────────────
        // File validation covers only properties intrinsic to the submitted
        // RDDF. The active simulator/session owns the live spawn and verifies
        // pose proximity at its start gate; this loader has no map-start input.
        auto validationResult = ::AppMain::RddfValidator::validate(waypoints);
        if (!validationResult.accepted)
        {
            std::cerr << "[RddfLoader] validation failed: "
                    << validationResult.reason << "\n";
            return false;
        }
        for (const auto& warning : validationResult.warnings)
        {
            std::cerr << "[RddfLoader] warning: " << warning << "\n";
        }

        // ── WGS84 -> UTM -> ENU ─────────────────────────────────────────────
        // 원점은 0번 웨이포인트입니다 (TrackerTypes.hpp의 좌표 규약).
        // Origin is waypoint 0, per the convention in TrackerTypes.hpp.
        auto originUtm = ::AppMain::CoordTransform::wgs84ToUtm(waypoints[0].lat,
                waypoints[0].lon);
        std::vector< ::AppMain::tracking::TrackerWaypoint > trackedWaypoints;
        trackedWaypoints.reserve(waypoints.size());
        for (const auto& wp : waypoints)
        {
            auto utm = ::AppMain::CoordTransform::wgs84ToUtm(wp.lat, wp.lon);
            auto enu = ::AppMain::CoordTransform::utmToEnu(utm.easting,
                    utm.northing, originUtm.easting, originUtm.northing);
            trackedWaypoints.push_back(
            { enu.x, enu.z, wp.speed, wp.implementFlag });
        }

        // ── 트래커 주입 / Install the tracker ───────────────────────────────
        // 어느 알고리즘이 도는지는 controller 가 보관한 선택(기본값은
        // PATH_TRACKER 환경변수)이 정합니다. 튜닝 UI 가 적재 전에 고른
        // 알고리즘과 게인이 여기서 그대로 적용됩니다.
        // Which algorithm runs comes from the controller's stored choice
        // (defaulting to the PATH_TRACKER env var), so an algorithm and gains
        // picked in the tuning UI before loading apply here.
        auto tracker = ::AppMain::tracking::createTracker(
                controller.getTrackerName());
        tracker->setParams(controller.getTrackerParams());
        tracker->setTrack(trackedWaypoints);

        // 새 트래커를 공개하기 전에 지난 주행 상태를 지웁니다. 완주 래치와 누적
        // 경과 시간이 남아 있으면 두 번째 경로에서 완주 방송이 나가지 않고 시간이
        // 이전 주행에서 이어집니다.
        //
        // Clear the previous run before the new tracker becomes visible: a latched
        // completion flag and accumulated elapsed time would stop the second path
        // from ever broadcasting completion and carry its timing over.
        controller.prepareForNewTrack();
        controller.setTracker(tracker);
        // 모르는 이름으로 폴백했을 수 있으므로 실제 이름을 되기록합니다.
        // Record the actual name in case createTracker() fell back.
        controller.setTrackerName(tracker->name());
        // ENU 웨이포인트를 보관합니다 — 알고리즘 전환 시 파일을 다시 읽지 않고
        // 새 트래커에 setTrack() 하기 위해서입니다 (TrackerSwitch 참고).
        // Keep the ENU waypoints so an algorithm switch can setTrack() without
        // re-reading the file (see TrackerSwitch).
        controller.setTrackedWaypoints(trackedWaypoints);
        // 원본 WGS84 웨이포인트는 대시보드가 경로를 그릴 때 쓸 수 있게 보관합니다.
        // Raw WGS84 waypoints are kept so a dashboard can draw the path.
        controller.setRawWaypoints(waypoints);
        controller.setTrackOrigin(originUtm.easting, originUtm.northing);
        controller.setTrackOriginGps(waypoints[0].lat, waypoints[0].lon);

        std::cerr << "[RddfLoader] loaded " << trackedWaypoints.size()
                << " waypoints from " << filePath << "\n";
        return true;
    } catch (const std::exception& e)
    {
        std::cerr << "[RddfLoader] exception: " << e.what() << "\n";
        return false;
    } catch (...)
    {
        std::cerr << "[RddfLoader] unknown exception\n";
        return false;
    }
}

} // namespace AppMain
