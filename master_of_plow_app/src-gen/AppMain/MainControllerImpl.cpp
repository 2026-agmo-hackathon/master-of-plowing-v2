/*
 * AppMain/MainControllerImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MainController.hpp"
#include <stdexcept>

#include <AppMain/ApplicationInputData.hpp>
#include <AppMain/IController.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>
#include <nevonex/isopgn/ISOPGNProvider.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(MainControllerImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <nevonex.hpp>

#include "web/WebSocketEndPoint.hpp"

/*PROTECTED REGION ID(MainControllerImpl_Headers) ENABLED START*/
#include <AppMain/tracking/TrackerTypes.hpp>
#include <AppMain/tracking/TrackingLoop.hpp>
#include <algorithm>
#include <mutex>
#include <thread>
/*PROTECTED REGION END*/

using namespace ::ecore;
using namespace ::ecorecpp::mapping;

using namespace ::nevonex;
using namespace ::nevonex::fcal;
using namespace ::nevonex::common;
using namespace ::nevonex::types;

using namespace ::nevonex::can_agmo_customized_tractor;

using namespace ::nevonex::can_agmo_mtlt305;

using namespace ::nevonex::can_agmo_steermotor;

using namespace ::nevonex::isopgn;

using namespace ::nevonex::serial_ext_gps_nmea0183;

using namespace ::AppMain;
using namespace ::nevonex::log;

/*PROTECTED REGION ID(MainControllerImpl_Methods) ENABLED START*/
namespace {

// ── KY170 조향 모터 CAN 프레임 / KY170 steering motor CAN frames ─────────────
// 매뉴얼 4.5.2 기준. 앞 4바이트가 명령 종류, 뒤 4바이트가 값입니다.
// Per manual 4.5.2: first four bytes select the command, last four carry the value.
const int CMD_ENABLE[4]      = { 0x23, 0x0D, 0x20, 0x01 };
const int CMD_DISABLE[4]     = { 0x23, 0x0C, 0x20, 0x01 };
const int CMD_SPEED[4]       = { 0x23, 0x00, 0x20, 0x01 };
const int QUERY_ENC_COUNT[4] = { 0x40, 0x04, 0x21, 0x01 };
const int QUERY_ENC_SPEED[4] = { 0x40, 0x03, 0x21, 0x01 };
const int ZERO_VALUE[4]      = { 0x00, 0x00, 0x00, 0x00 };

// ┌─ 한국어 ───────────────────────────────────────────────────────────────────┐
// 32비트 값의 바이트 배치가 특이합니다. 매뉴얼 표기로
// DATA_L(H) DATA_L(L) DATA_H(H) DATA_H(L) — 하위워드가 먼저 오고 각 워드는
// 빅엔디언입니다.
//   +1000 = 0x000003E8 -> 03 E8 00 00
//   -1000 = 0xFFFFFC18 -> FC 18 FF FF
// (value >> 8) 이 음수에서 산술 시프트에 의존하므로 인자는 int 여야 합니다.
// └────────────────────────────────────────────────────────────────────────────┘
// ┌─ English ──────────────────────────────────────────────────────────────────┐
// Unusual byte order: low word first, each word big-endian. Relies on an
// arithmetic right shift for negatives, so the argument must be int.
// └────────────────────────────────────────────────────────────────────────────┘
void fillValueBytes(int value, int out[4])
{
    out[0] = (value >> 8) & 0xFF;
    out[1] = value & 0xFF;
    out[2] = (value < 0) ? 0xFF : 0x00;
    out[3] = (value < 0) ? 0xFF : 0x00;
}

void sendSteerFrame(
        const ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr &motor,
        const ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr &frame,
        const int header[4], const int value[4])
{
    frame.get()->initialize(header[0], header[1], header[2], header[3], value[0],
            value[1], value[2], value[3]);
    motor->setMotor_Request(frame);
}

/**
 * 네 개 트랙터 CMD 가 공유하는 페이로드입니다.
 * authority 는 자율 제어권 요청 비트입니다 (1 = 요청, 0 = 해제). 해제 상태에서는
 * 값도 0으로 보냅니다 — 제어권을 놓는 동안 마지막 지령이 남아 있으면 안 됩니다.
 *
 * The payload shared by all four tractor commands. `authority` is the autonomous
 * request bit; when released the value is zeroed too, so no stale command stands.
 */
::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr makeTractorCmd(int value,
        bool authority)
{
    ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr data = ::ecore::Ptr<
            ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W >(
            new ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W());
    data.get()->setAD_Generic_Mode_Cmd(authority ? 0x01 : 0x00);
    data.get()->setAD_Generic_Cmd(authority ? value : 0x00);
    return data;
}

const double KNOTS_TO_MPS = 0.514444;
// NMEA 헤딩이 없을 때 오는 센티널. 참조 구현과 같은 값입니다.
// Sentinel for a missing NMEA heading, same value as the reference implementation.
const double NMEA_HEADING_INVALID = -99.99;

} // namespace
/*PROTECTED REGION END*/
void MainController::run()
{
    /*PROTECTED REGION ID(MainControllerImpl_runStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    try
    {
        // Write your application logic by enabling the protected regions.

        using namespace ::nevonex::can_agmo_customized_tractor;

        CAN_AGMO_Customized_TractorProvider_ptr cAN_AGMO_Customized_TractorProvider =
                getCAN_AGMO_Customized_TractorProvider();
        if (cAN_AGMO_Customized_TractorProvider != nullptr)
        {
            /*PROTECTED REGION ID(MainControllerRun_CAN_AGMO_Customized_TractorProviderInnerMethod) ENABLED START*/
            // ┌─ 한국어 ───────────────────────────────────────────────────────┐
            // 트랙터 상태를 읽고 네 개 지령을 씁니다.
            //
            // 지령은 제어권이 없어도 매 주기 보냅니다 (mode=0, cmd=0). 보내기를
            // 멈추는 것과 제어권을 해제하는 것은 다르고, 참조 구현도 그렇습니다.
            //
            // 이 블록이 다섯 개 중 첫 번째라서, 여기서 쓰는 지령은 이번 주기의
            // GPS·IMU 를 아직 읽지 않은 상태의 것입니다. 다만 지령 자체가 원래
            // TrackingLoop 스레드에서 비동기로 만들어지므로(MachineOutputs),
            // 어느 순서로 써도 한 주기 이상 묵은 값이라는 점은 같습니다.
            // └────────────────────────────────────────────────────────────────┘
            // ┌─ English ──────────────────────────────────────────────────────┐
            // Reads tractor state and writes the four commands.
            //
            // Commands go out every tick even without authority (mode=0, cmd=0):
            // ceasing to send is not the same as releasing, and the reference
            // implementation behaves the same way.
            //
            // This is the first of the five blocks, so the command written here
            // predates this tick's GPS/IMU reads. That costs nothing, because the
            // command is produced asynchronously on the TrackingLoop thread anyway
            // and is always at least one tick old whatever the order.
            // └────────────────────────────────────────────────────────────────┘
            if (CAN_AGMO_Customized_Tractor_ptr tractor =
                    cAN_AGMO_Customized_TractorProvider->getCAN_AGMO_Customized_Tractor())
            {
                ::AppMain::MachineInputs &in = machineInputScratch();

                if (tractor->isReceive_FNR_INFO_Valid())
                {
                    if (RECEIVE_FNR_INFO_T_R_ptr info = tractor->getReceive_FNR_INFO())
                    {
                        in.fnrState = static_cast< int >(info->getTRZ_FNR_STATE());
                        in.fnrAuto = (info->getTRZ_FNR_AUTO() == 1);
                        in.fnrDiag = static_cast< int >(info->getTRZ_FNR_DIAG());
                        in.valid = true;
                    }
                }
                if (tractor->isReceive_SFT_INFO_Valid())
                {
                    if (RECEIVE_SFT_INFO_T_R_ptr info = tractor->getReceive_SFT_INFO())
                    {
                        in.shiftState = static_cast< int >(info->getTRZ_SFT_STATE());
                        in.sftAuto = (info->getTRZ_SFT_AUTO() == 1);
                        in.sftDiag = static_cast< int >(info->getTRZ_SFT_DIAG());
                        in.valid = true;
                    }
                }
                if (tractor->isReceive_ACC_INFO_Valid())
                {
                    if (RECEIVE_ACC_INFO_T_R_ptr info = tractor->getReceive_ACC_INFO())
                    {
                        in.accSig1V = static_cast< double >(info->getTRZ_ACC_SIG1_V());
                        in.accSig2V = static_cast< double >(info->getTRZ_ACC_SIG2_V());
                        in.accAuto = (info->getTRZ_ACC_AUTO() == 1);
                        in.accDiag = static_cast< int >(info->getTRZ_ACC_DIAG());
                        in.valid = true;
                    }
                }
                if (tractor->isReceive_HYD_INFO_Valid())
                {
                    if (RECEIVE_HYD_INFO_T_R_ptr info = tractor->getReceive_HYD_INFO())
                    {
                        in.hydAuto = (info->getTRZ_HYD_AUTO() == 1);
                        in.hydDiag = static_cast< int >(info->getTRZ_HYD_DIAG());
                        in.valid = true;
                    }
                }

                const ::AppMain::MachineOutputs out = getMachineOutputs();
                const bool authority = out.valid && out.authority;
                const auto accCmd = makeTractorCmd(
                        std::clamp(out.accelPct, 0, 100), authority);
                const auto fnrCmd = makeTractorCmd(
                        std::clamp(out.fnr, 1, 3), authority);
                const auto sftCmd = makeTractorCmd(
                        std::clamp(out.shift, 1, 3), authority);
                const auto hydCmd = makeTractorCmd(
                        std::clamp(out.hitchPct, 0, 100), authority);
                tractor->setSend_ACC_CMD(accCmd);
                tractor->setSend_FNR_CMD(fnrCmd);
                tractor->setSend_SFT_CMD(sftCmd);
                tractor->setSend_HYD_CMD(hydCmd);
                if (authority) {
                    std::cerr << "MACHINE_PUB"
                              << " valid=" << (out.valid ? 1 : 0)
                              << " auth=" << (out.authority ? 1 : 0)
                              << " out_acc=" << out.accelPct
                              << " out_fnr=" << out.fnr
                              << " out_sft=" << out.shift
                              << " obj_acc=" << accCmd->getAD_Generic_Cmd()
                              << "/" << accCmd->getAD_Generic_Mode_Cmd()
                              << " obj_fnr=" << fnrCmd->getAD_Generic_Cmd()
                              << "/" << fnrCmd->getAD_Generic_Mode_Cmd()
                              << " obj_sft=" << sftCmd->getAD_Generic_Cmd()
                              << "/" << sftCmd->getAD_Generic_Mode_Cmd()
                              << std::endl;
                }
            }
            /*PROTECTED REGION END*/
        }

        using namespace ::nevonex::can_agmo_mtlt305;

        CAN_AGMO_MTLT305Provider_ptr cAN_AGMO_MTLT305Provider =
                getCAN_AGMO_MTLT305Provider();
        // machineInputScratch_ persists across ProcessTimer cycles so retained
        // numeric values remain available for diagnosis. Validity does not:
        // every IMU interface must prove itself again on this cycle, including
        // when the provider/object is absent or an individual payload is invalid.
        machineInputScratch().attitudeValid = false;
        machineInputScratch().gyroValid = false;
        machineInputScratch().accelValid = false;
        if (cAN_AGMO_MTLT305Provider != nullptr)
        {
            /*PROTECTED REGION ID(MainControllerRun_CAN_AGMO_MTLT305ProviderInnerMethod) ENABLED START*/
            ::AppMain::MachineInputs &in = machineInputScratch();
            in.gyroValid = false;
            if (CAN_AGMO_MTLT305_ptr imu =
                    cAN_AGMO_MTLT305Provider->getCAN_AGMO_MTLT305())
            {
                // 인터페이스가 도(deg)와 deg/s 로 주므로 여기서는 그대로 담고,
                // 라디안 변환은 VehicleState 를 만들 때 한 번만 합니다.
                // The interface reports degrees; the single conversion to radians
                // happens when VehicleState is built.
                if (imu->isAceinna_Angles_Valid())
                {
                    if (ACEINNA_ANGLES_T_R_ptr angles = imu->getAceinna_Angles())
                    {
                        in.rollDeg = static_cast< double >(angles->getRoll());
                        in.pitchDeg = static_cast< double >(angles->getPitch());
                        in.attitudeValid = true;
                        in.valid = true;
                    }
                }
                const std::int64_t gyroSampleId = static_cast<std::int64_t>(
                        imu->getAceinna_AngleRate_Timestamp());
                const bool gyroFresh = gyroFreshness_.observe(gyroSampleId,
                        imu->isAceinna_AngleRate_Valid());
                in.gyroSampleId = gyroSampleId;
                in.gyroValid = false;
                if (gyroFresh)
                {
                    if (ACEINNA_ANGLERATE_T_R_ptr rate = imu->getAceinna_AngleRate())
                    {
                        in.gyroZDegS = static_cast< double >(rate->getGyroZ());
                        in.gyroValid = true;
                        in.valid = true;
                    }
                }
                if (imu->isAceinna_Accel_Valid())
                {
                    if (ACEINNA_ACCEL_T_R_ptr accel = imu->getAceinna_Accel())
                    {
                        in.accelX = static_cast< double >(accel->getAccX());
                        in.accelY = static_cast< double >(accel->getAccY());
                        in.accelValid = true;
                        in.valid = true;
                    }
                }
            }
            /*PROTECTED REGION END*/
        }

        using namespace ::nevonex::can_agmo_steermotor;

        CAN_AGMO_SteerMotorProvider_ptr cAN_AGMO_SteerMotorProvider =
                getCAN_AGMO_SteerMotorProvider();
        machineInputScratch().steerEncoderValid = false;
        if (cAN_AGMO_SteerMotorProvider != nullptr)
        {
            /*PROTECTED REGION ID(MainControllerRun_CAN_AGMO_SteerMotorProviderInnerMethod) ENABLED START*/
            ::AppMain::MachineInputs &in = machineInputScratch();
            in.steerEncoderValid = false;
            if (CAN_AGMO_SteerMotor_ptr motor =
                    cAN_AGMO_SteerMotorProvider->getCAN_AGMO_SteerMotor())
            {
                const bool encoderFresh = steerEncoderFreshness_.observe(
                        static_cast< std::int64_t >(
                                motor->getMotor_Response_EncoderCountValue_Timestamp()),
                        motor->isMotor_Response_EncoderCountValue_Valid());
                const double encoderValue = static_cast< double >(
                        motor->getMotor_Response_EncoderCountValue());
                in.steerEncoderSampleId = static_cast<std::int64_t>(
                        motor->getMotor_Response_EncoderCountValue_Timestamp());
                in.steerEncoderValid = encoderFresh;
                if (encoderFresh)
                {
                    in.steerEncoderDeg = encoderValue;
                    in.valid = true;
                }
                if (motor->isMotor_Response_Encoder_Speed_Valid())
                {
                    in.steerEncoderSpeedRpm = static_cast< int >(
                            motor->getMotor_Response_Encoder_Speed());
                    in.steerEncoderSpeedValid = true;
                }
                if (motor->isMotor_Heartbeat_Valid())
                {
                    if (MOTOR_HEARTBEAT_T_R_ptr hb = motor->getMotor_Heartbeat())
                    {
                        using namespace ::AppMain::tracking;
                        unsigned int bits = 0u;
                        if (hb->getHeartbeat_ControlStatus_HallFailure())
                            bits |= MOTOR_FAULT_HALL;
                        if (hb->getHeartbeat_ControlStatus_CANdisconnected())
                            bits |= MOTOR_FAULT_CAN_DISCONNECTED;
                        if (hb->getHeartbeat_ControlStatus_MotorStalled())
                            bits |= MOTOR_FAULT_STALLED;
                        if (hb->getHeartbeat_ControlStatus_Disabled())
                            bits |= MOTOR_FAULT_DISABLED;
                        if (hb->getHeartbeat_ControlStatus_Overvoltage())
                            bits |= MOTOR_FAULT_OVERVOLTAGE;
                        if (hb->getHeartbeat_ControlStatus_HardwareProtection())
                            bits |= MOTOR_FAULT_HW_PROTECTION;
                        if (hb->getHeartbeat_ControlStatus_E2PROM())
                            bits |= MOTOR_FAULT_E2PROM;
                        if (hb->getHeartbeat_ControlStatus_Undervoltage())
                            bits |= MOTOR_FAULT_UNDERVOLTAGE;
                        if (hb->getHeartbeat_ControlStatus_Overcurrent())
                            bits |= MOTOR_FAULT_OVERCURRENT;
                        if (hb->getHeartbeat_ControlStatus_ModeFailure())
                            bits |= MOTOR_FAULT_MODE_FAILURE;
                        in.motorFaultBits = bits;
                        in.motorHeartbeatValid = true;
                        in.valid = true;
                    }
                }

                // 프레임 객체를 매 주기 새로 만들지 않고 하나를 재사용합니다.
                // initialize() 가 8바이트를 전부 덮어쓰므로 잔상이 남지 않습니다.
                // One frame object is reused; initialize() overwrites all eight
                // bytes, so nothing carries over between sends.
                if (!steerFrame_)
                {
                    steerFrame_ = ::ecore::Ptr< RAW_CAN_DATA_DLC_8_W >(
                            new RAW_CAN_DATA_DLC_8_W());
                }

                const ::AppMain::MachineOutputs out = getMachineOutputs();
                const int speed = out.valid ? out.motorCmd : 0;

                if (speed != 0)
                {
                    if (!steerMotorEnabled_)
                    {
                        sendSteerFrame(motor, steerFrame_, CMD_ENABLE, ZERO_VALUE);
                        steerMotorEnabled_ = true;
                    }
                    int value[4];
                    fillValueBytes(speed, value);
                    sendSteerFrame(motor, steerFrame_, CMD_SPEED, value);
                }
                else
                {
                    // 지령 0 은 Disable 입니다. KY170 에는 "속도 0 유지"와 Disable
                    // 을 구분할 수단이 없고, 참조 구현도 이렇게 씁니다.
                    // A zero command means Disable: the KY170 cannot express "hold
                    // at zero speed", and the reference does the same.
                    sendSteerFrame(motor, steerFrame_, CMD_DISABLE, ZERO_VALUE);
                    steerMotorEnabled_ = false;
                }

                // ┌─ 한국어 ───────────────────────────────────────────────────┐
                // 엔코더 응답은 Adhoc 입니다 — 질의를 보내야 옵니다. 지령 뒤에
                // 붙여 보내면 되고 별도 스케줄러가 필요하지 않습니다. 매 주기
                // 지령이 나가므로 모터의 1000ms 워치독도 자동 충족됩니다.
                // └────────────────────────────────────────────────────────────┘
                // ┌─ English ──────────────────────────────────────────────────┐
                // The encoder responses are Adhoc: they arrive only if queried. The
                // queries ride along after the command, so no scheduler is needed,
                // and a command every tick satisfies the motor's 1000 ms watchdog.
                // └────────────────────────────────────────────────────────────┘
                sendSteerFrame(motor, steerFrame_, QUERY_ENC_COUNT, ZERO_VALUE);
                sendSteerFrame(motor, steerFrame_, QUERY_ENC_SPEED, ZERO_VALUE);
            }
            /*PROTECTED REGION END*/
        }

        using namespace ::nevonex::isopgn;

        ISOPGNProvider_ptr iSOPGNProvider =
                getISOPGNProvider();
        if (iSOPGNProvider != nullptr)
        {
            /*PROTECTED REGION ID(MainControllerRun_ISOPGNProviderInnerMethod) ENABLED START*/
            if (ISOPGN_ptr isopgn = iSOPGNProvider->getISOPGN())
            {
                if (isopgn->isHitchPosPerRe_Valid())
                {
                    ::AppMain::MachineInputs &in = machineInputScratch();
                    in.hitchPct = static_cast< double >(isopgn->getHitchPosPerRe());
                    in.hitchValid = true;
                    in.valid = true;
                }
            }
            /*PROTECTED REGION END*/
        }

        using namespace ::nevonex::serial_ext_gps_nmea0183;

        Serial_Ext_GPS_NMEA0183Provider_ptr serial_Ext_GPS_NMEA0183Provider =
                getSerial_Ext_GPS_NMEA0183Provider();
        if (serial_Ext_GPS_NMEA0183Provider != nullptr)
        {
            /*PROTECTED REGION ID(MainControllerRun_Serial_Ext_GPS_NMEA0183ProviderInnerMethod) ENABLED START*/
            if (Serial_Ext_GPS_NMEA0183_ptr gps =
                    serial_Ext_GPS_NMEA0183Provider->getSerial_Ext_GPS_NMEA0183())
            {
                if (gps->isSerial_Ext_GPS_NMEA0183_Data_Valid())
                {
                    if (GPS_NMEA0183_T_R_ptr data =
                            gps->getSerial_Ext_GPS_NMEA0183_Data())
                    {
                        // ┌─ 한국어 ───────────────────────────────────────────┐
                        // 방위는 Course(진행방향)를 씁니다. Heading(기수방위)은
                        // 듀얼 안테나에서만 유효하고 없으면 -99.99 가 옵니다.
                        // 참조 구현도 Course 를 기본으로 씁니다.
                        //
                        // 속도는 knots 로 옵니다. 추종 계층이 m/s 를 기대하므로
                        // 여기서 변환합니다.
                        // └───────────────────────────────────────────────────┘
                        // ┌─ English ──────────────────────────────────────────┐
                        // Course over ground is the bearing source; the true
                        // Heading is dual-antenna only and reports -99.99 when
                        // absent. Speed arrives in knots and is converted to m/s,
                        // which is what the tracking layer expects.
                        // └───────────────────────────────────────────────────┘
                        const double rawHeading =
                                static_cast< double >(data->getHeading());
                        const double headingDeg =
                                (rawHeading != NMEA_HEADING_INVALID)
                                ? rawHeading
                                : static_cast< double >(data->getCourse());

                        setCurrentGps(
                                static_cast< double >(data->getLatitude()),
                                static_cast< double >(data->getLongitude()),
                                headingDeg,
                                static_cast< double >(data->getSpeed()) * KNOTS_TO_MPS,
                                static_cast< double >(data->getAltitude()),
                                static_cast< std::int64_t >(
                                    gps->getSerial_Ext_GPS_NMEA0183_Data_Timestamp()));

                        ::AppMain::MachineInputs &in = machineInputScratch();
                        in.gpsFixType = static_cast< int >(
                                data->getGPS_Quality_Indicator());
                        in.gpsSats = static_cast< int >(data->getNo_Satellites());
                        in.gpsHdop = static_cast< double >(data->getHDOP());
                        in.valid = true;
                    }
                }
            }
            /*PROTECTED REGION END*/
        }

        /*PROTECTED REGION ID(MainControllerImpl_RunAdditionalSection) ENABLED START*/
        // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
        // 머신 I/O 는 위쪽 provider 블록 다섯 개에서 전부 끝났습니다. 각 블록이
        // 자기 계통을 읽어 scratch 에 담고 자기 계통 지령을 썼습니다.
        //
        // 여기서 그 scratch 를 한 번에 발행합니다. 블록마다 공유 스냅샷을
        // 갱신하면 TrackingLoop 이 반쯤 갱신된 상태를 볼 수 있기 때문입니다 —
        // 새 엔코더값과 지난 주기 IMU 가 섞이면 조향각 융합이 어긋납니다.
        // └────────────────────────────────────────────────────────────────────┘
        // ┌─ English ──────────────────────────────────────────────────────────┐
        // All machine I/O is done: the five provider blocks above each read their
        // own subsystem into the scratch snapshot and wrote their own commands.
        //
        // Publishing happens here, once. Had each block updated the shared snapshot
        // directly, TrackingLoop could observe a half-updated one — a fresh encoder
        // reading paired with last tick's IMU skews the steering-angle fusion.
        // └────────────────────────────────────────────────────────────────────┘
        publishMachineInputs();

        // 경로 추종은 ProcessTimer 가 아니라 자체 스레드에서 돌립니다. 첫 run()
        // 호출에서 시작하고 이후 호출은 아무것도 하지 않습니다. 루프가 하는 일은
        // 전부 tracking::TrackingLoop 에 있습니다.
        //
        // Path tracking runs on its own thread rather than on the ProcessTimer.
        // The thread is launched on the first run() call; later calls are no-ops.
        // Everything the loop does lives in tracking::TrackingLoop.
        static std::once_flag trackingLaunchOnce;
        std::call_once(trackingLaunchOnce,
                [this]
                {
                    trackingLoop_ = std::make_shared
                            < ::AppMain::tracking::TrackingLoop > (*this);
                    auto loop = trackingLoop_; // keep alive for the detached thread
                    std::thread([loop]
                    {
                        loop->run();
                    }).detach();
                });
        return;
        /*PROTECTED REGION END*/
    } catch (...)
    {
        NEVONEX_LOG(SeverityLevel::error) << "Exception in MainController::run:"
                << boost::current_exception_diagnostic_information();
    }
    /*PROTECTED REGION ID(MainControllerImpl_runEnd) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

void MainController::start(const int intervalInMilliSecond)
{
    NEVONEX_LOG(SeverityLevel::trace) << "Controller run method started";

    using namespace std::chrono;
    system_clock::time_point begin = system_clock::now();
    this->run();
    system_clock::time_point end = system_clock::now();
    int timeTaken = duration_cast < milliseconds > (end - begin).count();
    if (timeTaken > intervalInMilliSecond)
    {
        //Total time taken to execute the Run method should be less than the given interval. Else consider increasing the interval.
        NEVONEX_LOG(SeverityLevel::warning)
                << "Run method taking more time than interval. Time take is "
                << std::to_string(timeTaken) << "ms";
    }
}

void MainController::setWebSocketEndPoint(
        std::shared_ptr< web::WebSocketEndPoint > &webSocketEndPoint)
{
    this->webSocketEndPoint = webSocketEndPoint;
}

std::shared_ptr< ::AppMain::web::WebSocketEndPoint > MainController::getWebSocketEndPoint() const
{
    return this->webSocketEndPoint;
}

void MainController::_initialize()
{
    // Supertypes
    ::AppMain::ApplicationInputData::_initialize();
    ::AppMain::IController::_initialize();

    // References

    /*PROTECTED REGION ID(MainControllerImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

/*PROTECTED REGION ID(MainControllerImpl_MethodsEnd) ENABLED START*/
void MainController::prepareForNewTrack()
{
    resetTrackCompleteSent();
    {
        std::lock_guard<std::mutex> guard(tuningMutex_);
        runGeometryFrozen_ = false;
    }
    // 마지막 지령이 남아 계속 머신으로 나가지 않게 안전 상태로 되돌립니다.
    // Return the command to the safe state so nothing stale keeps going out.
    releaseMachineOutputs();
    if (trackingLoop_) {
        trackingLoop_->resetRunState();
    }
}
/*PROTECTED REGION END*/
