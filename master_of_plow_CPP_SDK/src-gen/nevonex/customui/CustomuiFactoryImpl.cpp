/*
 * nevonex/customui/CustomuiFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/customui/CustomuiFactory.hpp>
#include <nevonex/customui/CustomuiPackage.hpp>
#include <nevonex/customui/AbstractWebsocketEndPoint.hpp>
#include <nevonex/customui/NevonexRoute.hpp>
#include <nevonex/customui/UIWebServiceProvider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::customui;

CustomuiFactory::CustomuiFactory()
{
}

::ecore::EObject_ptr CustomuiFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case CustomuiPackage::ABSTRACTWEBSOCKETENDPOINT:
        return createAbstractWebsocketEndPoint();
    case CustomuiPackage::NEVONEXROUTE:
        return createNevonexRoute();
    case CustomuiPackage::UIWEBSERVICEPROVIDER:
        return createUIWebServiceProvider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject CustomuiFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case CustomuiPackage::PORTENUM:
    {
        ::ecore::EJavaObject _any;
        CustomuiPackage_ptr _epkg =
                dynamic_cast< ::nevonex::customui::CustomuiPackage* >(getEPackage().get());
        return _epkg->getPortEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString CustomuiFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case CustomuiPackage::PORTENUM:
    {
        CustomuiPackage_ptr _epkg = ::ecore::as < ::nevonex::customui::CustomuiPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getPortEnum()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

AbstractWebsocketEndPoint_ptr CustomuiFactory::createAbstractWebsocketEndPoint()
{
    return ::ecore::Ptr < AbstractWebsocketEndPoint > (new AbstractWebsocketEndPoint);
}
NevonexRoute_ptr CustomuiFactory::createNevonexRoute()
{
    return ::ecore::Ptr < NevonexRoute > (new NevonexRoute);
}
UIWebServiceProvider_ptr CustomuiFactory::createUIWebServiceProvider()
{
    return ::ecore::Ptr < UIWebServiceProvider > (new UIWebServiceProvider);
}
