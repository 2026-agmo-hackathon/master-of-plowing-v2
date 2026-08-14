/*
 * nevonex/fcb_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCB_FORWARD_HPP
#define _NEVONEX_FCB_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_fcb_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace fcb
    {

// EDataType

// EClass

// FCALController
        class FCALController;
        using FCALController_ptr = ::ecore::Ptr<FCALController>;

// ConnectionFactory
        class ConnectionFactory;
        using ConnectionFactory_ptr = ::ecore::Ptr<ConnectionFactory>;

// PublishConnectionFactory
        class PublishConnectionFactory;
        using PublishConnectionFactory_ptr = ::ecore::Ptr<PublishConnectionFactory>;

// SubscribeConnectionFactory
        class SubscribeConnectionFactory;
        using SubscribeConnectionFactory_ptr = ::ecore::Ptr<SubscribeConnectionFactory>;

// EEnum

// SubscriberEnum
        enum class SubscriberEnum
        ;

// Package & Factory
        class FcbFactory;
        using FcbFactory_ptr = ::ecore::Ptr<FcbFactory>;
        class FcbPackage;
        using FcbPackage_ptr = ::ecore::Ptr<FcbPackage>;

    } // fcb
} // nevonex

#endif // _NEVONEX_FCB_FORWARD_HPP
