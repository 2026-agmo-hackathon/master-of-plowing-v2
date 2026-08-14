/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 *
 * @Project Name  : NEVONEX
 * @File Name     : NtripCommon.hpp
 * @Author        : Adhith Gopal
 * @Created       : 2025
 * @Last modified :
 * @Description   : This class provides generic headers.
 */
#ifndef F5BCDF0B_8860_4750_8799_6C8DF6AD4362
#define F5BCDF0B_8860_4750_8799_6C8DF6AD4362

#include <string>

namespace nevonex::ntrip
{
    /**
     * @brief This enum contains status information for triggerNtripConnection(...).
     *
     */
    enum NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM
    {
        CONNECTION_TRIGGERED,
        CONNECTION_ACTIVE_USE_OVERRIDE_FLAG_TO_OVERRIDE,
        CONNECTION_OVERRIDE_TRIGGERED,
        UNKNOWN_NCTR
    };

    /**
     * @brief This enum contains the NTRIP Caster connection status, this will be notified
     * via NtripConnectionHandlerCallback.
     *
     */
    enum NTRIP_CONNECTION_ENUM
    {
        CONNECTED,
        DISCONNECTED,
        UNKNOWN_NC
    };

    /**
     * @brief This enum contains status information for triggerNtripDisconnection(...).
     *
     */
    enum NTRIP_DISCONNECTION_RESPONSE_ENUM
    {
        DISCONNECTION_TRIGGERED,
        DISCONNECTION_NOT_POSSIBLE_USED_BY_OTHERS,
        UNKNOWN_NDR
    };

    class NtripConnectionTriggerResponse
    {
    public:
        NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM status = NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::UNKNOWN_NCTR;
        std::string error = "";
    };

    class NtripDisconnectionResponse
    {
    public:
        NTRIP_DISCONNECTION_RESPONSE_ENUM status = NTRIP_DISCONNECTION_RESPONSE_ENUM::UNKNOWN_NDR;
        std::string error = "";
    };

    class NtripEnableResponse
    {
    public:
        std::string url = "";
        std::string mountPoint = "";
        NTRIP_CONNECTION_ENUM connectionStatus = NTRIP_CONNECTION_ENUM::UNKNOWN_NC;
        std::string error = "";
    };

    /**
     * @brief This is a util class which provides utilities to convert between enum and string representations.
     *
     */
    class NtripCommonHelper
    {
    public:
        NtripCommonHelper() = default;
        ~NtripCommonHelper() = default;

        std::string getNtripConnectionTriggerResponseAsString(NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM input)
        {
            if (NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_ACTIVE_USE_OVERRIDE_FLAG_TO_OVERRIDE == input)
            {
                return "CONNECTION_ACTIVE_USE_OVERRIDE_FLAG_TO_OVERRIDE";
            }
            else if (NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_OVERRIDE_TRIGGERED == input)
            {
                return "CONNECTION_OVERRIDE_TRIGGERED";
            }
            else if (NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_TRIGGERED == input)
            {
                return "CONNECTION_TRIGGERED";
            }
            return "UNKNOWN_NCTR";
        }

        NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM getNtripConnectionTriggerResponseFromString(std::string input)
        {
            if ("CONNECTION_ACTIVE_USE_OVERRIDE_FLAG_TO_OVERRIDE" == input)
            {
                return NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_ACTIVE_USE_OVERRIDE_FLAG_TO_OVERRIDE;
            }
            else if ("CONNECTION_OVERRIDE_TRIGGERED" == input)
            {
                return NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_OVERRIDE_TRIGGERED;
            }
            else if ("CONNECTION_TRIGGERED" == input)
            {
                return NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::CONNECTION_TRIGGERED;
            }
            return NTRIP_CONNECTION_TRIGGER_RESPONSE_ENUM::UNKNOWN_NCTR;
        }

        std::string getNtripConnectionAsString(NTRIP_CONNECTION_ENUM input)
        {
            if (NTRIP_CONNECTION_ENUM::CONNECTED == input)
            {
                return "CONNECTED";
            }
            else if (NTRIP_CONNECTION_ENUM::DISCONNECTED == input)
            {
                return "DISCONNECTED";
            }
            return "UNKNOWN_NC";
        }

        NTRIP_CONNECTION_ENUM getNtripConnectionFromString(std::string input)
        {
            if ("CONNECTED" == input)
            {
                return NTRIP_CONNECTION_ENUM::CONNECTED;
            }
            else if ("DISCONNECTED" == input)
            {
                return NTRIP_CONNECTION_ENUM::DISCONNECTED;
            }
            return NTRIP_CONNECTION_ENUM::UNKNOWN_NC;
        }

        std::string getNtripDisconnectionResponseAsString(NTRIP_DISCONNECTION_RESPONSE_ENUM input)
        {
            if (NTRIP_DISCONNECTION_RESPONSE_ENUM::DISCONNECTION_TRIGGERED == input)
            {
                return "DISCONNECTION_TRIGGERED";
            }
            else if (NTRIP_DISCONNECTION_RESPONSE_ENUM::DISCONNECTION_NOT_POSSIBLE_USED_BY_OTHERS == input)
            {
                return "DISCONNECTION_NOT_POSSIBLE_USED_BY_OTHERS";
            }
            return "UNKNOWN_NDR";
        }

        NTRIP_DISCONNECTION_RESPONSE_ENUM getNtripDisonnectionResponseFromString(std::string input)
        {
            if ("DISCONNECTION_TRIGGERED" == input)
            {
                return NTRIP_DISCONNECTION_RESPONSE_ENUM::DISCONNECTION_TRIGGERED;
            }
            else if ("DISCONNECTION_NOT_POSSIBLE_USED_BY_OTHERS" == input)
            {
                return NTRIP_DISCONNECTION_RESPONSE_ENUM::DISCONNECTION_NOT_POSSIBLE_USED_BY_OTHERS;
            }
            return NTRIP_DISCONNECTION_RESPONSE_ENUM::UNKNOWN_NDR;
        }
    };

};

#endif /* F5BCDF0B_8860_4750_8799_6C8DF6AD4362 */
