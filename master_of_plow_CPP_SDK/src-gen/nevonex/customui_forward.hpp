/*
 * nevonex/customui_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CUSTOMUI_FORWARD_HPP
#define _NEVONEX_CUSTOMUI_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_customui_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace customui
    {

// EDataType

// EClass

// AbstractWebsocketEndPoint
        class AbstractWebsocketEndPoint;
        using AbstractWebsocketEndPoint_ptr = ::ecore::Ptr<AbstractWebsocketEndPoint>;

// NevonexRoute
        class NevonexRoute;
        using NevonexRoute_ptr = ::ecore::Ptr<NevonexRoute>;

// UIWebServiceProvider
        class UIWebServiceProvider;
        using UIWebServiceProvider_ptr = ::ecore::Ptr<UIWebServiceProvider>;

// EEnum

// PortEnum
        enum class PortEnum
        ;

// Package & Factory
        class CustomuiFactory;
        using CustomuiFactory_ptr = ::ecore::Ptr<CustomuiFactory>;
        class CustomuiPackage;
        using CustomuiPackage_ptr = ::ecore::Ptr<CustomuiPackage>;

    } // customui
} // nevonex

#endif // _NEVONEX_CUSTOMUI_FORWARD_HPP
