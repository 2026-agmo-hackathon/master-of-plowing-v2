/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.hpp>

using namespace ::nevonex::serial_ext_gps_nmea0183;

::ecore::Ptr< ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Factory > Serial_ext_gps_nmea0183Factory::s_holder;

::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Factory_ptr Serial_ext_gps_nmea0183Factory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < Serial_ext_gps_nmea0183Factory > (new Serial_ext_gps_nmea0183Factory());

    return s_holder;
}
