/*
 * nevonex/isopgn/ISOPGNImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ISOPGN.hpp"
#include <stdexcept>

#include <nevonex/isopgn/IsopgnPackage.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ISOPGNImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::isopgn;
using namespace ::nevonex::log;

void ISOPGN::_initialize()
{
    // Supertypes
    ::nevonex::common::TopicObject::_initialize();

    // References

    /*PROTECTED REGION ID(ISOPGNImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ISOPGN::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGN__HITCHPOSPERRE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_hitchPosPerRe);
        return _any;
    }
    }
    return ::nevonex::common::TopicObject::eGet(_featureID, _resolve);
}

void ISOPGN::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGN__HITCHPOSPERRE:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::isopgn::ISOPGN::setHitchPosPerRe(_t0);
        return;
    }
    }
    ::nevonex::common::TopicObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ISOPGN::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::isopgn::IsopgnPackage::ISOPGN__HITCHPOSPERRE:
    {
        return m_hitchPosPerRe != 0.0;
    }
    }
    return ::nevonex::common::TopicObject::eIsSet(_featureID);
}

void ISOPGN::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::isopgn::IsopgnPackage::ISOPGN__HITCHPOSPERRE:
    {
        // 0.0
        setHitchPosPerRe(0.0);
        return;
    }
    }
    ::nevonex::common::TopicObject::eUnset(_featureID);
}

::ecore::EClass_ptr ISOPGN::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::isopgn::IsopgnPackage* >(::nevonex::isopgn::IsopgnPackage::_instance().get())->getISOPGN();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ISOPGN::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ISOPGN::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
