/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_SteerMotor.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <nevonex/fcb/PublishConnectionFactory.hpp>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <vector>

/*PROTECTED REGION ID(CAN_AGMO_SteerMotor.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_steermotor;
using namespace ::nevonex::log;

// Default constructor
CAN_AGMO_SteerMotor::CAN_AGMO_SteerMotor() : m_motor_Heartbeat(0), m_motor_Request(0), m_motor_Response_Encoder_Speed(0), m_motor_Response_EncoderCountValue(0)
{
    // Machine does NOT call _initialize() — Provider manages initialization

    m_interfaceDetailMap =
    {
    { 9428, ::nevonex::types::InterfaceDetails("fek/9428") },
    { 9434, ::nevonex::types::InterfaceDetails("fek/9434") },
    { 9435, ::nevonex::types::InterfaceDetails("fek/9435") } };

    /*PROTECTED REGION ID(CAN_AGMO_SteerMotor__CAN_AGMO_SteerMotor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_SteerMotor::~CAN_AGMO_SteerMotor()
{
    /*PROTECTED REGION ID(CAN_AGMO_SteerMotor__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// References
::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr CAN_AGMO_SteerMotor::getMotor_Heartbeat() const
{
    return m_motor_Heartbeat;
}

void CAN_AGMO_SteerMotor::setMotor_Heartbeat(::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr _motor_Heartbeat)
{
    ::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr _old_motor_Heartbeat = m_motor_Heartbeat;
    m_motor_Heartbeat = _motor_Heartbeat;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_SteerMotor property \"motor_Heartbeat\" has changed.";
    notifyPropertyChange("motor_Heartbeat", _old_motor_Heartbeat, m_motor_Heartbeat);
}

::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr CAN_AGMO_SteerMotor::getMotor_Request() const
{
    return m_motor_Request;
}

void CAN_AGMO_SteerMotor::setMotor_Request(::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _motor_Request)
{
    setMotor_RequestInternal(_motor_Request);

    // Publish the value to FIL layer
    std::vector< ::ecore::EObject_ptr > _publishPayload;
    _publishPayload.push_back(::ecore::as< ::ecore::EObject >(_this()));
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
            "CAN_AGMO_SteerMotor.motor_Request.pub", "motor_Request",
            _publishPayload, ::nevonex::fcb::SubscriberEnum::FIL);
}

void CAN_AGMO_SteerMotor::setMotor_RequestInternal(::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _motor_Request)
{
    ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _old_motor_Request = m_motor_Request;
    m_motor_Request = _motor_Request;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_SteerMotor property \"motor_Request\" has changed.";
    notifyPropertyChange("motor_Request", _old_motor_Request, m_motor_Request);
}

::ecore::EInt CAN_AGMO_SteerMotor::getMotor_Response_Encoder_Speed() const
{
    return m_motor_Response_Encoder_Speed;
}

void CAN_AGMO_SteerMotor::setMotor_Response_Encoder_Speed(::ecore::EInt _motor_Response_Encoder_Speed)
{
    ::ecore::EInt _old_motor_Response_Encoder_Speed = m_motor_Response_Encoder_Speed;
    m_motor_Response_Encoder_Speed = _motor_Response_Encoder_Speed;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_SteerMotor property \"motor_Response_Encoder_Speed\" has changed.";
    notifyPropertyChange("motor_Response_Encoder_Speed", _old_motor_Response_Encoder_Speed, m_motor_Response_Encoder_Speed);
}

::ecore::EFloat CAN_AGMO_SteerMotor::getMotor_Response_EncoderCountValue() const
{
    return m_motor_Response_EncoderCountValue;
}

void CAN_AGMO_SteerMotor::setMotor_Response_EncoderCountValue(::ecore::EFloat _motor_Response_EncoderCountValue)
{
    ::ecore::EFloat _old_motor_Response_EncoderCountValue = m_motor_Response_EncoderCountValue;
    m_motor_Response_EncoderCountValue = _motor_Response_EncoderCountValue;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_SteerMotor property \"motor_Response_EncoderCountValue\" has changed.";
    notifyPropertyChange("motor_Response_EncoderCountValue", _old_motor_Response_EncoderCountValue, m_motor_Response_EncoderCountValue);
}

// FCAL timestamp/valid
::ecore::ELong CAN_AGMO_SteerMotor::getMotor_Heartbeat_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9428);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_SteerMotor::isMotor_Heartbeat_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9428);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::ELong CAN_AGMO_SteerMotor::getMotor_Request_Timestamp()
{
    return -1;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::EBoolean CAN_AGMO_SteerMotor::isMotor_Request_Valid()
{
    return false;
}

::ecore::ELong CAN_AGMO_SteerMotor::getMotor_Response_Encoder_Speed_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9434);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_SteerMotor::isMotor_Response_Encoder_Speed_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9434);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_SteerMotor::getMotor_Response_EncoderCountValue_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9435);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_SteerMotor::isMotor_Response_EncoderCountValue_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9435);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::EBoolean CAN_AGMO_SteerMotor::updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp)
{
    auto _detail = m_interfaceDetailMap.find(_topicKey);
    if (_detail == m_interfaceDetailMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::warning) << _topicKey
                << " interface details are not present in the interface detail map[CAN_AGMO_SteerMotor].";
        return false;
    }

    if (_detail->second.getTimestamp() > _timestamp)
    {
        SDK_FCAL_LOG(SeverityLevel::warning)
                << "Old or duplicate message received. Old TS : "
                << _detail->second.getTimestamp() << " new TS : " << _timestamp
                << " feature: CAN_AGMO_SteerMotor topic:" << _topicKey;
        return false;
    }

    _detail->second.setValid(_valid, _timestamp);
    return true;
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_SteerMotor::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_SteerMotor::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_SteerMotor::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
