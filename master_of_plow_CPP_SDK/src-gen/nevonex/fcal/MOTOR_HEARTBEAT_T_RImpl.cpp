/*
 * nevonex/fcal/MOTOR_HEARTBEAT_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MOTOR_HEARTBEAT_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void MOTOR_HEARTBEAT_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject MOTOR_HEARTBEAT_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_HallFailure);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_CANdisconnected);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_MotorStalled);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_Disabled);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_Overvoltage);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_HardwareProtection);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_E2PROM);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_Undervoltage);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_Overcurrent);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_heartbeat_ControlStatus_ModeFailure);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void MOTOR_HEARTBEAT_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_HallFailure(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_CANdisconnected(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_MotorStalled(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Disabled(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Overvoltage(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_HardwareProtection(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_E2PROM(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Undervoltage(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Overcurrent(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_ModeFailure(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean MOTOR_HEARTBEAT_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE:
    {
        return m_heartbeat_ControlStatus_HallFailure != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED:
    {
        return m_heartbeat_ControlStatus_CANdisconnected != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED:
    {
        return m_heartbeat_ControlStatus_MotorStalled != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED:
    {
        return m_heartbeat_ControlStatus_Disabled != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE:
    {
        return m_heartbeat_ControlStatus_Overvoltage != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION:
    {
        return m_heartbeat_ControlStatus_HardwareProtection != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM:
    {
        return m_heartbeat_ControlStatus_E2PROM != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE:
    {
        return m_heartbeat_ControlStatus_Undervoltage != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT:
    {
        return m_heartbeat_ControlStatus_Overcurrent != 0;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE:
    {
        return m_heartbeat_ControlStatus_ModeFailure != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void MOTOR_HEARTBEAT_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HALLFAILURE:
    {
        // 0
        setHeartbeat_ControlStatus_HallFailure(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_CANDISCONNECTED:
    {
        // 0
        setHeartbeat_ControlStatus_CANdisconnected(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MOTORSTALLED:
    {
        // 0
        setHeartbeat_ControlStatus_MotorStalled(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_DISABLED:
    {
        // 0
        setHeartbeat_ControlStatus_Disabled(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERVOLTAGE:
    {
        // 0
        setHeartbeat_ControlStatus_Overvoltage(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_HARDWAREPROTECTION:
    {
        // 0
        setHeartbeat_ControlStatus_HardwareProtection(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_E2PROM:
    {
        // 0
        setHeartbeat_ControlStatus_E2PROM(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_UNDERVOLTAGE:
    {
        // 0
        setHeartbeat_ControlStatus_Undervoltage(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_OVERCURRENT:
    {
        // 0
        setHeartbeat_ControlStatus_Overcurrent(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROLSTATUS_MODEFAILURE:
    {
        // 0
        setHeartbeat_ControlStatus_ModeFailure(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr MOTOR_HEARTBEAT_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getMOTOR_HEARTBEAT_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void MOTOR_HEARTBEAT_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void MOTOR_HEARTBEAT_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
