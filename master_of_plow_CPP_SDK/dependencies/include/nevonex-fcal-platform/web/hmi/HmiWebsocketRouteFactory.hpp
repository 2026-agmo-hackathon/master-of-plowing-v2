/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_HMI_HMIWEBSOCKETROUTEFACTORY_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_HMI_HMIWEBSOCKETROUTEFACTORY_HPP_

#include "../../web/server/WebSocketRouteFactory.hpp"
#include "../../web/server/MessageQueueWebsocket.hpp"

#include <future>

namespace nevonex
{
namespace web
{
namespace hmi
{

/**
 * Internal
 */
class HmiWebsocketRouteFactory : public ::nevonex::web::server::WebSocketRouteFactory
{
	/** Handler type for when a websocket message is received */
	using message_handler = std::function<void(const std::string& message)>;
	/** Handler when connection established **/
	using onConnectionHandler = std::function<void()>;
public:
	HmiWebsocketRouteFactory();
	virtual ~HmiWebsocketRouteFactory();

	void sendMessage(const std::string & message);

	void set_message_handler(message_handler msgHandler);
	void setConnectionCallBack(onConnectionHandler connectionCallBack);
	void connected(const std::string & message);
	void disconnected(const std::string & message);
	virtual void disconnect() override;

	virtual ::nevonex::web::server::WebSocketRoute* createWebsocketRoute(const Poco::Net::HTTPServerRequest &request) override;

	bool isWebsocketConnected();

	void setAuthWebToken(const std::string &authTokenLoc);

private:
	bool websocketDisconnected;
	::nevonex::web::server::MessageQueueWebsocket* m_route;
	message_handler msgHandler_;
	onConnectionHandler connectionCallBack_;
	std::string authWebToken;

};

} /* namespace hmi */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_HMI_HMIWEBSOCKETROUTEFACTORY_HPP_ */
