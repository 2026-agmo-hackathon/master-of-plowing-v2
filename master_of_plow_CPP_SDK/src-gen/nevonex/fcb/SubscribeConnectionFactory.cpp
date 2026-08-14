/*
 * nevonex/fcb/SubscribeConnectionFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "SubscribeConnectionFactory.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcb/FcbPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <nevonex-fcal-platform/config/GlobalConfig.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <mqtt/async_client.h>
#include <mqtt/callback.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>
#include <mqtt/message.h>
#include <nevonex-fcal-platform/notify/NotificationManager.hpp>
#include <nevonex/types/IMachineProvider.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <rapidjson/document.h>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <sstream>
#include <unistd.h>
#include <nevonex/cloud/Cloud.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(SubscribeConnectionFactory.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcb;
using namespace ::nevonex::log;

// Default constructor
SubscribeConnectionFactory::SubscribeConnectionFactory()
{

    /*PROTECTED REGION ID(SubscribeConnectionFactory__SubscribeConnectionFactory) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

SubscribeConnectionFactory::~SubscribeConnectionFactory()
{

    /*PROTECTED REGION ID(SubscribeConnectionFactory__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

// References

// SubscribeConnectionFactory framework operations (MQTT subscribe connection lifecycle)

::ecore::Ptr< ::nevonex::fcb::SubscribeConnectionFactory > SubscribeConnectionFactory::s_instance;

::nevonex::fcb::SubscribeConnectionFactory_ptr SubscribeConnectionFactory::getInstance()
{
    if (!s_instance.get())
    {
        s_instance = ::ecore::Ptr< SubscribeConnectionFactory >(new SubscribeConnectionFactory());
    }
    return s_instance;
}

void SubscribeConnectionFactory::initConnectionFactory()
{
    if (m_initialized) return;
    auto& _cfg = ::nevonex::config::GlobalConfig::getInstance();

    // FD fd.so 0xdb2a4-0xdb360: raw getMqttSSLHost() already carries the scheme
    // prefix (e.g. "tcp://192.168.40.100"), so do NOT prepend "ssl://" again —
    // that caused paho to receive "ssl://tcp://host:port" and block in futex.
    std::string _serverURI = _cfg.getMqttSSLHost() + ":" + std::to_string(static_cast<unsigned>(_cfg.getMqttSSLPort()));
    std::string _clientId = std::string("FCAL2FIL_Subscribe_") + std::to_string(::getpid());

    m_client.reset(new ::mqtt::async_client(_serverURI, _clientId));
    m_client->set_callback(*this);

    m_sslOpts = ::mqtt::ssl_options();
    m_sslOpts.set_trust_store(_cfg.getCaCertPath());
    m_sslOpts.set_key_store(_cfg.getClientCertPath());
    m_sslOpts.set_private_key(_cfg.getClientKeyPath());

    m_connOpts = ::mqtt::connect_options();
    m_connOpts.set_ssl(m_sslOpts);
    // FD fd.so 0xddee4-0xddf54: credentials path sets user_name + password
    m_connOpts.set_user_name(_cfg.getMqttUserName());
    m_connOpts.set_password(_cfg.getMqttPassword());

    try {
        // FD uses async connect + handler-driven wait, NOT token->wait()
        m_client->connect(m_connOpts);
        std::unique_lock<std::mutex> _lk(m_connectMutex);
        unsigned _timeoutSec = static_cast<unsigned>(_cfg.getMqttConnectionTimeout());
        if (!m_connectCv.wait_for(_lk, std::chrono::seconds(_timeoutSec > 0 ? _timeoutSec : 10),
                                  [this]{ return m_connected.load(); })) {
            // Timeout — leave uninitialized so a later retry can try again.
            return;
        }
    } catch (...) {
        // Swallow — FD wraps exceptions into CommunicationException; for now
        // just log path (log is not wired in this scaffolding), leave
        // m_initialized=false so the lifecycle can recover.
        return;
    }

    m_initialized = true;
}

void SubscribeConnectionFactory::stopSubscribeClient()
{
    if (m_client && m_client->is_connected())
    {
        m_client->disconnect()->wait();
    }
    m_connected.store(false);
    m_initialized = false;
}

void SubscribeConnectionFactory::connected(const std::string& /*_cause*/)
{
    try {
        const std::string& _featureId = ::nevonex::config::GlobalConfig::getInstance().getFeatureId();
        for (const auto& _topicEntry : m_topicMap)
        {
            for (const auto& _prefixEntry : m_prefixMap)
            {
                const std::string& _prefix = _prefixEntry.second;
                if (_prefix.empty()) continue;
                m_client->subscribe(_prefix + _topicEntry.first, 1);
            }
        }
        m_client->subscribe(std::string("fek/dataavailable/") + _featureId, 1);
    } catch (...) {
        // ignore subscribe failures; reconnect will retry
    }
    {
        std::lock_guard<std::mutex> _lk(m_connectMutex);
        m_connected.store(true);
    }
    m_connectCv.notify_all();

    // FD fd.so SubscribeConnectionFactory::connected() → pushNotification(FEATURE_CLIENT_STARTED)
    // NotificationManager requires FEATURE_CLIENT_STARTED to advance the ordered
    // notification sequence: INIT → CLIENT_STARTED → BEFORE_READY → READY.
    // Without this, FIF progress stalls at 70% waiting for the full sequence.
    ::nevonex::notify::NotificationManager::getInstance().pushNotification(
        ::nevonex::notify::NOTIFICATION_MESSAGE::FEATURE_CLIENT_STARTED);
}
void SubscribeConnectionFactory::connection_lost(const std::string& /*_cause*/) {}

void SubscribeConnectionFactory::setTopicMap(const std::unordered_map<std::string, std::string>& _map)
{ m_topicMap = _map; }
void SubscribeConnectionFactory::setPrefixMap(const std::unordered_map<std::string, std::string>& _map)
{ m_prefixMap = _map; }
void SubscribeConnectionFactory::setMachinePathMap(const std::unordered_map<std::string, std::string>& _map)
{ m_machinePathMap = _map; }
std::vector< ::ecore::EObject_ptr > SubscribeConnectionFactory::getProviders() const
{ return m_providers; }
void SubscribeConnectionFactory::addProvider(::ecore::EObject_ptr _prov)
{ m_providers.push_back(_prov); }

void SubscribeConnectionFactory::message_arrived(::mqtt::const_message_ptr _msg)
{
    if (!_msg) return;
    const std::string _topic = _msg->get_topic();
    const std::string _message = _msg->to_string();
    const std::string _cloudDownloadPrefix = "fek/dataavailable/";
    if (_topic.compare(0, _cloudDownloadPrefix.size(), _cloudDownloadPrefix) == 0)
    {
        ::nevonex::cloud::Cloud::getInstance()->processDownloadMessage(_message);
        return;
    }
    rapidjson::Document _doc;
    if (_doc.Parse(_message.c_str()).HasParseError() || !_doc.IsObject()) return;
    std::int64_t _timestamp = 0;
    if (_doc.HasMember("TS") && _doc["TS"].IsInt64())
    {
        _timestamp = _doc["TS"].GetInt64();
    }
    const rapidjson::Value& _payload = _doc.HasMember("PL") ? _doc["PL"] : static_cast<const rapidjson::Value&>(_doc);
    processMessage(_topic, _payload, _timestamp);
}

void SubscribeConnectionFactory::processMessage(const std::string& _topic, const rapidjson::Value& _payload, std::int64_t _timestamp)
{
    std::string _suffix = _topic;
    for (const auto& _prefixEntry : m_prefixMap)
    {
        const std::string& _prefix = _prefixEntry.second;
        if (!_prefix.empty() && _suffix.compare(0, _prefix.size(), _prefix) == 0)
        {
            _suffix.erase(0, _prefix.size());
            break;
        }
    }
    if (_suffix.empty() || _suffix[0] != '/')
    {
        _suffix.insert(0, "/");
    }

    if (_suffix.compare(0, 3, "/0/") == 0)
    {
        for (auto& _obj : m_providers)
        {
            auto _provider = ::ecore::as < ::nevonex::types::IMachineProvider > (_obj);
            if (!_provider) continue;
            std::istringstream _stream(_payload.IsString() ? _payload.GetString() : "");
            _provider->createMachines(_stream);
        }
        return;
    }
    if (_suffix.compare(0, 3, "/1/") == 0)
    {
        return;
    }

    auto _topicIt = m_topicMap.find(_suffix);
    if (_topicIt == m_topicMap.end()) return;
    const std::string& _interfaceKey = _topicIt->second;
    if (_interfaceKey.size() < 4 || _interfaceKey.compare(_interfaceKey.size() - 4, 4, ".sub") != 0) return;
    const std::size_t _dot = _interfaceKey.find('.');
    if (_dot == std::string::npos) return;
    const std::string _machineName = _interfaceKey.substr(0, _dot);
    auto _pathIt = m_machinePathMap.find(_machineName);
    if (_pathIt == m_machinePathMap.end()) return;
    const std::string& _machinePath = _pathIt->second;
    const std::size_t _pathDot = _machinePath.find('.');
    const std::string _root = _pathDot == std::string::npos ? _machinePath : _machinePath.substr(0, _pathDot);

    for (auto& _obj : m_providers)
    {
        auto _provider = ::ecore::as < ::nevonex::types::IMachineProvider > (_obj);
        if (!_provider) continue;
        if (!boost::iequals(_provider->getProviderName(), _root + "Provider")) continue;
        _provider->getTopicElement(_machinePath);
        _provider->processMessage(_interfaceKey, _payload, _timestamp);
    }
}
void SubscribeConnectionFactory::delivery_complete(::mqtt::delivery_token_ptr /*_tok*/) {}

