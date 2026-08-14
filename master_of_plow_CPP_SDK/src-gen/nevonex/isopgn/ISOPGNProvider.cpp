/*
 * nevonex/isopgn/ISOPGNProvider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ISOPGNProvider.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/isopgn/IsopgnPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <sstream>

/*PROTECTED REGION ID(ISOPGNProvider.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::isopgn;
using namespace ::nevonex::log;

ISOPGNProvider::ISOPGNProvider() : m_iSOPGN(0)
{
    _initialize();

    /*PROTECTED REGION ID(ISOPGNProvider__ISOPGNProvider) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ISOPGNProvider::~ISOPGNProvider()
{
    m_iSOPGN.reset();
}

// References
::nevonex::isopgn::ISOPGN_ptr ISOPGNProvider::getISOPGN() const
{
    return m_iSOPGN;
}

void ISOPGNProvider::setISOPGN(::nevonex::isopgn::ISOPGN_ptr _iSOPGN)
{
    if (m_iSOPGN)
        m_iSOPGN->_setEContainer(ISOPGNProvider_ptr(),
                IsopgnPackage::_instance()->getISOPGNProvider__iSOPGN());
    if (_iSOPGN)
        _iSOPGN->_setEContainer(_this(),
                IsopgnPackage::_instance()->getISOPGNProvider__iSOPGN());
    m_iSOPGN = _iSOPGN;
}

// Operations
void ISOPGNProvider::createMachines(std::istream & _stream)
{
    bool _newMachineCreated = false;
    if (!m_iSOPGN)
    {
        setISOPGN(::nevonex::isopgn::ISOPGN_ptr(new ::nevonex::isopgn::ISOPGN()));
        _newMachineCreated = true;
    }
    auto _machineObj = ::ecore::as < ::ecore::EObject > (m_iSOPGN);
    if (_machineObj)
    {
        constructChildTypes(::ecore::EJavaObject(), _machineObj, "ISOPGN");
        _machineObj->_initialize();
    }
    if (_newMachineCreated)
    {
        notifyPropertyChange("root", nullptr, m_iSOPGN);
    }
    (void)_stream;
    /*PROTECTED REGION ID(ISOPGNProvider__createMachines) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
    (void)_parent;
    if (!_parentObj) return;
    auto _topicObject = ::ecore::as < ::nevonex::common::TopicObject > (_parentObj);
    if (!_topicObject) return;
    ::ecore::EString _machinePath = _path;
    if (_machinePath.empty())
    {
        _machinePath = "ISOPGN";
    }
    m_topicObjectMap[_machinePath] = _topicObject;
    m_topicObjectMap["ISOPGN"] = _topicObject;
    m_topicObjectMap["isopgn"] = _topicObject;
    m_topicObjectMap["isopgn/0"] = _topicObject;
    /*PROTECTED REGION ID(ISOPGNProvider__constructChildTypes) START*/
    /*PROTECTED REGION END*/
}

::nevonex::common::TopicObject_ptr ISOPGNProvider::getTopicElement(::ecore::EString const& _index)
{
    if (!m_iSOPGN)
    {
        std::istringstream _emptyStream;
        createMachines(_emptyStream);
    }
    auto _it = m_topicObjectMap.find(_index);
    if (_it != m_topicObjectMap.end())
        return _it->second;
    if (boost::iequals(_index, "ISOPGN")
            || boost::iequals(_index, "isopgn")
            || boost::iequals(_index, "isopgn/0"))
    {
        return ::ecore::as < ::nevonex::common::TopicObject > (m_iSOPGN);
    }
    return nullptr;
    /*PROTECTED REGION ID(ISOPGNProvider__getTopicElement) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::initMachineProvider()
{
    ::nevonex::types::IMachineProvider::initMachineProvider();
    /*PROTECTED REGION ID(ISOPGNProvider__initMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::stopMachineProvider()
{
    /*PROTECTED REGION ID(ISOPGNProvider__stopMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::start()
{
    ::nevonex::types::IMachineProvider::start();
    /*PROTECTED REGION ID(ISOPGNProvider__start) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::stop()
{
    ::nevonex::types::IMachineProvider::stop();
    /*PROTECTED REGION ID(ISOPGNProvider__stop) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::restart()
{
    ::nevonex::types::IMachineProvider::restart();
    /*PROTECTED REGION ID(ISOPGNProvider__restart) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp)
{
    if (_topic == "ISOPGN.HitchPosPerRe.sub" || _topic == "/251")
    {
        if (!m_iSOPGN)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_iSOPGN->updateInterfaceDetail(251, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0)
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("hitchPosPerRe"))
            {
                _item = &(*_item)["hitchPosPerRe"];
            }
            else if (_item->HasMember("HitchPosPerRe"))
            {
                _item = &(*_item)["HitchPosPerRe"];
            }
            else if (_item->HasMember("ISOPGN"))
            {
                _item = &(*_item)["ISOPGN"];
            }
            else if (_item->HasMember("isopgn"))
            {
                _item = &(*_item)["isopgn"];
            }
            else if (_item->HasMember("isopgn/0"))
            {
                _item = &(*_item)["isopgn/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject())
        {
            if (_item->HasMember("hitchPosPerRe"))
            {
                _item = &(*_item)["hitchPosPerRe"];
            }
            else if (_item->HasMember("HitchPosPerRe"))
            {
                _item = &(*_item)["HitchPosPerRe"];
            }
            else if (_item->HasMember("ISOPGN"))
            {
                _item = &(*_item)["ISOPGN"];
            }
            else if (_item->HasMember("isopgn"))
            {
                _item = &(*_item)["isopgn"];
            }
            else if (_item->HasMember("isopgn/0"))
            {
                _item = &(*_item)["isopgn/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsArray() && _item->Size() > 0)
        {
            _item = &(*_item)[0];
        }
        if (_item->IsNumber())
        {
            m_iSOPGN->setHitchPosPerRe(static_cast< ::ecore::EFloat >(_item->GetFloat()));
        }
        return;
    }

    /*PROTECTED REGION ID(ISOPGNProvider__processMessage) START*/
    /*PROTECTED REGION END*/
}

void ISOPGNProvider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
    /*PROTECTED REGION ID(ISOPGNProvider__onConnectionStatusChange) START*/
    /*PROTECTED REGION END*/
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void ISOPGNProvider::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void ISOPGNProvider::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void ISOPGNProvider::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
