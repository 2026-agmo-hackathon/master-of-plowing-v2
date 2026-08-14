/*
 * nevonex/types/TypesPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/types/TypesPackage.hpp>
#include <nevonex/types/TypesFactory.hpp>
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
#include <nevonex/fcb/FcbPackage.hpp>

using namespace ::nevonex::types;

TypesPackage::TypesPackage()
{

    // Feature definitions of PropertyChange
    m_PropertyChange__listeners = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of IMachineProvider
    m_IMachineProvider__controller = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void TypesPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = TypesFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // PropertyChange
    {
        m_PropertyChangeEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_PropertyChangeEClass->setClassifierID(PROPERTYCHANGE);
        m_PropertyChangeEClass->setEPackage(_this());
        getEClassifiers().push_back(m_PropertyChangeEClass);
    }
    // m_PropertyChange__listeners has already been allocated above
    m_PropertyChange__listeners->setFeatureID(
            ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS);
    m_PropertyChangeEClass->getEStructuralFeatures().push_back(
            m_PropertyChange__listeners);

    // IMachine
    {
        m_IMachineEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_IMachineEClass->setClassifierID(IMACHINE);
        m_IMachineEClass->setEPackage(_this());
        getEClassifiers().push_back(m_IMachineEClass);
    }

    // IMachineProvider
    {
        m_IMachineProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_IMachineProviderEClass->setClassifierID(IMACHINEPROVIDER);
        m_IMachineProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_IMachineProviderEClass);
    }
    // m_IMachineProvider__controller has already been allocated above
    m_IMachineProvider__controller->setFeatureID(
            ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER);
    m_IMachineProviderEClass->getEStructuralFeatures().push_back(
            m_IMachineProvider__controller);

    // Create enums

    // Create data types

    {
        m_FileEDataType = ::ecore::Ptr < ::ecore::EDataType
                > (new ::ecore::EDataType);
        m_FileEDataType->setClassifierID(FILE);
        m_FileEDataType->setEPackage(_this());
        getEClassifiers().push_back(m_FileEDataType);
    }

    // Initialize package
    setName("types");
    setNsPrefix("types");
    setNsURI("com.bosch.nevonex.types");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // PropertyChange
    m_PropertyChangeEClass->setName("PropertyChange");
    m_PropertyChangeEClass->setAbstract(false);
    m_PropertyChangeEClass->setInterface(false);
    m_PropertyChange__listeners->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEJavaObject());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_PropertyChange__listeners),
            "listeners", true, false, false, "nullptr", false, false, true, true, 0,
            1, false, false);
    m_PropertyChange__listeners->setID(false);
    // IMachine
    m_IMachineEClass->setName("IMachine");
    m_IMachineEClass->setAbstract(false);
    m_IMachineEClass->setInterface(false);
    // IMachineProvider
    m_IMachineProviderEClass->setName("IMachineProvider");
    m_IMachineProviderEClass->setAbstract(false);
    m_IMachineProviderEClass->setInterface(false);
    m_IMachineProvider__controller->setEType(
            dynamic_cast< ::nevonex::fcb::FcbPackage* >(::nevonex::fcb::FcbPackage::_instance().get())->getFCALController());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_IMachineProvider__controller),
            "controller", false, false, false, "", false, false, true, true, 0,
            1, false, false);

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_PropertyChangeEClass, ::ecore::EClassifier_ptr(),
                "addPropertyChangeListener", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_listener", 0, 1, true, true);
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_PropertyChangeEClass, ::ecore::EClassifier_ptr(),
                "removePropertyChangeListener", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_listener", 0, 1, true, true);
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_PropertyChangeEClass, ::ecore::EClassifier_ptr(),
                "notifyPropertyChange", 0, 1, true, true);
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "_name", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_oldValue", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_newValue", 0, 1, true, true);
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_IMachineProviderEClass, ::ecore::EClassifier_ptr(),
                "createMachines", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_stream", 0, 1, true, true);
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_IMachineProviderEClass, ::ecore::EClassifier_ptr(),
                "constructChildTypes", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_parent", 0, 1, true, true);
        addEParameter(_op, ::ecore::EClassifier_ptr(), "_parentObj", 0, 1, true, true);
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "_path", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_IMachineProviderEClass, ::ecore::EClassifier_ptr(),
                "initMachineProvider", 0, 1, true, true);
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_IMachineProviderEClass, ::ecore::EClassifier_ptr(),
                "getTopicElement", 0, 1, true, true);
        _op->setEType(
                dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEJavaObject());
        {
            ::ecore::EParameter_ptr _pa = addEParameter(_op, ::ecore::EClassifier_ptr(), "_index", 0, 1, true, true);
            _pa->setEType(
                    dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEString());
        }
    }

    {
        ::ecore::EOperation_ptr _op = addEOperation(m_IMachineProviderEClass, ::ecore::EClassifier_ptr(),
                "stopMachineProvider", 0, 1, true, true);
    }

    // TODO: Initialize data types

    // File
    m_FileEDataType->setName("File");
    m_FileEDataType->setInstanceClassName("java.io.File");

    _initialize();
}

::ecore::EClass_ptr TypesPackage::getPropertyChange()
{
    return m_PropertyChangeEClass;
}
::ecore::EClass_ptr TypesPackage::getIMachine()
{
    return m_IMachineEClass;
}
::ecore::EClass_ptr TypesPackage::getIMachineProvider()
{
    return m_IMachineProviderEClass;
}
::ecore::EDataType_ptr TypesPackage::getFile()
{
    return m_FileEDataType;
}

::ecore::EAttribute_ptr TypesPackage::getPropertyChange__listeners()
{
    return m_PropertyChange__listeners;
}
::ecore::EReference_ptr TypesPackage::getIMachineProvider__controller()
{
    return m_IMachineProvider__controller;
}
