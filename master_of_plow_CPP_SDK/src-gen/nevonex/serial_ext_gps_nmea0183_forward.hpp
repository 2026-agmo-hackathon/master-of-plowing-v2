/*
 * nevonex/serial_ext_gps_nmea0183_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_SERIAL_EXT_GPS_NMEA0183_FORWARD_HPP
#define _NEVONEX_SERIAL_EXT_GPS_NMEA0183_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_serial_ext_gps_nmea0183_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace serial_ext_gps_nmea0183
    {

// EDataType

// EClass

// Serial_Ext_GPS_NMEA0183
        class Serial_Ext_GPS_NMEA0183;
        using Serial_Ext_GPS_NMEA0183_ptr = ::ecore::Ptr<Serial_Ext_GPS_NMEA0183>;

// Serial_Ext_GPS_NMEA0183Provider
        class Serial_Ext_GPS_NMEA0183Provider;
        using Serial_Ext_GPS_NMEA0183Provider_ptr = ::ecore::Ptr<Serial_Ext_GPS_NMEA0183Provider>;

// EEnum

// Package & Factory
        class Serial_ext_gps_nmea0183Factory;
        using Serial_ext_gps_nmea0183Factory_ptr = ::ecore::Ptr<Serial_ext_gps_nmea0183Factory>;
        class Serial_ext_gps_nmea0183Package;
        using Serial_ext_gps_nmea0183Package_ptr = ::ecore::Ptr<Serial_ext_gps_nmea0183Package>;

    } // serial_ext_gps_nmea0183
} // nevonex

#endif // _NEVONEX_SERIAL_EXT_GPS_NMEA0183_FORWARD_HPP
