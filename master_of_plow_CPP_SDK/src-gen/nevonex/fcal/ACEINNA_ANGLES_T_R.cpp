/*
 * nevonex/fcal/ACEINNA_ANGLES_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ANGLES_T_R.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcal/FcalPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(ACEINNA_ANGLES_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
ACEINNA_ANGLES_T_R::ACEINNA_ANGLES_T_R() : m_pitch(0.0f), m_roll(0.0f), m_pitch_Compensation(0), m_pitch_FigureOfMerit(0), m_roll_Compensation(0), m_roll_FigureOfMerit(0), m_pitchRoll_Latency(0.0f)
{

    /*PROTECTED REGION ID(ACEINNA_ANGLES_T_R__ACEINNA_ANGLES_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

ACEINNA_ANGLES_T_R::~ACEINNA_ANGLES_T_R()
{

    /*PROTECTED REGION ID(ACEINNA_ANGLES_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EFloat ACEINNA_ANGLES_T_R::getPitch() const
{
    return m_pitch;
}

void ACEINNA_ANGLES_T_R::setPitch(::ecore::EFloat _pitch)
{

    ::ecore::EFloat _old_pitch = m_pitch;
    m_pitch = _pitch;

}

::ecore::EFloat ACEINNA_ANGLES_T_R::getRoll() const
{
    return m_roll;
}

void ACEINNA_ANGLES_T_R::setRoll(::ecore::EFloat _roll)
{

    ::ecore::EFloat _old_roll = m_roll;
    m_roll = _roll;

}

::ecore::EInt ACEINNA_ANGLES_T_R::getPitch_Compensation() const
{
    return m_pitch_Compensation;
}

void ACEINNA_ANGLES_T_R::setPitch_Compensation(::ecore::EInt _pitch_Compensation)
{

    ::ecore::EInt _old_pitch_Compensation = m_pitch_Compensation;
    m_pitch_Compensation = _pitch_Compensation;

}

::ecore::EInt ACEINNA_ANGLES_T_R::getPitch_FigureOfMerit() const
{
    return m_pitch_FigureOfMerit;
}

void ACEINNA_ANGLES_T_R::setPitch_FigureOfMerit(::ecore::EInt _pitch_FigureOfMerit)
{

    ::ecore::EInt _old_pitch_FigureOfMerit = m_pitch_FigureOfMerit;
    m_pitch_FigureOfMerit = _pitch_FigureOfMerit;

}

::ecore::EInt ACEINNA_ANGLES_T_R::getRoll_Compensation() const
{
    return m_roll_Compensation;
}

void ACEINNA_ANGLES_T_R::setRoll_Compensation(::ecore::EInt _roll_Compensation)
{

    ::ecore::EInt _old_roll_Compensation = m_roll_Compensation;
    m_roll_Compensation = _roll_Compensation;

}

::ecore::EInt ACEINNA_ANGLES_T_R::getRoll_FigureOfMerit() const
{
    return m_roll_FigureOfMerit;
}

void ACEINNA_ANGLES_T_R::setRoll_FigureOfMerit(::ecore::EInt _roll_FigureOfMerit)
{

    ::ecore::EInt _old_roll_FigureOfMerit = m_roll_FigureOfMerit;
    m_roll_FigureOfMerit = _roll_FigureOfMerit;

}

::ecore::EFloat ACEINNA_ANGLES_T_R::getPitchRoll_Latency() const
{
    return m_pitchRoll_Latency;
}

void ACEINNA_ANGLES_T_R::setPitchRoll_Latency(::ecore::EFloat _pitchRoll_Latency)
{

    ::ecore::EFloat _old_pitchRoll_Latency = m_pitchRoll_Latency;
    m_pitchRoll_Latency = _pitchRoll_Latency;

}

// References

