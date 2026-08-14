/*
 * AppMain/tracking/impl/PurePursuitTracker.cpp
 *
 * 자전거(bicycle) 운동학 모델 기반 Pure Pursuit 구현.
 * Bicycle-model Pure Pursuit implementation.
 *
 * ┌─ 한국어: update() 읽는 순서 ───────────────────────────────────────────────┐
 * update() 본문에는 (a2)~(h) 라벨이 붙어 있습니다. 순서대로 읽으면 됩니다.
 *
 *   (a3) yaw rate 수치 미분           - D 항에 쓸 회전 각속도
 *   (a2) 경로 이탈(stale) 감지        - 30m 이상 벗어나면 전체 재탐색
 *   (b)  차량 최근접 인덱스            - 전방 윈도우 + 히스테리시스
 *   (c2) 작업기 기준 명령 인덱스 전진
 *   (c3) 차량 기준 명령 인덱스 전진    - 기어/속도를 결정
 *   (c)  완주 판정
 *   (d)  전방 목표점(lookahead) 결정
 *   (e0) LTD(횡방향 편차) + 구간 접선 방향
 *   (e1) 방향 오차
 *   (e)  Pure Pursuit 기하 조향
 *   (e1c)곡률 피드포워드
 *   (e2) 다섯 항의 합 = 최종 조향      ★ 여기가 조향의 핵심
 *   (f)  속도/기어/스로틀/브레이크
 *   (g)  작업기 올림/내림
 *   (h)  진단 정보 채우기
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English: reading order for update() ──────────────────────────────────────┐
 * The body of update() is labelled (a2) through (h); read them in order.
 * The steering law itself is the five-term sum at (e2).
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#include <AppMain/tracking/impl/PurePursuitTracker.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace AppMain {
namespace tracking {

namespace {

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 튜닝 게인(lookahead, LTD/HDE 게인, PP 가중치, yaw 감쇠)은 constexpr 에서
// TrackerParams(params_)로 옮겨 런타임에 튜닝 UI로 바꿀 수 있습니다. 기본값은
// TrackerTypes.hpp 의 TrackerParams 에 있으며 기존 상수와 동일합니다.
// 아래에 남은 것은 튜닝 대상이 아닌 물리/기하 상수뿐입니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The tuning gains (lookahead, LTD/HDE gains, PP weight, yaw damping) moved from
// constexpr into TrackerParams (params_) so the tuning UI can change them at
// runtime; defaults live in TrackerTypes.hpp and equal the old constants. Only
// non-tunable physical/geometric constants remain below.
// └────────────────────────────────────────────────────────────────────────────┘

// ── 기하 / Geometry ─────────────────────────────────────────────────────────
constexpr double KMH_TO_MS = 1.0 / 3.6;
// 마지막 웨이포인트에 이만큼 가까워지면 완주. / Within this of the last waypoint = finished.
constexpr double GOAL_TOLERANCE_M = 1.0;
// 작업기는 차량보다 이만큼 뒤에 끌려옵니다. / The implement trails the vehicle by this much.
constexpr double IMPLEMENT_OFFSET_M = 3.0;

// ── 인덱스 탐색 윈도우 / Index search windows ───────────────────────────────
// 최근접 탐색을 전방 몇 개까지만 볼지. 좁게 두는 이유는 K턴에서 경로가 교차할 때
// 반대편 구간으로 인덱스가 튀는 것을 막기 위해서입니다.
// How far forward to search. Kept narrow so a self-intersecting K-turn cannot
// make the index jump onto the opposite segment.
// 방향값이 이 시간 동안 그대로면 yaw rate를 0으로 봅니다. GPS 주기(1초)보다는
// 넉넉하고, 감쇠항을 오래 붙들고 있지는 않을 만큼 짧아야 합니다.
// Heading unchanged for this long means zero yaw rate: comfortably longer than
// the 1 s GPS period, short enough not to hold a stale damping term.
constexpr double YAW_STALE_S = 1.5;

constexpr int VEH_SEARCH_FORWARD = 10;
// 작업기 최근접 탐색의 후방 여유. 3m 오프셋 + K턴 원호의 곡률 여유를 포함합니다.
// Lookback for the implement's closest index: covers the 3m offset plus slack
// for the curvature of a K-turn arc.
constexpr size_t IMPL_LOOKBACK = 8;

double estimatePathCurvature(const std::vector<TrackerWaypoint>& waypoints,
                             std::size_t closestIndex)
{
    constexpr double MIN_SEG_LEN_M = 0.05;
    constexpr int OFFSETS[3] = {-1, 0, 1};
    constexpr double WEIGHTS[3] = {0.5, 1.0, 0.5};
    double weightedCurvature = 0.0;
    double weightSum = 0.0;
    for (int k = 0; k < 3; ++k) {
        const long long a = static_cast<long long>(closestIndex) + OFFSETS[k] - 1;
        const long long b = static_cast<long long>(closestIndex) + OFFSETS[k];
        const long long c = static_cast<long long>(closestIndex) + OFFSETS[k] + 1;
        if (a < 0 || c >= static_cast<long long>(waypoints.size())) continue;
        const auto& wpA = waypoints[static_cast<std::size_t>(a)];
        const auto& wpB = waypoints[static_cast<std::size_t>(b)];
        const auto& wpC = waypoints[static_cast<std::size_t>(c)];
        if (wpA.speed * wpB.speed < 0.0 || wpB.speed * wpC.speed < 0.0) continue;
        const double ux = wpB.x - wpA.x;
        const double uz = wpB.z - wpA.z;
        const double vx = wpC.x - wpB.x;
        const double vz = wpC.z - wpB.z;
        const double uLen = std::hypot(ux, uz);
        const double vLen = std::hypot(vx, vz);
        if (uLen < MIN_SEG_LEN_M || vLen < MIN_SEG_LEN_M) continue;
        double change = std::atan2(vz, vx) - std::atan2(uz, ux);
        while (change > M_PI) change -= 2.0 * M_PI;
        while (change < -M_PI) change += 2.0 * M_PI;
        weightedCurvature += WEIGHTS[k] * change / (0.5 * (uLen + vLen));
        weightSum += WEIGHTS[k];
    }
    return weightSum > 1e-6 ? weightedCurvature / weightSum : 0.0;
}

} // namespace

PurePursuitTracker::PurePursuitTracker()
{
    resetState();
}

void PurePursuitTracker::onTrackSet()
{
    resetState();
}

void PurePursuitTracker::resetState()
{
    trackComplete_ = false;
    pathStarted_ = false;
    lastClosestIndex_ = 0;
    commandIndex_ = 0;
    vehicleCommandIndex_ = 0;
    lastImplementClosestIndex_ = 0;
    maxReachedCommandIndex_ = 0;
    speed_.reset();
    prevHeading_ = 0.0;
    prevHeadingValid_ = false;
    lastPoseSampleId_ = -1;
    headingAgeS_ = 0.0;
    lastYawRate_ = 0.0;
    prevTargetIndex_ = 0;
    prevTargetIndexValid_ = false;
    lastValidCmd_ = TrackerCommand{0.0, 0.0, 0.0, 1, 0};
}

TrackerCommand PurePursuitTracker::update(const VehicleState& state, double dtS)
{
    // 아래 알고리즘 본문에서 쓰는 별칭입니다. 경로 자체는 PathTrackerBase에서
    // 상속받은 waypoints_ 입니다.
    // Aliases for the algorithm body below. The path itself is waypoints_,
    // inherited from PathTrackerBase.
    const double currentX = state.x;
    const double currentZ = state.z;
    const double currentHeading = state.headingRad;
    const double currentSpeed = state.speedMps;

    TrackerCommand idle{0.0, 0.0, 0.0, 1, 0};

    if (waypoints_.empty()) {
        return idle;
    }
    if (trackComplete_) {
        return lastValidCmd_;
    }

    const size_t n = waypoints_.size();

    // 주행 시작 가드. 차량이 waypoints_[0] 의 3m 이내에 들어오기 전까지
    // pathStarted_ 는 false로 유지됩니다. 이전 주행의 종료 지점 근처에서 새
    // 세션을 시작할 때 즉시 완주로 오판하는 것을 막습니다.
    //
    // Check if vehicle has started near the first waypoint (path-start guard).
    // pathStarted_ stays false until vehicle comes within 3m of waypoints_[0].
    // This prevents trackComplete_ from firing on a new session when the vehicle
    // happens to be already near the last waypoint from a previous run.
    if (!pathStarted_) {
        const double dx0 = currentX - waypoints_[0].x;
        const double dz0 = currentZ - waypoints_[0].z;
        constexpr double PATH_START_TOLERANCE_M = 3.0;
        if (std::sqrt(dx0 * dx0 + dz0 * dz0) < PATH_START_TOLERANCE_M) {
            pathStarted_ = true;
        }
    }

    // 작업기의 위치를 추정합니다. 차량 진행 방향의 반대쪽으로 3m 뒤입니다.
    // Implement position: offset behind vehicle along heading direction.
    const double implCos = std::cos(currentHeading);
    const double implSin = std::sin(currentHeading);
    const double implX = currentX - implCos * IMPLEMENT_OFFSET_M;
    const double implZ = currentZ - implSin * IMPLEMENT_OFFSET_M;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (a3) yaw rate: 방향값을 수치 미분해 D 항 감쇠에 씁니다.
    //
    // ★ 나누는 시간은 제어 주기(dtS)가 아니라 "방향값이 마지막으로 바뀐 뒤 흐른
    // 시간"입니다. 수정 전 mux에서는 10Hz 루프에 GPS가 약 1Hz였기 때문에 dtS로
    // 나누면 새 측정이 들어온 틱에서만 1초치 변화량을 0.1초로 나누게 됩니다 —
    // 10배 과대평가입니다. 나머지 9틱은 변화량이 0이라 yaw rate도 0입니다.
    //
    // 실제로 이게 얼마나 큰 값인지: 선회 중 실제 각속도 11.4°/s(=0.199 rad/s)면
    // 계산값은 1.99 rad/s가 되고, ppKYawDamp를 곱한 감쇠항은 예전 기본값 1.5에서
    // 2.99 rad = 171°였습니다. 최종 클램프가 잘라 내니, 초당 한 번씩 조향
    // 지령이 풀락으로 튀고 나머지 0.9초는 감쇠가 아예 없는 상태가 됩니다.
    // 감쇠항이 오히려 최대 진동원이었습니다.
    //
    // SpeedController가 GPS 속도를 다루는 방식과 같은 문제, 같은 해법입니다.
    // 측정이 갱신된 틱에서만 미분하고, 그 사이에는 직전 값을 유지합니다.
    //
    // 첫 호출(또는 리셋 직후)에는 0으로 둡니다. 안 그러면 없는 각속도가 갑자기
    // 튀어 보입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (a3) Yaw rate: numerical derivative of heading for D-term damping.
    //
    // The divisor is the time since the heading last CHANGED, not the control
    // period dtS. Before the mux fix, a 10 Hz loop received heading at roughly
    // 1 Hz, so
    // dividing by dtS spreads a whole second of heading change over 0.1 s on the
    // tick a new fix lands — a 10x overestimate — while the other nine ticks see
    // no change and report zero.
    //
    // The magnitude matters: a real 11.4 deg/s turn (0.199 rad/s) came out as
    // 1.99 rad/s, and at the former ppKYawDamp of 1.5 the damping term was
    // 2.99 rad = 171 deg. The final clamp cut that back, so once per second
    // the steering demand slammed to full lock and for the other 0.9 s there was
    // no damping at all. The damper was the largest source of oscillation.
    //
    // Same problem and same fix as SpeedController's handling of GPS speed:
    // differentiate only on a fresh measurement and hold the value in between.
    //
    // On first call (or post-reset) yaw rate is 0 to avoid a spurious spike.
    // └────────────────────────────────────────────────────────────────────────┘
    if (dtS > 0.0) {
        headingAgeS_ += dtS;
    }
    double yawRate = lastYawRate_;
    if (!prevHeadingValid_) {
        yawRate           = 0.0;
        lastYawRate_      = 0.0;
        prevHeading_      = currentHeading;
        prevHeadingValid_ = true;
        lastPoseSampleId_ = state.poseSampleId;
        headingAgeS_      = 0.0;
    } else if ((state.poseSampleId >= 0 && state.poseSampleId != lastPoseSampleId_) ||
               (state.poseSampleId < 0 && currentHeading != prevHeading_)) {
        double dHdg = currentHeading - prevHeading_;
        // ±pi 경계를 넘는 경우를 처리하려면 반드시 정규화해야 합니다.
        // Wrap to [-pi, pi] to handle ±pi crossings.
        while (dHdg > M_PI)  dHdg -= 2.0 * M_PI;
        while (dHdg < -M_PI) dHdg += 2.0 * M_PI;
        yawRate      = dHdg / std::max(1e-3, headingAgeS_);
        lastYawRate_ = yawRate;
        prevHeading_ = currentHeading;
        lastPoseSampleId_ = state.poseSampleId;
        headingAgeS_ = 0.0;
    } else if (headingAgeS_ >= YAW_STALE_S) {
        // 방향값이 이만큼 그대로면 정말로 안 돌고 있는 것입니다. 마지막 각속도를
        // 무한정 붙들고 있으면 정지 중에도 감쇠항이 남습니다.
        // Heading unchanged this long means the vehicle really is not turning;
        // holding the last rate forever would keep damping a standstill.
        yawRate      = 0.0;
        lastYawRate_ = 0.0;
        headingAgeS_ = 0.0;
    }

    // (a2) 경로 이탈 감지. 차량이 waypoints_[vehicleCommandIndex_] 에서 너무 멀리
    // 떨어졌다면 좁은 전방 윈도우가 더 이상 의미가 없습니다. 이때는 전체 탐색을
    // 한 번 하고 인덱스를 모두 재설정합니다. 기준은 작업기가 아니라 차량입니다.
    // vehicleCommandIndex_ 가 경로 진행도의 기준값이고 commandIndex_ 는 작업기를
    // 따라가며 뒤처지기 때문입니다.
    //
    // (a2) Stale detection: if vehicle is too far from waypoints_[vehicleCommandIndex_],
    // the local window may be invalid. Do a full search and reset both indices.
    // vehicleCommandIndex_ is the vehicle's path-progress ground truth; commandIndex_
    // tracks implement (lags behind). Anchor stale on vehicle, not implement.
    if (n > 0 && vehicleCommandIndex_ < n) {
        const double dxStale = currentX - waypoints_[vehicleCommandIndex_].x;
        const double dzStale = currentZ - waypoints_[vehicleCommandIndex_].z;
        const double staleDistSq = dxStale * dxStale + dzStale * dzStale;
        constexpr double STALE_THRESHOLD_M = 30.0;
        constexpr double STALE_THRESHOLD_SQ = STALE_THRESHOLD_M * STALE_THRESHOLD_M;
        if (staleDistSq > STALE_THRESHOLD_SQ) {
            // Full search over all waypoints.
            double bestDistSq = std::numeric_limits<double>::infinity();
            size_t bestIdx = 0;
            for (size_t i = 0; i < n; ++i) {
                const double dx = currentX - waypoints_[i].x;
                const double dz = currentZ - waypoints_[i].z;
                const double d2 = dx * dx + dz * dz;
                if (d2 < bestDistSq) {
                    bestDistSq = d2;
                    bestIdx = i;
                }
            }
            vehicleCommandIndex_ = bestIdx;
            commandIndex_ = bestIdx;
            lastClosestIndex_ = bestIdx;
            lastImplementClosestIndex_ = bestIdx;
        }
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (b) 차량 기준 최근접 인덱스. 기준점은 vehicleCommandIndex_(차량 진행도)입니다.
    //
    // K턴 중에는 차량이 작업기보다 앞서 갑니다. lookahead, LTD(횡방향 편차), 방향
    // 오차가 모두 "차량이 실제로 어디 있는지"를 반영해야 하므로 최근접 인덱스는
    // 차량을 따라야 합니다.
    //
    // 세 가지 안전장치가 들어 있습니다.
    //  - 전방만 보는 윈도우: 경로가 교차하는 K턴 고리를 건너뛰지 못하게 합니다.
    //  - 히스테리시스(30cm): GPS 잡음 때문에 인덱스가 깜박거리는 것을 막습니다.
    //  - F/R 경계 절단: 속도 부호가 바뀌는 첫 지점에서 윈도우를 자릅니다. 그래서
    //    최근접 인덱스가 전진/후진 구간을 건너뛸 수 없습니다. 경계를 넘는 유일한
    //    경로는 아래 (c3)의 무조건 전진뿐입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (b) Vehicle-based closestIndex: anchored on vehicleCommandIndex_ (vehicle path-progress).
    // During K-turn the vehicle leads the implement; closestIndex must follow the
    // vehicle so lookahead, lateral-deviation, and heading-error all reflect the vehicle's
    // actual position. Forward-only window prevents jumping across self-intersecting
    // K-turn loops; hysteresis bias prevents GPS-noise flicker.
    // F/R boundary clipping: window terminates at first speed-sign change so closest
    // can't skip across reverse/forward segments (vehicle must traverse each direction
    // segment, F/R unconditional advance is the only path across the boundary).
    // └────────────────────────────────────────────────────────────────────────┘
    size_t lo = vehicleCommandIndex_;
    size_t hi = std::min(n - 1, vehicleCommandIndex_ + static_cast<size_t>(VEH_SEARCH_FORWARD));
    {
        double curSign = waypoints_[vehicleCommandIndex_].speed;
        for (size_t i = lo; i + 1 <= hi; ++i) {
            if (waypoints_[i + 1].speed * curSign < 0.0) {
                // 바로 앞이 F/R 경계 — 윈도우를 여기서 자릅니다.
                hi = i;  // F/R boundary just ahead — clip window
                break;
            }
        }
    }

    // 직전 값이 윈도우 안에 있으면 그것으로, 아니면 윈도우 하한으로 시작합니다.
    // Initialize with lastClosestIndex_ if it's in window, else lo.
    size_t closestIndex = (lastClosestIndex_ >= lo && lastClosestIndex_ <= hi)
                            ? lastClosestIndex_ : lo;
    double bestDistSq = std::numeric_limits<double>::infinity();
    for (size_t i = lo; i <= hi; ++i) {
        double dx = waypoints_[i].x - currentX;
        double dz = waypoints_[i].z - currentZ;
        double dist = std::sqrt(dx * dx + dz * dz);
        double effectiveDist = dist;
        if (i != lastClosestIndex_) {
            // 히스테리시스 30cm: 현재 인덱스를 유지하는 쪽에 유리하게 만들어
            // GPS 잡음으로 인덱스가 왕복하는 것을 막습니다.
            effectiveDist += 0.30;  // hysteresis bias (30cm) prevents GPS-noise flicker
        }
        if (effectiveDist < bestDistSq) {
            bestDistSq = effectiveDist;
            closestIndex = i;
        }
    }
    lastClosestIndex_ = closestIndex;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 작업기 기준 최근접 인덱스. 기준점은 역시 vehicleCommandIndex_(=vci)이고,
    // 3m 오프셋과 K턴 원호의 곡률 여유를 덮는 만큼 뒤쪽으로 윈도우를 엽니다.
    //
    // commandIndex_ 와는 독립적으로 계산하는 것이 중요합니다. commandIndex_ 는
    // F/R 경계에서 "무조건 전진"하기 때문에, 작업기가 아직 K턴 원호에 남아 있는
    // 동안에도 경계를 건너뛸 수 있습니다. 그 값으로 작업기를 제어하면 쟁기가
    // 너무 일찍 내려갑니다.
    //
    // 윈도우 범위:
    //   하한 = max(vci - IMPL_LOOKBACK, lastImplementClosestIndex_)
    //   상한 = vci  (작업기는 항상 차량을 뒤따르며, 앞설 수 없습니다)
    //
    // lastImplementClosestIndex_ 를 하한으로 쓰는 이유는 진행도의 단조성입니다.
    // K턴에서 경로가 교차하면, 새 레인의 웨이포인트와 기하적으로 비슷하게 가까운
    // 후진 구간 웨이포인트로 2D 최근접이 되돌아가 버릴 수 있습니다. 하지만 작업기가
    // 물리적으로 경로를 "되짚어" 갈 수는 없으므로, 이미 도달한 지점보다 뒤의 값은
    // 절대 받아들이지 않습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Implement-based closest index: anchored on vehicleCommandIndex_ (vehicle path-
    // progress) with a lookback window covering the 3m implement offset plus K-turn
    // arc curvature slack. INDEPENDENT of commandIndex_ so the F/R unconditional
    // advance of commandIndex_ does not desync implement output from physical
    // implement position. Lower bound: vci - IMPL_LOOKBACK (clamped to 0). Upper
    // bound: vci (implement always trails vehicle, never ahead).
    // Lower bound: max(vci - IMPL_LOOKBACK, lastImplementClosestIndex_).
    // The lastImplementClosestIndex_ floor enforces path-progress monotonicity.
    // At K-turn self-intersections, the 2D-closest can ambiguously snap back to
    // a reverse-segment wp that is geometrically close to the new lane wp.
    // Path-progress is monotonic (implement physically cannot un-traverse a path),
    // so we never accept an impl_ci behind what we've already reached.
    // └────────────────────────────────────────────────────────────────────────┘
    size_t implLoRaw = (vehicleCommandIndex_ >= IMPL_LOOKBACK)
                         ? vehicleCommandIndex_ - IMPL_LOOKBACK : 0;
    size_t implLo = std::max(implLoRaw, lastImplementClosestIndex_);
    size_t implHi = vehicleCommandIndex_;
    if (implHi >= n) implHi = n - 1;
    // 예외 상황: 하한이 vci를 넘어서면(정상 주행에서는 없지만 GPS가 크게 튀는
    // 경우에 대비) 하한을 상한에 맞춥니다.
    // Edge case: if floor exceeds vci (shouldn't happen during normal operation but
    // guard against any GPS-jump scenario), clamp implLo to implHi.
    if (implLo > implHi) implLo = implHi;

    size_t implementClosestIndex = implLo;
    {
        double bestImplDistSq = std::numeric_limits<double>::infinity();
        for (size_t i = implLo; i <= implHi; ++i) {
            double dx = waypoints_[i].x - implX;
            double dz = waypoints_[i].z - implZ;
            double d2 = dx * dx + dz * dz;
            if (d2 < bestImplDistSq) {
                bestImplDistSq = d2;
                implementClosestIndex = i;
            }
        }
    }

    // 다음 주기의 단조 하한으로 저장합니다.
    // Store back as monotonic floor for next iteration.
    lastImplementClosestIndex_ = implementClosestIndex;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (c2) 명령 웨이포인트 전진. wp[commandIndex_+1] 지점에 세운 "수직 평면"을
    // 통과했으면 인덱스를 하나 전진시킵니다.
    //
    // 평면은 구간 방향(wp[i-1] -> wp[i])에 수직입니다. 그 방향으로의 투영값이
    // 0보다 크다는 것은 경로 진행 관점에서 wp[i]를 지났다는 뜻입니다. 단순한
    // 거리 비교보다 견고합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (c2) Command waypoint advancement: advance commandIndex_ while vehicle has
    // crossed the perpendicular plane at wp[commandIndex_+1].
    // Plane is perpendicular to segment direction (wp[i-1] -> wp[i]).
    // Forward projection > 0 means vehicle has passed wp[i] in path-progress sense.
    // └────────────────────────────────────────────────────────────────────────┘

    // 하한 보정: commandIndex_ 가 작업기 위치까지 따라왔는지 보장합니다. K턴에서
    // 코너를 잘라 지나갈 때 평면 통과만으로는 놓치는 경우를 복구합니다. -1 여유는
    // GPS 잡음으로 경계에서 떠는 것을 막습니다.
    //
    // Floor: ensure commandIndex_ has caught up to implement position.
    // Recovers from K-turn cutting where plane-crossing alone fails.
    // Margin -1 prevents GPS-noise boundary jitter.
    if (implementClosestIndex > 0 && implementClosestIndex - 1 > commandIndex_) {
        commandIndex_ = implementClosestIndex - 1;
    }

    while (commandIndex_ + 1 < n) {
        const size_t i = commandIndex_ + 1;
        // F/R 경계: wp[i]의 속도 부호가 wp[i-1]과 반대라면 무조건 전진시킵니다.
        // 여기서는 "구간 방향으로 wp[i]를 지났다"는 평면 통과 판정이 의미가 없습니다.
        // 구간 방향 자체가 뒤집히기 때문입니다. wp[i-1]에 도달한 시점(즉 이미
        // commandIndex_=i-1 인 상태)이 wp[i]의 새 방향으로 기어/속도를 바꿀
        // 올바른 순간입니다.
        //
        // F/R boundary: if wp[i] has opposite speed sign from wp[i-1], advance
        // unconditionally. Plane-crossing's "past wp[i] in segment direction"
        // semantics doesn't apply here because segment direction reverses; reaching
        // wp[i-1] (which is what commandIndex_=i-1 already represents) is the
        // correct moment to flip gear/speed to the new direction at wp[i].
        if (waypoints_[i].speed * waypoints_[i - 1].speed < 0.0) {
            commandIndex_ = i;
            break;
        }
        const double fdx = waypoints_[i].x - waypoints_[i - 1].x;
        const double fdz = waypoints_[i].z - waypoints_[i - 1].z;
        const double flen = std::sqrt(fdx * fdx + fdz * fdz);
        if (flen < 1e-9) {
            // 길이가 0에 가까운 구간(중복 웨이포인트) — 무조건 전진.
            // Degenerate segment, advance unconditionally
            commandIndex_ = i;
            continue;
        }
        const double inv = 1.0 / flen;
        const double ux = fdx * inv;
        const double uz = fdz * inv;
        const double vx = implX - waypoints_[i].x;
        const double vz = implZ - waypoints_[i].z;
        const double proj = vx * ux + vz * uz;
        if (proj > 0.0) {
            commandIndex_ = i;
        } else {
            break;
        }
    }

    // 차량 기준 명령 인덱스: 기어와 속도를 결정합니다.
    // 하한 보정: 차량의 최근접 인덱스가 이미 앞서 있으면 그만큼 끌어올립니다.
    //
    // Vehicle-based command index: drives gear/speed.
    // Floor: if vehicle's closestIndex has advanced past vehicleCommandIndex_, jump.
    if (closestIndex > 0 && closestIndex - 1 > vehicleCommandIndex_) {
        vehicleCommandIndex_ = closestIndex - 1;
    }

    // (c3) 차량 기준 명령 웨이포인트 전진. 위 commandIndex_ 루프와 논리는 같지만
    // 작업기가 아니라 차량 위치를 씁니다. K턴의 전진/후진 전환이 "차량이" 경계를
    // 넘을 때 일어나야 하기 때문입니다.
    //
    // (c3) Vehicle command waypoint advancement: same logic as commandIndex_ but
    // uses vehicle position (not implement). Drives gear/speed transitions for
    // K-turn F/R behavior to fire when vehicle (not implement) crosses boundaries.
    while (vehicleCommandIndex_ + 1 < n) {
        const size_t i = vehicleCommandIndex_ + 1;
        // F/R 경계에서는 무조건 전진 (위 commandIndex_ 루프의 설명과 동일).
        // 차량이 같은 방향의 마지막 웨이포인트 wp[i-1]에 물리적으로 도달한
        // 시점이므로, 지금이 기어를 전진<->후진으로 바꿀 순간입니다.
        //
        // F/R boundary: advance unconditionally (see commandIndex_ loop above).
        // This is the moment to flip gear from forward to reverse (or vice versa)
        // because vehicle has physically reached wp[i-1] (last same-direction wp).
        if (waypoints_[i].speed * waypoints_[i - 1].speed < 0.0) {
            vehicleCommandIndex_ = i;
            break;
        }
        const double fdx = waypoints_[i].x - waypoints_[i - 1].x;
        const double fdz = waypoints_[i].z - waypoints_[i - 1].z;
        const double flen = std::sqrt(fdx * fdx + fdz * fdz);
        if (flen < 1e-9) {
            vehicleCommandIndex_ = i;
            continue;
        }
        const double inv = 1.0 / flen;
        const double ux = fdx * inv;
        const double uz = fdz * inv;
        const double vx = currentX - waypoints_[i].x;
        const double vz = currentZ - waypoints_[i].z;
        const double proj = vx * ux + vz * uz;
        if (proj > 0.0) {
            vehicleCommandIndex_ = i;
        } else {
            break;
        }
    }
    // 정상적인 전진으로 도달한 최고 인덱스만 기록합니다(경로 이탈 재탐색으로
    // 튄 값은 제외). 완주 게이트가 vehicleCommandIndex_ 대신 이 값을 보는 이유는,
    // 전체 재탐색이 우연히 경로 끝 근처에 착지했을 때 진행도 조건이 앞당겨
    // 충족되는 것을 막기 위해서입니다.
    //
    // Track the highest index reached via normal forward advance only (not stale-jump).
    // Finish gate checks this instead of vehicleCommandIndex_ to prevent a stale full-search
    // jump landing near the end from satisfying the progress guard prematurely.
    if (vehicleCommandIndex_ > maxReachedCommandIndex_) {
        maxReachedCommandIndex_ = vehicleCommandIndex_;
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (c) 완주 판정. 마지막 웨이포인트까지의 거리가 GOAL_TOLERANCE_M 미만일 때.
    //
    // 두 개의 가드가 함께 걸려 있습니다. 둘 다 필요합니다.
    //  - pathStarted_ : 이전 주행의 종료 지점 근처에서 새 세션을 시작할 때 즉시
    //                   완주로 판정되는 것을 막습니다.
    //  - 진행도 게이트: 마지막 10개 웨이포인트 안에 들어와 있어야 합니다. 레인을
    //                   건너뛰며 짜는 경로처럼 경로가 자기 자신과 교차하면
    //                   "거리가 가깝다"만으로는 속기 때문입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (c) Termination: distance to last waypoint < GOAL_TOLERANCE_M.
    // Guard: only complete if pathStarted_ is true so we don't fire immediately
    // on a new session when the vehicle is still near the previous run's endpoint.
    // Also require path-progress gate: vehicleCommandIndex_ must be within last 10 waypoints
    // to prevent self-crossing paths (skip-lane patterns) from fooling termination.
    // └────────────────────────────────────────────────────────────────────────┘
    {
        const TrackerWaypoint& last = waypoints_.back();
        double dx = last.x - currentX;
        double dz = last.z - currentZ;
        double dist = std::sqrt(dx * dx + dz * dz);
        if (pathStarted_ && dist < GOAL_TOLERANCE_M
            && maxReachedCommandIndex_ >= (waypoints_.size() >= 10 ? waypoints_.size() - 10 : 0)) {
            trackComplete_ = true;
            return lastValidCmd_;
        }
    }

    // (d) 전방 목표점(lookahead). 최근접 지점부터 앞으로 걸어가며 호길이를
    // 누적하고, lookahead 거리에 도달하면 그 웨이포인트를 목표로 삼습니다.
    //
    // (d) Lookahead target: walk forward accumulating arc length until the
    // lookahead distance is reached.
    const double speed = std::abs(currentSpeed);
    // Preserve the long, delay-tolerant straight lookahead. On a sustained
    // curved window, reduce it continuously so the target does not cut across
    // the arc. The floor remains four metres (nearly one minimum-radius arc),
    // and the time term still scales with speed and transport delay.
    const double previewCurvature = std::fabs(
            estimatePathCurvature(waypoints_, closestIndex));
    constexpr double CURVE_BLEND_START = 0.05; // R20
    constexpr double CURVE_BLEND_FULL = 0.20;  // R5, just above physical Rmin
    // The catalog's steering rate and two first-order lags define how quickly
    // the selected vehicle can establish full wheel angle. They only raise the
    // proven curve floor; they never shorten the conservative straight preview.
    const double fullTravelS = geometry_.maxWheelAngleRad * (180.0 / M_PI)
            * geometry_.steeringRatio / geometry_.steeringWheelMaxRateDegS;
    const double steeringDelayS = fullTravelS + geometry_.steeringWheelTauS
            + geometry_.frontWheelTauS;
    const double curveTimeMinS = std::max(4.5, 2.0 * steeringDelayS);
    constexpr double CURVE_DISTANCE_MIN_M = 4.0;
    const double curveBlend = std::clamp(
            (previewCurvature - CURVE_BLEND_START) /
            (CURVE_BLEND_FULL - CURVE_BLEND_START), 0.0, 1.0);
    const double lookaheadTime = params_.ppLookaheadTimeS - curveBlend *
            std::max(0.0, params_.ppLookaheadTimeS - curveTimeMinS);
    const double lookaheadMin = params_.ppLookaheadMinM - curveBlend *
            std::max(0.0, params_.ppLookaheadMinM - CURVE_DISTANCE_MIN_M);
    const double lookahead = std::clamp(lookaheadTime * speed,
                                        lookaheadMin,
                                        params_.ppLookaheadMaxM);
    size_t targetIndex = closestIndex;
    double accumulated = 0.0;
    for (size_t i = closestIndex; i + 1 < n; ++i) {
        // 방향 전환 경계: 기본적으로 여기서 멈춥니다. 그래야 목표점이 현재 진행
        // 방향과 같은 쪽에 남습니다. 예외는 vehicleCommandIndex_ 가 이미 이 경계를
        // 넘어간 경우(= 기어가 이미 뒤집힌 경우)입니다. 그때는 실제 진행 방향이
        // 경계의 반대쪽이므로, 목표점도 후진 구간으로 따라 들어가게 허용해야
        // PP 기하와 후진 부호 뒤집기가 서로 어긋나지 않습니다.
        //
        // Direction-reversal boundary: normally break here so lookahead stays on
        // current motion side of the boundary. EXCEPTION: if vehicleCommandIndex_
        // has already advanced past this boundary (gear has flipped), then motion
        // direction is on the OTHER side — allow lookahead to follow into the
        // reverse segment so PP geometry and reverse-correction flip stay consistent.
        if ((waypoints_[i].speed > 0.0 && waypoints_[i + 1].speed < 0.0) ||
            (waypoints_[i].speed < 0.0 && waypoints_[i + 1].speed > 0.0)) {
            if (vehicleCommandIndex_ <= i) {
                break;
            }
            // 그 외: 이미 경계를 넘어간 상태이므로 계속 진행합니다.
            // Else: motion has already crossed; continue past boundary.
        }
        // 작업기 플래그 경계: 목표점이 작업/이동 경계를 넘어가지 못하게 막습니다.
        // 차량이 아직 직선 작업 구간에 있는데 목표점이 K턴 원호로 넘어가면, 조향이
        // 미리 원호 쪽으로 당겨져 코너를 잘라먹습니다.
        // 나중에 차량이 실제로 경계를 넘는 순간 targetIdx가 여러 칸 튀게 되는데,
        // 그건 아래의 변화율 제한이 완만하게 풀어 줍니다.
        //
        double sx = waypoints_[i + 1].x - waypoints_[i].x;
        double sz = waypoints_[i + 1].z - waypoints_[i].z;
        accumulated += std::sqrt(sx * sx + sz * sz);
        targetIndex = i + 1;
        if (accumulated >= lookahead) {
            break;
        }
    }

    // 목표 인덱스의 전진 속도를 제한합니다. 위의 작업기 플래그 경계 제약이
    // 풀리는 순간(최근접 인덱스가 경계를 넘어 더 이상 잘리지 않게 되는 순간)
    // 목표 인덱스가 여러 칸 한꺼번에 튀고, 그러면 PP 항과 HDE 항이 급격히
    // 변합니다. 프레임당 최대 몇 칸으로 묶어 여러 프레임에 걸쳐 부드럽게
    // 넘어가게 만듭니다.
    //
    // Rate-limit targetIndex advance after a closest-index jump so PP/HDG
    // geometry transitions over several frames rather than one control tick.
    {
        constexpr size_t TARGET_IDX_MAX_ADVANCE = 1;  // wp per frame
        if (prevTargetIndexValid_ && targetIndex > prevTargetIndex_) {
            const size_t maxAllowed = prevTargetIndex_ + TARGET_IDX_MAX_ADVANCE;
            if (targetIndex > maxAllowed) {
                targetIndex = maxAllowed;
            }
        }
        // 목표 인덱스가 뒤로 가는 것은 허용합니다(경로 이탈 재탐색 후에는 드물게
        // 최근접 인덱스가 되돌아갈 수 있습니다). 변화율 제한은 전진에만 적용됩니다.
        //
        // Allow targetIndex to MOVE BACKWARD (closestIndex regressions are
        // rare but possible after stale-detection full search). Rate limit
        // only applies to forward advances.
        prevTargetIndex_      = targetIndex;
        prevTargetIndexValid_ = true;
    }

    const TrackerWaypoint& target = waypoints_[targetIndex];

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (e0) LTD(횡방향 편차)(cte)와 구간 접선 방향. 조향 피드백(Stanley 방식)과 진단
    // 텔레메트리에 모두 쓰이므로 미리 계산합니다.
    //
    // ★ 핵심 아이디어: cte를 "현재 위치"가 아니라 "예측 위치"에서 측정합니다.
    //   예측 위치 = 현재 위치 + 진행 방향 × 속력 × T
    //   즉 차량이 T초 후에 있을 곳을 미리 보고, 경로를 넘어가기 전에 반응합니다.
    //   현재 위치만 보는 반응형 피드백은 필연적으로 늦게 반응해 S자로 흔들립니다.
    //
    // T를 곱하므로 예측 거리가 속도에 비례합니다. 레인 주행(0.83 m/s)에서는 약
    // 0.83m, 선회(0.28 m/s)에서는 약 0.28m 앞을 봅니다. 오버슈트 위험이 작은
    // 저속에서 자연스럽게 예측 거리도 짧아집니다.
    //
    // 예측 시간은 작업기 플래그가 아니라 경로 곡률로 정합니다. 직선에서는 전체
    // 예측을 쓰고, 반지름 10m 이하 원호에서는 현재 위치 기준 cte를 씁니다.
    //
    // 부호 규약: 구간 진행 방향의 오른쪽(+z쪽)이 양수 cte입니다. 조향 양수 = 좌회전
    // 규약과 맞물려 있습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (e0) Lateral-deviation error + segment heading: computed early so they can be
    // used for steering feedback (Stanley-style) and debug telemetry.
    //
    // CTE is measured at the PREDICTED vehicle position (current + heading × |speed| × T).
    // This makes the controller anticipatory: vehicle sees where it WILL BE in T seconds
    // and reacts before crossing the path. Prevents S-curve overshoot caused by reactive
    // (current-position) feedback.
    //
    // T scales offset by speed: at lane (0.83 m/s) offset ≈ 0.83m, at turn (0.28 m/s)
    // offset ≈ 0.28m. Naturally smaller offset at low speeds where overshoot risk is less.
    //
    // Sign convention: segment-right is positive cte (consistent with steer + = left).
    // Prediction blends by stable local path curvature: full on a straight and
    // zero at a 10 m radius. Implement up/down is deliberately irrelevant.
    // └────────────────────────────────────────────────────────────────────────┘
    constexpr double CTE_PREDICT_T_STRAIGHT = 1.5;
    constexpr double CTE_PREDICT_ZERO_CURVATURE = 0.10; // 1/m: 10 m radius
    const double localPathCurvature = estimatePathCurvature(waypoints_, closestIndex);
    double lateralDeviation = 0.0;
    double segHeading = 0.0;
    {
        const double v_abs = std::abs(currentSpeed);
        // Blend anticipation from full on a straight to zero at a 10 m radius.
        // This is path geometry, never the participant's implement up/down flag.
        const double straightBlend = std::clamp(
                1.0 - std::abs(localPathCurvature) / CTE_PREDICT_ZERO_CURVATURE,
                0.0, 1.0);
        const double pred_T = CTE_PREDICT_T_STRAIGHT * straightBlend;
        const double predX = currentX + std::cos(currentHeading) * v_abs * pred_T;
        const double predZ = currentZ + std::sin(currentHeading) * v_abs * pred_T;
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 보통은 현재 웨이포인트에서 "나가는" 구간을 접선으로 씁니다. 그런데
        // 마지막 웨이포인트에는 나가는 구간이 없습니다. segA == segB 로 두면
        // 구간 길이가 0이 되어 아래 if 문이 건너뛰어지고, lateralDeviation 과
        // segHeading 이 초기값(0.0)으로 남습니다. 그 결과:
        //
        //   (1) cte 항이 사라져 최종 접근 구간이 무보정이 됩니다. closestIndex
        //       가 마지막에 도달한 뒤 차량이 목표에 닿을 때까지가 해당됩니다.
        //   (2) 진단값이 거짓이 됩니다. lateralDeviationM 은 0으로, headingError 은
        //       segHeading(0.0) - currentHeading = -currentHeading 이라는
        //       경로와 무관한 값으로 나갑니다.
        //   (3) lateralDeviationM 은 RunSummary 의 deviationM(CTE의 RMS)으로 누적되고
        //       penaltyS = deviationM × 1000 으로 최종 시간에 반영되므로,
        //       패널티가 과소 계상됩니다.
        //
        // 그래서 마지막에서는 "들어오는" 구간으로 대체합니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Normally the tangent comes from the segment leaving the current
        // waypoint, but the last waypoint has no such segment. Letting
        // segA == segB makes segLen 0, so the block below is skipped and
        // lateralDeviation / segHeading keep their 0.0 defaults. That means:
        //   (1) the cte term vanishes, leaving the final approach uncorrected;
        //   (2) diagnostics lie — lateralDeviationM reads 0 and headingError becomes
        //       -currentHeading, which is unrelated to the path;
        //   (3) lateralDeviationM accumulates into RunSummary::deviationM (RMS of CTE)
        //       and reaches the score as penaltyS = deviationM * 1000, so the
        //       penalty is under-counted.
        // Fall back to the segment arriving at the last waypoint instead.
        // └────────────────────────────────────────────────────────────────────┘
        size_t segA = closestIndex;
        size_t segB = closestIndex + 1;
        if (segB >= n) {
            segB = closestIndex;
            if (closestIndex > 0) {
                segA = closestIndex - 1;
            }
        }
        double sdx = waypoints_[segB].x - waypoints_[segA].x;
        double sdz = waypoints_[segB].z - waypoints_[segA].z;
        double segLen = std::sqrt(sdx * sdx + sdz * sdz);
        if (segLen > 1e-9) {
            double ndx = sdx / segLen;
            double ndz = sdz / segLen;
            double vpx = predX - waypoints_[segA].x;
            double vpz = predZ - waypoints_[segA].z;
            lateralDeviation = ndx * vpz - ndz * vpx;
            segHeading = std::atan2(sdz, sdx);
        }
    }

    // (e1a) 목표점 구간의 접선 방향. 목표점으로 "들어오는" 구간
    // (targetIndex-1 -> targetIndex)을 씁니다. 목표점에서 "나가는" 구간을 쓰면
    // 안 됩니다. 목표점이 후진 구간 직전의 마지막 전진 웨이포인트일 때 나가는
    // 구간은 후진 방향을 가리키고, 그러면 HDE 항이 차량을 후진 방향으로
    // 틀어 버립니다.
    //
    // (e1a) Future segment heading: tangent direction of the segment APPROACHING
    // the lookahead target (targetIndex-1 -> targetIndex). NOT the segment leaving
    // target, because that would pick up F/R boundary direction (reverse) when
    // target is the last-forward wp before a reverse segment, causing HDG to
    // wrongly steer toward reverse direction.
    double futureSegHeading = segHeading;  // fallback to current segment heading
    if (targetIndex >= 1) {
        size_t fsA = targetIndex - 1;
        size_t fsB = targetIndex;
        double fsdx = waypoints_[fsB].x - waypoints_[fsA].x;
        double fsdz = waypoints_[fsB].z - waypoints_[fsA].z;
        double fslen = std::sqrt(fsdx * fsdx + fsdz * fsdz);
        if (fslen > 1e-9) {
            futureSegHeading = std::atan2(fsdz, fsdx);
        }
    }

    // (e1) 진단용 방향 오차(최근접 구간 - 차량). 텔레메트리 표시용으로만 씁니다.
    // (e1) Heading error for debug (closest segment - vehicle): kept for telemetry.
    double headingError = segHeading - currentHeading;
    while (headingError >  M_PI) headingError -= 2.0 * M_PI;
    while (headingError < -M_PI) headingError += 2.0 * M_PI;

    // (e1b) 조향 피드백에 실제로 쓰는 방향 오차는 "앞으로 갈 구간" 기준입니다.
    // 후진 주행에서는 차량이 경로 진행 방향의 반대를 향하므로 목표 방향이
    // segHeading + π 가 됩니다.
    //
    // (e1b) Effective heading error for steering feedback uses the FUTURE segment.
    // For reverse driving, desired heading is segHeading + π (vehicle faces opposite
    // of path-progress direction).
    double effHdeErr = futureSegHeading - currentHeading;
    while (effHdeErr >  M_PI) effHdeErr -= 2.0 * M_PI;
    while (effHdeErr < -M_PI) effHdeErr += 2.0 * M_PI;
    if (target.speed < 0.0) {
        effHdeErr += M_PI;
        while (effHdeErr >  M_PI) effHdeErr -= 2.0 * M_PI;
        while (effHdeErr < -M_PI) effHdeErr += 2.0 * M_PI;
    }

    // (e) Pure Pursuit 기하 조향. 목표점을 지나는 원호를 자전거 모델의 곡률로
    // 환산합니다. x_local / z_local 은 목표점을 차량 좌표계로 옮긴 값이며,
    // z_local 이 양수면 목표점이 차량의 왼쪽에 있습니다(TrackerTypes.hpp 참고).
    //
    // (e) Pure-pursuit geometric steer (bicycle model curvature toward target).
    // x_local / z_local are the target in the vehicle frame; positive z_local
    // means the target is to the vehicle's left.
    double dx = target.x - currentX;
    double dz = target.z - currentZ;
    double h = currentHeading;
    double cosh = std::cos(h);
    double sinh = std::sin(h);
    double x_local =  dx * cosh + dz * sinh;
    double z_local =  dx * sinh - dz * cosh;
    double L = std::sqrt(dx * dx + dz * dz);

    double pp_steer = 0.0;
    if (L > 1e-6) {
        double curvature = 2.0 * z_local / (L * L);
        pp_steer = std::atan(curvature * geometry_.wheelbaseM);
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (e1c) 곡률 피드포워드. 차량의 오차와 무관하게, "경로의 곡률을 그대로 따라
    // 가려면 몇 도를 꺾어야 하는가"를 기하학적으로 계산합니다. 오차가 0인
    // (ltd=0, 방향 오차=0) 이상적인 상태에서 원호를 도는 데 필요한 조향값입니다.
    //
    // 이 항이 없으면 K턴 원호처럼 급한 곡선에서 오차 기반 항들만으로는 조향
    // 권한이 부족해 계속 밖으로 밀려 나갑니다. 오차가 생긴 다음에야 반응하기
    // 때문입니다. 피드포워드는 오차가 생기기 전에 미리 꺾어 줍니다.
    //
    //   κ = dθ/ds     최근접 지점 주변 연속 3개 웨이포인트로 추정
    //   δ_ff = -atan(WHEELBASE * κ)
    //   부호는 PP 규약과 일치합니다(목표가 왼쪽 -> δ > 0).
    //
    // 윈도우 안에 F/R 경계가 있으면 건너뜁니다. 방향이 뒤집히는 지점에서 곡률을
    // 계산하면 의미 없는 값이 나오기 때문입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (e1c) Curvature feedforward: computes geometric steer needed to follow
    // local path curvature, INDEPENDENT of vehicle errors. This is the steer
    // that would track a clean arc with ltd=0 and hde=0. Critical for
    // following tight curves (like K-turn arcs) where error-based terms alone
    // give insufficient steer authority.
    //   κ = dθ/ds estimated from 3 consecutive wps around closestIndex
    //   δ_ff = -atan(WHEELBASE * κ)   [sign matches PP convention: target-left → δ>0]
    // F/R boundary in window is detected and skipped (would give nonsense κ).
    // └────────────────────────────────────────────────────────────────────────┘
    // The same stable five-waypoint geometry estimate also drives feedforward;
    // implement up/down state never changes lateral control.
    const double cf_steer = -std::atan(geometry_.wheelbaseM * localPathCurvature);

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // ★ (e2) 조향의 핵심. 다섯 항의 합입니다. 이 시점의 모든 값은 "전진 기준"이며,
    // 후진 보정은 바로 아래에서 한 번에 처리합니다.
    //
    //   cfTerm      곡률 피드포워드 — 경로 곡률을 따라가는 기하 조향(속도 무관)
    //   ppTerm      Pure Pursuit 잔차 항 — 목표점을 향한 원호와 곡률 앞먹임의 차이를
    //                                기본 가중치 ppWeightPp = 0.7로 혼합합니다
    //   hdeTerm     HDE 항   — 차량 방향을 경로 접선에 맞춤 (보조)
    //   ltdTerm     LTD(횡방향 편차) 항 — 경로에서 벗어난 거리를 되돌림 (보조)
    //   yawDampTerm D 항          — 회전 각속도를 상쇄해 오버슈트를 억제
    //
    // 역할 분담을 이렇게 보면 이해가 쉽습니다.
    //   피드포워드(cf)  = 경로가 요구하는 조향 (오차와 무관)
    //   추종(pp)        = 앞을 보고 목표점으로 향하는 주 조향
    //   보정(hde,ltd)   = pp 만으로 남는 정상상태 오프셋을 마저 지우는 조향
    //   감쇠(yawDamp)   = 위 항들이 과하게 반응했을 때 진동을 잡는 조향
    //
    // ★ 세 개의 피드백 항(pp, hde, ltd)은 모두 "같은 오차"를 봅니다. 게인을 올릴
    // 때는 셋의 합이 물리 한계(WHEEL_MAX_RAD)를 상시로 넘지 않는지부터 확인하세요. 합이
    // 한계에 붙으면 조향은 릴레이가 되고 그 리밋 사이클이 S자 사행입니다.
    // 기본값의 근거와 회귀 검증은 TrackerTypes.hpp 의 TrackerParams 주석과
    // tests/ClosedLoopTrackingTest.cpp 를 보세요.
    //
    // 튜닝할 때는 /tmp/pp_trace.csv 에 각 항이 따로 기록되니 어느 항이 폭주하는지
    // 바로 볼 수 있습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (e2) Hybrid Stanley-PP-FF steering (pre-flip, all in forward semantics):
    //   - Curvature feedforward: geometric δ for current path curvature (speed-independent)
    //   - PP residual term: blends the difference between lookahead pursuit and
    //     curvature feedforward with the actual default ppWeightPp = 0.7
    //   - Heading-error term: trims alignment with the path tangent (secondary)
    //   - CTE term: atan-based, trims residual lateral offset (secondary)
    //   - Yaw-rate damper: counters rotation rate to suppress overshoot
    //
    // All three feedback terms (pp, hde, ltd) watch the SAME error. Before raising
    // any of their gains, check that their sum does not sit at the WHEEL_MAX_RAD
    // limit: a saturated sum turns steering into a relay, and that limit cycle is
    // the S-weave. See the TrackerParams comment in TrackerTypes.hpp for how the
    // defaults were chosen, and tests/ClosedLoopTrackingTest.cpp for the guard.
    //
    // Each term is logged separately in /tmp/pp_trace.csv, so when tuning you can
    // see which one is running away.
    // └────────────────────────────────────────────────────────────────────────┘
    // 앞바퀴 물리 한계. 근거와 실측 방법은 PurePursuitTracker.hpp 참고.
    // Physical front-wheel limit; see PurePursuitTracker.hpp for the measurement.
    // 오차 항 하나가 조향 권한을 독점하지 못하도록 물리 한계의 절반으로 제한.
    // Cap each error term at half the physical limit so no single one takes over.
    const double ERR_TERM_CAP = geometry_.maxWheelAngleRad * 0.5;

    const double cfTerm      = cf_steer;
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // atan은 90°에서 포화하는데 바퀴는 26°에서 멈춥니다. 그래서 "atan으로 감싸면
    // 한 항이 조향을 독점하지 못한다"던 예전 보호 장치는 물리 한계보다 3.5배
    // 위에서야 걸려 아무 역할도 하지 못했습니다. 에뮬레이터 4km/h 주행 로그에서
    // ltdTerm 혼자 평균 52.7°였고 전체 틱의 80.9%에서 한계를 넘었습니다. 이제 각
    // 오차 항을 ERR_TERM_CAP으로 명시적으로 자릅니다.
    //
    // ltdTerm에는 속도 항도 없었습니다. 고전 Stanley는 atan(k·e/v)라 속도가
    // 오르면 지령이 줄어드는데, atan(k·e)는 같은 편차에 같은 각도를 요구합니다 —
    // 반응할 시간은 절반인데 지령은 그대로이니 속도를 올릴수록 오버슈트가
    // 커집니다. 같은 경로를 2km/h에서 4km/h로 올렸을 때 횡편차 평균이
    // 0.78m -> 3.49m로 악화된 것이 정확히 이 이유였습니다.
    //
    // 분모는 max(|v|, 0.8) 이 아니라 (|v| + softening) 입니다. StanleyTracker 와
    // 같은 형태입니다. max() 하한은 저속에서 분모를 "더 작게" 묶어 게인을 오히려
    // 키웁니다 — 트랙터 속도대(0.5~2 m/s)가 전부 그 구간입니다. 덧셈형 완화항은
    // 모든 속도에서 분모를 키우고 정지 상태에서도 발산하지 않습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // atan saturates at 90 deg but the wheels stop at 26 deg, so the old "wrap
    // in atan so one term cannot dominate" guard only engaged 3.5x above the
    // physical limit and did nothing. In the emulator's 4 km/h run ltdTerm alone
    // averaged 52.7 deg and exceeded the limit on 80.9% of ticks. Each error term is
    // now explicitly clipped to ERR_TERM_CAP.
    //
    // ltdTerm also lacked a speed term. Classic Stanley is atan(k*e/v), which
    // shrinks the demand as speed rises; atan(k*e) asks for the same angle while
    // giving the vehicle half the time to act, so overshoot grows with speed.
    // Taking the same path from 2 to 4 km/h moved mean cross-track error from
    // 0.78 m to 3.49 m for exactly this reason.
    //
    // The denominator is (|v| + softening), matching StanleyTracker, not
    // max(|v|, 0.8). A max() floor pins the denominator SMALL at low speed and so
    // raises the gain exactly where tractors live (0.5-2 m/s); an additive
    // softening raises it at every speed and still cannot divide by zero at rest.
    // └────────────────────────────────────────────────────────────────────────┘
    const double ltdSpeed = std::fabs(currentSpeed) + params_.ppLtdSofteningMps;
    const double hdeTerm     = std::clamp(-params_.ppKHde * std::atan(effHdeErr),
                                          -ERR_TERM_CAP, ERR_TERM_CAP);
    const double ltdTerm     = std::clamp(
            std::atan(params_.ppKLtd * lateralDeviation / ltdSpeed),
            -ERR_TERM_CAP, ERR_TERM_CAP);
    // Curvature feedforward already supplies the nominal wheel angle for a
    // clean arc. Pure pursuit therefore contributes only its residual relative
    // to that nominal angle; adding both absolute angles double-counts path
    // curvature and pins otherwise valid 5-10 m arcs at the wheel stop.
    const double ppTerm = params_.ppWeightPp * (pp_steer - cf_steer);
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // ★ 부호에 주의하세요. 여기 좌표계는 왼손계입니다 — 조향 양수 = 좌회전이고,
    // 좌회전은 방위각을 "줄입니다". 자전거 모델로 쓰면
    //
    //     yawRate = -(v / L) * tan(delta)
    //
    // 이므로 지금 걸려 있는 조향각은 delta ~= -atan(L * yawRate / v) 입니다.
    // 감쇠항은 "지금 돌고 있는 방향의 반대"를 더해야 하므로 부호는 +입니다.
    //
    //     yawDampTerm = +k * yawRate  =  -k * (v/L) * tan(delta)
    //
    // 교과서(오른손계) 공식은 -k*yawRate 인데, 그걸 그대로 옮겨 오면 여기서는
    // 부호가 뒤집혀 "이미 돌고 있는 쪽으로 더 꺾는" 양의 피드백이 됩니다.
    // 에뮬레이터 로그 실측: -k*yawRate 는 이동 중 GPS 1,060샘플 전부(100.0%)에서
    // 이미 걸려 있던 조향각과 같은 방향이었습니다. 감쇠항이 아니라 증폭항이었습니다.
    //
    // HDE 항과의 정합성으로도 같은 결론이 나옵니다. e = 경로방위 - 차량방위 이고
    // hdeTerm = -k_hde * e 이므로, 짝이 되는 미분항은 -k_d * de/dt 이고
    // de/dt ~= -yawRate 이니 결국 +k_d * yawRate 입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Mind the sign. This frame is left-handed: positive steer means LEFT, and a
    // left turn DECREASES the bearing, so the bicycle model reads
    //
    //     yawRate = -(v / L) * tan(delta)
    //
    // and the angle currently on the wheels is delta ~= -atan(L * yawRate / v).
    // A damper has to push against the turn in progress, so the sign is PLUS:
    //
    //     yawDampTerm = +k * yawRate  =  -k * (v/L) * tan(delta)
    //
    // The textbook (right-handed) form is -k*yawRate; copied across unchanged it
    // becomes positive feedback that steers further into the turn already
    // happening. Measured on the emulator log, -k*yawRate pointed the same way as
    // the angle already on the wheels on 1,060 of 1,060 moving GPS samples
    // (100.0%) — the damper was an amplifier.
    //
    // Consistency with the heading term gives the same answer: with
    // e = pathHeading - vehicleHeading and hdeTerm = -k_hde * e, the matching
    // derivative term is -k_d * de/dt, and de/dt ~= -yawRate, i.e. +k_d * yawRate.
    // └────────────────────────────────────────────────────────────────────────┘
    const double yawDampTerm = params_.ppKYawDamp * yawRate;
    const double feedbackRaw = hdeTerm + ltdTerm + ppTerm + yawDampTerm;
    // Feedforward owns the nominal geometric demand. The correlated correction
    // terms share only the physical angle left after that demand, independent of
    // implement state. Keeping a small numerical/servo margin avoids converting
    // a valid near-minimum-radius arc into a command stuck exactly at hard lock.
    constexpr double STEER_MARGIN_RAD = 0.005;
    const double feedbackHeadroom = std::max(
            0.0, geometry_.maxWheelAngleRad - std::abs(cfTerm) - STEER_MARGIN_RAD);
    const double feedbackApplied = std::clamp(
            feedbackRaw, -feedbackHeadroom, feedbackHeadroom);
    double steerAngle = cfTerm + feedbackApplied;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 안티 와인드업은 액추에이터가 실제로 낼 수 있는 값까지만 잘라야 의미가
    // 있습니다. 상한이 실제 한계보다 크면 잘라도 도달 불가능한 목표가 그대로
    // 남아서, 하위 조향 루프(steerKp=50)는 영구적인 오차를 보고 모터를 ±600에
    // 물어 둡니다. 그 상태의 조향은 서보가 아니라 온/오프 릴레이이고, 릴레이가
    // 부호를 뒤집는 데 걸리는 4~5초 동안 차량은 경로를 가로질러 반대편으로
    // 넘어갑니다 — 이것이 S자 주행의 정체입니다.
    //
    // 실제로 이 함정을 두 번 밟았습니다. 처음 상한은 1.0 rad(57.3°)였고, 물리
    // 한계라고 믿은 0.56 rad(32°)로 낮췄습니다. 그런데 32°도 실측이 아니었습니다.
    // 진짜 한계는 ±26°였고(PurePursuitTracker.hpp 의 WHEEL_MAX_RAD 주석 참고),
    // 32°로 낮춘 뒤에도 지령이 상한에 붙어 있던 시간이 55.6%, 모터가 ±600에
    // 물려 있던 시간이 55.0%로 릴레이 동작이 그대로 남아 있었습니다.
    // 상한은 반드시 실측해서 넣으세요.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Anti-windup only means something if it clamps to what the actuator can
    // actually reach. A cap above the real limit still leaves an unreachable
    // target, so the inner steering loop (steerKp = 50) sees a permanent error
    // and pins the motor at +/-600. Steering is then a relay rather than a
    // servo, and the 4-5 s that relay needs to change sign is long enough for
    // the vehicle to cross the path and come out the other side — the S-weave.
    //
    // This trap was walked into twice. The cap started at 1.0 rad (57.3 deg) and
    // was lowered to 0.56 rad (32 deg), believed to be the physical limit. It was
    // not measured, and the real limit is +/-26 deg (see the WHEEL_MAX_RAD
    // comment in PurePursuitTracker.hpp). Even at 32 deg the demand still sat at
    // the cap on 55.6% of ticks with the motor pinned on 55.0% — the relay
    // behaviour survived the first fix. Measure the cap; do not assume it.
    // └────────────────────────────────────────────────────────────────────────┘
    steerAngle = std::clamp(steerAngle, -geometry_.maxWheelAngleRad,
                           geometry_.maxWheelAngleRad);

    // 후진 보정. 위의 모든 항은 전진을 가정하고 계산했으므로, 후진 구간이면
    // 마지막에 부호를 한 번 뒤집습니다. 후진 중에는 같은 방향으로 꺾어도 차체가
    // 반대로 돌기 때문입니다.
    //
    // Reverse correction: all pre-flip terms assume forward semantics; flip when reverse.
    if (target.speed < 0.0) {
        steerAngle = -steerAngle;
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (f) 속도/기어/스로틀/브레이크. SpeedController에 위임합니다.
    //
    // 기준 인덱스는 vehicleCommandIndex_ 입니다(차량 기준). 그래야 전진/후진
    // 전환이 차량이 실제로 경계를 넘는 순간에 일어납니다.
    //
    // 참고: 예전에는 cte가 클 때 속도를 줄이는 로직이 있었는데 제거했습니다.
    // 횡방향 복귀 속도는 lateral_velocity = v × sin(방향 오차) 이므로, 속도를
    // 줄이면 오히려 경로로 복귀하는 데 더 오래 걸립니다. 지금은 RDDF에 적힌
    // 속도를 그대로 존중합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (f) Speed / gear / throttle / brake — delegated to SpeedController.
    // Speed/gear driven by vehicleCommandIndex_ (vehicle-based, so F/R transitions
    // fire when vehicle physically crosses path direction boundaries).
    //
    // CTE-adaptive speed scaling was removed: slowing down during large-cte
    // recovery makes lateral recovery slower (lateral_velocity = v × sin(hdg_err)).
    // Per-RDDF speed is respected directly; speed comes solely from the waypoint.
    // └────────────────────────────────────────────────────────────────────────┘
    const TrackerWaypoint& speedWp = waypoints_[vehicleCommandIndex_];
    const double targetSpeedKmh = speedWp.speed;

    const SpeedController::Output longitudinal =
        speed_.update(targetSpeedKmh, currentSpeed, dtS, state.poseSampleId);
    const int gear = longitudinal.gear;
    const double throttle = longitudinal.throttle;
    const double brake = longitudinal.brake;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // (g) 작업기 올림/내림. 반드시 implementClosestIndex(작업기의 물리적 위치)에서
    // 가져와야 하고, commandIndex_ 를 쓰면 안 됩니다.
    //
    // commandIndex_ 는 F/R 경계에서 무조건 전진하기 때문에, 작업기가 아직 K턴
    // 원호에 남아 있는 동안에도 후진->전진 경계를 건너뛸 수 있습니다. 그 값으로
    // 판단하면 쟁기가 너무 일찍 내려가 밭이 아닌 곳을 갈게 됩니다.
    // implementClosestIndex 를 쓰면, 작업기가 물리적으로 flag=1 웨이포인트에
    // 도달한 뒤에만 내려갑니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // (g) Implement flag: derived from implementClosestIndex (implement PHYSICAL
    // body position), NOT commandIndex_. commandIndex_ has F/R unconditional advance
    // that can jump it across reverse->forward boundary while implement body is
    // still trailing in the K-turn arc, causing premature DOWN command. Using
    // implementClosestIndex ensures DOWN fires only when implement physically
    // reaches a flag=1 waypoint.
    // └────────────────────────────────────────────────────────────────────────┘
    int implement = waypoints_[implementClosestIndex].implementFlag;

    // (h) 외부(UI, CSV 트레이스)로 나갈 진단 정보를 채웁니다.
    // (h) Fill debug telemetry for external consumers.
    {
        // lateralDeviation, segHeading, headingError 은 위 (e0)/(e1)에서 이미 계산했습니다.
        // lateralDeviation, segHeading, headingError already computed in (e0)/(e1) above.

        // 전방 목표점까지의 직선 거리. / Euclidean distance to the lookahead point.
        double lhDx = target.x - currentX;
        double lhDz = target.z - currentZ;
        double lookaheadDistM = std::sqrt(lhDx * lhDx + lhDz * lhDz);

        // 속도 오차: 목표(m/s) - 현재(m/s). 현재 속도의 부호는 그대로 둡니다.
        // Speed error: target (m/s) - current (m/s). current speed sign preserved.
        double targetSpeedMs = targetSpeedKmh * KMH_TO_MS;
        double debugSpeedErr = targetSpeedMs - currentSpeed;

        // 모든 트래커가 반드시 채워야 하는 계약 필드입니다.
        // Contract fields every tracker fills.
        debug_.lateralDeviationM    = lateralDeviation;
        debug_.headingErrorRad  = headingError;
        debug_.speedErrMps    = debugSpeedErr;
        // speedWp(=vehicleCommandIndex_) 의 속도여야 합니다. 예전에는 전방 목표점
        // target.speed 를 내보내서, 대시보드와 PP_LOG 의 tV_kmh 가 제어기가 실제로
        // 쓰는 목표와 다른 웨이포인트를 가리켰습니다.
        // Must be speedWp (= vehicleCommandIndex_). This used to publish the
        // lookahead target.speed, so the dashboard and PP_LOG's tV_kmh showed a
        // different waypoint from the one the controller actually tracks.
        debug_.targetSpeedKmh = targetSpeedKmh;
        debug_.targetIdx      = static_cast<int>(targetIndex);
        debug_.closestIdx     = static_cast<int>(closestIndex);
        debug_.commandIdx     = static_cast<int>(commandIndex_);

        // 여기서부터는 Pure Pursuit 고유 값입니다. 이름 그대로 텔레메트리와 CSV
        // 트레이스로 흘러갑니다. 다른 알고리즘은 그냥 다른 항목을 발행하면 됩니다.
        //
        // Pure-Pursuit specifics. These flow through to telemetry and the CSV
        // trace by name; another algorithm simply publishes a different set.
        debug_.clearExtras();
        debug_.put("lookaheadDist", lookaheadDistM);
        debug_.put("xLocal", x_local);
        debug_.put("zLocal", z_local);
        debug_.put("implementClosestIdx", static_cast<double>(implementClosestIndex));
        debug_.put("vehicleCmdIdx", static_cast<double>(vehicleCommandIndex_));
        // 조향 항 분해(후진 부호 뒤집기 전 값이며, 다 더하면 뒤집기 전 조향값이
        // 됩니다). 튜닝할 때 어느 항이 폭주하는지 이걸로 확인합니다.
        // Steering term decomposition (pre-flip; sums to steerAngle pre-flip).
        debug_.put("cfTerm", cfTerm);
        debug_.put("hdeTerm", hdeTerm);
        debug_.put("ltdTerm", ltdTerm);
        debug_.put("ppTerm", ppTerm);
        debug_.put("yawDampTerm", yawDampTerm);
        debug_.put("feedbackRaw", feedbackRaw);
        debug_.put("feedbackApplied", feedbackApplied);
        // 종방향 PID 분해. SpeedController 안에서만 알 수 있는 값이라 여기서
        // 꺼내 실어 보냅니다 — 대시보드의 종방향 화면이 이걸로 게인을 봅니다.
        // Longitudinal PID decomposition. These are only visible inside
        // SpeedController, so they are surfaced here for the dashboard's
        // longitudinal screen to tune against.
        debug_.put("longP", longitudinal.pTerm);
        debug_.put("longI", longitudinal.iTerm);
        debug_.put("longD", longitudinal.dTerm);
        debug_.put("longTargetMps", longitudinal.targetMps);
        debug_.put("longReversing", longitudinal.brakingForReversal ? 1.0 : 0.0);
        // 경로가 요구한 속도와, 기계 하한 때문에 올려서 따라간 속도. 둘이 다르면
        // 그 구간은 RDDF가 요구한 속도로 달리지 않은 것입니다.
        // What the path asked for vs. what was followed after the machine-floor
        // clamp. Differing values mean that stretch did not run at the RDDF speed.
        debug_.put("longRequestedMps", longitudinal.requestedMps);
        debug_.put("longFloorClamped", longitudinal.targetRaisedToFloor ? 1.0 : 0.0);
    }

    TrackerCommand cmd{steerAngle, throttle, brake, gear, implement};
    lastValidCmd_ = cmd;
    return cmd;
}

} // namespace tracking
} // namespace AppMain
