/*
 * nevonex/isopgn/ISOPGNProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ISOPGNProvider.hpp"
#include <stdexcept>

#include <nevonex/isopgn/IsopgnPackage.hpp>
#include <nevonex/isopgn/ISOPGN.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ISOPGNProviderImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::isopgn;
using namespace ::nevonex::log;

void ISOPGNProvider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ISOPGNProviderImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ISOPGNProvider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
        if (m_iSOPGN)
            _any = ::ecore::as < ::ecore::EObject > (m_iSOPGN);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void ISOPGNProvider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::isopgn::ISOPGN_ptr _t1 =
                dynamic_cast< ::nevonex::isopgn::ISOPGN* >(_t0.get());
        ::nevonex::isopgn::ISOPGNProvider::setISOPGN(_t1);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ISOPGNProvider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
        return (bool) m_iSOPGN;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void ISOPGNProvider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
        // nullptr
        setISOPGN(nullptr);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr ISOPGNProvider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::isopgn::IsopgnPackage* >(::nevonex::isopgn::IsopgnPackage::_instance().get())->getISOPGNProvider();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ISOPGNProvider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ISOPGNProvider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
