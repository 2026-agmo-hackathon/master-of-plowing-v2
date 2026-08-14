/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_SERVER_ABSTRACTHTTPSERVERPROVIDER_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_SERVER_ABSTRACTHTTPSERVERPROVIDER_HPP_

#include "../../web/server/NevonexRouteFactory.hpp"
#include "../../web/server/WebSocketRouteFactory.hpp"

#include <memory>

namespace nevonex
{
namespace web
{
namespace server
{

class HTTPRestServer;
/**
 * Internal.
 */
class AbstractHttpServerProvider
{
public:
	AbstractHttpServerProvider();
	virtual ~AbstractHttpServerProvider() = default;
public:
	virtual void registerWebsocketRoute(const std::string &path,
			std::shared_ptr<WebSocketRouteFactory> router);
	virtual void registerRoute(const std::string &path,
			std::shared_ptr<NevonexRouteFactory> router);
	virtual void start();
	virtual void stop();
protected:
	virtual void start(const std::string &address, std::uint16_t portValue);
private:
	::nevonex::web::server::HTTPRestServer *httpRestServer;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_SERVER_ABSTRACTHTTPSERVERPROVIDER_HPP_ */
