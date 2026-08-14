/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_SteerMotorProvider.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_SteerMotorProviderImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_steermotor;
using namespace ::nevonex::log;

void CAN_AGMO_SteerMotorProvider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProviderImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_SteerMotorProvider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
        if (m_cAN_AGMO_SteerMotor)
            _any = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_SteerMotor);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_SteerMotorProvider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr _t1 =
                dynamic_cast< ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor* >(_t0.get());
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider::setCAN_AGMO_SteerMotor(_t1);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_SteerMotorProvider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
        return (bool) m_cAN_AGMO_SteerMotor;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void CAN_AGMO_SteerMotorProvider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
        // nullptr
        setCAN_AGMO_SteerMotor(nullptr);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_SteerMotorProvider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage* >(::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::_instance().get())->getCAN_AGMO_SteerMotorProvider();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_SteerMotorProvider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_SteerMotorProvider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
