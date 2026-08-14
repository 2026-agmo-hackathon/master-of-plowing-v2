/*
 * AppMain/MachineConnectListenerImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MachineConnectListener.hpp"
#include <stdexcept>

#include <AppMain/MainController.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(MachineConnectListenerImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

#include <nevonex.hpp>

#include "web/WebSocketEndPoint.hpp"
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_Tractor.hpp>
#include <nevonex/fcal/RECEIVE_ACC_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_FNR_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_HYD_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_SFT_INFO_T_R.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305.hpp>
#include <nevonex/fcal/ACEINNA_ACCEL_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLERATE_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLES_T_R.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.hpp>
#include <nevonex/fcal/MOTOR_HEARTBEAT_T_R.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183.hpp>
#include <nevonex/fcal/GPS_NMEA0183_T_R.hpp>
#include <sstream>


using namespace ::AppMain;
using namespace ::nevonex::log;

/*PROTECTED REGION ID(MachineConnectListenerImpl_Methods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
void MachineConnectListener::machineConnected(
        ::nevonex::feature::AbstractMachine_ptr machine)
{
    (void) machine;

    /*PROTECTED REGION ID(MachineConnectListener_machineConnected) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

void MachineConnectListener::machineDisconnected(
        ::nevonex::feature::AbstractMachine_ptr machine,
        ::nevonex::machine::MachineConnectionInfo_ptr machineConnectionInfo)
{
    (void) machine;
    (void) machineConnectionInfo;

    /*PROTECTED REGION ID(MachineConnectListener_machineDisconnected) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

void MachineConnectListener::propertyChange(
        ::nevonex::propertychange::PropertyChangeEvent<
                const ::ecore::EJavaObject &,
                const ::ecore::EJavaObject & > changeEvent)
{
    /*PROTECTED REGION ID(MachineConnectListener_propertyChange) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
    // 플러그인 속성 변경을 CustomUI 소켓으로 중계하지 않는다.
    //
    // 예전 로컬 시뮬레이터 디버그 페이지가 {"topic":...} 프레임을
    // 읽던 흔적인데, 지금 React UI(customui-src/src/dashboard/useTelemetry.ts)는 kind 기반
    // 프레임만 처리하고 topic 프레임은 전부 버린다. 그런데 이 중계는 초당 약
    // 290프레임을 쏟아부어 소켓 트래픽의 97%(측정: 15초에 4,229 / 4,376)를
    // 차지했고, 누적 약 4,400프레임(약 15초)마다 발행이 통째로 멎었다. 그러면
    // pp_telemetry 까지 같이 끊겨 UI 는 "프레임 정지"로 굳고 모든 조작이 잠긴다.
    // 주행 시작이 안 되던 실제 원인이다.
    //
    // 로컬 시뮬레이터를 디버깅할 때만 아래 상수를 true 로 바꾼다. 다시 켤 때는
    // 반드시 발행 경로를 스로틀하거나 구독을 좁혀라 — 무제한 중계는 소켓을 죽인다.
    //
    // Do not relay plugin property changes to the CustomUI socket.
    //
    // This mirrored every machine signal as a {"topic":...} frame for the old
    // local-sim debug page. The current React UI
    // (customui-src/src/dashboard/useTelemetry.ts) only handles kind-based frames and
    // discards topic frames outright, yet the relay pushed ~290 frames/s — 97%
    // of all socket traffic (measured: 4,229 of 4,376 frames in 15 s) — and
    // publishing wedged completely after ~4,400 frames (~15 s). pp_telemetry
    // died with it, freezing the UI as "frames stopped" and disabling every
    // control. That is why a run could not be started.
    //
    // Flip the constant below to debug against the local simulator. If you do,
    // throttle the publish path or narrow the subscription first — an
    // unthrottled relay kills the socket.
    // └────────────────────────────────────────────────────────────────────┘
    constexpr bool RELAY_MACHINE_SIGNALS_TO_CUSTOMUI = false;
    if (!RELAY_MACHINE_SIGNALS_TO_CUSTOMUI) return;
    if (!m_mainController) return;
    auto ws = m_mainController->getWebSocketEndPoint();
    if (!ws) return;
    std::string framed;
    const std::string &name = changeEvent.getPropertyName();
    std::string payload("{}");
    if (name == "receive_ACC_INFO")
    {
        auto _provider = m_mainController->getCAN_AGMO_Customized_TractorProvider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_Customized_Tractor();
            if (_machine)
            {
                ::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr _payload = _machine->getReceive_ACC_INFO();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"TRZ_ACC_SIG1_V\":" << _payload->getTRZ_ACC_SIG1_V()
                            << ",\"TRZ_ACC_SIG2_V\":" << _payload->getTRZ_ACC_SIG2_V()
                            << ",\"TRZ_ACC_DIAG\":" << _payload->getTRZ_ACC_DIAG()
                            << ",\"TRZ_ACC_AUTO\":" << _payload->getTRZ_ACC_AUTO()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "receive_FNR_INFO")
    {
        auto _provider = m_mainController->getCAN_AGMO_Customized_TractorProvider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_Customized_Tractor();
            if (_machine)
            {
                ::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr _payload = _machine->getReceive_FNR_INFO();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"TRZ_FNR_SIG1_V\":" << _payload->getTRZ_FNR_SIG1_V()
                            << ",\"TRZ_FNR_SIG2_V\":" << _payload->getTRZ_FNR_SIG2_V()
                            << ",\"TRZ_FNR_DIAG\":" << _payload->getTRZ_FNR_DIAG()
                            << ",\"TRZ_FNR_AUTO\":" << _payload->getTRZ_FNR_AUTO()
                            << ",\"TRZ_FNR_STATE\":" << _payload->getTRZ_FNR_STATE()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "receive_HYD_INFO")
    {
        auto _provider = m_mainController->getCAN_AGMO_Customized_TractorProvider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_Customized_Tractor();
            if (_machine)
            {
                ::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr _payload = _machine->getReceive_HYD_INFO();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"TRZ_HYD_SIG1_V\":" << _payload->getTRZ_HYD_SIG1_V()
                            << ",\"TRZ_HYD_SIG2_V\":" << _payload->getTRZ_HYD_SIG2_V()
                            << ",\"TRZ_HYD_DIAG\":" << _payload->getTRZ_HYD_DIAG()
                            << ",\"TRZ_HYD_AUTO\":" << _payload->getTRZ_HYD_AUTO()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "receive_SFT_INFO")
    {
        auto _provider = m_mainController->getCAN_AGMO_Customized_TractorProvider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_Customized_Tractor();
            if (_machine)
            {
                ::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr _payload = _machine->getReceive_SFT_INFO();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"TRZ_SFT_SIG1_V\":" << _payload->getTRZ_SFT_SIG1_V()
                            << ",\"TRZ_SFT_SIG2_V\":" << _payload->getTRZ_SFT_SIG2_V()
                            << ",\"TRZ_SFT_DIAG\":" << _payload->getTRZ_SFT_DIAG()
                            << ",\"TRZ_SFT_AUTO\":" << _payload->getTRZ_SFT_AUTO()
                            << ",\"TRZ_SFT_STATE\":" << _payload->getTRZ_SFT_STATE()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "aceinna_Accel")
    {
        auto _provider = m_mainController->getCAN_AGMO_MTLT305Provider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_MTLT305();
            if (_machine)
            {
                ::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr _payload = _machine->getAceinna_Accel();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"AccY\":" << _payload->getAccY()
                            << ",\"AccX\":" << _payload->getAccX()
                            << ",\"AccZ\":" << _payload->getAccZ()
                            << ",\"LateralAcc_FigureOfMerit\":" << _payload->getLateralAcc_FigureOfMerit()
                            << ",\"LongiAcc_FigureOfMerit\":" << _payload->getLongiAcc_FigureOfMerit()
                            << ",\"VerticAcc_FigureOfMerit\":" << _payload->getVerticAcc_FigureOfMerit()
                            << ",\"Support_Rate_Acc\":" << _payload->getSupport_Rate_Acc()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "aceinna_AngleRate")
    {
        auto _provider = m_mainController->getCAN_AGMO_MTLT305Provider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_MTLT305();
            if (_machine)
            {
                ::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr _payload = _machine->getAceinna_AngleRate();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"GyroY\":" << _payload->getGyroY()
                            << ",\"GyroX\":" << _payload->getGyroX()
                            << ",\"GyroZ\":" << _payload->getGyroZ()
                            << ",\"GyroY_FigureOfMerit\":" << _payload->getGyroY_FigureOfMerit()
                            << ",\"GyroX_FigureOfMerit\":" << _payload->getGyroX_FigureOfMerit()
                            << ",\"GyroZ_FigureOfMerit\":" << _payload->getGyroZ_FigureOfMerit()
                            << ",\"AngleRate_Latency\":" << _payload->getAngleRate_Latency()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "aceinna_Angles")
    {
        auto _provider = m_mainController->getCAN_AGMO_MTLT305Provider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_MTLT305();
            if (_machine)
            {
                ::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr _payload = _machine->getAceinna_Angles();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"Pitch\":" << _payload->getPitch()
                            << ",\"Roll\":" << _payload->getRoll()
                            << ",\"Pitch_Compensation\":" << _payload->getPitch_Compensation()
                            << ",\"Pitch_FigureOfMerit\":" << _payload->getPitch_FigureOfMerit()
                            << ",\"Roll_Compensation\":" << _payload->getRoll_Compensation()
                            << ",\"Roll_FigureOfMerit\":" << _payload->getRoll_FigureOfMerit()
                            << ",\"PitchRoll_Latency\":" << _payload->getPitchRoll_Latency()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "motor_Heartbeat")
    {
        auto _provider = m_mainController->getCAN_AGMO_SteerMotorProvider();
        if (_provider)
        {
            auto _machine = _provider->getCAN_AGMO_SteerMotor();
            if (_machine)
            {
                ::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr _payload = _machine->getMotor_Heartbeat();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"Heartbeat_ControlStatus_HallFailure\":" << _payload->getHeartbeat_ControlStatus_HallFailure()
                            << ",\"Heartbeat_ControlStatus_CANdisconnected\":" << _payload->getHeartbeat_ControlStatus_CANdisconnected()
                            << ",\"Heartbeat_ControlStatus_MotorStalled\":" << _payload->getHeartbeat_ControlStatus_MotorStalled()
                            << ",\"Heartbeat_ControlStatus_Disabled\":" << _payload->getHeartbeat_ControlStatus_Disabled()
                            << ",\"Heartbeat_ControlStatus_Overvoltage\":" << _payload->getHeartbeat_ControlStatus_Overvoltage()
                            << ",\"Heartbeat_ControlStatus_HardwareProtection\":" << _payload->getHeartbeat_ControlStatus_HardwareProtection()
                            << ",\"Heartbeat_ControlStatus_E2PROM\":" << _payload->getHeartbeat_ControlStatus_E2PROM()
                            << ",\"Heartbeat_ControlStatus_Undervoltage\":" << _payload->getHeartbeat_ControlStatus_Undervoltage()
                            << ",\"Heartbeat_ControlStatus_Overcurrent\":" << _payload->getHeartbeat_ControlStatus_Overcurrent()
                            << ",\"Heartbeat_ControlStatus_ModeFailure\":" << _payload->getHeartbeat_ControlStatus_ModeFailure()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    if (name == "serial_Ext_GPS_NMEA0183_Data")
    {
        auto _provider = m_mainController->getSerial_Ext_GPS_NMEA0183Provider();
        if (_provider)
        {
            auto _machine = _provider->getSerial_Ext_GPS_NMEA0183();
            if (_machine)
            {
                ::nevonex::fcal::GPS_NMEA0183_T_R_ptr _payload = _machine->getSerial_Ext_GPS_NMEA0183_Data();
                if (_payload)
                {
                    std::ostringstream _json;
                    _json << "{"
                            << "\"UTC_ms\":\"" << _payload->getUTC_ms() << "\""
                            << ",\"Latitude\":" << _payload->getLatitude()
                            << ",\"Longitude\":" << _payload->getLongitude()
                            << ",\"GPS_Quality_Indicator\":" << _payload->getGPS_Quality_Indicator()
                            << ",\"No_Satellites\":" << _payload->getNo_Satellites()
                            << ",\"Altitude\":" << _payload->getAltitude()
                            << ",\"Geoidal_Separation\":" << _payload->getGeoidal_Separation()
                            << ",\"DGPS_Age\":" << _payload->getDGPS_Age()
                            << ",\"Differential_Ref_Station\":" << _payload->getDifferential_Ref_Station()
                            << ",\"Selection_Mode\":\"" << _payload->getSelection_Mode() << "\""
                            << ",\"Fix_Type\":" << _payload->getFix_Type()
                            << ",\"PDOP\":" << _payload->getPDOP()
                            << ",\"HDOP\":" << _payload->getHDOP()
                            << ",\"VDOP\":" << _payload->getVDOP()
                            << ",\"Status\":" << _payload->getStatus()
                            << ",\"Speed\":" << _payload->getSpeed()
                            << ",\"Course\":" << _payload->getCourse()
                            << ",\"Magnetic_variation\":" << _payload->getMagnetic_variation()
                            << ",\"FAA_Mode_Indicator\":\"" << _payload->getFAA_Mode_Indicator() << "\""
                            << ",\"Heading\":" << _payload->getHeading()
                            << ",\"HeadingTrue\":" << _payload->getHeadingTrue()
                            << "}";
                    payload = _json.str();
                }
            }
        }
    }
    framed.reserve(name.size() + payload.size() + 48);
    framed.append("{\"topic\":\"fek/machine/").append(name)
            .append("\",\"payload\":").append(payload).append("}");
    ws->publishMessage(framed);
}

void MachineConnectListener::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(MachineConnectListenerImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

/*PROTECTED REGION ID(MachineConnectListenerImpl_MethodsEnd) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
