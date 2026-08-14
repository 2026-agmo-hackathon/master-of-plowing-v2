/*
 * AppMain/ApplicationMainImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ApplicationMain.hpp"
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#include <AppMain/BuildInfo.hpp>
#include <AppMain/MainController.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ApplicationMainImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/lexical_cast.hpp>

#include <ecore/EAttribute.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex.hpp>
#include <nevonex/common/ProviderEnum.hpp>
#include <nevonex/exception/CommunicationException.hpp>
#include <nevonex/exception/NotInitializedException.hpp>

#include <AppMain/MachineConnectListener.hpp>    

#include "web/SampleRouteFactory.hpp"
#include "web/WebSocketEndPoint.hpp"
#include <nevonex/customui/UIWebServiceProvider.hpp>

#ifdef SEAMOS_DEV_LOCAL_SIM
#include <mqtt/async_client.h>
#include <memory>
#include <string>
#endif

#include "CloudDownloadListener.hpp"
#include <nevonex/cloud/Cloud.hpp>

/*PROTECTED REGION ID(ApplicationMainImpl_Headers) ENABLED START*/

/*PROTECTED REGION END*/

using namespace ::ecore;
using namespace ::ecorecpp::mapping;

using namespace ::nevonex;
using namespace ::nevonex::fcal;
using namespace ::nevonex::common;
using namespace ::nevonex::types;
using namespace ::nevonex::can_agmo_customized_tractor;
using namespace ::nevonex::can_agmo_mtlt305;
using namespace ::nevonex::can_agmo_steermotor;
using namespace ::nevonex::isopgn;
using namespace ::nevonex::serial_ext_gps_nmea0183;

/*PROTECTED REGION ID(ApplicationMainImpl_AddnlMethods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

#ifdef SEAMOS_DEV_LOCAL_SIM
namespace {
class TestSimMqttBridge : public virtual mqtt::callback
{
public:
    TestSimMqttBridge(const std::string &serverUri, const std::string &clientId) :
            m_client(serverUri, clientId)
    {
        m_client.set_callback(*this);
    }

    bool start()
    {
        try
        {
            mqtt::connect_options opts;
            opts.set_clean_session(true);
            opts.set_automatic_reconnect(true);
            m_client.connect(opts)->wait();
            const std::string topicFilter = std::string("fek/") + "#";
            m_client.subscribe(topicFilter, 0)->wait();
            NEVONEX_LOG(::nevonex::log::SeverityLevel::info)
                    << "[MQTT bridge] connected + subscribed " << topicFilter;
            return true;
        } catch (const std::exception &e)
        {
            NEVONEX_LOG(::nevonex::log::SeverityLevel::warning)
                    << "[MQTT bridge] start failed: " << e.what();
            return false;
        }
    }

    void connection_lost(const std::string &cause) override
    {
        NEVONEX_LOG(::nevonex::log::SeverityLevel::warning)
                << "[MQTT bridge] connection lost: " << cause;
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        const std::string topic = msg->get_topic();
        const std::string payload = msg->to_string();

        std::string framed;
        framed.reserve(topic.size() + payload.size() + 32);
        framed.append("{\"topic\":\"").append(topic).append("\",\"payload\":")
                .append(payload).append("}");

        auto ws = ::AppMain::web::WebSocketEndPoint::getInstance();
        if (ws)
        {
            ws->publishMessage(framed);
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr /*token*/) override {}

private:
    mqtt::async_client m_client;
};

std::unique_ptr< TestSimMqttBridge > g_testSimBridge;
} // anonymous namespace
#endif

using namespace ::AppMain;
using namespace ::nevonex::log;

typedef std::vector< ::ecore::EObject_ptr > SEAMOS_PROVIDER_LIST;
extern "C" SEAMOS_PROVIDER_LIST & getProviders(void);

namespace {
void bootstrapLinkedMachineProviders(ApplicationMain *sa, const std::vector<std::string>& requestedProviders)
{
    if (!sa) return;
    try
    {
        SEAMOS_PROVIDER_LIST& linkedProviders = getProviders();
        for (auto& _obj : linkedProviders)
        {
            auto _provider = ::ecore::as < ::nevonex::feature::AbstractMachineProvider > (_obj);
            if (!_provider) continue;
            const std::string providerName = _provider->getProviderName();
            if (std::find(requestedProviders.begin(), requestedProviders.end(), providerName) == requestedProviders.end())
            {
                continue;
            }
            if (sa->getMachineProvider(providerName) != nullptr)
            {
                continue;
            }
            _provider->initMachineProvider();
            sa->onStart(_provider);
            _provider->start();
            NEVONEX_LOG(SeverityLevel::info) << "Bootstrapped linked machine provider: " << providerName;
        }
    } catch (...)
    {
        NEVONEX_LOG(SeverityLevel::warning) << "Linked machine provider bootstrap failed: "
                << boost::current_exception_diagnostic_information();
    }
}
} // anonymous namespace

/*PROTECTED REGION ID(ApplicationMainImpl_Methods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
int main(int argc, char** argv)
{
    /*PROTECTED REGION ID(ApplicationMain_Main) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    if (argc == 2 && std::strcmp(argv[1], "--build-info") == 0)
    {
        std::cout << ::AppMain::BUILD_ID << "\n";
        return 0;
    }

    std::cerr << "[BuildInfo] " << ::AppMain::BUILD_ID << "\n";

    try
    {
        ApplicationMain *sa = new ApplicationMain();
        // Calling runtime to start the application
        std::vector < std::string > vec;

        vec.push_back(
                common::getLiteral(ProviderEnum::CAN_AGMO_Customized_TractorProvider));

        vec.push_back(
                common::getLiteral(ProviderEnum::CAN_AGMO_MTLT305Provider));

        vec.push_back(
                common::getLiteral(ProviderEnum::CAN_AGMO_SteerMotorProvider));

        vec.push_back(
                common::getLiteral(ProviderEnum::ISOPGNProvider));

        vec.push_back(
                common::getLiteral(ProviderEnum::Serial_Ext_GPS_NMEA0183Provider));

        /*PROTECTED REGION ID(ApplicationMain_MainBeforeInitialize) ENABLED START*/

        /*PROTECTED REGION END*/

        sa->initialize(vec);
        bootstrapLinkedMachineProviders(sa, vec);

        sa->addCustomUIListener();

        sa->addCloudDownloadListener();

        /*PROTECTED REGION ID(ApplicationMain_MainBeforeRuntime) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

#ifdef SEAMOS_DEV_LOCAL_SIM
        sa->addProcessTimer();
        ::nevonex::customui::UIWebServiceProvider::getInstance()->start();
        g_testSimBridge.reset(new TestSimMqttBridge(
                "tcp://127.0.0.1:1883", "seamos_local_sim_bridge"));
        g_testSimBridge->start();
#endif

        /*PROTECTED REGION ID(ApplicationMain_MainAfterRuntime) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        sa->start();
    } catch (const std::exception &ex)
    {
        NEVONEX_LOG(SeverityLevel::fatal) << boost::diagnostic_information(ex);
    }
}

void ApplicationMain::onStart(::nevonex::feature::AbstractMachine_ptr machine)
{
    /*PROTECTED REGION ID(Machine_onStart_First) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    if (CAN_AGMO_Customized_Tractor_ptr cAN_AGMO_Customized_Tractor =
            ::ecore::as < CAN_AGMO_Customized_Tractor > (machine))
    {
        MachineConnectListener *machineListener = new MachineConnectListener();
        machineListener->setMainController(getMainController());
        cAN_AGMO_Customized_Tractor->addListeners(machineListener);

        // Machine has started. Check for Type and Index.
        /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProviderCAN_AGMO_Customized_Tractor_onStart) ENABLED START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        NEVONEX_LOG(SeverityLevel::info)
                << "DOM received for Machine CAN_AGMO_Customized_Tractor index:"
                << cAN_AGMO_Customized_Tractor->getIndex();
        /*PROTECTED REGION END*/
    }

    else if (CAN_AGMO_MTLT305_ptr cAN_AGMO_MTLT305 =
            ::ecore::as < CAN_AGMO_MTLT305 > (machine))
    {
        MachineConnectListener *machineListener = new MachineConnectListener();
        machineListener->setMainController(getMainController());
        cAN_AGMO_MTLT305->addListeners(machineListener);

        // Machine has started. Check for Type and Index.
        /*PROTECTED REGION ID(CAN_AGMO_MTLT305ProviderCAN_AGMO_MTLT305_onStart) ENABLED START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        NEVONEX_LOG(SeverityLevel::info)
                << "DOM received for Machine CAN_AGMO_MTLT305 index:"
                << cAN_AGMO_MTLT305->getIndex();
        /*PROTECTED REGION END*/
    }

    else if (CAN_AGMO_SteerMotor_ptr cAN_AGMO_SteerMotor =
            ::ecore::as < CAN_AGMO_SteerMotor > (machine))
    {
        MachineConnectListener *machineListener = new MachineConnectListener();
        machineListener->setMainController(getMainController());
        cAN_AGMO_SteerMotor->addListeners(machineListener);

        // Machine has started. Check for Type and Index.
        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProviderCAN_AGMO_SteerMotor_onStart) ENABLED START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        NEVONEX_LOG(SeverityLevel::info)
                << "DOM received for Machine CAN_AGMO_SteerMotor index:"
                << cAN_AGMO_SteerMotor->getIndex();
        /*PROTECTED REGION END*/
    }

    else if (ISOPGN_ptr iSOPGN =
            ::ecore::as < ISOPGN > (machine))
    {
        MachineConnectListener *machineListener = new MachineConnectListener();
        machineListener->setMainController(getMainController());
        iSOPGN->addListeners(machineListener);

        // Machine has started. Check for Type and Index.
        /*PROTECTED REGION ID(ISOPGNProviderISOPGN_onStart) ENABLED START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        NEVONEX_LOG(SeverityLevel::info)
                << "DOM received for Machine ISOPGN index:"
                << iSOPGN->getIndex();
        /*PROTECTED REGION END*/
    }

    else if (Serial_Ext_GPS_NMEA0183_ptr serial_Ext_GPS_NMEA0183 =
            ::ecore::as < Serial_Ext_GPS_NMEA0183 > (machine))
    {
        MachineConnectListener *machineListener = new MachineConnectListener();
        machineListener->setMainController(getMainController());
        serial_Ext_GPS_NMEA0183->addListeners(machineListener);

        // Machine has started. Check for Type and Index.
        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183ProviderSerial_Ext_GPS_NMEA0183_onStart) ENABLED START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        NEVONEX_LOG(SeverityLevel::info)
                << "DOM received for Machine Serial_Ext_GPS_NMEA0183 index:"
                << serial_Ext_GPS_NMEA0183->getIndex();
        /*PROTECTED REGION END*/
    }

    /*PROTECTED REGION ID(Machine_onStart_AdditionalMethods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

bool ApplicationMain::onStart(
        ::nevonex::feature::AbstractMachineProvider_ptr provider)
{
    /*PROTECTED REGION ID(Provider_onStart_First) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    if (CAN_AGMO_Customized_TractorProvider_ptr cAN_AGMO_Customized_TractorProvider =
            ::ecore::as < CAN_AGMO_Customized_TractorProvider > (provider))
    {
        m_mainController->setCAN_AGMO_Customized_TractorProvider(
                cAN_AGMO_Customized_TractorProvider);
        /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider_onStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    }

    else if (CAN_AGMO_MTLT305Provider_ptr cAN_AGMO_MTLT305Provider =
            ::ecore::as < CAN_AGMO_MTLT305Provider > (provider))
    {
        m_mainController->setCAN_AGMO_MTLT305Provider(
                cAN_AGMO_MTLT305Provider);
        /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider_onStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    }

    else if (CAN_AGMO_SteerMotorProvider_ptr cAN_AGMO_SteerMotorProvider =
            ::ecore::as < CAN_AGMO_SteerMotorProvider > (provider))
    {
        m_mainController->setCAN_AGMO_SteerMotorProvider(
                cAN_AGMO_SteerMotorProvider);
        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider_onStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    }

    else if (ISOPGNProvider_ptr iSOPGNProvider =
            ::ecore::as < ISOPGNProvider > (provider))
    {
        m_mainController->setISOPGNProvider(
                iSOPGNProvider);
        /*PROTECTED REGION ID(ISOPGNProvider_onStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    }

    else if (Serial_Ext_GPS_NMEA0183Provider_ptr serial_Ext_GPS_NMEA0183Provider =
            ::ecore::as < Serial_Ext_GPS_NMEA0183Provider > (provider))
    {
        m_mainController->setSerial_Ext_GPS_NMEA0183Provider(
                serial_Ext_GPS_NMEA0183Provider);
        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Provider_onStart) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    }

    /*PROTECTED REGION ID(Provider_onStart_AdditionalMethods) ENABLED START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    addProcessTimer();

    /*PROTECTED REGION END*/

    return true;
}

void ApplicationMain::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ApplicationMainImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

void ApplicationMain::addProcessTimer()
{

    static bool atleastOneMachineCreated = false;
    if (!atleastOneMachineCreated)
    {
        ProcessTimer(m_mainController, 100);
        atleastOneMachineCreated = true;
    }
    else
    {
        NEVONEX_LOG(SeverityLevel::warning) << "Timer already created.";
    }

}

void ApplicationMain::addCustomUIListener()
{

    using namespace ::AppMain::web;

    /*PROTECTED REGION ID(AppMainImpl_addCustomUIListener) ENABLED START*/
    // ┌─ 한국어 ───────────────────────────────────────────────────────────────┐
    // 남은 것은 WebSocket 전송 계층 하나뿐입니다. 앞으로 대시보드가 자율주행 중
    // 관측값을 받아 볼 통로이므로 등록은 그대로 유지합니다.
    //
    // 걷어낸 것:
    //   - "/helloworld" REST 라우트 (SampleRoute) — 쓰이지 않던 샘플
    //   - LeaderboardClient 생성과 에러 콜백      — 외부 리더보드 서버
    //   - RddfStagingManager                     — RDDF는 이제 도착 즉시 적재
    // └────────────────────────────────────────────────────────────────────────┘
    // ┌─ English ──────────────────────────────────────────────────────────────┐
    // Only the WebSocket transport survives — it is how the observability
    // dashboard will receive live values, so the route stays registered.
    //
    // Removed: the unused "/helloworld" sample REST route, the LeaderboardClient
    // and its error callback (external server), and RddfStagingManager (RDDF is
    // now loaded on arrival instead of staged for UI approval).
    // └────────────────────────────────────────────────────────────────────────┘
    std::shared_ptr < WebSocketEndPoint > webSocketEndPointsSharedPtr =
            WebSocketEndPoint::getInstance();
    getMainController()->setWebSocketEndPoint(webSocketEndPointsSharedPtr);
    webSocketEndPointsSharedPtr->setMainController(getMainController());
    ::nevonex::customui::UIWebServiceProvider::getInstance()->registerWebsocketRoute(
            "/socket", webSocketEndPointsSharedPtr);
    /*PROTECTED REGION END*/

}

void ApplicationMain::addCloudDownloadListener()
{
    using namespace ::nevonex::cloud;
    CloudDownloadListener *cloudDownloadListener = new CloudDownloadListener();
    cloudDownloadListener->setMainController(getMainController());
    Cloud::getInstance()->addPropertyChangeListener(cloudDownloadListener);
    /*PROTECTED REGION ID(AppMainImpl_addCloudDownloadListener) ENABLED START*/
    // RddfStagingManager 주입이 있던 자리입니다. handleFile()이 도착한 .rddf를
    // 곧바로 적재하므로 더 이상 필요하지 않습니다.
    //
    // The RddfStagingManager injection was here; handleFile() now loads an
    // arriving .rddf directly, so there is nothing left to inject.
    /*PROTECTED REGION END*/
}

/*PROTECTED REGION ID(ApplicationMainImpl_MethodsEnd) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
