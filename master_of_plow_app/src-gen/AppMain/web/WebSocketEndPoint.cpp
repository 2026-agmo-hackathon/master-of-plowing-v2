/*
 * AppMain/web/WebSocketEndPoint.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "WebSocketEndPoint.hpp"
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

/*PROTECTED REGION ID(WebSocketEndPoint.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain::web;
using namespace ::nevonex::log;

// Default constructor
WebSocketEndPoint::WebSocketEndPoint() : m_mainController(0)
{

    websocket = nullptr;

    /*PROTECTED REGION ID(WebSocketEndPoint__WebSocketEndPoint) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

WebSocketEndPoint::~WebSocketEndPoint()
{

    /*PROTECTED REGION ID(WebSocketEndPoint__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References

::AppMain::MainController_ptr WebSocketEndPoint::getMainController() const
{
    return m_mainController;
}

void WebSocketEndPoint::setMainController(
        ::AppMain::MainController_ptr _mainController)
{

    m_mainController = _mainController;

}

