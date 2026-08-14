/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTE_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTE_HPP_

#include "../../web/server/NevonexRoute.hpp"
#include "../../notify/NotificationManager.hpp"

#include <Poco/Net/WebSocket.h>

namespace nevonex
{
namespace web
{
namespace server
{

/**
 * Implements a WebsocketRoute aka. WebSocket Session.
 *
 * @see https://pocoproject.org/docs/Poco.Net.WebSocket.html for more detailed information.
 */
class WebSocketRoute: public NevonexRoute
{
public:
	WebSocketRoute();
	virtual ~WebSocketRoute();
	/**
	 * Override this method to take full control of the websocket session.
	 */
	virtual void handleWebsocket(Poco::Net::WebSocket &ws) = 0;

	Poco::Net::WebSocket& ws();
	/**
	 * Request close of the websocket.
	 */
	virtual void close();

protected:
	virtual void run();
	void writeMessage(const std::string & writeMsg);
	std::string getConnectionCloseStatusCodeReason(int code);

private:
	Poco::Net::WebSocket* webSocket;
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_SERVER_WEBSOCKETROUTE_HPP_ */
