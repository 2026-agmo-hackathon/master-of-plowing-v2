/*
 * nevonex/fcal/MOTOR_HEARTBEAT_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MOTOR_HEARTBEAT_T_R.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcal/FcalPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
MOTOR_HEARTBEAT_T_R::MOTOR_HEARTBEAT_T_R() : m_heartbeat_ControlStatus_HallFailure(0), m_heartbeat_ControlStatus_CANdisconnected(0), m_heartbeat_ControlStatus_MotorStalled(0), m_heartbeat_ControlStatus_Disabled(0), m_heartbeat_ControlStatus_Overvoltage(0), m_heartbeat_ControlStatus_HardwareProtection(0), m_heartbeat_ControlStatus_E2PROM(0), m_heartbeat_ControlStatus_Undervoltage(0), m_heartbeat_ControlStatus_Overcurrent(0), m_heartbeat_ControlStatus_ModeFailure(0)
{

    /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R__MOTOR_HEARTBEAT_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

MOTOR_HEARTBEAT_T_R::~MOTOR_HEARTBEAT_T_R()
{

    /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_HallFailure() const
{
    return m_heartbeat_ControlStatus_HallFailure;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_HallFailure(::ecore::EInt _heartbeat_ControlStatus_HallFailure)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_HallFailure = m_heartbeat_ControlStatus_HallFailure;
    m_heartbeat_ControlStatus_HallFailure = _heartbeat_ControlStatus_HallFailure;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_CANdisconnected() const
{
    return m_heartbeat_ControlStatus_CANdisconnected;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_CANdisconnected(::ecore::EInt _heartbeat_ControlStatus_CANdisconnected)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_CANdisconnected = m_heartbeat_ControlStatus_CANdisconnected;
    m_heartbeat_ControlStatus_CANdisconnected = _heartbeat_ControlStatus_CANdisconnected;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_MotorStalled() const
{
    return m_heartbeat_ControlStatus_MotorStalled;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_MotorStalled(::ecore::EInt _heartbeat_ControlStatus_MotorStalled)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_MotorStalled = m_heartbeat_ControlStatus_MotorStalled;
    m_heartbeat_ControlStatus_MotorStalled = _heartbeat_ControlStatus_MotorStalled;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_Disabled() const
{
    return m_heartbeat_ControlStatus_Disabled;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Disabled(::ecore::EInt _heartbeat_ControlStatus_Disabled)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_Disabled = m_heartbeat_ControlStatus_Disabled;
    m_heartbeat_ControlStatus_Disabled = _heartbeat_ControlStatus_Disabled;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_Overvoltage() const
{
    return m_heartbeat_ControlStatus_Overvoltage;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Overvoltage(::ecore::EInt _heartbeat_ControlStatus_Overvoltage)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_Overvoltage = m_heartbeat_ControlStatus_Overvoltage;
    m_heartbeat_ControlStatus_Overvoltage = _heartbeat_ControlStatus_Overvoltage;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_HardwareProtection() const
{
    return m_heartbeat_ControlStatus_HardwareProtection;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_HardwareProtection(::ecore::EInt _heartbeat_ControlStatus_HardwareProtection)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_HardwareProtection = m_heartbeat_ControlStatus_HardwareProtection;
    m_heartbeat_ControlStatus_HardwareProtection = _heartbeat_ControlStatus_HardwareProtection;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_E2PROM() const
{
    return m_heartbeat_ControlStatus_E2PROM;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_E2PROM(::ecore::EInt _heartbeat_ControlStatus_E2PROM)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_E2PROM = m_heartbeat_ControlStatus_E2PROM;
    m_heartbeat_ControlStatus_E2PROM = _heartbeat_ControlStatus_E2PROM;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_Undervoltage() const
{
    return m_heartbeat_ControlStatus_Undervoltage;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Undervoltage(::ecore::EInt _heartbeat_ControlStatus_Undervoltage)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_Undervoltage = m_heartbeat_ControlStatus_Undervoltage;
    m_heartbeat_ControlStatus_Undervoltage = _heartbeat_ControlStatus_Undervoltage;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_Overcurrent() const
{
    return m_heartbeat_ControlStatus_Overcurrent;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_Overcurrent(::ecore::EInt _heartbeat_ControlStatus_Overcurrent)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_Overcurrent = m_heartbeat_ControlStatus_Overcurrent;
    m_heartbeat_ControlStatus_Overcurrent = _heartbeat_ControlStatus_Overcurrent;

}

::ecore::EInt MOTOR_HEARTBEAT_T_R::getHeartbeat_ControlStatus_ModeFailure() const
{
    return m_heartbeat_ControlStatus_ModeFailure;
}

void MOTOR_HEARTBEAT_T_R::setHeartbeat_ControlStatus_ModeFailure(::ecore::EInt _heartbeat_ControlStatus_ModeFailure)
{

    ::ecore::EInt _old_heartbeat_ControlStatus_ModeFailure = m_heartbeat_ControlStatus_ModeFailure;
    m_heartbeat_ControlStatus_ModeFailure = _heartbeat_ControlStatus_ModeFailure;

}

// References

