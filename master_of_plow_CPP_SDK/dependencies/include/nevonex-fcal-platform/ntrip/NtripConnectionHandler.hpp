/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 *
 * @Project Name  : NEVONEX
 * @File Name     : NtripConnectionHandler.hpp
 * @Author        : Adhith Gopal
 * @Created       : 2025
 * @Last modified : 
 * @Description   : This class is the client util for Ntrip related functionality.
 */
#ifndef F6CCFFCC_B66A_4046_9540_11DC66E01076
#define F6CCFFCC_B66A_4046_9540_11DC66E01076

#include <string>
#include "NtripConnectionHandlerCallback.hpp"
#include "NtripMqttClient.hpp"
#include <json/value.h>
#include <json/json.h>

namespace nevonex::ntrip
{
    /**
     * @brief This class is a convenience class that allows the users to enable NTRIP feature for their application
     * and also has helper methods which enables the users to start/stop NTRIP connection. The NTRIP connection status 
     * will flow to the application via callback in async manner.
     * 
     */
    class NtripConnectionHandler final
    {
    public:
        NtripConnectionHandler() = default;
        ~NtripConnectionHandler();
        NtripConnectionHandler(NtripConnectionHandler&) = delete;
        void operator=(NtripConnectionHandler&) = delete;

        //Public API's

        /**
         * @brief This api should be the first call from user app if NTRIP is required. If this api is not invoked before 
         * triggering other api's then NTRIP handler wont work.
         * 
         * @param callback - The NtripConnectionHandlerCallback that will be used for ASYNC status updates.
         */
        NtripEnableResponse enableNtrip(std::shared_ptr<NtripConnectionHandlerCallback> callback);
        /**
         * @brief This api is used to trigger the Ntrip Connection. The NtripConnectionTriggerResponse
         * would be returned and this will contain the connection trigger status. The NTRIP connection trigger might not be 
         * successful at all times and the status of this can be found as part of the response. So if there is no active NTRIP
         * connection then this trigger would be successful, but if there is already an active connection then this call will not
         * be successful unless override flag is set to true. And the actual connection status would be available as part of 
         * NtripConnectionHandlerCallback::connectionStatusUpdate as a separate async flow.
         * 
         * @param url - The NTRIP caster url.
         * @param mountPoint - The NTRIP caster mountpoint.
         * @param userName - The NTRIP caster connection username.
         * @param password - The NTRIP caster connection password.
         * @param isOverride - This flag can be used if this request should override any other active connection.
         */
        NtripConnectionTriggerResponse triggerNtripConnection(std::string url, std::string mountPoint, std::string userName, std::string password, bool isOverride=false);
        /**
         * @brief This api is used to trigger the Ntrip disconnection. The NTRIP connection would be disconnected only if there are no other 
         * features using NTRIP, the status of the same would be returned as part of NtripDisconnectionResponse.
         * 
         */
        NtripDisconnectionResponse triggerNtripDisconnection();

    private:
        bool isNtripEnabled = false;
        bool isNtripMqttInitialized = false;
        std::unique_ptr<NtripMqttClient> ntripMqttClient;
        std::shared_ptr<NtripConnectionHandlerCallback> ntripConnectionHandlerCallback;

        Json::Value convertStringToJson(std::string &input);
        void onMessageArrivedHandler(std::string message);
    };
};

#endif /* F6CCFFCC_B66A_4046_9540_11DC66E01076 */
