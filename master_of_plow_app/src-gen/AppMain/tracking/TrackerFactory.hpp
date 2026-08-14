/*
 * AppMain/tracking/TrackerFactory.hpp
 *
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 *
 * 어떤 트래커 구현이 존재하는지 아는 유일한 장소입니다.
 *
 * 새 알고리즘을 추가하는 방법:
 *   1. tracking/impl/ 아래에 PathTrackerBase를 상속한 클래스를 작성한다
 *   2. TrackerFactory.cpp의 레지스트리에 한 줄 추가한다
 *   3. PATH_TRACKER=<이름> 환경변수를 주고 실행한다
 *
 * 앱의 다른 어떤 코드도 구체적인 트래커 타입을 언급하지 않습니다. 즉 알고리즘
 * 교체는 재빌드가 아니라 환경변수 하나로 끝납니다.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 *
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 *
 * The single place that knows which tracker implementations exist.
 *
 * To add an algorithm:
 *   1. write a class deriving from PathTrackerBase under tracking/impl/
 *   2. add one line to the registry in TrackerFactory.cpp
 *   3. run with PATH_TRACKER=<its name>
 *
 * Nothing else in the app refers to a concrete tracker type, so swapping the
 * algorithm is an environment variable rather than a rebuild.
 *
 * └────────────────────────────────────────────────────────────────────────────┘
 */

#ifndef APPMAIN_TRACKING_TRACKERFACTORY_HPP
#define APPMAIN_TRACKING_TRACKERFACTORY_HPP

#include <AppMain/tracking/IPathTracker.hpp>

#include <memory>
#include <string>
#include <vector>

namespace AppMain
{
namespace tracking
{

/**
 * ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
 * 이름으로 트래커를 생성합니다. 모르는 이름이면 경고를 남기고 기본 구현을
 * 돌려줍니다 — PATH_TRACKER에 오타가 나도 널 트래커로 죽는 대신 정상 주행으로
 * 안전하게 떨어지도록 한 설계입니다.
 * └────────────────────────────────────────────────────────────────────────────┘
 * ┌─ English ──────────────────────────────────────────────────────────────────┐
 * Creates a tracker by name. Returns the default implementation (and logs a
 * warning) if the name is unknown, so a typo in PATH_TRACKER degrades to a
 * working run rather than a null tracker.
 * └────────────────────────────────────────────────────────────────────────────┘
 */
std::shared_ptr<IPathTracker> createTracker(const std::string& name);

/**
 * PATH_TRACKER 환경변수의 값, 없으면 "pure_pursuit".
 * Name from the PATH_TRACKER environment variable, else "pure_pursuit".
 */
std::string defaultTrackerName();

/**
 * 등록된 모든 이름. 진단과 테스트에 사용합니다.
 * All registered names, for diagnostics.
 */
std::vector<std::string> availableTrackerNames();

} // namespace tracking
} // namespace AppMain

#endif // APPMAIN_TRACKING_TRACKERFACTORY_HPP
