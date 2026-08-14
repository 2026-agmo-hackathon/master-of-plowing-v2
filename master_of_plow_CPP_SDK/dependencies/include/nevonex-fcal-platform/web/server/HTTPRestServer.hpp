/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_SERVER_HTTPSERVER_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_SERVER_HTTPSERVER_HPP_

#include "../../web/server/NevonexRoute.hpp"
#include "../../web/server/NevonexRouteFactory.hpp"
#include "../../web/server/AbstractHttpServerProvider.hpp"
#include "../../web/hmi/AbstractHmiServerProvider.hpp"

#include <unordered_map>
#include <memory>
#include <Poco/URI.h>
#include <Poco/Net/HTTPServer.h>

#include "../../config/GlobalConfig.hpp"

namespace nevonex
{
namespace web
{
namespace server
{

/**
 * Internal
 */
class HTTPRestServer final
{
public:
	virtual ~HTTPRestServer();
	HTTPRestServer(HTTPRestServer const&) = delete;
	void operator=(HTTPRestServer const&) = delete;

protected:
	HTTPRestServer();
	void start(const std::string &address, boost::uint16_t portValue);
	void stop();
	void registerRoute(const std::string &path, std::shared_ptr<NevonexRouteFactory> route);
	friend class ::nevonex::web::server::AbstractHttpServerProvider;

private:

	class RouteNotAvailable: public Poco::Net::HTTPRequestHandler
	{
	public:
		void handleRequest(Poco::Net::HTTPServerRequest&,
				Poco::Net::HTTPServerResponse&) override;
	};

	class NevonexRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		NevonexRequestHandlerFactory() = default;
		virtual ~NevonexRequestHandlerFactory() = default;

		Poco::Net::HTTPRequestHandler* createRequestHandler(
				const Poco::Net::HTTPServerRequest &request) override;

	private:
		std::unordered_map<std::string, std::shared_ptr<NevonexRouteFactory>> m_route;
		friend class HTTPRestServer;
	};

private:
	/**
	 * Raw Pointer access so that no UniquePointer can invalidate.
	 */
	NevonexRequestHandlerFactory *m_requestHandlerFactory;
	Poco::Net::HTTPServer *m_server;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_SERVER_HTTPSERVER_HPP_ */
