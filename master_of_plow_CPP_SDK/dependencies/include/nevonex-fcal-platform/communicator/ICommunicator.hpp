/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_COMMUNICATOR_ICOMMUNICATOR_HPP_
#define NEVONEX_FCAL_PLATFORM_COMMUNICATOR_ICOMMUNICATOR_HPP_

#include <string>
#include <vector>
#include <future>

namespace nevonex
{

namespace config
{
/**
 * INTERNAL DO NOT USE.
 */
enum class COMMUNICATOR_AUTH_TYPE
{
	NONE, PROD_NONE, CREDENTIALS, TLS, TLS_CRED
};

}

namespace communicator
{

class CommunicatorProperties;

/**
 * INTERNAL DO NOT USE.
 */
class ICommunicator
{
public:
	virtual ~ICommunicator() = default;

	static std::unique_ptr<ICommunicator> getAsyncClient(const std::string& featureId, const std::string& callerId,
			const std::string& mqttHost, const int& mqttPort);
	// Connecting with connection properties
	void connect(const CommunicatorProperties & communicatorProperties);
	// Connecting through FSM.
	void connect(const std::string & folderPath);

	void setHost(const std::string& _host);
	void setPort(int _port);

protected:
	/** Handler type for when a communicator connection is made */
	using on_connect_handler = std::function<void(const std::string& message)>;
	/** Handler type for when a communicator connection is made */
	using on_connection_lost_handler = std::function<void(const std::string& message)>;
	/** Handler type for when a communicator connection close */
	using on_disconnect_handler = std::function<void(const std::string & message)>;
	/** Handler type for when a message recieved */
	using on_message_recieved_handler = std::function<void(const std::string & topic, const std::string & message)>;

public:
	virtual void publish(const std::string &topicValue, const std::string &message) = 0;
	virtual void disconnect() = 0;
	virtual bool isClientConnected() = 0;

	virtual void setOnConnectHandler(on_connect_handler _connHandler) = 0;
	virtual void setConnectionLostHandler(on_connection_lost_handler _connLostHandler) = 0;
	virtual void setOnDisconnectHandler(on_disconnect_handler _disConnHandler) = 0;
	virtual void setOnMessageRecievedHandler(on_message_recieved_handler _onMessageReceivedHandler) = 0;
	virtual void setConnectionTimeout(uint32_t _connectionTimeout) = 0;

	virtual void setSubscribeTopicList(const std::vector<std::string> & _subscribeTopicList) = 0;

	ICommunicator(ICommunicator const&) = delete;
	void operator=(ICommunicator const&) = delete;

protected:

	/** Connection handler */
	on_connect_handler onConnectHandler;
	/** Connection lost handler */
	on_connection_lost_handler onConnectLostHandler;
	/** Disconnected handler */
	on_disconnect_handler onDisconnectHandler;
	/** Disconnected handler */
	on_message_recieved_handler onMessageRecievedHandler;

	virtual void preConnect() = 0;
	virtual void connect() = 0;
	virtual void connect(const std::string & userName, const std::string & password) = 0;
	virtual void connect(const std::string & _trustStorePath, const std::string & _keyStorePath, const std::string & _privateKeyPath) = 0;

	virtual void onConnect(const std::string & cause) = 0;
	virtual void onConnectionLost(const std::string & cause) = 0;
	virtual void onDisconnect(const std::string & cause) = 0;
	virtual void onMessageReceived(const std::string & topic, const std::string & message) = 0;

	ICommunicator();
	ICommunicator(const std::string & _callerId, const std::string & _host, const int & _port);

    std::string callerId;
    std::string host;
    int port;

    bool isFSMConnection;
    std::string trustStorePath;
	std::string keyStorePath;
	std::string privateKeyPath;
};

}//communicator
}//nevonex


#endif /* NEVONEX_FCAL_PLATFORM_COMMUNICATOR_ICOMMUNICATOR_HPP_ */
