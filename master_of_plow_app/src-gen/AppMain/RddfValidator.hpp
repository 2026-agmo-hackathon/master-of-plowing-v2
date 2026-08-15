/*
 * AppMain/RddfValidator.hpp
 */

#ifndef APPMAIN_RDDFVALIDATOR_HPP
#define APPMAIN_RDDFVALIDATOR_HPP

#include <cmath>
#include <string>
#include <vector>
#include "RddfParser.hpp"
#include <AppMain/tracking/SpeedController.hpp>
#include <AppMain/tracking/impl/PurePursuitTracker.hpp>

namespace AppMain {

struct ValidationResult {
    bool accepted;
    std::string reason;
    std::vector<std::string> warnings;
};

class RddfValidator {
public:
    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 이 기계가 연속으로 낼 수 있는 최저 지면속도입니다. 에뮬레이터 주행 4회의
     * GPS 샘플 3,865개를 보면 속도가 0.00 / 0.57 / 0.62 / 1.03 / 1.08 m/s 같은
     * 이산값에만 존재하고, 0.05~0.50 m/s 구간에 들어가는 샘플은 28개(0.72%)
     * 뿐입니다. Low단 크리프 하한이 약 0.57 m/s = 2.05 km/h 라는 뜻입니다.
     *
     * 이보다 느린 속도를 요구해도 목표는 그대로 지켜집니다. SpeedController 가
     * 가장 낮은 단을 물고 개도를 끊었다 열었다 해서 평균을 맞춥니다 — 목표를
     * 하한으로 올리던 처리는 없앴습니다. 참가자 RDDF 는 수정하거나 거부하지
     * 않고 그대로 적재합니다.
     *
     * 그래서 여기 경고의 뜻은 "이 속도는 연속으로 유지되지 않고 톱니처럼
     * 오르내린다"입니다. 못 낸다는 뜻이 아닙니다. 평균은 맞지만 순간속도는
     * 흔들리므로, 작업 품질이 걸린 구간이라면 알고 있어야 하는 사실입니다.
     *
     * 이 값은 compact 트랙터에서 잰 것이고, 무거운 조합의 1단은 이보다 느립니다
     * (large 4WD + 3.6m 쟁기: 1.48 km/h). 검증기에는 실행 맥락이 없으므로 —
     * 트랙터도 작업기도 아직 정해지지 않았습니다 — 이 보수적인 값으로 경고하고,
     * 실제 가장 느린 단의 속도는 SpeedController 가 주행 중 측정해
     * belowSlowestStage 로 알립니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Lowest ground speed this machine can hold continuously. Across 3,865 GPS
     * samples from four emulator runs the reported speed only ever takes
     * discrete values — 0.00 / 0.57 / 0.62 / 1.03 / 1.08 m/s — with just 28
     * samples (0.72%) anywhere in 0.05..0.50 m/s. The Low-gear creep floor is
     * therefore about 0.57 m/s = 2.05 km/h.
     *
     * A lower request is still honoured: SpeedController engages the slowest
     * stage and pulses the opening so the mean matches. Raising the target to the
     * floor is gone. Participant RDDFs are loaded without mutation or whole-file
     * rejection.
     *
     * The warning therefore means "this speed will not be held continuously, it
     * will saw around the value" — not "this speed is unreachable". The mean is
     * right but the instantaneous speed ripples, which is worth knowing where
     * work quality depends on it.
     *
     * This was measured on the compact tractor, and stage 1 on a heavier pairing
     * is slower (1.48 km/h for the large 4WD with a 3.6 m plough). Validation has
     * no run context — neither tractor nor implement is chosen yet — so it warns
     * against this conservative value while SpeedController measures the real
     * slowest stage during the run and reports it as belowSlowestStage.
     *
     * 값 자체는 SpeedController 가 갖고 있습니다. 검증기와 런타임이 같은 값을
     * 공유해야 경고와 텔레메트리가 서로 어긋나지 않습니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * The value itself lives in SpeedController. Validation and runtime share it
     * so the load warning and runtime telemetry cannot disagree.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    static constexpr double MIN_MACHINE_SPEED_KMH =
            tracking::SpeedController::MIN_MACHINE_SPEED_KMH;
    static constexpr double MAX_MACHINE_SPEED_KMH = 7.0;

    // The tracker assumes a dense polyline. Larger gaps allow the vehicle to
    // cut across unrepresented geometry; near-duplicates make curvature
    // estimates numerically unstable. Both are route errors, not tuning issues.
    static constexpr double MIN_WAYPOINT_SPACING_M = 0.05;
    static constexpr double MAX_WAYPOINT_SPACING_M = 5.0;

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 이 기계가 돌 수 있는 가장 작은 회전 반경입니다.
     *
     *   R = WHEELBASE_M / tan(WHEEL_MAX_RAD) = 2.05 / tan(0.44) = 4.36 m
     *
     * 경로가 이보다 급한 코너를 요구하면 트랙터는 물리적으로 못 따라갑니다.
     * 조향은 끝까지 물린 채로 코너 바깥으로 밀려나고, 벌어진 횡편차는 다음
     * 직선 구간에 그대로 넘어가 거기서 다시 복구 조향을 잡아먹습니다. 속도
     * 하한과 마찬가지로 제어 게인으로는 고칠 수 없습니다 — 경로를 고쳐야 합니다.
     *
     * 두 값 모두 트래커와 같은 상수를 써야 합니다. 검증기가 다른 숫자를 쓰면
     * 경고 없이 못 도는 코너가 생깁니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Tightest circle this machine can drive:
     *
     *   R = WHEELBASE_M / tan(WHEEL_MAX_RAD) = 2.05 / tan(0.44) = 4.36 m
     *
     * A path with a tighter corner than this cannot be followed. The steering
     * goes to full lock, the vehicle runs wide, and the cross-track error it
     * picks up carries into the next straight where it eats the steering
     * authority again. Like the speed floor, no control gain fixes it — the path
     * has to change.
     *
     * Both constants come from the tracker; a validator using different numbers
     * would leave corners that are undrivable but never warned about.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    static double minTurnRadiusM()
    {
        return tracking::PurePursuitTracker::WHEELBASE_M
             / std::tan(tracking::PurePursuitTracker::WHEEL_MAX_RAD);
    }

    /** Validate only properties intrinsic to the submitted RDDF. Runtime spawn
     * proximity belongs to the simulator/session start gate, not file parsing. */
    static ValidationResult validate(const std::vector<RddfWaypoint>& waypoints);

private:
    static double haversineMeters(double lat1, double lon1,
                                  double lat2, double lon2);
};

} // namespace AppMain

#endif // APPMAIN_RDDFVALIDATOR_HPP
