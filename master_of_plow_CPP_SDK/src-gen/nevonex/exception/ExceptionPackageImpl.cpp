/*
 * nevonex/exception/ExceptionPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/exception/ExceptionPackage.hpp>
#include <nevonex/exception/ExceptionFactory.hpp>
#include <ecore.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EAttribute.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecore/EParameter.hpp>
#include <ecore/EEnum.hpp>
#include <ecore/EEnumLiteral.hpp>
#include <ecore/EDataType.hpp>
#include <ecore/EGenericType.hpp>
#include <ecore/ETypeParameter.hpp>
#include <ecore/EcorePackage.hpp>

using namespace ::nevonex::exception;

ExceptionPackage::ExceptionPackage()
{

}

void ExceptionPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = ExceptionFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // Create enums

    // Create data types

    // Initialize package
    setName("exception");
    setNsPrefix("exception");
    setNsURI("com.bosch.nevonex.exception");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes

    // TODO: Initialize data types

    _initialize();
}


