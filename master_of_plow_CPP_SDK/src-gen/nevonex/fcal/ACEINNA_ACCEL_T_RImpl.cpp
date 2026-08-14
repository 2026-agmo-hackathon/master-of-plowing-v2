/*
 * nevonex/fcal/ACEINNA_ACCEL_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ACCEL_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ACEINNA_ACCEL_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void ACEINNA_ACCEL_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ACEINNA_ACCEL_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ACEINNA_ACCEL_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCY:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_accY);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCX:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_accX);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCZ:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_accZ);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_lateralAcc_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_longiAcc_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_verticAcc_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_support_Rate_Acc);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void ACEINNA_ACCEL_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCY:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setAccY(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCX:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setAccX(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCZ:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setAccZ(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setLateralAcc_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setLongiAcc_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setVerticAcc_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R::setSupport_Rate_Acc(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ACEINNA_ACCEL_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCY:
    {
        return m_accY != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCX:
    {
        return m_accX != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCZ:
    {
        return m_accZ != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT:
    {
        return m_lateralAcc_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT:
    {
        return m_longiAcc_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT:
    {
        return m_verticAcc_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
    {
        return m_support_Rate_Acc != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void ACEINNA_ACCEL_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCY:
    {
        // 0.0f
        setAccY(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCX:
    {
        // 0.0f
        setAccX(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__ACCZ:
    {
        // 0.0f
        setAccZ(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LATERALACC_FIGUREOFMERIT:
    {
        // 0
        setLateralAcc_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__LONGIACC_FIGUREOFMERIT:
    {
        // 0
        setLongiAcc_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__VERTICACC_FIGUREOFMERIT:
    {
        // 0
        setVerticAcc_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
    {
        // 0
        setSupport_Rate_Acc(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr ACEINNA_ACCEL_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ACCEL_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ACEINNA_ACCEL_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ACEINNA_ACCEL_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
