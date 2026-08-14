/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_SERVER_MESSAGEQUEUEWEBSOCKET_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_SERVER_MESSAGEQUEUEWEBSOCKET_HPP_

#include "../../web/server/WebSocketRoute.hpp"
#include <mqtt/thread_queue.h>
#include <string>
#include <functional>
#include <future>

namespace nevonex
{
namespace web
{
namespace server
{

/**
 * Implements a Queue for the messages to & from a Websocket.
 *
 * - In the case of a PING, a PONG message is returned.
 * - Use set_message_handler and publishMessage to read and write messages.
 * - Do not store the reference of the websocketroute
 *
 * @see ::nevonex::web::server::WebSocketRoute for to take more control of websockets.
 *
 * @see https://pocoproject.org/docs/Poco.Net.WebSocket.html for more detailed information.
 */
class MessageQueueWebsocket : public ::nevonex::web::server::WebSocketRoute
{
	/** Type for a thread-safe queue to consume messages synchronously */
	using consumer_queue_type = mqtt::thread_queue<std::string>;

	/** Handler type for when a websocket connection is made */
	using connection_handler = std::function<void(const std::string& message)>;
	/** Handler type for when a websocket connection close */
	using disconnected_handler = std::function<void(const std::string & message)>;

	/** Handler type for when a websocket message is received */
	using message_handler = std::function<void(const std::string& message)>;

public:
	MessageQueueWebsocket();
	virtual ~MessageQueueWebsocket();
	void set_connected_handler(connection_handler cb);
	void set_disconnected_handler(disconnected_handler cb);
	void set_message_handler(message_handler cb);

	/**
	 * Adds message to the queue. The message will be sent over websocket when there is a chance for it.
	 *
	 * @param message The value to add to the queue.
	 * @returns true if the item was added to the queue, false if the item was not added because the queue is currently full.
	 */
	bool publishMessage(const std::string & message);
	/**
	 * Request for close of the websocket connection.
	 */
	virtual void close() override;

protected:
	virtual void handleWebsocket(Poco::Net::WebSocket &ws) override;

private:
	consumer_queue_type readQueu;
	consumer_queue_type writeQueu;

	/** Connection handler */
	connection_handler connHandler_;
	/** Disconnected handler */
	disconnected_handler disconnectedHandler_;
	/**	Message handler */
	message_handler msgHandler_;

	std::thread writeThread;
	std::thread readThread;

	std::mutex m_mutex;
	volatile bool isConnected;

	void processWriteMessageQueue(Poco::Net::WebSocket& ws);
	void processReadMessageQueue();
};

} /* namespace server */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_SERVER_MESSAGEQUEUEWEBSOCKET_HPP_ */
