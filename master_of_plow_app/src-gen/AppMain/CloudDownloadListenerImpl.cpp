/*
 * AppMain/CloudDownloadListenerImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CloudDownloadListener.hpp"
#include <stdexcept>

#include <AppMain/MainController.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>
#include <json/json.h>
#include <memory>
#include <string>
#include "web/WebSocketEndPoint.hpp"

/*PROTECTED REGION ID(CloudDownloadListenerImpl.cpp) ENABLED START*/
#include "RddfLoader.hpp"
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

/*PROTECTED REGION ID(CloudDownloadListenerImpl_Methods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
void CloudDownloadListener::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(CloudDownloadListenerImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

void CloudDownloadListener::handleMessage(const std::string &_content)
{
    /*PROTECTED REGION ID(CloudDownloadListener_handleMessage_preParse) ENABLED START*/
    // 이 앱은 클라우드로 HTTP 요청을 보내지 않으므로 응답도 처리할 것이 없습니다.
    // 맵 리스트 조회와 리더보드 전송이 사라지면서 이 경로 전체가 비었습니다.
    // 클라우드에서 오는 것 중 앱이 쓰는 것은 .rddf 파일뿐이고 그건 handleFile이
    // 받습니다.
    //
    // The app makes no outbound cloud HTTP calls, so there are no responses to
    // dispatch: the map-list query and the leaderboard uploads are both gone.
    // The only cloud input this app consumes is the .rddf file, and that arrives
    // through handleFile().
    NEVONEX_LOG(SeverityLevel::debug) << "Cloud Message ignored: " << _content;
    /*PROTECTED REGION END*/

    broadcastCloudResponseToUi(_content);

    /*PROTECTED REGION ID(CloudDownloadListener_handleMessage_afterBroadcast) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Default handling: wrap the proxy-response JSON in an envelope and broadcast
// it to the UI over the WebSocket. Kept out of handleMessage so that user code
// in the protected regions above owns its own scope — return early from
// CloudDownloadListener_handleMessage_preParse to suppress this for a message.
void CloudDownloadListener::broadcastCloudResponseToUi(const std::string &_content)
{
    // Backend payloads observed from devices may carry the upstream response
    // directly or as a JSON-encoded payload under a top-level value object.
    // Broadcast envelope (outgoing): { "type": "external_api_response", "correlation-id": "...", "data": {...} }
    Json::CharReaderBuilder rb;
    std::string errs;
    std::unique_ptr<Json::CharReader> reader(rb.newCharReader());
    Json::Value root;
    if (reader->parse(_content.data(), _content.data() + _content.size(), &root, &errs))
    {
        std::string correlationId;
        if (root.isObject())
        {
            correlationId = root.get("correlation-id", root.get("correlationId", "")).asString();
        }
        Json::Value responseSource = root;
        if (root.isObject() && root.isMember("value") && root["value"].isObject())
        {
            const Json::Value& value = root["value"];
            if (correlationId.empty())
            {
                correlationId = value.get("correlation-id", value.get("correlationId", "")).asString();
            }
            if (value.isMember("payload") && value["payload"].isString())
            {
                const std::string payload = value["payload"].asString();
                Json::Value payloadJson;
                std::string payloadErrs;
                std::unique_ptr<Json::CharReader> payloadReader(rb.newCharReader());
                if (!payload.empty()
                    && payloadReader->parse(payload.data(),
                                            payload.data() + payload.size(),
                                            &payloadJson, &payloadErrs))
                {
                    responseSource = payloadJson;
                }
                else
                {
                    responseSource = payload;
                }
            }
            else if (value.isMember("payload") && !value["payload"].isNull())
            {
                responseSource = value["payload"];
            }
            else
            {
                responseSource = value;
            }
        }
        else if (root.isObject() && root.isMember("payload") && root["payload"].isString())
        {
            const std::string payload = root["payload"].asString();
            Json::Value payloadJson;
            std::string payloadErrs;
            std::unique_ptr<Json::CharReader> payloadReader(rb.newCharReader());
            if (!payload.empty()
                && payloadReader->parse(payload.data(),
                                        payload.data() + payload.size(),
                                        &payloadJson, &payloadErrs))
            {
                responseSource = payloadJson;
            }
            else
            {
                responseSource = payload;
            }
        }
        else if (root.isObject() && root.isMember("payload") && !root["payload"].isNull())
        {
            responseSource = root["payload"];
        }
        Json::Value responseData;
        if (!responseSource.isObject())
        {
            responseData = responseSource;
        }
        else if (responseSource.isMember("data") && !responseSource["data"].isNull())
        {
            responseData = responseSource["data"];
        }
        else if (responseSource.isMember("msg") && !responseSource["msg"].isNull())
        {
            responseData = responseSource["msg"];
        }
        else if (responseSource.isMember("body") && !responseSource["body"].isNull())
        {
            responseData = responseSource["body"];
        }
        else if (responseSource.isMember("content") && !responseSource["content"].isNull())
        {
            responseData = responseSource["content"];
        }
        else if (responseSource.isMember("response") && !responseSource["response"].isNull())
        {
            responseData = responseSource["response"];
        }
        else
        {
            responseData = responseSource;
        }
        Json::Value envelope;
        envelope["type"] = "external_api_response";
        envelope["correlation-id"] = correlationId;
        envelope["data"] = responseData;
        Json::StreamWriterBuilder wb;
        wb["indentation"] = "";
        const std::string broadcast = Json::writeString(wb, envelope);
        auto ws = ::AppMain::web::WebSocketEndPoint::getInstance();
        if (ws)
        {
            ws->publishMessage(broadcast);
        }
        NEVONEX_LOG(SeverityLevel::info) << "[CloudDownload] correlation-id="
                                          << envelope["correlation-id"].asString()
                                          << " forwarded to WS";
    }
    else
    {
        NEVONEX_LOG(SeverityLevel::warning) << "[CloudDownload] JSON parse failed: " << errs;
    }
}

void CloudDownloadListener::handleFile(const ::nevonex::resource::FilePath &_filePath)
{
    NEVONEX_LOG(SeverityLevel::debug) << "Cloud File: " << _filePath.get().string();
    /*PROTECTED REGION ID(CloudDownloadListener_handleFile_body) ENABLED START*/
    const auto path = _filePath.get();
    NEVONEX_LOG(SeverityLevel::debug) << "Cloud File: " << path.string();

    if (path.extension() != ".rddf")
    {
        return;
    }
    if (!m_mainController)
    {
        NEVONEX_LOG(SeverityLevel::warning)
                << "[CloudDownloadListener] rddf received but MainController not injected";
        return;
    }

    // 예전에는 여기서 RddfStagingManager::stage()로 pending 폴더에 넣어두고,
    // UI가 맵을 고르고 승인해야 비로소 적재됐습니다. 맵 리스트도 맵 선택도
    // 없어졌으니 받는 즉시 적재합니다.
    //
    // This used to stage the file into a pending folder and wait for the UI to
    // pick a map and approve it. With no map list and no map picker, the file is
    // loaded the moment it arrives.
    ::AppMain::RddfLoader::loadFromFile(path.string(), *m_mainController.get());
    /*PROTECTED REGION END*/
}

/*PROTECTED REGION ID(CloudDownloadListenerImpl_MethodsEnd) ENABLED START*/
// pushMessage()/handleMapListResponse() 는 외부 서버 맵 리스트 응답을 UI로
// 중계하던 코드였습니다. 두 경로 모두 사라져 남길 것이 없습니다.
//
// pushMessage()/handleMapListResponse() relayed external-server map-list
// responses to the UI. Both ends of that path are gone.
/*PROTECTED REGION END*/
