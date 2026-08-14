/*
 * nevonex/can_agmo_mtlt305/Can_agmo_mtlt305FactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.hpp>
#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::can_agmo_mtlt305;

Can_agmo_mtlt305Factory::Can_agmo_mtlt305Factory()
{
}

::ecore::EObject_ptr Can_agmo_mtlt305Factory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case Can_agmo_mtlt305Package::CAN_AGMO_MTLT305:
        return createCAN_AGMO_MTLT305();
    case Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER:
        return createCAN_AGMO_MTLT305Provider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject Can_agmo_mtlt305Factory::createFromString(
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

::ecore::EString Can_agmo_mtlt305Factory::convertToString(
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

CAN_AGMO_MTLT305_ptr Can_agmo_mtlt305Factory::createCAN_AGMO_MTLT305()
{
    return ::ecore::Ptr < CAN_AGMO_MTLT305 > (new CAN_AGMO_MTLT305);
}
CAN_AGMO_MTLT305Provider_ptr Can_agmo_mtlt305Factory::createCAN_AGMO_MTLT305Provider()
{
    return ::ecore::Ptr < CAN_AGMO_MTLT305Provider > (new CAN_AGMO_MTLT305Provider);
}
