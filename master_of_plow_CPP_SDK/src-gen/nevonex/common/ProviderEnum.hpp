/*
 * nevonex/common/ProviderEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_PROVIDERENUM_HPP
#define NEVONEX_COMMON_PROVIDERENUM_HPP

#include <string>

namespace nevonex
{
    namespace common
    {

        enum class ProviderEnum
        {
             CAN_AGMO_Customized_TractorProvider = 0 /* CAN_AGMO_Customized_TractorProvider*/
            ,CAN_AGMO_MTLT305Provider = 1 /* CAN_AGMO_MTLT305Provider*/
            ,CAN_AGMO_SteerMotorProvider = 2 /* CAN_AGMO_SteerMotorProvider*/
            ,ISOPGNProvider = 3 /* ISOPGNProvider*/
            ,Serial_Ext_GPS_NMEA0183Provider = 4 /* Serial_Ext_GPS_NMEA0183Provider*/
        };
// ProviderEnum Values array.
        static const ProviderEnum ProviderEnum_Values[] =
        { ProviderEnum::CAN_AGMO_Customized_TractorProvider, ProviderEnum::CAN_AGMO_MTLT305Provider, ProviderEnum::CAN_AGMO_SteerMotorProvider, ProviderEnum::ISOPGNProvider, ProviderEnum::Serial_Ext_GPS_NMEA0183Provider };
        inline const std::string getLiteral(ProviderEnum e)
        {
            switch (e)
            {
            case ProviderEnum::CAN_AGMO_Customized_TractorProvider:
                return "CAN_AGMO_Customized_TractorProvider";
            case ProviderEnum::CAN_AGMO_MTLT305Provider:
                return "CAN_AGMO_MTLT305Provider";
            case ProviderEnum::CAN_AGMO_SteerMotorProvider:
                return "CAN_AGMO_SteerMotorProvider";
            case ProviderEnum::ISOPGNProvider:
                return "ISOPGNProvider";
            case ProviderEnum::Serial_Ext_GPS_NMEA0183Provider:
                return "Serial_Ext_GPS_NMEA0183Provider";
            }
            return "";
        }
        ;
    } // common
} // nevonex

#endif
