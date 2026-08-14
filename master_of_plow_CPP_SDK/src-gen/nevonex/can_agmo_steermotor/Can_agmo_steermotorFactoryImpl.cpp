/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.hpp>
#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::can_agmo_steermotor;

Can_agmo_steermotorFactory::Can_agmo_steermotorFactory()
{
}

::ecore::EObject_ptr Can_agmo_steermotorFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR:
        return createCAN_AGMO_SteerMotor();
    case Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER:
        return createCAN_AGMO_SteerMotorProvider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject Can_agmo_steermotorFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    (void)_literalValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString Can_agmo_steermotorFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    (void)_instanceValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

CAN_AGMO_SteerMotor_ptr Can_agmo_steermotorFactory::createCAN_AGMO_SteerMotor()
{
    return ::ecore::Ptr < CAN_AGMO_SteerMotor > (new CAN_AGMO_SteerMotor);
}
CAN_AGMO_SteerMotorProvider_ptr Can_agmo_steermotorFactory::createCAN_AGMO_SteerMotorProvider()
{
    return ::ecore::Ptr < CAN_AGMO_SteerMotorProvider > (new CAN_AGMO_SteerMotorProvider);
}
