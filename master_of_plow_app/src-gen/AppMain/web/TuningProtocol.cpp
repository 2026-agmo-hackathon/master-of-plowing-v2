/*
 * AppMain/web/TuningProtocol.cpp
 *
 * 프로토콜 설명은 TuningProtocol.hpp 상단을 참고하세요.
 * See the top of TuningProtocol.hpp for the protocol description.
 */

#include <AppMain/web/TuningProtocol.hpp>

#include <AppMain/MainController.hpp>
#include <AppMain/tracking/TrackerFactory.hpp>
#include <AppMain/tracking/TrackerSwitch.hpp>
#include <AppMain/tracking/TrackerTypes.hpp>
#include <AppMain/web/WebSocketEndPoint.hpp>

#include <algorithm>
#include <iostream>
#include <string>

namespace AppMain
{
namespace web
{

namespace
{

using ::AppMain::tracking::TrackerParams;

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 파라미터 필드 하나 = 표 한 줄. JSON 키, 멤버 포인터, 허용 범위를 한 곳에
// 묶어 두면 부분 업데이트·clamp·tuning_state 직렬화가 전부 이 표 하나를
// 돌면서 끝납니다. 필드를 추가할 때 이 표에 한 줄만 더하면 됩니다.
//
// 범위는 "튜닝으로 의미가 있는" 영역이 아니라 "이 밖이면 확실히 위험한"
// 영역입니다. 음수 게인은 부호 반전(폭주), 0 lookahead 는 0 나눗셈에 준하는
// 기하 붕괴를 만듭니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// One row per parameter: JSON key, member pointer and allowed range in one
// table drives the partial update, the clamping and the tuning_state
// serialisation. The ranges mark "definitely dangerous beyond here", not
// "useful tuning range": a negative gain inverts the sign of the correction
// (runaway), a zero lookahead collapses the pursuit geometry.
// └────────────────────────────────────────────────────────────────────────────┘
struct ParamField
{
    const char* key;
    double TrackerParams::* member;
    double min;
    double max;
};

const ParamField PARAM_FIELDS[] = {
    // Pure Pursuit
    { "ppLookaheadMinM",  &TrackerParams::ppLookaheadMinM,  0.5,  30.0  },
    { "ppLookaheadMaxM",  &TrackerParams::ppLookaheadMaxM,  0.5,  30.0  },
    { "ppLookaheadTimeS", &TrackerParams::ppLookaheadTimeS, 0.0,  10.0  },
    { "ppKLtd",           &TrackerParams::ppKLtd,           0.0,  5.0   },
    { "ppKHde",           &TrackerParams::ppKHde,           0.0,  5.0   },
    { "ppWeightPp",       &TrackerParams::ppWeightPp,       0.0,  1.0   },
    { "ppKYawDamp",       &TrackerParams::ppKYawDamp,       0.0,  10.0  },
    // 0 이면 정지 상태에서 LTD 항 분모가 0 이 됩니다. stSofteningMps 와 같은 이유.
    // A zero softening divides by zero at rest, same as stSofteningMps.
    { "ppLtdSofteningMps", &TrackerParams::ppLtdSofteningMps, 0.05, 5.0 },
    // Stanley
    { "stKLtd",           &TrackerParams::stKLtd,           0.0,  5.0   },
    // 0 이면 정지 상태에서 atan 항 분모가 0 이 됩니다. 하한을 양수로 둡니다.
    // A zero softening divides by zero at rest; the floor stays positive.
    { "stSofteningMps",   &TrackerParams::stSofteningMps,   0.05, 5.0   },
    // 조향 PID — 오차 단위는 도(degree). 기본 KP=50 스케일 기준의 상한입니다.
    // Steering PID on a DEGREE error; caps sized to the KP=50 default scale.
    { "steerKp",          &TrackerParams::steerKp,          0.0,  300.0 },
    { "steerKi",          &TrackerParams::steerKi,          0.0,  10.0  },
    { "steerKd",          &TrackerParams::steerKd,          0.0,  50.0  },
    // 종방향 PID / Longitudinal PID
    { "longKp",           &TrackerParams::longKp,           0.0,  10.0  },
    { "longKi",           &TrackerParams::longKi,           0.0,  2.0   },
    { "longKd",           &TrackerParams::longKd,           0.0,  5.0   },
};

double clampField(const ParamField& f, double v)
{
    return std::clamp(v, f.min, f.max);
}

} // namespace

void broadcastTuningState(::AppMain::MainController& controller)
{
    auto ws = WebSocketEndPoint::getInstance();
    if (!ws)
    {
        return;
    }

    const TrackerParams params = controller.getTrackerParams();

    Json::Value msg;
    msg["kind"] = "tuning_state";
    msg["tracker"] = controller.getTrackerName();

    Json::Value available(Json::arrayValue);
    for (const std::string& name : ::AppMain::tracking::availableTrackerNames())
    {
        available.append(name);
    }
    msg["available"] = available;

    Json::Value p(Json::objectValue);
    for (const ParamField& f : PARAM_FIELDS)
    {
        p[f.key] = params.*(f.member);
    }
    msg["params"] = p;

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 주행 상태를 같은 프레임에 실어 보냅니다. UI 가 파라미터 잠금과 시작 버튼
    // 활성 여부를 이 하나로 판단하게 하려는 것입니다 — 상태를 두 프레임으로
    // 나누면 순서에 따라 "잠겼는데 스테퍼가 살아 있는" 창이 생깁니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // The run state rides in the same frame so the UI decides the parameter lock
    // and the start button from one source. Splitting it across two frames would
    // open a window where the run is locked but the steppers are still live.
    // └────────────────────────────────────────────────────────────────────────┘
    const ::AppMain::RunState runState = controller.getRunState();
    msg["runState"] = ::AppMain::runStateName(runState);
    msg["paramsLocked"] = ::AppMain::isRunLocked(runState);
    msg["readyToStart"] = controller.isReadyToStart();
    msg["waypointCount"] = static_cast< int >(
            controller.getTrackedWaypoints().size());

    Json::StreamWriterBuilder wb;
    wb["indentation"] = "";
    ws->publishMessage(Json::writeString(wb, msg));
}

bool handleTuningMessage(const Json::Value& msg,
                         ::AppMain::MainController& controller)
{
    if (!msg.isObject() || !msg["kind"].isString())
    {
        return false;
    }
    const std::string kind = msg["kind"].asString();

    if (kind == "get_tuning_state")
    {
        broadcastTuningState(controller);
        return true;
    }

    if (kind == "set_run_state")
    {
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 이전 UI 호환용 종료만 허용합니다. 시작은 오케스트레이션 트랜잭션이
        // 소유하고, 일시정지·재개는 v1.1.20 UI 계약에 없습니다.
        //
        // 성공/실패와 무관하게 tuning_state 를 방송합니다 — UI 는 자기가 보낸
        // 지시를 낙관적으로 반영하지 않고, 되돌아온 상태만 믿습니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // Only the legacy stop is retained as a safety-compatible alias. Start
        // belongs to the orchestration transaction; pause/resume are not in the
        // v1.1.20 UI contract.
        // tuning_state is broadcast either way: the UI never applies its own
        // request optimistically, it trusts only what comes back.
        // └────────────────────────────────────────────────────────────────────┘
        const std::string action = msg["action"].isString()
                ? msg["action"].asString() : std::string();
        if (action == "stop")
        {
            // Backward-compatible safety action only. Start is intentionally
            // owned by the staged orchestration transaction.
            controller.requestOrchestrationFinish(false);
        }
        else
        {
            std::cerr << "[TuningProtocol] unknown run action '" << action << "'\n";
        }
        broadcastTuningState(controller);
        return true;
    }

    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 주행 중(Running/Paused)에는 파라미터와 알고리즘을 잠급니다.
    //
    // UI 에서 스테퍼를 비활성화하는 것만으로는 부족합니다. UI 는 낡은 상태를
    // 들고 있을 수 있고, 다른 클라이언트가 붙을 수도 있습니다. 권위는 여기입니다.
    //
    // 왜 잠그는가: 주행 도중 게인이 바뀌면 거동이 갑자기 달라지고, 나중에
    // "어떤 설정으로 달린 결과인지"를 알 수 없게 됩니다. Paused 도 포함하는
    // 이유는 잠시 멈춘 것이지 회차가 끝난 게 아니기 때문입니다.
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // While a run is in progress (Running/Paused) the parameters and the algorithm
    // are locked. Disabling the steppers in the UI is not enough — a UI can hold
    // stale state and another client can connect; the authority is here.
    //
    // Why lock: a mid-run gain change alters the behaviour abruptly and destroys
    // the record of which setting produced the result. Paused counts because it is
    // a pause, not an end.
    // └────────────────────────────────────────────────────────────────────────┘
    if ((kind == "set_tracker" || kind == "set_params")
            && controller.areParamsLocked())
    {
        std::cerr << "[TuningProtocol] rejected '" << kind
                  << "' — a run is in progress\n";
        broadcastTuningState(controller);
        return true;
    }

    if (kind == "set_tracker")
    {
        // 이름 검증은 switchTracker 가 availableTrackerNames() 로 수행합니다.
        // 모르는 이름은 무시되지만, 그래도 tuning_state 를 방송해 UI 가
        // "적용되지 않았음"을 현재 상태로 확인할 수 있게 합니다.
        // switchTracker validates against availableTrackerNames(); unknown
        // names are ignored, but tuning_state is still broadcast so the UI
        // sees that nothing changed.
        if (msg["name"].isString())
        {
            ::AppMain::tracking::switchTracker(controller,
                    msg["name"].asString());
        }
        broadcastTuningState(controller);
        return true;
    }

    if (kind == "set_params")
    {
        // 부분 업데이트: 프레임에 존재하는 숫자 필드만 반영합니다. UI 가 값
        // 하나를 바꿀 때 전체를 보낼 필요가 없습니다.
        // Partial update: only numeric fields present in the frame apply, so
        // the UI never has to send the full set for one change.
        TrackerParams params = controller.getTrackerParams();
        for (const ParamField& f : PARAM_FIELDS)
        {
            const Json::Value& v = msg[f.key];
            if (v.isNumeric())
            {
                params.*(f.member) = clampField(f, v.asDouble());
            }
        }
        // lookahead min > max 이면 PurePursuitTracker 의 std::clamp(lo, hi) 가
        // 미정의 동작이 됩니다. min 을 max 쪽으로 눌러 불변식을 지킵니다.
        // std::clamp with lo > hi is UB in PurePursuitTracker; press min down
        // to max to keep the invariant.
        params.ppLookaheadMinM =
                std::min(params.ppLookaheadMinM, params.ppLookaheadMaxM);
        controller.setTrackerParams(params);
        broadcastTuningState(controller);
        return true;
    }

    // 그 외 kind 는 이 채널의 것이 아닙니다. 조용히 무시합니다.
    // Any other kind does not belong to this channel; ignore quietly.
    std::cerr << "[TuningProtocol] ignored message kind '" << kind << "'\n";
    return false;
}

} // namespace web
} // namespace AppMain
