/*
 * nevonex/common/CommonFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/common/CommonFactory.hpp>
#include <nevonex/common/CommonPackage.hpp>
#include <nevonex/common/AbsolutePosition.hpp>
#include <nevonex/common/TopicObject.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::common;

CommonFactory::CommonFactory()
{
}

::ecore::EObject_ptr CommonFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case CommonPackage::ABSOLUTEPOSITION:
        return createAbsolutePosition();
    case CommonPackage::TOPICOBJECT:
        return createTopicObject();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject CommonFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case CommonPackage::PROVIDERENUM:
    {
        ::ecore::EJavaObject _any;
        CommonPackage_ptr _epkg =
                dynamic_cast< ::nevonex::common::CommonPackage* >(getEPackage().get());
        return _epkg->getProviderEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case CommonPackage::CONNECTIONTYPEENUM:
    {
        ::ecore::EJavaObject _any;
        CommonPackage_ptr _epkg =
                dynamic_cast< ::nevonex::common::CommonPackage* >(getEPackage().get());
        return _epkg->getConnectionTypeEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case CommonPackage::PLATFORMSERVICESENUM:
    {
        ::ecore::EJavaObject _any;
        CommonPackage_ptr _epkg =
                dynamic_cast< ::nevonex::common::CommonPackage* >(getEPackage().get());
        return _epkg->getPlatformServicesEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case CommonPackage::HMISERVICESENUM:
    {
        ::ecore::EJavaObject _any;
        CommonPackage_ptr _epkg =
                dynamic_cast< ::nevonex::common::CommonPackage* >(getEPackage().get());
        return _epkg->getHMIServicesEnum()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString CommonFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case CommonPackage::PROVIDERENUM:
    {
        CommonPackage_ptr _epkg = ::ecore::as < ::nevonex::common::CommonPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getProviderEnum()->getEEnumLiteral(_value)->getName();
    }
    case CommonPackage::CONNECTIONTYPEENUM:
    {
        CommonPackage_ptr _epkg = ::ecore::as < ::nevonex::common::CommonPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getConnectionTypeEnum()->getEEnumLiteral(_value)->getName();
    }
    case CommonPackage::PLATFORMSERVICESENUM:
    {
        CommonPackage_ptr _epkg = ::ecore::as < ::nevonex::common::CommonPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getPlatformServicesEnum()->getEEnumLiteral(_value)->getName();
    }
    case CommonPackage::HMISERVICESENUM:
    {
        CommonPackage_ptr _epkg = ::ecore::as < ::nevonex::common::CommonPackage
                > (getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EInt > (_instanceValue);
        return _epkg->getHMIServicesEnum()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

AbsolutePosition_ptr CommonFactory::createAbsolutePosition()
{
    return ::ecore::Ptr < AbsolutePosition > (new AbsolutePosition);
}
TopicObject_ptr CommonFactory::createTopicObject()
{
    return ::ecore::Ptr < TopicObject > (new TopicObject);
}
