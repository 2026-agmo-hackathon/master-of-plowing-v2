/*
 * nevonex/fcal/GPS_NMEA0183_T_RImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "GPS_NMEA0183_T_R.hpp"
#include <stdexcept>

#include <nevonex/fcal/FcalPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(GPS_NMEA0183_T_RImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

void GPS_NMEA0183_T_R::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(GPS_NMEA0183_T_RImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject GPS_NMEA0183_T_R::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__UTC_MS:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::toAny(_any, m_uTC_ms);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LATITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::toAny(_any, m_latitude);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LONGITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::toAny(_any, m_longitude);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_gPS_Quality_Indicator);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__NO_SATELLITES:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_no_Satellites);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__ALTITUDE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_altitude);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_geoidal_Separation);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DGPS_AGE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_dGPS_Age);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_differential_Ref_Station);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SELECTION_MODE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::toAny(_any, m_selection_Mode);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FIX_TYPE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_fix_Type);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__PDOP:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_pDOP);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HDOP:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_hDOP);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__VDOP:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_vDOP);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__STATUS:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_status);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SPEED:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_speed);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__COURSE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_course);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_magnetic_variation);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::toAny(_any, m_fAA_Mode_Indicator);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADING:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::toAny(_any, m_heading);
        return _any;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADINGTRUE:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_headingTrue);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void GPS_NMEA0183_T_R::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__UTC_MS:
    {
        ::ecore::EString _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setUTC_ms(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LATITUDE:
    {
        ::ecore::EDouble _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setLatitude(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LONGITUDE:
    {
        ::ecore::EDouble _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EDouble
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setLongitude(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setGPS_Quality_Indicator(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__NO_SATELLITES:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setNo_Satellites(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__ALTITUDE:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setAltitude(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setGeoidal_Separation(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DGPS_AGE:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setDGPS_Age(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setDifferential_Ref_Station(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SELECTION_MODE:
    {
        ::ecore::EString _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setSelection_Mode(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FIX_TYPE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setFix_Type(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__PDOP:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setPDOP(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HDOP:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setHDOP(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__VDOP:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setVDOP(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__STATUS:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setStatus(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SPEED:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setSpeed(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__COURSE:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setCourse(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setMagnetic_variation(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
    {
        ::ecore::EString _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EString
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setFAA_Mode_Indicator(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADING:
    {
        ::ecore::EFloat _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EFloat
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setHeading(_t0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADINGTRUE:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::fcal::GPS_NMEA0183_T_R::setHeadingTrue(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean GPS_NMEA0183_T_R::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__UTC_MS:
    {
        return m_uTC_ms != "";
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LATITUDE:
    {
        return m_latitude != 0.0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LONGITUDE:
    {
        return m_longitude != 0.0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
    {
        return m_gPS_Quality_Indicator != 0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__NO_SATELLITES:
    {
        return m_no_Satellites != 0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__ALTITUDE:
    {
        return m_altitude != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
    {
        return m_geoidal_Separation != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DGPS_AGE:
    {
        return m_dGPS_Age != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
    {
        return m_differential_Ref_Station != 0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SELECTION_MODE:
    {
        return m_selection_Mode != "";
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FIX_TYPE:
    {
        return m_fix_Type != 0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__PDOP:
    {
        return m_pDOP != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HDOP:
    {
        return m_hDOP != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__VDOP:
    {
        return m_vDOP != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__STATUS:
    {
        return m_status != 0;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SPEED:
    {
        return m_speed != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__COURSE:
    {
        return m_course != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
    {
        return m_magnetic_variation != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
    {
        return m_fAA_Mode_Indicator != "";
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADING:
    {
        return m_heading != 0.0f;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADINGTRUE:
    {
        return m_headingTrue != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void GPS_NMEA0183_T_R::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__UTC_MS:
    {
        // ""
        setUTC_ms("");
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LATITUDE:
    {
        // 0.0
        setLatitude(0.0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__LONGITUDE:
    {
        // 0.0
        setLongitude(0.0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
    {
        // 0
        setGPS_Quality_Indicator(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__NO_SATELLITES:
    {
        // 0
        setNo_Satellites(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__ALTITUDE:
    {
        // 0.0f
        setAltitude(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
    {
        // 0.0f
        setGeoidal_Separation(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DGPS_AGE:
    {
        // 0.0f
        setDGPS_Age(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
    {
        // 0
        setDifferential_Ref_Station(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SELECTION_MODE:
    {
        // ""
        setSelection_Mode("");
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FIX_TYPE:
    {
        // 0
        setFix_Type(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__PDOP:
    {
        // 0.0f
        setPDOP(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HDOP:
    {
        // 0.0f
        setHDOP(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__VDOP:
    {
        // 0.0f
        setVDOP(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__STATUS:
    {
        // 0
        setStatus(0);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__SPEED:
    {
        // 0.0f
        setSpeed(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__COURSE:
    {
        // 0.0f
        setCourse(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
    {
        // 0.0f
        setMagnetic_variation(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
    {
        // ""
        setFAA_Mode_Indicator("");
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADING:
    {
        // 0.0f
        setHeading(0.0f);
        return;
    }
    case ::nevonex::fcal::FcalPackage::GPS_NMEA0183_T_R__HEADINGTRUE:
    {
        // 0
        setHeadingTrue(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr GPS_NMEA0183_T_R::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getGPS_NMEA0183_T_R();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void GPS_NMEA0183_T_R::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void GPS_NMEA0183_T_R::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
