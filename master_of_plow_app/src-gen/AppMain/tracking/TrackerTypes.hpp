/*
 * AppMain/tracking/TrackerTypes.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 경로 추종 알고리즘과 제어 루프가 주고받는 데이터 타입입니다.
 * SeamOS SDK나 JSON에 전혀 의존하지 않으므로, 트래커를 앱 없이 단독으로
 * 컴파일하고 테스트할 수 있습니다.
 *
 * ★ 트래커를 직접 만들기 전에 아래 "좌표계와 부호 규약"을 꼭 읽으세요.
 *   트래커가 제자리에서 뱅뱅 도는 버그는 거의 전부 부호를 반대로 잡아서
 *   생깁니다.
 *
 * ── 좌표계와 부호 규약 ──
 *
 *   위치   : ENU 평면상의 (x, z), 단위는 미터.
 *            +x = 동쪽(East), +z = 남쪽(South).
 *            원점은 RDDF의 0번 웨이포인트입니다.
 *            주의: 이 좌표계는 왼손 좌표계입니다 (아래 heading 설명 참고).
 *
 *   heading: 라디안. atan2(z, x)로 측정합니다.
 *            즉 heading 0 = 동쪽을 봄, +pi/2 = 남쪽을 봄.
 *            GPS가 주는 나침반 방위(0 = 북쪽, 시계방향 +)는 제어 루프가
 *            이 좌표계로 변환해 주므로, 트래커는 변환된 값만 받습니다.
 *
 *   조향   : 라디안. 양수 = 좌회전(LEFT).
 *            좌표계가 왼손계이므로 좌회전은 heading을 "감소"시킵니다.
 *            구체적으로: heading 0(동쪽)으로 달리는 차량이 경로보다 +z쪽
 *            (남쪽)으로 밀려 있다면, 경로로 돌아오려면 조향이 음수여야
 *            할 것 같지만 실제로는 양수(좌회전 = 북쪽으로 틀기)입니다.
 *
 *   속도   : 웨이포인트의 speed는 km/h이고 부호가 있습니다 (음수 = 후진 구간).
 *            반면 VehicleState::speedMps는 m/s이고 부호가 없습니다.
 *            GPS는 속력(크기)만 알려주기 때문에, 지금 전진 중인지 후진
 *            중인지는 측정값이 아니라 내가 지시한 기어로 판단해야 합니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Data types exchanged between a path-tracking algorithm and the control loop.
 * Deliberately free of SeamOS/JSON dependencies, so a tracker can be compiled
 * and unit-tested standalone without the app.
 *
 * ★ Read the conventions below before writing a tracker. Getting a sign
 *   backwards is the single most common reason a tracker drives in circles.
 *
 * ── Coordinate and sign conventions ──
 *
 *   Position:  ENU metres in a plane spanned by (x, z).
 *              +x = East, +z = South. Origin = waypoint 0 of the loaded RDDF.
 *              Note this is a LEFT-handed frame; see the heading note below.
 *
 *   Heading:   radians, measured as atan2(z, x). So heading 0 = facing East,
 *              +pi/2 = facing South. The control loop converts the GPS compass
 *              heading (0 = North, clockwise +) into this frame; a tracker only
 *              ever sees the converted value.
 *
 *   Steering:  radians, positive = steer LEFT. Because the frame is left-handed,
 *              a left turn DECREASES heading. Concretely: a vehicle at heading 0
 *              that is offset toward +z (south of the path) must steer POSITIVE
 *              (left, i.e. toward North) to come back — not negative, as the
 *              word "right" might suggest.
 *
 *   Speed:     waypoint speeds are km/h and signed (negative = reverse segment).
 *              VehicleState::speedMps is metres/second and UNSIGNED, because GPS
 *              reports a magnitude only — direction of travel has to be inferred
 *              from the commanded gear, not from the measurement.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_TRACKERTYPES_HPP
#define APPMAIN_TRACKING_TRACKERTYPES_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace AppMain
{
namespace tracking
{

/** Physical steering and transmission limits selected by the live simulator.
 * A copy is frozen for each run. Defaults preserve the original compact tractor
 * behavior when the live catalog is unavailable, but an orchestrated run never
 * starts until a catalog geometry has been selected and confirmed. */
struct VehicleGeometry
{
    double wheelbaseM = 2.05;
    double maxWheelAngleRad = 0.44;
    double steeringRatio = 24.2;
    double steeringWheelMaxRateDegS = 435.0;
    double steeringWheelTauS = 0.2;
    double frontWheelTauS = 0.05;
    int maxForwardGear = 4;

    bool valid() const
    {
        return wheelbaseM > 0.5 && wheelbaseM < 10.0 &&
               maxWheelAngleRad > 0.05 && maxWheelAngleRad < 1.2 &&
               steeringRatio > 1.0 && steeringRatio < 100.0 &&
               steeringWheelMaxRateDegS > 1.0 &&
               steeringWheelTauS >= 0.0 && frontWheelTauS >= 0.0 &&
               maxForwardGear >= 1 && maxForwardGear <= 12;
    }
};

/** Convert the IMU's raw counter-clockwise-positive GyroZ into the tracking
 * frame's clockwise-positive heading-rate convention. This is the single
 * sensor-boundary sign normalization; all controllers consume this convention. */
inline double internalYawRateFromRawGyroDegS(double rawGyroDegS)
{
    constexpr double DEG_TO_RAD = 0.017453292519943295;
    return -rawGyroDegS * DEG_TO_RAD;
}

/**
 * RDDF 웨이포인트 하나. 이미 ENU 좌표로 변환된 상태입니다.
 * One RDDF waypoint, already projected into the ENU frame.
 */
struct TrackerWaypoint
{
    double x = 0.0;         // ENU 미터 (원점 = 0번 웨이포인트) / ENU metres (origin = waypoint 0)
    double z = 0.0;         // ENU 미터 / ENU metres
    double speed = 0.0;     // km/h, 부호 있음 (음수 = 후진) / km/h, signed (negative = reverse)
    int implementFlag = 0;  // 0 = 작업기 올림(이동), 1 = 내림(작업) / 0 = up (transit), 1 = down (working)
};

/**
 * 매 제어 주기마다 트래커에게 전달되는 차량의 자세와 속력입니다.
 * Vehicle pose and speed handed to the tracker each control tick.
 */
struct VehicleState
{
    double x = 0.0;           // ENU 미터 / ENU metres
    double z = 0.0;           // ENU 미터 / ENU metres
    double headingRad = 0.0;  // atan2(z, x) 규약, 파일 상단 참고 / atan2(z, x) convention, see header comment
    double speedMps = 0.0;    // 부호 없는 속력 / unsigned magnitude
    // Coherent GPS tuple identity. It advances for every received NMEA sample,
    // including a fresh sample whose numeric values happen to be unchanged.
    std::int64_t poseSampleId = -1;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 아래는 SeamOS 머신 인터페이스에서 들어오는 값입니다. 위 네 개만 쓰는
    // 트래커는 그대로 동작합니다 — 전부 기본값이 있는 추가 필드입니다.
    //
    // 값이 없을 때를 구분해야 하는 필드에는 *Valid 플래그를 함께 둡니다.
    // 예: 조향 모터 응답이 아직 안 왔으면 steerAngleRad 는 0이 아니라 "모름"
    // 입니다. 0으로 착각해 PID를 돌리면 직진이라고 믿고 조향을 밀어붙입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Everything below arrives over the SeamOS machine interfaces. Trackers that
    // only use the four fields above keep working — these are purely additive.
    //
    // Fields that must distinguish "no reading yet" from zero carry a *Valid
    // flag. A missing steering feedback is not 0 rad; treating it as 0 would make
    // the PID believe the wheels are straight and wind up against the stop.
    // └────────────────────────────────────────────────────────────────────────┘

    // 실측 앞바퀴 조향각. 라디안, 양수 = 좌회전 (지령과 같은 부호 규약).
    // 조향 모터 엔코더와 yaw rate를 융합한 값입니다.
    // Measured front-wheel angle, radians, positive = left (same sign convention
    // as the command). Fused from the steering encoder and the yaw rate.
    double steerAngleRad = 0.0;
    bool steerAngleValid = false;

    // 요레이트. 라디안/초. 부호는 heading 증가(시계방향)를 따릅니다. 원시
    // GyroZ의 반시계방향 양수 규약은 생산 경계에서 정확히 한 번 뒤집힙니다.
    // Yaw rate in rad/s, positive with increasing (clockwise) heading. Raw
    // counter-clockwise-positive GyroZ is inverted exactly once at the boundary.
    double yawRateRadS = 0.0;
    bool yawRateValid = false;
    // Monotonic interface timestamp/sequence. Repeated values identify a held
    // frame and must not advance a discrete-time controller.
    std::int64_t yawRateSampleId = -1;

    // 차체 자세. 라디안. 경사 보상 피드포워드에 쓸 수 있습니다.
    // Body attitude in radians; usable for slope feed-forward.
    double pitchRad = 0.0;
    double rollRad = 0.0;
    bool attitudeValid = false;

    // 종/횡 가속도. m/s^2.
    // Longitudinal / lateral acceleration in m/s^2.
    double accelX = 0.0;
    double accelY = 0.0;
    bool accelValid = false;

    // 변속기가 실제로 물려 있는 상태. 지시값이 아니라 피드백입니다.
    //   fnrState   : 0 = 모름, 1 = 후진(R), 2 = 중립(N), 3 = 전진(F)
    //   shiftState : 0 = 모름, 1 = Low, 2 = Middle, 3 = High
    // Actual transmission state as reported by the tractor (feedback, not the
    // command). 0 means "unknown".
    int fnrState = 0;
    int shiftState = 0;

    // 히치 위치. 0..100 %.
    // Hitch position, 0..100 %.
    double hitchPct = 0.0;
    bool hitchValid = false;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 자율 제어권을 실제로 받았는가. 트랙터가 각 CMD의 mode 비트를 수락하면
    // TRZ_*_AUTO 가 1로 올라옵니다. 이 값이 false면 우리가 보내는 지령은
    // 무시되고 있는 것이므로, 제어가 듣지 않을 때 첫 번째로 봐야 하는 값입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Whether the tractor actually granted autonomous authority: it raises
    // TRZ_*_AUTO once it accepts the mode bit on our commands. While this is
    // false our commands are being ignored — the first thing to check when
    // nothing responds.
    // └────────────────────────────────────────────────────────────────────────┘
    bool controlAuthority = false;

    // GPS 품질. 자율주행 가능 여부를 좌우합니다.
    //   gpsFixType: NMEA GGA quality indicator (4 = RTK fixed, 5 = float)
    // GPS quality, which gates whether autonomous driving is viable at all.
    int gpsFixType = 0;
    int gpsSats = 0;
    double gpsHdop = 0.0;

    // 조향 모터 고장 비트. 0이 아니면 조향을 신뢰할 수 없습니다.
    // Steering motor fault bits; non-zero means steering cannot be trusted.
    unsigned int motorFaultBits = 0;
};

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * VehicleState::motorFaultBits 의 비트 위치입니다. Motor_Heartbeat 가 주는 10개
 * 상태 플래그를 하나의 정수로 접어 텔레메트리와 인터록에서 함께 쓰기 위한 것입니다.
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Bit positions in VehicleState::motorFaultBits: the ten Motor_Heartbeat status
 * flags folded into one integer so telemetry and the interlock can share it.
 * └────────────────────────────────────────────────────────────────────────────┘
 */
enum MotorFaultBit
{
    MOTOR_FAULT_HALL               = 1u << 0,
    MOTOR_FAULT_CAN_DISCONNECTED   = 1u << 1,
    MOTOR_FAULT_STALLED            = 1u << 2,
    MOTOR_FAULT_DISABLED           = 1u << 3,
    MOTOR_FAULT_OVERVOLTAGE        = 1u << 4,
    MOTOR_FAULT_HW_PROTECTION      = 1u << 5,
    MOTOR_FAULT_E2PROM             = 1u << 6,
    MOTOR_FAULT_UNDERVOLTAGE       = 1u << 7,
    MOTOR_FAULT_OVERCURRENT        = 1u << 8,
    MOTOR_FAULT_MODE_FAILURE       = 1u << 9
};

/**
 * 조향을 신뢰할 수 없게 만드는 고장들. MOTOR_FAULT_DISABLED 는 제외합니다 —
 * 지령 0을 보내면 우리가 스스로 Disable 하므로 정상 상태에서도 켜집니다.
 *
 * Faults that make steering untrustworthy. MOTOR_FAULT_DISABLED is excluded: we
 * disable the motor ourselves whenever the command is zero, so it is set during
 * normal operation too.
 */
inline bool isSteeringBlockingFault(unsigned int bits)
{
    return (bits & ~static_cast<unsigned int>(MOTOR_FAULT_DISABLED)) != 0u;
}

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * 런타임 튜닝 파라미터. 예전에는 각 파일의 constexpr 상수였는데, 튜닝 UI가
 * 주행 중에 바꿀 수 있도록 값으로 옮겼습니다. 기본값은 반드시 기존 상수와
 * 동일해야 합니다 — 튜닝 UI를 건드리지 않으면 동작이 이전과 같아야 합니다.
 *
 * 물리 상수(WHEELBASE_M, GOAL_TOLERANCE_M, 탐색 윈도우 등)는 튜닝 대상이
 * 아니므로 여기 넣지 않고 constexpr 로 남깁니다.
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Runtime tuning parameters. These used to be per-file constexpr constants;
 * they became values so the tuning UI can change them mid-run. Defaults MUST
 * equal the old constants — untouched tuning must behave exactly as before.
 *
 * Physical constants (wheelbase, goal tolerance, search windows, ...) are not
 * tuning knobs and stay constexpr in their own files.
 * └────────────────────────────────────────────────────────────────────────────┘
 */
struct TrackerParams
{
    // ┌─ 한국어: 선행거리는 "루프 총 지연"으로 정해집니다 ─────────────────────┐
    //
    // 선행거리(lookahead)는 경로 모양이 아니라 제어 루프의 지연으로 정해야
    // 합니다. 이 기계에서 실측한 지연은 다음과 같습니다.
    //
    //   수정 전 mux GPS ~1Hz → 위치·방위가 최대 1초 낡았음 (평균 0.5초)
    //   조향 불감시간 2초   → 지령이 10도 이상 어긋난 뒤 바퀴가 실제로 움직이기까지
    //                        중앙값 2초. 10Hz 루프 기준 20틱이므로 우리 PID가
    //                        아니라 유압/모터 쪽 특성입니다.
    //   조향 링크 지연       → 모터축 rate와 별개로 앞바퀴 응답이 늦음
    //
    // 당시 전체 지연 envelope는 약 4.5초였습니다. 현재 에뮬레이터는 GPS와
    // encoder를 약 10Hz로 내보내지만, 9초 기본값은 held/delayed 센서에도 견디는
    // 보수적 generic 기본값으로 matrix 회귀에서 유지합니다. 선행"시간"이
    // 안정합니다. 그렇지 않으면 제어기는 이미 지나간 오차를 보고 조향을 걸게
    // 되고, 조향은 좌우 한계를 오가는 릴레이가 됩니다. 그 리밋 사이클이 바로
    // S자 사행입니다.
    //
    // 예전 기본값(lkT 2.5초)은 4km/h에서 선행거리가 하한 5.5m에 붙어 있었고,
    // 에뮬레이터 실측에서 조향 지령이 상한에 붙은 시간이 72.5%, 좌우 풀락 반전이
    // 100초당 8.8회였습니다. 경로를 완주하지도 못했습니다.
    //
    // 시간으로 두면 선행거리가 속도에 따라 저절로 늘고 줄어, 저속 코너와 고속
    // 직선을 한 설정으로 덮습니다. 0.57m/s 에서 5.1m, 1.08m/s 에서 9.7m 입니다.
    //
    // ★ 참가자에게: 코너를 더 붙여 돌고 싶으면 ppLookaheadTimeS 를 줄이세요.
    //   다만 4.5초 아래로 내리면 위 지연 때문에 사행이 돌아옵니다. 조향
    //   불감시간을 직접 줄일 수 있다면(하드웨어/설정) 그만큼 더 내릴 수 있습니다.
    //   측정 근거와 스윕 결과는 tests/FieldReplayTest.cpp 를 보세요.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English: the lookahead is set by the loop's total lag ────────────────┐
    //
    // Lookahead is set by control-loop lag, not by path shape. Measured on this
    // machine: before the mux fix, GPS at roughly 1 Hz left pose up to 1 s stale;
    // the steering had a 2 s
    // median dead time from a >10 deg demand error to the wheel actually moving
    // (20 ticks of a 10 Hz loop, so it is the hydraulics, not our PID); and the
    // steering-link lag independent of the motor-column rate. That earlier
    // end-to-end envelope was about 4.5 s. The fixed emulator now publishes GPS
    // and encoder near 10 Hz; 9 s remains a conservative generic default guarded
    // by held/delayed-sensor matrix tests.
    //
    // The lookahead TIME must comfortably exceed that lag. Otherwise the
    // controller steers on an error that has already gone stale, and steering
    // degenerates into a relay between the two limits — that limit cycle is the
    // S-weave. The previous default (2.5 s) pinned the lookahead at its 5.5 m
    // floor at 4 km/h; the emulator then sat at the steering limit 72.5% of the
    // time with 8.8 lock-to-lock reversals per 100 s, and never finished the path.
    //
    // Expressed as a time, the distance scales with speed on its own and one
    // setting covers both cases: 5.1 m at 0.57 m/s, 9.7 m at 1.08 m/s.
    //
    // For participants: lower ppLookaheadTimeS to hug corners more tightly, but
    // below about 4.5 s the weave returns unless the dead time itself is reduced.
    // See tests/FieldReplayTest.cpp for the measurements and the sweep.
    // └────────────────────────────────────────────────────────────────────────┘
    // 상한은 선행"시간"을 지키기 위한 값이지 튜닝 값이 아닙니다. 상한이 물리면
    // 그 속도에서는 선행시간이 짧아지고, 위에서 설명한 지연 조건이 깨져 사행이
    // 돌아옵니다. 실제로 상한 12m 는 8km/h 에서 선행시간을 9초 -> 5.4초로 줄여
    // ClosedLoopTrackingTest 를 깨뜨렸습니다. 이 기계는 3.9km/h 가 최고속이라
    // 1.08m/s 에서 9.7m — 상한은 물리지 않습니다.
    // The cap exists to preserve the lookahead TIME, and is not a tuning knob:
    // where it binds, the time shortens and the lag condition above is violated.
    // A 12 m cap cut the time from 9 s to 5.4 s at 8 km/h and broke
    // ClosedLoopTrackingTest. This machine tops out at 3.9 km/h, giving 9.7 m, so
    // the cap does not bind in practice.
    double ppLookaheadMinM = 5.0;
    double ppLookaheadMaxM = 20.0;
    double ppLookaheadTimeS = 9.0;
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 기본값은 "Pure Pursuit 항이 주도하고 나머지는 거들기만 한다"로 잡혀 있습니다.
    // Pure Pursuit 는 이미 횡편차와 방향 오차를 동시에 잡는 알고리즘이라, LTD·HDE
    // 항을 같은 크기로 얹으면 같은 오차를 세 번 보정하게 됩니다.
    //
    // 예전 기본값(ppKLtd 0.5 / ppKHde 1.0 / ppWeightPp 0.45)이 정확히 그 상태였고,
    // 에뮬레이터 4km/h 주행에서 조향 지령이 상한에 붙어 있던 시간이
    // 주행 GPS 샘플의 92%였습니다. 오차 항 두 개가 중앙값 오차에서 이미 각자
    // 각자의 상한에 걸려 합이 항상 한계를 넘었기 때문입니다. 그러면 조향은 사실상
    // 릴레이(bang-bang)가 되고, 리밋 사이클이 곧 S자 사행입니다.
    //
    // 아래 값은 held pose 최대 1초, 방위 잡음 2°, 액추에이터 1차 지연 0.3s의
    // 재현한 시뮬레이션에서 정한 값입니다. 같은 조건에서 포화율 62~88% -> 1% 미만,
    // 선회 구간 횡편차 평균 0.224m -> 0.066m 입니다.
    // 회귀 검증은 tests/ClosedLoopTrackingTest.cpp 가 담당합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The defaults are set so the pure-pursuit term leads and the rest only trim.
    // Pure pursuit already corrects cross-track AND heading, so adding LTD and HDE
    // terms at comparable gain corrects the same error three times over.
    //
    // The former defaults (ppKLtd 0.5 / ppKHde 1.0 / ppWeightPp 0.45) did exactly
    // that: in the emulator at 4 km/h the steering demand sat at the physical
    // steering limit on 92% of moving GPS samples, because both error terms were
    // already at their 16 deg cap at the MEDIAN error and summed to the whole
    // range. That makes steering a bang-bang relay, and the limit cycle is the
    // S-weave.
    //
    // The values below come from a closed-loop simulation of those conditions
    // (pose held up to 1 s, 2 deg heading noise, 0.3 s actuator lag): saturation drops from
    // 62-88% to under 1%, and mean cross-track through a turn from 0.224 m to
    // 0.066 m. tests/ClosedLoopTrackingTest.cpp guards the result.
    // └────────────────────────────────────────────────────────────────────────┘
    double ppKLtd = 0.1;          // LTD(횡방향 편차) 게인 / LTD (lateral deviation) gain
    double ppKHde = 0.2;          // HDE(방향 오차) 게인 / HDE (heading error) gain
    // 선행거리를 늘려도 조향 지령이 여전히 한계에 붙어 있어 0.7 로 내렸습니다.
    // 위 실측 지연(약 4.5초)에서 1.0 은 포화 34%/반전 4.3회, 0.7 은 포화 15%/반전
    // 1.3회, 0.5 는 더 낮지만 코너 최대 편차가 커집니다(4.8m -> 5.4m).
    // Lowered from 1.0 because the demand still sat at the limit with the longer
    // lookahead. Against the measured ~4.5 s lag, 1.0 gives 34% saturation and
    // 4.3 reversals/100 s, 0.7 gives 15% and 1.3, and 0.5 trades a little more
    // saturation reduction for a worse worst-case corner error (4.8 -> 5.4 m).
    double ppWeightPp = 0.7;      // Pure Pursuit 항 가중치 / pure-pursuit term weight
    double ppKYawDamp = 0.3;      // yaw rate 감쇠(D 항) / yaw-rate damping (D term)
    // LTD 항 속도 정규화의 완화항: atan(k·e / (|v| + softening)).
    // 0 이면 정지 상태에서 분모가 0 이 되므로 하한을 양수로 둡니다.
    // Softening for the LTD speed normalisation: atan(k*e / (|v| + softening)).
    // Zero would divide by zero at rest, so the floor stays positive.
    double ppLtdSofteningMps = 0.5;
    // Stanley
    double stKLtd = 1.0;
    double stSofteningMps = 0.5;
    // 조향 PID (SteeringController) — 오차 단위는 도(degree)
    // Steering PID (SteeringController) — the error unit is DEGREES
    double steerKp = 50.0;
    double steerKi = 0.1;
    double steerKd = 5.0;
    // 종방향 PID (SpeedController)
    // Longitudinal PID (SpeedController)
    double longKp = 1.5;
    double longKi = 0.05;
    double longKd = 0.1;
};

/**
 * 매 제어 주기마다 트래커가 내놓는 액추에이터 명령입니다.
 * Actuator command produced by the tracker each control tick.
 */
struct TrackerCommand
{
    double steerAngle = 0.0;  // 라디안, 양수 = 좌회전 / radians, positive = left
    double throttle = 0.0;    // 0..1
    double brake = 0.0;       // 0..1
    int gear = 1;             // -1(후진), 또는 1..4(전진) / -1 (reverse), or 1..4 forward
    int implement = 0;        // 0 = 올림, 1 = 내림 / 0 = up, 1 = down
};

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * UI와 CSV 트레이스로 나가는 진단 정보입니다.
 *
 * 이름이 붙은 필드는 "계약"입니다. 모든 트래커가 채워야 하고, UI와 채점
 * 경로가 이 값을 읽습니다. 알고리즘마다 다른 값은 put()으로 extras에
 * 넣으면 되고, 그대로 텔레메트리와 CSV 트레이스까지 흘러갑니다.
 *
 * extras를 map이 아니라 순서가 있는 vector로 둔 이유: CSV 헤더는 첫 번째
 * 주기에 한 번만 기록되므로 컬럼 순서가 고정되어야 합니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Diagnostics published to the UI and the CSV trace.
 *
 * The named fields are the contract: every tracker fills them, and the UI and
 * scoring path read them. Anything algorithm-specific goes into `extras` via
 * put(), which flows through to telemetry and the CSV trace untouched.
 *
 * `extras` is an ordered vector rather than a map because the CSV header is
 * written once on the first tick — column order has to stay stable.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */
struct TrackerDebug
{
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // LTD (Lateral Deviation) — 경로와의 횡방향 편차. +z쪽에 있으면 양수.
    // HDE (Heading Error)     — 경로 접선과 차량 방향의 차이. [-pi, pi]로 정규화.
    //
    // 약어 LTD/HDE 는 팀 표준 용어입니다. 참조 구현(agmo_solution)의
    // lateral_deviation_gain_* / heading_error_gain_* / setWeight(weight_ltd_,
    // weight_hde_) 와 같은 이름을 쓰기 위해 예전 이름(lateralDeviationM, headingErrorRad)
    // 에서 바꿨습니다. 텔레메트리 키와 CSV 컬럼도 ltd / hde 로 통일돼 있습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // LTD (Lateral Deviation): signed offset from the path, positive on the +z side.
    // HDE (Heading Error): signed angle between the path tangent and the vehicle
    // heading, wrapped to [-pi, pi].
    //
    // LTD/HDE are the team's standard abbreviations, matching the reference
    // implementation's lateral_deviation_gain_* / heading_error_gain_* /
    // setWeight(weight_ltd_, weight_hde_). Renamed from lateralDeviationM/headingErrorRad;
    // the telemetry keys and CSV columns use ltd / hde too.
    // └────────────────────────────────────────────────────────────────────────┘
    double lateralDeviationM = 0.0;
    double headingErrorRad = 0.0;
    // 목표 속도 - 현재 속도. / Target minus current.
    double speedErrMps = 0.0;
    // 현재 따르고 있는 웨이포인트의 목표 속도(부호 있음). / Signed target speed at the active waypoint.
    double targetSpeedKmh = 0.0;
    // 조향의 목표로 삼고 있는 웨이포인트. / Waypoint the tracker is steering toward.
    int targetIdx = -1;
    // 차량에 가장 가까운 웨이포인트. / Waypoint nearest the vehicle.
    int closestIdx = -1;
    // 속도/작업기 지시를 따르고 있는 웨이포인트. / Waypoint whose speed/flag is being obeyed.
    int commandIdx = -1;

    // 알고리즘별 자유 항목. / Algorithm-specific entries.
    std::vector<std::pair<std::string, double> > extras;

    /**
     * extras에 항목을 추가합니다. 같은 키를 여러 번 넣지 마세요.
     * Appends an entry to extras. Do not add the same key twice.
     */
    void put(const std::string& key, double value)
    {
        extras.push_back(std::make_pair(key, value));
    }

    /**
     * extras에서 이름으로 값을 읽습니다. 이 트래커가 해당 항목을 발행하지
     * 않으면 fallback을 돌려줍니다.
     *
     * Reads an extra by name, or `fallback` if this tracker does not publish it.
     */
    double get(const std::string& key, double fallback = 0.0) const
    {
        for (std::vector<std::pair<std::string, double> >::const_iterator it =
                extras.begin(); it != extras.end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }
        return fallback;
    }

    /**
     * extras를 비웁니다. 매 주기 put()을 호출하기 전에 반드시 불러야
     * 합니다. 안 그러면 항목이 무한히 쌓입니다.
     *
     * Clears extras. Must be called before the put() calls of each tick,
     * otherwise entries accumulate without bound.
     */
    void clearExtras()
    {
        extras.clear();
    }
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_TRACKERTYPES_HPP
