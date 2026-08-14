/*
 * nevonex/fcal/FcalPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCALPACKAGE_HPP
#define _NEVONEX_FCALPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/fcal_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/types/TypesPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace fcal
    {

    class EXPORT_NEVONEX_DLL FcalPackage : public virtual ::ecore::EPackage
    {
    public:

        static FcalPackage_ptr _instance();
        static FcalPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int ACEINNA_ACCEL_T_R = 0;

        static const int ACEINNA_ANGLERATE_T_R = 1;

        static const int ACEINNA_ANGLES_T_R = 2;

        static const int AGMO_GENERIC_SEND_CMD_T_W = 3;

        static const int BULKPROCESSOR = 4;

        static const int GPS_NMEA0183_T_R = 5;

        static const int IBULKPROCESSOR = 6;

        static const int IFCALFACTORY = 7;

        static const int MOTOR_HEARTBEAT_T_R = 8;

        static const int RAW_CAN_DATA_DLC_8_W = 9;

        static const int RECEIVE_ACC_INFO_T_R = 10;

        static const int RECEIVE_FNR_INFO_T_R = 11;

        static const int RECEIVE_HYD_INFO_T_R = 12;

        static const int RECEIVE_SFT_INFO_T_R = 13;

        static const int RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V = 0;

        static const int RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V = 1;

        static const int RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG = 2;

        static const int RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO = 3;

        static const int RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V = 4;

        static const int RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V = 5;

        static const int RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG = 6;

        static const int RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO = 7;

        static const int RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE = 8;

        static const int RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V = 9;

        static const int RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V = 10;

        static const int RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG = 11;

        static const int RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO = 12;

        static const int RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V = 13;

        static const int RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V = 14;

        static const int RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG = 15;

        static const int RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO = 16;

        static const int RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE = 17;

        static const int AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD = 18;

        static const int AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD = 19;

        static const int ACEINNA_ACCEL_T_R__ACCY = 20;

        static const int ACEINNA_ACCEL_T_R__ACCX = 21;

        static const int ACEINNA_ACCEL_T_R__ACCZ = 22;

        static const int ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT = 23;

        static const int ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT = 24;

        static const int ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT = 25;

        static const int ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC = 26;

        static const int ACEINNA_ANGLERATE_T_R__GYROY = 27;

        static const int ACEINNA_ANGLERATE_T_R__GYROX = 28;

        static const int ACEINNA_ANGLERATE_T_R__GYROZ = 29;

        static const int ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT = 30;

        static const int ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT = 31;

        static const int ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT = 32;

        static const int ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY = 33;

        static const int ACEINNA_ANGLES_T_R__PITCH = 34;

        static const int ACEINNA_ANGLES_T_R__ROLL = 35;

        static const int ACEINNA_ANGLES_T_R__PITCH_COMPENSATION = 36;

        static const int ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT = 37;

        static const int ACEINNA_ANGLES_T_R__ROLL_COMPENSATION = 38;

        static const int ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT = 39;

        static const int ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY = 40;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE = 41;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED = 42;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED = 43;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED = 44;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE = 45;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION = 46;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM = 47;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE = 48;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT = 49;

        static const int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE = 50;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE0 = 51;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE1 = 52;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE2 = 53;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE3 = 54;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE4 = 55;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE5 = 56;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE6 = 57;

        static const int RAW_CAN_DATA_DLC_8_W__BYTE7 = 58;

        static const int GPS_NMEA0183_T_R__UTC_MS = 59;

        static const int GPS_NMEA0183_T_R__LATITUDE = 60;

        static const int GPS_NMEA0183_T_R__LONGITUDE = 61;

        static const int GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR = 62;

        static const int GPS_NMEA0183_T_R__NO_SATELLITES = 63;

        static const int GPS_NMEA0183_T_R__ALTITUDE = 64;

        static const int GPS_NMEA0183_T_R__GEOIDAL_SEPARATION = 65;

        static const int GPS_NMEA0183_T_R__DGPS_AGE = 66;

        static const int GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION = 67;

        static const int GPS_NMEA0183_T_R__SELECTION_MODE = 68;

        static const int GPS_NMEA0183_T_R__FIX_TYPE = 69;

        static const int GPS_NMEA0183_T_R__PDOP = 70;

        static const int GPS_NMEA0183_T_R__HDOP = 71;

        static const int GPS_NMEA0183_T_R__VDOP = 72;

        static const int GPS_NMEA0183_T_R__STATUS = 73;

        static const int GPS_NMEA0183_T_R__SPEED = 74;

        static const int GPS_NMEA0183_T_R__COURSE = 75;

        static const int GPS_NMEA0183_T_R__MAGNETIC_VARIATION = 76;

        static const int GPS_NMEA0183_T_R__FAA_MODE_INDICATOR = 77;

        static const int GPS_NMEA0183_T_R__HEADING = 78;

        static const int GPS_NMEA0183_T_R__HEADINGTRUE = 79;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getBulkProcessor();
        virtual ::ecore::EClass_ptr getRECEIVE_ACC_INFO_T_R();
        virtual ::ecore::EClass_ptr getRECEIVE_FNR_INFO_T_R();
        virtual ::ecore::EClass_ptr getRECEIVE_HYD_INFO_T_R();
        virtual ::ecore::EClass_ptr getRECEIVE_SFT_INFO_T_R();
        virtual ::ecore::EClass_ptr getAGMO_GENERIC_SEND_CMD_T_W();
        virtual ::ecore::EClass_ptr getACEINNA_ACCEL_T_R();
        virtual ::ecore::EClass_ptr getACEINNA_ANGLERATE_T_R();
        virtual ::ecore::EClass_ptr getACEINNA_ANGLES_T_R();
        virtual ::ecore::EClass_ptr getMOTOR_HEARTBEAT_T_R();
        virtual ::ecore::EClass_ptr getRAW_CAN_DATA_DLC_8_W();
        virtual ::ecore::EClass_ptr getGPS_NMEA0183_T_R();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getRECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG();
        virtual ::ecore::EAttribute_ptr getRECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO();
        virtual ::ecore::EAttribute_ptr getRECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG();
        virtual ::ecore::EAttribute_ptr getRECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO();
        virtual ::ecore::EAttribute_ptr getRECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE();
        virtual ::ecore::EAttribute_ptr getRECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG();
        virtual ::ecore::EAttribute_ptr getRECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO();
        virtual ::ecore::EAttribute_ptr getRECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V();
        virtual ::ecore::EAttribute_ptr getRECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG();
        virtual ::ecore::EAttribute_ptr getRECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO();
        virtual ::ecore::EAttribute_ptr getRECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE();
        virtual ::ecore::EAttribute_ptr getAGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd();
        virtual ::ecore::EAttribute_ptr getAGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__accY();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__accX();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__accZ();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ACCEL_T_R__support_Rate_Acc();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroY();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroX();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroZ();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLERATE_T_R__angleRate_Latency();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__pitch();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__roll();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__pitch_Compensation();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__pitch_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__roll_Compensation();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__roll_FigureOfMerit();
        virtual ::ecore::EAttribute_ptr getACEINNA_ANGLES_T_R__pitchRoll_Latency();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent();
        virtual ::ecore::EAttribute_ptr getMOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte0();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte1();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte2();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte3();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte4();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte5();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte6();
        virtual ::ecore::EAttribute_ptr getRAW_CAN_DATA_DLC_8_W__byte7();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__uTC_ms();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__latitude();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__longitude();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__gPS_Quality_Indicator();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__no_Satellites();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__altitude();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__geoidal_Separation();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__dGPS_Age();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__differential_Ref_Station();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__selection_Mode();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__fix_Type();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__pDOP();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__hDOP();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__vDOP();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__status();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__speed();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__course();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__magnetic_variation();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__fAA_Mode_Indicator();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__heading();
        virtual ::ecore::EAttribute_ptr getGPS_NMEA0183_T_R__headingTrue();

    protected:

        static ::ecore::Ptr< FcalPackage > s_instance;

        FcalPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_BulkProcessorEClass;

        ::ecore::EClass_ptr m_RECEIVE_ACC_INFO_T_REClass;

        ::ecore::EClass_ptr m_RECEIVE_FNR_INFO_T_REClass;

        ::ecore::EClass_ptr m_RECEIVE_HYD_INFO_T_REClass;

        ::ecore::EClass_ptr m_RECEIVE_SFT_INFO_T_REClass;

        ::ecore::EClass_ptr m_AGMO_GENERIC_SEND_CMD_T_WEClass;

        ::ecore::EClass_ptr m_ACEINNA_ACCEL_T_REClass;

        ::ecore::EClass_ptr m_ACEINNA_ANGLERATE_T_REClass;

        ::ecore::EClass_ptr m_ACEINNA_ANGLES_T_REClass;

        ::ecore::EClass_ptr m_MOTOR_HEARTBEAT_T_REClass;

        ::ecore::EClass_ptr m_RAW_CAN_DATA_DLC_8_WEClass;

        ::ecore::EClass_ptr m_GPS_NMEA0183_T_REClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG1_V;

        ::ecore::EAttribute_ptr m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_SIG2_V;

        ::ecore::EAttribute_ptr m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_DIAG;

        ::ecore::EAttribute_ptr m_RECEIVE_ACC_INFO_T_R__tRZ_ACC_AUTO;

        ::ecore::EAttribute_ptr m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG1_V;

        ::ecore::EAttribute_ptr m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_SIG2_V;

        ::ecore::EAttribute_ptr m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_DIAG;

        ::ecore::EAttribute_ptr m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_AUTO;

        ::ecore::EAttribute_ptr m_RECEIVE_FNR_INFO_T_R__tRZ_FNR_STATE;

        ::ecore::EAttribute_ptr m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG1_V;

        ::ecore::EAttribute_ptr m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_SIG2_V;

        ::ecore::EAttribute_ptr m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_DIAG;

        ::ecore::EAttribute_ptr m_RECEIVE_HYD_INFO_T_R__tRZ_HYD_AUTO;

        ::ecore::EAttribute_ptr m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG1_V;

        ::ecore::EAttribute_ptr m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_SIG2_V;

        ::ecore::EAttribute_ptr m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_DIAG;

        ::ecore::EAttribute_ptr m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_AUTO;

        ::ecore::EAttribute_ptr m_RECEIVE_SFT_INFO_T_R__tRZ_SFT_STATE;

        ::ecore::EAttribute_ptr m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Cmd;

        ::ecore::EAttribute_ptr m_AGMO_GENERIC_SEND_CMD_T_W__aD_Generic_Mode_Cmd;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__accY;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__accX;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__accZ;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__lateralAcc_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__longiAcc_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__verticAcc_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ACCEL_T_R__support_Rate_Acc;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroY;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroX;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroZ;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroY_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroX_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__gyroZ_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLERATE_T_R__angleRate_Latency;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__pitch;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__roll;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__pitch_Compensation;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__pitch_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__roll_Compensation;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__roll_FigureOfMerit;

        ::ecore::EAttribute_ptr m_ACEINNA_ANGLES_T_R__pitchRoll_Latency;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HallFailure;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_CANdisconnected;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_MotorStalled;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Disabled;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overvoltage;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_HardwareProtection;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_E2PROM;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Undervoltage;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_Overcurrent;

        ::ecore::EAttribute_ptr m_MOTOR_HEARTBEAT_T_R__heartbeat_ControlStatus_ModeFailure;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte0;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte1;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte2;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte3;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte4;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte5;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte6;

        ::ecore::EAttribute_ptr m_RAW_CAN_DATA_DLC_8_W__byte7;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__uTC_ms;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__latitude;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__longitude;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__gPS_Quality_Indicator;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__no_Satellites;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__altitude;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__geoidal_Separation;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__dGPS_Age;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__differential_Ref_Station;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__selection_Mode;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__fix_Type;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__pDOP;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__hDOP;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__vDOP;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__status;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__speed;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__course;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__magnetic_variation;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__fAA_Mode_Indicator;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__heading;

        ::ecore::EAttribute_ptr m_GPS_NMEA0183_T_R__headingTrue;

    };

}
 // fcal
}// nevonex

#endif // _NEVONEX_FCALPACKAGE_HPP
