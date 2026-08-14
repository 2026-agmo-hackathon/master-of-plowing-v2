/*
 * nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Impl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "Serial_Ext_GPS_NMEA0183.hpp"
#include <stdexcept>

#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/fcal/GPS_NMEA0183_T_R.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Impl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::serial_ext_gps_nmea0183;
using namespace ::nevonex::log;

void Serial_Ext_GPS_NMEA0183::_initialize()
{
    // Supertypes
    ::nevonex::common::TopicObject::_initialize();

    // References

    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Impl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject Serial_Ext_GPS_NMEA0183::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
        if (m_serial_Ext_GPS_NMEA0183_Data)
            _any = ::ecore::as < ::ecore::EObject > (m_serial_Ext_GPS_NMEA0183_Data);
        return _any;
    }
    }
    return ::nevonex::common::TopicObject::eGet(_featureID, _resolve);
}

void Serial_Ext_GPS_NMEA0183::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::GPS_NMEA0183_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::GPS_NMEA0183_T_R* >(_t0.get());
        ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183::setSerial_Ext_GPS_NMEA0183_Data(_t1);
        return;
    }
    }
    ::nevonex::common::TopicObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean Serial_Ext_GPS_NMEA0183::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
        return (bool) m_serial_Ext_GPS_NMEA0183_Data;
    }
    }
    return ::nevonex::common::TopicObject::eIsSet(_featureID);
}

void Serial_Ext_GPS_NMEA0183::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
        // nullptr
        setSerial_Ext_GPS_NMEA0183_Data(nullptr);
        return;
    }
    }
    ::nevonex::common::TopicObject::eUnset(_featureID);
}

::ecore::EClass_ptr Serial_Ext_GPS_NMEA0183::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package* >(::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::_instance().get())->getSerial_Ext_GPS_NMEA0183();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void Serial_Ext_GPS_NMEA0183::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void Serial_Ext_GPS_NMEA0183::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
