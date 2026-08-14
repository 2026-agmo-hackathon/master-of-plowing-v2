/*
 * nevonex/cloud_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#ifndef _NEVONEX_CLOUD_FORWARD_HPP
#define _NEVONEX_CLOUD_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <nevonex/propertychange/PropertyChangeListener.hpp>
#include <nevonex/machine/InterfaceDetails.hpp>

/*PROTECTED REGION ID(nevonex_cloud_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace cloud
    {

// EDataType

// EClass

// ICloudDownload
        class ICloudDownload;
        using ICloudDownload_ptr = ::ecore::Ptr<ICloudDownload>;

// Cloud
        class Cloud;
        using Cloud_ptr = ::ecore::Ptr<Cloud>;

// EEnum

// Package & Factory
        class CloudFactory;
        using CloudFactory_ptr = ::ecore::Ptr<CloudFactory>;
        class CloudPackage;
        using CloudPackage_ptr = ::ecore::Ptr<CloudPackage>;

    } // cloud
} // nevonex

#endif // _NEVONEX_CLOUD_FORWARD_HPP
