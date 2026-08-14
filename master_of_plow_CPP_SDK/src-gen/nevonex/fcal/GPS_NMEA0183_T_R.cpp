/*
 * nevonex/fcal/GPS_NMEA0183_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "GPS_NMEA0183_T_R.hpp"
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

/*PROTECTED REGION ID(GPS_NMEA0183_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
GPS_NMEA0183_T_R::GPS_NMEA0183_T_R() : m_uTC_ms(""), m_latitude(0.0), m_longitude(0.0), m_gPS_Quality_Indicator(0), m_no_Satellites(0), m_altitude(0.0f), m_geoidal_Separation(0.0f), m_dGPS_Age(0.0f), m_differential_Ref_Station(0), m_selection_Mode(""), m_fix_Type(0), m_pDOP(0.0f), m_hDOP(0.0f), m_vDOP(0.0f), m_status(0), m_speed(0.0f), m_course(0.0f), m_magnetic_variation(0.0f), m_fAA_Mode_Indicator(""), m_heading(0.0f), m_headingTrue(0)
{

    /*PROTECTED REGION ID(GPS_NMEA0183_T_R__GPS_NMEA0183_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

GPS_NMEA0183_T_R::~GPS_NMEA0183_T_R()
{

    /*PROTECTED REGION ID(GPS_NMEA0183_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EString GPS_NMEA0183_T_R::getUTC_ms() const
{
    return m_uTC_ms;
}

void GPS_NMEA0183_T_R::setUTC_ms(::ecore::EString _uTC_ms)
{

    ::ecore::EString _old_uTC_ms = m_uTC_ms;
    m_uTC_ms = _uTC_ms;

}

::ecore::EDouble GPS_NMEA0183_T_R::getLatitude() const
{
    return m_latitude;
}

void GPS_NMEA0183_T_R::setLatitude(::ecore::EDouble _latitude)
{

    ::ecore::EDouble _old_latitude = m_latitude;
    m_latitude = _latitude;

}

::ecore::EDouble GPS_NMEA0183_T_R::getLongitude() const
{
    return m_longitude;
}

void GPS_NMEA0183_T_R::setLongitude(::ecore::EDouble _longitude)
{

    ::ecore::EDouble _old_longitude = m_longitude;
    m_longitude = _longitude;

}

::ecore::EInt GPS_NMEA0183_T_R::getGPS_Quality_Indicator() const
{
    return m_gPS_Quality_Indicator;
}

void GPS_NMEA0183_T_R::setGPS_Quality_Indicator(::ecore::EInt _gPS_Quality_Indicator)
{

    ::ecore::EInt _old_gPS_Quality_Indicator = m_gPS_Quality_Indicator;
    m_gPS_Quality_Indicator = _gPS_Quality_Indicator;

}

::ecore::EInt GPS_NMEA0183_T_R::getNo_Satellites() const
{
    return m_no_Satellites;
}

void GPS_NMEA0183_T_R::setNo_Satellites(::ecore::EInt _no_Satellites)
{

    ::ecore::EInt _old_no_Satellites = m_no_Satellites;
    m_no_Satellites = _no_Satellites;

}

::ecore::EFloat GPS_NMEA0183_T_R::getAltitude() const
{
    return m_altitude;
}

void GPS_NMEA0183_T_R::setAltitude(::ecore::EFloat _altitude)
{

    ::ecore::EFloat _old_altitude = m_altitude;
    m_altitude = _altitude;

}

::ecore::EFloat GPS_NMEA0183_T_R::getGeoidal_Separation() const
{
    return m_geoidal_Separation;
}

void GPS_NMEA0183_T_R::setGeoidal_Separation(::ecore::EFloat _geoidal_Separation)
{

    ::ecore::EFloat _old_geoidal_Separation = m_geoidal_Separation;
    m_geoidal_Separation = _geoidal_Separation;

}

::ecore::EFloat GPS_NMEA0183_T_R::getDGPS_Age() const
{
    return m_dGPS_Age;
}

void GPS_NMEA0183_T_R::setDGPS_Age(::ecore::EFloat _dGPS_Age)
{

    ::ecore::EFloat _old_dGPS_Age = m_dGPS_Age;
    m_dGPS_Age = _dGPS_Age;

}

::ecore::EInt GPS_NMEA0183_T_R::getDifferential_Ref_Station() const
{
    return m_differential_Ref_Station;
}

void GPS_NMEA0183_T_R::setDifferential_Ref_Station(::ecore::EInt _differential_Ref_Station)
{

    ::ecore::EInt _old_differential_Ref_Station = m_differential_Ref_Station;
    m_differential_Ref_Station = _differential_Ref_Station;

}

::ecore::EString GPS_NMEA0183_T_R::getSelection_Mode() const
{
    return m_selection_Mode;
}

void GPS_NMEA0183_T_R::setSelection_Mode(::ecore::EString _selection_Mode)
{

    ::ecore::EString _old_selection_Mode = m_selection_Mode;
    m_selection_Mode = _selection_Mode;

}

::ecore::EInt GPS_NMEA0183_T_R::getFix_Type() const
{
    return m_fix_Type;
}

void GPS_NMEA0183_T_R::setFix_Type(::ecore::EInt _fix_Type)
{

    ::ecore::EInt _old_fix_Type = m_fix_Type;
    m_fix_Type = _fix_Type;

}

::ecore::EFloat GPS_NMEA0183_T_R::getPDOP() const
{
    return m_pDOP;
}

void GPS_NMEA0183_T_R::setPDOP(::ecore::EFloat _pDOP)
{

    ::ecore::EFloat _old_pDOP = m_pDOP;
    m_pDOP = _pDOP;

}

::ecore::EFloat GPS_NMEA0183_T_R::getHDOP() const
{
    return m_hDOP;
}

void GPS_NMEA0183_T_R::setHDOP(::ecore::EFloat _hDOP)
{

    ::ecore::EFloat _old_hDOP = m_hDOP;
    m_hDOP = _hDOP;

}

::ecore::EFloat GPS_NMEA0183_T_R::getVDOP() const
{
    return m_vDOP;
}

void GPS_NMEA0183_T_R::setVDOP(::ecore::EFloat _vDOP)
{

    ::ecore::EFloat _old_vDOP = m_vDOP;
    m_vDOP = _vDOP;

}

::ecore::EInt GPS_NMEA0183_T_R::getStatus() const
{
    return m_status;
}

void GPS_NMEA0183_T_R::setStatus(::ecore::EInt _status)
{

    ::ecore::EInt _old_status = m_status;
    m_status = _status;

}

::ecore::EFloat GPS_NMEA0183_T_R::getSpeed() const
{
    return m_speed;
}

void GPS_NMEA0183_T_R::setSpeed(::ecore::EFloat _speed)
{

    ::ecore::EFloat _old_speed = m_speed;
    m_speed = _speed;

}

::ecore::EFloat GPS_NMEA0183_T_R::getCourse() const
{
    return m_course;
}

void GPS_NMEA0183_T_R::setCourse(::ecore::EFloat _course)
{

    ::ecore::EFloat _old_course = m_course;
    m_course = _course;

}

::ecore::EFloat GPS_NMEA0183_T_R::getMagnetic_variation() const
{
    return m_magnetic_variation;
}

void GPS_NMEA0183_T_R::setMagnetic_variation(::ecore::EFloat _magnetic_variation)
{

    ::ecore::EFloat _old_magnetic_variation = m_magnetic_variation;
    m_magnetic_variation = _magnetic_variation;

}

::ecore::EString GPS_NMEA0183_T_R::getFAA_Mode_Indicator() const
{
    return m_fAA_Mode_Indicator;
}

void GPS_NMEA0183_T_R::setFAA_Mode_Indicator(::ecore::EString _fAA_Mode_Indicator)
{

    ::ecore::EString _old_fAA_Mode_Indicator = m_fAA_Mode_Indicator;
    m_fAA_Mode_Indicator = _fAA_Mode_Indicator;

}

::ecore::EFloat GPS_NMEA0183_T_R::getHeading() const
{
    return m_heading;
}

void GPS_NMEA0183_T_R::setHeading(::ecore::EFloat _heading)
{

    ::ecore::EFloat _old_heading = m_heading;
    m_heading = _heading;

}

::ecore::EInt GPS_NMEA0183_T_R::getHeadingTrue() const
{
    return m_headingTrue;
}

void GPS_NMEA0183_T_R::setHeadingTrue(::ecore::EInt _headingTrue)
{

    ::ecore::EInt _old_headingTrue = m_headingTrue;
    m_headingTrue = _headingTrue;

}

// References

