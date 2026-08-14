/*
 * nevonex/fcal/ACEINNA_ANGLES_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ANGLES_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ACEINNA_ANGLES_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void ACEINNA_ANGLES_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ACEINNA_ANGLES_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ACEINNA_ANGLES_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_pitch);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_roll);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_pitch_Compensation);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_pitch_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_roll_Compensation);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_roll_FigureOfMerit);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_pitchRoll_Latency);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void ACEINNA_ANGLES_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setPitch(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setRoll(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setPitch_Compensation(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setPitch_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setRoll_Compensation(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setRoll_FigureOfMerit(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R::setPitchRoll_Latency(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ACEINNA_ANGLES_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH:
    {
        return m_pitch != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL:
    {
        return m_roll != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
    {
        return m_pitch_Compensation != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT:
    {
        return m_pitch_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
    {
        return m_roll_Compensation != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT:
    {
        return m_roll_FigureOfMerit != 0;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY:
    {
        return m_pitchRoll_Latency != 0.0f;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void ACEINNA_ANGLES_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH:
    {
        // 0.0f
        setPitch(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL:
    {
        // 0.0f
        setRoll(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
    {
        // 0
        setPitch_Compensation(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCH_FIGUREOFMERIT:
    {
        // 0
        setPitch_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
    {
        // 0
        setRoll_Compensation(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__ROLL_FIGUREOFMERIT:
    {
        // 0
        setRoll_FigureOfMerit(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::ACEINNA_ANGLES_T_R__PITCHROLL_LATENCY:
    {
        // 0.0f
        setPitchRoll_Latency(0.0f);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr ACEINNA_ANGLES_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ANGLES_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ACEINNA_ANGLES_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ACEINNA_ANGLES_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
