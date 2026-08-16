/*
 * AppMain/RddfLoader.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * RDDF 파일 하나를 읽어 트래커에 적재합니다.
 *
 * 원래 이 로직은 web/WebSocketEndPointImpl.cpp 안의 익명 namespace에 있었고,
 * 다음 세 가지에 묶여 있었습니다.
 *
 *   1. mapId          — 외부 서버가 준 맵 식별자
 *   2. StartPoint     — 외부 서버 맵 리스트가 준 실행 시점 시작 좌표
 *   3. UI 브로드캐스트 — rddf_loaded / rddf_path 메시지
 *
 * 이 앱은 맵 리스트를 받지도, UI에서 맵을 고르지도 않으므로 셋 다 사라졌습니다.
 * 이 로더에는 실제 실행 시작점이 없으므로 정적 RDDF 검증에서 시작거리 규칙을
 * 제거했습니다. 시작 자세/위치는 simulator/session의 주행 시작 게이트가 실제
 * live pose로 확인합니다. waypoint 0을 가짜 시작점으로 넘기지 않습니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * Loads one RDDF file and installs the resulting track into the tracker.
 *
 * This used to live in an anonymous namespace inside web/WebSocketEndPointImpl.cpp
 * and depended on a server-supplied mapId, a server-supplied StartPoint, and UI
 * broadcast callbacks. None of those exist any more: the app neither fetches a
 * map list nor lets a UI pick a map.
 *
 * This loader has no live execution start point, so static RDDF validation no
 * longer contains a start-distance rule. The simulator/session start gate owns
 * live pose proximity. Waypoint 0 is not passed as a fabricated start point.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_RDDFLOADER_HPP
#define APPMAIN_RDDFLOADER_HPP

#include <string>

namespace AppMain
{

class MainController;

class RddfLoader
{
public:
    /**
     * 파일을 파싱·검증해 트래커를 만들고 controller에 주입합니다.
     * 실패는 표준에러로 기록하고 false를 돌려줍니다. 예외는 던지지 않습니다.
     *
     * Parses and validates the file, builds a tracker and installs it on the
     * controller. Failures are logged to stderr and reported as false; never
     * throws.
     */
    static bool loadFromFile(const std::string& filePath,
                             MainController& controller);

    /**
     * 디렉터리에서 가장 최근에 쓰인 .rddf 를 찾아 적재합니다. RDDF 는 평소
     * 플랫폼의 handleFile 로만 도착하는데, 앱이 재시작되면 그 전달은 다시
     * 오지 않고 파일만 /app/in/rf/ 에 남습니다. 부팅 때 이 함수로 되살려야
     * 재시작마다 RDDF 를 다시 업로드하는 일이 없어집니다. 파일이 없으면
     * 조용히 false 를 돌려줍니다 (신규 설치 직후의 정상 상태).
     *
     * Loads the most recently written .rddf in the directory. RDDFs normally
     * arrive only through the platform's handleFile callback; after an app
     * restart that delivery never repeats while the file itself is still in
     * /app/in/rf/. Calling this at boot revives it so a restart no longer
     * demands a fresh upload. Returns false quietly when the directory holds
     * no file (the normal state right after an install).
     */
    static bool loadNewestFromDirectory(const std::string& directory,
                                        MainController& controller);
};

} // namespace AppMain

#endif // APPMAIN_RDDFLOADER_HPP
