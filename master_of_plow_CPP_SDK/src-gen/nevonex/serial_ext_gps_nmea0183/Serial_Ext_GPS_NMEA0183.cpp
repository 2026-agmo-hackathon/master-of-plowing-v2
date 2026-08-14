/*
 * nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "Serial_Ext_GPS_NMEA0183.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::serial_ext_gps_nmea0183;
using namespace ::nevonex::log;

// Default constructor
Serial_Ext_GPS_NMEA0183::Serial_Ext_GPS_NMEA0183() : m_serial_Ext_GPS_NMEA0183_Data(0)
{
    // Machine does NOT call _initialize() — Provider manages initialization

    m_interfaceDetailMap =
    {
    { 9465, ::nevonex::types::InterfaceDetails("fek/9465") } };

    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183__Serial_Ext_GPS_NMEA0183) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Serial_Ext_GPS_NMEA0183::~Serial_Ext_GPS_NMEA0183()
{
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// References
::nevonex::fcal::GPS_NMEA0183_T_R_ptr Serial_Ext_GPS_NMEA0183::getSerial_Ext_GPS_NMEA0183_Data() const
{
    return m_serial_Ext_GPS_NMEA0183_Data;
}

void Serial_Ext_GPS_NMEA0183::setSerial_Ext_GPS_NMEA0183_Data(::nevonex::fcal::GPS_NMEA0183_T_R_ptr _serial_Ext_GPS_NMEA0183_Data)
{
    ::nevonex::fcal::GPS_NMEA0183_T_R_ptr _old_serial_Ext_GPS_NMEA0183_Data = m_serial_Ext_GPS_NMEA0183_Data;
    m_serial_Ext_GPS_NMEA0183_Data = _serial_Ext_GPS_NMEA0183_Data;
    SDK_FCAL_LOG(SeverityLevel::debug) << "Serial_Ext_GPS_NMEA0183 property \"serial_Ext_GPS_NMEA0183_Data\" has changed.";
    notifyPropertyChange("serial_Ext_GPS_NMEA0183_Data", _old_serial_Ext_GPS_NMEA0183_Data, m_serial_Ext_GPS_NMEA0183_Data);
}

// FCAL timestamp/valid
::ecore::ELong Serial_Ext_GPS_NMEA0183::getSerial_Ext_GPS_NMEA0183_Data_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9465);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean Serial_Ext_GPS_NMEA0183::isSerial_Ext_GPS_NMEA0183_Data_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9465);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::EBoolean Serial_Ext_GPS_NMEA0183::updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp)
{
    auto _detail = m_interfaceDetailMap.find(_topicKey);
    if (_detail == m_interfaceDetailMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::warning) << _topicKey
                << " interface details are not present in the interface detail map[Serial_Ext_GPS_NMEA0183].";
        return false;
    }

    if (_detail->second.getTimestamp() > _timestamp)
    {
        SDK_FCAL_LOG(SeverityLevel::warning)
                << "Old or duplicate message received. Old TS : "
                << _detail->second.getTimestamp() << " new TS : " << _timestamp
                << " feature: Serial_Ext_GPS_NMEA0183 topic:" << _topicKey;
        return false;
    }

    _detail->second.setValid(_valid, _timestamp);
    return true;
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void Serial_Ext_GPS_NMEA0183::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void Serial_Ext_GPS_NMEA0183::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void Serial_Ext_GPS_NMEA0183::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
