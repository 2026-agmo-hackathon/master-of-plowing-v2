/*
 * nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_WImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "AGMO_GENERIC_SEND_CMD_T_W.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_WImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void AGMO_GENERIC_SEND_CMD_T_W::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_WImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject AGMO_GENERIC_SEND_CMD_T_W::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_aD_Generic_Cmd);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_aD_Generic_Mode_Cmd);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void AGMO_GENERIC_SEND_CMD_T_W::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W::setAD_Generic_Cmd(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W::setAD_Generic_Mode_Cmd(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean AGMO_GENERIC_SEND_CMD_T_W::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
    {
        return m_aD_Generic_Cmd != 0;
    }
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
    {
        return m_aD_Generic_Mode_Cmd != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void AGMO_GENERIC_SEND_CMD_T_W::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
    {
        // 0
        setAD_Generic_Cmd(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
    {
        // 0
        setAD_Generic_Mode_Cmd(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr AGMO_GENERIC_SEND_CMD_T_W::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getAGMO_GENERIC_SEND_CMD_T_W();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void AGMO_GENERIC_SEND_CMD_T_W::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void AGMO_GENERIC_SEND_CMD_T_W::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
