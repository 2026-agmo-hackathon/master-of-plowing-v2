/*
 * nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_Customized_TractorProvider.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_Tractor.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProviderImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_customized_tractor;
using namespace ::nevonex::log;

void CAN_AGMO_Customized_TractorProvider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProviderImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_Customized_TractorProvider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
        if (m_cAN_AGMO_Customized_Tractor)
            _any = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_Customized_Tractor);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_Customized_TractorProvider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor_ptr _t1 =
                dynamic_cast< ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider::setCAN_AGMO_Customized_Tractor(_t1);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_Customized_TractorProvider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
        return (bool) m_cAN_AGMO_Customized_Tractor;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void CAN_AGMO_Customized_TractorProvider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
        // nullptr
        setCAN_AGMO_Customized_Tractor(nullptr);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_Customized_TractorProvider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage* >(::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::_instance().get())->getCAN_AGMO_Customized_TractorProvider();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_Customized_TractorProvider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_Customized_TractorProvider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
