/*
 * nevonex/common/AbsolutePosition.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "AbsolutePosition.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/common/CommonPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(AbsolutePosition.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::common;
using namespace ::nevonex::log;

// Default constructor
AbsolutePosition::AbsolutePosition() : m_altitude(0.0), m_latitude(0.0), m_longitude(0.0)
{

    /*PROTECTED REGION ID(AbsolutePosition__AbsolutePosition) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

AbsolutePosition::~AbsolutePosition()
{

    /*PROTECTED REGION ID(AbsolutePosition__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EDouble AbsolutePosition::getAltitude() const
{
    return m_altitude;
}

void AbsolutePosition::setAltitude(::ecore::EDouble _altitude)
{

    ::ecore::EDouble _old_altitude = m_altitude;
    m_altitude = _altitude;

}

::ecore::EDouble AbsolutePosition::getLatitude() const
{
    return m_latitude;
}

void AbsolutePosition::setLatitude(::ecore::EDouble _latitude)
{

    ::ecore::EDouble _old_latitude = m_latitude;
    m_latitude = _latitude;

}

::ecore::EDouble AbsolutePosition::getLongitude() const
{
    return m_longitude;
}

void AbsolutePosition::setLongitude(::ecore::EDouble _longitude)
{

    ::ecore::EDouble _old_longitude = m_longitude;
    m_longitude = _longitude;

}

// References

