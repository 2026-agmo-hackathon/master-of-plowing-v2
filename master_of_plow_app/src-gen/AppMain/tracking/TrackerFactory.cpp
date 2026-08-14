/*
 * AppMain/tracking/TrackerFactory.cpp
 */

#include <AppMain/tracking/TrackerFactory.hpp>

#include <AppMain/tracking/impl/PurePursuitTracker.hpp>
#include <AppMain/tracking/impl/StanleyTracker.hpp>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>

namespace AppMain {
namespace tracking {

namespace {

constexpr const char* DEFAULT_TRACKER = "pure_pursuit";

using Creator = std::function<std::shared_ptr<IPathTracker>()>;

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 레지스트리. 알고리즘 하나당 한 줄입니다.
// 새 트래커를 만들었다면 여기에 { "이름", 생성 람다 } 한 줄만 추가하세요.
// 이름은 그 클래스의 name()이 돌려주는 문자열과 같아야 합니다
// (PathTrackerContractTest가 이 일치를 검사합니다).
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// The registry. One line per algorithm: add { "name", creator lambda } here.
// The key must match what that class's name() returns — PathTrackerContractTest
// checks exactly that.
// └────────────────────────────────────────────────────────────────────────────┘
const std::map<std::string, Creator>& registry()
{
    static const std::map<std::string, Creator> kRegistry = {
        { "pure_pursuit", [] { return std::make_shared<PurePursuitTracker>(); } },
        { "stanley",      [] { return std::make_shared<StanleyTracker>();      } },
    };
    return kRegistry;
}

} // namespace

std::shared_ptr<IPathTracker> createTracker(const std::string& name)
{
    const auto& reg = registry();
    auto it = reg.find(name);
    if (it == reg.end()) {
        // 모르는 이름 -> 사용 가능한 목록을 보여주고 기본값으로 되돌립니다.
        // Unknown name -> list what is available and fall back to the default.
        std::cerr << "[TrackerFactory] unknown tracker '" << name
                  << "', falling back to '" << DEFAULT_TRACKER << "'. Known:";
        for (const auto& kv : reg) {
            std::cerr << " " << kv.first;
        }
        std::cerr << "\n";
        it = reg.find(DEFAULT_TRACKER);
    }
    auto tracker = it->second();
    // 어떤 알고리즘이 실제로 돌고 있는지 로그에 한 줄 남깁니다.
    // One log line so it is obvious which algorithm is actually running.
    std::cerr << "[TrackerFactory] using tracker '" << tracker->name()
              << "' (period " << tracker->preferredPeriodMs() << "ms)\n";
    return tracker;
}

std::string defaultTrackerName()
{
    const char* env = std::getenv("PATH_TRACKER");
    if (env != nullptr && env[0] != '\0') {
        return std::string(env);
    }
    return std::string(DEFAULT_TRACKER);
}

std::vector<std::string> availableTrackerNames()
{
    std::vector<std::string> names;
    for (const auto& kv : registry()) {
        names.push_back(kv.first);
    }
    return names;
}

} // namespace tracking
} // namespace AppMain
