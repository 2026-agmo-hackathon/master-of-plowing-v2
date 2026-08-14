/*
 * nevonex/customui/CustomuiPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/customui/CustomuiPackage.hpp>
#include <nevonex/customui/CustomuiFactory.hpp>
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

using namespace ::nevonex::customui;

CustomuiPackage::CustomuiPackage()
{

}

void CustomuiPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = CustomuiFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // AbstractWebsocketEndPoint
    {
        m_AbstractWebsocketEndPointEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_AbstractWebsocketEndPointEClass->setClassifierID(ABSTRACTWEBSOCKETENDPOINT);
        m_AbstractWebsocketEndPointEClass->setEPackage(_this());
        getEClassifiers().push_back(m_AbstractWebsocketEndPointEClass);
    }

    // NevonexRoute
    {
        m_NevonexRouteEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_NevonexRouteEClass->setClassifierID(NEVONEXROUTE);
        m_NevonexRouteEClass->setEPackage(_this());
        getEClassifiers().push_back(m_NevonexRouteEClass);
    }

    // UIWebServiceProvider
    {
        m_UIWebServiceProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_UIWebServiceProviderEClass->setClassifierID(UIWEBSERVICEPROVIDER);
        m_UIWebServiceProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_UIWebServiceProviderEClass);
    }

    // Create enums

    {
        m_PortEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_PortEnumEEnum->setClassifierID(PORTENUM);
        m_PortEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_PortEnumEEnum);
    }

    // Create data types

    // Initialize package
    setName("customui");
    setNsPrefix("customui");
    setNsURI("com.bosch.nevonex.customui");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // AbstractWebsocketEndPoint
    m_AbstractWebsocketEndPointEClass->setName("AbstractWebsocketEndPoint");
    m_AbstractWebsocketEndPointEClass->setAbstract(false);
    m_AbstractWebsocketEndPointEClass->setInterface(false);
    // NevonexRoute
    m_NevonexRouteEClass->setName("NevonexRoute");
    m_NevonexRouteEClass->setAbstract(false);
    m_NevonexRouteEClass->setInterface(false);
    // UIWebServiceProvider
    m_UIWebServiceProviderEClass->setName("UIWebServiceProvider");
    m_UIWebServiceProviderEClass->setAbstract(false);
    m_UIWebServiceProviderEClass->setInterface(false);

    // TODO: Initialize data types

    // PortEnum
    m_PortEnumEEnum->setName("PortEnum");
    m_PortEnumEEnum->setSerializable(true);

    {
        // HTTP
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_PortEnumEEnum, "HTTP", 8080, "HTTP");
    }

    {
        // WS
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_PortEnumEEnum, "WS", 8081, "WS");
    }

    _initialize();
}

::ecore::EClass_ptr CustomuiPackage::getAbstractWebsocketEndPoint()
{
    return m_AbstractWebsocketEndPointEClass;
}
::ecore::EClass_ptr CustomuiPackage::getNevonexRoute()
{
    return m_NevonexRouteEClass;
}
::ecore::EClass_ptr CustomuiPackage::getUIWebServiceProvider()
{
    return m_UIWebServiceProviderEClass;
}
::ecore::EEnum_ptr CustomuiPackage::getPortEnum()
{
    return m_PortEnumEEnum;
}

