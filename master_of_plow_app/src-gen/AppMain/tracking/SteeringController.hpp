/*
 * AppMain/tracking/SteeringController.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 조향 하위 루프: 목표 조향각 -> 조향 모터 속도 지령.
 *
 * 트래커는 "앞바퀴를 몇 도 꺾을까"까지만 정합니다. 그 각도를 실제로 만들어내는
 * 일은 KY170 조향 모터를 속도 모드로 돌려서 해야 하고, 그 폐루프가 이 클래스입니다.
 *
 *     목표각 ─┬─→ [PID] ─→ 모터 속도 지령(±600) ─→ Motor_Request
 *     실측각 ─┘
 *
 * ── 왜 앱이 이 루프를 가져야 하는가 ──
 *
 * 내장 UI 시뮬레이터에는 이 서보가 시뮬레이터 안에 있었습니다(KP=5.0). 실제
 * 머신에는 없습니다. Motor_Request 는 "모터 속도"만 받고 각도 setpoint 채널이
 * 없으므로, 각도를 각도로 만드는 책임은 전부 앱에 있습니다.
 *
 * ── 게인의 단위 (가장 사고 나기 쉬운 지점) ──
 *
 * 게인은 오차를 **도(degree)** 로 볼 때의 값입니다(참조 구현 agmo_solution 의
 * motor_kp=50 / motor_kd=5 / motor_ki=0.1 과 같은 스케일). 라디안 오차에 그대로
 * 곱하면 57.3배 작아져서 조향이 거의 듣지 않습니다. 그래서 이 클래스는 입력을
 * 라디안으로 받고 내부에서 도로 변환한 뒤 게인을 적용합니다.
 *
 * ── 실측각을 어디서 얻는가 ──
 *
 * 두 소스를 융합합니다. 성격이 상반되기 때문입니다.
 *
 *   엔코더 (Motor_Response_EncoderCountValue, 단위 deg)
 *       정지 상태에서도 유효하지만 "모터축 각도"이므로 앞바퀴각으로 환산해야
 *       하고(조향비), 직진일 때의 원점을 알아야 합니다.
 *
 *   yaw rate 역산 (Aceinna_AngleRate.GyroZ)
 *       delta ~= -atan(L * yawRate / v). 앞바퀴각을 직접 주므로 환산비가 필요
 *       없지만 v -> 0 에서 발산합니다.
 *
 * 그래서 저속에서는 엔코더를, 주행 중에는 융합 가중치대로 씁니다. 그리고 주행 중
 * yaw rate 기준으로 엔코더 원점(offset)을 아주 느리게 보정합니다. 이렇게 하면
 * 조향비와 원점을 정확히 몰라도 주행하면서 수렴합니다.
 *
 * 주행 중에는 정상 엔코더를 주 피드백으로 쓰되, 두 독립 센서가 지속적으로
 * 부호·크기 면에서 어긋나거나 엔코더만 멈추면 신뢰도를 낮춥니다. 다시 일치하는
 * 상태가 유지되어야 히스테리시스를 거쳐 천천히 복구됩니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The steering inner loop: target wheel angle -> steering motor speed command.
 *
 * A tracker decides how far to turn the front wheels; actually producing that
 * angle means driving the KY170 motor in speed mode, and this class closes that
 * loop. The built-in UI simulator used to close it internally (KP=5.0); a real
 * machine does not, because Motor_Request carries a motor speed and there is no
 * angle-setpoint channel.
 *
 * GAIN UNITS — the easiest thing to get wrong. The gains assume the error is in
 * DEGREES (same scale as agmo_solution's motor_kp=50 / motor_kd=5 / motor_ki=0.1).
 * Applying them to a radian error is 57.3x too small and the vehicle barely
 * steers. This class therefore takes radians and converts internally.
 *
 * MEASURED ANGLE — fused from two complementary sources: the motor encoder
 * (valid at standstill, but reads motor-shaft degrees, so it needs a steering
 * ratio and a centre offset) and yaw-rate inversion delta ~= -atan(L*yawRate/v)
 * (gives the wheel angle directly, no ratio needed, but diverges as v -> 0).
 * The encoder is used at low speed. While driving, a confidence weight favours
 * a plausible encoder, fades it after sustained sign/magnitude disagreement or
 * a stuck signal, and recovers only after sustained agreement. The yaw-rate
 * estimate also slowly trims the encoder centre while the sources agree.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_STEERINGCONTROLLER_HPP
#define APPMAIN_TRACKING_STEERINGCONTROLLER_HPP

#include <AppMain/tracking/TrackerTypes.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace AppMain
{
namespace tracking
{

class SteeringController
{
public:
    struct Output
    {
        // 모터 속도 지령. -1000..+1000 이 정격속도의 천분율이고, 여기서는
        // ±MOTOR_CMD_LIMIT 로 제한합니다. 0 은 "정지"가 아니라 Disable 입니다.
        //
        // Motor speed command in per-mille of rated speed, clamped to
        // ±MOTOR_CMD_LIMIT. Zero means Disable, not "hold".
        int motorCmd = 0;

        // 진단용. 대시보드가 그대로 표시합니다.
        // Diagnostics, published as-is by the dashboard.
        double targetDeg = 0.0;
        double measuredDeg = 0.0;
        double errorDeg = 0.0;
        double pTerm = 0.0;
        double iTerm = 0.0;
        double dTerm = 0.0;
        double encoderWeight = 0.0;
        bool measuredValid = false;
        bool blocked = false;  // 고장으로 조향을 끊었는가 / steering cut by a fault
    };

    SteeringController()
    {
        reset();
    }

    /**
     * 적분·미분 상태와 엔코더 원점 추정을 초기화합니다. 새 경로를 적재할 때
     * 호출하세요.
     *
     * Clears integral/derivative state and the encoder offset estimate. Call when
     * a new path is loaded.
     */
    void reset()
    {
        integral_ = 0.0;
        prevErrorDeg_ = 0.0;
        dFiltered_ = 0.0;
        havePrevError_ = false;
        encoderOffsetDeg_ = 0.0;
        haveOffset_ = false;
        measAgeS_ = 0.0;
        haveMeasurement_ = false;
        haveEncoderSampleId_ = false;
        haveYawSampleId_ = false;
        lastEncoderSampleId_ = -1;
        lastYawSampleId_ = -1;
        encoderConfidence_ = ENCODER_WEIGHT_INITIAL;
        encoderBadEvidenceS_ = 0.0;
        encoderGoodEvidenceS_ = 0.0;
        havePreviousEncoderDeg_ = false;
        havePreviousGyroDeg_ = false;
        heldMotorCmd_ = 0;
    }

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * PID 게인을 런타임에 교체합니다 (튜닝 UI 경로). 오차 단위는 도(degree)
     * 기준입니다 — 파일 상단의 "게인의 단위" 설명 참고. 적분 상태는 유지합니다:
     * 게인만 바꾸는데 정상상태 오프셋까지 버리면 매 조정마다 차가 출렁입니다.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Replaces the PID gains at runtime (tuning-UI path). Gains are on a
     * DEGREE error — see the unit note at the top. The integral state is kept:
     * dropping the steady-state offset on every gain tweak would jolt the
     * vehicle each time.
     * └──────────────────────────────────────────────────────────────────────┘
     */
    void setGains(double kp, double ki, double kd)
    {
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
    }

    void setVehicleGeometry(const VehicleGeometry& geometry)
    {
        if (geometry.valid()) geometry_ = geometry;
    }

    /**
     * @param targetRad   트래커가 낸 목표 앞바퀴 조향각 (양수 = 좌)
     * @param state       이번 주기의 차량 상태 (엔코더·yaw rate·고장 비트 포함)
     * @param encoderDeg  Motor_Response_EncoderCountValue 원값 (모터축 도)
     * @param encoderValid 엔코더 응답이 유효한가
     * @param encoderSampleId encoder interface timestamp/sequence
     * @param yawSampleId yaw-rate interface timestamp/sequence
     * @param dtS         이전 호출로부터 경과 초 (0보다 커야 함)
     */
    Output update(double targetRad, const VehicleState& state,
                  double encoderDeg, bool encoderValid,
                  std::int64_t encoderSampleId, std::int64_t yawSampleId,
                  double dtS)
    {
        Output out;
        out.targetDeg = targetRad * RAD_TO_DEG;

        // ── 고장 인터록 / Fault interlock ────────────────────────────────────
        // 스톨·CAN 단절·과전류 등이 걸린 상태에서 지령을 계속 밀어넣으면 상황이
        // 악화됩니다. 지령을 끊고 적분을 비웁니다.
        // Pushing commands into a stalled or disconnected motor makes things
        // worse. Cut the command and drop the integral.
        if (isSteeringBlockingFault(state.motorFaultBits))
        {
            integral_ = 0.0;
            havePrevError_ = false;
            out.blocked = true;
            out.motorCmd = 0;
            heldMotorCmd_ = 0;
            return out;
        }

        // ── 실측각 융합 / Fuse the measured angle ────────────────────────────
        const bool encoderUsable = encoderValid && encoderSampleId >= 0;
        const bool encoderSampleChanged = encoderUsable &&
                (!haveEncoderSampleId_ || encoderSampleId != lastEncoderSampleId_);
        const bool yawUsable = state.yawRateValid && yawSampleId >= 0 &&
                state.speedMps >= MIN_SPEED_FOR_GYRO_MPS;
        const bool yawSampleChanged = yawUsable &&
                (!haveYawSampleId_ || yawSampleId != lastYawSampleId_);
        if (encoderSampleChanged) {
            lastEncoderSampleId_ = encoderSampleId;
            haveEncoderSampleId_ = true;
        }
        if (yawSampleChanged) {
            lastYawSampleId_ = yawSampleId;
            haveYawSampleId_ = true;
        }

        if (dtS > 0.0)
        {
            measAgeS_ += dtS;
        }
        const double sampleDtS = (encoderSampleChanged || yawSampleChanged)
                ? measAgeS_ : 0.0;

        const double measRad = fuseMeasuredAngle(state, encoderDeg, encoderUsable,
                encoderSampleChanged, yawSampleChanged, sampleDtS);
        out.encoderWeight = encoderUsable && yawUsable ? encoderConfidence_
                                                       : (encoderUsable ? 1.0 : 0.0);
        out.measuredValid = encoderUsable || yawUsable;
        if (!out.measuredValid)
        {
            // 실측이 없으면 폐루프를 돌리지 않습니다. 0을 실측으로 착각하면
            // 직진이라고 믿고 최대 조향을 걸어버립니다.
            // Without a measurement there is no closed loop. Treating 0 as the
            // reading would command full lock while believing it is straight.
            integral_ = 0.0;
            havePrevError_ = false;
            out.motorCmd = 0;
            heldMotorCmd_ = 0;
            return out;
        }
        out.measuredDeg = measRad * RAD_TO_DEG;

        // PID time advances only when an authoritative interface timestamp or
        // sequence advances. Value deltas cannot identify samples: a stationary
        // wheel legitimately repeats its value, while fusion/trim can change a
        // held value slightly. The yaw-only fallback follows the same rule.
        const bool newSample = !haveMeasurement_ || encoderSampleChanged ||
                               yawSampleChanged;
        if (!newSample)
        {
            out.errorDeg = out.targetDeg - out.measuredDeg;
            out.pTerm = kp_ * out.errorDeg;
            out.iTerm = ki_ * integral_;
            out.dTerm = kd_ * dFiltered_;
            out.motorCmd = heldMotorCmd_;
            return out;
        }
        const double measDtS = measAgeS_;
        measAgeS_ = 0.0;
        haveMeasurement_ = true;

        // ── PID ─────────────────────────────────────────────────────────────
        const double errorDeg = out.targetDeg - out.measuredDeg;
        out.errorDeg = errorDeg;

        if (std::fabs(errorDeg) <= DEADBAND_DEG)
        {
            // 불감대: 진동을 막습니다. 적분은 유지해 정상상태 오프셋을 잃지
            // 않습니다. / Deadband prevents dither; the integral is kept so the
            // steady-state offset is not lost.
            out.motorCmd = 0;
            out.pTerm = 0.0;
            out.iTerm = ki_ * integral_;
            heldMotorCmd_ = 0;
            return out;
        }

        out.pTerm = kp_ * errorDeg;

        // 미분항은 저역통과를 걸어 씁니다. 100ms 주기에서 생미분은 노이즈를
        // 그대로 증폭합니다. / The derivative is low-pass filtered; at a 100 ms
        // tick a raw difference just amplifies noise.
        if (havePrevError_ && measDtS > 0.0)
        {
            const double raw = (errorDeg - prevErrorDeg_) / measDtS;
            const double filterTau = std::max(0.01, geometry_.steeringWheelTauS);
            const double alpha = measDtS / (filterTau + measDtS);
            dFiltered_ += alpha * (raw - dFiltered_);
        }
        prevErrorDeg_ = errorDeg;
        havePrevError_ = true;
        out.dTerm = kd_ * dFiltered_;

        // 적분은 먼저 시험 적분을 해보고, 출력이 포화하면 되돌립니다
        // (conditional integration 방식의 anti-windup).
        // Trial-integrate, then undo it if the output saturates — conditional
        // integration anti-windup.
        const double integralTrial = integral_ + errorDeg * measDtS;
        double rawCmd = out.pTerm + ki_ * integralTrial + out.dTerm;
        if (std::fabs(rawCmd) <= MOTOR_CMD_LIMIT)
        {
            integral_ = integralTrial;
        }
        else
        {
            rawCmd = out.pTerm + ki_ * integral_ + out.dTerm;
        }
        out.iTerm = ki_ * integral_;

        // ── 출력 정형 / Shape the output ────────────────────────────────────
        double cmd = std::clamp(rawCmd, -MOTOR_CMD_LIMIT, MOTOR_CMD_LIMIT);

        // 너무 작은 지령은 유압을 못 이기고 모터만 떨게 만듭니다. 최소 지령
        // 아래면 0(Disable)으로 떨어뜨립니다.
        // A command too small to overcome the hydraulics only buzzes the motor;
        // below the floor it collapses to 0 (Disable).
        if (std::fabs(cmd) < MOTOR_CMD_MIN)
        {
            cmd = 0.0;
        }

        out.motorCmd = static_cast<int>(cmd);
        heldMotorCmd_ = out.motorCmd;
        return out;
    }

    // Compatibility for isolated tracker examples that do not model transport
    // metadata. Every call is explicitly treated as a new sample; production
    // uses the timestamp-bearing overload above.
    Output update(double targetRad, const VehicleState& state,
                  double encoderDeg, bool encoderValid, double dtS)
    {
        ++compatibilitySampleId_;
        return update(targetRad, state, encoderDeg, encoderValid,
                      compatibilitySampleId_, compatibilitySampleId_, dtS);
    }

private:
    /**
     * 엔코더와 yaw rate 역산을 섞어 앞바퀴각(rad)을 추정합니다.
     * Fuses the encoder and the yaw-rate inversion into a wheel angle in radians.
     */
    double fuseMeasuredAngle(const VehicleState& state, double encoderDeg,
                             bool encoderValid, bool encoderSampleChanged,
                             bool yawSampleChanged, double dtS)
    {
        // This repository uses a left-handed tracking frame:
        // yawRate = -(v/L) * tan(delta), therefore
        // delta = -atan(L * yawRate / v).
        // 저속에서 발산하므로 최소 속도 위에서만 씁니다.
        // Yaw-rate inversion, only above a minimum speed because it diverges.
        bool gyroUsable = false;
        double gyroRad = 0.0;
        if (state.yawRateValid && state.speedMps >= MIN_SPEED_FOR_GYRO_MPS)
        {
            gyroRad = -std::atan(geometry_.wheelbaseM * state.yawRateRadS / state.speedMps);
            gyroUsable = true;
        }

        // 엔코더: 모터축 도 -> 앞바퀴 라디안. 원점 오프셋을 뺍니다.
        // Encoder: motor-shaft degrees -> front-wheel radians, minus the centre.
        double encRad = 0.0;
        if (encoderValid)
        {
            encRad = (encoderDeg - encoderOffsetDeg_) / geometry_.steeringRatio * DEG_TO_RAD;
        }

        if (encoderValid && gyroUsable)
        {
            const double encDeg = encRad * RAD_TO_DEG;
            const double gyroDeg = gyroRad * RAD_TO_DEG;
            if (encoderSampleChanged || yawSampleChanged)
            {
                const bool oppositeSign = std::fabs(encDeg) >= SIGN_CHECK_MIN_DEG &&
                        std::fabs(gyroDeg) >= SIGN_CHECK_MIN_DEG &&
                        ((encDeg > 0.0) != (gyroDeg > 0.0));
                const bool implausibleResidual =
                        std::fabs(encDeg - gyroDeg) >= DISAGREEMENT_DEG;
                bool stuck = false;
                if (encoderSampleChanged)
                {
                    stuck = havePreviousEncoderDeg_ && havePreviousGyroDeg_ &&
                            std::fabs(encoderDeg - previousEncoderDeg_) <=
                                    STUCK_ENCODER_MOTOR_DEG &&
                            std::fabs(gyroDeg - previousGyroDeg_) >=
                                    STUCK_GYRO_DELTA_DEG;
                    previousEncoderDeg_ = encoderDeg;
                    havePreviousEncoderDeg_ = true;
                }
                if (yawSampleChanged)
                {
                    previousGyroDeg_ = gyroDeg;
                    havePreviousGyroDeg_ = true;
                }

                const bool bad = oppositeSign || implausibleResidual || stuck;
                if (bad)
                {
                    encoderBadEvidenceS_ += std::max(0.0, dtS);
                    encoderGoodEvidenceS_ = 0.0;
                    if (encoderBadEvidenceS_ >= BAD_CONFIRM_S)
                    {
                        encoderConfidence_ = std::max(ENCODER_WEIGHT_MIN,
                                encoderConfidence_ - ENCODER_FADE_PER_S *
                                std::max(0.0, dtS));
                    }
                }
                else
                {
                    encoderGoodEvidenceS_ += std::max(0.0, dtS);
                    encoderBadEvidenceS_ = std::max(0.0,
                            encoderBadEvidenceS_ - std::max(0.0, dtS));
                    if (encoderGoodEvidenceS_ >= GOOD_RECOVER_S)
                    {
                        encoderConfidence_ = std::min(ENCODER_WEIGHT_MAX,
                                encoderConfidence_ + ENCODER_RECOVER_PER_S *
                                std::max(0.0, dtS));
                    }
                }
            }

            // 원점 보정: yaw rate 추정을 기준선으로 삼아 엔코더 오프셋을 아주
            // 느리게 끌어당깁니다. 게인이 작아야 순간적인 슬립이 원점을 흔들지
            // 않습니다.
            // Trim the encoder centre toward the yaw-rate estimate, slowly enough
            // that a moment of wheel slip cannot move it.
            const double residualDeg = (encRad - gyroRad) * RAD_TO_DEG
                                     * geometry_.steeringRatio;
            if (std::fabs(encDeg - gyroDeg) < DISAGREEMENT_DEG &&
                    encoderConfidence_ >= OFFSET_TRIM_MIN_WEIGHT)
            {
                encoderOffsetDeg_ += OFFSET_TRIM_GAIN * residualDeg * dtS;
            }
            // 원점 보정은 반드시 묶어 둡니다. 스케일이나 부호가 어긋난 신호를
            // 오프셋으로 지우려 들면 적분기가 영영 수렴하지 못하고 표류합니다.
            // 실측: 169초 주행에서 6.6도까지 밀렸고 계속 상승 중이었습니다.
            // The trim must be bounded. Asked to cancel a scale or sign error with
            // an offset, the integrator never converges and simply drifts —
            // measured at 6.6 deg and still climbing over a 169 s run.
            encoderOffsetDeg_ = std::clamp(encoderOffsetDeg_,
                                           -OFFSET_TRIM_LIMIT_DEG,
                                            OFFSET_TRIM_LIMIT_DEG);
            haveOffset_ = true;

            const double w = encoderConfidence_;
            return w * encRad + (1.0 - w) * gyroRad;
        }
        if (encoderValid)
        {
            haveOffset_ = true;
            return encRad;
        }
        if (gyroUsable)
        {
            return gyroRad;
        }
        return 0.0;
    }

    static constexpr double RAD_TO_DEG = 57.29577951308232;
    static constexpr double DEG_TO_RAD = 1.0 / RAD_TO_DEG;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 게인 기본값은 참조 구현(agmo_solution 의 ParameterSetting 기본값)과 같고,
    // 오차 단위가 도(degree)라는 전제입니다. 튜닝 UI가 setGains()로 런타임에
    // 바꿀 수 있도록 constexpr 에서 일반 멤버가 됐습니다 (TrackerParams 참고).
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Gain defaults match the reference implementation, on a degree error. They
    // became plain members (formerly constexpr) so the tuning UI can change
    // them at runtime via setGains(); see TrackerParams.
    // └────────────────────────────────────────────────────────────────────────┘
    double kp_ = 50.0;
    double ki_ = 0.1;
    double kd_ = 5.0;
    VehicleGeometry geometry_;

    // 출력 한계. 매뉴얼상 ±1000 이 정격속도지만, 참조 구현은 600(=60%)에서
    // 잘라 씁니다. 유압 조향에서 그 이상은 오버슈트를 만듭니다.
    // Output limit. ±1000 is rated speed per the manual, but the reference caps at
    // 600 (60%); more than that overshoots on hydrostatic steering.
    static constexpr double MOTOR_CMD_LIMIT = 600.0;
    // 이 아래 지령은 유압을 못 이깁니다. / Below this the hydraulics do not move.
    static constexpr double MOTOR_CMD_MIN = 30.0;

    static constexpr double DEADBAND_DEG = 0.3;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 모터축(핸들) 각도 -> 앞바퀴 각도 환산비.
    //
    // 원점 보정(encoderOffsetDeg_)은 "중심이 어긋난 것"만 흡수합니다. 비율
    // 자체가 틀리면 큰 조향에서 어긋나고, 원점 보정은 그 스케일 오차를 표현할
    // 수단이 없어 영원히 따라다니게 됩니다.
    //
    // 20.0 은 추정치였고 실측 결과 약 21% 낮았습니다. 재는 방법과 값:
    // 조향을 양쪽 끝까지 물린 상태에서 GPS 방위 변화율로 진짜 앞바퀴각을
    // 역산합니다 (delta = -atan(WHEELBASE_M * dPsi/dt / v); 이 좌표계는
    // 왼손계라 마이너스가 붙습니다). 엔드스톱 두 점이면 스케일과 중심이 같이
    // 풀립니다.
    //
    //   우측 엔드스톱: 보고값 -19.5°, 실제 -25.6°
    //   좌측 엔드스톱: 보고값 +22.6°, 실제 +25.3°
    //   실제 = s * (보고값 - c)  ->  s = 50.9 / 42.1 = 1.209,  c = +1.7°
    //   -> STEERING_RATIO = 20.0 * 1.209 = 24.2
    //
    // 전체 주행 1,060샘플 회귀로도 s = 1.17 (비율 23.4)로 같은 방향입니다.
    // 남는 c = 1.7° 는 중심 오차이므로 원점 보정이 흡수합니다.
    //
    // 다른 기계에서는 반드시 다시 재세요.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Motor-shaft to front-wheel ratio.
    //
    // The offset trim (encoderOffsetDeg_) absorbs a wrong CENTRE. A wrong RATIO
    // skews large angles instead, and the trim has no way to represent a scale
    // error, so it never goes away.
    //
    // 20.0 was a guess and measured ~21% low. To measure it: hold the steering
    // against each end stop and back-solve the true wheel angle from the GPS
    // heading rate (delta = -atan(WHEELBASE_M * dPsi/dt / v); the minus sign is
    // because this frame is left-handed). Two end stops give both scale and
    // centre:
    //
    //   right stop: reported -19.5 deg, true -25.6 deg
    //   left  stop: reported +22.6 deg, true +25.3 deg
    //   true = s * (reported - c)  ->  s = 50.9 / 42.1 = 1.209, c = +1.7 deg
    //   -> STEERING_RATIO = 20.0 * 1.209 = 24.2
    //
    // A regression over all 1,060 moving samples of the run agrees at s = 1.17
    // (ratio 23.4). The leftover c = 1.7 deg is a centre error, which is exactly
    // what the offset trim is for.
    //
    // Re-measure this for any other machine.
    // └────────────────────────────────────────────────────────────────────────┘
    // 이 속도 아래에서는 yaw rate 역산을 쓰지 않습니다.
    // Below this speed the yaw-rate inversion is unusable.
    static constexpr double MIN_SPEED_FOR_GYRO_MPS = 0.5;

    // Confidence is bounded, evidence must persist before degradation, and
    // recovery is deliberately slower than fade. This handles generic sensor
    // disagreement without a machine- or simulator-specific branch. At low
    // speed yaw inversion is unavailable, so a fresh encoder remains primary.
    static constexpr double ENCODER_WEIGHT_INITIAL = 0.7;
    static constexpr double ENCODER_WEIGHT_MIN = 0.05;
    static constexpr double ENCODER_WEIGHT_MAX = 0.85;
    static constexpr double DISAGREEMENT_DEG = 6.0;
    static constexpr double SIGN_CHECK_MIN_DEG = 2.0;
    static constexpr double STUCK_GYRO_DELTA_DEG = 1.0;
    static constexpr double STUCK_ENCODER_MOTOR_DEG = 0.25;
    static constexpr double BAD_CONFIRM_S = 0.5;
    static constexpr double GOOD_RECOVER_S = 1.5;
    static constexpr double ENCODER_FADE_PER_S = 0.9;
    static constexpr double ENCODER_RECOVER_PER_S = 0.12;
    static constexpr double OFFSET_TRIM_MIN_WEIGHT = 0.4;

    // 원점 보정 적분기의 한계(모터축 도). 약 2도(앞바퀴) 상당입니다.
    // Bound on the offset trim, in motor-shaft degrees — about 2 deg of wheel.
    static constexpr double OFFSET_TRIM_LIMIT_DEG = 48.0;

    // 원점 보정 게인. 작을수록 느리고 안정적입니다. (1/s)
    // Offset trim gain; smaller is slower and steadier. Units: 1/s.
    static constexpr double OFFSET_TRIM_GAIN = 0.02;

    double integral_;
    double prevErrorDeg_;
    double dFiltered_;
    bool havePrevError_;
    // held 엔코더 값을 중복 소비하지 않기 위한 영차 홀드 상태
    // Zero-order-hold state that prevents re-consuming held encoder values
    double measAgeS_;
    bool haveMeasurement_;
    std::int64_t lastEncoderSampleId_ = -1;
    std::int64_t lastYawSampleId_ = -1;
    bool haveEncoderSampleId_;
    bool haveYawSampleId_;
    double encoderConfidence_;
    double encoderBadEvidenceS_;
    double encoderGoodEvidenceS_;
    double previousEncoderDeg_ = 0.0;
    bool havePreviousEncoderDeg_;
    double previousGyroDeg_ = 0.0;
    bool havePreviousGyroDeg_;
    std::int64_t compatibilitySampleId_ = 0;
    int heldMotorCmd_;
    // 엔코더가 직진을 가리키는 값(모터축 도). 주행 중 추정합니다.
    // Encoder reading that corresponds to straight ahead, in motor-shaft degrees.
    double encoderOffsetDeg_;
    bool haveOffset_;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_STEERINGCONTROLLER_HPP
