/*
 * AppMain/ApplicationInputData.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ApplicationInputData.hpp"
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>
#include <nevonex/isopgn/ISOPGNProvider.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(ApplicationInputData.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

// Default constructor
ApplicationInputData::ApplicationInputData()
    : m_cAN_AGMO_Customized_TractorProvider(0), m_cAN_AGMO_MTLT305Provider(0), m_cAN_AGMO_SteerMotorProvider(0), m_iSOPGNProvider(0), m_serial_Ext_GPS_NMEA0183Provider(0)
{

    /*PROTECTED REGION ID(ApplicationInputData__ApplicationInputData) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

ApplicationInputData::~ApplicationInputData()
{

    /*PROTECTED REGION ID(ApplicationInputData__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References

::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider_ptr ApplicationInputData::getCAN_AGMO_Customized_TractorProvider() const
{
    return m_cAN_AGMO_Customized_TractorProvider;
}

void ApplicationInputData::setCAN_AGMO_Customized_TractorProvider(
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider_ptr _cAN_AGMO_Customized_TractorProvider)
{

    m_cAN_AGMO_Customized_TractorProvider = _cAN_AGMO_Customized_TractorProvider;

}

::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider_ptr ApplicationInputData::getCAN_AGMO_MTLT305Provider() const
{
    return m_cAN_AGMO_MTLT305Provider;
}

void ApplicationInputData::setCAN_AGMO_MTLT305Provider(
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider_ptr _cAN_AGMO_MTLT305Provider)
{

    m_cAN_AGMO_MTLT305Provider = _cAN_AGMO_MTLT305Provider;

}

::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider_ptr ApplicationInputData::getCAN_AGMO_SteerMotorProvider() const
{
    return m_cAN_AGMO_SteerMotorProvider;
}

void ApplicationInputData::setCAN_AGMO_SteerMotorProvider(
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider_ptr _cAN_AGMO_SteerMotorProvider)
{

    m_cAN_AGMO_SteerMotorProvider = _cAN_AGMO_SteerMotorProvider;

}

::nevonex::isopgn::ISOPGNProvider_ptr ApplicationInputData::getISOPGNProvider() const
{
    return m_iSOPGNProvider;
}

void ApplicationInputData::setISOPGNProvider(
        ::nevonex::isopgn::ISOPGNProvider_ptr _iSOPGNProvider)
{

    m_iSOPGNProvider = _iSOPGNProvider;

}

::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183Provider_ptr ApplicationInputData::getSerial_Ext_GPS_NMEA0183Provider() const
{
    return m_serial_Ext_GPS_NMEA0183Provider;
}

void ApplicationInputData::setSerial_Ext_GPS_NMEA0183Provider(
        ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183Provider_ptr _serial_Ext_GPS_NMEA0183Provider)
{

    m_serial_Ext_GPS_NMEA0183Provider = _serial_Ext_GPS_NMEA0183Provider;

}

