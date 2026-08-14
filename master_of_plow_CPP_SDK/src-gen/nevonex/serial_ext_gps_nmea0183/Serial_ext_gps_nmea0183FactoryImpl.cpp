/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183FactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::serial_ext_gps_nmea0183;

Serial_ext_gps_nmea0183Factory::Serial_ext_gps_nmea0183Factory()
{
}

::ecore::EObject_ptr Serial_ext_gps_nmea0183Factory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183:
        return createSerial_Ext_GPS_NMEA0183();
    case Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183PROVIDER:
        return createSerial_Ext_GPS_NMEA0183Provider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject Serial_ext_gps_nmea0183Factory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    (void)_literalValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString Serial_ext_gps_nmea0183Factory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    (void)_instanceValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

Serial_Ext_GPS_NMEA0183_ptr Serial_ext_gps_nmea0183Factory::createSerial_Ext_GPS_NMEA0183()
{
    return ::ecore::Ptr < Serial_Ext_GPS_NMEA0183 > (new Serial_Ext_GPS_NMEA0183);
}
Serial_Ext_GPS_NMEA0183Provider_ptr Serial_ext_gps_nmea0183Factory::createSerial_Ext_GPS_NMEA0183Provider()
{
    return ::ecore::Ptr < Serial_Ext_GPS_NMEA0183Provider > (new Serial_Ext_GPS_NMEA0183Provider);
}
