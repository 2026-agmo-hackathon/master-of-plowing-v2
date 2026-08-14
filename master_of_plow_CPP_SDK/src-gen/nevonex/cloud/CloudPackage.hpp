/*
 * nevonex/cloud/CloudPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#ifndef _NEVONEX_CLOUDPACKAGE_HPP
#define _NEVONEX_CLOUDPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/cloud_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/types/TypesPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace cloud
    {

    class EXPORT_NEVONEX_DLL CloudPackage : public virtual ::ecore::EPackage
    {
    public:

        static CloudPackage_ptr _instance();
        static CloudPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int CLOUD = 0;

        static const int ICLOUDDOWNLOAD = 1;

        // IDs for classifiers for class Cloud 

        static const int CLOUD__LISTENERS = ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS;

        // IDs for classifiers for class ICloudDownload 

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getICloudDownload();

        virtual ::ecore::EClass_ptr getCloud();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getPropertyChange__listeners();

    protected:

        static ::ecore::Ptr< CloudPackage > s_instance;

        CloudPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_ICloudDownloadEClass;

        ::ecore::EClass_ptr m_CloudEClass;

        // EEnuminstances 

        // EDataType instances 

        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_PropertyChange__listeners;

    };

}
 // cloud
}// nevonex

#endif // _NEVONEX_CLOUDPACKAGE_HPP

