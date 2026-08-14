/*
 * nevonex/fcal_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCAL_FORWARD_HPP
#define _NEVONEX_FCAL_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_fcal_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace fcal
    {

// EDataType

// EClass

// BulkProcessor
        class BulkProcessor;
        using BulkProcessor_ptr = ::ecore::Ptr<BulkProcessor>;

// RECEIVE_ACC_INFO_T_R
        class RECEIVE_ACC_INFO_T_R;
        using RECEIVE_ACC_INFO_T_R_ptr = ::ecore::Ptr<RECEIVE_ACC_INFO_T_R>;

// RECEIVE_FNR_INFO_T_R
        class RECEIVE_FNR_INFO_T_R;
        using RECEIVE_FNR_INFO_T_R_ptr = ::ecore::Ptr<RECEIVE_FNR_INFO_T_R>;

// RECEIVE_HYD_INFO_T_R
        class RECEIVE_HYD_INFO_T_R;
        using RECEIVE_HYD_INFO_T_R_ptr = ::ecore::Ptr<RECEIVE_HYD_INFO_T_R>;

// RECEIVE_SFT_INFO_T_R
        class RECEIVE_SFT_INFO_T_R;
        using RECEIVE_SFT_INFO_T_R_ptr = ::ecore::Ptr<RECEIVE_SFT_INFO_T_R>;

// AGMO_GENERIC_SEND_CMD_T_W
        class AGMO_GENERIC_SEND_CMD_T_W;
        using AGMO_GENERIC_SEND_CMD_T_W_ptr = ::ecore::Ptr<AGMO_GENERIC_SEND_CMD_T_W>;

// ACEINNA_ACCEL_T_R
        class ACEINNA_ACCEL_T_R;
        using ACEINNA_ACCEL_T_R_ptr = ::ecore::Ptr<ACEINNA_ACCEL_T_R>;

// ACEINNA_ANGLERATE_T_R
        class ACEINNA_ANGLERATE_T_R;
        using ACEINNA_ANGLERATE_T_R_ptr = ::ecore::Ptr<ACEINNA_ANGLERATE_T_R>;

// ACEINNA_ANGLES_T_R
        class ACEINNA_ANGLES_T_R;
        using ACEINNA_ANGLES_T_R_ptr = ::ecore::Ptr<ACEINNA_ANGLES_T_R>;

// MOTOR_HEARTBEAT_T_R
        class MOTOR_HEARTBEAT_T_R;
        using MOTOR_HEARTBEAT_T_R_ptr = ::ecore::Ptr<MOTOR_HEARTBEAT_T_R>;

// RAW_CAN_DATA_DLC_8_W
        class RAW_CAN_DATA_DLC_8_W;
        using RAW_CAN_DATA_DLC_8_W_ptr = ::ecore::Ptr<RAW_CAN_DATA_DLC_8_W>;

// GPS_NMEA0183_T_R
        class GPS_NMEA0183_T_R;
        using GPS_NMEA0183_T_R_ptr = ::ecore::Ptr<GPS_NMEA0183_T_R>;

// EEnum

// Package & Factory
        class FcalFactory;
        using FcalFactory_ptr = ::ecore::Ptr<FcalFactory>;
        class FcalPackage;
        using FcalPackage_ptr = ::ecore::Ptr<FcalPackage>;

    } // fcal
} // nevonex

#endif // _NEVONEX_FCAL_FORWARD_HPP
