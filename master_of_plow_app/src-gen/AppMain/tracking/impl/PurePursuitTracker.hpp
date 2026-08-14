/*
 * AppMain/tracking/impl/PurePursuitTracker.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 스켈레톤에 기본으로 딸려 오는 기준 트래커입니다.
 *
 * 이름은 Pure Pursuit이지만 교과서적인 Pure Pursuit는 아닙니다. 조향 명령은
 * 다섯 개 항의 가중합이고, 그중 Pure Pursuit 기하 항의 기여도는 45%뿐입니다.
 *
 *   1. 곡률 피드포워드 (cf)      경로 자체의 곡률을 따라가기 위한 기하학적 조향
 *   2. HDE 항 (hdg)        차량 방향을 경로 접선에 맞춤 (Stanley 계열)
 *   3. LTD(횡방향 편차) 항 (cte)      경로에서 벗어난 거리를 되돌림 (Stanley 계열)
 *   4. Pure Pursuit 항 (pp)      전방 목표점을 향한 원호 (예측적, 가중치 0.45)
 *   5. yaw rate 감쇠 항          회전 각속도를 상쇄해 오버슈트를 줄임 (D 항)
 *
 * 자세한 설명은 .cpp의 (e2) 블록 주석에 있습니다.
 *
 * ── 아래 멤버 변수가 많아 보이는 이유 ──
 *
 * 대부분은 K턴(전진 -> 후진 -> 전진으로 방향을 바꾸는 선회)에서 "경로 진행도가
 * 뒤로 되돌아가지 않게" 하기 위해 존재합니다. K턴 구간에서는 경로가 자기 자신과
 * 교차하기 때문에, 단순히 "가장 가까운 웨이포인트"를 찾으면 이미 지나온 구간의
 * 점으로 인덱스가 튀어 버립니다. 그러면 기어와 작업기 지시가 뒤엉킵니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The reference tracker shipped with the skeleton.
 *
 * Despite the name it is not textbook Pure Pursuit — the steering command is a
 * weighted sum of five terms (curvature feedforward, heading error, lateral-deviation
 * error, the Pure Pursuit arc, and a yaw-rate damper), of which the Pure Pursuit
 * geometry contributes 45%. See the block comment above the sum in the .cpp.
 *
 * Most of the state below exists to keep path progress monotonic across K-turns,
 * where the path self-intersects and a naive "nearest waypoint" search jumps
 * backwards onto the segment the vehicle already drove.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_IMPL_PUREPURSUITTRACKER_HPP
#define APPMAIN_TRACKING_IMPL_PUREPURSUITTRACKER_HPP

#include <AppMain/tracking/PathTrackerBase.hpp>
#include <AppMain/tracking/SpeedController.hpp>

#include <cstddef>

namespace AppMain
{
namespace tracking
{

class PurePursuitTracker: public PathTrackerBase
{
public:
    PurePursuitTracker();

    const char* name() const override
    {
        return "pure_pursuit";
    }

    TrackerCommand update(const VehicleState& state, double dtS) override;

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

    void setVehicleGeometry(const VehicleGeometry& geometry) override
    {
        PathTrackerBase::setVehicleGeometry(geometry);
        speed_.setVehicleGeometry(geometry);
    }

protected:
    void onTrackSet() override;

private:
    /**
     * 주행 1회분 상태 전체를 초기화합니다. 생성자와 onTrackSet()이 공유합니다.
     * Clears all per-run state. Shared by the constructor and onTrackSet().
     */
    void resetState();

    bool trackComplete_;
    // 차량이 첫 웨이포인트 3m 이내에 들어온 적이 있는가.
    // True once the vehicle comes within 3m of the first waypoint.
    bool pathStarted_;

    // ── 경로 진행도 인덱스 / Path-progress indices ──────────────────────────
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 인덱스가 하나가 아니라 넷인 이유:
    //  (1) 차량과 작업기는 3m 떨어져 있어서 같은 웨이포인트에 도달하는 시점이
    //      다릅니다.
    //  (2) 기어 변경은 "차량"이 경계를 넘을 때 일어나야 하고, 작업기 올림/내림은
    //      "작업기"가 경계를 넘을 때 일어나야 합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Four indices instead of one because the vehicle and the implement are 3m
    // apart and reach any given waypoint at different times, and because gear
    // changes must fire on the vehicle while the implement flag must fire on
    // the implement.
    // └────────────────────────────────────────────────────────────────────────┘

    // 차량에 가장 가까운 웨이포인트. / Waypoint nearest the vehicle.
    std::size_t lastClosestIndex_;
    // 작업기 위치 기준으로 현재 따르는 웨이포인트. / Active waypoint per implement position.
    std::size_t commandIndex_;
    // 차량 위치 기준으로 현재 따르는 웨이포인트 (기어/속도를 결정).
    // Active waypoint per vehicle position (drives gear/speed).
    std::size_t vehicleCommandIndex_;
    // 단조 증가 하한선: 경로 진행도는 절대 되돌아가지 않습니다.
    // Monotonic floor: path progress never regresses.
    std::size_t lastImplementClosestIndex_;
    // 지금까지 도달한 최고 인덱스. 완주 판정의 게이트로 씁니다.
    // High-water mark, gates completion.
    std::size_t maxReachedCommandIndex_;

    // ── 종방향 제어 / Longitudinal control ──────────────────────────────────
    // 기어/스로틀/브레이크와 "후진 전 정지" 처리를 담당합니다.
    // Gear/throttle/brake, including the stop-before-reverse handling.
    SpeedController speed_;

    // ── 미분·변화율 제한 상태 / Derivative and rate-limit state ─────────────
    // yaw rate를 수치 미분으로 구하기 위한 직전 방향값.
    // Previous heading, for the yaw-rate numerical derivative.
    double prevHeading_;
    bool prevHeadingValid_;
    std::int64_t lastPoseSampleId_ = -1;
    // 방향값이 마지막으로 "바뀐" 뒤 흐른 시간과, 그때 구한 yaw rate.
    // 제어 주기와 실제 센서 샘플 갱신이 어긋날 수 있으므로 둘 다 필요합니다.
    // Seconds since the heading last CHANGED, and the yaw rate computed then.
    // Both are needed because control ticks and sensor sample refresh can differ.
    double headingAgeS_;
    double lastYawRate_;
    // 최근접 인덱스 점프 뒤 목표 인덱스가 급히 튀는 것을 제한하기 위한 값.
    // For rate-limiting lookahead after closest-index jumps.
    std::size_t prevTargetIndex_;
    bool prevTargetIndexValid_;

    // 완주 후 계속 내보내는 마지막 명령. / The command held once the track is complete.
    TrackerCommand lastValidCmd_;

public:
    // 트랙터 휠베이스(앞뒤 축 간 거리). 자전거 모델의 기본 파라미터입니다.
    // Tractor wheelbase; the basic parameter of the bicycle model.
    static constexpr double WHEELBASE_M = 2.05;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 앞바퀴가 실제로 도달할 수 있는 최대 각도입니다. 조향 지령은 여기서
    // 잘립니다.
    //
    // ★ 이 값은 반드시 실측이어야 합니다. 여기보다 큰 값을 넣으면 제어기는
    // 존재하지 않는 각도를 계속 요구하게 되고, 그 순간 조향은 서보가 아니라
    // 온/오프 릴레이가 됩니다. 릴레이가 부호를 뒤집는 데 걸리는 시간(락투락
    // 약 4~5초, 그 사이 차량은 4m 넘게 이동) 동안 차량은 경로를 가로질러
    // 반대편으로 넘어갑니다 — 이것이 S자 사행의 정체입니다.
    //
    // 에뮬레이터 실측(1,127초 주행, 이동 중 GPS 1,060샘플):
    //   - 지령을 32°로 고정한 채 모터를 ±600(최대)으로 20초 이상 물려도
    //     엔코더 각도는 우측 -19.6°, 좌측 +22.9°에서 더 가지 않음 = 엔드스톱
    //   - GPS 방위 변화율로 역산한 진짜 각도: 엔드스톱에서 -25.6° / +25.3°,
    //     전체 주행의 p5/p95 = -26.4° / +26.1°
    //   -> 물리 한계는 ±32°가 아니라 ±26°. 지령이 32°에 붙어 있던 시간이
    //      전체 틱의 55.6%, 모터가 ±600에 물려 있던 시간이 55.0%였습니다.
    //
    // 0.44 rad = 25.2°. 엔드스톱보다 약간 안쪽에 두어, 기구가 멈추기 전에
    // 안티 와인드업이 먼저 걸리게 합니다.
    //
    // 다른 기계로 옮길 때는 이 값을 반드시 다시 재세요. 재는 법은 간단합니다.
    // 조향을 한쪽 끝까지 물린 채 정속으로 원을 그리고, 반지름 R을 재서
    // delta = atan(WHEELBASE_M / R).
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The largest front-wheel angle the machine can actually reach; the steering
    // demand is clamped to it.
    //
    // This MUST be a measured number. Set it above what the mechanism can do and
    // the controller spends its time demanding an angle that does not exist, at
    // which point steering stops being a servo and becomes an on/off relay. The
    // time that relay needs to change sign (lock to lock is 4-5 s, over 4 m of
    // travel) is long enough for the vehicle to cross the path and come out the
    // other side — that is the S-weave.
    //
    // Measured on the emulator (1,127 s run, 1,060 moving GPS samples):
    //   - holding the demand at 32 deg with the motor pinned at +/-600 for over
    //     20 s, the encoder angle stops at -19.6 deg right / +22.9 deg left
    //   - back-solving the true angle from the GPS heading rate gives -25.6 /
    //     +25.3 deg at those stops, and p5/p95 over the whole run of -26.4/+26.1
    //   -> the physical limit is +/-26 deg, not +/-32. The demand sat at the
    //      32 deg clamp on 55.6% of ticks and the motor at +/-600 on 55.0%.
    //
    // 0.44 rad = 25.2 deg, just inside the stop so anti-windup engages before
    // the mechanism does.
    //
    // Re-measure this for any other machine: drive a steady circle at full lock,
    // measure the radius R, then delta = atan(WHEELBASE_M / R).
    // └────────────────────────────────────────────────────────────────────────┘
    static constexpr double WHEEL_MAX_RAD = 0.44;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_IMPL_PUREPURSUITTRACKER_HPP
