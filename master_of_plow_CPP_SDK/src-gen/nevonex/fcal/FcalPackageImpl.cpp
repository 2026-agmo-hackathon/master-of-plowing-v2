/*
 * nevonex/fcal/FcalPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcal/FcalPackage.hpp>
#include <nevonex/fcal/FcalFactory.hpp>
#include <ecore.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EAttribute.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecore/EParameter.hpp>
#include <ecore/EEnum.hpp>
#include <ecore/EEnumLiteral.hpp>
#include <ecore/EDataType.hpp>
#include <ecore/EGenericType.hpp>
#include <ecore/ETypeParameter.hpp>
#include <ecore/EcorePackage.hpp>

using namespace ::nevonex::fcal;

FcalPackage::FcalPackage()
{

    // Feature definitions of RECEIVE_ACC_INFO_T_R
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of RECEIVE_FNR_INFO_T_R
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of RECEIVE_HYD_INFO_T_R
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of RECEIVE_SFT_INFO_T_R
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of AGMO_GENERIC_SEND_CMD_T_W
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of ACEINNA_ACCEL_T_R
    m_ACEINNA_ACCEL_T_R__accY = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__accX = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__accZ = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ACCEL_T_R__support_Rate_Acc = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of ACEINNA_ANGLERATE_T_R
    m_ACEINNA_ANGLERATE_T_R__gyroY = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__gyroX = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__gyroZ = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLERATE_T_R__angleRate_Latency = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of ACEINNA_ANGLES_T_R
    m_ACEINNA_ANGLES_T_R__pitch = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__roll = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__pitch_Compensation = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__roll_Compensation = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_ACEINNA_ANGLES_T_R__pitchRoll_Latency = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of MOTOR_HEARTBEAT_T_R
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of RAW_CAN_DATA_DLC_8_W
    m_RAW_CAN_DATA_DLC_8_W__byte0 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte1 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte2 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte3 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte4 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte5 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte6 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_RAW_CAN_DATA_DLC_8_W__byte7 = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of GPS_NMEA0183_T_R
    m_GPS_NMEA0183_T_R__uTC_ms = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__latitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__longitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__gPS_Quality_Indicator = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__no_Satellites = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__altitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__geoidal_Separation = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__dGPS_Age = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__differential_Ref_Station = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__selection_Mode = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__fix_Type = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__pDOP = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__hDOP = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__vDOP = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__status = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__speed = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__course = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__magnetic_variation = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__fAA_Mode_Indicator = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__heading = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_GPS_NMEA0183_T_R__headingTrue = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

}

void FcalPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = FcalFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // BulkProcessor
    {
        m_BulkProcessorEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_BulkProcessorEClass->setClassifierID(BULKPROCESSOR);
        m_BulkProcessorEClass->setEPackage(_this());
        getEClassifiers().push_back(m_BulkProcessorEClass);
    }

    // RECEIVE_ACC_INFO_T_R
    {
        m_RECEIVE_ACC_INFO_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_RECEIVE_ACC_INFO_T_REClass->setClassifierID(RECEIVE_ACC_INFO_T_R);
        m_RECEIVE_ACC_INFO_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_RECEIVE_ACC_INFO_T_REClass);
    }
    // m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V has already been allocated above
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V);
    m_RECEIVE_ACC_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V);
    // m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V has already been allocated above
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V);
    m_RECEIVE_ACC_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V);
    // m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG has already been allocated above
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG);
    m_RECEIVE_ACC_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG);
    // m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO has already been allocated above
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO);
    m_RECEIVE_ACC_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO);

    // RECEIVE_FNR_INFO_T_R
    {
        m_RECEIVE_FNR_INFO_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_RECEIVE_FNR_INFO_T_REClass->setClassifierID(RECEIVE_FNR_INFO_T_R);
        m_RECEIVE_FNR_INFO_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_RECEIVE_FNR_INFO_T_REClass);
    }
    // m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V has already been allocated above
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V);
    m_RECEIVE_FNR_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V);
    // m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V has already been allocated above
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V);
    m_RECEIVE_FNR_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V);
    // m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG has already been allocated above
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG);
    m_RECEIVE_FNR_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG);
    // m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO has already been allocated above
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO);
    m_RECEIVE_FNR_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO);
    // m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE has already been allocated above
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE);
    m_RECEIVE_FNR_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE);

    // RECEIVE_HYD_INFO_T_R
    {
        m_RECEIVE_HYD_INFO_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_RECEIVE_HYD_INFO_T_REClass->setClassifierID(RECEIVE_HYD_INFO_T_R);
        m_RECEIVE_HYD_INFO_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_RECEIVE_HYD_INFO_T_REClass);
    }
    // m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V has already been allocated above
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V);
    m_RECEIVE_HYD_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V);
    // m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V has already been allocated above
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V);
    m_RECEIVE_HYD_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V);
    // m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG has already been allocated above
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG);
    m_RECEIVE_HYD_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG);
    // m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO has already been allocated above
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO);
    m_RECEIVE_HYD_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO);

    // RECEIVE_SFT_INFO_T_R
    {
        m_RECEIVE_SFT_INFO_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_RECEIVE_SFT_INFO_T_REClass->setClassifierID(RECEIVE_SFT_INFO_T_R);
        m_RECEIVE_SFT_INFO_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_RECEIVE_SFT_INFO_T_REClass);
    }
    // m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V has already been allocated above
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V);
    m_RECEIVE_SFT_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V);
    // m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V has already been allocated above
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V);
    m_RECEIVE_SFT_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V);
    // m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG has already been allocated above
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG);
    m_RECEIVE_SFT_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG);
    // m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO has already been allocated above
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO);
    m_RECEIVE_SFT_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO);
    // m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE has already been allocated above
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE->setFeatureID(
            ::nevonex::fcal::FcalPackage::RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE);
    m_RECEIVE_SFT_INFO_T_REClass->getEStructuralFeatures().push_back(
            m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE);

    // AGMO_GENERIC_SEND_CMD_T_W
    {
        m_AGMO_GENERIC_SEND_CMD_T_WEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_AGMO_GENERIC_SEND_CMD_T_WEClass->setClassifierID(AGMO_GENERIC_SEND_CMD_T_W);
        m_AGMO_GENERIC_SEND_CMD_T_WEClass->setEPackage(_this());
        getEClassifiers().push_back(m_AGMO_GENERIC_SEND_CMD_T_WEClass);
    }
    // m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd has already been allocated above
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd->setFeatureID(
            ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD);
    m_AGMO_GENERIC_SEND_CMD_T_WEClass->getEStructuralFeatures().push_back(
            m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd);
    // m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd has already been allocated above
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd->setFeatureID(
            ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD);
    m_AGMO_GENERIC_SEND_CMD_T_WEClass->getEStructuralFeatures().push_back(
            m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd);

    // ACEINNA_ACCEL_T_R
    {
        m_ACEINNA_ACCEL_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ACEINNA_ACCEL_T_REClass->setClassifierID(ACEINNA_ACCEL_T_R);
        m_ACEINNA_ACCEL_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_ACEINNA_ACCEL_T_REClass);
    }
    // m_ACEINNA_ACCEL_T_R__accY has already been allocated above
    m_ACEINNA_ACCEL_T_R__accY->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCY);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__accY);
    // m_ACEINNA_ACCEL_T_R__accX has already been allocated above
    m_ACEINNA_ACCEL_T_R__accX->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCX);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__accX);
    // m_ACEINNA_ACCEL_T_R__accZ has already been allocated above
    m_ACEINNA_ACCEL_T_R__accZ->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCZ);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__accZ);
    // m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit has already been allocated above
    m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit);
    // m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit has already been allocated above
    m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit);
    // m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit has already been allocated above
    m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit);
    // m_ACEINNA_ACCEL_T_R__support_Rate_Acc has already been allocated above
    m_ACEINNA_ACCEL_T_R__support_Rate_Acc->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC);
    m_ACEINNA_ACCEL_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ACCEL_T_R__support_Rate_Acc);

    // ACEINNA_ANGLERATE_T_R
    {
        m_ACEINNA_ANGLERATE_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ACEINNA_ANGLERATE_T_REClass->setClassifierID(ACEINNA_ANGLERATE_T_R);
        m_ACEINNA_ANGLERATE_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_ACEINNA_ANGLERATE_T_REClass);
    }
    // m_ACEINNA_ANGLERATE_T_R__gyroY has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroY->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroY);
    // m_ACEINNA_ANGLERATE_T_R__gyroX has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroX->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroX);
    // m_ACEINNA_ANGLERATE_T_R__gyroZ has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroZ->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroZ);
    // m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit);
    // m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit);
    // m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit);
    // m_ACEINNA_ANGLERATE_T_R__angleRate_Latency has already been allocated above
    m_ACEINNA_ANGLERATE_T_R__angleRate_Latency->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY);
    m_ACEINNA_ANGLERATE_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLERATE_T_R__angleRate_Latency);

    // ACEINNA_ANGLES_T_R
    {
        m_ACEINNA_ANGLES_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ACEINNA_ANGLES_T_REClass->setClassifierID(ACEINNA_ANGLES_T_R);
        m_ACEINNA_ANGLES_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_ACEINNA_ANGLES_T_REClass);
    }
    // m_ACEINNA_ANGLES_T_R__pitch has already been allocated above
    m_ACEINNA_ANGLES_T_R__pitch->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__pitch);
    // m_ACEINNA_ANGLES_T_R__roll has already been allocated above
    m_ACEINNA_ANGLES_T_R__roll->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__roll);
    // m_ACEINNA_ANGLES_T_R__pitch_Compensation has already been allocated above
    m_ACEINNA_ANGLES_T_R__pitch_Compensation->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_COMPENSATION);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__pitch_Compensation);
    // m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit has already been allocated above
    m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit);
    // m_ACEINNA_ANGLES_T_R__roll_Compensation has already been allocated above
    m_ACEINNA_ANGLES_T_R__roll_Compensation->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_COMPENSATION);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__roll_Compensation);
    // m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit has already been allocated above
    m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit);
    // m_ACEINNA_ANGLES_T_R__pitchRoll_Latency has already been allocated above
    m_ACEINNA_ANGLES_T_R__pitchRoll_Latency->setFeatureID(
            ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY);
    m_ACEINNA_ANGLES_T_REClass->getEStructuralFeatures().push_back(
            m_ACEINNA_ANGLES_T_R__pitchRoll_Latency);

    // MOTOR_HEARTBEAT_T_R
    {
        m_MOTOR_HEARTBEAT_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_MOTOR_HEARTBEAT_T_REClass->setClassifierID(MOTOR_HEARTBEAT_T_R);
        m_MOTOR_HEARTBEAT_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_MOTOR_HEARTBEAT_T_REClass);
    }
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent);
    // m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure has already been allocated above
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure->setFeatureID(
            ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE);
    m_MOTOR_HEARTBEAT_T_REClass->getEStructuralFeatures().push_back(
            m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure);

    // RAW_CAN_DATA_DLC_8_W
    {
        m_RAW_CAN_DATA_DLC_8_WEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_RAW_CAN_DATA_DLC_8_WEClass->setClassifierID(RAW_CAN_DATA_DLC_8_W);
        m_RAW_CAN_DATA_DLC_8_WEClass->setEPackage(_this());
        getEClassifiers().push_back(m_RAW_CAN_DATA_DLC_8_WEClass);
    }
    // m_RAW_CAN_DATA_DLC_8_W__byte0 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte0->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE0);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte0);
    // m_RAW_CAN_DATA_DLC_8_W__byte1 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte1->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE1);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte1);
    // m_RAW_CAN_DATA_DLC_8_W__byte2 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte2->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE2);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte2);
    // m_RAW_CAN_DATA_DLC_8_W__byte3 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte3->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE3);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte3);
    // m_RAW_CAN_DATA_DLC_8_W__byte4 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte4->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE4);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte4);
    // m_RAW_CAN_DATA_DLC_8_W__byte5 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte5->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE5);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte5);
    // m_RAW_CAN_DATA_DLC_8_W__byte6 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte6->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE6);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte6);
    // m_RAW_CAN_DATA_DLC_8_W__byte7 has already been allocated above
    m_RAW_CAN_DATA_DLC_8_W__byte7->setFeatureID(
            ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE7);
    m_RAW_CAN_DATA_DLC_8_WEClass->getEStructuralFeatures().push_back(
            m_RAW_CAN_DATA_DLC_8_W__byte7);

    // GPS_NMEA0183_T_R
    {
        m_GPS_NMEA0183_T_REClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_GPS_NMEA0183_T_REClass->setClassifierID(GPS_NMEA0183_T_R);
        m_GPS_NMEA0183_T_REClass->setEPackage(_this());
        getEClassifiers().push_back(m_GPS_NMEA0183_T_REClass);
    }
    // m_GPS_NMEA0183_T_R__uTC_ms has already been allocated above
    m_GPS_NMEA0183_T_R__uTC_ms->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__UTC_MS);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__uTC_ms);
    // m_GPS_NMEA0183_T_R__latitude has already been allocated above
    m_GPS_NMEA0183_T_R__latitude->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LATITUDE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__latitude);
    // m_GPS_NMEA0183_T_R__longitude has already been allocated above
    m_GPS_NMEA0183_T_R__longitude->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LONGITUDE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__longitude);
    // m_GPS_NMEA0183_T_R__gPS_Quality_Indicator has already been allocated above
    m_GPS_NMEA0183_T_R__gPS_Quality_Indicator->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__gPS_Quality_Indicator);
    // m_GPS_NMEA0183_T_R__no_Satellites has already been allocated above
    m_GPS_NMEA0183_T_R__no_Satellites->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__NO_SATELLITES);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__no_Satellites);
    // m_GPS_NMEA0183_T_R__altitude has already been allocated above
    m_GPS_NMEA0183_T_R__altitude->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__ALTITUDE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__altitude);
    // m_GPS_NMEA0183_T_R__geoidal_Separation has already been allocated above
    m_GPS_NMEA0183_T_R__geoidal_Separation->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GEOIDAL_SEPARATION);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__geoidal_Separation);
    // m_GPS_NMEA0183_T_R__dGPS_Age has already been allocated above
    m_GPS_NMEA0183_T_R__dGPS_Age->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DGPS_AGE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__dGPS_Age);
    // m_GPS_NMEA0183_T_R__differential_Ref_Station has already been allocated above
    m_GPS_NMEA0183_T_R__differential_Ref_Station->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__differential_Ref_Station);
    // m_GPS_NMEA0183_T_R__selection_Mode has already been allocated above
    m_GPS_NMEA0183_T_R__selection_Mode->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SELECTION_MODE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__selection_Mode);
    // m_GPS_NMEA0183_T_R__fix_Type has already been allocated above
    m_GPS_NMEA0183_T_R__fix_Type->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FIX_TYPE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__fix_Type);
    // m_GPS_NMEA0183_T_R__pDOP has already been allocated above
    m_GPS_NMEA0183_T_R__pDOP->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__PDOP);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__pDOP);
    // m_GPS_NMEA0183_T_R__hDOP has already been allocated above
    m_GPS_NMEA0183_T_R__hDOP->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HDOP);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__hDOP);
    // m_GPS_NMEA0183_T_R__vDOP has already been allocated above
    m_GPS_NMEA0183_T_R__vDOP->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__VDOP);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__vDOP);
    // m_GPS_NMEA0183_T_R__status has already been allocated above
    m_GPS_NMEA0183_T_R__status->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__STATUS);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__status);
    // m_GPS_NMEA0183_T_R__speed has already been allocated above
    m_GPS_NMEA0183_T_R__speed->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SPEED);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__speed);
    // m_GPS_NMEA0183_T_R__course has already been allocated above
    m_GPS_NMEA0183_T_R__course->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__COURSE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__course);
    // m_GPS_NMEA0183_T_R__magnetic_variation has already been allocated above
    m_GPS_NMEA0183_T_R__magnetic_variation->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__MAGNETIC_VARIATION);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__magnetic_variation);
    // m_GPS_NMEA0183_T_R__fAA_Mode_Indicator has already been allocated above
    m_GPS_NMEA0183_T_R__fAA_Mode_Indicator->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FAA_MODE_INDICATOR);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__fAA_Mode_Indicator);
    // m_GPS_NMEA0183_T_R__heading has already been allocated above
    m_GPS_NMEA0183_T_R__heading->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADING);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__heading);
    // m_GPS_NMEA0183_T_R__headingTrue has already been allocated above
    m_GPS_NMEA0183_T_R__headingTrue->setFeatureID(
            ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADINGTRUE);
    m_GPS_NMEA0183_T_REClass->getEStructuralFeatures().push_back(
            m_GPS_NMEA0183_T_R__headingTrue);

    // Create enums

    // Create data types

    // Initialize package
    setName("fcal");
    setNsPrefix("fcal");
    setNsURI("com.bosch.nevonex.fcal");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // BulkProcessor
    m_BulkProcessorEClass->setName("BulkProcessor");
    m_BulkProcessorEClass->setAbstract(false);
    m_BulkProcessorEClass->setInterface(false);
    // RECEIVE_ACC_INFO_T_R
    m_RECEIVE_ACC_INFO_T_REClass->setName("RECEIVE_ACC_INFO_T_R");
    m_RECEIVE_ACC_INFO_T_REClass->setAbstract(false);
    m_RECEIVE_ACC_INFO_T_REClass->setInterface(false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V),
            "tRZ_ACC_SIG1_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V->setID(false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V),
            "tRZ_ACC_SIG2_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V->setID(false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG),
            "tRZ_ACC_DIAG", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG->setID(false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO),
            "tRZ_ACC_AUTO", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO->setID(false);
    // RECEIVE_FNR_INFO_T_R
    m_RECEIVE_FNR_INFO_T_REClass->setName("RECEIVE_FNR_INFO_T_R");
    m_RECEIVE_FNR_INFO_T_REClass->setAbstract(false);
    m_RECEIVE_FNR_INFO_T_REClass->setInterface(false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V),
            "tRZ_FNR_SIG1_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V->setID(false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V),
            "tRZ_FNR_SIG2_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V->setID(false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG),
            "tRZ_FNR_DIAG", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG->setID(false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO),
            "tRZ_FNR_AUTO", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO->setID(false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE),
            "tRZ_FNR_STATE", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE->setID(false);
    // RECEIVE_HYD_INFO_T_R
    m_RECEIVE_HYD_INFO_T_REClass->setName("RECEIVE_HYD_INFO_T_R");
    m_RECEIVE_HYD_INFO_T_REClass->setAbstract(false);
    m_RECEIVE_HYD_INFO_T_REClass->setInterface(false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V),
            "tRZ_HYD_SIG1_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V->setID(false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V),
            "tRZ_HYD_SIG2_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V->setID(false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG),
            "tRZ_HYD_DIAG", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG->setID(false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO),
            "tRZ_HYD_AUTO", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO->setID(false);
    // RECEIVE_SFT_INFO_T_R
    m_RECEIVE_SFT_INFO_T_REClass->setName("RECEIVE_SFT_INFO_T_R");
    m_RECEIVE_SFT_INFO_T_REClass->setAbstract(false);
    m_RECEIVE_SFT_INFO_T_REClass->setInterface(false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V),
            "tRZ_SFT_SIG1_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V->setID(false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V),
            "tRZ_SFT_SIG2_V", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V->setID(false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG),
            "tRZ_SFT_DIAG", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG->setID(false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO),
            "tRZ_SFT_AUTO", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO->setID(false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE),
            "tRZ_SFT_STATE", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE->setID(false);
    // AGMO_GENERIC_SEND_CMD_T_W
    m_AGMO_GENERIC_SEND_CMD_T_WEClass->setName("AGMO_GENERIC_SEND_CMD_T_W");
    m_AGMO_GENERIC_SEND_CMD_T_WEClass->setAbstract(false);
    m_AGMO_GENERIC_SEND_CMD_T_WEClass->setInterface(false);
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd),
            "aD_Generic_Cmd", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd->setID(false);
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd),
            "aD_Generic_Mode_Cmd", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd->setID(false);
    // ACEINNA_ACCEL_T_R
    m_ACEINNA_ACCEL_T_REClass->setName("ACEINNA_ACCEL_T_R");
    m_ACEINNA_ACCEL_T_REClass->setAbstract(false);
    m_ACEINNA_ACCEL_T_REClass->setInterface(false);
    m_ACEINNA_ACCEL_T_R__accY->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__accY),
            "accY", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__accY->setID(false);
    m_ACEINNA_ACCEL_T_R__accX->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__accX),
            "accX", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__accX->setID(false);
    m_ACEINNA_ACCEL_T_R__accZ->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__accZ),
            "accZ", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__accZ->setID(false);
    m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit),
            "lateralAcc_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit->setID(false);
    m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit),
            "longiAcc_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit->setID(false);
    m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit),
            "verticAcc_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit->setID(false);
    m_ACEINNA_ACCEL_T_R__support_Rate_Acc->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ACCEL_T_R__support_Rate_Acc),
            "support_Rate_Acc", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ACCEL_T_R__support_Rate_Acc->setID(false);
    // ACEINNA_ANGLERATE_T_R
    m_ACEINNA_ANGLERATE_T_REClass->setName("ACEINNA_ANGLERATE_T_R");
    m_ACEINNA_ANGLERATE_T_REClass->setAbstract(false);
    m_ACEINNA_ANGLERATE_T_REClass->setInterface(false);
    m_ACEINNA_ANGLERATE_T_R__gyroY->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroY),
            "gyroY", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroY->setID(false);
    m_ACEINNA_ANGLERATE_T_R__gyroX->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroX),
            "gyroX", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroX->setID(false);
    m_ACEINNA_ANGLERATE_T_R__gyroZ->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroZ),
            "gyroZ", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroZ->setID(false);
    m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit),
            "gyroY_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit->setID(false);
    m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit),
            "gyroX_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit->setID(false);
    m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit),
            "gyroZ_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit->setID(false);
    m_ACEINNA_ANGLERATE_T_R__angleRate_Latency->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLERATE_T_R__angleRate_Latency),
            "angleRate_Latency", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLERATE_T_R__angleRate_Latency->setID(false);
    // ACEINNA_ANGLES_T_R
    m_ACEINNA_ANGLES_T_REClass->setName("ACEINNA_ANGLES_T_R");
    m_ACEINNA_ANGLES_T_REClass->setAbstract(false);
    m_ACEINNA_ANGLES_T_REClass->setInterface(false);
    m_ACEINNA_ANGLES_T_R__pitch->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__pitch),
            "pitch", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__pitch->setID(false);
    m_ACEINNA_ANGLES_T_R__roll->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__roll),
            "roll", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__roll->setID(false);
    m_ACEINNA_ANGLES_T_R__pitch_Compensation->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__pitch_Compensation),
            "pitch_Compensation", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__pitch_Compensation->setID(false);
    m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit),
            "pitch_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit->setID(false);
    m_ACEINNA_ANGLES_T_R__roll_Compensation->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__roll_Compensation),
            "roll_Compensation", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__roll_Compensation->setID(false);
    m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit),
            "roll_FigureOfMerit", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit->setID(false);
    m_ACEINNA_ANGLES_T_R__pitchRoll_Latency->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ACEINNA_ANGLES_T_R__pitchRoll_Latency),
            "pitchRoll_Latency", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_ACEINNA_ANGLES_T_R__pitchRoll_Latency->setID(false);
    // MOTOR_HEARTBEAT_T_R
    m_MOTOR_HEARTBEAT_T_REClass->setName("MOTOR_HEARTBEAT_T_R");
    m_MOTOR_HEARTBEAT_T_REClass->setAbstract(false);
    m_MOTOR_HEARTBEAT_T_REClass->setInterface(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure),
            "heartbeat_ControlStatus_HallFailure", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected),
            "heartbeat_ControlStatus_CANdisconnected", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled),
            "heartbeat_ControlStatus_MotorStalled", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled),
            "heartbeat_ControlStatus_Disabled", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage),
            "heartbeat_ControlStatus_Overvoltage", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection),
            "heartbeat_ControlStatus_HardwareProtection", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM),
            "heartbeat_ControlStatus_E2PROM", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage),
            "heartbeat_ControlStatus_Undervoltage", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent),
            "heartbeat_ControlStatus_Overcurrent", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent->setID(false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure),
            "heartbeat_ControlStatus_ModeFailure", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure->setID(false);
    // RAW_CAN_DATA_DLC_8_W
    m_RAW_CAN_DATA_DLC_8_WEClass->setName("RAW_CAN_DATA_DLC_8_W");
    m_RAW_CAN_DATA_DLC_8_WEClass->setAbstract(false);
    m_RAW_CAN_DATA_DLC_8_WEClass->setInterface(false);
    m_RAW_CAN_DATA_DLC_8_W__byte0->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte0),
            "byte0", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte0->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte1->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte1),
            "byte1", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte1->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte2->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte2),
            "byte2", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte2->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte3->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte3),
            "byte3", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte3->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte4->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte4),
            "byte4", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte4->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte5->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte5),
            "byte5", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte5->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte6->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte6),
            "byte6", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte6->setID(false);
    m_RAW_CAN_DATA_DLC_8_W__byte7->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_RAW_CAN_DATA_DLC_8_W__byte7),
            "byte7", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_RAW_CAN_DATA_DLC_8_W__byte7->setID(false);
    // GPS_NMEA0183_T_R
    m_GPS_NMEA0183_T_REClass->setName("GPS_NMEA0183_T_R");
    m_GPS_NMEA0183_T_REClass->setAbstract(false);
    m_GPS_NMEA0183_T_REClass->setInterface(false);
    m_GPS_NMEA0183_T_R__uTC_ms->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__uTC_ms),
            "uTC_ms", true, false, false, """", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__uTC_ms->setID(false);
    m_GPS_NMEA0183_T_R__latitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEDouble());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__latitude),
            "latitude", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__latitude->setID(false);
    m_GPS_NMEA0183_T_R__longitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEDouble());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__longitude),
            "longitude", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__longitude->setID(false);
    m_GPS_NMEA0183_T_R__gPS_Quality_Indicator->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__gPS_Quality_Indicator),
            "gPS_Quality_Indicator", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__gPS_Quality_Indicator->setID(false);
    m_GPS_NMEA0183_T_R__no_Satellites->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__no_Satellites),
            "no_Satellites", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__no_Satellites->setID(false);
    m_GPS_NMEA0183_T_R__altitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__altitude),
            "altitude", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__altitude->setID(false);
    m_GPS_NMEA0183_T_R__geoidal_Separation->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__geoidal_Separation),
            "geoidal_Separation", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__geoidal_Separation->setID(false);
    m_GPS_NMEA0183_T_R__dGPS_Age->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__dGPS_Age),
            "dGPS_Age", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__dGPS_Age->setID(false);
    m_GPS_NMEA0183_T_R__differential_Ref_Station->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__differential_Ref_Station),
            "differential_Ref_Station", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__differential_Ref_Station->setID(false);
    m_GPS_NMEA0183_T_R__selection_Mode->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__selection_Mode),
            "selection_Mode", true, false, false, """", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__selection_Mode->setID(false);
    m_GPS_NMEA0183_T_R__fix_Type->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__fix_Type),
            "fix_Type", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__fix_Type->setID(false);
    m_GPS_NMEA0183_T_R__pDOP->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__pDOP),
            "pDOP", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__pDOP->setID(false);
    m_GPS_NMEA0183_T_R__hDOP->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__hDOP),
            "hDOP", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__hDOP->setID(false);
    m_GPS_NMEA0183_T_R__vDOP->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__vDOP),
            "vDOP", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__vDOP->setID(false);
    m_GPS_NMEA0183_T_R__status->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__status),
            "status", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__status->setID(false);
    m_GPS_NMEA0183_T_R__speed->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__speed),
            "speed", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__speed->setID(false);
    m_GPS_NMEA0183_T_R__course->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__course),
            "course", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__course->setID(false);
    m_GPS_NMEA0183_T_R__magnetic_variation->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__magnetic_variation),
            "magnetic_variation", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__magnetic_variation->setID(false);
    m_GPS_NMEA0183_T_R__fAA_Mode_Indicator->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__fAA_Mode_Indicator),
            "fAA_Mode_Indicator", true, false, false, """", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__fAA_Mode_Indicator->setID(false);
    m_GPS_NMEA0183_T_R__heading->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__heading),
            "heading", true, false, false, "0.0f", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__heading->setID(false);
    m_GPS_NMEA0183_T_R__headingTrue->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_GPS_NMEA0183_T_R__headingTrue),
            "headingTrue", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_GPS_NMEA0183_T_R__headingTrue->setID(false);

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_AGMO_GENERIC_SEND_CMD_T_WEClass, ::ecore::EClassifier_ptr(),
                "initialize", 0, 1, true, true);
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "aD_Generic_Cmd", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "aD_Generic_Mode_Cmd", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_RAW_CAN_DATA_DLC_8_WEClass, ::ecore::EClassifier_ptr(),
                "initialize", 0, 1, true, true);
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte0", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte1", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte2", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte3", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte4", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte5", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte6", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "byte7", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
    }

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr FcalPackage::getBulkProcessor()
{
    return m_BulkProcessorEClass;
}
::ecore::EClass_ptr FcalPackage::getRECEIVE_ACC_INFO_T_R()
{
    return m_RECEIVE_ACC_INFO_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R()
{
    return m_RECEIVE_FNR_INFO_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getRECEIVE_HYD_INFO_T_R()
{
    return m_RECEIVE_HYD_INFO_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R()
{
    return m_RECEIVE_SFT_INFO_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getAGMO_GENERIC_SEND_CMD_T_W()
{
    return m_AGMO_GENERIC_SEND_CMD_T_WEClass;
}
::ecore::EClass_ptr FcalPackage::getACEINNA_ACCEL_T_R()
{
    return m_ACEINNA_ACCEL_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getACEINNA_ANGLERATE_T_R()
{
    return m_ACEINNA_ANGLERATE_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getACEINNA_ANGLES_T_R()
{
    return m_ACEINNA_ANGLES_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R()
{
    return m_MOTOR_HEARTBEAT_T_REClass;
}
::ecore::EClass_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W()
{
    return m_RAW_CAN_DATA_DLC_8_WEClass;
}
::ecore::EClass_ptr FcalPackage::getGPS_NMEA0183_T_R()
{
    return m_GPS_NMEA0183_T_REClass;
}

::ecore::EAttribute_ptr FcalPackage::getRECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V()
{
    return m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V()
{
    return m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG()
{
    return m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO()
{
    return m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V()
{
    return m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V()
{
    return m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG()
{
    return m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO()
{
    return m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE()
{
    return m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V()
{
    return m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V()
{
    return m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG()
{
    return m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO()
{
    return m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V()
{
    return m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V()
{
    return m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG()
{
    return m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO()
{
    return m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO;
}
::ecore::EAttribute_ptr FcalPackage::getRECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE()
{
    return m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE;
}
::ecore::EAttribute_ptr FcalPackage::getAGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd()
{
    return m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd;
}
::ecore::EAttribute_ptr FcalPackage::getAGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd()
{
    return m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__accY()
{
    return m_ACEINNA_ACCEL_T_R__accY;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__accX()
{
    return m_ACEINNA_ACCEL_T_R__accX;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__accZ()
{
    return m_ACEINNA_ACCEL_T_R__accZ;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit()
{
    return m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit()
{
    return m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit()
{
    return m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ACCEL_T_R__support_Rate_Acc()
{
    return m_ACEINNA_ACCEL_T_R__support_Rate_Acc;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroY()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroY;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroX()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroX;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroZ()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroZ;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit()
{
    return m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLERATE_T_R__angleRate_Latency()
{
    return m_ACEINNA_ANGLERATE_T_R__angleRate_Latency;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__pitch()
{
    return m_ACEINNA_ANGLES_T_R__pitch;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__roll()
{
    return m_ACEINNA_ANGLES_T_R__roll;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__pitch_Compensation()
{
    return m_ACEINNA_ANGLES_T_R__pitch_Compensation;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__pitch_FigureOfMerit()
{
    return m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__roll_Compensation()
{
    return m_ACEINNA_ANGLES_T_R__roll_Compensation;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__roll_FigureOfMerit()
{
    return m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit;
}
::ecore::EAttribute_ptr FcalPackage::getACEINNA_ANGLES_T_R__pitchRoll_Latency()
{
    return m_ACEINNA_ANGLES_T_R__pitchRoll_Latency;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent;
}
::ecore::EAttribute_ptr FcalPackage::getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure()
{
    return m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte0()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte0;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte1()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte1;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte2()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte2;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte3()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte3;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte4()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte4;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte5()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte5;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte6()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte6;
}
::ecore::EAttribute_ptr FcalPackage::getRAW_CAN_DATA_DLC_8_W__byte7()
{
    return m_RAW_CAN_DATA_DLC_8_W__byte7;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__uTC_ms()
{
    return m_GPS_NMEA0183_T_R__uTC_ms;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__latitude()
{
    return m_GPS_NMEA0183_T_R__latitude;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__longitude()
{
    return m_GPS_NMEA0183_T_R__longitude;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__gPS_Quality_Indicator()
{
    return m_GPS_NMEA0183_T_R__gPS_Quality_Indicator;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__no_Satellites()
{
    return m_GPS_NMEA0183_T_R__no_Satellites;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__altitude()
{
    return m_GPS_NMEA0183_T_R__altitude;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__geoidal_Separation()
{
    return m_GPS_NMEA0183_T_R__geoidal_Separation;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__dGPS_Age()
{
    return m_GPS_NMEA0183_T_R__dGPS_Age;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__differential_Ref_Station()
{
    return m_GPS_NMEA0183_T_R__differential_Ref_Station;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__selection_Mode()
{
    return m_GPS_NMEA0183_T_R__selection_Mode;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__fix_Type()
{
    return m_GPS_NMEA0183_T_R__fix_Type;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__pDOP()
{
    return m_GPS_NMEA0183_T_R__pDOP;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__hDOP()
{
    return m_GPS_NMEA0183_T_R__hDOP;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__vDOP()
{
    return m_GPS_NMEA0183_T_R__vDOP;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__status()
{
    return m_GPS_NMEA0183_T_R__status;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__speed()
{
    return m_GPS_NMEA0183_T_R__speed;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__course()
{
    return m_GPS_NMEA0183_T_R__course;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__magnetic_variation()
{
    return m_GPS_NMEA0183_T_R__magnetic_variation;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__fAA_Mode_Indicator()
{
    return m_GPS_NMEA0183_T_R__fAA_Mode_Indicator;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__heading()
{
    return m_GPS_NMEA0183_T_R__heading;
}
::ecore::EAttribute_ptr FcalPackage::getGPS_NMEA0183_T_R__headingTrue()
{
    return m_GPS_NMEA0183_T_R__headingTrue;
}
