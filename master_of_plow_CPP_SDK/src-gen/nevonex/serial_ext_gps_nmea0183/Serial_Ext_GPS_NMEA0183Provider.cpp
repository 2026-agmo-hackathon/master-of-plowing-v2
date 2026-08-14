/*
 * nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "Serial_Ext_GPS_NMEA0183Provider.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <sstream>

/*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::serial_ext_gps_nmea0183;
using namespace ::nevonex::log;

Serial_Ext_GPS_NMEA0183Provider::Serial_Ext_GPS_NMEA0183Provider() : m_serial_Ext_GPS_NMEA0183(0)
{
    _initialize();

    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__Serial_Ext_GPS_NMEA0183Provider) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Serial_Ext_GPS_NMEA0183Provider::~Serial_Ext_GPS_NMEA0183Provider()
{
    m_serial_Ext_GPS_NMEA0183.reset();
}

// References
::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183_ptr Serial_Ext_GPS_NMEA0183Provider::getSerial_Ext_GPS_NMEA0183() const
{
    return m_serial_Ext_GPS_NMEA0183;
}

void Serial_Ext_GPS_NMEA0183Provider::setSerial_Ext_GPS_NMEA0183(::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183_ptr _serial_Ext_GPS_NMEA0183)
{
    if (m_serial_Ext_GPS_NMEA0183)
        m_serial_Ext_GPS_NMEA0183->_setEContainer(Serial_Ext_GPS_NMEA0183Provider_ptr(),
                Serial_ext_gps_nmea0183Package::_instance()->getSerial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183());
    if (_serial_Ext_GPS_NMEA0183)
        _serial_Ext_GPS_NMEA0183->_setEContainer(_this(),
                Serial_ext_gps_nmea0183Package::_instance()->getSerial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183());
    m_serial_Ext_GPS_NMEA0183 = _serial_Ext_GPS_NMEA0183;
}

// Operations
void Serial_Ext_GPS_NMEA0183Provider::createMachines(std::istream & _stream)
{
    bool _newMachineCreated = false;
    if (!m_serial_Ext_GPS_NMEA0183)
    {
        setSerial_Ext_GPS_NMEA0183(::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183_ptr(new ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183()));
        _newMachineCreated = true;
    }
    auto _machineObj = ::ecore::as < ::ecore::EObject > (m_serial_Ext_GPS_NMEA0183);
    if (_machineObj)
    {
        constructChildTypes(::ecore::EJavaObject(), _machineObj, "Serial_Ext_GPS_NMEA0183");
        _machineObj->_initialize();
    }
    if (_newMachineCreated)
    {
        notifyPropertyChange("root", nullptr, m_serial_Ext_GPS_NMEA0183);
    }
    (void)_stream;
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__createMachines) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
    (void)_parent;
    if (!_parentObj) return;
    auto _topicObject = ::ecore::as < ::nevonex::common::TopicObject > (_parentObj);
    if (!_topicObject) return;
    ::ecore::EString _machinePath = _path;
    if (_machinePath.empty())
    {
        _machinePath = "Serial_Ext_GPS_NMEA0183";
    }
    m_topicObjectMap[_machinePath] = _topicObject;
    m_topicObjectMap["Serial_Ext_GPS_NMEA0183"] = _topicObject;
    m_topicObjectMap["serial_ext_gps_nmea0183"] = _topicObject;
    m_topicObjectMap["serial_ext_gps_nmea0183/0"] = _topicObject;
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__constructChildTypes) START*/
    /*PROTECTED REGION END*/
}

::nevonex::common::TopicObject_ptr Serial_Ext_GPS_NMEA0183Provider::getTopicElement(::ecore::EString const& _index)
{
    if (!m_serial_Ext_GPS_NMEA0183)
    {
        std::istringstream _emptyStream;
        createMachines(_emptyStream);
    }
    auto _it = m_topicObjectMap.find(_index);
    if (_it != m_topicObjectMap.end())
        return _it->second;
    if (boost::iequals(_index, "Serial_Ext_GPS_NMEA0183")
            || boost::iequals(_index, "serial_ext_gps_nmea0183")
            || boost::iequals(_index, "serial_ext_gps_nmea0183/0"))
    {
        return ::ecore::as < ::nevonex::common::TopicObject > (m_serial_Ext_GPS_NMEA0183);
    }
    return nullptr;
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__getTopicElement) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::initMachineProvider()
{
    ::nevonex::types::IMachineProvider::initMachineProvider();
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__initMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::stopMachineProvider()
{
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__stopMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::start()
{
    ::nevonex::types::IMachineProvider::start();
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__start) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::stop()
{
    ::nevonex::types::IMachineProvider::stop();
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__stop) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::restart()
{
    ::nevonex::types::IMachineProvider::restart();
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__restart) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp)
{
    if (_topic == "Serial_Ext_GPS_NMEA0183.Serial_Ext_GPS_NMEA0183_Data.sub" || _topic == "/9465")
    {
        if (!m_serial_Ext_GPS_NMEA0183)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_serial_Ext_GPS_NMEA0183->updateInterfaceDetail(9465, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0 && _jsonPlData[0].IsObject())
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject() && !(_item->HasMember("uTC_ms") || _item->HasMember("utc_ms") || _item->HasMember("UTC_MS") || _item->HasMember("latitude") || _item->HasMember("LATITUDE") || _item->HasMember("longitude") || _item->HasMember("LONGITUDE") || _item->HasMember("gPS_Quality_Indicator") || _item->HasMember("gps_quality_indicator") || _item->HasMember("GPS_QUALITY_INDICATOR") || _item->HasMember("no_Satellites") || _item->HasMember("no_satellites") || _item->HasMember("NO_SATELLITES") || _item->HasMember("altitude") || _item->HasMember("ALTITUDE") || _item->HasMember("geoidal_Separation") || _item->HasMember("geoidal_separation") || _item->HasMember("GEOIDAL_SEPARATION") || _item->HasMember("dGPS_Age") || _item->HasMember("dgps_age") || _item->HasMember("DGPS_AGE") || _item->HasMember("differential_Ref_Station") || _item->HasMember("differential_ref_station") || _item->HasMember("DIFFERENTIAL_REF_STATION") || _item->HasMember("selection_Mode") || _item->HasMember("selection_mode") || _item->HasMember("SELECTION_MODE") || _item->HasMember("fix_Type") || _item->HasMember("fix_type") || _item->HasMember("FIX_TYPE") || _item->HasMember("pDOP") || _item->HasMember("pdop") || _item->HasMember("PDOP") || _item->HasMember("hDOP") || _item->HasMember("hdop") || _item->HasMember("HDOP") || _item->HasMember("vDOP") || _item->HasMember("vdop") || _item->HasMember("VDOP") || _item->HasMember("status") || _item->HasMember("STATUS") || _item->HasMember("speed") || _item->HasMember("SPEED") || _item->HasMember("course") || _item->HasMember("COURSE") || _item->HasMember("magnetic_variation") || _item->HasMember("MAGNETIC_VARIATION") || _item->HasMember("fAA_Mode_Indicator") || _item->HasMember("faa_mode_indicator") || _item->HasMember("FAA_MODE_INDICATOR") || _item->HasMember("heading") || _item->HasMember("HEADING") || _item->HasMember("headingTrue") || _item->HasMember("headingtrue") || _item->HasMember("HEADINGTRUE")))
        {
            if (_item->HasMember("serial_Ext_GPS_NMEA0183_Data"))
            {
                _item = &(*_item)["serial_Ext_GPS_NMEA0183_Data"];
            }
            else if (_item->HasMember("Serial_Ext_GPS_NMEA0183_Data"))
            {
                _item = &(*_item)["Serial_Ext_GPS_NMEA0183_Data"];
            }
            else if (_item->HasMember("Serial_Ext_GPS_NMEA0183"))
            {
                _item = &(*_item)["Serial_Ext_GPS_NMEA0183"];
            }
            else if (_item->HasMember("serial_ext_gps_nmea0183"))
            {
                _item = &(*_item)["serial_ext_gps_nmea0183"];
            }
            else if (_item->HasMember("serial_ext_gps_nmea0183/0"))
            {
                _item = &(*_item)["serial_ext_gps_nmea0183/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("uTC_ms") || _item->HasMember("utc_ms") || _item->HasMember("UTC_MS") || _item->HasMember("latitude") || _item->HasMember("LATITUDE") || _item->HasMember("longitude") || _item->HasMember("LONGITUDE") || _item->HasMember("gPS_Quality_Indicator") || _item->HasMember("gps_quality_indicator") || _item->HasMember("GPS_QUALITY_INDICATOR") || _item->HasMember("no_Satellites") || _item->HasMember("no_satellites") || _item->HasMember("NO_SATELLITES") || _item->HasMember("altitude") || _item->HasMember("ALTITUDE") || _item->HasMember("geoidal_Separation") || _item->HasMember("geoidal_separation") || _item->HasMember("GEOIDAL_SEPARATION") || _item->HasMember("dGPS_Age") || _item->HasMember("dgps_age") || _item->HasMember("DGPS_AGE") || _item->HasMember("differential_Ref_Station") || _item->HasMember("differential_ref_station") || _item->HasMember("DIFFERENTIAL_REF_STATION") || _item->HasMember("selection_Mode") || _item->HasMember("selection_mode") || _item->HasMember("SELECTION_MODE") || _item->HasMember("fix_Type") || _item->HasMember("fix_type") || _item->HasMember("FIX_TYPE") || _item->HasMember("pDOP") || _item->HasMember("pdop") || _item->HasMember("PDOP") || _item->HasMember("hDOP") || _item->HasMember("hdop") || _item->HasMember("HDOP") || _item->HasMember("vDOP") || _item->HasMember("vdop") || _item->HasMember("VDOP") || _item->HasMember("status") || _item->HasMember("STATUS") || _item->HasMember("speed") || _item->HasMember("SPEED") || _item->HasMember("course") || _item->HasMember("COURSE") || _item->HasMember("magnetic_variation") || _item->HasMember("MAGNETIC_VARIATION") || _item->HasMember("fAA_Mode_Indicator") || _item->HasMember("faa_mode_indicator") || _item->HasMember("FAA_MODE_INDICATOR") || _item->HasMember("heading") || _item->HasMember("HEADING") || _item->HasMember("headingTrue") || _item->HasMember("headingtrue") || _item->HasMember("HEADINGTRUE")))
        {
            if (_item->HasMember("serial_Ext_GPS_NMEA0183_Data"))
            {
                _item = &(*_item)["serial_Ext_GPS_NMEA0183_Data"];
            }
            else if (_item->HasMember("Serial_Ext_GPS_NMEA0183_Data"))
            {
                _item = &(*_item)["Serial_Ext_GPS_NMEA0183_Data"];
            }
            else if (_item->HasMember("Serial_Ext_GPS_NMEA0183"))
            {
                _item = &(*_item)["Serial_Ext_GPS_NMEA0183"];
            }
            else if (_item->HasMember("serial_ext_gps_nmea0183"))
            {
                _item = &(*_item)["serial_ext_gps_nmea0183"];
            }
            else if (_item->HasMember("serial_ext_gps_nmea0183/0"))
            {
                _item = &(*_item)["serial_ext_gps_nmea0183/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::GPS_NMEA0183_T_R > (new ::nevonex::fcal::GPS_NMEA0183_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _uTC_msField = nullptr;
        if ((*_item).HasMember("uTC_ms"))
        {
            _uTC_msField = &(*_item)["uTC_ms"];
        }
        else if ((*_item).HasMember("utc_ms"))
        {
            _uTC_msField = &(*_item)["utc_ms"];
        }
        else if ((*_item).HasMember("UTC_MS"))
        {
            _uTC_msField = &(*_item)["UTC_MS"];
        }
        if (_uTC_msField && _uTC_msField->IsString())
        {
            _value->setUTC_ms(_uTC_msField->GetString());
        }
        const rapidjson::Value* _latitudeField = nullptr;
        if ((*_item).HasMember("latitude"))
        {
            _latitudeField = &(*_item)["latitude"];
        }
        else if ((*_item).HasMember("LATITUDE"))
        {
            _latitudeField = &(*_item)["LATITUDE"];
        }
        if (_latitudeField && _latitudeField->IsNumber())
        {
            _value->setLatitude(static_cast< ::ecore::EDouble >(_latitudeField->GetDouble()));
        }
        const rapidjson::Value* _longitudeField = nullptr;
        if ((*_item).HasMember("longitude"))
        {
            _longitudeField = &(*_item)["longitude"];
        }
        else if ((*_item).HasMember("LONGITUDE"))
        {
            _longitudeField = &(*_item)["LONGITUDE"];
        }
        if (_longitudeField && _longitudeField->IsNumber())
        {
            _value->setLongitude(static_cast< ::ecore::EDouble >(_longitudeField->GetDouble()));
        }
        const rapidjson::Value* _gPS_Quality_IndicatorField = nullptr;
        if ((*_item).HasMember("gPS_Quality_Indicator"))
        {
            _gPS_Quality_IndicatorField = &(*_item)["gPS_Quality_Indicator"];
        }
        else if ((*_item).HasMember("gps_quality_indicator"))
        {
            _gPS_Quality_IndicatorField = &(*_item)["gps_quality_indicator"];
        }
        else if ((*_item).HasMember("GPS_QUALITY_INDICATOR"))
        {
            _gPS_Quality_IndicatorField = &(*_item)["GPS_QUALITY_INDICATOR"];
        }
        if (_gPS_Quality_IndicatorField && _gPS_Quality_IndicatorField->IsNumber())
        {
            _value->setGPS_Quality_Indicator(static_cast< ::ecore::EInt >(_gPS_Quality_IndicatorField->GetDouble()));
        }
        const rapidjson::Value* _no_SatellitesField = nullptr;
        if ((*_item).HasMember("no_Satellites"))
        {
            _no_SatellitesField = &(*_item)["no_Satellites"];
        }
        else if ((*_item).HasMember("no_satellites"))
        {
            _no_SatellitesField = &(*_item)["no_satellites"];
        }
        else if ((*_item).HasMember("NO_SATELLITES"))
        {
            _no_SatellitesField = &(*_item)["NO_SATELLITES"];
        }
        if (_no_SatellitesField && _no_SatellitesField->IsNumber())
        {
            _value->setNo_Satellites(static_cast< ::ecore::EInt >(_no_SatellitesField->GetDouble()));
        }
        const rapidjson::Value* _altitudeField = nullptr;
        if ((*_item).HasMember("altitude"))
        {
            _altitudeField = &(*_item)["altitude"];
        }
        else if ((*_item).HasMember("ALTITUDE"))
        {
            _altitudeField = &(*_item)["ALTITUDE"];
        }
        if (_altitudeField && _altitudeField->IsNumber())
        {
            _value->setAltitude(static_cast< ::ecore::EFloat >(_altitudeField->GetFloat()));
        }
        const rapidjson::Value* _geoidal_SeparationField = nullptr;
        if ((*_item).HasMember("geoidal_Separation"))
        {
            _geoidal_SeparationField = &(*_item)["geoidal_Separation"];
        }
        else if ((*_item).HasMember("geoidal_separation"))
        {
            _geoidal_SeparationField = &(*_item)["geoidal_separation"];
        }
        else if ((*_item).HasMember("GEOIDAL_SEPARATION"))
        {
            _geoidal_SeparationField = &(*_item)["GEOIDAL_SEPARATION"];
        }
        if (_geoidal_SeparationField && _geoidal_SeparationField->IsNumber())
        {
            _value->setGeoidal_Separation(static_cast< ::ecore::EFloat >(_geoidal_SeparationField->GetFloat()));
        }
        const rapidjson::Value* _dGPS_AgeField = nullptr;
        if ((*_item).HasMember("dGPS_Age"))
        {
            _dGPS_AgeField = &(*_item)["dGPS_Age"];
        }
        else if ((*_item).HasMember("dgps_age"))
        {
            _dGPS_AgeField = &(*_item)["dgps_age"];
        }
        else if ((*_item).HasMember("DGPS_AGE"))
        {
            _dGPS_AgeField = &(*_item)["DGPS_AGE"];
        }
        if (_dGPS_AgeField && _dGPS_AgeField->IsNumber())
        {
            _value->setDGPS_Age(static_cast< ::ecore::EFloat >(_dGPS_AgeField->GetFloat()));
        }
        const rapidjson::Value* _differential_Ref_StationField = nullptr;
        if ((*_item).HasMember("differential_Ref_Station"))
        {
            _differential_Ref_StationField = &(*_item)["differential_Ref_Station"];
        }
        else if ((*_item).HasMember("differential_ref_station"))
        {
            _differential_Ref_StationField = &(*_item)["differential_ref_station"];
        }
        else if ((*_item).HasMember("DIFFERENTIAL_REF_STATION"))
        {
            _differential_Ref_StationField = &(*_item)["DIFFERENTIAL_REF_STATION"];
        }
        if (_differential_Ref_StationField && _differential_Ref_StationField->IsNumber())
        {
            _value->setDifferential_Ref_Station(static_cast< ::ecore::EInt >(_differential_Ref_StationField->GetDouble()));
        }
        const rapidjson::Value* _selection_ModeField = nullptr;
        if ((*_item).HasMember("selection_Mode"))
        {
            _selection_ModeField = &(*_item)["selection_Mode"];
        }
        else if ((*_item).HasMember("selection_mode"))
        {
            _selection_ModeField = &(*_item)["selection_mode"];
        }
        else if ((*_item).HasMember("SELECTION_MODE"))
        {
            _selection_ModeField = &(*_item)["SELECTION_MODE"];
        }
        if (_selection_ModeField && _selection_ModeField->IsString())
        {
            _value->setSelection_Mode(_selection_ModeField->GetString());
        }
        const rapidjson::Value* _fix_TypeField = nullptr;
        if ((*_item).HasMember("fix_Type"))
        {
            _fix_TypeField = &(*_item)["fix_Type"];
        }
        else if ((*_item).HasMember("fix_type"))
        {
            _fix_TypeField = &(*_item)["fix_type"];
        }
        else if ((*_item).HasMember("FIX_TYPE"))
        {
            _fix_TypeField = &(*_item)["FIX_TYPE"];
        }
        if (_fix_TypeField && _fix_TypeField->IsNumber())
        {
            _value->setFix_Type(static_cast< ::ecore::EInt >(_fix_TypeField->GetDouble()));
        }
        const rapidjson::Value* _pDOPField = nullptr;
        if ((*_item).HasMember("pDOP"))
        {
            _pDOPField = &(*_item)["pDOP"];
        }
        else if ((*_item).HasMember("pdop"))
        {
            _pDOPField = &(*_item)["pdop"];
        }
        else if ((*_item).HasMember("PDOP"))
        {
            _pDOPField = &(*_item)["PDOP"];
        }
        if (_pDOPField && _pDOPField->IsNumber())
        {
            _value->setPDOP(static_cast< ::ecore::EFloat >(_pDOPField->GetFloat()));
        }
        const rapidjson::Value* _hDOPField = nullptr;
        if ((*_item).HasMember("hDOP"))
        {
            _hDOPField = &(*_item)["hDOP"];
        }
        else if ((*_item).HasMember("hdop"))
        {
            _hDOPField = &(*_item)["hdop"];
        }
        else if ((*_item).HasMember("HDOP"))
        {
            _hDOPField = &(*_item)["HDOP"];
        }
        if (_hDOPField && _hDOPField->IsNumber())
        {
            _value->setHDOP(static_cast< ::ecore::EFloat >(_hDOPField->GetFloat()));
        }
        const rapidjson::Value* _vDOPField = nullptr;
        if ((*_item).HasMember("vDOP"))
        {
            _vDOPField = &(*_item)["vDOP"];
        }
        else if ((*_item).HasMember("vdop"))
        {
            _vDOPField = &(*_item)["vdop"];
        }
        else if ((*_item).HasMember("VDOP"))
        {
            _vDOPField = &(*_item)["VDOP"];
        }
        if (_vDOPField && _vDOPField->IsNumber())
        {
            _value->setVDOP(static_cast< ::ecore::EFloat >(_vDOPField->GetFloat()));
        }
        const rapidjson::Value* _statusField = nullptr;
        if ((*_item).HasMember("status"))
        {
            _statusField = &(*_item)["status"];
        }
        else if ((*_item).HasMember("STATUS"))
        {
            _statusField = &(*_item)["STATUS"];
        }
        if (_statusField && _statusField->IsNumber())
        {
            _value->setStatus(static_cast< ::ecore::EInt >(_statusField->GetDouble()));
        }
        const rapidjson::Value* _speedField = nullptr;
        if ((*_item).HasMember("speed"))
        {
            _speedField = &(*_item)["speed"];
        }
        else if ((*_item).HasMember("SPEED"))
        {
            _speedField = &(*_item)["SPEED"];
        }
        if (_speedField && _speedField->IsNumber())
        {
            _value->setSpeed(static_cast< ::ecore::EFloat >(_speedField->GetFloat()));
        }
        const rapidjson::Value* _courseField = nullptr;
        if ((*_item).HasMember("course"))
        {
            _courseField = &(*_item)["course"];
        }
        else if ((*_item).HasMember("COURSE"))
        {
            _courseField = &(*_item)["COURSE"];
        }
        if (_courseField && _courseField->IsNumber())
        {
            _value->setCourse(static_cast< ::ecore::EFloat >(_courseField->GetFloat()));
        }
        const rapidjson::Value* _magnetic_variationField = nullptr;
        if ((*_item).HasMember("magnetic_variation"))
        {
            _magnetic_variationField = &(*_item)["magnetic_variation"];
        }
        else if ((*_item).HasMember("MAGNETIC_VARIATION"))
        {
            _magnetic_variationField = &(*_item)["MAGNETIC_VARIATION"];
        }
        if (_magnetic_variationField && _magnetic_variationField->IsNumber())
        {
            _value->setMagnetic_variation(static_cast< ::ecore::EFloat >(_magnetic_variationField->GetFloat()));
        }
        const rapidjson::Value* _fAA_Mode_IndicatorField = nullptr;
        if ((*_item).HasMember("fAA_Mode_Indicator"))
        {
            _fAA_Mode_IndicatorField = &(*_item)["fAA_Mode_Indicator"];
        }
        else if ((*_item).HasMember("faa_mode_indicator"))
        {
            _fAA_Mode_IndicatorField = &(*_item)["faa_mode_indicator"];
        }
        else if ((*_item).HasMember("FAA_MODE_INDICATOR"))
        {
            _fAA_Mode_IndicatorField = &(*_item)["FAA_MODE_INDICATOR"];
        }
        if (_fAA_Mode_IndicatorField && _fAA_Mode_IndicatorField->IsString())
        {
            _value->setFAA_Mode_Indicator(_fAA_Mode_IndicatorField->GetString());
        }
        const rapidjson::Value* _headingField = nullptr;
        if ((*_item).HasMember("heading"))
        {
            _headingField = &(*_item)["heading"];
        }
        else if ((*_item).HasMember("HEADING"))
        {
            _headingField = &(*_item)["HEADING"];
        }
        if (_headingField && _headingField->IsNumber())
        {
            _value->setHeading(static_cast< ::ecore::EFloat >(_headingField->GetFloat()));
        }
        const rapidjson::Value* _headingTrueField = nullptr;
        if ((*_item).HasMember("headingTrue"))
        {
            _headingTrueField = &(*_item)["headingTrue"];
        }
        else if ((*_item).HasMember("headingtrue"))
        {
            _headingTrueField = &(*_item)["headingtrue"];
        }
        else if ((*_item).HasMember("HEADINGTRUE"))
        {
            _headingTrueField = &(*_item)["HEADINGTRUE"];
        }
        if (_headingTrueField && _headingTrueField->IsNumber())
        {
            _value->setHeadingTrue(static_cast< ::ecore::EInt >(_headingTrueField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsString())
            {
                _value->setUTC_ms((*_item)[0].GetString());
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setLatitude(static_cast< ::ecore::EDouble >((*_item)[1].GetDouble()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setLongitude(static_cast< ::ecore::EDouble >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setGPS_Quality_Indicator(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setNo_Satellites(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
            if ((*_item).Size() > 5 && (*_item)[5].IsNumber())
            {
                _value->setAltitude(static_cast< ::ecore::EFloat >((*_item)[5].GetFloat()));
            }
            if ((*_item).Size() > 6 && (*_item)[6].IsNumber())
            {
                _value->setGeoidal_Separation(static_cast< ::ecore::EFloat >((*_item)[6].GetFloat()));
            }
            if ((*_item).Size() > 7 && (*_item)[7].IsNumber())
            {
                _value->setDGPS_Age(static_cast< ::ecore::EFloat >((*_item)[7].GetFloat()));
            }
            if ((*_item).Size() > 8 && (*_item)[8].IsNumber())
            {
                _value->setDifferential_Ref_Station(static_cast< ::ecore::EInt >((*_item)[8].GetDouble()));
            }
            if ((*_item).Size() > 9 && (*_item)[9].IsString())
            {
                _value->setSelection_Mode((*_item)[9].GetString());
            }
            if ((*_item).Size() > 10 && (*_item)[10].IsNumber())
            {
                _value->setFix_Type(static_cast< ::ecore::EInt >((*_item)[10].GetDouble()));
            }
            if ((*_item).Size() > 11 && (*_item)[11].IsNumber())
            {
                _value->setPDOP(static_cast< ::ecore::EFloat >((*_item)[11].GetFloat()));
            }
            if ((*_item).Size() > 12 && (*_item)[12].IsNumber())
            {
                _value->setHDOP(static_cast< ::ecore::EFloat >((*_item)[12].GetFloat()));
            }
            if ((*_item).Size() > 13 && (*_item)[13].IsNumber())
            {
                _value->setVDOP(static_cast< ::ecore::EFloat >((*_item)[13].GetFloat()));
            }
            if ((*_item).Size() > 14 && (*_item)[14].IsNumber())
            {
                _value->setStatus(static_cast< ::ecore::EInt >((*_item)[14].GetDouble()));
            }
            if ((*_item).Size() > 15 && (*_item)[15].IsNumber())
            {
                _value->setSpeed(static_cast< ::ecore::EFloat >((*_item)[15].GetFloat()));
            }
            if ((*_item).Size() > 16 && (*_item)[16].IsNumber())
            {
                _value->setCourse(static_cast< ::ecore::EFloat >((*_item)[16].GetFloat()));
            }
            if ((*_item).Size() > 17 && (*_item)[17].IsNumber())
            {
                _value->setMagnetic_variation(static_cast< ::ecore::EFloat >((*_item)[17].GetFloat()));
            }
            if ((*_item).Size() > 18 && (*_item)[18].IsString())
            {
                _value->setFAA_Mode_Indicator((*_item)[18].GetString());
            }
            if ((*_item).Size() > 19 && (*_item)[19].IsNumber())
            {
                _value->setHeading(static_cast< ::ecore::EFloat >((*_item)[19].GetFloat()));
            }
            if ((*_item).Size() > 20 && (*_item)[20].IsNumber())
            {
                _value->setHeadingTrue(static_cast< ::ecore::EInt >((*_item)[20].GetDouble()));
            }
        }
        m_serial_Ext_GPS_NMEA0183->setSerial_Ext_GPS_NMEA0183_Data(_value);
        return;
    }

    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__processMessage) START*/
    /*PROTECTED REGION END*/
}

void Serial_Ext_GPS_NMEA0183Provider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
    /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider__onConnectionStatusChange) START*/
    /*PROTECTED REGION END*/
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void Serial_Ext_GPS_NMEA0183Provider::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void Serial_Ext_GPS_NMEA0183Provider::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void Serial_Ext_GPS_NMEA0183Provider::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
