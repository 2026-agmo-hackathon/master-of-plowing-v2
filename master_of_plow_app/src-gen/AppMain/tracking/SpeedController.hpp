/*
 * AppMain/tracking/SpeedController.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 종방향(앞뒤) 제어: 부호 있는 웨이포인트 목표속도 -> 기어 + 스로틀 + 브레이크.
 *
 * PurePursuitTracker에서 분리해 낸 이유는 간단합니다. 조향 법칙(Stanley,
 * Pure Pursuit, MPC 등)은 알고리즘마다 다르지만 종방향 제어는 그렇지 않습니다.
 * 실제로 교과서적인 Stanley 제어기는 조향각만 정의하고 스로틀에 대해서는
 * 아무 말도 하지 않습니다.
 *
 * 그래서 트래커는 이 클래스를 멤버로 하나 들고, "어느 웨이포인트의 속도를
 * 따를 것인가"만 결정하면 됩니다.
 *
 * ── 이 기계에는 브레이크 채널이 없습니다 ──
 *
 * 여기서 내보내는 brake는 유압 브레이크 지령이 아닙니다. SeamOS 트랙터 인터페이스
 * 의 Out 신호는 조향 모터 / 가속 개도(ACC) / 전후진(FNR) / 변속(SFT) / 히치(HYD)
 * 다섯 개뿐이고 제동 채널이 없습니다. 그래서 액추에이터 계층이 brake를 이렇게
 * 해석합니다.
 *
 *     brake > 0  ->  ACC 개도 0 (엔진 브레이크) + 필요하면 다운시프트
 *
 * 즉 제동 권한이 실제로는 훨씬 약하고 느립니다. 감속이 필요한 구간은 미리
 * 목표속도를 낮추는 편이 확실하고, 여기 게인을 올려서 해결할 수는 없습니다.
 * brake 필드를 그대로 남겨둔 이유는 (1) 트래커 계약을 바꾸지 않기 위해서,
 * (2) 내장 시뮬레이터/단위테스트의 차량 모델이 이 값을 쓰기 때문입니다.
 *
 * ── PID ──
 *
 * 원래 P 제어뿐이었습니다. 경사와 견인부하(작업기가 흙을 물었을 때)에서
 * 정상상태 오차가 남기 때문에 I 항을 넣었고, 질량 지연에서의 오버슈트를 줄이려
 * D 항을 저역통과와 함께 넣었습니다. 그래서 update()가 dt를 받습니다.
 * 제어 주기는 TrackingLoop이 결정하므로 고정값을 가정할 수 없습니다.
 *
 * ── 여기서 까다로운 부분: 전진/후진 전환 ──
 *
 * GPS는 속력의 크기만 알려주므로 지금 전진 중인지 후진 중인지 측정값만으로는
 * 알 수 없습니다. 그래서 상태 기계로 처리합니다. 지시 속도의 부호가 바뀌면
 * "완전 제동" 상태로 latch하고, 차량이 실제로 멈춘 뒤에야 빠져나옵니다.
 * 이렇게 하지 않으면 앞으로 굴러가는 중에 변속기에 후진을 요구하게 됩니다.
 *
 * 브레이크가 없는 실기에서는 이 정지 대기가 코스팅으로 이뤄지므로 시뮬레이터
 * 보다 오래 걸립니다. 로직은 그대로 유효합니다 — 실제로 멈췄는지를 속력으로
 * 판단하기 때문입니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Longitudinal control: signed waypoint speed -> gear + throttle + brake.
 *
 * Split out of PurePursuitTracker because steering laws (Stanley, PP, MPC, ...)
 * differ while longitudinal control does not — a textbook Stanley controller
 * defines a steering angle and says nothing about throttle. A tracker holds one
 * of these and only has to decide *which* waypoint's speed to obey.
 *
 * THERE IS NO BRAKE CHANNEL ON THIS MACHINE. The SeamOS tractor interface exposes
 * exactly five outputs — steering motor, accelerator opening (ACC), forward/
 * neutral/reverse (FNR), gear stage (SFT) and hitch (HYD) — and none of them is a
 * brake. The actuator layer therefore reads a non-zero brake as "ACC to 0 (engine
 * braking), downshift if needed". Braking authority is consequently much weaker
 * and slower than a real brake, and raising the gain here cannot fix that: slow
 * down by lowering the target speed ahead of time instead. The field is kept
 * because the tracker contract and the unit-test vehicle model both use it.
 *
 * PID: this was P-only. An I term was added because slope and draft load (the
 * implement biting into soil) leave a steady-state error, and a low-pass filtered
 * D term to damp overshoot from drivetrain lag — which is why update() now takes
 * dt. The control period is chosen by TrackingLoop, so it cannot be assumed.
 *
 * FORWARD/REVERSE TRANSITIONS: GPS reports an unsigned speed magnitude, so a
 * direction change cannot be detected from the measurement. When the commanded
 * speed changes sign the controller latches into a full-brake state and stays
 * there until the vehicle has actually stopped — otherwise the transmission would
 * be asked to reverse while still rolling forward. On the real machine that wait
 * happens by coasting and takes longer; the logic still holds because it keys off
 * the measured speed.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_SPEEDCONTROLLER_HPP
#define APPMAIN_TRACKING_SPEEDCONTROLLER_HPP

#include <cstdint>
#include <AppMain/tracking/TrackerTypes.hpp>
#include <algorithm>
#include <cmath>

namespace AppMain
{
namespace tracking
{

class SpeedController
{
public:
    struct Output
    {
        double throttle = 0.0;  // 0..1
        double brake = 0.0;     // 0..1 — 실기에서는 "개도 0 + 다운시프트" / on the machine: ACC 0 + downshift
        int gear = 1;           // -1 후진, 1..3 전진(Low/Middle/High) / -1 reverse, 1..3 forward

        // 진단용. 대시보드가 그대로 표시합니다. / Diagnostics for the dashboard.
        double targetMps = 0.0;   // 실제로 추종한 목표 / the target actually followed
        double errorMps = 0.0;
        double ffTerm = 0.0;   // 앞먹임 개도 / feedforward opening
        // 경로가 요구한 원래 목표. targetMps 와 다르면 하한으로 올린 것입니다.
        // What the path asked for. Differing from targetMps means it was raised.
        double requestedMps = 0.0;
        bool targetRaisedToFloor = false;
        double pTerm = 0.0;
        double iTerm = 0.0;
        double dTerm = 0.0;
        bool brakingForReversal = false;
    };

    SpeedController()
    {
        reset();
    }

    /**
     * 전진/후진 전환 상태 기계와 PID 상태를 초기화합니다. 새 경로를 적재할 때
     * 호출하세요.
     * Clears the reversal state machine and the PID state. Call when a new path is
     * loaded.
     */
    void reset()
    {
        prevTargetSign_ = 0;
        brakingForReversal_ = false;
        integral_ = 0.0;
        prevError_ = 0.0;
        dFiltered_ = 0.0;
        havePrevError_ = false;
        haveMeasurement_ = false;
        lastMeasuredSpeed_ = 0.0;
        measAgeS_ = 0.0;
        heldThrottle_ = 0.0;
        heldBrake_ = 0.0;
        curGear_ = 1;
        pendingGear_ = 1;
        gearDwellS_ = 0.0;
        haveGear_ = false;
    }

    /**
     * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
     * PID 게인을 런타임에 교체합니다 (튜닝 UI 경로). 적분 상태는 유지합니다 —
     * 게인 조정 때마다 경사에서 잡아 둔 오프셋을 버리면 차가 흘러내립니다.
     * └──────────────────────────────────────────────────────────────────────┘
     * ┌─ English ────────────────────────────────────────────────────────────┐
     * Replaces the PID gains at runtime (tuning-UI path). The integral state
     * is kept — dropping the slope offset on every tweak would let the
     * vehicle sag.
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
        if (geometry.valid()) maxForwardGear_ = std::clamp(geometry.maxForwardGear, 1, 3);
    }

    /**
     * @param targetSpeedKmh  현재 따르는 웨이포인트의 목표속도(부호 있음)
     *                        signed target speed of the active waypoint
     * @param currentSpeedMps 측정된 속력의 크기
     *                        measured speed magnitude
     * @param dtS             이전 호출로부터 경과 초. 0 이하면 I/D를 건너뜁니다.
     *                        seconds since the previous call; I/D are skipped if <= 0
     */
    Output update(double targetSpeedKmh, double currentSpeedMps, double dtS,
                  std::int64_t sampleId = -1)
    {
        Output out;

        out.requestedMps = std::fabs(targetSpeedKmh) * KMH_TO_MS;
        const double followedKmh = raiseToMachineFloor(targetSpeedKmh);
        out.targetRaisedToFloor = (followedKmh != targetSpeedKmh);

        const double target_abs_ms = std::fabs(followedKmh) * KMH_TO_MS;
        const double current_abs_ms = std::fabs(currentSpeedMps);
        const double speedErr = target_abs_ms - current_abs_ms;  // m/s

        // 기어도 실제로 따를 속도로 골라야 합니다. 원래 목표로 고르면 낼 수 없는
        // 속도에 맞춘 단으로 들어갑니다.
        // The gear follows the speed we will actually hold; picking it from the
        // original target would select a ratio for a speed the machine cannot make.
        out.gear = gearWithHysteresis(followedKmh, dtS);

        out.targetMps = target_abs_ms;
        out.errorMps = speedErr;

        // 목표 부호를 갱신하고, 전진<->후진 전환이 일어났는지 감지합니다.
        // Update target sign and detect F/R reversal entry.
        const int curTargetSign = (targetSpeedKmh > 0.01) ? 1
                                : (targetSpeedKmh < -0.01 ? -1 : 0);
        if (curTargetSign != 0) {
            if (prevTargetSign_ != 0 && curTargetSign != prevTargetSign_) {
                // 방금 F/R 경계를 넘었습니다. / F/R boundary just crossed.
                brakingForReversal_ = true;
            }
            // 0이 아닐 때만 갱신 (정지 지시로 방향 기억을 잃지 않도록).
            // Only update on non-zero, so a stop command does not erase the direction.
            prevTargetSign_ = curTargetSign;
        }
        // 차량이 실제로 멈추면 제동 상태에서 빠져나옵니다.
        // Exit braking once the vehicle has stopped.
        if (brakingForReversal_ && std::fabs(currentSpeedMps) <= STOP_THR_MS) {
            brakingForReversal_ = false;
        }
        out.brakingForReversal = brakingForReversal_;

        if (brakingForReversal_) {
            // 방향 전환 대기: 스로틀을 끊고 완전 제동. 적분은 비웁니다 —
            // 정지를 기다리는 동안 쌓인 오차는 전환 후 아무 의미가 없습니다.
            // Waiting to reverse: cut throttle, brake hard. The integral is
            // cleared because error accumulated while stopping means nothing
            // after the direction flips.
            out.throttle = 0.0;
            out.brake = 1.0;
            integral_ = 0.0;
            havePrevError_ = false;
            heldThrottle_ = out.throttle;
            heldBrake_ = out.brake;
            return out;
        }

        /*
         * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
         * 수정 전 mux 경로에서는 GPS 속도가 약 1Hz로 들어왔습니다. 예전에는
         * 같은 측정값 하나를 PID가 10번 소비하면서 매 틱
         * 적분했기 때문에 실효 적분 게인이 의도의 10배가 됐고, 미분은 9틱 동안
         * 0이다가 새 샘플에서 한 번에 튀었습니다. 에뮬레이터 4km/h 주행에서
         * 차량이 정지 상태인 틱이 49.9%였고, 그중 목표가 0.3m/s를 넘는데도 멈춰
         * 있던 경우가 전체의 44.8%였습니다(경로가 정지를 요구한 틱은 1.7%뿐).
         * 남은 "간헐적 멈춤"의 정체가 이 리밋 사이클이었습니다.
         *
         * 이제 PID는 측정값이 실제로 갱신됐을 때만 한 스텝 전진하고, dt로는 그
         * 사이 실제로 흐른 시간을 씁니다. 샘플 사이에는 마지막 지령을 그대로
         * 유지합니다(영차 홀드) — 새 정보가 없으면 지령도 바뀌지 않는 것이
         * 샘플 데이터 제어의 정상 동작입니다. MAX_MEAS_HOLD_S 는 속도가 정말로
         * 일정해서 측정값이 연속으로 같게 나오는 경우에도 적분이 멈춰 있지
         * 않도록 하는 안전장치입니다.
         * └────────────────────────────────────────────────────────────────────────┘
         * ┌─ English ────────────────────────────────────────────────────────────┐
         * Before the mux cadence fix, GPS speed arrived at roughly 1 Hz. The PID
         * used to consume the same sample ten
         * times, integrating on every tick, which made the effective integral
         * gain 10x the intended one and left the derivative at zero for nine
         * ticks before spiking on the tenth. In the emulator's 4 km/h run the
         * vehicle read stopped on 49.9% of ticks, and on 44.8% of all ticks it
         * was stopped while the target exceeded 0.3 m/s (only 1.7% of ticks had
         * a commanded stop). That limit cycle was the remaining "intermittent
         * stopping".
         *
         * The PID now advances one step per genuine measurement update and uses
         * the real elapsed interval as dt. Between samples the last command is
         * held (zero-order hold): with no new information the command should not
         * change. MAX_MEAS_HOLD_S keeps the integral alive when the speed is
         * genuinely steady and consecutive samples read identical.
         * └────────────────────────────────────────────────────────────────────────┘
         */
        if (dtS > 0.0) {
            measAgeS_ += dtS;
        }
        const bool identifiedSample = sampleId >= 0;
        const bool newSample = !haveMeasurement_ ||
                (identifiedSample ? (!haveSampleId_ || sampleId != lastSampleId_)
                                  : currentSpeedMps != lastMeasuredSpeed_) ||
                measAgeS_ >= MAX_MEAS_HOLD_S;
        if (!newSample) {
            out.pTerm = kp_ * speedErr;
            out.iTerm = ki_ * integral_;
            out.dTerm = kd_ * dFiltered_;
            out.throttle = heldThrottle_;
            out.brake = heldBrake_;
            return out;
        }
        const double measDtS = measAgeS_;
        measAgeS_ = 0.0;
        lastMeasuredSpeed_ = currentSpeedMps;
        haveMeasurement_ = true;
        if (identifiedSample) { lastSampleId_ = sampleId; haveSampleId_ = true; }

        // ── 미분항 (저역통과) / Derivative, low-pass filtered ────────────────
        if (havePrevError_ && measDtS > 0.0) {
            const double raw = (speedErr - prevError_) / measDtS;
            const double alpha = measDtS / (D_TAU_S + measDtS);
            dFiltered_ += alpha * (raw - dFiltered_);
        }
        prevError_ = speedErr;
        havePrevError_ = true;

        out.pTerm = kp_ * speedErr;
        out.dTerm = kd_ * dFiltered_;

        /*
         * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
         * 예전 코드는 불감대 안에서도, 초과속 쪽 분기에서도 throttle을 강제로
         * 0으로 만들었습니다. 그러면 정상상태에서 열려 있는 개도가 아예 없어져
         * 스로틀이 100%(가속) 아니면 0%(브레이크) 사이만 오가는 bang-bang
         * 릴레이가 되고, TrackingLoop::act()가 brake>0을 accelPct=0으로 매핑하니
         * 가속<->정지를 반복하는 리밋 사이클이 됩니다 — longKp/longKi/longKd를
         * 바꿔도 분기 자체가 출력을 0으로 밀어버리므로 없어지지 않습니다.
         *
         * 이제는 단일 연속 PID 출력 하나로 처리합니다. 불감대는 적분만
         * 얼립니다(freeze) — 노이즈를 적분하지 않게 막는 용도이고, 출력 자체는
         * 항상 연속이라 미세 진동(dither) 걱정은 없습니다. 안티와인드업은 대칭
         * 조건부 적분으로 바뀌어 포화되지 않은 출력이 [0,1] 안일 때만 시험
         * 적분을 채택하고, 더 이상 0에서 바닥을 두지 않습니다(내리막에서는 약간
         * 음수인 적분이 맞는 값입니다). brake는 이제 -speedErr가 아니라 음의
         * PID 출력에서 나오므로, 진짜 감속 요구에만 걸리고 0.03 m/s 흔들림에는
         * 걸리지 않습니다.
         * └────────────────────────────────────────────────────────────────────────┘
         * ┌─ English ────────────────────────────────────────────────────────────┐
         * The old code forced throttle to 0 both inside the deadband and on the
         * overspeed branch, leaving no steady-state opening at all: throttle
         * only ever sat at 100% (accelerate) or 0% (brake), and since
         * TrackingLoop::act() maps any brake > 0 to accelPct = 0, that is a
         * bang-bang relay — a permanent accelerate/stop limit cycle that no
         * amount of longKp/longKi/longKd tuning removes, because the branch
         * itself zeroes the output.
         *
         * A single continuous PID output replaces it. The deadband now only
         * freezes the integral, to guard against integrating noise; the output
         * stays continuous, so dither is not a risk. Anti-windup is symmetric
         * conditional integration — the trial integral is accepted only when
         * the unsaturated output lands in [0,1] — and the integral is no
         * longer floored at 0 (a slightly negative integral is correct on a
         * downslope). brake is now derived from the negative PID output rather
         * than -speedErr directly, so it engages only for a genuine
         * deceleration demand, not a 0.03 m/s twitch.
         * └────────────────────────────────────────────────────────────────────────┘
         */
        /*
         * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
         * 앞먹임(feedforward). 이게 없으면 PID가 정상상태 개도를 적분만으로
         * 만들어내야 합니다.
         *
         * 에뮬레이터 로그에서 속도를 유지하는 데 실제로 필요한 개도를 재보면:
         *
         *     0.57 m/s (2.05 km/h)  ->  0.448      n=6722
         *     1.08 m/s (3.89 km/h)  ->  0.515      n=8818
         *
         * 즉 어떤 속도든 0.45~0.52 는 있어야 굴러갑니다 — 쟁기 견인 부하와
         * 구름 저항이고, 속도에 거의 무관한 큰 상수항입니다. 그런데 오차가
         * 목표 근처에서는 0.03 m/s 수준이라 적분은 ki*e = 0.05*0.03 =
         * 0.0015/s 로 자랍니다. 0.5 를 쌓는 데 5분이 넘게 걸립니다.
         *
         * 그동안 무슨 일이 일어나느냐: 목표에 근접하면 P항이 작아져 개도가
         * 0.02 까지 떨어지고, 그 개도로는 부하를 못 이겨 4초 만에 속도가 0이
         * 되고, 그러면 오차가 커져 개도가 1.0 이 되고, 다시 튀어 올라갔다
         * 떨어지고 — 4초 주기의 "갔다 섰다"입니다. 게인을 아무리 만져도
         * 안 없어집니다. 적분이 다 자라기 전까지는 구조적으로 그렇습니다.
         *
         * 위 두 점을 직선으로 맞추면 ff = 0.40 + 0.10 * v 입니다
         * (0.57 -> 0.457, 1.08 -> 0.508). PID 는 이 값 주위를 다듬기만 하면
         * 되므로 적분이 0에서 출발해도 처음부터 제대로 굴러갑니다.
         *
         * 목표가 0이면 앞먹임도 0입니다. "정지"에 개도를 넣으면 안 됩니다.
         * └────────────────────────────────────────────────────────────────────────┘
         * ┌─ English ────────────────────────────────────────────────────────────┐
         * Feedforward. Without it the PID has to manufacture the entire
         * steady-state opening out of its integral.
         *
         * Measured from the emulator logs, the opening actually needed to hold a
         * speed is:
         *
         *     0.57 m/s (2.05 km/h)  ->  0.448      n=6722
         *     1.08 m/s (3.89 km/h)  ->  0.515      n=8818
         *
         * So 0.45-0.52 is required at any speed — plough draft plus rolling
         * resistance, a large and nearly speed-independent constant. But near the
         * target the error is only ~0.03 m/s, so the integral grows at
         * ki*e = 0.05 * 0.03 = 0.0015/s. Building 0.5 that way takes over five
         * minutes.
         *
         * What happens meanwhile: as the speed approaches target the P term
         * shrinks, the opening falls to ~0.02, that opening cannot hold the load,
         * the machine is at a standstill within four seconds, the error jumps, the
         * opening slams to 1.0, and it surges back — a four-second drive/stop
         * limit cycle. No gain change removes it; it is structural until the
         * integral has grown.
         *
         * A line through those two points is ff = 0.40 + 0.10 * v (0.57 -> 0.457,
         * 1.08 -> 0.508). The PID only trims around it, so the loop works from the
         * first tick even with the integral starting at zero.
         *
         * A zero target gets zero feedforward: "stop" must not carry an opening.
         * └────────────────────────────────────────────────────────────────────────┘
         */
        const double ff = (target_abs_ms > 0.0)
                        ? FF_BIAS + FF_PER_MPS * target_abs_ms
                        : 0.0;
        out.ffTerm = ff;

        double raw;
        if (std::fabs(speedErr) <= SPEED_DEADBAND) {
            raw = ff + out.pTerm + ki_ * integral_ + out.dTerm;
        } else {
            const double integralTrial = integral_ + speedErr * measDtS;
            raw = ff + out.pTerm + ki_ * integralTrial + out.dTerm;
            if (raw >= 0.0 && raw <= 1.0) {
                integral_ = integralTrial;
            } else {
                raw = ff + out.pTerm + ki_ * integral_ + out.dTerm;
            }
        }
        out.iTerm = ki_ * integral_;

        /*
         * ┌─ 한국어: 감속에 브레이크를 쓰면 안 됩니다 ────────────────────────────┐
         *
         * 이 기계에는 비례 브레이크 채널이 없습니다. TrackingLoop::act() 는
         * brake > 0 을 "ACC 개도 0 + 다운시프트"로 바꿔 보냅니다. 그래서 0.13 을
         * 내보내든 0.23 을 내보내든 물리적 결과는 같습니다 — 개도가 0이 되고,
         * 쟁기 견인 부하 때문에 1.5~2초 만에 완전히 섭니다.
         *
         * 에뮬레이터 실측(4.0 -> 2.4 km/h 목표 하강 지점마다 재현):
         *
         *     t=17.6  v=1.08  thr=0.00  brk=0.23  gear=2
         *     t=18.2  v=1.08  thr=0.00  brk=0.23  gear=1
         *     t=19.7  v=0.00  thr=1.00  brk=0.00  gear=1   <- 완전 정지
         *     t=21.5  v=0.62  thr=0.50
         *
         * 즉 "선회 진입에서 간헐적으로 멈추는 현상"의 정체는 브레이크입니다.
         * 그리고 감속에 브레이크가 필요하지도 않습니다 — 같은 로그에서 기어가
         * 2->1 로 내려가고, 결국 gear=1 / thr=0.65 / v=0.57 로 안정됩니다.
         * 감속은 기어가 다 하고 있고, 브레이크는 그 위에 얹혀서 세우기만 합니다.
         *
         * 그래서 목표속도가 0이 아니면 브레이크를 내보내지 않고, 개도를 "기계
         * 최저속도를 유지하는 개도"로 바닥 처리합니다. 목표는 이미
         * raiseToMachineFloor() 로 최저속도 이상이므로, 그보다 적은 개도를
         * 요구하는 것은 어차피 의미가 없습니다. 그것은 감속이 아니라 정지입니다.
         *
         * 다만 "기어로 잡을 수 있는 감속"에만 해당합니다. 다운시프트 한 단이
         * 바꿀 수 있는 속도차는 대략 기계 최저속도(0.57 m/s) 한 칸입니다.
         * 초과분이 그보다 크면 기어로는 못 잡으므로 브레이크가 진짜로 필요합니다.
         * 실측된 해로운 경우의 초과분은 0.41 m/s (목표 0.667, 실제 1.08) 로 이
         * 문턱 아래이고, 진짜 폭주(목표 1.111, 실제 3.0)는 1.9 m/s 로 위입니다.
         *
         * 브레이크는 그 외에는 진짜로 세워야 할 때만 씁니다: 목표 0, 그리고
         * 전후진 전환(brakingForReversal_, 위쪽에서 따로 처리).
         * └────────────────────────────────────────────────────────────────────┘
         * ┌─ English: do not brake for a routine slowdown ─────────────────────┐
         *
         * There is no proportional brake channel. TrackingLoop::act() turns any
         * brake > 0 into "accelerator opening 0 plus a downshift", so 0.13 and
         * 0.23 have identical physical effect: the opening goes to zero and the
         * plough draft brings the machine to a full stop within 1.5-2 s.
         *
         * Measured on the emulator, reproducing at every 4.0 -> 2.4 km/h target
         * step (see the trace above): brake 0.23 took it from 1.08 m/s to a dead
         * stop in 2.1 s. That is the "intermittent stopping when turning".
         *
         * The brake is not needed for the slowdown either: in the same trace the
         * gear drops 2 -> 1 and the machine settles at gear 1 / 0.65 opening /
         * 0.57 m/s. The gear does the work; the brake only stops it.
         *
         * So while a non-zero speed is commanded, emit no brake and floor the
         * opening at the value that holds the machine's slowest speed. The target
         * has already been raised to that floor by raiseToMachineFloor(), so
         * asking for less opening is not a slowdown — it is a stop.
         *
         * This only covers slowdowns a gear can deliver. One downshift is worth
         * roughly one machine-floor step (0.57 m/s); beyond that the gear cannot
         * do it and the brake is genuinely required. The measured harmful case is
         * 0.41 m/s of excess (target 0.667, actual 1.08), below the threshold; a
         * real runaway (target 1.111, actual 3.0) is 1.9 m/s, above it.
         *
         * Otherwise the brake is kept for a real stop (target 0) and for the
         * direction reversal handled above.
         * └────────────────────────────────────────────────────────────────────┘
         */
        const double RUNAWAY_EXCESS_MPS = MIN_MACHINE_SPEED_KMH * KMH_TO_MS;
        const bool gearCanHandleIt = (current_abs_ms - target_abs_ms)
                                   <= RUNAWAY_EXCESS_MPS;
        if (raw >= 0.0) {
            out.throttle = std::clamp(raw, 0.0, 1.0);
            out.brake = 0.0;
        } else if (target_abs_ms > 0.0 && gearCanHandleIt) {
            out.throttle = FF_BIAS + FF_PER_MPS * RUNAWAY_EXCESS_MPS;
            out.brake = 0.0;
        } else {
            out.throttle = 0.0;
            out.brake = std::min(1.0, KP_BRAKE * (-raw));
        }
        heldThrottle_ = out.throttle;
        heldBrake_ = out.brake;
        return out;
    }

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 속도(km/h) -> 기어. 목표가 음수면 후진(-1)입니다.
     *
     * 3단입니다. 트랙터의 Send_SFT_CMD 는 Low(1) / Middle(2) / High(3) 세 값만
     * 받습니다 (Receive_SFT_INFO.TRZ_SFT_STATE 도 같은 코드). 예전 4단 구분은
     * 내장 시뮬레이터의 가상 변속기 기준이었습니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Speed magnitude (km/h) -> gear; a negative target means reverse.
     *
     * Three stages, because Send_SFT_CMD accepts only Low(1) / Middle(2) /
     * High(3) — the same codes TRZ_SFT_STATE reports back. The old four-way split
     * belonged to the built-in simulator's virtual gearbox.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    int gearForSpeed(double targetSpeedKmh) const
    {
        if (targetSpeedKmh < 0.0) {
            return -1;
        }
        const double abs_kmh = std::fabs(targetSpeedKmh);
        if (maxForwardGear_ == 1 || abs_kmh < 3.0) return 1;  // Low
        if (maxForwardGear_ == 2 || abs_kmh < 6.0) return 2;  // Middle
        return maxForwardGear_;                              // High
    }

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 이 기계가 연속으로 유지할 수 있는 최저 지면속도(km/h)입니다. 에뮬레이터
     * 주행 4회의 GPS 샘플 3,865개에서 보고 속도는 0.00 / 0.57 / 0.62 / 1.03 /
     * 1.08 m/s 같은 이산값에만 존재하고 0.05~0.50 m/s 구간은 0.72%뿐입니다.
     * Low단 크리프 하한이 약 0.57 m/s = 2.05 km/h 라는 뜻입니다.
     *
     * RddfValidator::MIN_MACHINE_SPEED_KMH 가 이 값을 그대로 씁니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Lowest ground speed this machine can hold continuously, in km/h. Across
     * 3,865 GPS samples the reported speed only takes discrete values — 0.00 /
     * 0.57 / 0.62 / 1.03 / 1.08 m/s — with 0.72% anywhere in 0.05..0.50 m/s, so
     * the Low-gear creep floor is about 0.57 m/s = 2.05 km/h.
     *
     * RddfValidator::MIN_MACHINE_SPEED_KMH aliases this value.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    static constexpr double MIN_MACHINE_SPEED_KMH = 2.05;

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 기계가 낼 수 없는 목표속도를 하한으로 올립니다. 부호는 유지합니다.
     *
     * 참가자는 RDDF를 직접 만들어 제출합니다. 하한보다 느린 속도가 들어오는 것은
     * 예외가 아니라 정상적으로 일어날 일이고, 그때 스켈레톤이 어떻게 행동하느냐는
     * 설계 선택입니다. 선택지는 두 개뿐입니다.
     *
     *   A. 하한으로 올려 연속 주행한다  — 목표보다 빠르지만 계속 굴러간다
     *   B. 오차를 0으로 만들려 한다      — 크리프 하한과 정지를 오가며 평균만 맞춘다
     *
     * B가 "전진했다 멈췄다"의 정체입니다. 같은 경로 구간에서 헤드랜드 속도를
     * 1.2 -> 2.4 km/h 로 올리자 정지 시간이 15.9% -> 3.6% 로 떨어졌습니다.
     * 쟁기를 땅에 박고 있는 트랙터에게는 A가 낫고, 무엇보다 예측 가능합니다.
     * 목표를 못 지킨 사실은 targetRaisedToFloor 로 텔레메트리에 나갑니다.
     *
     * 0은 올리지 않습니다. 0은 "낼 수 없는 느린 속도"가 아니라 "여기서 정지"입니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Raises an unreachable target to the machine floor, keeping its sign.
     *
     * Participants author and submit their own RDDFs, so a below-floor speed is
     * a normal input, not an exceptional one, and what the skeleton does with it
     * is a design choice. There are only two options:
     *
     *   A. raise it to the floor  — faster than asked, but the machine keeps moving
     *   B. drive the error to zero — surge between the creep floor and a standstill
     *
     * B is what "drives, stops, drives, stops" actually is. Raising a path's
     * headland speed from 1.2 to 2.4 km/h took stopped time from 15.9% to 3.6%
     * over the same segment. For a tractor with an implement in the ground A is
     * better, and above all it is predictable. The fact that the request was not
     * honoured leaves through targetRaisedToFloor in telemetry.
     *
     * Zero is never raised: it means "stop here", not "a speed too slow to hold".
     * └────────────────────────────────────────────────────────────────────────┘
     */
    static double raiseToMachineFloor(double targetSpeedKmh)
    {
        const double mag = std::fabs(targetSpeedKmh);
        if (mag <= 0.0 || mag >= MIN_MACHINE_SPEED_KMH) {
            return targetSpeedKmh;
        }
        return (targetSpeedKmh < 0.0) ? -MIN_MACHINE_SPEED_KMH
                                      :  MIN_MACHINE_SPEED_KMH;
    }

private:
    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 변속 헌팅 방지. gearForSpeed 의 임계값은 딱딱해서, 목표속도가 3.0km/h
     * 근처에서 흔들리면 기어가 매초 1<->2로 오갑니다. 에뮬레이터 4km/h 주행에서
     * 기어 변경 40회가 목표 플립 40회와 정확히 1:1로 일치했고, 변속 직후 1초간
     * 이동거리는 0.639m로 평소 0.769m보다 17% 짧았습니다. 실기 변속기는 초당
     * 한 번씩 단을 바꿀 수 없습니다.
     *
     * 그래서 새 단이 GEAR_DWELL_S 동안 연속으로 요구될 때만 실제로 바꿉니다.
     * 후진 전환은 예외로 즉시 반영합니다 — 그 경로는 이미 brakingForReversal_
     * 이 차량을 세운 뒤이므로 지연시킬 이유가 없습니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Anti-hunt for the transmission. The thresholds in gearForSpeed are hard,
     * so a target hovering near 3.0 km/h flips the gear between 1 and 2 every
     * second. In the emulator's 4 km/h run 40 gear changes matched 40 target
     * flips exactly, and the second after a shift covered 0.639 m against
     * 0.769 m otherwise — 17% lost. A real transmission cannot change stage
     * once a second.
     *
     * A new stage is therefore adopted only after it has been requested
     * continuously for GEAR_DWELL_S. Engaging reverse is exempt and immediate:
     * on that path brakingForReversal_ has already brought the vehicle to a
     * stop, so there is nothing to protect by waiting.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    int gearWithHysteresis(double targetSpeedKmh, double dtS)
    {
        const int want = gearForSpeed(targetSpeedKmh);
        if (!haveGear_) {
            curGear_ = want;
            pendingGear_ = want;
            gearDwellS_ = 0.0;
            haveGear_ = true;
            return curGear_;
        }
        if (want == curGear_) {
            pendingGear_ = want;
            gearDwellS_ = 0.0;
            return curGear_;
        }
        if (want != pendingGear_) {
            pendingGear_ = want;
            gearDwellS_ = 0.0;
        }
        gearDwellS_ += (dtS > 0.0) ? dtS : 0.0;
        const bool reversalInvolved = (want < 0) || (curGear_ < 0);
        if (reversalInvolved || gearDwellS_ >= GEAR_DWELL_S) {
            curGear_ = want;
            gearDwellS_ = 0.0;
        }
        return curGear_;
    }

    static constexpr double KMH_TO_MS = 1.0 / 3.6;
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 게인을 일부러 낮게 잡았습니다. 차량에는 질량과 구동계 지연이 있어서
    // KP가 크면 스로틀/브레이크가 목표를 지나쳐 진동합니다. 1.5가 실험적으로
    // 안정적인 값이었습니다. 불감대를 좁게(0.03) 두면 정상상태 정확도는
    // 유지하면서도 미세 진동은 생기지 않습니다.
    //
    // KI는 아주 작게 시작합니다. 경사·견인부하의 정상상태 오차를 수십 초에 걸쳐
    // 메우는 정도이고, 이보다 크면 감속 구간에서 적분이 남아 과가속합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Gains kept conservative to avoid overshoot oscillation: the vehicle has
    // dynamics lag (mass + drivetrain), and high KP makes throttle/brake swing
    // past the target. 1.5 is the empirically stable value; the tight deadband
    // retains steady-state precision without inducing dither.
    //
    // KI starts very small — enough to close a slope/draft offset over tens of
    // seconds. Larger values leave integral behind after a slow section and the
    // vehicle over-accelerates out of it.
    // └────────────────────────────────────────────────────────────────────────┘
    // 튜닝 UI가 setGains()로 런타임에 바꿀 수 있도록 constexpr 에서 일반 멤버가
    // 됐습니다 (TrackerParams 참고). 기본값은 기존 상수와 동일합니다.
    // Plain members (formerly constexpr) so the tuning UI can change them at
    // runtime via setGains(); defaults equal the old constants.
    double kp_ = 1.5;
    double ki_ = 0.05;
    double kd_ = 0.1;
    static constexpr double KP_BRAKE = 1.5;
    static constexpr double D_TAU_S = 0.3;
    static constexpr double SPEED_DEADBAND = 0.03;  // m/s

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 앞먹임 계수: ff = FF_BIAS + FF_PER_MPS * 목표속도(m/s).
    // 에뮬레이터 정상상태 실측 두 점(0.57 m/s -> 0.448, 1.08 m/s -> 0.515)을
    // 지나는 직선입니다. 상수항이 큰 이유는 쟁기 견인 부하 때문이고, 이 기계는
    // 어떤 속도든 0.45 정도는 열어야 굴러갑니다. 작업기나 토양이 바뀌면 이 두
    // 값을 다시 재세요 — update() 안의 유도 과정 주석을 참고하면 됩니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Feedforward: ff = FF_BIAS + FF_PER_MPS * target speed in m/s, the line
    // through two measured steady states (0.57 m/s -> 0.448, 1.08 m/s -> 0.515).
    // The constant term is large because of plough draft: this machine needs
    // roughly 0.45 open to move at all. Re-measure both if the implement or the
    // soil changes; update() documents how they were derived.
    // └────────────────────────────────────────────────────────────────────────┘
    static constexpr double FF_BIAS = 0.40;
    static constexpr double FF_PER_MPS = 0.10;
    // 이 속력 아래면 정지로 간주. / Below this the vehicle counts as stopped.
    static constexpr double STOP_THR_MS = 0.05;     // m/s
    // 측정값이 연속으로 같아도 이 시간이 지나면 PID를 한 스텝 전진시킵니다.
    // 정상 약 10Hz cadence보다 훨씬 길어 held sample 이상 때만 걸립니다.
    // Advance the PID after this long even if consecutive samples read identical.
    // Far above the normal ~10 Hz cadence, so only a held sample reaches it.
    static constexpr double MAX_MEAS_HOLD_S = 1.5;  // s
    // 새 기어단이 이 시간만큼 연속으로 요구돼야 실제로 변속합니다.
    // A new gear stage must be requested continuously for this long to be adopted.
    static constexpr double GEAR_DWELL_S = 1.5;  // s

    // 마지막으로 관측한 0이 아닌 목표 속도의 부호. / Last non-zero sign of the target speed.
    int prevTargetSign_;
    // 방향 전환을 위해 제동 중인가. / Are we braking in order to reverse.
    bool brakingForReversal_;

    double integral_;
    double prevError_;
    double dFiltered_;
    bool havePrevError_;

    // 샘플 데이터 게이트 상태. / Sampled-data gate state.
    bool haveMeasurement_;      // 측정값을 한 번이라도 소비했는가 / consumed any sample yet
    bool haveSampleId_ = false;
    std::int64_t lastSampleId_ = -1;
    double lastMeasuredSpeed_;  // 마지막으로 PID를 돌린 측정값 / sample the PID last ran on
    double measAgeS_;           // 그 뒤로 누적된 시간 / seconds accumulated since then
    double heldThrottle_;       // 샘플 사이에 유지할 지령 / command held between samples
    double heldBrake_;

    // 변속 히스테리시스 상태. / Gear hysteresis state.
    int curGear_;        // 현재 물려 있는 단 / stage currently engaged
    int pendingGear_;    // 요구되고 있는 단 / stage being requested
    double gearDwellS_;  // 그 요구가 이어진 시간 / how long that request has held
    bool haveGear_;
    int maxForwardGear_ = 3;
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_SPEEDCONTROLLER_HPP
