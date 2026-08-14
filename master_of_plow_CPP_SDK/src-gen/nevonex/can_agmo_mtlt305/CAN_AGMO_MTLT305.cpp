/*
 * nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_MTLT305.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_MTLT305.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_mtlt305;
using namespace ::nevonex::log;

// Default constructor
CAN_AGMO_MTLT305::CAN_AGMO_MTLT305() : m_aceinna_Accel(0), m_aceinna_AngleRate(0), m_aceinna_Angles(0)
{
    // Machine does NOT call _initialize() — Provider manages initialization

    m_interfaceDetailMap =
    {
    { 9440, ::nevonex::types::InterfaceDetails("fek/9440") },
    { 9441, ::nevonex::types::InterfaceDetails("fek/9441") },
    { 9442, ::nevonex::types::InterfaceDetails("fek/9442") } };

    /*PROTECTED REGION ID(CAN_AGMO_MTLT305__CAN_AGMO_MTLT305) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_MTLT305::~CAN_AGMO_MTLT305()
{
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// References
::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr CAN_AGMO_MTLT305::getAceinna_Accel() const
{
    return m_aceinna_Accel;
}

void CAN_AGMO_MTLT305::setAceinna_Accel(::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr _aceinna_Accel)
{
    ::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr _old_aceinna_Accel = m_aceinna_Accel;
    m_aceinna_Accel = _aceinna_Accel;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_MTLT305 property \"aceinna_Accel\" has changed.";
    notifyPropertyChange("aceinna_Accel", _old_aceinna_Accel, m_aceinna_Accel);
}

::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr CAN_AGMO_MTLT305::getAceinna_AngleRate() const
{
    return m_aceinna_AngleRate;
}

void CAN_AGMO_MTLT305::setAceinna_AngleRate(::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr _aceinna_AngleRate)
{
    ::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr _old_aceinna_AngleRate = m_aceinna_AngleRate;
    m_aceinna_AngleRate = _aceinna_AngleRate;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_MTLT305 property \"aceinna_AngleRate\" has changed.";
    notifyPropertyChange("aceinna_AngleRate", _old_aceinna_AngleRate, m_aceinna_AngleRate);
}

::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr CAN_AGMO_MTLT305::getAceinna_Angles() const
{
    return m_aceinna_Angles;
}

void CAN_AGMO_MTLT305::setAceinna_Angles(::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr _aceinna_Angles)
{
    ::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr _old_aceinna_Angles = m_aceinna_Angles;
    m_aceinna_Angles = _aceinna_Angles;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_MTLT305 property \"aceinna_Angles\" has changed.";
    notifyPropertyChange("aceinna_Angles", _old_aceinna_Angles, m_aceinna_Angles);
}

// FCAL timestamp/valid
::ecore::ELong CAN_AGMO_MTLT305::getAceinna_Accel_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9440);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_MTLT305::isAceinna_Accel_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9440);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_MTLT305::getAceinna_AngleRate_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9441);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_MTLT305::isAceinna_AngleRate_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9441);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_MTLT305::getAceinna_Angles_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9442);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_MTLT305::isAceinna_Angles_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9442);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::EBoolean CAN_AGMO_MTLT305::updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp)
{
    auto _detail = m_interfaceDetailMap.find(_topicKey);
    if (_detail == m_interfaceDetailMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::warning) << _topicKey
                << " interface details are not present in the interface detail map[CAN_AGMO_MTLT305].";
        return false;
    }

    if (_detail->second.getTimestamp() > _timestamp)
    {
        SDK_FCAL_LOG(SeverityLevel::warning)
                << "Old or duplicate message received. Old TS : "
                << _detail->second.getTimestamp() << " new TS : " << _timestamp
                << " feature: CAN_AGMO_MTLT305 topic:" << _topicKey;
        return false;
    }

    _detail->second.setValid(_valid, _timestamp);
    return true;
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_MTLT305::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_MTLT305::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_MTLT305::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
