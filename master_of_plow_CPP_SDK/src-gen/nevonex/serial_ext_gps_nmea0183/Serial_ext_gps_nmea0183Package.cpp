/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp>

using namespace ::nevonex::serial_ext_gps_nmea0183;

::ecore::Ptr< ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package > Serial_ext_gps_nmea0183Package::s_instance;

::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package_ptr Serial_ext_gps_nmea0183Package::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< Serial_ext_gps_nmea0183Package >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < Serial_ext_gps_nmea0183Package > (new Serial_ext_gps_nmea0183Package());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package_ptr Serial_ext_gps_nmea0183Package::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
