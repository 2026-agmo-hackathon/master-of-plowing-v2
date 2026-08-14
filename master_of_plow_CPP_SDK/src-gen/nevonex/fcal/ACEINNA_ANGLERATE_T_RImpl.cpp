/*
 * nevonex/fcal/ACEINNA_ANGLERATE_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ANGLERATE_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ACEINNA_ANGLERATE_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void ACEINNA_ANGLERATE_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ACEINNA_ANGLERATE_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ACEINNA_ANGLERATE_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_gyroY);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_gyroX);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_gyroZ);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_gyroY_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_gyroX_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_gyroZ_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_angleRate_Latency);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void ACEINNA_ANGLERATE_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroY(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroX(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroZ(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroY_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroX_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setGyroZ_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R::setAngleRate_Latency(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ACEINNA_ANGLERATE_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY:
    {
        return m_gyroY != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX:
    {
        return m_gyroX != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ:
    {
        return m_gyroZ != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT:
    {
        return m_gyroY_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT:
    {
        return m_gyroX_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT:
    {
        return m_gyroZ_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY:
    {
        return m_angleRate_Latency != 0.0f;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void ACEINNA_ANGLERATE_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY:
    {
        // 0.0f
        setGyroY(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX:
    {
        // 0.0f
        setGyroX(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ:
    {
        // 0.0f
        setGyroZ(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROY_FIGUREOFMERIT:
    {
        // 0
        setGyroY_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROX_FIGUREOFMERIT:
    {
        // 0
        setGyroX_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__GYROZ_FIGUREOFMERIT:
    {
        // 0
        setGyroZ_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLERATE_T_R__ANGLERATE_LATENCY:
    {
        // 0.0f
        setAngleRate_Latency(0.0f);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr ACEINNA_ANGLERATE_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ANGLERATE_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ACEINNA_ANGLERATE_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ACEINNA_ANGLERATE_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
