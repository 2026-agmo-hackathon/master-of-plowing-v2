/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_HMI_WEBSOCKETROUTE_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_HMI_WEBSOCKETROUTE_HPP_

#include "../../web/server/MessageQueueWebsocket.hpp"

namespace nevonex
{
namespace web
{
namespace hmi
{

class HmiWebsocketRoute : public ::nevonex::web::server::MessageQueueWebsocket
{

public:
	explicit HmiWebsocketRoute(const std::string & authTokenLoc);
	virtual ~HmiWebsocketRoute() = default;

protected:
	bool authenticate() override;

private:
	std::string authToken;

};

} /* namespace hmi */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_HMI_WEBSOCKETROUTE_HPP_ */
