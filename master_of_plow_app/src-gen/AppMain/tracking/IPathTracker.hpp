/*
 * AppMain/tracking/IPathTracker.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 경로 추종 알고리즘이 지켜야 할 계약(인터페이스)입니다.
 *
 * 트래커가 하는 일은 딱 하나입니다. 매 제어 주기마다
 *   "지금 내 위치/자세" + "따라가야 할 경로"  ->  "액추에이터 명령"
 * 을 계산하는 것.
 *
 * 그 주변의 모든 배관 작업 — GPS를 ENU 좌표로 바꾸고, 명령을 시뮬레이터로
 * 보내고, 텔레메트리를 쏘고, 점수를 집계하는 일 — 은 TrackingLoop이 담당하며
 * 알고리즘을 바꿔도 그대로입니다. 그래서 새 알고리즘(Stanley, MPC 등)을
 * 추가하는 데 필요한 작업은 딱 두 가지입니다.
 *
 *   1. 이 인터페이스를 구현한 클래스를 tracking/impl/ 아래에 하나 만든다
 *   2. TrackerFactory.cpp의 레지스트리에 한 줄 추가한다
 *
 * 직접 상속하지 말고 PathTrackerBase를 상속하는 걸 권합니다. 웨이포인트
 * 보관 같은 반복 코드가 이미 구현되어 있어서, 실제로 여러분이 작성할 것은
 * update()와 isTrackComplete() 두 개뿐입니다.
 *
 * ★ 시작 전에 TrackerTypes.hpp 상단의 좌표계·부호 규약을 먼저 읽으세요.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The path-tracking contract. A tracker turns "where am I" plus "where is the
 * path" into actuator commands, once per control tick.
 *
 * Everything around it — GPS projection, command publishing, telemetry, scoring
 * — lives in TrackingLoop and does not change when the algorithm changes. So a
 * new algorithm (Stanley, MPC, ...) means one new class here plus one line in
 * TrackerFactory.cpp, and nothing else.
 *
 * Prefer deriving from PathTrackerBase, which already implements the waypoint
 * bookkeeping; then only update() and isTrackComplete() are actually yours.
 *
 * ★ Read the convention block at the top of TrackerTypes.hpp first. Getting the
 *   steering sign wrong is the single most common way to make a tracker drive in
 *   a circle.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_IPATHTRACKER_HPP
#define APPMAIN_TRACKING_IPATHTRACKER_HPP

#include <AppMain/tracking/TrackerTypes.hpp>

#include <cstddef>
#include <vector>

namespace AppMain
{
namespace tracking
{

class IPathTracker
{
public:
    virtual ~IPathTracker() = default;

    // ── 경로 / Path ─────────────────────────────────────────────────────────

    /**
     * 경로를 적재합니다. 내부의 진행 상태를 전부 초기화해야 합니다.
     * 한 세션 안에서 여러 번 호출될 수 있습니다 (참가자가 RDDF를 다시 올릴 때).
     *
     * Load a path. Must reset all internal progress state.
     */
    virtual void setTrack(const std::vector<TrackerWaypoint>& waypoints) = 0;
    virtual const std::vector<TrackerWaypoint>& getWaypoints() const = 0;
    virtual bool hasTrack() const = 0;
    virtual std::size_t waypointCount() const = 0;

    // ── 튜닝 / Tuning ───────────────────────────────────────────────────────

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * 런타임 튜닝 파라미터를 주입합니다. 기본 구현은 no-op — 파라미터가 없는
     * 트래커는 아무것도 안 해도 됩니다(추가만 하는 변경이라 기존 구현이
     * 깨지지 않습니다). PathTrackerBase 가 params_ 저장으로 override 합니다.
     * TrackingLoop 이 매 주기 호출하므로 가볍게 유지하세요.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Injects the runtime tuning parameters. The default is a no-op so
     * existing trackers keep compiling; PathTrackerBase overrides it to store
     * params_. Called by TrackingLoop every tick — keep it cheap.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual void setParams(const TrackerParams& params)
    {
        (void) params;
    }

    /** Freeze machine geometry for the current run. */
    virtual void setVehicleGeometry(const VehicleGeometry& geometry)
    {
        (void) geometry;
    }

    // ── 제어 / Control ──────────────────────────────────────────────────────

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * 제어 주기 1회. isTrackComplete()가 false인 동안, 대략
     * preferredPeriodMs() 간격으로 계속 호출됩니다.
     *
     * @param state  현재 차량의 위치·방향·속력
     * @param dtS    이전 호출로부터 실제 경과한 초 (항상 0보다 큼)
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * One control tick. Called at roughly preferredPeriodMs() intervals for as
     * long as isTrackComplete() is false.
     *
     * @param state  current vehicle pose and speed
     * @param dtS    wall-clock seconds since the previous call (never <= 0)
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual TrackerCommand update(const VehicleState& state, double dtS) = 0;

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * 경로를 다 주행했으면 true. 이 값이 true가 되면 루프는 새 명령 계산을
     * 멈추고 종료 시퀀스(기록 저장, track_complete 방송)를 시작합니다.
     *
     * 그래서 너무 일찍 true가 되면 안 됩니다. 특히 주행 시작 시점에 차량이
     * 우연히 마지막 웨이포인트 근처에 있는 경우를 반드시 걸러내세요.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * True once the vehicle has finished the path. The loop stops issuing new
     * commands and starts the finish sequence, so this must not latch early:
     * guard it against the vehicle merely being near the last waypoint at the
     * start of a run.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual bool isTrackComplete() const = 0;

    // ── 진단 / Diagnostics ──────────────────────────────────────────────────

    /**
     * 가장 최근 update()가 계산한 진단 정보.
     * Diagnostics for the tick that update() last computed.
     */
    virtual const TrackerDebug& lastDebug() const = 0;

    /**
     * 고정된 식별자. 로그와 TrackerFactory 조회에 쓰입니다.
     * Stable identifier, used for logs and TrackerFactory lookup.
     */
    virtual const char* name() const = 0;

    // ── 루프가 트래커에게 물어보는 값 / Tuning the loop asks about ──────────

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * 원하는 제어 주기(밀리초). 루프가 이 값을 시뮬레이션 배속으로 나눠
     * 쓰므로, 트래커는 "내 알고리즘의 자연스러운 주기"만 선언하면 됩니다.
     *
     * 10Hz(100ms)보다 촘촘하거나 느슨한 주기가 필요할 때만 재정의하세요.
     * 이 값만 바꾸면 TrackingLoop은 건드릴 필요가 없습니다.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Desired control period in milliseconds. The loop divides this by the
     * simulation speed multiplier, so a tracker only states its natural rate.
     * Override if the algorithm needs a tighter or looser tick than 10 Hz.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual int preferredPeriodMs() const
    {
        return 100;
    }
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_IPATHTRACKER_HPP
