/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.hpp>
#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_Tractor.hpp>
#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::can_agmo_customized_tractor;

Can_agmo_customized_tractorFactory::Can_agmo_customized_tractorFactory()
{
}

::ecore::EObject_ptr Can_agmo_customized_tractorFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR:
        return createCAN_AGMO_Customized_Tractor();
    case Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER:
        return createCAN_AGMO_Customized_TractorProvider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject Can_agmo_customized_tractorFactory::createFromString(
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

::ecore::EString Can_agmo_customized_tractorFactory::convertToString(
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

CAN_AGMO_Customized_Tractor_ptr Can_agmo_customized_tractorFactory::createCAN_AGMO_Customized_Tractor()
{
    return ::ecore::Ptr < CAN_AGMO_Customized_Tractor > (new CAN_AGMO_Customized_Tractor);
}
CAN_AGMO_Customized_TractorProvider_ptr Can_agmo_customized_tractorFactory::createCAN_AGMO_Customized_TractorProvider()
{
    return ::ecore::Ptr < CAN_AGMO_Customized_TractorProvider > (new CAN_AGMO_Customized_TractorProvider);
}
