/*
 * nevonex/exception_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_EXCEPTION_FORWARD_HPP
#define _NEVONEX_EXCEPTION_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_exception_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace exception
    {

// EDataType

// EClass

// EEnum

// Package & Factory
        class ExceptionFactory;
        using ExceptionFactory_ptr = ::ecore::Ptr<ExceptionFactory>;
        class ExceptionPackage;
        using ExceptionPackage_ptr = ::ecore::Ptr<ExceptionPackage>;

    } // exception
} // nevonex

#endif // _NEVONEX_EXCEPTION_FORWARD_HPP
