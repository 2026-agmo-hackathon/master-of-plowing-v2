/*
 * AppMain/web/WebSocketEndPoint.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_WEB_WEBSOCKETENDPOINT_HPP
#define APPMAIN_WEB_WEBSOCKETENDPOINT_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain/web_forward.hpp>

#include <AppMain_forward.hpp>

#include <nevonex-fcal-platform/web/server/WebSocketRouteFactory.hpp>
#include <nevonex-fcal-platform/web/server/MessageQueueWebsocket.hpp>

#include <nevonex-fcal-platform/common/CommonUtils.hpp>

/*PROTECTED REGION ID(WebSocketEndPoint_rapidjson_define_headers) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

#ifdef ENABLE_RAPID_JSON_API
        #include <rapidjson/document.h>
        #include <rapidjson/stringbuffer.h>
        #include <rapidjson/writer.h>
    #else // ENABLE_RAPID_JSON_API
#include <jsoncpp/json/json.h>
#endif // ENABLE_RAPID_JSON_API

using namespace ::nevonex::common::utils;

/*PROTECTED REGION ID(WebSocketEndPoint_additional_headers) ENABLED START*/
// SessionProtocol.hpp(세션/게임 메시지 27종)와 RddfStagingManager 전방선언이
// 있던 자리입니다. 이 엔드포인트는 이제 순수 전송 계층이며, 곧 자율주행
// 관측값을 내보내는 대시보드 텔레메트리 통로가 됩니다.
//
// SessionProtocol.hpp (27 session/game message kinds) and the
// RddfStagingManager forward declaration were here. This endpoint is now a plain
// transport, to be used for outbound dashboard telemetry.
#include <cstddef>
#include <mutex>
#include <vector>
/*PROTECTED REGION END*/

namespace AppMain
{
    namespace web
    {

        class WebSocketEndPoint: public virtual ::ecore::EObject,
                public ::nevonex::web::server::WebSocketRouteFactory
        {
            /*PROTECTED REGION ID(WebSocketEndPoint_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        public:
            WebSocketEndPoint();

            virtual ~WebSocketEndPoint();

            virtual void _initialize() override;

            // Operations from Parent(s)

            // Operations

            // Attributes

            // References
            /**
             * \brief 
             */
        public:
            virtual ::AppMain::MainController_ptr getMainController() const;
            /**
             * \brief 
             */
        public:
            virtual void setMainController(
                    ::AppMain::MainController_ptr _mainController);

        public:

            /*PROTECTED REGION ID(WebSocketEndPoint) ENABLED START*/
            // 걷어낸 것:
            //   getSpeedMultiplier()      — 시뮬 배속. 실기/외부 시뮬에서는 무의미
            //   getElapsedS()             — 게임 세션 경과시간
            //   setRddfStagingManager()   — RDDF 승인 흐름
            //   publishLeaderboardError() — 외부 리더보드 서버
            //   publishStartSessionError()— 게임 세션 시작 실패 통보
            //
            // Removed: the simulation speed multiplier and session elapsed time
            // (both game-session concepts), the RDDF staging injection, and the two
            // error publishers for the leaderboard server and session start.

            // 연결을 `websocket` 포인터 하나로만 들고 있으면 통신이 영구히 죽는다.
            //
            // 두 번째 클라이언트가 붙으면 createWebsocketRoute 가 그 포인터를 덮어써서
            // 첫 번째는 프레임을 못 받고, 그 두 번째가 나가면 onDisconnect 가 포인터를
            // nullptr 로 만들어 **아무도** 못 받는다. 첫 번째의 TCP 는 ESTABLISHED 로
            // 남고 수신 핸들러도 살아 있어서, 브라우저는 명령을 보낼 수는 있지만
            // 응답을 영영 못 받는다 — UI 는 link='stalled' 로 굳어 Start 를 포함한
            // 모든 조작이 이유 없이 비활성된다. 탭 두 개, 새로고침, 대시보드 iframe,
            // 진단 프로브 어느 것이든 두 번째 클라이언트가 된다.
            //
            // Holding the connection in a single `websocket` pointer kills the
            // transport for good. A second client makes createWebsocketRoute
            // overwrite the pointer, cutting the first client off; when that second
            // client leaves, onDisconnect nulls the pointer and *nobody* receives
            // anything. The first client's TCP stays ESTABLISHED and its inbound
            // handler keeps working, so the browser can still send commands but
            // never gets a reply — the UI freezes at link='stalled' and disables
            // every control, Start included, with nothing on screen to explain it.
            // A second tab, a reload, the dashboard iframe or a diagnostic probe
            // are all it takes. Measured: client A stopped at frame 49 the instant
            // B attached and stayed dead for the rest of the run, including the
            // 16 s after B had disconnected.
            //
            // So keep every live connection and fan publishes out to all of them.
            // socketsMutex_ is recursive because MessageQueueWebsocket may run a
            // disconnect callback synchronously from inside publishMessage/close.
            static constexpr std::size_t MAX_WEBSOCKET_CLIENTS = 8;
            std::recursive_mutex socketsMutex_;
            std::vector<::nevonex::web::server::MessageQueueWebsocket *> sockets_;
            /*PROTECTED REGION END*/

        public:
            static std::shared_ptr< WebSocketEndPoint > getInstance();

            void onWebSocketMessage(const std::string &message);
            void publishMessage(const std::string &message);
            virtual void disconnect() override;

#ifdef ENABLE_RAPID_JSON_API
        void onWebSocketJsonMessage(rapidjson::Document &outputJsonObject);
        void publishMessage(const rapidjson::Document &jsonMessage);
    #else // ENABLE_RAPID_JSON_API        
            void onWebSocketJsonMessage(Json::Value &jsonMessage);
            void publishMessage(const Json::Value &jsonMessage);
#endif // ENABLE_RAPID_JSON_API   

            void onDisconnect(const std::string &message);

            virtual ::nevonex::web::server::WebSocketRoute* createWebsocketRoute(
                    const Poco::Net::HTTPServerRequest &request) override;
        protected:
            static std::shared_ptr< WebSocketEndPoint > s_holder;
        private:
            ::nevonex::web::server::MessageQueueWebsocket *websocket;

        protected:
            WebSocketEndPoint_ptr _this()
            {
                return WebSocketEndPoint_ptr(this);
            }

        private:
            // Attributes

            // References

            ::AppMain::MainController_ptr m_mainController;

            /*PROTECTED REGION ID(WebSocketEndPoint_privateSection) ENABLED START*/
            // m_sessionStore / m_snapshot / m_rddfStagingManager 가 있던 자리입니다.
            // m_sessionStore, m_snapshot and m_rddfStagingManager were here.
            /*PROTECTED REGION END*/
        };

    } // web
} // AppMain

#endif // APPMAIN_WEB_WEBSOCKETENDPOINT_HPP
