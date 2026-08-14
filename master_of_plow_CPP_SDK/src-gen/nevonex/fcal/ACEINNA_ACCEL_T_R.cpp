/*
 * nevonex/fcal/ACEINNA_ACCEL_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ACCEL_T_R.hpp"
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

/*PROTECTED REGION ID(ACEINNA_ACCEL_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
ACEINNA_ACCEL_T_R::ACEINNA_ACCEL_T_R() : m_accY(0.0f), m_accX(0.0f), m_accZ(0.0f), m_lateralAcc_FigureOfMerit(0), m_longiAcc_FigureOfMerit(0), m_verticAcc_FigureOfMerit(0), m_support_Rate_Acc(0)
{

    /*PROTECTED REGION ID(ACEINNA_ACCEL_T_R__ACEINNA_ACCEL_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

ACEINNA_ACCEL_T_R::~ACEINNA_ACCEL_T_R()
{

    /*PROTECTED REGION ID(ACEINNA_ACCEL_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EFloat ACEINNA_ACCEL_T_R::getAccY() const
{
    return m_accY;
}

void ACEINNA_ACCEL_T_R::setAccY(::ecore::EFloat _accY)
{

    ::ecore::EFloat _old_accY = m_accY;
    m_accY = _accY;

}

::ecore::EFloat ACEINNA_ACCEL_T_R::getAccX() const
{
    return m_accX;
}

void ACEINNA_ACCEL_T_R::setAccX(::ecore::EFloat _accX)
{

    ::ecore::EFloat _old_accX = m_accX;
    m_accX = _accX;

}

::ecore::EFloat ACEINNA_ACCEL_T_R::getAccZ() const
{
    return m_accZ;
}

void ACEINNA_ACCEL_T_R::setAccZ(::ecore::EFloat _accZ)
{

    ::ecore::EFloat _old_accZ = m_accZ;
    m_accZ = _accZ;

}

::ecore::EInt ACEINNA_ACCEL_T_R::getLateralAcc_FigureOfMerit() const
{
    return m_lateralAcc_FigureOfMerit;
}

void ACEINNA_ACCEL_T_R::setLateralAcc_FigureOfMerit(::ecore::EInt _lateralAcc_FigureOfMerit)
{

    ::ecore::EInt _old_lateralAcc_FigureOfMerit = m_lateralAcc_FigureOfMerit;
    m_lateralAcc_FigureOfMerit = _lateralAcc_FigureOfMerit;

}

::ecore::EInt ACEINNA_ACCEL_T_R::getLongiAcc_FigureOfMerit() const
{
    return m_longiAcc_FigureOfMerit;
}

void ACEINNA_ACCEL_T_R::setLongiAcc_FigureOfMerit(::ecore::EInt _longiAcc_FigureOfMerit)
{

    ::ecore::EInt _old_longiAcc_FigureOfMerit = m_longiAcc_FigureOfMerit;
    m_longiAcc_FigureOfMerit = _longiAcc_FigureOfMerit;

}

::ecore::EInt ACEINNA_ACCEL_T_R::getVerticAcc_FigureOfMerit() const
{
    return m_verticAcc_FigureOfMerit;
}

void ACEINNA_ACCEL_T_R::setVerticAcc_FigureOfMerit(::ecore::EInt _verticAcc_FigureOfMerit)
{

    ::ecore::EInt _old_verticAcc_FigureOfMerit = m_verticAcc_FigureOfMerit;
    m_verticAcc_FigureOfMerit = _verticAcc_FigureOfMerit;

}

::ecore::EInt ACEINNA_ACCEL_T_R::getSupport_Rate_Acc() const
{
    return m_support_Rate_Acc;
}

void ACEINNA_ACCEL_T_R::setSupport_Rate_Acc(::ecore::EInt _support_Rate_Acc)
{

    ::ecore::EInt _old_support_Rate_Acc = m_support_Rate_Acc;
    m_support_Rate_Acc = _support_Rate_Acc;

}

// References

