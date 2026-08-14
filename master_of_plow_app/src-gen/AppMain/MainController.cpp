/*
 * AppMain/MainController.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MainController.hpp"
#include <AppMain/ApplicationInputData.hpp>
#include <AppMain/IController.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>
#include <nevonex/isopgn/ISOPGNProvider.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(MainController.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

// Default constructor
MainController::MainController()
{

    /*PROTECTED REGION ID(MainController__MainController) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

MainController::~MainController()
{
    // Close admission and wait for every launch caller to finish publishing its
    // managed future before touching any member used by orchestration.
    orchestrationLifecycle_.closeAndWait();
    orchestrationShutdown_=true;
    orchestrationFinishRequested_=true;
    setRunState(::AppMain::RunState::Idle);
    releaseMachineOutputs();
    std::shared_ptr< ::AppMain::orchestration::RunOrchestrator > orchestrator;
    {
        std::lock_guard<std::mutex> lock(orchestrationMutex_);
        orchestrationTasks_.clear();
        orchestrator=orchestrator_;
    }
    if(orchestrator) orchestrator->requestShutdown();
    std::future<void> worker;
    { std::lock_guard<std::mutex> lock(orchestrationMutex_);
      worker=std::move(orchestrationWorker_); }
    if(worker.valid()) {
        try {
            worker.get();
        } catch(...) {
            // Destructors must not throw; the feature is already fail-safe Idle.
        }
    }
    /*PROTECTED REGION ID(MainController__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References
