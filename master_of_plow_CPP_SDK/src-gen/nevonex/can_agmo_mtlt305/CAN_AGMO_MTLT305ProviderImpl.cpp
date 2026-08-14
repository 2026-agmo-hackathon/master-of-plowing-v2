/*
 * nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305ProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_MTLT305Provider.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_MTLT305ProviderImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_mtlt305;
using namespace ::nevonex::log;

void CAN_AGMO_MTLT305Provider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(CAN_AGMO_MTLT305ProviderImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_MTLT305Provider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
        if (m_cAN_AGMO_MTLT305)
            _any = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_MTLT305);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_MTLT305Provider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305_ptr _t1 =
                dynamic_cast< ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305* >(_t0.get());
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider::setCAN_AGMO_MTLT305(_t1);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_MTLT305Provider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
        return (bool) m_cAN_AGMO_MTLT305;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void CAN_AGMO_MTLT305Provider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
        // nullptr
        setCAN_AGMO_MTLT305(nullptr);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_MTLT305Provider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package* >(::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::_instance().get())->getCAN_AGMO_MTLT305Provider();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_MTLT305Provider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_MTLT305Provider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
