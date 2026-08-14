/*
 * nevonex/cloud/CloudPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include <nevonex/cloud/CloudPackage.hpp>
#include <nevonex/cloud/CloudFactory.hpp>
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
#include <nevonex/types/TypesPackage.hpp>
#include <ecore/EcorePackage.hpp>
#include <nevonex/common/CommonPackage.hpp>

using namespace ::nevonex::cloud;

CloudPackage::CloudPackage()
{

    // Feature definitions of ICloudDownload

    // Feature definitions of Cloud

}

void CloudPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = CloudFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // ICloudDownload
    {
        m_ICloudDownloadEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ICloudDownloadEClass->setClassifierID(ICLOUDDOWNLOAD);
        m_ICloudDownloadEClass->setEPackage(_this());
        getEClassifiers().push_back(m_ICloudDownloadEClass);
    }

    // Cloud
    {
        m_CloudEClass = ::ecore::Ptr < ::ecore::EClass > (new ::ecore::EClass);
        m_CloudEClass->setClassifierID(CLOUD);
        m_CloudEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CloudEClass);
    }

    // Create enums

    // Create data types

    // Initialize package
    setName("cloud");
    setNsPrefix("cloud");
    setNsURI("com.bosch.nevonex.cloud");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_CloudEClass->getESuperTypes().push_back(m_ICloudDownloadEClass);
    m_CloudEClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getPropertyChange());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // ICloudDownload
    m_ICloudDownloadEClass->setName("ICloudDownload");
    m_ICloudDownloadEClass->setAbstract(true);
    m_ICloudDownloadEClass->setInterface(true);
    // Cloud
    m_CloudEClass->setName("Cloud");
    m_CloudEClass->setAbstract(false);
    m_CloudEClass->setInterface(false);
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "processDownloadMessage", 0, 1, true, true);
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "message", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "stopPlatformService", 0, 1, true, true);
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadData", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "data", 0, 1, true,
                    true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadData", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "data", 0, 1, true,
                    true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "connectionType",
                    0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getConnectionTypeEnum());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadFile", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "file", 0, 1, true,
                    true);
            _pa->setEType(
                    dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getFile());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadFile", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "filePath", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadFile", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "file", 0, 1, true,
                    true);
            _pa->setEType(
                    dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getFile());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "connectionType",
                    0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getConnectionTypeEnum());
        }
    }
    {
        ::ecore::EOperation_ptr _op = addEOperation(m_CloudEClass, nullptr,
                "uploadFile", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "filePath", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "priority", 0, 1,
                    true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
        }
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, "connectionType",
                    0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getConnectionTypeEnum());
        }
    }

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr CloudPackage::getICloudDownload()
{
    return m_ICloudDownloadEClass;
}
::ecore::EClass_ptr CloudPackage::getCloud()
{
    return m_CloudEClass;
}

::ecore::EAttribute_ptr CloudPackage::getPropertyChange__listeners()
{
    return m_PropertyChange__listeners;
}

