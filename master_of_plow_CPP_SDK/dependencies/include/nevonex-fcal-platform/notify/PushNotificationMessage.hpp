/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef SOURCE_NOTIFY_PUSHNOTIFICATIONMESSAGE_HPP_
#define SOURCE_NOTIFY_PUSHNOTIFICATIONMESSAGE_HPP_

#include <string>
#include <chrono>
#include <unordered_map>
#include <rapidjson/document.h>

namespace nevonex
{
namespace notify
{

enum class PUSH_PRIORITY
{
    DEFAULT,
    LOW, 
    HIGH,
    MAX
};

/**
 * @brief Push notification message structure following the required format:
 * {
 *   "featureID": "string",
 *   "featureName": "string", 
 *   "type": "string",
 *   "title": "string",
 *   "body": "string",
 *   "priority": "default|low|high|max",
 *   "timestamp": long
 * }
 */
class PushNotificationMessage
{
public:
    std::string featureID;
    std::string featureName;
    std::string type;
    std::string title;
    std::string body;
    PUSH_PRIORITY priority;
    std::chrono::system_clock::time_point timestamp;
    
    // Default constructor
    PushNotificationMessage();
    
    // Constructor with basic parameters
    PushNotificationMessage(const std::string& _title, 
                           const std::string& _body, 
                           const std::string& _type = "notification",
                           PUSH_PRIORITY _priority = PUSH_PRIORITY::DEFAULT);
};

/**
 * @brief Utility class for push notification message formatting and validation
 */
class PushNotificationFormatter
{
public:
    /**
     * @brief Convert PushNotificationMessage to JSON format
     */
    static rapidjson::Document toJson(const PushNotificationMessage& message);
    
    /**
     * @brief Convert JSON to PushNotificationMessage  
     */
    static PushNotificationMessage fromJson(const rapidjson::Document& jsonDoc);
    
    /**
     * @brief Validate push notification message
     */
    static bool validate(const PushNotificationMessage& message);
    
    /**
     * @brief Get priority string representation
     */
    static std::string getPriorityString(PUSH_PRIORITY priority);
    
    /**
     * @brief Convert priority string to enum
     */
    static PUSH_PRIORITY getPriorityFromString(const std::string& priorityStr);
    
private:
    PushNotificationFormatter() = default;
};

} // namespace notify
} // namespace nevonex

#endif /* SOURCE_NOTIFY_PUSHNOTIFICATIONMESSAGE_HPP_ */