/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_HMI_ABSTRACTUSERCONTROLS_HPP_
#define NEVONEX_FCAL_PLATFORM_HMI_ABSTRACTUSERCONTROLS_HPP_

#include "../web/hmi/AbstractHmiServerProvider.hpp"

#include <unordered_map>
#include <queue>

namespace nevonex
{
namespace hmi
{

/**
 * INTERNAL DO NOT USE.
 */
enum class STATE_RETENTION_TYPE
{
	NONE, TABLE, CHART, CHART_DEF, INPUT_RATE, CLEAR_INPUT_RATE
};

class AbstractUserControls : public ::nevonex::web::hmi::AbstractHmiServerProvider
{
public:
	AbstractUserControls();
	virtual ~AbstractUserControls() = default;
protected:
	void initUserControls();
	void onSocketConnected();
	virtual void onWebSocketMessage(const std::string & msg) = 0;
	void sendMessage(const std::string & controlId, const std::string & message, bool isScalar,
			::nevonex::hmi::STATE_RETENTION_TYPE stateRetentionType);
private:
	void message_handler(const std::string & message);
	std::shared_ptr<::nevonex::web::hmi::HmiWebsocketRouteFactory> router;

	std::unordered_map<std::string, std::string> scalarMap;
	std::unordered_map<std::string, std::queue<std::string>> queueMap;
};

} /* namespace hmi */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_HMI_ABSTRACTUSERCONTROLS_HPP_ */
