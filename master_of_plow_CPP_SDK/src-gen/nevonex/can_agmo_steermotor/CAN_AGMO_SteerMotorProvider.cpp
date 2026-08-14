/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_SteerMotorProvider.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <sstream>

/*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_steermotor;
using namespace ::nevonex::log;

CAN_AGMO_SteerMotorProvider::CAN_AGMO_SteerMotorProvider() : m_cAN_AGMO_SteerMotor(0)
{
    _initialize();

    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__CAN_AGMO_SteerMotorProvider) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_SteerMotorProvider::~CAN_AGMO_SteerMotorProvider()
{
    m_cAN_AGMO_SteerMotor.reset();
}

// References
::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr CAN_AGMO_SteerMotorProvider::getCAN_AGMO_SteerMotor() const
{
    return m_cAN_AGMO_SteerMotor;
}

void CAN_AGMO_SteerMotorProvider::setCAN_AGMO_SteerMotor(::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr _cAN_AGMO_SteerMotor)
{
    if (m_cAN_AGMO_SteerMotor)
        m_cAN_AGMO_SteerMotor->_setEContainer(CAN_AGMO_SteerMotorProvider_ptr(),
                Can_agmo_steermotorPackage::_instance()->getCAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor());
    if (_cAN_AGMO_SteerMotor)
        _cAN_AGMO_SteerMotor->_setEContainer(_this(),
                Can_agmo_steermotorPackage::_instance()->getCAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor());
    m_cAN_AGMO_SteerMotor = _cAN_AGMO_SteerMotor;
}

// Operations
void CAN_AGMO_SteerMotorProvider::createMachines(std::istream & _stream)
{
    bool _newMachineCreated = false;
    if (!m_cAN_AGMO_SteerMotor)
    {
        setCAN_AGMO_SteerMotor(::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr(new ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor()));
        _newMachineCreated = true;
    }
    auto _machineObj = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_SteerMotor);
    if (_machineObj)
    {
        constructChildTypes(::ecore::EJavaObject(), _machineObj, "CAN_AGMO_SteerMotor");
        _machineObj->_initialize();
    }
    if (_newMachineCreated)
    {
        notifyPropertyChange("root", nullptr, m_cAN_AGMO_SteerMotor);
    }
    (void)_stream;
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__createMachines) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
    (void)_parent;
    if (!_parentObj) return;
    auto _topicObject = ::ecore::as < ::nevonex::common::TopicObject > (_parentObj);
    if (!_topicObject) return;
    ::ecore::EString _machinePath = _path;
    if (_machinePath.empty())
    {
        _machinePath = "CAN_AGMO_SteerMotor";
    }
    m_topicObjectMap[_machinePath] = _topicObject;
    m_topicObjectMap["CAN_AGMO_SteerMotor"] = _topicObject;
    m_topicObjectMap["can_agmo_steermotor"] = _topicObject;
    m_topicObjectMap["can_agmo_steermotor/0"] = _topicObject;
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__constructChildTypes) START*/
    /*PROTECTED REGION END*/
}

::nevonex::common::TopicObject_ptr CAN_AGMO_SteerMotorProvider::getTopicElement(::ecore::EString const& _index)
{
    if (!m_cAN_AGMO_SteerMotor)
    {
        std::istringstream _emptyStream;
        createMachines(_emptyStream);
    }
    auto _it = m_topicObjectMap.find(_index);
    if (_it != m_topicObjectMap.end())
        return _it->second;
    if (boost::iequals(_index, "CAN_AGMO_SteerMotor")
            || boost::iequals(_index, "can_agmo_steermotor")
            || boost::iequals(_index, "can_agmo_steermotor/0"))
    {
        return ::ecore::as < ::nevonex::common::TopicObject > (m_cAN_AGMO_SteerMotor);
    }
    return nullptr;
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__getTopicElement) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::initMachineProvider()
{
    ::nevonex::types::IMachineProvider::initMachineProvider();
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__initMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::stopMachineProvider()
{
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__stopMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::start()
{
    ::nevonex::types::IMachineProvider::start();
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__start) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::stop()
{
    ::nevonex::types::IMachineProvider::stop();
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__stop) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::restart()
{
    ::nevonex::types::IMachineProvider::restart();
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__restart) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp)
{
    if (_topic == "CAN_AGMO_SteerMotor.Motor_Heartbeat.sub" || _topic == "/9428")
    {
        if (!m_cAN_AGMO_SteerMotor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_SteerMotor->updateInterfaceDetail(9428, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0 && _jsonPlData[0].IsObject())
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject() && !(_item->HasMember("heartbeat_ControlStatus_HallFailure") || _item->HasMember("heartbeat_controlstatus_hallfailure") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_HALLFAILURE") || _item->HasMember("heartbeat_ControlStatus_CANdisconnected") || _item->HasMember("heartbeat_controlstatus_candisconnected") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED") || _item->HasMember("heartbeat_ControlStatus_MotorStalled") || _item->HasMember("heartbeat_controlstatus_motorstalled") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_MOTORSTALLED") || _item->HasMember("heartbeat_ControlStatus_Disabled") || _item->HasMember("heartbeat_controlstatus_disabled") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_DISABLED") || _item->HasMember("heartbeat_ControlStatus_Overvoltage") || _item->HasMember("heartbeat_controlstatus_overvoltage") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE") || _item->HasMember("heartbeat_ControlStatus_HardwareProtection") || _item->HasMember("heartbeat_controlstatus_hardwareprotection") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION") || _item->HasMember("heartbeat_ControlStatus_E2PROM") || _item->HasMember("heartbeat_controlstatus_e2prom") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_E2PROM") || _item->HasMember("heartbeat_ControlStatus_Undervoltage") || _item->HasMember("heartbeat_controlstatus_undervoltage") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE") || _item->HasMember("heartbeat_ControlStatus_Overcurrent") || _item->HasMember("heartbeat_controlstatus_overcurrent") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_OVERCURRENT") || _item->HasMember("heartbeat_ControlStatus_ModeFailure") || _item->HasMember("heartbeat_controlstatus_modefailure") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_MODEFAILURE")))
        {
            if (_item->HasMember("motor_Heartbeat"))
            {
                _item = &(*_item)["motor_Heartbeat"];
            }
            else if (_item->HasMember("Motor_Heartbeat"))
            {
                _item = &(*_item)["Motor_Heartbeat"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("heartbeat_ControlStatus_HallFailure") || _item->HasMember("heartbeat_controlstatus_hallfailure") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_HALLFAILURE") || _item->HasMember("heartbeat_ControlStatus_CANdisconnected") || _item->HasMember("heartbeat_controlstatus_candisconnected") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED") || _item->HasMember("heartbeat_ControlStatus_MotorStalled") || _item->HasMember("heartbeat_controlstatus_motorstalled") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_MOTORSTALLED") || _item->HasMember("heartbeat_ControlStatus_Disabled") || _item->HasMember("heartbeat_controlstatus_disabled") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_DISABLED") || _item->HasMember("heartbeat_ControlStatus_Overvoltage") || _item->HasMember("heartbeat_controlstatus_overvoltage") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE") || _item->HasMember("heartbeat_ControlStatus_HardwareProtection") || _item->HasMember("heartbeat_controlstatus_hardwareprotection") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION") || _item->HasMember("heartbeat_ControlStatus_E2PROM") || _item->HasMember("heartbeat_controlstatus_e2prom") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_E2PROM") || _item->HasMember("heartbeat_ControlStatus_Undervoltage") || _item->HasMember("heartbeat_controlstatus_undervoltage") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE") || _item->HasMember("heartbeat_ControlStatus_Overcurrent") || _item->HasMember("heartbeat_controlstatus_overcurrent") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_OVERCURRENT") || _item->HasMember("heartbeat_ControlStatus_ModeFailure") || _item->HasMember("heartbeat_controlstatus_modefailure") || _item->HasMember("HEARTBEAT_CONTROLSTATUS_MODEFAILURE")))
        {
            if (_item->HasMember("motor_Heartbeat"))
            {
                _item = &(*_item)["motor_Heartbeat"];
            }
            else if (_item->HasMember("Motor_Heartbeat"))
            {
                _item = &(*_item)["Motor_Heartbeat"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::MOTOR_HEARTBEAT_T_R > (new ::nevonex::fcal::MOTOR_HEARTBEAT_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _heartbeat_ControlStatus_HallFailureField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_HallFailure"))
        {
            _heartbeat_ControlStatus_HallFailureField = &(*_item)["heartbeat_ControlStatus_HallFailure"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_hallfailure"))
        {
            _heartbeat_ControlStatus_HallFailureField = &(*_item)["heartbeat_controlstatus_hallfailure"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_HALLFAILURE"))
        {
            _heartbeat_ControlStatus_HallFailureField = &(*_item)["HEARTBEAT_CONTROLSTATUS_HALLFAILURE"];
        }
        if (_heartbeat_ControlStatus_HallFailureField && _heartbeat_ControlStatus_HallFailureField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_HallFailure(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_HallFailureField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_CANdisconnectedField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_CANdisconnected"))
        {
            _heartbeat_ControlStatus_CANdisconnectedField = &(*_item)["heartbeat_ControlStatus_CANdisconnected"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_candisconnected"))
        {
            _heartbeat_ControlStatus_CANdisconnectedField = &(*_item)["heartbeat_controlstatus_candisconnected"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED"))
        {
            _heartbeat_ControlStatus_CANdisconnectedField = &(*_item)["HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED"];
        }
        if (_heartbeat_ControlStatus_CANdisconnectedField && _heartbeat_ControlStatus_CANdisconnectedField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_CANdisconnected(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_CANdisconnectedField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_MotorStalledField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_MotorStalled"))
        {
            _heartbeat_ControlStatus_MotorStalledField = &(*_item)["heartbeat_ControlStatus_MotorStalled"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_motorstalled"))
        {
            _heartbeat_ControlStatus_MotorStalledField = &(*_item)["heartbeat_controlstatus_motorstalled"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_MOTORSTALLED"))
        {
            _heartbeat_ControlStatus_MotorStalledField = &(*_item)["HEARTBEAT_CONTROLSTATUS_MOTORSTALLED"];
        }
        if (_heartbeat_ControlStatus_MotorStalledField && _heartbeat_ControlStatus_MotorStalledField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_MotorStalled(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_MotorStalledField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_DisabledField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_Disabled"))
        {
            _heartbeat_ControlStatus_DisabledField = &(*_item)["heartbeat_ControlStatus_Disabled"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_disabled"))
        {
            _heartbeat_ControlStatus_DisabledField = &(*_item)["heartbeat_controlstatus_disabled"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_DISABLED"))
        {
            _heartbeat_ControlStatus_DisabledField = &(*_item)["HEARTBEAT_CONTROLSTATUS_DISABLED"];
        }
        if (_heartbeat_ControlStatus_DisabledField && _heartbeat_ControlStatus_DisabledField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_Disabled(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_DisabledField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_OvervoltageField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_Overvoltage"))
        {
            _heartbeat_ControlStatus_OvervoltageField = &(*_item)["heartbeat_ControlStatus_Overvoltage"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_overvoltage"))
        {
            _heartbeat_ControlStatus_OvervoltageField = &(*_item)["heartbeat_controlstatus_overvoltage"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE"))
        {
            _heartbeat_ControlStatus_OvervoltageField = &(*_item)["HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE"];
        }
        if (_heartbeat_ControlStatus_OvervoltageField && _heartbeat_ControlStatus_OvervoltageField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_Overvoltage(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_OvervoltageField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_HardwareProtectionField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_HardwareProtection"))
        {
            _heartbeat_ControlStatus_HardwareProtectionField = &(*_item)["heartbeat_ControlStatus_HardwareProtection"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_hardwareprotection"))
        {
            _heartbeat_ControlStatus_HardwareProtectionField = &(*_item)["heartbeat_controlstatus_hardwareprotection"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION"))
        {
            _heartbeat_ControlStatus_HardwareProtectionField = &(*_item)["HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION"];
        }
        if (_heartbeat_ControlStatus_HardwareProtectionField && _heartbeat_ControlStatus_HardwareProtectionField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_HardwareProtection(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_HardwareProtectionField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_E2PROMField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_E2PROM"))
        {
            _heartbeat_ControlStatus_E2PROMField = &(*_item)["heartbeat_ControlStatus_E2PROM"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_e2prom"))
        {
            _heartbeat_ControlStatus_E2PROMField = &(*_item)["heartbeat_controlstatus_e2prom"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_E2PROM"))
        {
            _heartbeat_ControlStatus_E2PROMField = &(*_item)["HEARTBEAT_CONTROLSTATUS_E2PROM"];
        }
        if (_heartbeat_ControlStatus_E2PROMField && _heartbeat_ControlStatus_E2PROMField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_E2PROM(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_E2PROMField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_UndervoltageField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_Undervoltage"))
        {
            _heartbeat_ControlStatus_UndervoltageField = &(*_item)["heartbeat_ControlStatus_Undervoltage"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_undervoltage"))
        {
            _heartbeat_ControlStatus_UndervoltageField = &(*_item)["heartbeat_controlstatus_undervoltage"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE"))
        {
            _heartbeat_ControlStatus_UndervoltageField = &(*_item)["HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE"];
        }
        if (_heartbeat_ControlStatus_UndervoltageField && _heartbeat_ControlStatus_UndervoltageField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_Undervoltage(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_UndervoltageField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_OvercurrentField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_Overcurrent"))
        {
            _heartbeat_ControlStatus_OvercurrentField = &(*_item)["heartbeat_ControlStatus_Overcurrent"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_overcurrent"))
        {
            _heartbeat_ControlStatus_OvercurrentField = &(*_item)["heartbeat_controlstatus_overcurrent"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_OVERCURRENT"))
        {
            _heartbeat_ControlStatus_OvercurrentField = &(*_item)["HEARTBEAT_CONTROLSTATUS_OVERCURRENT"];
        }
        if (_heartbeat_ControlStatus_OvercurrentField && _heartbeat_ControlStatus_OvercurrentField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_Overcurrent(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_OvercurrentField->GetDouble()));
        }
        const rapidjson::Value* _heartbeat_ControlStatus_ModeFailureField = nullptr;
        if ((*_item).HasMember("heartbeat_ControlStatus_ModeFailure"))
        {
            _heartbeat_ControlStatus_ModeFailureField = &(*_item)["heartbeat_ControlStatus_ModeFailure"];
        }
        else if ((*_item).HasMember("heartbeat_controlstatus_modefailure"))
        {
            _heartbeat_ControlStatus_ModeFailureField = &(*_item)["heartbeat_controlstatus_modefailure"];
        }
        else if ((*_item).HasMember("HEARTBEAT_CONTROLSTATUS_MODEFAILURE"))
        {
            _heartbeat_ControlStatus_ModeFailureField = &(*_item)["HEARTBEAT_CONTROLSTATUS_MODEFAILURE"];
        }
        if (_heartbeat_ControlStatus_ModeFailureField && _heartbeat_ControlStatus_ModeFailureField->IsNumber())
        {
            _value->setHeartbeat_ControlStatus_ModeFailure(static_cast< ::ecore::EInt >(_heartbeat_ControlStatus_ModeFailureField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_HallFailure(static_cast< ::ecore::EInt >((*_item)[0].GetDouble()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_CANdisconnected(static_cast< ::ecore::EInt >((*_item)[1].GetDouble()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_MotorStalled(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_Disabled(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_Overvoltage(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
            if ((*_item).Size() > 5 && (*_item)[5].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_HardwareProtection(static_cast< ::ecore::EInt >((*_item)[5].GetDouble()));
            }
            if ((*_item).Size() > 6 && (*_item)[6].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_E2PROM(static_cast< ::ecore::EInt >((*_item)[6].GetDouble()));
            }
            if ((*_item).Size() > 7 && (*_item)[7].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_Undervoltage(static_cast< ::ecore::EInt >((*_item)[7].GetDouble()));
            }
            if ((*_item).Size() > 8 && (*_item)[8].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_Overcurrent(static_cast< ::ecore::EInt >((*_item)[8].GetDouble()));
            }
            if ((*_item).Size() > 9 && (*_item)[9].IsNumber())
            {
                _value->setHeartbeat_ControlStatus_ModeFailure(static_cast< ::ecore::EInt >((*_item)[9].GetDouble()));
            }
        }
        m_cAN_AGMO_SteerMotor->setMotor_Heartbeat(_value);
        return;
    }

    if (_topic == "CAN_AGMO_SteerMotor.Motor_Response_Encoder_Speed.sub" || _topic == "/9434")
    {
        if (!m_cAN_AGMO_SteerMotor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_SteerMotor->updateInterfaceDetail(9434, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0)
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("motor_Response_Encoder_Speed"))
            {
                _item = &(*_item)["motor_Response_Encoder_Speed"];
            }
            else if (_item->HasMember("Motor_Response_Encoder_Speed"))
            {
                _item = &(*_item)["Motor_Response_Encoder_Speed"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("motor_Response_Encoder_Speed"))
            {
                _item = &(*_item)["motor_Response_Encoder_Speed"];
            }
            else if (_item->HasMember("Motor_Response_Encoder_Speed"))
            {
                _item = &(*_item)["Motor_Response_Encoder_Speed"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsArray() && _item->Size() > 0)
        {
            _item = &(*_item)[0];
        }
        if (_item->IsNumber())
        {
            m_cAN_AGMO_SteerMotor->setMotor_Response_Encoder_Speed(static_cast< ::ecore::EInt >(_item->GetDouble()));
        }
        return;
    }

    if (_topic == "CAN_AGMO_SteerMotor.Motor_Response_EncoderCountValue.sub" || _topic == "/9435")
    {
        if (!m_cAN_AGMO_SteerMotor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_SteerMotor->updateInterfaceDetail(9435, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0)
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("motor_Response_EncoderCountValue"))
            {
                _item = &(*_item)["motor_Response_EncoderCountValue"];
            }
            else if (_item->HasMember("Motor_Response_EncoderCountValue"))
            {
                _item = &(*_item)["Motor_Response_EncoderCountValue"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("motor_Response_EncoderCountValue"))
            {
                _item = &(*_item)["motor_Response_EncoderCountValue"];
            }
            else if (_item->HasMember("Motor_Response_EncoderCountValue"))
            {
                _item = &(*_item)["Motor_Response_EncoderCountValue"];
            }
            else if (_item->HasMember("CAN_AGMO_SteerMotor"))
            {
                _item = &(*_item)["CAN_AGMO_SteerMotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor"))
            {
                _item = &(*_item)["can_agmo_steermotor"];
            }
            else if (_item->HasMember("can_agmo_steermotor/0"))
            {
                _item = &(*_item)["can_agmo_steermotor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsArray() && _item->Size() > 0)
        {
            _item = &(*_item)[0];
        }
        if (_item->IsNumber())
        {
            m_cAN_AGMO_SteerMotor->setMotor_Response_EncoderCountValue(static_cast< ::ecore::EFloat >(_item->GetFloat()));
        }
        return;
    }

    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__processMessage) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_SteerMotorProvider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider__onConnectionStatusChange) START*/
    /*PROTECTED REGION END*/
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_SteerMotorProvider::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_SteerMotorProvider::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_SteerMotorProvider::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
