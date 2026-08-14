/*
 * AppMain/web/WebSocketEndPointImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "WebSocketEndPoint.hpp"
#include <stdexcept>

#include <AppMain/MainController.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>
#include <functional>
#include <mutex>
#include <sstream>
#include <string>

#include <nevonex-fcal-platform/log/Logger.hpp>

#include <json/json.h>
#include <nevonex/cloud/Cloud.hpp>
#include <chrono>
#include <memory>

/*PROTECTED REGION ID(WebSocketEndPointImpl.cpp) ENABLED START*/

/*PROTECTED REGION END*/

/*PROTECTED REGION ID(WebSocketEndPoint_additional_headers_Impl) ENABLED START*/
#include <algorithm>
#include <json/json.h>
#include <string>
#include <AppMain/web/TuningProtocol.hpp>
#include <AppMain/web/OrchestrationProtocol.hpp>
/*PROTECTED REGION END*/

using namespace ::AppMain::web;
using namespace ::nevonex::log;

namespace {

#ifdef ENABLE_RAPID_JSON_API
std::string getRapidStringMember(const rapidjson::Value& ui, const char* key, const char* defaultValue = "")
{
    if (ui.HasMember(key) && ui[key].IsString())
    {
        return ui[key].GetString();
    }
    return defaultValue;
}

bool isExternalApiRequest(const rapidjson::Value& ui)
{
    return ui.IsObject()
            && ui.HasMember("endPoint")
            && ui["endPoint"].IsString()
            && !ui.HasMember("type")
            && !ui.HasMember("subType");
}

std::string buildExternalApiRequestJson(const rapidjson::Value& ui)
{
    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
    std::ostringstream corr;
    corr << "WS" << ms;

    const std::string uiCid = getRapidStringMember(ui, "correlation-id");
    const std::string correlationId = uiCid.empty() ? corr.str() : uiCid;
    const std::string externalUrl = getRapidStringMember(ui, "endPoint");
    const std::string method = getRapidStringMember(ui, "methodSelect", "GET");

    rapidjson::Document req;
    req.SetObject();
    rapidjson::Document::AllocatorType& allocator = req.GetAllocator();
    req.AddMember("correlation-id", rapidjson::Value(correlationId.c_str(), allocator).Move(), allocator);
    req.AddMember("externalUrl", rapidjson::Value(externalUrl.c_str(), allocator).Move(), allocator);
    req.AddMember("method", rapidjson::Value(method.c_str(), allocator).Move(), allocator);

    if (ui.HasMember("reqHeader"))
    {
        rapidjson::Value header;
        header.CopyFrom(ui["reqHeader"], allocator);
        req.AddMember("header", header, allocator);
    }
    else
    {
        req.AddMember("header", rapidjson::Value(rapidjson::kNullType).Move(), allocator);
    }

    if (ui.HasMember("reqBody"))
    {
        rapidjson::Value msg;
        msg.CopyFrom(ui["reqBody"], allocator);
        req.AddMember("msg", msg, allocator);
    }
    else
    {
        req.AddMember("msg", rapidjson::Value(rapidjson::kNullType).Move(), allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    req.Accept(writer);
    return std::string(buffer.GetString(), buffer.GetSize());
}

void dispatchExternalApiRequest(const rapidjson::Value& ui)
{
    const std::string payload = buildExternalApiRequestJson(ui);
    ::nevonex::cloud::Cloud::getInstance()->uploadData(payload, 1);
}
#else
bool isExternalApiRequest(const Json::Value& ui)
{
    return ui.isObject()
            && ui.isMember("endPoint")
            && ui["endPoint"].isString()
            && !ui.isMember("type")
            && !ui.isMember("subType");
}

// Build proxy-request JSON for backend external URL forwarding.
// Contract: { correlation-id, externalUrl, method, header, msg }
std::string buildExternalApiRequestJson(const Json::Value& ui)
{
    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
    std::ostringstream corr;
    corr << "WS" << ms;

    const std::string uiCid = ui.get("correlation-id", "").asString();
    Json::Value req;
    req["correlation-id"] = uiCid.empty() ? corr.str() : uiCid;
    req["externalUrl"] = ui.get("endPoint", "").asString();
    req["method"] = ui.get("methodSelect", "GET").asString();
    req["header"] = ui.get("reqHeader", Json::Value());
    req["msg"] = ui.get("reqBody", Json::Value());

    Json::StreamWriterBuilder wb;
    wb["indentation"] = "";
    return Json::writeString(wb, req);
}

// Dispatch UI-originated external API request through Cloud proxy channel.
void dispatchExternalApiRequest(const Json::Value& ui)
{
    const std::string payload = buildExternalApiRequestJson(ui);
    ::nevonex::cloud::Cloud::getInstance()->uploadData(payload, 1);
}
#endif

} // anonymous namespace

/*PROTECTED REGION ID(WebSocketEndPointImpl_Methods) ENABLED START*/
// loadRddfFromFile() 이 있던 자리입니다. AppMain/RddfLoader 로 옮겼습니다.
// loadRddfFromFile() lived here; it now lives in AppMain/RddfLoader.
/*PROTECTED REGION END*/
void WebSocketEndPoint::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(WebSocketEndPointImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// Singleton
std::shared_ptr< WebSocketEndPoint > WebSocketEndPoint::s_holder;

std::shared_ptr< WebSocketEndPoint > WebSocketEndPoint::getInstance()
{
    if (!s_holder)
    {
        s_holder = std::shared_ptr< WebSocketEndPoint >(new WebSocketEndPoint());
    }
    return s_holder;
}

void WebSocketEndPoint::onWebSocketMessage(const std::string &message)
{
    NEVONEX_LOG(SeverityLevel::debug) << "Received message from web socket client: " << message;
#ifdef ENABLE_RAPID_JSON_API
    rapidjson::Document outputJsonObject;
    outputJsonObject.Parse(message.c_str());
    if (!outputJsonObject.HasParseError())
    {
        if (isExternalApiRequest(outputJsonObject))
        {
            dispatchExternalApiRequest(outputJsonObject);
            return;
        }
        onWebSocketJsonMessage(outputJsonObject);
        return;
    }
    NEVONEX_LOG(SeverityLevel::error) << "onWebSocketMessage: Failed to parse JSON message";
#else // ENABLE_RAPID_JSON_API
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    std::string errs;
    Json::Value jsonMessage;
    auto payloadMessage = std::istringstream(message);
    bool parsingSuccessful = Json::parseFromStream(builder, payloadMessage,
            &jsonMessage, &errs);
    if (parsingSuccessful)
    {
        if (isExternalApiRequest(jsonMessage))
        {
            dispatchExternalApiRequest(jsonMessage);
            return;
        }
        onWebSocketJsonMessage(jsonMessage);
        return;
    }
    NEVONEX_LOG(SeverityLevel::error)
            << "onWebSocketMessage: Failed to parse JSON message" << errs;
#endif // ENABLE_RAPID_JSON_API

    /*PROTECTED REGION ID(WebSocketEndPoint__onWebSocketMessage) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

void WebSocketEndPoint::publishMessage(const std::string &message)
{
    /*PROTECTED REGION ID(WebSocketEndPoint_publishMessage_string_method) ENABLED START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    // 발행은 여러 스레드에서 들어온다: ProcessTimer(10Hz pp_telemetry),
    // 오케스트레이션 워커(스냅샷/거부), 플러그인 콜백. 뮤텍스 없이 같은 큐를
    // 밀면 경쟁이 나고, 목록 자체도 createWebsocketRoute/disconnect 콜백과
    // 경쟁한다. 붙어 있는 모든 클라이언트에 같은 잠금 아래에서 내보낸다.
    //
    // Publishes arrive from several threads: ProcessTimer (10 Hz pp_telemetry),
    // the orchestration worker (snapshots and rejections), and plugin callbacks.
    // Pushing the same queue without a mutex races, and the list itself races
    // with createWebsocketRoute and the disconnect callbacks. Fan out to every
    // attached client under that one lock.
    std::lock_guard<std::recursive_mutex> publishLock(socketsMutex_);
    if (sockets_.empty())
    {
        return;
    }
    NEVONEX_LOG(SeverityLevel::debug)
            << "Websocket publish message to " << sockets_.size()
            << " client(s). Message:" << message;
    for (auto *client : sockets_)
    {
        client->publishMessage(message);
    }

    /*PROTECTED REGION END*/
}

void WebSocketEndPoint::disconnect()
{
    NEVONEX_LOG(SeverityLevel::info)
            << "The feature is going to be stopped, so web socket client is getting disconnected.";
    if (websocket != nullptr)
    {
        websocket->close();
        websocket = nullptr;
    }
}

::nevonex::web::server::WebSocketRoute* WebSocketEndPoint::createWebsocketRoute(
        const Poco::Net::HTTPServerRequest &request)
{
    /*PROTECTED REGION ID(WebSocketEndPoint_createWebsocketRoute_method) ENABLED START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    using namespace ::nevonex::web::server;
    (void) request;

    // 새 클라이언트가 와도 기존 연결을 끊지 않는다.
    //
    // 원래 코드는 "클라이언트는 하나만 지원"이라며 이전 연결에 close() 를 부르고
    // websocket 포인터를 갈아치웠다. 그런데 그 close() 는 TCP 를 실제로 내리지
    // 않는다(실측: 밀려난 소켓이 계속 ESTABLISHED). 그래서 밀려난 클라이언트는
    // 자기가 밀려났다는 사실조차 모르고 재접속하지 않으며, 그 상태로 프레임만
    // 영영 못 받는다. 게다가 나중에 붙은 쪽이 나가면 onDisconnect 가 포인터를
    // nullptr 로 만들어 남아 있던 클라이언트까지 같이 죽는다.
    //
    // Do not evict the existing client when a new one arrives. The original code
    // called close() on the previous connection and swapped the pointer, on the
    // grounds that "only one client is supported". That close() does not actually
    // tear the TCP connection down (measured: the superseded socket stays
    // ESTABLISHED), so the displaced client never learns it was displaced and
    // never reconnects — it just stops receiving. Worse, when the newer client
    // leaves, onDisconnect nulls the pointer and takes the surviving client with
    // it. Track them all instead.
    auto *created = new MessageQueueWebsocket();
    using std::placeholders::_1;
    created->set_message_handler(
            std::bind(&WebSocketEndPoint::onWebSocketMessage, this, _1));
    created->set_disconnected_handler(
            [this, created](const std::string &reason) {
                std::size_t remaining = 0;
                {
                    std::lock_guard<std::recursive_mutex> lock(socketsMutex_);
                    sockets_.erase(
                            std::remove(sockets_.begin(), sockets_.end(), created),
                            sockets_.end());
                    remaining = sockets_.size();
                    // 생성된 코드가 쓰는 필드는 "가장 최근 연결"로 유지한다.
                    // Keep the generated field pointing at the newest connection.
                    websocket = sockets_.empty() ? nullptr : sockets_.back();
                }
                if (remaining == 0)
                {
                    onDisconnect(reason);
                    return;
                }
                NEVONEX_LOG(SeverityLevel::info)
                        << "Web socket client disconnected, " << remaining
                        << " still attached: " << reason;
            });

    // 상한을 두고 가장 오래된 것부터 정리한다. 브라우저가 FIN 없이 사라지면
    // (탭 강제 종료, 릴레이 중단) 좀비 연결이 남는데, 그것까지 무한히 들고
    // 있으면 매 프레임이 죽은 큐로도 복사된다.
    //
    // Cap the list and retire the oldest entries. A browser that vanishes without
    // a FIN (killed tab, interrupted relay) leaves a zombie connection behind;
    // keeping those forever would copy every frame into dead queues too.
    // close() runs outside the lock: it may invoke the disconnect handler inline.
    std::vector<MessageQueueWebsocket *> evicted;
    {
        std::lock_guard<std::recursive_mutex> lock(socketsMutex_);
        sockets_.push_back(created);
        websocket = created;
        while (sockets_.size() > MAX_WEBSOCKET_CLIENTS)
        {
            evicted.push_back(sockets_.front());
            sockets_.erase(sockets_.begin());
        }
    }
    for (auto *stale : evicted)
    {
        NEVONEX_LOG(SeverityLevel::warning)
                << "Too many web socket clients, closing the oldest one.";
        stale->close();
    }
    return created;

    /*PROTECTED REGION END*/
}

void WebSocketEndPoint::onDisconnect(const std::string &message)
{
    NEVONEX_LOG(SeverityLevel::info) << "Web socket client disconnected: " << message;
    websocket = nullptr;
}

#ifdef ENABLE_RAPID_JSON_API
void WebSocketEndPoint::onWebSocketJsonMessage(rapidjson::Document &outputJsonObject)
{
    /*PROTECTED REGION ID(WebSocketEndPointonWebSocketRapidJsonMessage_method) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

void WebSocketEndPoint::publishMessage(const rapidjson::Document &jsonMessage)
{
    /*PROTECTED REGION ID(WebSocketEndPoint_publishMessage_rapidjson) ENABLED START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
     ::rapidjson::StringBuffer stringBuffer;
     CommonUtils::convertJsonToString(jsonMessage, stringBuffer);
     std::string stringFromJson = std::string(stringBuffer.GetString());

    /*PROTECTED REGION END*/
    rapidjson::StringBuffer seamosStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> seamosWriter(seamosStringBuffer);
    jsonMessage.Accept(seamosWriter);
    this->publishMessage(std::string(seamosStringBuffer.GetString(), seamosStringBuffer.GetSize()));
}
#else // ENABLE_RAPID_JSON_API
void WebSocketEndPoint::onWebSocketJsonMessage(Json::Value &jsonMessage)
{
    /*PROTECTED REGION ID(WebSocketEndPointonWebSocketJsonMessage_method) ENABLED START*/
    // ┌─ 한국어 ───────────────────────────────────────────────────────────┐
    // 이 소켓의 인바운드는 튜닝 제어 채널 3종뿐입니다 (get_tuning_state /
    // set_tracker / set_params — web/TuningProtocol 참고). 예전 게임 세션
    // 프로토콜 27종을 되살리는 것이 아닙니다. 파싱·검증·전환 로직은 FD
    // 재생성이 지우지 못하도록 마커 없는 별도 파일에 있고, 여기서는 호출만
    // 합니다.
    // └────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────┐
    // The only inbound traffic on this socket is the 3-message tuning
    // control channel (get_tuning_state / set_tracker / set_params — see
    // web/TuningProtocol). This does not resurrect the old 27-kind session
    // protocol. Parsing/validation/switching live in a marker-free file so
    // regeneration cannot delete them; this region only calls it.
    // └────────────────────────────────────────────────────────────────────┘
    if (m_mainController.get() != nullptr)
    {
        if (!::AppMain::web::handleOrchestrationMessage(jsonMessage,
                *m_mainController.get())) {
            ::AppMain::web::handleTuningMessage(jsonMessage,
                    *m_mainController.get());
        }
    }
    /*PROTECTED REGION END*/
}

void WebSocketEndPoint::publishMessage(const Json::Value &jsonMessage)
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    /*PROTECTED REGION ID(WebSocketEndPoint_publishMessage_json) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    this->publishMessage(Json::writeString(builder, jsonMessage));
}
#endif // ENABLE_RAPID_JSON_API

/*PROTECTED REGION ID(WebSocketEndPointImpl_MethodsEnd) ENABLED START*/
// publishLeaderboardError() 와 publishStartSessionError() 가 있던 자리입니다.
// 각각 외부 리더보드 서버 오류와 게임 세션 시작 실패를 UI로 보냈습니다.
//
// publishLeaderboardError() and publishStartSessionError() were here; they
// reported leaderboard-server errors and session-start failures to the UI.
/*PROTECTED REGION END*/
