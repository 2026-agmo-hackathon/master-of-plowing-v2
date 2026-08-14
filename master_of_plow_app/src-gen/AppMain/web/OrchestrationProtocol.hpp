#ifndef APPMAIN_WEB_ORCHESTRATIONPROTOCOL_HPP
#define APPMAIN_WEB_ORCHESTRATIONPROTOCOL_HPP

#include <AppMain/orchestration/RunOrchestrator.hpp>
#include <json/json.h>
#include <string>

namespace AppMain {
class MainController;
std::string orchestrationSnapshotJson(const orchestration::Snapshot& snapshot);
namespace web {
bool handleOrchestrationMessage(const Json::Value& message,
                                MainController& controller);
}
}

#endif
