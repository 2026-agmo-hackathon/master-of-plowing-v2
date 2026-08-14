/*
 * nevonex/isopgn/ISOPGN.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ISOPGN.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/isopgn/IsopgnPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ISOPGN.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::isopgn;
using namespace ::nevonex::log;

// Default constructor
ISOPGN::ISOPGN() : m_hitchPosPerRe(0)
{
    // Machine does NOT call _initialize() — Provider manages initialization

    m_interfaceDetailMap =
    {
    { 251, ::nevonex::types::InterfaceDetails("fek/251") } };

    /*PROTECTED REGION ID(ISOPGN__ISOPGN) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ISOPGN::~ISOPGN()
{
    /*PROTECTED REGION ID(ISOPGN__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// References
::ecore::EFloat ISOPGN::getHitchPosPerRe() const
{
    return m_hitchPosPerRe;
}

void ISOPGN::setHitchPosPerRe(::ecore::EFloat _hitchPosPerRe)
{
    ::ecore::EFloat _old_hitchPosPerRe = m_hitchPosPerRe;
    m_hitchPosPerRe = _hitchPosPerRe;
    SDK_FCAL_LOG(SeverityLevel::debug) << "ISOPGN property \"hitchPosPerRe\" has changed.";
    notifyPropertyChange("hitchPosPerRe", _old_hitchPosPerRe, m_hitchPosPerRe);
}

// FCAL timestamp/valid
::ecore::ELong ISOPGN::getHitchPosPerRe_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(251);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean ISOPGN::isHitchPosPerRe_Valid()
{
    auto _detail = m_interfaceDetailMap.find(251);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::EBoolean ISOPGN::updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp)
{
    auto _detail = m_interfaceDetailMap.find(_topicKey);
    if (_detail == m_interfaceDetailMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::warning) << _topicKey
                << " interface details are not present in the interface detail map[ISOPGN].";
        return false;
    }

    if (_detail->second.getTimestamp() > _timestamp)
    {
        SDK_FCAL_LOG(SeverityLevel::warning)
                << "Old or duplicate message received. Old TS : "
                << _detail->second.getTimestamp() << " new TS : " << _timestamp
                << " feature: ISOPGN topic:" << _topicKey;
        return false;
    }

    _detail->second.setValid(_valid, _timestamp);
    return true;
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void ISOPGN::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void ISOPGN::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void ISOPGN::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
