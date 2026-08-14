/*
 * AppMain/tracking/TrackerSwitch.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 경로추종 알고리즘을 런타임에 전환합니다 (튜닝 UI 의 set_tracker 경로).
 *
 * RDDF 를 다시 읽지 않습니다 — RddfLoader 가 MainController 에 보관해 둔 ENU
 * 웨이포인트(getTrackedWaypoints)를 새 트래커에 그대로 적재합니다. 손으로 쓴
 * 코드가 PROTECTED REGION 밖에 남지 않도록, WebSocketEndPointImpl 의 리전에서는
 * 이 함수를 호출만 합니다.
 *
 * ★ 주행 중 전환의 함정: 두 트래커 모두 pathStarted_ 가드(0번 웨이포인트 3m
 *   이내 접근 이력)가 있어, 경로 중간에서 전환하면 새 트래커는 끝까지 가도
 *   완주로 판정하지 못합니다. UI 가 이 사실을 경고로 표시합니다 — 여기서
 *   임의로 가드를 우회하지 않습니다(가드는 오판 방지 장치입니다).
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Switches the path-tracking algorithm at runtime (the tuning UI's set_tracker
 * path). The RDDF is not re-read: the ENU waypoints RddfLoader stored on
 * MainController are installed into the freshly created tracker. The
 * WebSocketEndPointImpl protected region only calls this function, keeping the
 * hand-written logic out of regenerated code.
 *
 * Mid-run caveat: both trackers carry a pathStarted_ guard (must have come
 * within 3 m of waypoint 0), so a tracker swapped in mid-path will never report
 * completion. The UI warns about this; the guard is deliberately not bypassed
 * here because it exists to prevent false completions.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_TRACKERSWITCH_HPP
#define APPMAIN_TRACKING_TRACKERSWITCH_HPP

#include <string>

namespace AppMain
{

class MainController;

namespace tracking
{

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * name 의 트래커로 전환합니다. availableTrackerNames() 에 없는 이름이면
 * 아무것도 바꾸지 않고 false 를 돌려줍니다. 전환 순서:
 *   createTracker(name) → setParams(현재 파라미터) → setTrack(보관된 ENU 경로)
 *   → prepareForNewTrack() → setTracker(새 트래커) → setTrackerName(name)
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Switches to the tracker named `name`. Unknown names change nothing and
 * return false. Sequence: create → setParams(current) → setTrack(stored ENU
 * path) → prepareForNewTrack() → setTracker(new) → setTrackerName(name).
 * └────────────────────────────────────────────────────────────────────────────┘
 */
bool switchTracker(MainController& controller, const std::string& name);

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_TRACKERSWITCH_HPP
