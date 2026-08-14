/*
 * AppMain/CloudDownloadListener.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CloudDownloadListener.hpp"
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

/*PROTECTED REGION ID(CloudDownloadListener.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

// Default constructor
CloudDownloadListener::CloudDownloadListener() : m_mainController(0)
{

    /*PROTECTED REGION ID(CloudDownloadListener__CloudDownloadListener) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

CloudDownloadListener::~CloudDownloadListener()
{

    /*PROTECTED REGION ID(CloudDownloadListener__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References

::AppMain::MainController_ptr CloudDownloadListener::getMainController() const
{
    return m_mainController;
}

void CloudDownloadListener::setMainController(
        ::AppMain::MainController_ptr _mainController)
{

    m_mainController = _mainController;

}

