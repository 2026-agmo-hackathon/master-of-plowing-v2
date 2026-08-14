/*
 * AppMain/tracking/impl/StanleyTracker.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 최소 구현 Stanley 조향 법칙입니다. IPathTracker 뒤에 두 번째 알고리즘을
 * 붙이면 어떤 모습이 되는지 보여주는 예제로 제공합니다.
 *
 * PurePursuitTracker가 약 700줄인데 이쪽은 약 100줄입니다. 그 차이는 제어
 * 법칙의 복잡도가 아니라 거의 전부 "예외 상황 대응(robustness)"입니다.
 * 새 알고리즘을 시작할 때 이 파일을 복사해서 출발하면 편합니다.
 *
 *   delta = -psi_e + atan(k * e / (v + eps))
 *
 *     psi_e  방향 오차 (경로 접선 방향 - 차량 방향)
 *     e      "앞바퀴 축" 기준의 LTD(횡방향 편차)
 *            (뒷바퀴 축에서 재면 다른 제어기가 됩니다. 앞축에서 재는 것이
 *             Stanley를 Stanley로 만드는 핵심 선택입니다.)
 *     k      LTD(횡방향 편차) 게인
 *     eps    저속에서 분모가 0이 되지 않게 하는 완화항
 *
 * 앞의 마이너스 부호는 이 프로젝트의 좌표계 규약 때문입니다. 조향 양수는
 * 좌회전이고, 좌회전은 heading을 감소시킵니다. TrackerTypes.hpp 참고.
 *
 * ── 일부러 남겨 둔 한계 (여기가 개선 지점입니다) ──
 *   - 최근접 웨이포인트 탐색이 단조 증가 인덱스에서 전방으로만 진행합니다.
 *     직선 레인에는 충분하지만, K턴처럼 경로가 자기 자신과 교차하는 구간에서는
 *     PurePursuitTracker의 인덱스 관리보다 약합니다.
 *   - 진행 인덱스가 **앞바퀴 축** 기준이라 전진/후진 전환도 한 휠베이스(2.05m)
 *     일찍 감지됩니다. 그래서 SpeedController가 아직 정지를 위해 제동하는 동안
 *     (= 차량이 아직 앞으로 굴러가는 동안) 조향 부호가 이미 후진 기준으로
 *     뒤집혀 잠깐 반대로 꺾습니다.
 *     ★ 주의: 이걸 PurePursuitTracker처럼 "탐색 윈도우를 F/R 경계에서 자르는"
 *     방식으로 고치면 안 됩니다. PurePursuit에는 경계에서 인덱스를 무조건
 *     전진시키는 별도 로직이 있지만 Stanley에는 없어서, 윈도우만 자르면
 *     진행 인덱스가 경계에 영구히 갇힙니다.
 *   - 작업기 플래그를 차량 진행 인덱스에서 그대로 가져옵니다. 그래서 쟁기가
 *     **약 5m**(작업기 오프셋 3m + 휠베이스 2.05m) 일찍 내려가고 일찍
 *     올라갑니다. 진행 인덱스를 앞바퀴 축에서 재기 때문에 오프셋 3m에 휠베이스가
 *     더해집니다. PurePursuitTracker는 작업기 위치를 따로 모델링합니다.
 *   - 곡률 피드포워드가 없어 급한 원호 구간에서 조향이 부족합니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Minimal Stanley steering law, provided as a worked example of a second
 * algorithm behind IPathTracker. Roughly 100 lines against PurePursuitTracker's
 * 700 — the difference is almost entirely robustness, not the control law. Copy
 * this file as the starting point for your own algorithm.
 *
 *   delta = -psi_e + atan(k * e / (v + eps))
 *
 *     psi_e  heading error   (path tangent - vehicle heading)
 *     e      lateral deviation (LTD) at the FRONT axle (that is Stanley's defining
 *            choice; measuring at the rear axle gives a different controller)
 *     k      lateral-deviation gain
 *
 * The leading minus is this project's frame convention: positive steer means
 * left, and left DECREASES heading. See TrackerTypes.hpp.
 *
 * Known limitations, left in on purpose so they are visible:
 *   - Nearest-waypoint search is forward-only from a monotonic progress index.
 *     That is enough for straight lanes but is weaker than PurePursuitTracker's
 *     bookkeeping where a K-turn makes the path self-intersect.
 *   - The progress index is measured at the FRONT axle, so a forward/reverse
 *     change is also detected one wheelbase (2.05m) early: the steering sign
 *     flips to reverse semantics while SpeedController is still braking to a
 *     stop, i.e. while the vehicle is still rolling forward, so it briefly
 *     steers the wrong way.
 *     NOTE: do not "fix" this by clipping the search window at the F/R boundary
 *     the way PurePursuitTracker does. PurePursuit has a separate unconditional
 *     advance at the boundary; Stanley does not, so clipping alone would pin the
 *     progress index at the boundary forever.
 *   - The implement flag is taken from the vehicle's progress index, so the plow
 *     drops and lifts about **5m** early (3m implement offset + 2.05m wheelbase,
 *     because the progress index is measured at the front axle).
 *     PurePursuitTracker models the implement position separately.
 *   - No curvature feedforward, so tight arcs are under-steered.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_IMPL_STANLEYTRACKER_HPP
#define APPMAIN_TRACKING_IMPL_STANLEYTRACKER_HPP

#include <AppMain/tracking/PathTrackerBase.hpp>
#include <AppMain/tracking/SpeedController.hpp>

#include <cstddef>

namespace AppMain
{
namespace tracking
{

class StanleyTracker: public PathTrackerBase
{
public:
    StanleyTracker();

    const char* name() const override
    {
        return "stanley";
    }

    TrackerCommand update(const VehicleState& state, double dtS) override;

    void setVehicleGeometry(const VehicleGeometry& geometry) override
    {
        PathTrackerBase::setVehicleGeometry(geometry);
        speed_.setVehicleGeometry(geometry);
    }

    bool isTrackComplete() const override
    {
        return trackComplete_;
    }

    // 종방향 게인은 SpeedController(speed_)가 트래커의 private 멤버라서,
    // 파라미터를 받는 순간 여기서 전달합니다.
    // The longitudinal gains are forwarded here because SpeedController
    // (speed_) is a private member of the tracker.
    void setParams(const TrackerParams& params) override
    {
        PathTrackerBase::setParams(params);
        speed_.setGains(params.longKp, params.longKi, params.longKd);
    }

protected:
    void onTrackSet() override;

private:
    /**
     * 주행 1회분 상태 전체를 초기화합니다. 생성자와 onTrackSet()이 공유하며,
     * 생성자에서 virtual 함수를 부르지 않기 위해 비virtual로 둡니다.
     *
     * Clears all per-run state. Shared by the constructor and onTrackSet();
     * non-virtual so the constructor never dispatches virtually.
     */
    void resetState();

    /**
     * progressIndex_ 를 기준으로 전방만 보는 최근접 웨이포인트 탐색입니다.
     * 뒤를 보지 않기 때문에 진행 인덱스가 되돌아갈 수 없습니다.
     *
     * Forward-only nearest-waypoint search anchored on progressIndex_, so path
     * progress can never regress.
     */
    std::size_t findClosestIndex(double x, double z) const;

    bool trackComplete_;
    // 차량이 경로 시작점에 도달했는가 (완주 오판 방지용).
    // Has the vehicle reached the start of the path (guards false completion).
    bool pathStarted_;
    // 단조 증가: 경로 진행도는 절대 되돌아가지 않습니다.
    // Monotonic: path progress never regresses.
    std::size_t progressIndex_;
    // 종방향 제어(기어/스로틀/브레이크)는 공유 컴포넌트에 위임합니다.
    // Longitudinal control is delegated to the shared component.
    SpeedController speed_;
    // 완주 후 계속 내보내는 마지막 명령. / The command held after completion.
    TrackerCommand lastValidCmd_;

    static constexpr double WHEELBASE_M = 2.05;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_IMPL_STANLEYTRACKER_HPP
