/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef SOURCE_NOTIFY_NOTIFICATIONMANAGER_HPP_
#define SOURCE_NOTIFY_NOTIFICATIONMANAGER_HPP_

#include "AbstractFeatureNotification.hpp"
#include "AbstractIgnitionStateNotification.hpp"

#include <string>
#include <queue>
#include <functional>
#include <unordered_map>

#include <rapidjson/document.h>
#include "../communicator/ICommunicator.hpp"
namespace nevonex
{
namespace notify
{

class PushNotificationMessage;

/**
 * INTERNAL DO NOT USE.
 */

enum class NOTIFICATION_MESSAGE
{
	NONE, FEATURE_INIT, FEATURE_CLIENT_STARTED, FEATURE_BEFORE_READY, FEATURE_READY
};

inline const std::string getLiteral(NOTIFICATION_MESSAGE e)
{
	switch (e)
	{
	case NOTIFICATION_MESSAGE::FEATURE_INIT:
		return "FEATURE_INIT";
	case NOTIFICATION_MESSAGE::FEATURE_CLIENT_STARTED:
		return "FEATURE_CLIENT_STARTED";
	case NOTIFICATION_MESSAGE::FEATURE_BEFORE_READY:
		return "FEATURE_BEFORE_READY";
	case NOTIFICATION_MESSAGE::FEATURE_READY:
		return "FEATURE_READY";
	case NOTIFICATION_MESSAGE::NONE:
		return "NONE";
	}
	return "INVALID";
}

class NotificationManager
{
public:
	void _initialize();
	void pushNotification(const NOTIFICATION_MESSAGE & notificationMessage);
	bool getFeatureStopState();

	// Push notification methods
	void sendPushNotification(const PushNotificationMessage& message);

	static NotificationManager& getInstance()
	{
		static NotificationManager instance;
		return instance;
	}
	~NotificationManager() = default;
	NotificationManager(NotificationManager const&) = delete;
	void operator=(NotificationManager const&) = delete;
	/**
	 * @brief Internal API
	 * 
	 * @param _abstractFeatureNotification 
	 */
	void setFeatureManager(std::unique_ptr<AbstractFeatureNotification> _abstractFeatureNotification);
    /**
     * @brief Internal API
     *
     * @param _abstractIgnitionStateNotification
     */
    void setIgnitionStateManager(std::unique_ptr<AbstractIgnitionStateNotification> _abstractIgnitionStateNotification);
private:
	NotificationManager();
	void onAsyncClientConnect(const std::string &msg);
	bool isValidOrderNotification(const NOTIFICATION_MESSAGE & messageToBeSent);
	void pushNotificationToCommunicator(const NOTIFICATION_MESSAGE & messageToBeSent);
	void onMessageArrived(const std::string& topicStr, const std::string& payload);
	void processWaitingNotifications();
	bool isFeatureStopNotified;

	const std::string featureId;

	NOTIFICATION_MESSAGE lastSentMessage;
	std::queue<NOTIFICATION_MESSAGE> notificationQueue;
	std::vector<NOTIFICATION_MESSAGE> waitingNotifications;
	std::unordered_map<NOTIFICATION_MESSAGE, rapidjson::Document> notificationMessageMap;

	static const std::string TOPIC_VALUE;
	static const std::string PUSH_NOTIFICATION_TOPIC;
	std::unique_ptr<::nevonex::communicator::ICommunicator> asyncClient;

	std::unique_ptr<AbstractFeatureNotification> abstractFeatureNotification;
	std::unique_ptr<AbstractIgnitionStateNotification> abstractIgnitionStateNotification;

};
}//notify
}//nevonex


#endif /* SOURCE_MQTTCLIENT_ASYNCMQTTCLIENT_HPP_ */
