/*
 * nevonex/fcb/PublishConnectionFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "PublishConnectionFactory.hpp"
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
#include <nevonex-fcal-platform/communicator/ICommunicator.hpp>
#include <nevonex-fcal-platform/communicator/CommunicatorProperties.hpp>
#include <rapidjson/document.h>
#include <nevonex-fcal-platform/common/CommonUtils.hpp>
#include <unordered_map>
#include <map>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EClassifier.hpp>
#include <ecorecpp/mapping/any.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <chrono>
#include <cstdint>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(PublishConnectionFactory.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcb;
using namespace ::nevonex::log;

// Default constructor
PublishConnectionFactory::PublishConnectionFactory()
{

    /*PROTECTED REGION ID(PublishConnectionFactory__PublishConnectionFactory) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

PublishConnectionFactory::~PublishConnectionFactory()
{

    /*PROTECTED REGION ID(PublishConnectionFactory__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

// References

// PublishConnectionFactory framework operations (MQTT publish connection lifecycle)

::ecore::Ptr< ::nevonex::fcb::PublishConnectionFactory > PublishConnectionFactory::s_instance;

::nevonex::fcb::PublishConnectionFactory_ptr PublishConnectionFactory::getInstance()
{
    if (!s_instance.get())
    {
        s_instance = ::ecore::Ptr< PublishConnectionFactory >(new PublishConnectionFactory());
    }
    return s_instance;
}

void PublishConnectionFactory::initConnectionFactory()
{
    if (m_initialized) return;
    auto& _cfg = ::nevonex::config::GlobalConfig::getInstance();
    const std::string& _featureId = _cfg.getFeatureId();
    const std::string& _host = _cfg.getMqttHost();
    int _port = static_cast<int>(_cfg.getMqttPort());

    m_client = ::nevonex::communicator::ICommunicator::getAsyncClient(
        _featureId, std::string("FCAL2FIL_Publish"), _host, _port);

    m_client->setOnMessageRecievedHandler(
        std::bind(&PublishConnectionFactory::message_arrived, this,
            std::placeholders::_1, std::placeholders::_2));

    std::vector<std::string> _subscribeTopicList;
    _subscribeTopicList.push_back("fek/" + _featureId + "/intf/enable");
    _subscribeTopicList.push_back("fek/" + _featureId + "/intf/disable");
    m_client->setSubscribeTopicList(_subscribeTopicList);

    ::nevonex::communicator::CommunicatorProperties _props;
    _props.callerId = std::string("FCAL2FIL_Publish");
    _props.host = _host;
    _props.port = _port;
    _props.userName = _cfg.getMqttUserName();
    _props.password = _cfg.getMqttPassword();
    _props.sslHost = _cfg.getMqttSSLHost();
    _props.sslPort = static_cast<int>(_cfg.getMqttSSLPort());
    _props.trustStorePath = _cfg.getCaCertPath();
    _props.keyStorePath = _cfg.getClientCertPath();
    _props.privateKeyPath = _cfg.getClientKeyPath();
    _props.communicationAuthType = _cfg.getCommunicatorAuthType();

    m_client->setConnectionTimeout(_cfg.getMqttConnectionTimeout());
    m_client->connect(_props);
    m_initialized = true;
}

void PublishConnectionFactory::stopPublishClient()
{
    if (m_client && m_client->isClientConnected())
    {
        m_client->disconnect();
    }
    m_initialized = false;
}

void PublishConnectionFactory::terminateConnectionFactory()
{
    stopPublishClient();
    m_topicMap.clear();
    m_payloadMap.clear();
    m_prefixMap.clear();
    m_machinePathMap.clear();
    m_enabledDisabledTopicMap.clear();
}

namespace
{
/** TopicObject::index. FD 의 getMachineIndex 와 동일. */
int _machineIndexOf(const ::ecore::EObject_ptr& _obj)
{
    auto _topic = ::ecore::as< ::nevonex::common::TopicObject >(_obj);
    if (_topic) return _topic->getIndex();
    return 0;
}

/** 스칼라 피처 하나를 JSON 값으로. FD getPayloadValueNew 의 스칼라 분기와 동일. */
void _scalarToJson(const ::ecore::EObject_ptr& _obj,
    const ::ecore::EStructuralFeature_ptr& _feat,
    rapidjson::Value& _out, rapidjson::Document& _doc)
{
    _out.SetNull();
    if (!_obj || !_feat) return;
    auto _eType = _feat->getEType();
    if (!_eType) return;
    const auto& _tn = _eType->getName();
    try
    {
        auto _v = _obj->eGet(_feat);
        if (_tn == "EString")
        {
            auto& _s = ::ecorecpp::mapping::any::any_cast< ::ecore::EString >(_v);
            _out.SetString(_s.c_str(), _doc.GetAllocator());
        }
        else if (_tn == "EInt")
        {
            _out.SetInt(::ecorecpp::mapping::any::any_cast< ::ecore::EInt >(_v));
        }
        else if (_tn == "ELong")
        {
            _out.SetInt64(::ecorecpp::mapping::any::any_cast< ::ecore::ELong >(_v));
        }
        else if (_tn == "EDouble")
        {
            _out.SetDouble(::ecorecpp::mapping::any::any_cast< ::ecore::EDouble >(_v));
        }
        else if (_tn == "EFloat")
        {
            _out.SetFloat(::ecorecpp::mapping::any::any_cast< ::ecore::EFloat >(_v));
        }
        else if (_tn == "EBoolean")
        {
            _out.SetInt(::ecorecpp::mapping::any::any_cast< ::ecore::EBoolean >(_v) ? 1 : 0);
        }
    } catch (...) { _out.SetNull(); }
}

/**
 * 인터페이스 값 하나를 FD 규약 JSON 으로.
 *
 * _isArray 는 payloadMap(= 카탈로그의 dataType) 에서 온다. IDE 는 스칼라 인터페이스도
 * 값 클래스로 감싸지만(FD 는 머신에 EAttribute 로 직접 둔다), 와이어에서는 FD 와 같이
 * 배열이면 위치 배열, 스칼라면 값 하나여야 한다. 객체 모양만 보고는 둘을 구분할 수
 * 없어서 dataType 을 근거로 쓴다.
 */
void _payloadValue(const ::ecore::EObject_ptr& _machine,
    const ::ecore::EString& _featureName, bool _isArray,
    rapidjson::Value& _out, rapidjson::Document& _doc)
{
    _out.SetNull();
    if (!_machine || _featureName.empty()) return;
    auto _feat = _machine->eClass()->getEStructuralFeature(_featureName);
    if (!_feat) return;

    ::ecore::EJavaObject _val;
    try { _val = _machine->eGet(_feat); } catch (...) { return; }

    ::ecore::EObject_ptr _child;
    try
    {
        _child = ::ecorecpp::mapping::any::any_cast< ::ecore::EObject_ptr >(_val);
    } catch (...) { _child = nullptr; }

    if (_child)
    {
        // 상속 피처(TopicObject::index)는 빼야 한다 — FD 의 ArrayType 은 피처가 없고,
        // 잎에는 인터페이스 값만 들어간다. 그래서 getEAllStructuralFeatures 가 아니라
        // getEStructuralFeatures(자기 피처만) 를 쓴다.
        auto& _feats = _child->eClass()->getEStructuralFeatures();
        if (_isArray)
        {
            // 배열 인터페이스: 선언 순서대로 위치 기반 스칼라 배열.
            _out.SetArray();
            for (size_t _i = 0; _i < _feats.size(); ++_i)
            {
                rapidjson::Value _el;
                _scalarToJson(_child, _feats[_i], _el, _doc);
                _out.PushBack(_el, _doc.GetAllocator());
            }
            return;
        }
        // 스칼라 인터페이스: 감싼 값 클래스를 벗겨 값 하나만 놓는다.
        // (ecorecpp 의 EList 에는 empty() 가 없다 — size() 로 검사.)
        if (_feats.size() > 0) _scalarToJson(_child, _feats[0], _out, _doc);
        return;
    }

    // 머신에 직접 달린 스칼라 속성.
    _scalarToJson(_machine, _feat, _out, _doc);
}

/** boost 의존 없이 FD 의 boost::to_lower 와 같은 결과. */
std::string _toLowerAscii(const std::string& _s)
{
    std::string _out(_s);
    for (size_t _i = 0; _i < _out.size(); ++_i)
    {
        if (_out[_i] >= 'A' && _out[_i] <= 'Z') _out[_i] = static_cast<char>(_out[_i] - 'A' + 'a');
    }
    return _out;
}

/** boost 의존 없이 FD 의 boost::algorithm::ends_with 와 같은 결과. */
bool _endsWith(const std::string& _s, const std::string& _suffix)
{
    if (_s.size() < _suffix.size()) return false;
    return _s.compare(_s.size() - _suffix.size(), _suffix.size(), _suffix) == 0;
}
} // namespace

void PublishConnectionFactory::publish(const ::ecore::EString& _interfaceKey,
    const ::ecore::EString& _interfaceName,
    const std::vector< ::ecore::EObject_ptr >& _payload,
    ::nevonex::fcb::SubscriberEnum _subscriber)
{
    if (_interfaceKey.empty() || _payload.empty()) return;
    if (!isWriteEnabled(_interfaceKey))
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "Currently disabled for write access. Interface Key:" << _interfaceKey;
        return;
    }
    auto _it = m_topicMap.find(_interfaceKey);
    if (_it == m_topicMap.end())
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "publish: topicMap key not found. Key:" << _interfaceKey;
        return;
    }

    // machine_path 깊이가 2 이상이면 FD 는 부모/자식 중첩(boom) 구조를 만든다.
    // IDE 카탈로그에는 아직 그런 provider 가 없어(machine_path 가 전부 name -> name)
    // 여기 도달하지 않는다. 서브머신 provider 가 생기면 FD 의 constructPayload 를
    // 옮겨와야 한다 — 조용히 틀린 형태로 보내느니 막고 로그를 남긴다.
    auto _mpIt = m_machinePathMap.find(_interfaceKey.substr(0, _interfaceKey.find('.')));
    if (_mpIt != m_machinePathMap.end()
            && _mpIt->second.find('.') != std::string::npos)
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "publish: nested machine path is not supported. Path:" << _mpIt->second;
        return;
    }

    // 배열/스칼라 구분은 카탈로그의 dataType 을 그대로 옮긴 payloadMap 이 근거다.
    bool _isArray = false;
    auto _plIt = m_payloadMap.find(_interfaceKey);
    if (_plIt != m_payloadMap.end()) _isArray = (_plIt->second == "array");

    rapidjson::Document _doc;
    _doc.SetObject();
    auto& _alloc = _doc.GetAllocator();

    rapidjson::Value _byIndex(rapidjson::kObjectType);
    std::string _root;
    for (auto& _obj : _payload)
    {
        if (!_obj) continue;
        rapidjson::Value _leaf;
        _payloadValue(_obj, _interfaceName, _isArray, _leaf, _doc);
        const std::string _idx = std::to_string(_machineIndexOf(_obj));
        rapidjson::Value _idxKey(_idx.c_str(), _alloc);
        _byIndex.AddMember(_idxKey, _leaf, _alloc);
        _root = _obj->eClass()->getName();
    }
    if (_root.empty()) return;

    rapidjson::Value _rootKey(_toLowerAscii(_root).c_str(), _alloc);
    rapidjson::Value _pl(rapidjson::kObjectType);
    _pl.AddMember(_rootKey, _byIndex, _alloc);
    _doc.AddMember("PL", _pl, _alloc);

    int64_t _ts = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    _doc.AddMember("TS", _ts, _alloc);

    publishFinal(_subscriber, _it->second, _doc);
}

void PublishConnectionFactory::publish(const ::ecore::EString& _interfaceName,
    ::ecore::EObject_ptr /*_obj*/,
    rapidjson::Value& /*_val*/,
    rapidjson::Document& _doc,
    ::nevonex::fcb::SubscriberEnum _subscriber)
{
    auto _it = m_topicMap.find(_interfaceName);
    if (_it == m_topicMap.end()) return;
    publishFinal(_subscriber, _it->second, _doc);
}

void PublishConnectionFactory::publishFinal(::nevonex::fcb::SubscriberEnum _subscriber,
    const ::ecore::EString& _topic, rapidjson::Document& _doc)
{
    (void)_subscriber;
    if (!m_client || !m_client->isClientConnected()) return;
    std::string _payload = ::nevonex::common::utils::CommonUtils::convertJsonToString(_doc);
    for (auto& _e : m_prefixMap)
    {
        const std::string _finalTopic = _e.second + _topic + "/process";
        m_client->publish(_finalTopic, _payload);
        SDK_FCAL_LOG(SeverityLevel::debug) << "Topic published from FCAL Layer: "
                << _finalTopic << " data:" << _payload;
    }
}

void PublishConnectionFactory::setTopicMap(const std::unordered_map<std::string, std::string>& _map)
{ m_topicMap = _map; }
void PublishConnectionFactory::setPayloadMap(const std::unordered_map<std::string, std::string>& _map)
{ m_payloadMap = _map; }
void PublishConnectionFactory::setPrefixMap(const std::unordered_map<std::string, std::string>& _map)
{ m_prefixMap = _map; }
void PublishConnectionFactory::setMachinePathMap(const std::unordered_map<std::string, std::string>& _map)
{ m_machinePathMap = _map; }
void PublishConnectionFactory::setEnabledDisabledTopicMap(const std::unordered_map<std::string, bool>& _map)
{
    m_enabledDisabledTopicMap = _map;
    for (const auto& _key : ::nevonex::config::GlobalConfig::getInstance().getDisabledFeatures())
    {
        auto _it = m_enabledDisabledTopicMap.find(_key);
        if (_it == m_enabledDisabledTopicMap.end())
        {
            SDK_FCAL_LOG(SeverityLevel::error)
                    << "The feature we want to disable does not exists. Key:" << _key;
            continue;
        }
        _it->second = false;
    }
}

bool PublishConnectionFactory::isWriteEnabled(const ::ecore::EString& _interfaceKey) const
{
    auto _it = m_topicMap.find(_interfaceKey);
    if (_it == m_topicMap.end()) return false;
    auto _flag = m_enabledDisabledTopicMap.find(_it->second);
    if (_flag == m_enabledDisabledTopicMap.end()) return true;
    return _flag->second;
}

std::vector< ::ecore::EObject_ptr > PublishConnectionFactory::getProviders() const
{ return m_providers; }

void PublishConnectionFactory::addProvider(::ecore::EObject_ptr _prov)
{ m_providers.push_back(_prov); }

void PublishConnectionFactory::connection_lost(const ::ecore::EString& /*_cause*/)
{
    m_initialized = false;
    initConnectionFactory();
}

void PublishConnectionFactory::message_arrived(const std::string& _topic, const std::string& _message)
{
    if (_message.empty())
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "PublishConnectionFactory payload is empty for the topic :" << _topic;
        return;
    }

    bool _writeFlag = false;
    if (_endsWith(_topic, "enable"))
    {
        _writeFlag = true;
    }
    else if (!_endsWith(_topic, "disable"))
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "PublishConnectionFactory wrong message arrived :" << _topic
                << " Payload : " << _message;
        return;
    }

    rapidjson::Document _in;
    _in.Parse(_message.c_str());
    if (_in.HasParseError() || !_in.IsObject() || !_in.HasMember("PL"))
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "payload does not contains PL tag. Topic :" << _topic
                << " Payload : " << _message;
        return;
    }
    if (!_in["PL"].IsArray())
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "payload with PL tag is not an array. Topic :" << _topic
                << " Payload : " << _message;
        return;
    }

    rapidjson::Document _out;
    _out.SetObject();
    auto& _alloc = _out.GetAllocator();
    rapidjson::Value _accepted(rapidjson::kArrayType);

    for (auto& _v : _in["PL"].GetArray())
    {
        if (!_v.IsInt()) continue;
        const int _id = _v.GetInt();
        const std::string _key = "/" + std::to_string(_id);
        auto _it = m_enabledDisabledTopicMap.find(_key);
        if (_it == m_enabledDisabledTopicMap.end())
        {
            SDK_FCAL_LOG(SeverityLevel::warning)
                    << "Given write interface is not present. Interface Key :" << _key;
            continue;
        }
        _it->second = _writeFlag;
        _accepted.PushBack(_id, _alloc);
    }

    _out.AddMember("PL", _accepted, _alloc);
    int64_t _ackTs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    _out.AddMember("TS", _ackTs, _alloc);

    if (m_client && m_client->isClientConnected())
    {
        m_client->publish(_topic + "/ack",
                ::nevonex::common::utils::CommonUtils::convertJsonToString(_out));
    }
}

