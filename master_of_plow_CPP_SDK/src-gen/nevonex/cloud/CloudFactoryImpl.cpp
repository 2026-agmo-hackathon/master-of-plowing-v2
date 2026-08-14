/*
 * nevonex/cloud/CloudFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include <nevonex/cloud/CloudFactory.hpp>
#include <nevonex/cloud/CloudPackage.hpp>
#include <nevonex/cloud/ICloudDownload.hpp>
#include <nevonex/cloud/Cloud.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::cloud;

CloudFactory::CloudFactory()
{
}

::ecore::EObject_ptr CloudFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case CloudPackage::CLOUD:
        return createCloud();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject CloudFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString CloudFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

Cloud_ptr CloudFactory::createCloud()
{
    return Cloud::getInstance();
}

