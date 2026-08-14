/*
 * nevonex/common_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_COMMON_FORWARD_HPP
#define _NEVONEX_COMMON_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_common_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace common
    {

// EDataType

// EClass

// AbsolutePosition
        class AbsolutePosition;
        using AbsolutePosition_ptr = ::ecore::Ptr<AbsolutePosition>;

// TopicObject
        class TopicObject;
        using TopicObject_ptr = ::ecore::Ptr<TopicObject>;

// EEnum

// ProviderEnum
        enum class ProviderEnum
        ;

// ConnectionTypeEnum
        enum class ConnectionTypeEnum
        ;

// PlatformServicesEnum
        enum class PlatformServicesEnum
        ;

// HMIServicesEnum
        enum class HMIServicesEnum
        ;

// Package & Factory
        class CommonFactory;
        using CommonFactory_ptr = ::ecore::Ptr<CommonFactory>;
        class CommonPackage;
        using CommonPackage_ptr = ::ecore::Ptr<CommonPackage>;

    } // common
} // nevonex

#endif // _NEVONEX_COMMON_FORWARD_HPP
