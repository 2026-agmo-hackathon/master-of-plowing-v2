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
 * ── 개도가 아니라 변속단이 속도를 결정합니다 ──
 *
 * 에뮬레이터에서 같은 경로를 조합을 바꿔가며 네 번 주행하고 10Hz 텔레메트리를
 * 받아 보면, 한 단 안에서는 개도를 얼마로 주든 정상상태 속도가 같습니다.
 *
 *     large 4WD + 3.6m 쟁기, 1단:  개도 1.5V -> 1.48 km/h   4.0V -> 1.48 km/h
 *     compact  + 작업기 없음, 2단: 개도 1.5V -> 5.37 km/h   2.5V -> 5.37 km/h
 *
 * 그리고 단별 속도는 트랙터/작업기 조합에 따라 두 배 가까이 달라집니다.
 *
 *     조합                       1단     2단     3단
 *     compact + 없음             2.78    5.37     -
 *     medium  + 쟁기 1.6m        1.85    3.33     -
 *     large   + 쟁기 3.6m        1.48    2.78    4.07
 *
 * 예전 코드는 이 관계를 거꾸로 잡고 있었습니다. 속도는 PID가 개도로 맞추고,
 * 정작 속도를 결정하는 변속단은 "목표속도 -> 단" 하드코딩 표(3.0/6.0 km/h)로
 * 개루프로 골랐습니다. 그 표는 compact 트랙터에서 잰 값이라, large + 3.6m
 * 쟁기로 4.00 km/h 경로를 돌리면 2단이 걸리고 개도가 99.1% 시간 포화된 채
 * 2.78 km/h 밖에 못 냅니다. 같은 조합에 경로 속도만 6.00 으로 바꿔 3단이
 * 걸리게 하면 4.07 km/h 가 나옵니다 — 낼 수 있는 속도였는데 요청한 적이
 * 없었을 뿐입니다.
 *
 * 그래서 이제 단 선택을 폐루프로 돌립니다. 단별로 "실제로 유지된 속도"를
 * 주행 중 학습하고(sustainedKmh), 목표를 낼 수 있는 가장 낮은 단을 고릅니다.
 * 아직 학습되지 않은 단은 예전 표를 사전값으로 씁니다 — 그래서 냉시동
 * 동작은 예전과 같고, 측정이 사전값과 어긋날 때만 달라집니다.
 *
 * ── 그런데 단은 목표를 "넘겨서만" 맞출 수 있습니다 ──
 *
 * 단이 낼 수 있는 속도는 이산값이고, 경로가 적어 낸 속도는 그 사이에 있습니다.
 * 위 표에서 medium + 쟁기 1.6m 로 4.00 km/h 를 요구하면 2단은 3.33 으로 모자라고
 * 3단은 4.82 로 넘칩니다. 어느 단도 4.00 이 아닙니다. 그래서 "목표를 낼 수 있는
 * 가장 낮은 단"만으로는 항상 목표보다 빠르거나 느립니다.
 *
 * ── 개도는 비례가 아니라 on/off 입니다 ──
 *
 * 같은 텔레메트리를 다시 보면 개도의 정체가 드러납니다. accV1 은 0.8V(0%)에서
 * 4.0V(100%)까지인데,
 *
 *     accV1 1.06V (개도 8%),  compact 1단  ->  2.78 km/h 를 몇 초씩 그대로 유지
 *     accV1 4.00V (개도 100%), compact 1단 ->  2.78 km/h
 *     accV1 0.80V (개도 0%)               ->  약 0.47 m/s^2 로 감속 (5.19 -> 1.67
 *                                             km/h 를 2.1초에)
 *
 * 즉 0 보다 크기만 하면 값은 아무 의미가 없고, 0 이면 감속합니다. 이 기계의
 * 개도는 릴레이입니다. 비례 제어기를 아무리 잘 조율해도 정상상태 속도를
 * 바꾸지 못합니다 — 실제로 v1.1.52 는 3단에서 개도를 26% 만 포화시키고도
 * 4.07 km/h 를 그대로 유지했습니다.
 *
 * ── 그래서 시간축으로 변조합니다 ──
 *
 * 크기로 못 하면 시간으로 합니다. 목표를 낼 수 있는 가장 낮은 단을 물고, 목표를
 * 넘어서면 개도를 0 으로 끊었다가 목표 아래로 내려오면 다시 엽니다. 평균이
 * 목표가 됩니다. 단이 4.82 를 내든 5.37 을 내든 상관없습니다.
 *
 * 여기에 두 가지가 필요했습니다.
 *
 *   1. 적분이 실제로 개도를 닫을 수 있어야 합니다. 안티와인드업이 아래쪽에서
 *      시험 적분을 거부하면 raw 가 0 바로 위에 멈추는데, 그 값을 그대로 내보내면
 *      "완전히 닫으라"가 "활짝 열어라"가 됩니다(0 보다 큰 개도는 전부 같은
 *      지령이므로). 아래쪽 포화를 출력에 반영하도록 고쳤습니다. KI 도 0.05 에서
 *      0.5 로 올렸습니다 — 0.05 는 정상상태 오차를 없애는 데 100초가 걸려서
 *      사실상 적분이 없는 것과 같았습니다.
 *
 *   2. 스위칭 지점을 거리로 보정해야 합니다. PID 만으로 만들어진 릴레이는
 *      평균이 목표와 맞을 이유가 없습니다(compact 조합에서 4.00 요청에 평균
 *      4.35). "초과속으로 앞서 나간 거리"를 장부로 들고 문턱을 밀어 줍니다.
 *
 * 개도에 진짜 권한이 있는 실기에서 이것이 켜지면 안 됩니다. 두 경우를 가르는
 * 표식은 초과속의 지속시간입니다 — 가속 중의 오버슈트는 곧 사라지고, 단이
 * 넘쳐서 생기는 초과속은 영원히 이어집니다. 그래서 초과속이 2초 동안 끊기지
 * 않고 이어졌을 때만 무장합니다.
 *
 * 그래서 목표를 기계 하한으로 "올리는" 처리도 없앴습니다. 하한보다 느린 목표는
 * 이제 가장 낮은 단 + 개도 변조로 정확히 만들어 냅니다. RDDF 에 적힌 속도는
 * 트랙터/작업기 조합과 무관하게 그대로 지켜집니다.
 *
 * 개도 PID 는 그대로 둡니다. 실기에서는 개도에 실제 권한이 있고, 단 안에서
 * 목표를 다듬는 것은 여전히 PID 의 몫이기 때문입니다.
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
 * THE GEAR STAGE SETS THE SPEED, NOT THE OPENING. Four runs of one route on the
 * emulator, logged at 10 Hz, show that within a stage the steady-state speed is
 * independent of the accelerator opening (1.5 V and 4.0 V of feedback settle at
 * the same km/h), while the speed each stage delivers varies by nearly 2x with
 * the tractor and implement:
 *
 *     tractor + implement          stage 1   stage 2   stage 3
 *     compact + none                 2.78      5.37       -
 *     medium  + plough 1.6 m         1.85      3.33       -
 *     large   + plough 3.6 m         1.48      2.78      4.07
 *
 * The old code had that relationship backwards: the PID chased the speed with
 * the opening, while the stage — the thing that actually decides it — came from
 * a hard-coded target-to-stage table (3.0 / 6.0 km/h) measured on the compact.
 * On the large tractor with a 3.6 m plough a 4.00 km/h route therefore ran in
 * stage 2 at 2.78 km/h with the opening saturated 99.1% of the time. Raising the
 * route's number to 6.00 on the same machine selects stage 3 and yields
 * 4.07 km/h: the speed was available and was never asked for.
 *
 * Stage selection is now closed-loop. The speed each stage actually sustains is
 * learned during the run (sustainedKmh) and the lowest stage that can deliver
 * the target is engaged. Stages not yet measured fall back to the old table as a
 * prior, so cold behaviour is unchanged and only contradicted priors move it.
 *
 * BUT A STAGE CAN ONLY MEET A TARGET BY OVERSHOOTING IT. Stage speeds are
 * discrete and the route's number falls between them: on the medium tractor with
 * a 1.6 m plough a 4.00 km/h request has stage 2 short at 3.33 and stage 3 over
 * at 4.82. "The lowest stage that can deliver the target" is therefore always
 * too fast or too slow, never right.
 *
 * THE OPENING IS NOT PROPORTIONAL, IT IS A RELAY. The same telemetry says what
 * the opening really is. accV1 spans 0.8 V (0%) to 4.0 V (100%), and
 *
 *     accV1 1.06 V (8% open),  compact stage 1 -> holds 2.78 km/h for seconds
 *     accV1 4.00 V (100%),     compact stage 1 -> 2.78 km/h
 *     accV1 0.80 V (0%)                        -> decelerates at ~0.47 m/s^2
 *                                                 (5.19 -> 1.67 km/h in 2.1 s)
 *
 * Anything above zero is the same command; zero coasts. No amount of tuning a
 * proportional law changes a steady-state speed here — v1.1.52 held 4.07 km/h in
 * stage 3 with the opening saturated only 26% of the time.
 *
 * SO MODULATE IN TIME INSTEAD. What cannot be done with amplitude is done with
 * duty cycle: engage the lowest stage that can reach the target, cut the opening
 * to zero once past the target and restore it once below. The mean lands on the
 * target whether the stage delivers 4.82 or 5.37.
 *
 * Two things had to be fixed for that to work.
 *
 *   1. The integral has to be able to close the opening. When anti-windup
 *      refuses the trial on the low side, raw stops just above zero, and emitting
 *      that turns "close completely" into "open fully" (every opening above zero
 *      is the same command). Low-side saturation now reaches the output. KI also
 *      went from 0.05 to 0.5: at 0.05 clearing a steady-state error took nearly
 *      100 s, which is no integral action at all.
 *
 *   2. The switching point has to be trimmed by distance. A relay produced by the
 *      PID alone has no reason to average the target — on its own it averages
 *      4.35 km/h against a 4.00 request on the compact pairing. A ledger carrying
 *      the distance gained by overspeeding pushes the threshold until it does.
 *
 * This must not engage on a real machine, where the opening has authority.
 * Duration separates the two cases — an overshoot while accelerating passes, the
 * overspeed a too-fast stage produces does not — so it arms only after two
 * seconds of uninterrupted overspeed.
 *
 * Raising the target to the machine floor is gone with it: a below-floor request
 * is now delivered exactly, by the slowest stage plus modulation. The speed
 * written in the RDDF is honoured whatever the tractor and implement are.
 *
 * The opening PID stays: on the real machine the opening does have authority,
 * and trimming to the target within a stage is still its job.
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
        // 경로가 요구한 원래 목표. 이제 targetMps 와 항상 같습니다 — 하한으로
        // 올리는 처리를 없애고 개도 변조로 정확히 만들어 내기 때문입니다.
        // What the path asked for. Now always equal to targetMps: the floor-raise
        // is gone and the request is delivered exactly, by modulating the opening.
        double requestedMps = 0.0;
        double pTerm = 0.0;
        double iTerm = 0.0;
        double dTerm = 0.0;
        bool brakingForReversal = false;

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 단 선택 폐루프의 상태. 목표를 못 내고 있을 때 그것이 (a) 아직 학습이
        // 안 끝난 것인지 (b) 최상단인데도 부족한 것인지 이 셋을 봐야 구분됩니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Closed-loop stage state. When the target is not being met these three
        // separate "still learning" from "top stage and still short".
        // └────────────────────────────────────────────────────────────────────┘
        double stageSustainedKmh = 0.0;  // 이 단이 낼 수 있다고 보는 속도 / credited to the engaged stage
        bool stageMeasured = false;      // 실측인가 사전값인가 / measured, or still the prior
        double machineFloorKmh = 0.0;    // 가장 느린 단이 내는 속도 / what the slowest stage delivers

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 개도 변조기의 상태. debtM 이 밴드 안에서 흔들리고 있으면 목표를
        // 지키는 중이고, 한쪽 클램프에 붙어 있으면 못 지키는 중입니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Modulator state. debtM oscillating inside the band means the target is
        // being held; pinned at a clamp means it is not.
        // └────────────────────────────────────────────────────────────────────┘
        double debtM = 0.0;              // 계획 거리 - 실제 거리 / planned minus travelled, metres
        // 개도 0 에서의 감속률(실측). 0 이면 아직 관측 전이고, 그동안은 추측항법이
        // 꺼져 있어 코스팅이 측정 지연만큼 길어집니다.
        // Measured deceleration at zero opening. Zero means not yet observed, and
        // until then dead reckoning is off and a coast runs as long as the
        // measurement lag.
        double coastDecelMps2 = 0.0;
        bool throttleGated = false;      // 변조기가 개도를 끊었는가 / opening cut by the modulator
        bool belowSlowestStage = false;  // 요청이 1단보다 느린가 / request slower than stage 1
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
        debtM_ = 0.0;
        overspeedS_ = 0.0;
        modulating_ = false;
        gateOpen_ = true;
        heldGated_ = false;
        heldOpenThrottle_ = 0.0;
        heldCoastBelowMps_ = 0.0;
        lastMeasuredAbsSpeed_ = 0.0;
        lastChangedAbsSpeed_ = 0.0;
        speedValueAgeS_ = 0.0;
        coastDecelMps2_ = 0.0;
        resetStageTable();
    }

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 학습한 단별 속도를 버리고 사전값으로 되돌립니다. 트랙터나 작업기가 바뀌면
     * 이전 조합에서 잰 값은 전부 틀리기 때문에, 주행 시작마다(reset 경유)
     * 그리고 지오메트리가 바뀔 때 호출합니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Drops the learned per-stage speeds back to their priors. Everything
     * measured on the previous tractor/implement pair is wrong for the next one,
     * so this runs at every run start (through reset) and on a geometry change.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    void resetStageTable()
    {
        for (int stage = 0; stage <= MAX_STAGES; ++stage) {
            stageKmh_[stage] = 0.0;
            stageMeasured_[stage] = false;
        }
        learnStage_ = 0;
        stageEngagedS_ = 0.0;
        shortfallS_ = 0.0;
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

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 카탈로그의 maxForwardGear 는 4/5/6 이지만 Send_SFT_CMD 는 Low/Middle/High
     * 셋만 받습니다. 그래서 3으로 자릅니다 — 시뮬레이터가 더 많은 단을
     * 모델링하더라도 이 인터페이스로 요청할 수 있는 것은 3단까지입니다.
     * 기계가 바뀌면 학습한 단별 속도는 더 이상 유효하지 않습니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * The catalog reports maxForwardGear as 4/5/6, but Send_SFT_CMD accepts only
     * Low/Middle/High, so this clamps to 3: whatever the simulator models, three
     * stages is all this interface can ask for. A different machine invalidates
     * everything learned about the stages.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    void setVehicleGeometry(const VehicleGeometry& geometry)
    {
        if (!geometry.valid()) return;
        const int stages = std::clamp(geometry.maxForwardGear, 1, MAX_STAGES);
        if (stages != maxForwardGear_) {
            maxForwardGear_ = stages;
            resetStageTable();
        }
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
        out.machineFloorKmh = machineFloorKmh();
        out.belowSlowestStage = (out.requestedMps > 0.0)
                             && (std::fabs(targetSpeedKmh) < out.machineFloorKmh);

        const double target_abs_ms = out.requestedMps;
        const double current_abs_ms = std::fabs(currentSpeedMps);
        const double speedErr = target_abs_ms - current_abs_ms;  // m/s

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 목표를 낼 수 있는 가장 낮은 단. 이 단은 목표 이상이므로 남는 만큼은
        // 아래 변조기가 시간축에서 깎아 냅니다. 하한보다 느린 목표에서도 1단이
        // 나오므로 별도 처리가 필요 없습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // The lowest stage that can deliver the target. It is at or above the
        // request, and the modulator below takes the excess out in the time
        // domain. A below-floor target lands on stage 1, so it needs no case.
        // └────────────────────────────────────────────────────────────────────┘
        out.gear = gearWithHysteresis(targetSpeedKmh, dtS);
        if (out.gear >= 1 && out.gear <= MAX_STAGES) {
            // 실측이 있으면 그 값을, 없으면 0을 내보냅니다. 사전값을 실측인 척
            // 내보내면 대시보드에서 둘을 구분할 수 없습니다 — 최상단의 사전값은
            // 애초에 "상한 없음"이라 숫자로 쓸 수도 없습니다.
            // The measurement if there is one, otherwise 0. Publishing a prior as
            // if it were measured would make the two indistinguishable on the
            // dashboard, and the top stage's prior is "unbounded" anyway.
            out.stageMeasured = stageMeasured_[out.gear];
            out.stageSustainedKmh = out.stageMeasured ? stageKmh_[out.gear] : 0.0;
        }

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
            // 방향이 바뀌면 그 전까지의 거리 장부도 의미가 없습니다.
            // A direction change voids the distance ledger with everything else.
            debtM_ = 0.0;
                gateOpen_ = true;
            out.debtM = debtM_;
        out.coastDecelMps2 = coastDecelMps2_;
            // 세우는 중의 속도는 어떤 단의 능력도 말해주지 않습니다.
            // Speed while stopping says nothing about any stage's capability.
            abandonStageObservation();
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
            /*
             * ┌─ 한국어 ───────────────────────────────────────────────────────┐
             * 샘플의 나이와 "값"의 나이는 다릅니다. 이 기계는 자세를 10Hz 로
             * 보내지만 속도는 0.185 km/h (0.1 knot) 단위로 양자화돼 있어 값이
             * 실제로 움직이는 것은 0.3초에 한 번쯤입니다. 샘플 id 만 보면 매 틱
             * 새 정보가 온 것처럼 보이지만, 감속 중에는 그 값이 최대 0.3초
             * 낡았습니다. 쟁기를 박은 조합의 코스팅 감속 1.7 m/s^2 에서 0.3초는
             * 0.51 m/s 이고, 두렁 목표속도 자체가 0.33 m/s 입니다 — 낡은 값을
             * 그대로 믿으면 멈출 때까지 개도를 끊고 있게 됩니다.
             * └────────────────────────────────────────────────────────────────┘
             * ┌─ English ──────────────────────────────────────────────────────┐
             * The age of a sample and the age of its value are different things.
             * This machine sends pose at 10 Hz, but speed is quantised to
             * 0.185 km/h (0.1 knot), so the value only actually moves about every
             * 0.3 s. By sample id every tick looks like fresh information, while
             * during a deceleration the value is up to 0.3 s stale. At the
             * 1.7 m/s^2 coast of a pairing with its plough down, 0.3 s is
             * 0.51 m/s, and the headland target is itself 0.33 m/s — trusting the
             * stale value keeps the opening shut until the machine has stopped.
             * └────────────────────────────────────────────────────────────────┘
             */
            // 개도 0 에서의 감속률은 조합마다 크게 다릅니다 — 작업기 없는
            // compact 는 0.47 m/s^2, 쟁기를 박은 medium 은 1.7 m/s^2 로 3배가
            // 넘습니다. 상수로 박아 두면 한쪽에서 반드시 틀리므로 실측합니다.
            // Deceleration at zero opening varies enormously with the pairing:
            // 0.47 m/s^2 for the bare compact against 1.7 m/s^2 for the medium
            // with its plough down, over a factor of three. A constant would be
            // wrong on one of them, so it is measured.
            if (haveMeasurement_ && current_abs_ms == lastChangedAbsSpeed_) {
                speedValueAgeS_ += dtS;
            } else {
                if (haveMeasurement_ && heldGated_ && speedValueAgeS_ > 0.0) {
                    // 값이 움직였습니다. 개도를 끊고 있었다면 그 낙폭이 곧
                    // 코스팅 감속입니다.
                    // The value moved; if the opening was shut, that drop is the
                    // coast deceleration.
                    const double drop = lastChangedAbsSpeed_ - current_abs_ms;
                    if (drop > 0.0) {
                        const double observed =
                                std::clamp(drop / speedValueAgeS_,
                                           MIN_COAST_MPS2, MAX_COAST_MPS2);
                        coastDecelMps2_ +=
                                COAST_LEARN_ALPHA * (observed - coastDecelMps2_);
                    }
                }
                lastChangedAbsSpeed_ = current_abs_ms;
                speedValueAgeS_ = 0.0;
            }
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

            /*
             * ┌─ 한국어 ───────────────────────────────────────────────────────┐
             * 개도를 끊어 둔 동안에는 영차 홀드를 깨고 매 틱 다시 판단합니다.
             *
             * 지령은 10Hz 로 나가지만 속도 측정은 약 3Hz 입니다. 홀드를 그대로
             * 두면 개도를 한 번 끊었을 때 최소 0.3초를 끊게 되는데, 쟁기를 박은
             * medium 조합에서 실측 감속이 1.7 m/s^2 이라 그 0.3초가 0.51 m/s 를
             * 지웁니다. 두렁 목표속도 1.20 km/h 는 0.33 m/s 이므로, 한 번 끊으면
             * 무조건 정지입니다. 실제로 그렇게 됐습니다 — 두렁 시간의 25%가
             * 정지였습니다.
             *
             * 새 측정이 없어도 감속률은 압니다(위에서 실측해 둡니다). 그래서
             * 마지막 측정값에서 추측항법으로 속도를 내리다가, 목표까지 내려온
             * 순간 개도를 되돌립니다. 액추에이터는 10Hz 로 쓸 수 있는데 측정
             * 주기에 묶여 있을 이유가 없습니다.
             *
             * 반대 방향(열려 있는데 끊기)은 하지 않습니다. 그쪽은 측정 없이는
             * 초과속을 알 수 없고, 추측으로 개도를 끊는 것은 위험합니다.
             * └────────────────────────────────────────────────────────────────┘
             * ┌─ English ──────────────────────────────────────────────────────┐
             * While the opening is shut the zero-order hold is broken and the
             * decision is retaken every tick.
             *
             * Commands go out at 10 Hz but speed is measured at about 3 Hz.
             * Holding through that makes the shortest possible cut 0.3 s, and at
             * the 1.7 m/s^2 coast measured for the medium tractor with its plough
             * in the ground, 0.3 s erases 0.51 m/s. A 1.20 km/h headland target is
             * 0.33 m/s, so any cut at all is a stop — and that is what happened:
             * a quarter of the headland was spent at a standstill.
             *
             * The coast rate is known without a new measurement (it is measured
             * above), so the speed is dead-reckoned down from the last sample and
             * the opening is restored the moment it reaches the target. The
             * actuator runs at 10 Hz; there is no reason to tie it to the sensor.
             *
             * The opposite direction is not done: without a measurement there is
             * no evidence of overspeed, and cutting the opening on a guess is not
             * a risk worth taking.
             * └────────────────────────────────────────────────────────────────┘
             */
            if (heldGated_ && target_abs_ms > 0.0 && coastDecelMps2_ > 0.0) {
                const double predicted = std::max(
                        0.0, current_abs_ms - coastDecelMps2_ * speedValueAgeS_);
                if (target_abs_ms - predicted >= heldCoastBelowMps_) {
                    heldThrottle_ = heldOpenThrottle_;
                    heldBrake_ = 0.0;
                    heldGated_ = false;
                }
            }

            out.throttle = heldThrottle_;
            out.brake = heldBrake_;
            out.debtM = debtM_;
        out.coastDecelMps2 = coastDecelMps2_;
            out.throttleGated = heldGated_;
            return out;
        }
        const double measDtS = measAgeS_;
        measAgeS_ = 0.0;

        lastMeasuredSpeed_ = currentSpeedMps;
        lastMeasuredAbsSpeed_ = current_abs_ms;
        haveMeasurement_ = true;
        if (identifiedSample) { lastSampleId_ = sampleId; haveSampleId_ = true; }

        /*
         * ┌─ 한국어 ─────────────────────────────────────────────────────────────┐
         * 개도 변조기: 초과속 릴레이 + 거리 장부 보정.
         *
         * 이 기계의 개도는 릴레이입니다(파일 머리말 참고). 0보다 크기만 하면 지금
         * 물린 단의 속도가 나오고, 0이면 약 0.47 m/s^2 로 감속합니다. 단이 낼 수
         * 있는 속도는 이산값이라 경로가 적어 낸 4.00 km/h 같은 값과 정확히
         * 일치하는 일이 없습니다. 크기로 못 맞추면 시간으로 맞춰야 합니다.
         *
         * PID 만으로도 릴레이는 만들어집니다 — 초과속이면 적분이 개도를 0까지
         * 끌어내리고, 목표 아래로 내려오면 다시 엽니다. 그런데 그 스위칭 지점은
         * PID 상태가 정하는 것이라 평균 속도가 목표와 맞을 이유가 없습니다.
         * 실제로 그것만으로는 compact 조합에서 4.00 요청에 평균 4.35 가 나옵니다.
         *
         * 그래서 스위칭 지점을 거리로 보정합니다. "초과속으로 앞서 나간 거리"를
         * 미터 장부(debtM_)로 들고 다니며 코스팅 문턱을 밀어 줍니다. 장부가
         * 쌓일수록 더 이른 초과속에서, 결국에는 목표보다 조금 느린 지점에서도
         * 개도를 끊게 되어 평균이 목표로 내려옵니다.
         *
         *     코스팅 조건:  오차 < -COAST_BASE_MPS - DEBT_GAIN_PER_S * 장부
         *
         * ── 이 변조기가 켜지면 안 되는 경우 ──
         *
         * 개도에 진짜 권한이 있는 기계에서는 PID 가 목표를 잡아냅니다. 거기서
         * 개도를 끊으면 잘 돌던 비례 루프에 릴레이를 심는 셈이고, 예전에 겪은
         * 가속/정지 리밋 사이클이 그대로 돌아옵니다.
         *
         * 두 경우를 가르는 표식은 초과속의 "지속시간"입니다. 가속 중에 목표를
         * 지나쳐 가는 것은 과도응답이라 곧 사라지고(단위테스트 플랜트에서 0.6초),
         * 단이 넘쳐서 생기는 초과속은 영원히 이어집니다. 그래서 초과속이
         * COAST_ARM_S 동안 끊기지 않고 이어졌을 때만 무장합니다.
         *
         * 한 번 무장하면 주행이 끝날 때까지 유지합니다. 해제 조건을 두면 무장과
         * 해제를 오가는 느린 헌팅이 생기고, 무장 자체는 해롭지 않습니다 —
         * 목표를 지키고 있으면 문턱에 걸리는 일이 없습니다.
         *
         * 불감대 안의 오차는 장부에 올리지 않고, 그 동안은 장부가 0 으로
         * 잊혀집니다(DEBT_LEAK_TAU_S). 목표를 잘 잡고 있는데 옛 잔액이 문턱을
         * 밀어 둔 채로 굳으면 없어도 될 진동이 생깁니다.
         *
         * 장부는 한쪽만 씁니다. 초과속으로 앞서 나간 거리는 기록하되 뒤처진
         * 거리는 쌓지 않습니다. 뒤처짐을 쌓으면 그것이 "따라잡을 권리"가 되어
         * 목표를 넘겨도 개도를 열어 두게 되는데, 못 간 거리를 나중에 과속으로
         * 메우는 것은 애초에 속도 프로파일의 뜻이 아닙니다.
         * └────────────────────────────────────────────────────────────────────────┘
         * ┌─ English ────────────────────────────────────────────────────────────┐
         * The opening modulator: an overspeed relay, trimmed by a distance ledger.
         *
         * The opening on this machine is a relay (see the file header): above zero
         * it delivers the engaged stage's speed, at zero it coasts down at about
         * 0.47 m/s^2. Stage speeds are discrete and never coincide with a route
         * number like 4.00 km/h, so what cannot be matched in amplitude has to be
         * matched in time.
         *
         * The PID alone already produces a relay — under overspeed the integral
         * drives the opening to zero, and below target it opens again. But where
         * it switches is decided by PID state, so there is no reason for the mean
         * speed to land on the target: on its own it averages 4.35 km/h against a
         * 4.00 request on the compact pairing.
         *
         * So the switching point is trimmed by distance. A ledger in metres
         * (debtM_) carries the distance gained by overspeeding and pushes the
         * coast threshold: the fuller it is, the earlier the opening is cut, until
         * eventually it is cut slightly below target and the mean comes down.
         *
         *     coast when:  error < -COAST_BASE_MPS - DEBT_GAIN_PER_S * ledger
         *
         * WHEN THIS MUST NOT ENGAGE. On a machine where the opening has real
         * authority the PID holds the target, and cutting the opening there plants
         * a relay in a proportional loop that was working — the old
         * accelerate/stop limit cycle, back again.
         *
         * Duration is what separates the two cases. Sailing past the target while
         * accelerating is a transient and passes (0.6 s on the unit-test plant);
         * the overspeed a too-fast stage produces lasts forever. So the modulator
         * arms only after COAST_ARM_S of uninterrupted overspeed.
         *
         * Once armed it stays armed for the run. A disarm condition only produces
         * slow arm/disarm hunting, and being armed is harmless while the target is
         * held, because then the threshold is never crossed.
         *
         * Errors inside the deadband are not booked, and the ledger is forgotten
         * while there (DEBT_LEAK_TAU_S). An old balance frozen off-centre would
         * manufacture an oscillation on a machine that was holding the target.
         *
         * The ledger is one-sided: distance gained by overspeeding is booked,
         * distance lost is not. Booking a shortfall turns into a licence to catch
         * up, holding the opening past the target — and making up lost ground by
         * overspeeding later is not what a speed profile means.
         * └────────────────────────────────────────────────────────────────────────┘
         */
        if (target_abs_ms <= 0.0) {
            // 정지 지시에는 장부가 없습니다. 여기서 뒤처짐을 쌓으면 다시
            // 출발할 때 그만큼 과속합니다.
            // A commanded stop has no ledger; debt banked here would be repaid as
            // overspeed on the way out.
            debtM_ = 0.0;
            gateOpen_ = false;
        } else {
            if (measDtS > 0.0) {
                if (std::fabs(speedErr) > SPEED_DEADBAND) {
                    // 장부는 한쪽만 씁니다. 초과속으로 앞서 나간 거리는 기록하되,
                    // 뒤처진 거리는 쌓지 않습니다. 뒤처짐을 쌓으면 그것이 나중에
                    // "따라잡을 권리"가 되어 목표를 넘겨도 개도를 열어 두게
                    // 되는데, 개도에 권한이 있는 기계에서는 그게 곧바로 릴레이
                    // 진동이 됩니다. 못 간 거리를 나중에 과속으로 메우는 것은
                    // 애초에 속도 프로파일의 뜻이 아니기도 합니다.
                    // The ledger is one-sided: distance gained by overspeeding is
                    // booked, distance lost is not. Booking a shortfall turns into
                    // a licence to catch up, which holds the opening past the
                    // target — and on a machine where the opening has authority
                    // that is immediately a relay oscillation. Making up lost
                    // ground by overspeeding later is not what a speed profile
                    // means anyway.
                    debtM_ = std::clamp(debtM_ + speedErr * measDtS,
                                        -DEBT_CLAMP_M, 0.0);
                } else {
                    // 목표를 지키고 있는 동안은 장부를 잊습니다. 이게 없으면
                    // 잔여 장부가 스위칭 문턱을 밀어 둔 채로 굳어, 목표를 이미
                    // 잘 잡고 있는 기계에서 없어도 될 릴레이 진동을 만듭니다.
                    // The ledger is forgotten while the target is being held.
                    // Without this a residual balance freezes the threshold
                    // off-centre and manufactures a relay oscillation on a
                    // machine that was already holding the target.
                    debtM_ -= debtM_ * std::min(1.0, measDtS / DEBT_LEAK_TAU_S);
                }
            }
            // 문턱은 넉넉한 값에서 시작해 장부만큼 0 쪽으로 당겨집니다. 0 을
            // 넘지는 않습니다 — 목표보다 느린데 개도를 끊는 일은 없어야 합니다.
            //
            // 시작을 넉넉하게 두는 것이 이 변조기를 실기에서 재우는 방법입니다.
            // 지나가는 오버슈트로는 장부가 거의 쌓이지 않아(측정: 0.008 m) 문턱이
            // 그대로 남고, 초과속이 계속되는 기계에서만 장부가 바닥까지 내려가
            // 문턱을 0 으로 끌어당깁니다. 그때부터는 "목표보다 빠르면 개도를
            // 열어 두지 않는다"가 됩니다.
            // The threshold starts generous and the ledger pulls it toward zero,
            // never past it: the opening is never cut while below target.
            //
            // Starting generous is what keeps this modulator asleep on a real
            // machine. A passing overshoot banks almost nothing (0.008 m,
            // measured) so the threshold stays put; only a machine that is
            // persistently over drains the ledger to its clamp and pulls the
            // threshold to zero, at which point the rule becomes "never hold the
            // opening while faster than commanded".
            // ┌─ 한국어 ─────────────────────────────────────────────────────┐
            // 무장 조건: 초과속이 COAST_ARM_S 동안 끊기지 않고 이어질 것.
            //
            // 가속 중에 목표를 지나쳐 가는 것은 초과속이 아니라 과도응답이고,
            // 거기서 개도를 끊으면 잘 굴러가던 비례 제어에 릴레이를 심는
            // 셈입니다. DRAG 플랜트의 기동 오버슈트는 0.6초 지속됩니다.
            // 반대로 단이 넘쳐서 생기는 초과속은 영원히 이어집니다 —
            // 지속시간이 두 경우를 가르는 유일하고 확실한 표식입니다.
            //
            // 한 번 무장하면 주행이 끝날 때까지 유지합니다. 해제 조건을 두면
            // 무장/해제를 반복하는 느린 헌팅이 생기고, 무장 자체는 해롭지
            // 않습니다 — 목표를 잘 지키고 있으면 문턱에 걸리지 않습니다.
            // └──────────────────────────────────────────────────────────────┘
            // ┌─ English ────────────────────────────────────────────────────┐
            // Arming needs COAST_ARM_S of uninterrupted overspeed.
            //
            // Sailing past the target while accelerating is a transient, not an
            // overspeed, and cutting the opening there plants a relay in a
            // proportional loop that was working. The DRAG plant's start-up
            // overshoot lasts 0.6 s. The overspeed a too-fast stage produces
            // lasts forever — duration is the one reliable thing that separates
            // them.
            //
            // Once armed it stays armed for the rest of the run: a disarm
            // condition only produces slow arm/disarm hunting, and being armed
            // is harmless while the target is being held, because then the
            // threshold is never crossed.
            // └──────────────────────────────────────────────────────────────┘
            if (speedErr < -COAST_BASE_MPS) {
                overspeedS_ += measDtS;
                if (overspeedS_ >= COAST_ARM_S) { modulating_ = true; }
            } else {
                overspeedS_ = 0.0;
            }
            const double coastBelowMps =
                    std::min(COAST_SLACK_FRACTION * target_abs_ms,
                             -COAST_BASE_MPS - DEBT_GAIN_PER_S * debtM_);
            // 개도를 끊고 있는 동안에는 마지막으로 값이 움직인 시점부터
            // 추측항법으로 내려서 판단합니다. 되돌리는 쪽에만 씁니다 — 측정
            // 없이 초과속을 추정해 개도를 끊는 것은 위험합니다.
            // While the opening is shut the decision uses a speed dead-reckoned
            // from the last time the value moved. Only in the restoring
            // direction: cutting the opening on an estimate is not a risk worth
            // taking.
            const double gateSpeedMps =
                    (heldGated_ && coastDecelMps2_ > 0.0)
                        ? std::max(0.0, current_abs_ms
                                        - coastDecelMps2_ * speedValueAgeS_)
                        : current_abs_ms;
            gateOpen_ = !modulating_
                     || ((target_abs_ms - gateSpeedMps) >= coastBelowMps);
            heldCoastBelowMps_ = coastBelowMps;
        }
        out.debtM = debtM_;
        out.coastDecelMps2 = coastDecelMps2_;

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
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 안티와인드업이 위쪽에서 적분을 거부했다는 것은 "지금보다 더 열고 싶은데
        // 열 수 없다"는 뜻입니다. 이것이 개도 포화의 정확한 신호입니다 —
        // 출력을 0.999 같은 값과 비교하는 것으로는 잡히지 않습니다. 적분이
        // 1.0 을 넘기 직전에 멈추므로 출력은 1.0 에 닿지 않은 채 수렴합니다.
        //
        // 아래쪽도 대칭입니다. 그리고 아래쪽 포화는 반드시 출력에 반영해야
        // 합니다. 시험 적분이 거부되면 raw 는 거부 직전 값, 즉 0 바로 위에서
        // 멈춥니다. 그것을 그대로 개도로 내보내면 0.001 같은 값이 나가는데,
        // 이 기계에서 0 보다 큰 개도는 100% 와 똑같은 지령입니다(파일 머리말).
        // 즉 "완전히 닫으라"는 결론이 "활짝 열어라"로 뒤집힙니다. 실제로
        // 에뮬레이터에서 4.82 km/h 가 4.00 요청에 그대로 유지된 경로가
        // 이것이었습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Anti-windup refusing the integral on the high side means "I want to
        // open further and cannot". That is the exact saturation signal;
        // comparing the output against something like 0.999 does not catch it,
        // because the integral stops just short of crossing 1.0 and the output
        // converges without ever reaching it.
        //
        // The low side is symmetric, and there the saturation has to reach the
        // output. When the trial is refused, raw holds its pre-refusal value —
        // just above zero — and emitting that as the opening sends something like
        // 0.001, which on this machine is the same command as 100% (see the file
        // header). A conclusion of "close completely" would be applied as "open
        // fully". That is exactly the path by which the emulator held 4.82 km/h
        // against a 4.00 request.
        // └────────────────────────────────────────────────────────────────────┘
        bool demandExceedsOpening = false;
        bool demandBelowClosed = false;
        if (std::fabs(speedErr) <= SPEED_DEADBAND || !gateOpen_) {
            // 변조기가 개도를 끊고 있는 동안은 적분도 멈춥니다. 실제로 나가지
            // 않는 지령에 대해 적분하는 것은 와인드업입니다.
            // The integral stops while the modulator holds the opening shut:
            // integrating against a command that is not being applied is windup.
            raw = ff + out.pTerm + ki_ * integral_ + out.dTerm;
        } else {
            const double integralTrial = integral_ + speedErr * measDtS;
            const double trialRaw = ff + out.pTerm + ki_ * integralTrial + out.dTerm;
            if (trialRaw >= 0.0 && trialRaw <= 1.0) {
                integral_ = integralTrial;
                raw = trialRaw;
            } else {
                raw = ff + out.pTerm + ki_ * integral_ + out.dTerm;
                demandExceedsOpening = (trialRaw > 1.0);
                demandBelowClosed = (trialRaw < 0.0);
            }
        }
        out.iTerm = ki_ * integral_;

        /*
         * ┌─ 한국어: 감속은 코스팅으로, 브레이크는 정지에만 ──────────────────────┐
         *
         * 이 기계에는 비례 브레이크 채널이 없습니다. TrackingLoop::act() 는
         * brake > 0 을 "ACC 개도 0 + 다운시프트"로 바꿔 보냅니다. 그래서 0.13 을
         * 내보내든 0.23 을 내보내든 물리적 결과는 같고, 게다가 단까지 하나
         * 내려갑니다.
         *
         * 감속에 필요한 것은 개도 0 뿐입니다 — 그것만으로 약 0.47 m/s^2 로
         * 줄어듭니다(에뮬레이터 실측: 개도 0 에서 5.19 -> 1.67 km/h 를 2.1초에).
         * 브레이크가 얹는 다운시프트는 해롭습니다. 지금 물린 단은 목표를 낼 수
         * 있는 가장 낮은 단인데, 한 단 내려가면 그 단으로는 목표를 못 내고
         * 다시 올라오는 데 GEAR_DWELL_S 가 걸립니다. 예전 로그에서 4.0 -> 2.4
         * km/h 지점마다 완전 정지가 재현된 것이 이것입니다.
         *
         *     t=17.6  v=1.08  thr=0.00  brk=0.23  gear=2
         *     t=18.2  v=1.08  thr=0.00  brk=0.23  gear=1
         *     t=19.7  v=0.00  thr=1.00  brk=0.00  gear=1   <- 완전 정지
         *
         * 그래서 0 이 아닌 속도가 지시된 동안에는 브레이크를 내보내지 않고
         * 코스팅만 합니다. 얼마나 오래 코스팅할지는 위의 거리 장부가 정하므로,
         * 예전처럼 "얼마를 넘으면 브레이크"라는 문턱을 둘 필요가 없어졌습니다 —
         * 장부는 초과속의 크기가 아니라 누적량을 보고, 갚는 즉시 개도를 돌려
         * 놓습니다.
         *
         * 브레이크는 진짜로 세워야 할 때만 남습니다: 목표 0, 그리고 전후진
         * 전환(brakingForReversal_, 위쪽에서 따로 처리).
         * └────────────────────────────────────────────────────────────────────┘
         * ┌─ English: coast to slow down, brake only to stop ──────────────────┐
         *
         * There is no proportional brake channel. TrackingLoop::act() turns any
         * brake > 0 into "accelerator opening 0 plus a downshift", so 0.13 and
         * 0.23 have identical physical effect — and both drop a stage.
         *
         * Slowing down needs only the zero opening: that alone sheds about
         * 0.47 m/s^2 (measured on the emulator, 5.19 -> 1.67 km/h in 2.1 s at
         * zero opening). The downshift the brake adds is harmful. The engaged
         * stage is the lowest one that can make the target, so one stage down
         * cannot make it at all and climbing back costs GEAR_DWELL_S. That is
         * what produced a dead stop at every 4.0 -> 2.4 km/h step in the old
         * logs:
         *
         *     t=17.6  v=1.08  thr=0.00  brk=0.23  gear=2
         *     t=18.2  v=1.08  thr=0.00  brk=0.23  gear=1
         *     t=19.7  v=0.00  thr=1.00  brk=0.00  gear=1   <- full stop
         *
         * So while a non-zero speed is commanded this coasts and never brakes.
         * How long to coast is decided by the distance ledger above, which is
         * why the old "excess beyond one gear step" threshold is gone: the
         * ledger watches accumulated error rather than instantaneous excess and
         * restores the opening the moment it is repaid.
         *
         * The brake is kept for a real stop (target 0) and for the direction
         * reversal handled above.
         * └────────────────────────────────────────────────────────────────────┘
         */
        if (target_abs_ms > 0.0 && (!gateOpen_ || raw < 0.0 || demandBelowClosed)) {
            // 코스팅. 개도만 끊고 단은 그대로 둡니다 — 지금 물린 단은 목표를
            // 낼 수 있는 가장 낮은 단이므로, 여기서 내리면 다시 못 올라옵니다.
            // Coast: cut the opening, keep the stage. The engaged stage is the
            // lowest one that can make the target, and dropping it here would
            // give up a speed we then cannot get back.
            out.throttle = 0.0;
            out.brake = 0.0;
            out.throttleGated = true;
        } else if (raw >= 0.0) {
            out.throttle = std::clamp(raw, 0.0, 1.0);
            out.brake = 0.0;
        } else {
            // 목표가 0일 때만 남는 분기입니다. 여기서만 브레이크를 씁니다 —
            // TrackingLoop 은 brake > 0 을 다운시프트로도 해석하므로, 통상
            // 감속에 쓰면 낼 수 있는 속도를 스스로 버리게 됩니다.
            // Only reachable with a zero target, and the only place the brake is
            // used: TrackingLoop also reads brake > 0 as a downshift, so using it
            // for a routine slowdown throws away a speed the machine had.
            out.throttle = 0.0;
            out.brake = std::min(1.0, KP_BRAKE * (-raw));
        }
        heldThrottle_ = out.throttle;
        heldBrake_ = out.brake;
        heldGated_ = out.throttleGated;
        if (!out.throttleGated && out.throttle > 0.0) {
            heldOpenThrottle_ = out.throttle;
        }

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 변조 중에는 단에 대해 아무것도 배우지 않습니다. 개도를 끊었다 열었다
        // 하는 동안의 속도는 그 단의 능력이 아니라 듀티비의 결과입니다. 이걸
        // 학습으로 받으면 단의 능력을 실제보다 낮게 기억하고(예: 1단 1.48 을
        // 1.34 로), 다음에 그 단으로 낼 수 있는 목표에도 위로 올라가게 됩니다.
        //
        // 학습이 정말 필요한 경우는 반대쪽입니다 — 개도를 끝까지 열었는데도
        // 목표에 못 미칠 때. 그때는 변조가 걸리지 않으므로 이 조건에 걸리지
        // 않습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Nothing is learned about a stage while it is being modulated: the speed
        // under a pulsing opening is the duty cycle's result, not the stage's
        // capability. Taking it as evidence would remember the stage as slower
        // than it is (1.48 as 1.34 for stage 1) and then climb away from targets
        // that stage could hold.
        //
        // Learning matters in the opposite case — wide open and still short — and
        // there the modulator is not engaged, so this never suppresses it.
        // └────────────────────────────────────────────────────────────────────┘
        if (out.throttleGated) {
            abandonStageObservation();
        } else {
            observeStage(out.gear, target_abs_ms, current_abs_ms,
                         demandExceedsOpening || out.throttle >= SATURATED_OPENING,
                         out.brake, measDtS);
        }
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
        const double magnitudeKmh = std::fabs(targetSpeedKmh);
        // 목표를 낼 수 있는 가장 낮은 단. 어느 단도 못 내면 최상단으로 최선을
        // 다합니다 — 낼 수 없는 속도를 낮은 단에서 기다리는 것보다 낫습니다.
        // The lowest stage that can deliver the target; if none can, the top
        // stage does its best, which beats waiting for it in a low one.
        for (int stage = 1; stage < maxForwardGear_; ++stage) {
            if (magnitudeKmh <= sustainedKmh(stage)) return stage;
        }
        return maxForwardGear_;
    }

    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 이 단이 유지할 수 있다고 보는 속도(km/h)입니다. 실측이 있으면 실측을,
     * 없으면 예전 하드코딩 표를 사전값으로 씁니다.
     *
     * 사전값을 예전 임계값(3.0 / 6.0)과 똑같이 두는 것이 중요합니다. 아무것도
     * 학습하지 않은 상태의 단 선택이 예전 gearForSpeed 와 동일해지므로, 이
     * 변경은 "측정이 사전값과 어긋났을 때"에만 동작을 바꿉니다. 최상단에는
     * 상한이 없습니다 — 그 위로 올라갈 곳이 없으므로 상한이 의미가 없습니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * The speed this stage is believed to sustain, in km/h: the measurement when
     * there is one, otherwise the old hard-coded table as a prior.
     *
     * Keeping the priors equal to the old thresholds (3.0 / 6.0) matters: with
     * nothing learned, stage selection is exactly the old gearForSpeed, so this
     * change only alters behaviour where a measurement contradicts a prior. The
     * top stage is unbounded — there is nothing above it for a bound to select.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    double sustainedKmh(int stage) const
    {
        if (stage < 1 || stage > MAX_STAGES) return UNBOUNDED_KMH;
        if (stageMeasured_[stage]) return stageKmh_[stage];
        if (stage >= maxForwardGear_) return UNBOUNDED_KMH;
        return PRIOR_STAGE_KMH[stage];
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
     * 이 조합에서 가장 느린 단이 내는 속도입니다. 1단이 실제로 낸 속도를 알고
     * 있으면 그것을, 모르면 위의 상수를 씁니다.
     *
     * 이제 목표를 여기까지 "올리는" 데는 쓰지 않습니다 — 이보다 느린 목표는
     * 1단 + 개도 변조로 정확히 만들어 냅니다. 남은 용도는 두 가지입니다.
     * (1) 요청이 가장 느린 단보다도 느린가(belowSlowestStage) 를 텔레메트리로
     * 알리는 것, (2) 그 상태에서는 개도가 반드시 끊겼다 열렸다 한다는 사실을
     * 대시보드에서 설명할 수 있게 하는 것.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * What the slowest stage delivers on this pairing: stage 1's measured speed
     * once it is known, the constant above until then.
     *
     * It no longer raises the target. A request below it is delivered exactly, by
     * stage 1 plus modulation of the opening. Two uses remain: reporting whether
     * the request is slower than the slowest stage (belowSlowestStage), and giving
     * the dashboard the number that explains why the opening must pulse there.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    double machineFloorKmh() const
    {
        return stageMeasured_[1] ? stageKmh_[1] : MIN_MACHINE_SPEED_KMH;
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
    /**
     * ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
     * 지금 물려 있는 단이 실제로 무엇을 해냈는지 기록합니다. 규칙은 둘뿐이고,
     * 서로 반대 방향으로만 움직입니다.
     *
     *   올림: 이 단에서 지금까지 본 것보다 빠르게 달리고 있다면, 그 단은
     *         적어도 그만큼은 낼 수 있습니다. 개도가 얼마든 상관없습니다.
     *
     *   내림: 개도가 끝까지 열렸는데도 목표에 못 미치는 상태가
     *         SHORTFALL_HOLD_S 동안 이어지면, 그것이 이 단의 천장입니다.
     *         한 번 이렇게 확정되면 다음 틱에 gearForSpeed 가 윗단을 고릅니다.
     *
     * 개도가 포화되지 않은 채 목표보다 느린 것은 그냥 PID 가 덜 연 것이지
     * 단의 한계가 아니므로 내리지 않습니다. 이 비대칭이 핵심입니다.
     *
     * 변속 직후 STAGE_SETTLE_S 동안은 아무것도 믿지 않습니다 — 구동계가
     * 따라오는 중이라 그 속도는 어느 단의 능력도 아닙니다.
     * └────────────────────────────────────────────────────────────────────────┘
     * ┌─ English ──────────────────────────────────────────────────────────────┐
     * Records what the engaged stage actually achieved. Two rules only, and they
     * move in opposite directions:
     *
     *   raise: running faster than anything seen in this stage proves the stage
     *          can do at least that, whatever the opening happens to be.
     *
     *   lower: the opening pinned wide open while still short of the target for
     *          SHORTFALL_HOLD_S is this stage's ceiling. Once recorded,
     *          gearForSpeed picks the stage above it on the next tick.
     *
     * Being slower than the target with the opening *not* saturated only means
     * the PID has not opened it, not that the stage cannot, so it never lowers.
     * That asymmetry is the whole idea.
     *
     * Nothing is trusted for STAGE_SETTLE_S after a shift: the drivetrain is
     * still catching up and that speed belongs to no stage.
     * └────────────────────────────────────────────────────────────────────────┘
     */
    void observeStage(int gear, double targetMps, double measuredMps,
                      bool wideOpen, double brake, double measDtS)
    {
        if (gear < 1 || gear > MAX_STAGES || measDtS <= 0.0) {
            abandonStageObservation();
            return;
        }
        if (gear != learnStage_) {
            learnStage_ = gear;
            stageEngagedS_ = 0.0;
            shortfallS_ = 0.0;
        }
        stageEngagedS_ += measDtS;
        if (stageEngagedS_ < STAGE_SETTLE_S) return;
        // 제동 중에는 속도가 단의 능력이 아니라 감속의 결과입니다.
        // While braking the speed is the deceleration, not the stage.
        if (brake > 0.0) {
            shortfallS_ = 0.0;
            return;
        }

        const double measuredKmh = measuredMps / KMH_TO_MS;
        if (measuredKmh > sustainedKmh(gear)) {
            stageKmh_[gear] = measuredKmh;
            stageMeasured_[gear] = true;
        }

        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 아직 가속 중인 것을 천장으로 착각하면 안 됩니다. 부하가 큰 기계는
        // 목표에 도달하는 데 SHORTFALL_HOLD_S 보다 오래 걸릴 수 있고, 그동안
        // 개도는 당연히 끝까지 열려 있습니다. dFiltered_ 는 오차의 미분이므로,
        // 오차가 아직 줄고 있으면(음수) 이 단은 아직 자기 능력을 다 보여준
        // 것이 아닙니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Still accelerating is not a ceiling. A heavily loaded machine can take
        // longer than SHORTFALL_HOLD_S to reach the target, with the opening
        // legitimately wide open the whole way. dFiltered_ is the derivative of
        // the error, so while the error is still shrinking (negative) this stage
        // has not yet shown what it can do.
        // └────────────────────────────────────────────────────────────────────┘
        const bool short_ = measuredMps < targetMps - SHORTFALL_MPS;
        const bool stillClosing = dFiltered_ <= -STILL_CLOSING_MPS2;
        if (wideOpen && short_ && !stillClosing) {
            shortfallS_ += measDtS;
            if (shortfallS_ >= SHORTFALL_HOLD_S) {
                stageKmh_[gear] = measuredKmh;
                stageMeasured_[gear] = true;
                shortfallS_ = 0.0;
            }
        } else {
            shortfallS_ = 0.0;
        }
    }

    /** 관측을 버리고 정착 시간을 다시 셉니다. / Drop the observation, re-settle. */
    void abandonStageObservation()
    {
        learnStage_ = 0;
        stageEngagedS_ = 0.0;
        shortfallS_ = 0.0;
    }

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
    // KI 는 0.05 에서 0.5 로 올렸습니다. 0.05 는 사실상 적분이 없는 것과
    // 같았습니다 — 개도가 속도로 적분되는 계에서 PI 제어의 특성근은
    // wn = sqrt(g*ki), zeta = g*kp/(2*wn) 이고, kp=1.5 / ki=0.05 이면
    // zeta 가 5.8 로 과도하게 과감쇠라 정상상태 오차를 없애는 데 100초 가까이
    // 걸립니다. 단위테스트 플랜트에서 0.113 m/s (0.4 km/h) 의 초과속이 180초
    // 주행 내내 사라지지 않는 것으로 실측됩니다. 경로 한 구간이 그보다 짧으니
    // 실질적으로 적분은 한 번도 일한 적이 없었습니다.
    //
    // 0.5 에서는 zeta 가 1.8 로 여전히 과감쇠이면서 정착이 1.3초입니다. 개도에
    // 권한이 없는 에뮬레이터에서도 같은 이유로 중요합니다: 초과속일 때 개도를
    // 닫는 일을 하는 것이 적분이고, 0.05 로는 15초, 0.5 로는 1.5초가 걸립니다.
    // 감속 구간에서 적분이 남는 문제는 조건부 적분(안티와인드업)이 막습니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Gains kept conservative to avoid overshoot oscillation: the vehicle has
    // dynamics lag (mass + drivetrain), and high KP makes throttle/brake swing
    // past the target. 1.5 is the empirically stable value; the tight deadband
    // retains steady-state precision without inducing dither.
    //
    // KI was raised from 0.05 to 0.5. At 0.05 there was effectively no integral
    // action at all: opening integrates into speed, so PI control has
    // wn = sqrt(g*ki) and zeta = g*kp/(2*wn), and kp = 1.5 with ki = 0.05 gives
    // zeta = 5.8 — so overdamped that clearing a steady-state error takes nearly
    // 100 s. Measured on the unit-test plant, a 0.113 m/s (0.4 km/h) overspeed
    // survived an entire 180 s run. Route segments are shorter than that, so in
    // practice the integral never did any work.
    //
    // At 0.5, zeta is 1.8 — still overdamped — and settling is 1.3 s. It matters
    // for the same reason on the emulator, where the opening has no authority:
    // closing the opening under overspeed is the integral's job, and it took 15 s
    // at 0.05 against 1.5 s at 0.5. Integral left over after a slow section is
    // prevented by the conditional integration (anti-windup), not by a small gain.
    // └────────────────────────────────────────────────────────────────────────┘
    // 튜닝 UI가 setGains()로 런타임에 바꿀 수 있도록 constexpr 에서 일반 멤버가
    // 됐습니다 (TrackerParams 참고). 기본값은 기존 상수와 동일합니다.
    // Plain members (formerly constexpr) so the tuning UI can change them at
    // runtime via setGains(); defaults equal the old constants.
    double kp_ = 1.5;
    double ki_ = 0.5;
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
    // 장부가 비어 있을 때 코스팅을 시작하는 초과속. 이보다 작은 초과속은
    // 개도 PID 가 알아서 처리하게 둡니다.
    // Overspeed at which coasting starts with an empty ledger; anything smaller
    // is left to the opening PID.
    static constexpr double COAST_BASE_MPS = 0.10;  // m/s
    // 초과속이 이만큼 끊기지 않고 이어져야 변조기가 무장합니다.
    // Uninterrupted overspeed needed before the modulator arms.
    static constexpr double COAST_ARM_S = 2.0;      // s
    // 문턱이 0 을 넘어 목표 아래까지 갈 수 있는 폭. 목표속도에 비례합니다 —
    // 빠른 구간에서는 내줄 속도가 있어 평균을 끌어내리는 데 쓸 수 있고, 느린
    // 구간에서는 같은 폭이 곧 정지라 좁아야 합니다.
    // How far past zero the threshold may reach, as a fraction of the target. It
    // scales with speed: a fast section has speed to give away for the mean, while
    // in a slow one the same margin would be a standstill.
    static constexpr double COAST_SLACK_FRACTION = 0.25;
    // 코스팅 감속 추정의 허용 범위와 시정수. 범위는 실측(compact 0.47,
    // medium+쟁기 1.7)을 넉넉히 감싸고, alpha 는 몇 번의 관측으로 수렴하되
    // 한 번의 이상치에 끌려가지 않을 만큼입니다.
    // Bounds and time constant for the coast estimate. The bounds comfortably
    // bracket what was measured (0.47 for the compact, 1.7 for the medium with a
    // plough); alpha converges in a few observations without chasing one outlier.
    static constexpr double MIN_COAST_MPS2 = 0.1;
    static constexpr double MAX_COAST_MPS2 = 5.0;
    static constexpr double COAST_LEARN_ALPHA = 0.3;
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

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 단 학습 상수.
    //
    // Send_SFT_CMD 가 Low/Middle/High 셋만 받으므로 단은 최대 3입니다.
    //
    // 사전값은 예전 gearForSpeed 의 임계값(3.0 / 6.0 km/h)과 같습니다. 학습 전
    // 단 선택이 예전과 동일하려면 반드시 같아야 합니다.
    //
    // STAGE_SETTLE_S 는 에뮬레이터에서 변속 후 속도가 새 값에 자리잡는 데
    // 걸린 시간(약 1.5초)보다 넉넉하게 잡았습니다. SHORTFALL_HOLD_S 는 그 위에
    // 한 번 더 확인하는 시간이라, 잘못된 확정까지 최소 4초가 필요합니다 —
    // 흙에 걸려 한 번 느려진 것으로 단 천장을 낮춰 쓰면 안 되니까요.
    //
    // SHORTFALL_MPS 는 GPS 속도의 이산 간격(약 0.05 m/s)보다 크게 잡아,
    // 양자화 잡음이 부족으로 오인되지 않게 합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Stage-learning constants.
    //
    // Three stages, because Send_SFT_CMD only accepts Low/Middle/High.
    //
    // The priors equal the old gearForSpeed thresholds (3.0 / 6.0 km/h); they
    // have to, for pre-learning selection to match the old behaviour exactly.
    //
    // STAGE_SETTLE_S is comfortably longer than the ~1.5 s the emulator takes to
    // settle on a stage's speed after a shift, and SHORTFALL_HOLD_S confirms on
    // top of that, so a wrong verdict needs at least 4 s of evidence — one patch
    // of heavy soil must not permanently lower a stage's ceiling.
    //
    // SHORTFALL_MPS sits above the GPS speed quantum (~0.05 m/s) so quantization
    // noise is never read as a shortfall.
    // └────────────────────────────────────────────────────────────────────────┘
    static constexpr int MAX_STAGES = 3;
    static constexpr double PRIOR_STAGE_KMH[MAX_STAGES + 1] = {0.0, 3.0, 6.0, 0.0};
    static constexpr double UNBOUNDED_KMH = 1.0e9;
    static constexpr double STAGE_SETTLE_S = 2.0;      // s
    static constexpr double SHORTFALL_HOLD_S = 2.0;    // s
    static constexpr double SATURATED_OPENING = 0.999;  // 0..1
    static constexpr double SHORTFALL_MPS = 0.10;      // m/s
    static constexpr double STILL_CLOSING_MPS2 = 0.05;  // m/s^2

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 개도 변조기 상수.
    //
    // DEBT_CLAMP_M 은 장부의 기억 길이입니다. 4 km/h 에서 0.5 m 는 0.45초치라,
    // 한 웨이포인트짜리 속도 튐(이 RDDF 에는 65개 구간 중 12개가 웨이포인트
    // 하나뿐입니다)이나 잠깐의 권한 상실이 나중의 긴 과속으로 바뀌지 않습니다.
    //
    // DEBT_LEAK_TAU_S 는 목표를 불감대 안으로 지키고 있는 동안 장부를 잊는
    // 속도입니다. 이 변조기를 필요 없는 기계에서 재우는 장치이기도 합니다 —
    // 거기서는 오차가 불감대 안에 자리잡으므로 잔액이 0으로 빠지고 문턱이
    // 가운데로 돌아옵니다. 다만 변조 한 주기보다는 충분히 길어야 합니다.
    // 변조 중인 기계는 매 주기 불감대를 지나가므로, 짧게 잡으면 끊는 사이사이에
    // 잔액이 빠져 보정이 모자랍니다: 2초에서는 compact 조합이 4.00 요청에
    // 4.16 km/h 에 머물렀고, 10초에서는 목표에 닿습니다.
    //
    // DEBT_GAIN_PER_S 는 장부를 스위칭 문턱으로 바꾸는 환산 계수입니다(1/s).
    // 클램프와 곱해서 0.25 m/s — 실측 조합에서 단이 넘치는 양(medium 3단은
    // 4.82 vs 4.00 = 0.23 m/s)과 같은 규모라, 문턱을 반대쪽 끝까지 밀 수 있습니다.
    //
    // 문턱이 0 을 넘어갈 수 있는 이유: 장부가 차면 목표보다 조금 느린 지점에서도
    // 개도를 끊어야 평균이 목표까지 내려옵니다. 한 번 열면 다음 판단까지
    // 그만큼을 넘겨 버리기 때문입니다. 그 여유의 폭은 COAST_SLACK_FRACTION 이
    // 정합니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Modulator constants.
    //
    // DEBT_CLAMP_M is how long the ledger remembers. At 4 km/h, 0.5 m is 0.45 s,
    // short enough that a one-waypoint speed blip (12 of this RDDF's 65 segments
    // are a single waypoint) or a brief loss of authority cannot turn into a long
    // overspeed later.
    //
    // DEBT_GAIN_PER_S converts the ledger into a switching threshold (1/s). Times
    // the clamp it is 0.25 m/s — the same size as a stage's excess on the measured
    // pairings (stage 3 on the medium is 4.82 against 4.00, i.e. 0.23 m/s) — so it
    // can drive the threshold to the far side.
    //
    // The threshold is allowed past zero because with a full ledger the opening has
    // to be cut slightly below target for the mean to come down — reopening gains
    // speed before the next decision. COAST_SLACK_FRACTION bounds that margin.
    //
    // DEBT_LEAK_TAU_S is how fast the ledger is forgotten while the target is
    // being held to inside the deadband. It is what keeps this off a machine that
    // does not need it: there the error settles inside the deadband, the balance
    // decays to zero, the threshold recentres and the relay never fires again.
    //
    // It has to be long compared with one modulation cycle. A modulating machine
    // crosses the deadband on every cycle, so a short leak drains the balance
    // between cuts and caps the trim short of what it needs: at 2 s the compact
    // pairing settled at 4.16 km/h against a 4.00 request, at 10 s it reaches it.
    // └────────────────────────────────────────────────────────────────────────┘
    static constexpr double DEBT_CLAMP_M = 0.5;       // m
    static constexpr double DEBT_GAIN_PER_S = 0.5;   // 1/s
    static constexpr double DEBT_LEAK_TAU_S = 10.0;  // s

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
    bool heldGated_ = false;
    double heldOpenThrottle_ = 0.0;   // 되돌릴 개도 / opening to restore
    double heldCoastBelowMps_ = 0.0;  // 마지막 스위칭 문턱 / last switching threshold
    double lastMeasuredAbsSpeed_ = 0.0;
    double lastChangedAbsSpeed_ = 0.0;  // 값이 마지막으로 움직인 속도 / speed when the value last moved
    double speedValueAgeS_ = 0.0;       // 그 뒤로 흐른 시간 / seconds since then
    double coastDecelMps2_ = 0.0;     // 실측 코스팅 감속 / measured coast deceleration

    // 개도 변조기 상태. / Opening-modulator state.
    double debtM_ = 0.0;      // 계획 거리 - 실제 거리 / planned minus travelled, metres
    double overspeedS_ = 0.0;  // 초과속이 이어진 시간 / how long overspeed has held
    bool modulating_ = false;  // 개도에 권한이 없다고 판정했는가 / opening judged to have no authority
    bool gateOpen_ = true;    // 개도를 내보내도 되는가 / may the opening be applied

    // 변속 히스테리시스 상태. / Gear hysteresis state.
    int curGear_;        // 현재 물려 있는 단 / stage currently engaged
    int pendingGear_;    // 요구되고 있는 단 / stage being requested
    double gearDwellS_;  // 그 요구가 이어진 시간 / how long that request has held
    bool haveGear_;
    int maxForwardGear_ = MAX_STAGES;

    // 단별 학습 상태. 인덱스 0은 쓰지 않습니다(단 번호가 1부터라 그대로 씁니다).
    // Per-stage learning. Index 0 is unused so the stage number indexes directly.
    double stageKmh_[MAX_STAGES + 1];
    bool stageMeasured_[MAX_STAGES + 1];
    int learnStage_;       // 관측 중인 단 / stage being observed
    double stageEngagedS_; // 그 단이 물린 뒤 흐른 시간 / time engaged
    double shortfallS_;    // 개도 포화 + 목표 미달이 이어진 시간 / saturated-and-short
};

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_SPEEDCONTROLLER_HPP
