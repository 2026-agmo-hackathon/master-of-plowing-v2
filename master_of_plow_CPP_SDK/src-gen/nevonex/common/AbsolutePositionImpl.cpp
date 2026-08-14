/*
 * nevonex/common/AbsolutePositionImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "AbsolutePosition.hpp"
#include <stdexcept>

#include <nevonex/common/CommonPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(AbsolutePositionImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::common;
using namespace ::nevonex::log;

void AbsolutePosition::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(AbsolutePositionImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject AbsolutePosition::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__ALTITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::toAny(_any, m_altitude);
        return _any;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LATITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::toAny(_any, m_latitude);
        return _any;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LONGITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::toAny(_any, m_longitude);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void AbsolutePosition::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__ALTITUDE:
    {
        ::ecore::EDouble _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::fromAny(_newValue, _t0);
        ::nevonex::common::AbsolutePosition::setAltitude(_t0);
        return;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LATITUDE:
    {
        ::ecore::EDouble _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::fromAny(_newValue, _t0);
        ::nevonex::common::AbsolutePosition::setLatitude(_t0);
        return;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LONGITUDE:
    {
        ::ecore::EDouble _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::fromAny(_newValue, _t0);
        ::nevonex::common::AbsolutePosition::setLongitude(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean AbsolutePosition::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__ALTITUDE:
    {
        return m_altitude != 0.0;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LATITUDE:
    {
        return m_latitude != 0.0;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LONGITUDE:
    {
        return m_longitude != 0.0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void AbsolutePosition::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__ALTITUDE:
    {
        // 0.0
        setAltitude(0.0);
        return;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LATITUDE:
    {
        // 0.0
        setLatitude(0.0);
        return;
    }
    case ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LONGITUDE:
    {
        // 0.0
        setLongitude(0.0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr AbsolutePosition::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getAbsolutePosition();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void AbsolutePosition::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void AbsolutePosition::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
