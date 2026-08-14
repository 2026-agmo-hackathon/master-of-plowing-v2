/*
 * nevonex/fcal/RAW_CAN_DATA_DLC_8_WImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "RAW_CAN_DATA_DLC_8_W.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_WImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void RAW_CAN_DATA_DLC_8_W::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_WImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject RAW_CAN_DATA_DLC_8_W::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE0:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte0);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE1:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte1);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE2:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte2);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE3:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte3);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE4:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte4);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE5:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte5);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE6:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte6);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE7:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_byte7);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void RAW_CAN_DATA_DLC_8_W::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE0:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte0(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE1:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte1(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE2:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte2(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE3:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte3(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE4:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte4(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE5:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte5(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE6:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte6(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE7:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W::setByte7(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean RAW_CAN_DATA_DLC_8_W::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE0:
    {
        return m_byte0 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE1:
    {
        return m_byte1 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE2:
    {
        return m_byte2 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE3:
    {
        return m_byte3 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE4:
    {
        return m_byte4 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE5:
    {
        return m_byte5 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE6:
    {
        return m_byte6 != 0;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE7:
    {
        return m_byte7 != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void RAW_CAN_DATA_DLC_8_W::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE0:
    {
        // 0
        setByte0(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE1:
    {
        // 0
        setByte1(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE2:
    {
        // 0
        setByte2(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE3:
    {
        // 0
        setByte3(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE4:
    {
        // 0
        setByte4(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE5:
    {
        // 0
        setByte5(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE6:
    {
        // 0
        setByte6(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RAW_CAN_DATA_DLC_8_W__BYTE7:
    {
        // 0
        setByte7(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr RAW_CAN_DATA_DLC_8_W::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRAW_CAN_DATA_DLC_8_W();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void RAW_CAN_DATA_DLC_8_W::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void RAW_CAN_DATA_DLC_8_W::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
