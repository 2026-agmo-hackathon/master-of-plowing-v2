/*
 * nevonex/fcb/FcbPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcb/FcbPackage.hpp>
#include <nevonex/fcb/FcbFactory.hpp>
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

using namespace ::nevonex::fcb;

FcbPackage::FcbPackage()
{

}

void FcbPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = FcbFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // FCALController
    {
        m_FCALControllerEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_FCALControllerEClass->setClassifierID(FCALCONTROLLER);
        m_FCALControllerEClass->setEPackage(_this());
        getEClassifiers().push_back(m_FCALControllerEClass);
    }

    // ConnectionFactory
    {
        m_ConnectionFactoryEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ConnectionFactoryEClass->setClassifierID(CONNECTIONFACTORY);
        m_ConnectionFactoryEClass->setEPackage(_this());
        getEClassifiers().push_back(m_ConnectionFactoryEClass);
    }

    // PublishConnectionFactory
    {
        m_PublishConnectionFactoryEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_PublishConnectionFactoryEClass->setClassifierID(PUBLISHCONNECTIONFACTORY);
        m_PublishConnectionFactoryEClass->setEPackage(_this());
        getEClassifiers().push_back(m_PublishConnectionFactoryEClass);
    }

    // SubscribeConnectionFactory
    {
        m_SubscribeConnectionFactoryEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_SubscribeConnectionFactoryEClass->setClassifierID(SUBSCRIBECONNECTIONFACTORY);
        m_SubscribeConnectionFactoryEClass->setEPackage(_this());
        getEClassifiers().push_back(m_SubscribeConnectionFactoryEClass);
    }

    // Create enums

    {
        m_SubscriberEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_SubscriberEnumEEnum->setClassifierID(SUBSCRIBERENUM);
        m_SubscriberEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_SubscriberEnumEEnum);
    }

    // Create data types

    // Initialize package
    setName("fcb");
    setNsPrefix("fcb");
    setNsURI("com.bosch.nevonex.fcb");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // FCALController
    m_FCALControllerEClass->setName("FCALController");
    m_FCALControllerEClass->setAbstract(false);
    m_FCALControllerEClass->setInterface(false);
    // ConnectionFactory
    m_ConnectionFactoryEClass->setName("ConnectionFactory");
    m_ConnectionFactoryEClass->setAbstract(false);
    m_ConnectionFactoryEClass->setInterface(false);
    // PublishConnectionFactory
    m_PublishConnectionFactoryEClass->setName("PublishConnectionFactory");
    m_PublishConnectionFactoryEClass->setAbstract(false);
    m_PublishConnectionFactoryEClass->setInterface(false);
    // SubscribeConnectionFactory
    m_SubscribeConnectionFactoryEClass->setName("SubscribeConnectionFactory");
    m_SubscribeConnectionFactoryEClass->setAbstract(false);
    m_SubscribeConnectionFactoryEClass->setInterface(false);

    // TODO: Initialize data types

    // SubscriberEnum
    m_SubscriberEnumEEnum->setName("SubscriberEnum");
    m_SubscriberEnumEEnum->setSerializable(true);

    {
        // FIL
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_SubscriberEnumEEnum, "FIL", 0, "FIL");
    }

    {
        // FGF
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_SubscriberEnumEEnum, "FGF", 1, "FGF");
    }

    {
        // ALL
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_SubscriberEnumEEnum, "ALL", 2, "ALL");
    }

    _initialize();
}

::ecore::EClass_ptr FcbPackage::getFCALController()
{
    return m_FCALControllerEClass;
}
::ecore::EClass_ptr FcbPackage::getConnectionFactory()
{
    return m_ConnectionFactoryEClass;
}
::ecore::EClass_ptr FcbPackage::getPublishConnectionFactory()
{
    return m_PublishConnectionFactoryEClass;
}
::ecore::EClass_ptr FcbPackage::getSubscribeConnectionFactory()
{
    return m_SubscribeConnectionFactoryEClass;
}
::ecore::EEnum_ptr FcbPackage::getSubscriberEnum()
{
    return m_SubscriberEnumEEnum;
}

