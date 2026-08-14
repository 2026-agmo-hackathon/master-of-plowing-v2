/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTEFACTORY_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTEFACTORY_HPP_

#include "../../web/server/NevonexRouteFactory.hpp"
#include "../../web/server/WebSocketRoute.hpp"

namespace nevonex
{
namespace web
{
namespace server
{

/**
 * Implements a Websocket router.
 *
 * NOTE: Do not store the WebSocketRoute object instance. It will be nullified once the session is closed.
 */
class WebSocketRouteFactory: public NevonexRouteFactory
{
public:
	WebSocketRouteFactory() = default;
	virtual ~WebSocketRouteFactory() = default;
protected:
	virtual ::nevonex::web::server::WebSocketRoute* createWebsocketRoute(const Poco::Net::HTTPServerRequest &request) = 0;
	virtual ::nevonex::web::server::NevonexRoute* createRoute(const Poco::Net::HTTPServerRequest &request) override;
private:
	virtual NevonexRoute* createNevonexRoute(const Poco::Net::HTTPServerRequest &request) override;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTEFACTORY_HPP_ */
