/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTE_HPP_
#define NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTE_HPP_

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/AbstractHTTPRequestHandler.h>
namespace nevonex
{
namespace web
{
namespace server
{
/**
 * A new NevonexRoute object will be created for each new HTTP request that is received by the HTTPServer.
 *
 * @see https://pocoproject.org/docs/Poco.Net.AbstractHTTPRequestHandler.html for additional documentation
 * @see https://pocoproject.org/docs/Poco.Net.HTMLForm.html to access HTML form details.
 *
 * @see WebSocketRoute to use a websocket connection from the clients.
 */
class NevonexRoute: public Poco::Net::AbstractHTTPRequestHandler
{
public:
	typedef Poco::SharedPtr<NevonexRoute> Ptr;
	NevonexRoute() = default;
	virtual ~NevonexRoute() = default;

private:
	NevonexRoute(const NevonexRoute&);
	NevonexRoute& operator =(const NevonexRoute&);

protected:
	virtual void handleGet(Poco::Net::HTTPServerRequest &request,
			Poco::Net::HTTPServerResponse &response);

	virtual void handlePut(Poco::Net::HTTPServerRequest &request,
			Poco::Net::HTTPServerResponse &response);

	virtual void handlePost(Poco::Net::HTTPServerRequest &request,
			Poco::Net::HTTPServerResponse &response);

	virtual void handleDelete(Poco::Net::HTTPServerRequest &request,
			Poco::Net::HTTPServerResponse &response);

	virtual void handleOptions(Poco::Net::HTTPServerRequest &request,
			Poco::Net::HTTPServerResponse &response);

protected:
	virtual void run();
private:
	using Poco::Net::AbstractHTTPRequestHandler::handleRequest;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_SOURCE_WEB_SERVER_NEVONEXROUTE_HPP_ */
