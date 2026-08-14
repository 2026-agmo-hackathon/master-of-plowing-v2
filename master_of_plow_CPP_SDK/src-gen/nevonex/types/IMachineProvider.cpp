/*
 * nevonex/types/IMachineProvider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "IMachineProvider.hpp"
#include <nevonex/fcb/FCALController.hpp>
#include <nevonex/fcb/PublishConnectionFactory.hpp>
#include <nevonex/fcb/SubscribeConnectionFactory.hpp>
#include <unordered_map>
#include <map>
#include <string>

using namespace ::nevonex::types;

// Default constructor
IMachineProvider::IMachineProvider()
 : m_controller(0)
{
}

IMachineProvider::~IMachineProvider()
{
}

// References

::nevonex::fcb::FCALController_ptr IMachineProvider::getController() const
{
    return m_controller;
}

void IMachineProvider::setController(
        ::nevonex::fcb::FCALController_ptr _controller)
{

    m_controller = _controller;

}

// IMachineProvider framework operations

void IMachineProvider::createMachines(std::istream & _stream)
{
}

void IMachineProvider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
}

void IMachineProvider::initMachineProvider()
{
    auto _subCF = ::nevonex::fcb::SubscribeConnectionFactory::getInstance();

    std::unordered_map<std::string, std::string> _subTopicMap;
    _subTopicMap["/1/+"] = "CAN_AGMO_Customized_Tractor.machineconnect.sub";
    _subTopicMap["/0/+"] = "CAN_AGMO_Customized_Tractor.machinedata.sub";
    _subTopicMap["/1/+"] = "CAN_AGMO_MTLT305.machineconnect.sub";
    _subTopicMap["/0/+"] = "CAN_AGMO_MTLT305.machinedata.sub";
    _subTopicMap["/1/+"] = "CAN_AGMO_SteerMotor.machineconnect.sub";
    _subTopicMap["/0/+"] = "CAN_AGMO_SteerMotor.machinedata.sub";
    _subTopicMap["/1/+"] = "ISOPGN.machineconnect.sub";
    _subTopicMap["/0/+"] = "ISOPGN.machinedata.sub";
    _subTopicMap["/1/+"] = "Serial_Ext_GPS_NMEA0183.machineconnect.sub";
    _subTopicMap["/0/+"] = "Serial_Ext_GPS_NMEA0183.machinedata.sub";
    _subTopicMap["/9455"] = "CAN_AGMO_Customized_Tractor.Receive_ACC_INFO.sub";
    _subTopicMap["/9456"] = "CAN_AGMO_Customized_Tractor.Receive_FNR_INFO.sub";
    _subTopicMap["/9457"] = "CAN_AGMO_Customized_Tractor.Receive_HYD_INFO.sub";
    _subTopicMap["/9459"] = "CAN_AGMO_Customized_Tractor.Receive_SFT_INFO.sub";
    _subTopicMap["/9440"] = "CAN_AGMO_MTLT305.Aceinna_Accel.sub";
    _subTopicMap["/9441"] = "CAN_AGMO_MTLT305.Aceinna_AngleRate.sub";
    _subTopicMap["/9442"] = "CAN_AGMO_MTLT305.Aceinna_Angles.sub";
    _subTopicMap["/9428"] = "CAN_AGMO_SteerMotor.Motor_Heartbeat.sub";
    _subTopicMap["/9434"] = "CAN_AGMO_SteerMotor.Motor_Response_Encoder_Speed.sub";
    _subTopicMap["/9435"] = "CAN_AGMO_SteerMotor.Motor_Response_EncoderCountValue.sub";
    _subTopicMap["/251"] = "ISOPGN.HitchPosPerRe.sub";
    _subTopicMap["/9465"] = "Serial_Ext_GPS_NMEA0183.Serial_Ext_GPS_NMEA0183_Data.sub";
    _subCF->setTopicMap(_subTopicMap);

    std::unordered_map<std::string, std::string> _subPrefixMap;
    _subPrefixMap["FCAL2FIL_Subscribe"] = "fek";
    _subCF->setPrefixMap(_subPrefixMap);

    std::unordered_map<std::string, std::string> _subMachinePathMap;
    _subMachinePathMap["CAN_AGMO_Customized_Tractor"] = "CAN_AGMO_Customized_Tractor";
    _subMachinePathMap["CAN_AGMO_MTLT305"] = "CAN_AGMO_MTLT305";
    _subMachinePathMap["CAN_AGMO_SteerMotor"] = "CAN_AGMO_SteerMotor";
    _subMachinePathMap["ISOPGN"] = "ISOPGN";
    _subMachinePathMap["Serial_Ext_GPS_NMEA0183"] = "Serial_Ext_GPS_NMEA0183";
    _subCF->setMachinePathMap(_subMachinePathMap);
    _subCF->addProvider(::ecore::EObject_ptr(this));

    auto _pubCF = ::nevonex::fcb::PublishConnectionFactory::getInstance();

    std::unordered_map<std::string, std::string> _topicMap;
    _topicMap["CAN_AGMO_Customized_Tractor.send_ACC_CMD.pub"] = "/9460";
    _topicMap["CAN_AGMO_Customized_Tractor.send_FNR_CMD.pub"] = "/9461";
    _topicMap["CAN_AGMO_Customized_Tractor.send_HYD_CMD.pub"] = "/9462";
    _topicMap["CAN_AGMO_Customized_Tractor.send_SFT_CMD.pub"] = "/9464";
    _topicMap["CAN_AGMO_SteerMotor.motor_Request.pub"] = "/9436";
    _pubCF->setTopicMap(_topicMap);

    std::unordered_map<std::string, std::string> _payloadMap;
    _payloadMap["CAN_AGMO_Customized_Tractor.send_ACC_CMD.pub"] = "array";
    _payloadMap["CAN_AGMO_Customized_Tractor.send_FNR_CMD.pub"] = "array";
    _payloadMap["CAN_AGMO_Customized_Tractor.send_HYD_CMD.pub"] = "array";
    _payloadMap["CAN_AGMO_Customized_Tractor.send_SFT_CMD.pub"] = "array";
    _payloadMap["CAN_AGMO_SteerMotor.motor_Request.pub"] = "array";
    _pubCF->setPayloadMap(_payloadMap);

    std::unordered_map<std::string, std::string> _pubPrefixMap;
    _pubPrefixMap["FCAL2FIL_Publish"] = "fek";
    _pubCF->setPrefixMap(_pubPrefixMap);

    std::unordered_map<std::string, std::string> _pubMachinePathMap;
    _pubMachinePathMap["CAN_AGMO_Customized_Tractor"] = "CAN_AGMO_Customized_Tractor";
    _pubMachinePathMap["CAN_AGMO_MTLT305"] = "CAN_AGMO_MTLT305";
    _pubMachinePathMap["CAN_AGMO_SteerMotor"] = "CAN_AGMO_SteerMotor";
    _pubMachinePathMap["ISOPGN"] = "ISOPGN";
    _pubMachinePathMap["Serial_Ext_GPS_NMEA0183"] = "Serial_Ext_GPS_NMEA0183";
    _pubCF->setMachinePathMap(_pubMachinePathMap);

    std::unordered_map<std::string, bool> _enabledMap;
    _enabledMap["/9460"] = true;
    _enabledMap["/9461"] = true;
    _enabledMap["/9462"] = true;
    _enabledMap["/9464"] = true;
    _enabledMap["/9436"] = true;
    _pubCF->setEnabledDisabledTopicMap(_enabledMap);

    _pubCF->addProvider(::ecore::EObject_ptr(this));
}

::nevonex::common::TopicObject_ptr IMachineProvider::getTopicElement(::ecore::EString const& _index)
{
    return nullptr;
}

void IMachineProvider::stopMachineProvider()
{
}

void IMachineProvider::start()
{
    ::nevonex::fcb::SubscribeConnectionFactory::getInstance()->initConnectionFactory();
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->initConnectionFactory();
}

void IMachineProvider::stop()
{
    ::nevonex::fcb::SubscribeConnectionFactory::getInstance()->stopSubscribeClient();
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->stopPublishClient();
}

void IMachineProvider::restart()
{
    stop();
    start();
}

void IMachineProvider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
}

::nevonex::common::TopicObject_ptr IMachineProvider::getTopicObject(const ::ecore::EString & path) const
{
    auto it = m_topicObjectMap.find(path);
    if (it != m_topicObjectMap.end())
        return it->second;
    return nullptr;
}
