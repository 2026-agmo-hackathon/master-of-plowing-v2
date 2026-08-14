/*
 * AppMain/web/TuningProtocol.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 튜닝 제어 채널: 대시보드가 이 소켓으로 보내는 유일한 인바운드 메시지들입니다.
 * (예전 게임 세션 프로토콜 27종을 되살리는 것이 아닙니다.)
 *
 *   UI → 백엔드
 *     { "kind": "get_tuning_state" }                        접속 시 현재 상태 요청
 *     { "kind": "set_tracker", "name": "pure_pursuit" }     또는 "stanley"
 *     { "kind": "set_params", "ppKLtd": 0.6, ... }          바뀐 필드만 부분 전송
 *
 *   백엔드 → UI (세 메시지 모두의 응답)
 *     { "kind": "tuning_state", "tracker": "...",
 *       "available": ["pure_pursuit","stanley"],
 *       "params": { 전 필드 } }
 *
 * 값은 전부 범위 검증 후 clamp 됩니다 — 음수 게인이나 터무니없는 lookahead 가
 * 들어오면 조향이 폭주하기 때문입니다. clamp 된 실제 값이 tuning_state 로
 * 되돌아가므로 UI 는 "내가 보낸 것"이 아니라 "실제 반영된 것"을 표시합니다.
 *
 * 파싱·검증이 길어서 WebSocketEndPointImpl 의 PROTECTED REGION 에는 이 함수
 * 호출 한 줄만 둡니다 (FD 재생성이 리전 밖 코드를 지우기 때문입니다).
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The tuning control channel — the only inbound messages this socket accepts
 * (this does not resurrect the old 27-kind game-session protocol). All values
 * are range-validated and clamped; the clamped truth is broadcast back as
 * tuning_state so the UI shows what was actually applied, not what it sent.
 * The logic lives here so the WebSocketEndPointImpl protected region holds a
 * single call.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_WEB_TUNINGPROTOCOL_HPP
#define APPMAIN_WEB_TUNINGPROTOCOL_HPP

#include <jsoncpp/json/json.h>

namespace AppMain
{

class MainController;

namespace web
{

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * 인바운드 프레임 하나를 처리합니다. 튜닝 메시지였으면 true 를 돌려주고,
 * 처리 후 항상 tuning_state 를 방송합니다 — UI 가 "보낸 것이 실제로 적용됐는지"
 * 확인하는 유일한 수단입니다(지령↔실제 원리).
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Handles one inbound frame. Returns true if it was a tuning message; always
 * broadcasts tuning_state afterwards — the UI's only way to confirm that what
 * it sent was actually applied (the command-vs-actual principle).
 * └────────────────────────────────────────────────────────────────────────────┘
 */
bool handleTuningMessage(const Json::Value& msg,
                         ::AppMain::MainController& controller);

/**
 * 현재 튜닝 상태(tuning_state)를 WebSocket 으로 방송합니다.
 * Broadcasts the current tuning_state over the WebSocket.
 */
void broadcastTuningState(::AppMain::MainController& controller);

} // namespace web
} // namespace AppMain

#endif // APPMAIN_WEB_TUNINGPROTOCOL_HPP
