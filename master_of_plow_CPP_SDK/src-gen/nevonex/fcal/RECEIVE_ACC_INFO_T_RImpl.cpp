/*
 * nevonex/fcal/RECEIVE_ACC_INFO_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "RECEIVE_ACC_INFO_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(RECEIVE_ACC_INFO_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void RECEIVE_ACC_INFO_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(RECEIVE_ACC_INFO_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject RECEIVE_ACC_INFO_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_tRZ_ACC_SIG1_V);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_tRZ_ACC_SIG2_V);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_tRZ_ACC_DIAG);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_tRZ_ACC_AUTO);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void RECEIVE_ACC_INFO_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R::setTRZ_ACC_SIG1_V(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R::setTRZ_ACC_SIG2_V(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R::setTRZ_ACC_DIAG(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R::setTRZ_ACC_AUTO(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean RECEIVE_ACC_INFO_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
    {
        return m_tRZ_ACC_SIG1_V != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
    {
        return m_tRZ_ACC_SIG2_V != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
    {
        return m_tRZ_ACC_DIAG != 0;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
    {
        return m_tRZ_ACC_AUTO != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void RECEIVE_ACC_INFO_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
    {
        // 0.0f
        setTRZ_ACC_SIG1_V(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
    {
        // 0.0f
        setTRZ_ACC_SIG2_V(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
    {
        // 0
        setTRZ_ACC_DIAG(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
    {
        // 0
        setTRZ_ACC_AUTO(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr RECEIVE_ACC_INFO_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRECEIVE_ACC_INFO_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void RECEIVE_ACC_INFO_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void RECEIVE_ACC_INFO_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
