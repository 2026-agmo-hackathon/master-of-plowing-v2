/*
 * AppMain/tracking/impl/StanleyTracker.cpp
 *
 * 제어 법칙과 알려진 한계는 StanleyTracker.hpp 상단에 정리해 두었습니다.
 * The control law and its known limitations are documented in StanleyTracker.hpp.
 */

#include <AppMain/tracking/impl/StanleyTracker.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace AppMain {
namespace tracking {

namespace {

// K_LTD 와 SOFTENING_MPS 는 TrackerParams(params_.stKLtd / params_.stSofteningMps)
// 로 옮겨 런타임 튜닝이 가능해졌습니다. 기본값은 기존 상수와 동일합니다.
// K_LTD and SOFTENING_MPS moved into TrackerParams (params_.stKLtd /
// params_.stSofteningMps) for runtime tuning; defaults equal the old constants.

// 마지막 웨이포인트에 이만큼 가까워지면 완주로 봅니다.
// Within this distance of the final waypoint counts as finished.
constexpr double GOAL_TOLERANCE_M = 1.0;
// 0번 웨이포인트에 이만큼 접근하면 "주행 시작"으로 간주합니다.
// Coming this close to waypoint 0 counts as having started the path.
constexpr double PATH_START_TOLERANCE_M = 3.0;
// 조향 명령 상한. PurePursuitTracker와 동일한 값입니다.
// Steering command cap, matching PurePursuitTracker.
constexpr double STEER_CMD_CAP = 1.0;
// 최근접 탐색에서 앞으로 몇 개의 웨이포인트까지 볼지.
// How many waypoints ahead of progressIndex_ to consider.
constexpr int SEARCH_FORWARD = 10;
// 완주 판정의 진행도 게이트: 마지막 이 개수 안에 들어와 있어야 합니다. 거리만
// 보면 자기교차 경로에서 속기 때문입니다. (위 SEARCH_FORWARD 와는 무관한 값)
// Progress gate for completion: must be within this many waypoints of the end.
// Distance alone is fooled by self-crossing paths. Unrelated to SEARCH_FORWARD.
constexpr std::size_t GOAL_GATE_WPS = 10;
constexpr double KMH_TO_MS = 1.0 / 3.6;

/**
 * 각도를 [-pi, pi] 범위로 정규화합니다. 방향 오차를 다룰 때 필수입니다.
 * Wraps an angle into [-pi, pi]; essential when handling heading errors.
 */
double wrapPi(double a)
{
    while (a >  M_PI) a -= 2.0 * M_PI;
    while (a < -M_PI) a += 2.0 * M_PI;
    return a;
}

} // namespace

StanleyTracker::StanleyTracker()
{
    // 생성자에서는 virtual인 onTrackSet()이 아니라 비virtual resetState()를
    // 부릅니다. 생성 중 virtual 디스패치는 파생 클래스의 재정의에 닿지 않아
    // 나중에 이 클래스를 상속하면 조용히 깨지는 함정입니다.
    // Call the non-virtual resetState(), not the virtual onTrackSet(): virtual
    // dispatch during construction never reaches a derived override, which
    // silently breaks anyone who later inherits from this class.
    resetState();
}

void StanleyTracker::onTrackSet()
{
    resetState();
}

void StanleyTracker::resetState()
{
    // 주행 1회분 상태 전체를 초기화합니다. 하나라도 빼먹으면 두 번째 주행이
    // 첫 주행의 진행 상태를 물려받습니다.
    // Reset all per-run state; missing one makes the second run inherit the first.
    trackComplete_ = false;
    pathStarted_ = false;
    progressIndex_ = 0;
    speed_.reset();
    lastValidCmd_ = TrackerCommand{0.0, 0.0, 0.0, 1, 0};
}

std::size_t StanleyTracker::findClosestIndex(double x, double z) const
{
    // 호출하는 쪽에서 경로가 비어 있지 않음을 보장합니다. 비어 있으면 아래
    // n - 1 이 unsigned 언더플로로 SIZE_MAX가 됩니다.
    // The caller guarantees a non-empty path; otherwise n - 1 below underflows.
    const std::size_t n = waypoints_.size();
    const std::size_t hi = std::min(n - 1,
        progressIndex_ + static_cast<std::size_t>(SEARCH_FORWARD));

    // 하한이 progressIndex_ 이므로 뒤로는 절대 가지 않습니다.
    // The lower bound is progressIndex_, so this can never move backwards.
    std::size_t best = progressIndex_;
    double bestDistSq = std::numeric_limits<double>::infinity();
    for (std::size_t i = progressIndex_; i <= hi; ++i) {
        const double dx = waypoints_[i].x - x;
        const double dz = waypoints_[i].z - z;
        const double d2 = dx * dx + dz * dz;
        if (d2 < bestDistSq) {
            bestDistSq = d2;
            best = i;
        }
    }
    return best;
}

TrackerCommand StanleyTracker::update(const VehicleState& state, double dtS)
{
    // 경로가 없으면 아무것도 하지 않습니다. / No path: stay inert.
    if (waypoints_.empty()) {
        return TrackerCommand{0.0, 0.0, 0.0, 1, 0};
    }
    // 이미 완주했으면 마지막 명령을 유지합니다. / Already finished: hold the last command.
    if (trackComplete_) {
        return lastValidCmd_;
    }

    const std::size_t n = waypoints_.size();

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 주행 시작 가드: 차량이 실제로 경로 시작점에 도달하기 전에는 완주 판정이
    // 걸리지 않게 막습니다. 이게 없으면, 이전 주행의 끝 지점 근처에서 새 세션을
    // 시작할 때 즉시 "완주"로 오판합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Path-start guard: do not allow completion to latch before the vehicle has
    // actually reached the beginning of the path. Without it, a new session that
    // starts near the previous run's endpoint completes instantly.
    // └────────────────────────────────────────────────────────────────────────┘
    if (!pathStarted_) {
        const double dx0 = state.x - waypoints_[0].x;
        const double dz0 = state.z - waypoints_[0].z;
        if (std::sqrt(dx0 * dx0 + dz0 * dz0) < PATH_START_TOLERANCE_M) {
            pathStarted_ = true;
        }
    }

    // Stanley는 LTD(횡방향 편차)를 뒷바퀴 축이 아니라 앞바퀴 축에서 측정합니다.
    // 이것이 Stanley를 다른 제어기와 구분하는 지점입니다.
    // Stanley measures lateral deviation (LTD) at the front axle, not the rear.
    const double frontX = state.x + std::cos(state.headingRad) * geometry_.wheelbaseM;
    const double frontZ = state.z + std::sin(state.headingRad) * geometry_.wheelbaseM;

    progressIndex_ = findClosestIndex(frontX, frontZ);
    const TrackerWaypoint& wp = waypoints_[progressIndex_];

    // 완주 판정: 마지막 웨이포인트 근처이고, 실제로 경로 끝까지 진행했을 때.
    // 두 조건이 모두 필요합니다. 경로가 자기 자신과 교차하면 "가깝다"만으로는
    // 속기 때문입니다.
    // Completion: near the last waypoint, and actually at the end of the path —
    // proximity alone is fooled by self-crossing paths.
    {
        const TrackerWaypoint& last = waypoints_.back();
        const double dx = last.x - state.x;
        const double dz = last.z - state.z;
        if (pathStarted_ && std::sqrt(dx * dx + dz * dz) < GOAL_TOLERANCE_M
            && progressIndex_ + GOAL_GATE_WPS >= n) {
            trackComplete_ = true;
            return lastValidCmd_;
        }
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 경로 접선 방향. 보통은 현재 웨이포인트에서 "나가는" 구간을 씁니다.
    //
    // 그런데 마지막 웨이포인트에는 나가는 구간이 없습니다. 이때 segA == segB로
    // 두면 구간 길이가 0이 되어 아래 if 문이 건너뛰어지고, LTD와 방향
    // 오차가 둘 다 0으로 접혀 **조향이 0으로 얼어붙습니다**. 게다가 진단값
    // lateralDeviationM도 0으로 나가서 CSV가 "오차 없음"이라고 거짓 보고합니다.
    //
    // progressIndex_ 는 앞바퀴 축 기준이므로 뒷바퀴가 목표까지 아직 휠베이스
    // (2.05 m)만큼 남았을 때 이미 마지막 인덱스에 도달합니다. 즉 최종 접근
    // 1-2 m 구간이 통째로 무보정이 됩니다. 완주 판정 허용 오차가 1 m뿐이라,
    // 그 시점에 LTD(횡방향 편차)가 1 m를 넘으면 목표를 스쳐 지나가 완주하지
    // 못할 수 있습니다.
    //
    // 그래서 마지막에서는 "들어오는" 구간으로 대체합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Path tangent: normally the segment leaving the current waypoint. At the
    // last waypoint there is no such segment, and letting segA == segB makes
    // segLen 0, which collapses both lateralDeviation and the heading error to 0 —
    // steering freezes and the reported lateralDeviationM lies about the error.
    //
    // Because progressIndex_ is measured at the front axle, it reaches the last
    // index while the rear axle is still one wheelbase (2.05 m) short of the
    // goal, so the whole final approach would run uncorrected. With only 1 m of
    // goal tolerance, more than 1 m of lateral error at that point can make the
    // vehicle miss the goal and never complete.
    //
    // Fall back to the segment arriving at the last waypoint instead.
    // └────────────────────────────────────────────────────────────────────────┘
    std::size_t segA = progressIndex_;
    std::size_t segB = progressIndex_ + 1;
    if (segB >= n) {
        segB = progressIndex_;
        if (progressIndex_ > 0) {
            segA = progressIndex_ - 1;
        }
    }
    const double sdx = waypoints_[segB].x - waypoints_[segA].x;
    const double sdz = waypoints_[segB].z - waypoints_[segA].z;
    const double segLen = std::sqrt(sdx * sdx + sdz * sdz);

    double lateralDeviation = 0.0;
    double segHeading = state.headingRad;
    if (segLen > 1e-9) {
        const double ndx = sdx / segLen;
        const double ndz = sdz / segLen;
        const double vpx = frontX - waypoints_[segA].x;
        const double vpz = frontZ - waypoints_[segA].z;
        // 2D 외적으로 부호 있는 횡방향 거리를 얻습니다. 차량이 +z쪽에 있으면 양수.
        // A 2D cross product gives the signed lateral distance; + = vehicle on the +z side.
        lateralDeviation = ndx * vpz - ndz * vpx;
        segHeading = std::atan2(sdz, sdx);
    }

    // 방향 오차. 후진 구간에서는 차량이 경로 진행 방향의 반대를 향하므로
    // 목표 방향이 pi 만큼 돌아갑니다.
    // Heading error. On a reverse segment the vehicle faces opposite the
    // direction of path progress, so the desired heading is rotated by pi.
    double headingError = wrapPi(segHeading - state.headingRad);
    if (wp.speed < 0.0) {
        headingError = wrapPi(headingError + M_PI);
    }

    // Stanley 제어 법칙: delta = -psi_e + atan(k * e / (v + eps))
    // 마이너스 부호는 좌표계 규약 때문입니다(TrackerTypes.hpp 참고).
    // The minus follows from the frame convention; see TrackerTypes.hpp.
    const double hdeTerm = -headingError;
    const double ltdTerm = std::atan(params_.stKLtd * lateralDeviation
                                     / (std::fabs(state.speedMps) + params_.stSofteningMps));
    double steerAngle = std::clamp(hdeTerm + ltdTerm,
                                   -geometry_.maxWheelAngleRad,
                                   geometry_.maxWheelAngleRad);

    // 위의 모든 항은 "전진 기준"으로 계산됐습니다. 후진이면 부호를 뒤집습니다.
    // All terms above are written in forward semantics; flip when reversing.
    if (wp.speed < 0.0) {
        steerAngle = -steerAngle;
    }

    // 기어/스로틀/브레이크는 공유 컴포넌트가 처리합니다. 트래커는 "어느
    // 웨이포인트의 속도를 따를지"만 정하면 됩니다.
    // Gear/throttle/brake come from the shared component; the tracker only picks
    // which waypoint's speed to obey.
    const SpeedController::Output longitudinal = speed_.update(wp.speed,
            state.speedMps, dtS, state.poseSampleId);

    // 진단 정보. 이름 있는 필드는 계약이므로 반드시 채우고, 알고리즘 고유
    // 항목은 extras에 넣습니다.
    // Diagnostics: fill the contract fields, put algorithm specifics in extras.
    debug_.lateralDeviationM    = lateralDeviation;
    debug_.headingErrorRad  = wrapPi(segHeading - state.headingRad);
    debug_.speedErrMps    = wp.speed * KMH_TO_MS - state.speedMps;
    debug_.targetSpeedKmh = wp.speed;
    debug_.targetIdx      = static_cast<int>(progressIndex_);
    debug_.closestIdx     = static_cast<int>(progressIndex_);
    debug_.commandIdx     = static_cast<int>(progressIndex_);
    // 아래 두 항은 후진 부호 뒤집기 **전** 값입니다. 후진 구간에서는 두 항의
    // 합이 실제 발행된 steerAngle의 부호 반대가 되므로, CSV로 후진 구간을
    // 디버깅할 때 헷갈리지 마세요.
    // These two are pre-flip values: on a reverse segment their sum is the
    // negative of the emitted steerAngle.
    debug_.clearExtras();
    debug_.put("hdeTerm", hdeTerm);
    debug_.put("ltdTerm", ltdTerm);
    // 종방향 PID 분해. PurePursuitTracker 와 같은 키를 씁니다 — 대시보드가
    // 어느 트래커가 돌든 같은 화면을 그릴 수 있어야 합니다.
    // Longitudinal PID decomposition, under the same keys PurePursuitTracker
    // uses, so the dashboard renders identically whichever tracker runs.
    debug_.put("longP", longitudinal.pTerm);
    debug_.put("longI", longitudinal.iTerm);
    debug_.put("longD", longitudinal.dTerm);
    debug_.put("longTargetMps", longitudinal.targetMps);
    debug_.put("longReversing", longitudinal.brakingForReversal ? 1.0 : 0.0);
    debug_.put("longRequestedMps", longitudinal.requestedMps);
    debug_.put("longThrottleGated", longitudinal.throttleGated ? 1.0 : 0.0);
    debug_.put("longBelowSlowestStage",
               longitudinal.belowSlowestStage ? 1.0 : 0.0);
    debug_.put("longDebtM", longitudinal.debtM);
    debug_.put("longCoastDecel", longitudinal.coastDecelMps2);
    debug_.put("longStageKmh", longitudinal.stageSustainedKmh);
    debug_.put("longStageMeasured", longitudinal.stageMeasured ? 1.0 : 0.0);
    debug_.put("longFloorKmh", longitudinal.machineFloorKmh);

    TrackerCommand cmd{steerAngle, longitudinal.throttle, longitudinal.brake,
                       longitudinal.gear, wp.implementFlag};
    lastValidCmd_ = cmd;
    return cmd;
}

} // namespace tracking
} // namespace AppMain
