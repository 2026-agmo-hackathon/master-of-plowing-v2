/*
 * nevonex/fcb/FcbFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcb/FcbFactory.hpp>
#include <nevonex/fcb/FcbPackage.hpp>
#include <nevonex/fcb/FCALController.hpp>
#include <nevonex/fcb/ConnectionFactory.hpp>
#include <nevonex/fcb/PublishConnectionFactory.hpp>
#include <nevonex/fcb/SubscribeConnectionFactory.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::fcb;

FcbFactory::FcbFactory()
{
}

::ecore::EObject_ptr FcbFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case FcbPackage::FCALCONTROLLER:
        return createFCALController();
    case FcbPackage::CONNECTIONFACTORY:
        return createConnectionFactory();
    case FcbPackage::PUBLISHCONNECTIONFACTORY:
        return createPublishConnectionFactory();
    case FcbPackage::SUBSCRIBECONNECTIONFACTORY:
        return createSubscribeConnectionFactory();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject FcbFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case FcbPackage::SUBSCRIBERENUM:
    {
        ::ecore::EJavaObject _any;
        FcbPackage_ptr _epkg =
                dynamic_cast< ::nevonex::fcb::FcbPackage* >(getEPackage().get());
        return _epkg->getSubscriberEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString FcbFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case FcbPackage::SUBSCRIBERENUM:
    {
        FcbPackage_ptr _epkg = ::ecore::as < ::nevonex::fcb::FcbPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getSubscriberEnum()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

FCALController_ptr FcbFactory::createFCALController()
{
    return ::ecore::Ptr < FCALController > (new FCALController);
}
ConnectionFactory_ptr FcbFactory::createConnectionFactory()
{
    return ::ecore::Ptr < ConnectionFactory > (new ConnectionFactory);
}
PublishConnectionFactory_ptr FcbFactory::createPublishConnectionFactory()
{
    return ::ecore::Ptr < PublishConnectionFactory > (new PublishConnectionFactory);
}
SubscribeConnectionFactory_ptr FcbFactory::createSubscribeConnectionFactory()
{
    return ::ecore::Ptr < SubscribeConnectionFactory > (new SubscribeConnectionFactory);
}
