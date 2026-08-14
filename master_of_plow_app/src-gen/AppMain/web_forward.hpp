/*
 * AppMain/web_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _APPMAIN_WEB_FORWARD_HPP
#define _APPMAIN_WEB_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <nevonex/propertychange/PropertyChangeListener.hpp>
#include <nevonex/machine/InterfaceDetails.hpp>

/*PROTECTED REGION ID(AppMain_web_forward) ENABLED START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace AppMain
{
    namespace web
    {

// EDataType

// EClass

// SampleRoute
        class SampleRoute;
        using SampleRoute_ptr = ::ecore::Ptr<SampleRoute>;

// SampleRouteFactory
        class SampleRouteFactory;
        using SampleRouteFactory_ptr = ::ecore::Ptr<SampleRouteFactory>;

// WebSocketEndPoint
        class WebSocketEndPoint;
        using WebSocketEndPoint_ptr = ::ecore::Ptr<WebSocketEndPoint>;

// EEnum

    }// web
} // AppMain

#endif // _APPMAIN_WEB_FORWARD_HPP
