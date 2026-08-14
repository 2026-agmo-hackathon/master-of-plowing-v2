/*
 * nevonex/types_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_TYPES_FORWARD_HPP
#define _NEVONEX_TYPES_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_types_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes
#include <nevonex/propertychange/PropertyChangeListener.hpp>
#include <nevonex/machine/InterfaceDetails.hpp>
#include <boost/filesystem.hpp>

namespace nevonex
{
    namespace types
    {

// EDataType
        using PropertyChangeListener = ::nevonex::propertychange::PropertyChangeListener*;
        using InterfaceDetails = ::nevonex::machine::InterfaceDetails;
        using Date = int;
        using File = boost::filesystem::path;
        using StringArray = std::vector< ::ecore::EString >;
        using ObjectArray = std::vector< ::ecore::EJavaObject >;
        using IntArray = std::vector< ::ecore::EInt >;
        using FloatArray = std::vector< ::ecore::EFloat >;
        using DoubleArray = std::vector< ::ecore::EDouble >;
        using LongArray = std::vector< ::ecore::ELong >;
        using BooleanArray = std::vector< ::ecore::EBoolean >;

// EClass

// PropertyChange
        class PropertyChange;
        using PropertyChange_ptr = ::ecore::Ptr<PropertyChange>;

// IMachine
        class IMachine;
        using IMachine_ptr = ::ecore::Ptr<IMachine>;

// IMachineProvider
        class IMachineProvider;
        using IMachineProvider_ptr = ::ecore::Ptr<IMachineProvider>;

// EEnum

// Package & Factory
        class TypesFactory;
        using TypesFactory_ptr = ::ecore::Ptr<TypesFactory>;
        class TypesPackage;
        using TypesPackage_ptr = ::ecore::Ptr<TypesPackage>;

    } // types
} // nevonex

#endif // _NEVONEX_TYPES_FORWARD_HPP
