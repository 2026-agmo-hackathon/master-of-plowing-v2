/*
 * AppMain/MachineConnectListener.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "MachineConnectListener.hpp"
#include <AppMain/MainController.hpp>
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

/*PROTECTED REGION ID(MachineConnectListener.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

// Default constructor
MachineConnectListener::MachineConnectListener() : m_mainController(0)
{

    /*PROTECTED REGION ID(MachineConnectListener__MachineConnectListener) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

MachineConnectListener::~MachineConnectListener()
{

    /*PROTECTED REGION ID(MachineConnectListener__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References

::AppMain::MainController_ptr MachineConnectListener::getMainController() const
{
    return m_mainController;
}

void MachineConnectListener::setMainController(
        ::AppMain::MainController_ptr _mainController)
{

    m_mainController = _mainController;

}

