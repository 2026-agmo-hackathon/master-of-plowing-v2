/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 *
 * @Project Name  : NEVONEX
 * @File Name     : NtripMqttClient.hpp
 * @Author        : Adhith Gopal
 * @Created       : 2025
 * @Last modified : 
 * @Description   : 
 */
#ifndef B579586E_2F79_4177_A166_571016DAB955
#define B579586E_2F79_4177_A166_571016DAB955

#include <mqtt/async_client.h>
#include <string>
#include <memory>

namespace nevonex::ntrip
{
    /**
     * @brief This is an internal class.
     * 
     */
    class NtripConnectionHandler;

    using mqttOnMessageHandler = std::function<void(std::string)>;

    class NtripMqttClient // NOSONAR - cpp:S3624 since destructor is the case is use to disconnect mqtt in case of rare condition crash
    {
    public:
        ~NtripMqttClient();
    private:
        mqtt::async_client_ptr asyncClient;
        explicit NtripMqttClient(mqttOnMessageHandler mqttOnMessageHandlerPtr_):mqttOnMessageHandlerPtr(mqttOnMessageHandlerPtr_){};        

        mqttOnMessageHandler mqttOnMessageHandlerPtr = nullptr;
        bool connect();
        bool disconnect();

        void mqttConnected(const std::string &s);
        void mqttDisconnected(const mqtt::properties &props, mqtt::ReasonCode code);
        void mqttConnectionLost(const std::string &s);
        void mqttMsgArrived(mqtt::const_message_ptr msg);

        void connect(const std::string & userName, const std::string & password);
        void connect(const std::string & _trustStorePath, const std::string & _keyStorePath, const std::string & _privateKeyPath);
        void connect(std::string & host, int port, std::string & uniqueCallerId, mqtt::connect_options & connOpts);
        void connectMqtt();

        std::vector < std::string > subscribeTopicList;

        friend class NtripConnectionHandler;

        
    };
};

#endif /* B579586E_2F79_4177_A166_571016DAB955 */
