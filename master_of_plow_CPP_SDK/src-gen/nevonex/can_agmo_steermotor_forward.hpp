/*
 * nevonex/can_agmo_steermotor_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_STEERMOTOR_FORWARD_HPP
#define _NEVONEX_CAN_AGMO_STEERMOTOR_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_can_agmo_steermotor_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace can_agmo_steermotor
    {

// EDataType

// EClass

// CAN_AGMO_SteerMotor
        class CAN_AGMO_SteerMotor;
        using CAN_AGMO_SteerMotor_ptr = ::ecore::Ptr<CAN_AGMO_SteerMotor>;

// CAN_AGMO_SteerMotorProvider
        class CAN_AGMO_SteerMotorProvider;
        using CAN_AGMO_SteerMotorProvider_ptr = ::ecore::Ptr<CAN_AGMO_SteerMotorProvider>;

// EEnum

// Package & Factory
        class Can_agmo_steermotorFactory;
        using Can_agmo_steermotorFactory_ptr = ::ecore::Ptr<Can_agmo_steermotorFactory>;
        class Can_agmo_steermotorPackage;
        using Can_agmo_steermotorPackage_ptr = ::ecore::Ptr<Can_agmo_steermotorPackage>;

    } // can_agmo_steermotor
} // nevonex

#endif // _NEVONEX_CAN_AGMO_STEERMOTOR_FORWARD_HPP
