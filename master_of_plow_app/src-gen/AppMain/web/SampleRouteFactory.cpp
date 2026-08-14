/*
 * AppMain/web/SampleRouteFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "SampleRouteFactory.hpp"
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

/*PROTECTED REGION ID(SampleRouteFactory.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain::web;
using namespace ::nevonex::log;

// Default constructor
SampleRouteFactory::SampleRouteFactory() : m_mainController(0)
{

    /*PROTECTED REGION ID(SampleRouteFactory__SampleRouteFactory) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

SampleRouteFactory::~SampleRouteFactory()
{

    /*PROTECTED REGION ID(SampleRouteFactory__Destructor) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

}

// Attributes

// References

::AppMain::MainController_ptr SampleRouteFactory::getMainController() const
{
    return m_mainController;
}

void SampleRouteFactory::setMainController(
        ::AppMain::MainController_ptr _mainController)
{

    m_mainController = _mainController;

}

