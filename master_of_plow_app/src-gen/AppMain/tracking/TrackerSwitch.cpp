/*
 * AppMain/tracking/TrackerSwitch.cpp
 *
 * 설계 배경은 TrackerSwitch.hpp 상단을 참고하세요.
 * See the top of TrackerSwitch.hpp for the rationale.
 */

#include <AppMain/tracking/TrackerSwitch.hpp>

#include <AppMain/MainController.hpp>
#include <AppMain/tracking/IPathTracker.hpp>
#include <AppMain/tracking/TrackerFactory.hpp>

#include <algorithm>
#include <iostream>

namespace AppMain
{
namespace tracking
{

bool switchTracker(MainController& controller, const std::string& name)
{
    // 모르는 이름은 무시합니다. createTracker() 의 기본값 폴백에 맡기면
    // "stanley 오타 → 조용히 pure_pursuit" 가 되어 UI 가 상태를 오해합니다.
    // Reject unknown names here: relying on createTracker()'s fallback would
    // silently turn a typo into pure_pursuit and desync the UI.
    const std::vector<std::string> names = availableTrackerNames();
    if (std::find(names.begin(), names.end(), name) == names.end())
    {
        std::cerr << "[TrackerSwitch] unknown tracker '" << name
                  << "', ignored\n";
        return false;
    }

    const std::shared_ptr<IPathTracker> current = controller.getTracker();
    if (current && name == current->name())
    {
        // 이미 그 알고리즘입니다. 진행 상태를 버릴 이유가 없으므로 재생성하지
        // 않습니다. / Already active — do not recreate and lose progress.
        controller.setTrackerName(name);
        return true;
    }

    // RDDF 적재 전에는 이름만 기억해 둡니다. RddfLoader 가 적재 시점에
    // getTrackerName() 으로 이 선택을 사용합니다.
    // Before an RDDF is loaded only the name is recorded; RddfLoader picks it
    // up via getTrackerName() at load time.
    const std::vector<TrackerWaypoint> waypoints =
            controller.getTrackedWaypoints();
    if (!current && waypoints.empty())
    {
        controller.setTrackerName(name);
        std::cerr << "[TrackerSwitch] no track loaded yet; '" << name
                  << "' will be used when an RDDF arrives\n";
        return true;
    }

    std::shared_ptr<IPathTracker> tracker = createTracker(name);
    tracker->setParams(controller.getTrackerParams());
    if (!waypoints.empty())
    {
        tracker->setTrack(waypoints);
        // 완주 래치·경과 시간·조향 적분이 이전 트래커의 주행에서 이어지지
        // 않게 합니다 (RddfLoader 의 새 경로 적재와 같은 순서).
        // Clear the previous run (same order RddfLoader uses on a new path).
        controller.prepareForNewTrack();
    }
    controller.setTracker(tracker);
    controller.setTrackerName(name);
    std::cerr << "[TrackerSwitch] switched to '" << name << "' ("
              << waypoints.size() << " waypoints re-installed)\n";
    return true;
}

} // namespace tracking
} // namespace AppMain
