/*
 * nevonex/common/ConnectionTypeEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_CONNECTIONTYPEENUM_HPP
#define NEVONEX_COMMON_CONNECTIONTYPEENUM_HPP

#include <string>

namespace nevonex
{
    namespace common
    {

        enum class ConnectionTypeEnum
        {
             MQTT = 0 /* MQTT*/
            ,CAN = 1 /* CAN*/
            ,GPIO = 2 /* GPIO*/
            ,GPS = 3 /* GPS*/
            ,ISOBUS = 4 /* ISOBUS*/
            ,Serial = 5 /* Serial*/
            ,WIFI = 6 /* WIFI*/
            ,SATELLITE = 7 /* SATELLITE*/
        };
// ConnectionTypeEnum Values array.
        static const ConnectionTypeEnum ConnectionTypeEnum_Values[] =
        { ConnectionTypeEnum::MQTT, ConnectionTypeEnum::CAN, ConnectionTypeEnum::GPIO, ConnectionTypeEnum::GPS, ConnectionTypeEnum::ISOBUS, ConnectionTypeEnum::Serial, ConnectionTypeEnum::WIFI, ConnectionTypeEnum::SATELLITE };
        inline const std::string getLiteral(ConnectionTypeEnum e)
        {
            switch (e)
            {
            case ConnectionTypeEnum::MQTT:
                return "MQTT";
            case ConnectionTypeEnum::CAN:
                return "CAN";
            case ConnectionTypeEnum::GPIO:
                return "GPIO";
            case ConnectionTypeEnum::GPS:
                return "GPS";
            case ConnectionTypeEnum::ISOBUS:
                return "ISOBUS";
            case ConnectionTypeEnum::Serial:
                return "Serial";
            case ConnectionTypeEnum::WIFI:
                return "WIFI";
            case ConnectionTypeEnum::SATELLITE:
                return "SATELLITE";
            }
            return "";
        }
        ;
    } // common
} // nevonex

#endif
