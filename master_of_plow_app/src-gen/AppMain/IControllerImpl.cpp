/*
 * AppMain/IControllerImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "IController.hpp"
#include <stdexcept>

#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(IControllerImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/exception/diagnostic_information.hpp>

using namespace ::AppMain;
using namespace ::nevonex::log;

/*PROTECTED REGION ID(IControllerImpl_Methods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
ProcessTimer::ProcessTimer(IController_ptr controller,
        const int intervalInMilliSecond)
{
    if (controller == nullptr)
    {
        NEVONEX_LOG(SeverityLevel::error)
                << "Controller is null. ProcessTimer has not started";
        return;
    }
    std::thread(
            [controller, intervalInMilliSecond]
            {
                int interval = intervalInMilliSecond;
                while (true)
                {
                    controller->start(interval);
                    interval = controller->nextProcessIntervalMs(
                            intervalInMilliSecond);
                    std::this_thread::sleep_for(
                            std::chrono::milliseconds(interval));
                }
            }).detach();
}

void IController::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(IControllerImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

/*PROTECTED REGION ID(IControllerImpl_MethodsEnd) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
