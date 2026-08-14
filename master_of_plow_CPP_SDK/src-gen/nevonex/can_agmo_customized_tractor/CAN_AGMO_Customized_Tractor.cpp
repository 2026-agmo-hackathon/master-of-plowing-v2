/*
 * nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_Tractor.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_Customized_Tractor.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <nevonex/fcb/PublishConnectionFactory.hpp>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <vector>

/*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_customized_tractor;
using namespace ::nevonex::log;

// Default constructor
CAN_AGMO_Customized_Tractor::CAN_AGMO_Customized_Tractor() : m_receive_ACC_INFO(0), m_receive_FNR_INFO(0), m_receive_HYD_INFO(0), m_receive_SFT_INFO(0), m_send_ACC_CMD(0), m_send_FNR_CMD(0), m_send_HYD_CMD(0), m_send_SFT_CMD(0)
{
    // Machine does NOT call _initialize() — Provider manages initialization

    m_interfaceDetailMap =
    {
    { 9455, ::nevonex::types::InterfaceDetails("fek/9455") },
    { 9456, ::nevonex::types::InterfaceDetails("fek/9456") },
    { 9457, ::nevonex::types::InterfaceDetails("fek/9457") },
    { 9459, ::nevonex::types::InterfaceDetails("fek/9459") } };

    /*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor__CAN_AGMO_Customized_Tractor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_Customized_Tractor::~CAN_AGMO_Customized_Tractor()
{
    /*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// References
::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr CAN_AGMO_Customized_Tractor::getReceive_ACC_INFO() const
{
    return m_receive_ACC_INFO;
}

void CAN_AGMO_Customized_Tractor::setReceive_ACC_INFO(::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr _receive_ACC_INFO)
{
    ::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr _old_receive_ACC_INFO = m_receive_ACC_INFO;
    m_receive_ACC_INFO = _receive_ACC_INFO;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"receive_ACC_INFO\" has changed.";
    notifyPropertyChange("receive_ACC_INFO", _old_receive_ACC_INFO, m_receive_ACC_INFO);
}

::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr CAN_AGMO_Customized_Tractor::getReceive_FNR_INFO() const
{
    return m_receive_FNR_INFO;
}

void CAN_AGMO_Customized_Tractor::setReceive_FNR_INFO(::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr _receive_FNR_INFO)
{
    ::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr _old_receive_FNR_INFO = m_receive_FNR_INFO;
    m_receive_FNR_INFO = _receive_FNR_INFO;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"receive_FNR_INFO\" has changed.";
    notifyPropertyChange("receive_FNR_INFO", _old_receive_FNR_INFO, m_receive_FNR_INFO);
}

::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr CAN_AGMO_Customized_Tractor::getReceive_HYD_INFO() const
{
    return m_receive_HYD_INFO;
}

void CAN_AGMO_Customized_Tractor::setReceive_HYD_INFO(::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr _receive_HYD_INFO)
{
    ::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr _old_receive_HYD_INFO = m_receive_HYD_INFO;
    m_receive_HYD_INFO = _receive_HYD_INFO;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"receive_HYD_INFO\" has changed.";
    notifyPropertyChange("receive_HYD_INFO", _old_receive_HYD_INFO, m_receive_HYD_INFO);
}

::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr CAN_AGMO_Customized_Tractor::getReceive_SFT_INFO() const
{
    return m_receive_SFT_INFO;
}

void CAN_AGMO_Customized_Tractor::setReceive_SFT_INFO(::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr _receive_SFT_INFO)
{
    ::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr _old_receive_SFT_INFO = m_receive_SFT_INFO;
    m_receive_SFT_INFO = _receive_SFT_INFO;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"receive_SFT_INFO\" has changed.";
    notifyPropertyChange("receive_SFT_INFO", _old_receive_SFT_INFO, m_receive_SFT_INFO);
}

::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr CAN_AGMO_Customized_Tractor::getSend_ACC_CMD() const
{
    return m_send_ACC_CMD;
}

void CAN_AGMO_Customized_Tractor::setSend_ACC_CMD(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_ACC_CMD)
{
    setSend_ACC_CMDInternal(_send_ACC_CMD);

    // Publish the value to FIL layer
    std::vector< ::ecore::EObject_ptr > _publishPayload;
    _publishPayload.push_back(::ecore::as< ::ecore::EObject >(_this()));
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
            "CAN_AGMO_Customized_Tractor.send_ACC_CMD.pub", "send_ACC_CMD",
            _publishPayload, ::nevonex::fcb::SubscriberEnum::FIL);
}

void CAN_AGMO_Customized_Tractor::setSend_ACC_CMDInternal(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_ACC_CMD)
{
    ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _old_send_ACC_CMD = m_send_ACC_CMD;
    m_send_ACC_CMD = _send_ACC_CMD;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"send_ACC_CMD\" has changed.";
    notifyPropertyChange("send_ACC_CMD", _old_send_ACC_CMD, m_send_ACC_CMD);
}

::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr CAN_AGMO_Customized_Tractor::getSend_FNR_CMD() const
{
    return m_send_FNR_CMD;
}

void CAN_AGMO_Customized_Tractor::setSend_FNR_CMD(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_FNR_CMD)
{
    setSend_FNR_CMDInternal(_send_FNR_CMD);

    // Publish the value to FIL layer
    std::vector< ::ecore::EObject_ptr > _publishPayload;
    _publishPayload.push_back(::ecore::as< ::ecore::EObject >(_this()));
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
            "CAN_AGMO_Customized_Tractor.send_FNR_CMD.pub", "send_FNR_CMD",
            _publishPayload, ::nevonex::fcb::SubscriberEnum::FIL);
}

void CAN_AGMO_Customized_Tractor::setSend_FNR_CMDInternal(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_FNR_CMD)
{
    ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _old_send_FNR_CMD = m_send_FNR_CMD;
    m_send_FNR_CMD = _send_FNR_CMD;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"send_FNR_CMD\" has changed.";
    notifyPropertyChange("send_FNR_CMD", _old_send_FNR_CMD, m_send_FNR_CMD);
}

::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr CAN_AGMO_Customized_Tractor::getSend_HYD_CMD() const
{
    return m_send_HYD_CMD;
}

void CAN_AGMO_Customized_Tractor::setSend_HYD_CMD(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_HYD_CMD)
{
    setSend_HYD_CMDInternal(_send_HYD_CMD);

    // Publish the value to FIL layer
    std::vector< ::ecore::EObject_ptr > _publishPayload;
    _publishPayload.push_back(::ecore::as< ::ecore::EObject >(_this()));
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
            "CAN_AGMO_Customized_Tractor.send_HYD_CMD.pub", "send_HYD_CMD",
            _publishPayload, ::nevonex::fcb::SubscriberEnum::FIL);
}

void CAN_AGMO_Customized_Tractor::setSend_HYD_CMDInternal(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_HYD_CMD)
{
    ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _old_send_HYD_CMD = m_send_HYD_CMD;
    m_send_HYD_CMD = _send_HYD_CMD;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"send_HYD_CMD\" has changed.";
    notifyPropertyChange("send_HYD_CMD", _old_send_HYD_CMD, m_send_HYD_CMD);
}

::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr CAN_AGMO_Customized_Tractor::getSend_SFT_CMD() const
{
    return m_send_SFT_CMD;
}

void CAN_AGMO_Customized_Tractor::setSend_SFT_CMD(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_SFT_CMD)
{
    setSend_SFT_CMDInternal(_send_SFT_CMD);

    // Publish the value to FIL layer
    std::vector< ::ecore::EObject_ptr > _publishPayload;
    _publishPayload.push_back(::ecore::as< ::ecore::EObject >(_this()));
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
            "CAN_AGMO_Customized_Tractor.send_SFT_CMD.pub", "send_SFT_CMD",
            _publishPayload, ::nevonex::fcb::SubscriberEnum::FIL);
}

void CAN_AGMO_Customized_Tractor::setSend_SFT_CMDInternal(::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_SFT_CMD)
{
    ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _old_send_SFT_CMD = m_send_SFT_CMD;
    m_send_SFT_CMD = _send_SFT_CMD;
    SDK_FCAL_LOG(SeverityLevel::debug) << "CAN_AGMO_Customized_Tractor property \"send_SFT_CMD\" has changed.";
    notifyPropertyChange("send_SFT_CMD", _old_send_SFT_CMD, m_send_SFT_CMD);
}

// FCAL timestamp/valid
::ecore::ELong CAN_AGMO_Customized_Tractor::getReceive_ACC_INFO_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9455);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::isReceive_ACC_INFO_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9455);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_Customized_Tractor::getReceive_FNR_INFO_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9456);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::isReceive_FNR_INFO_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9456);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_Customized_Tractor::getReceive_HYD_INFO_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9457);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::isReceive_HYD_INFO_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9457);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

::ecore::ELong CAN_AGMO_Customized_Tractor::getReceive_SFT_INFO_Timestamp()
{
    auto _detail = m_interfaceDetailMap.find(9459);
    if (_detail == m_interfaceDetailMap.end())
        return -1;
    return _detail->second.getTimestamp();
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::isReceive_SFT_INFO_Valid()
{
    auto _detail = m_interfaceDetailMap.find(9459);
    if (_detail == m_interfaceDetailMap.end())
        return false;
    return _detail->second.isValid();
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::ELong CAN_AGMO_Customized_Tractor::getSend_ACC_CMD_Timestamp()
{
    return -1;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::EBoolean CAN_AGMO_Customized_Tractor::isSend_ACC_CMD_Valid()
{
    return false;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::ELong CAN_AGMO_Customized_Tractor::getSend_FNR_CMD_Timestamp()
{
    return -1;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::EBoolean CAN_AGMO_Customized_Tractor::isSend_FNR_CMD_Valid()
{
    return false;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::ELong CAN_AGMO_Customized_Tractor::getSend_HYD_CMD_Timestamp()
{
    return -1;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::EBoolean CAN_AGMO_Customized_Tractor::isSend_HYD_CMD_Valid()
{
    return false;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::ELong CAN_AGMO_Customized_Tractor::getSend_SFT_CMD_Timestamp()
{
    return -1;
}

// FD 미생성 API, 호환용 — Publish 전용 인터페이스에는 유효성/타임스탬프가 없다.
::ecore::EBoolean CAN_AGMO_Customized_Tractor::isSend_SFT_CMD_Valid()
{
    return false;
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp)
{
    auto _detail = m_interfaceDetailMap.find(_topicKey);
    if (_detail == m_interfaceDetailMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::warning) << _topicKey
                << " interface details are not present in the interface detail map[CAN_AGMO_Customized_Tractor].";
        return false;
    }

    if (_detail->second.getTimestamp() > _timestamp)
    {
        SDK_FCAL_LOG(SeverityLevel::warning)
                << "Old or duplicate message received. Old TS : "
                << _detail->second.getTimestamp() << " new TS : " << _timestamp
                << " feature: CAN_AGMO_Customized_Tractor topic:" << _topicKey;
        return false;
    }

    _detail->second.setValid(_valid, _timestamp);
    return true;
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_Customized_Tractor::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_Customized_Tractor::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_Customized_Tractor::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
