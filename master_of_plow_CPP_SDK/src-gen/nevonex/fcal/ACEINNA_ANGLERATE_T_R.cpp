/*
 * nevonex/fcal/ACEINNA_ANGLERATE_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ACEINNA_ANGLERATE_T_R.hpp"
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

/*PROTECTED REGION ID(ACEINNA_ANGLERATE_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
ACEINNA_ANGLERATE_T_R::ACEINNA_ANGLERATE_T_R() : m_gyroY(0.0f), m_gyroX(0.0f), m_gyroZ(0.0f), m_gyroY_FigureOfMerit(0), m_gyroX_FigureOfMerit(0), m_gyroZ_FigureOfMerit(0), m_angleRate_Latency(0.0f)
{

    /*PROTECTED REGION ID(ACEINNA_ANGLERATE_T_R__ACEINNA_ANGLERATE_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

ACEINNA_ANGLERATE_T_R::~ACEINNA_ANGLERATE_T_R()
{

    /*PROTECTED REGION ID(ACEINNA_ANGLERATE_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EFloat ACEINNA_ANGLERATE_T_R::getGyroY() const
{
    return m_gyroY;
}

void ACEINNA_ANGLERATE_T_R::setGyroY(::ecore::EFloat _gyroY)
{

    ::ecore::EFloat _old_gyroY = m_gyroY;
    m_gyroY = _gyroY;

}

::ecore::EFloat ACEINNA_ANGLERATE_T_R::getGyroX() const
{
    return m_gyroX;
}

void ACEINNA_ANGLERATE_T_R::setGyroX(::ecore::EFloat _gyroX)
{

    ::ecore::EFloat _old_gyroX = m_gyroX;
    m_gyroX = _gyroX;

}

::ecore::EFloat ACEINNA_ANGLERATE_T_R::getGyroZ() const
{
    return m_gyroZ;
}

void ACEINNA_ANGLERATE_T_R::setGyroZ(::ecore::EFloat _gyroZ)
{

    ::ecore::EFloat _old_gyroZ = m_gyroZ;
    m_gyroZ = _gyroZ;

}

::ecore::EInt ACEINNA_ANGLERATE_T_R::getGyroY_FigureOfMerit() const
{
    return m_gyroY_FigureOfMerit;
}

void ACEINNA_ANGLERATE_T_R::setGyroY_FigureOfMerit(::ecore::EInt _gyroY_FigureOfMerit)
{

    ::ecore::EInt _old_gyroY_FigureOfMerit = m_gyroY_FigureOfMerit;
    m_gyroY_FigureOfMerit = _gyroY_FigureOfMerit;

}

::ecore::EInt ACEINNA_ANGLERATE_T_R::getGyroX_FigureOfMerit() const
{
    return m_gyroX_FigureOfMerit;
}

void ACEINNA_ANGLERATE_T_R::setGyroX_FigureOfMerit(::ecore::EInt _gyroX_FigureOfMerit)
{

    ::ecore::EInt _old_gyroX_FigureOfMerit = m_gyroX_FigureOfMerit;
    m_gyroX_FigureOfMerit = _gyroX_FigureOfMerit;

}

::ecore::EInt ACEINNA_ANGLERATE_T_R::getGyroZ_FigureOfMerit() const
{
    return m_gyroZ_FigureOfMerit;
}

void ACEINNA_ANGLERATE_T_R::setGyroZ_FigureOfMerit(::ecore::EInt _gyroZ_FigureOfMerit)
{

    ::ecore::EInt _old_gyroZ_FigureOfMerit = m_gyroZ_FigureOfMerit;
    m_gyroZ_FigureOfMerit = _gyroZ_FigureOfMerit;

}

::ecore::EFloat ACEINNA_ANGLERATE_T_R::getAngleRate_Latency() const
{
    return m_angleRate_Latency;
}

void ACEINNA_ANGLERATE_T_R::setAngleRate_Latency(::ecore::EFloat _angleRate_Latency)
{

    ::ecore::EFloat _old_angleRate_Latency = m_angleRate_Latency;
    m_angleRate_Latency = _angleRate_Latency;

}

// References

