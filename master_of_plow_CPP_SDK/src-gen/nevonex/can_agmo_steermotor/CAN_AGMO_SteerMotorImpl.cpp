/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_SteerMotor.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/fcal/MOTOR_HEARTBEAT_T_R.hpp>
#include <nevonex/fcal/RAW_CAN_DATA_DLC_8_W.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_SteerMotorImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_steermotor;
using namespace ::nevonex::log;

void CAN_AGMO_SteerMotor::_initialize()
{
    // Supertypes
    ::nevonex::common::TopicObject::_initialize();

    // References

    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_SteerMotor::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
        if (m_motor_Heartbeat)
            _any = ::ecore::as < ::ecore::EObject > (m_motor_Heartbeat);
        return _any;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
        if (m_motor_Request)
            _any = ::ecore::as < ::ecore::EObject > (m_motor_Request);
        return _any;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_motor_Response_Encoder_Speed);
        return _any;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_motor_Response_EncoderCountValue);
        return _any;
    }
    }
    return ::nevonex::common::TopicObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_SteerMotor::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::MOTOR_HEARTBEAT_T_R* >(_t0.get());
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor::setMotor_Heartbeat(_t1);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W* >(_t0.get());
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor::setMotor_RequestInternal(_t1);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor::setMotor_Response_Encoder_Speed(_t0);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor::setMotor_Response_EncoderCountValue(_t0);
        return;
    }
    }
    ::nevonex::common::TopicObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_SteerMotor::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
        return (bool) m_motor_Heartbeat;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
        return (bool) m_motor_Request;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
    {
        return m_motor_Response_Encoder_Speed != 0;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE:
    {
        return m_motor_Response_EncoderCountValue != 0.0;
    }
    }
    return ::nevonex::common::TopicObject::eIsSet(_featureID);
}

void CAN_AGMO_SteerMotor::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
        // nullptr
        setMotor_Heartbeat(nullptr);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
        // nullptr
        setMotor_Request(nullptr);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
    {
        // 0
        setMotor_Response_Encoder_Speed(0);
        return;
    }
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE:
    {
        // 0.0
        setMotor_Response_EncoderCountValue(0.0);
        return;
    }
    }
    ::nevonex::common::TopicObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_SteerMotor::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage* >(::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::_instance().get())->getCAN_AGMO_SteerMotor();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_SteerMotor::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
    }
        return;
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_SteerMotor::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT:
    {
    }
        return;
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
