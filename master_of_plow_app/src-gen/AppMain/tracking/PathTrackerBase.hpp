/*
 * AppMain/tracking/PathTrackerBase.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * IPathTracker의 "반복 코드" 절반을 미리 구현해 둔 기반 클래스입니다.
 * 경로(waypoints_)와 진단 기록(debug_)을 보관해 주므로, 여러분이 실제로
 * 작성해야 하는 것은 update()와 isTrackComplete() 두 개뿐입니다.
 *
 * 가장 작은 트래커의 뼈대:
 *
 *   class MyTracker : public PathTrackerBase {
 *   public:
 *       const char* name() const override { return "my_tracker"; }
 *       bool isTrackComplete() const override { return done_; }
 *       TrackerCommand update(const VehicleState& s, double dtS) override {
 *           // waypoints_ 에 경로가 들어 있습니다.
 *           // 진단값은 debug_ 에 채워 넣으세요.
 *       }
 *   protected:
 *       void onTrackSet() override { done_ = false; }  // 내 상태 초기화
 *   private:
 *       bool done_ = false;
 *   };
 *
 * 새 트래커를 만든 뒤에는 TrackerFactory.cpp의 레지스트리에 한 줄만 추가하면
 * `PATH_TRACKER=my_tracker` 환경변수로 바로 돌려볼 수 있습니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Boilerplate half of IPathTracker: stores the path and the debug record so a
 * concrete tracker only has to implement update() and isTrackComplete().
 *
 * Minimal tracker:
 *
 *   class MyTracker : public PathTrackerBase {
 *   public:
 *       const char* name() const override { return "my_tracker"; }
 *       bool isTrackComplete() const override { return done_; }
 *       TrackerCommand update(const VehicleState& s, double dtS) override {
 *           // waypoints_ holds the path; write diagnostics into debug_
 *       }
 *   protected:
 *       void onTrackSet() override { done_ = false; }  // reset your own state
 *   private:
 *       bool done_ = false;
 *   };
 *
 * Once it exists, add one line to the registry in TrackerFactory.cpp and run it
 * with PATH_TRACKER=my_tracker.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_PATHTRACKERBASE_HPP
#define APPMAIN_TRACKING_PATHTRACKERBASE_HPP

#include <AppMain/tracking/IPathTracker.hpp>

namespace AppMain
{
namespace tracking
{

class PathTrackerBase: public IPathTracker
{
public:
    void setTrack(const std::vector<TrackerWaypoint>& waypoints) override
    {
        waypoints_ = waypoints;
        debug_ = TrackerDebug{};
        onTrackSet();
    }

    const std::vector<TrackerWaypoint>& getWaypoints() const override
    {
        return waypoints_;
    }

    bool hasTrack() const override
    {
        return !waypoints_.empty();
    }

    std::size_t waypointCount() const override
    {
        return waypoints_.size();
    }

    const TrackerDebug& lastDebug() const override
    {
        return debug_;
    }

    void setParams(const TrackerParams& params) override
    {
        params_ = params;
    }

    void setVehicleGeometry(const VehicleGeometry& geometry) override
    {
        if (geometry.valid()) geometry_ = geometry;
    }

protected:
    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * setTrack()의 마지막에, waypoints_에 새 경로가 이미 담긴 상태로
     * 호출됩니다. 주행 1회분에 해당하는 모든 내부 상태를 여기서 초기화하세요.
     *
     * 중요: 트래커 인스턴스는 같은 세션 안에서 재사용됩니다. 여기서 초기화를
     * 빼먹으면 두 번째 주행이 첫 주행의 진행 상태를 물려받아 오작동합니다.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Called at the end of setTrack(), once waypoints_ holds the new path.
     * Reset every piece of per-run state here — a tracker instance is reused
     * across runs of the same session.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    virtual void onTrackSet()
    {
    }

    // 적재된 경로. / The loaded path.
    std::vector<TrackerWaypoint> waypoints_;
    // 매 주기 채워 넣는 진단 기록. / Diagnostics record, filled in each tick.
    TrackerDebug debug_;
    // 런타임 튜닝 파라미터. 기본값 = 기존 constexpr 상수와 동일.
    // Runtime tuning parameters; defaults equal the old constexpr constants.
    TrackerParams params_;
    VehicleGeometry geometry_;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_PATHTRACKERBASE_HPP
