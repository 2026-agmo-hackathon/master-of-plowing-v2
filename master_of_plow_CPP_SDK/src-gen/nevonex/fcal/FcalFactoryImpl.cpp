/*
 * nevonex/fcal/FcalFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcal/FcalFactory.hpp>
#include <nevonex/fcal/FcalPackage.hpp>
#include <nevonex/fcal/BulkProcessor.hpp>
#include <nevonex/fcal/RECEIVE_ACC_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_FNR_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_HYD_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_SFT_INFO_T_R.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/ACEINNA_ACCEL_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLERATE_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLES_T_R.hpp>
#include <nevonex/fcal/MOTOR_HEARTBEAT_T_R.hpp>
#include <nevonex/fcal/RAW_CAN_DATA_DLC_8_W.hpp>
#include <nevonex/fcal/GPS_NMEA0183_T_R.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::fcal;

FcalFactory::FcalFactory()
{
}

::ecore::EObject_ptr FcalFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case FcalPackage::BULKPROCESSOR:
        return createBulkProcessor();
    case FcalPackage::RECEIVE_ACC_INFO_T_R:
        return createRECEIVE_ACC_INFO_T_R();
    case FcalPackage::RECEIVE_FNR_INFO_T_R:
        return createRECEIVE_FNR_INFO_T_R();
    case FcalPackage::RECEIVE_HYD_INFO_T_R:
        return createRECEIVE_HYD_INFO_T_R();
    case FcalPackage::RECEIVE_SFT_INFO_T_R:
        return createRECEIVE_SFT_INFO_T_R();
    case FcalPackage::AGMO_GENERIC_SEND_CMD_T_W:
        return createAGMO_GENERIC_SEND_CMD_T_W();
    case FcalPackage::ACEINNA_ACCEL_T_R:
        return createACEINNA_ACCEL_T_R();
    case FcalPackage::ACEINNA_ANGLERATE_T_R:
        return createACEINNA_ANGLERATE_T_R();
    case FcalPackage::ACEINNA_ANGLES_T_R:
        return createACEINNA_ANGLES_T_R();
    case FcalPackage::MOTOR_HEARTBEAT_T_R:
        return createMOTOR_HEARTBEAT_T_R();
    case FcalPackage::RAW_CAN_DATA_DLC_8_W:
        return createRAW_CAN_DATA_DLC_8_W();
    case FcalPackage::GPS_NMEA0183_T_R:
        return createGPS_NMEA0183_T_R();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject FcalFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    (void)_literalValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString FcalFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    (void)_instanceValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

BulkProcessor_ptr FcalFactory::createBulkProcessor()
{
    return ::ecore::Ptr < BulkProcessor > (new BulkProcessor);
}
RECEIVE_ACC_INFO_T_R_ptr FcalFactory::createRECEIVE_ACC_INFO_T_R()
{
    return ::ecore::Ptr < RECEIVE_ACC_INFO_T_R > (new RECEIVE_ACC_INFO_T_R);
}
RECEIVE_FNR_INFO_T_R_ptr FcalFactory::createRECEIVE_FNR_INFO_T_R()
{
    return ::ecore::Ptr < RECEIVE_FNR_INFO_T_R > (new RECEIVE_FNR_INFO_T_R);
}
RECEIVE_HYD_INFO_T_R_ptr FcalFactory::createRECEIVE_HYD_INFO_T_R()
{
    return ::ecore::Ptr < RECEIVE_HYD_INFO_T_R > (new RECEIVE_HYD_INFO_T_R);
}
RECEIVE_SFT_INFO_T_R_ptr FcalFactory::createRECEIVE_SFT_INFO_T_R()
{
    return ::ecore::Ptr < RECEIVE_SFT_INFO_T_R > (new RECEIVE_SFT_INFO_T_R);
}
AGMO_GENERIC_SEND_CMD_T_W_ptr FcalFactory::createAGMO_GENERIC_SEND_CMD_T_W()
{
    return ::ecore::Ptr < AGMO_GENERIC_SEND_CMD_T_W > (new AGMO_GENERIC_SEND_CMD_T_W);
}
ACEINNA_ACCEL_T_R_ptr FcalFactory::createACEINNA_ACCEL_T_R()
{
    return ::ecore::Ptr < ACEINNA_ACCEL_T_R > (new ACEINNA_ACCEL_T_R);
}
ACEINNA_ANGLERATE_T_R_ptr FcalFactory::createACEINNA_ANGLERATE_T_R()
{
    return ::ecore::Ptr < ACEINNA_ANGLERATE_T_R > (new ACEINNA_ANGLERATE_T_R);
}
ACEINNA_ANGLES_T_R_ptr FcalFactory::createACEINNA_ANGLES_T_R()
{
    return ::ecore::Ptr < ACEINNA_ANGLES_T_R > (new ACEINNA_ANGLES_T_R);
}
MOTOR_HEARTBEAT_T_R_ptr FcalFactory::createMOTOR_HEARTBEAT_T_R()
{
    return ::ecore::Ptr < MOTOR_HEARTBEAT_T_R > (new MOTOR_HEARTBEAT_T_R);
}
RAW_CAN_DATA_DLC_8_W_ptr FcalFactory::createRAW_CAN_DATA_DLC_8_W()
{
    return ::ecore::Ptr < RAW_CAN_DATA_DLC_8_W > (new RAW_CAN_DATA_DLC_8_W);
}
GPS_NMEA0183_T_R_ptr FcalFactory::createGPS_NMEA0183_T_R()
{
    return ::ecore::Ptr < GPS_NMEA0183_T_R > (new GPS_NMEA0183_T_R);
}
