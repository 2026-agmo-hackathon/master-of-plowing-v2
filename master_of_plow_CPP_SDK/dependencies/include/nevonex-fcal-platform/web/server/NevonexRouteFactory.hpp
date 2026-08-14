/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTEFACTORY_HPP_
#define NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTEFACTORY_HPP_

#include "../../web/server/NevonexRoute.hpp"

#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace nevonex
{
namespace web
{
namespace server
{
class HTTPRestServer;

/**
 * Implements a Router HTTP requests.
 *
 * @see https://pocoproject.org/docs/Poco.Net.HTTPRequestHandlerFactory.html for additional documentation
 */
class NevonexRouteFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	NevonexRouteFactory() = default;
	virtual ~NevonexRouteFactory() = default;

	/**
	 * Create route for the incoming request.
	 * @see https://pocoproject.org/docs/Poco.Net.HTTPServerRequest.html for additional documentation
	 */
	virtual NevonexRoute* createRoute(const Poco::Net::HTTPServerRequest &request) = 0;
protected:
	virtual NevonexRoute* createNevonexRoute(const Poco::Net::HTTPServerRequest & request);
	virtual void disconnect() = 0;
private:
	virtual Poco::Net::HTTPRequestHandler* createRequestHandler(
			const Poco::Net::HTTPServerRequest &request) override;
	friend class HTTPRestServer;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTEFACTORY_HPP_ */
