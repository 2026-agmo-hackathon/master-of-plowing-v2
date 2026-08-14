/*
 * nevonex/exception/ExceptionFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/exception/ExceptionFactory.hpp>
#include <nevonex/exception/ExceptionPackage.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::exception;

ExceptionFactory::ExceptionFactory()
{
}

::ecore::EObject_ptr ExceptionFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject ExceptionFactory::createFromString(
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

::ecore::EString ExceptionFactory::convertToString(
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

