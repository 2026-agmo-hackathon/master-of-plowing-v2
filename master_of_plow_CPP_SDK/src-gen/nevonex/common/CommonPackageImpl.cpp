/*
 * nevonex/common/CommonPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/common/CommonPackage.hpp>
#include <nevonex/common/CommonFactory.hpp>
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

using namespace ::nevonex::common;

CommonPackage::CommonPackage()
{

    // Feature definitions of AbsolutePosition
    m_AbsolutePosition__altitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_AbsolutePosition__latitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_AbsolutePosition__longitude = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of TopicObject
    m_TopicObject__index = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

}

void CommonPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = CommonFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // AbsolutePosition
    {
        m_AbsolutePositionEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_AbsolutePositionEClass->setClassifierID(ABSOLUTEPOSITION);
        m_AbsolutePositionEClass->setEPackage(_this());
        getEClassifiers().push_back(m_AbsolutePositionEClass);
    }
    // m_AbsolutePosition__altitude has already been allocated above
    m_AbsolutePosition__altitude->setFeatureID(
            ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__ALTITUDE);
    m_AbsolutePositionEClass->getEStructuralFeatures().push_back(
            m_AbsolutePosition__altitude);
    // m_AbsolutePosition__latitude has already been allocated above
    m_AbsolutePosition__latitude->setFeatureID(
            ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LATITUDE);
    m_AbsolutePositionEClass->getEStructuralFeatures().push_back(
            m_AbsolutePosition__latitude);
    // m_AbsolutePosition__longitude has already been allocated above
    m_AbsolutePosition__longitude->setFeatureID(
            ::nevonex::common::CommonPackage::ABSOLUTEPOSITION__LONGITUDE);
    m_AbsolutePositionEClass->getEStructuralFeatures().push_back(
            m_AbsolutePosition__longitude);

    // TopicObject
    {
        m_TopicObjectEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_TopicObjectEClass->setClassifierID(TOPICOBJECT);
        m_TopicObjectEClass->setEPackage(_this());
        getEClassifiers().push_back(m_TopicObjectEClass);
    }
    // m_TopicObject__index has already been allocated above
    m_TopicObject__index->setFeatureID(
            ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX);
    m_TopicObjectEClass->getEStructuralFeatures().push_back(
            m_TopicObject__index);

    // Create enums

    {
        m_ProviderEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_ProviderEnumEEnum->setClassifierID(PROVIDERENUM);
        m_ProviderEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_ProviderEnumEEnum);
    }

    {
        m_ConnectionTypeEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_ConnectionTypeEnumEEnum->setClassifierID(CONNECTIONTYPEENUM);
        m_ConnectionTypeEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_ConnectionTypeEnumEEnum);
    }

    {
        m_PlatformServicesEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_PlatformServicesEnumEEnum->setClassifierID(PLATFORMSERVICESENUM);
        m_PlatformServicesEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_PlatformServicesEnumEEnum);
    }

    {
        m_HMIServicesEnumEEnum = ::ecore::Ptr < ::ecore::EEnum
                > (new ::ecore::EEnum);
        m_HMIServicesEnumEEnum->setClassifierID(HMISERVICESENUM);
        m_HMIServicesEnumEEnum->setEPackage(_this());
        getEClassifiers().push_back(m_HMIServicesEnumEEnum);
    }

    // Create data types

    // Initialize package
    setName("common");
    setNsPrefix("common");
    setNsURI("com.bosch.nevonex.common");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // AbsolutePosition
    m_AbsolutePositionEClass->setName("AbsolutePosition");
    m_AbsolutePositionEClass->setAbstract(false);
    m_AbsolutePositionEClass->setInterface(false);
    m_AbsolutePosition__altitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEDouble());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_AbsolutePosition__altitude),
            "altitude", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_AbsolutePosition__altitude->setID(false);
    m_AbsolutePosition__latitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEDouble());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_AbsolutePosition__latitude),
            "latitude", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_AbsolutePosition__latitude->setID(false);
    m_AbsolutePosition__longitude->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEDouble());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_AbsolutePosition__longitude),
            "longitude", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_AbsolutePosition__longitude->setID(false);
    // TopicObject
    m_TopicObjectEClass->setName("TopicObject");
    m_TopicObjectEClass->setAbstract(false);
    m_TopicObjectEClass->setInterface(false);
    m_TopicObject__index->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_TopicObject__index),
            "index", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_TopicObject__index->setID(false);

    // TODO: Initialize data types

    // ProviderEnum
    m_ProviderEnumEEnum->setName("ProviderEnum");
    m_ProviderEnumEEnum->setSerializable(true);

    {
        // CAN_AGMO_Customized_TractorProvider
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ProviderEnumEEnum, "CAN_AGMO_Customized_TractorProvider", 0, "CAN_AGMO_Customized_TractorProvider");
    }

    {
        // CAN_AGMO_MTLT305Provider
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ProviderEnumEEnum, "CAN_AGMO_MTLT305Provider", 1, "CAN_AGMO_MTLT305Provider");
    }

    {
        // CAN_AGMO_SteerMotorProvider
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ProviderEnumEEnum, "CAN_AGMO_SteerMotorProvider", 2, "CAN_AGMO_SteerMotorProvider");
    }

    {
        // ISOPGNProvider
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ProviderEnumEEnum, "ISOPGNProvider", 3, "ISOPGNProvider");
    }

    {
        // Serial_Ext_GPS_NMEA0183Provider
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ProviderEnumEEnum, "Serial_Ext_GPS_NMEA0183Provider", 4, "Serial_Ext_GPS_NMEA0183Provider");
    }

    // ConnectionTypeEnum
    m_ConnectionTypeEnumEEnum->setName("ConnectionTypeEnum");
    m_ConnectionTypeEnumEEnum->setSerializable(true);

    {
        // MQTT
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "MQTT", 0, "MQTT");
    }

    {
        // CAN
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "CAN", 1, "CAN");
    }

    {
        // GPIO
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "GPIO", 2, "GPIO");
    }

    {
        // GPS
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "GPS", 3, "GPS");
    }

    {
        // ISOBUS
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "ISOBUS", 4, "ISOBUS");
    }

    {
        // Serial
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "Serial", 5, "Serial");
    }

    {
        // WIFI
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "WIFI", 6, "WIFI");
    }

    {
        // SATELLITE
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_ConnectionTypeEnumEEnum, "SATELLITE", 7, "SATELLITE");
    }

    // PlatformServicesEnum
    m_PlatformServicesEnumEEnum->setName("PlatformServicesEnum");
    m_PlatformServicesEnumEEnum->setSerializable(true);

    {
        // CLOUD_DOWNLOAD
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_PlatformServicesEnumEEnum, "CLOUD_DOWNLOAD", 0, "CLOUD_DOWNLOAD");
    }

    {
        // CLOUD_UPLOAD
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_PlatformServicesEnumEEnum, "CLOUD_UPLOAD", 1, "CLOUD_UPLOAD");
    }

    // HMIServicesEnum
    m_HMIServicesEnumEEnum->setName("HMIServicesEnum");
    m_HMIServicesEnumEEnum->setSerializable(true);

    {
        // CUSTOMUI
        ::ecore::EEnumLiteral_ptr _el = addEEnumLiteral(
                m_HMIServicesEnumEEnum, "CUSTOMUI", 0, "CUSTOMUI");
    }

    _initialize();
}

::ecore::EClass_ptr CommonPackage::getAbsolutePosition()
{
    return m_AbsolutePositionEClass;
}
::ecore::EClass_ptr CommonPackage::getTopicObject()
{
    return m_TopicObjectEClass;
}
::ecore::EEnum_ptr CommonPackage::getProviderEnum()
{
    return m_ProviderEnumEEnum;
}
::ecore::EEnum_ptr CommonPackage::getConnectionTypeEnum()
{
    return m_ConnectionTypeEnumEEnum;
}
::ecore::EEnum_ptr CommonPackage::getPlatformServicesEnum()
{
    return m_PlatformServicesEnumEEnum;
}
::ecore::EEnum_ptr CommonPackage::getHMIServicesEnum()
{
    return m_HMIServicesEnumEEnum;
}

::ecore::EAttribute_ptr CommonPackage::getAbsolutePosition__altitude()
{
    return m_AbsolutePosition__altitude;
}
::ecore::EAttribute_ptr CommonPackage::getAbsolutePosition__latitude()
{
    return m_AbsolutePosition__latitude;
}
::ecore::EAttribute_ptr CommonPackage::getAbsolutePosition__longitude()
{
    return m_AbsolutePosition__longitude;
}
::ecore::EAttribute_ptr CommonPackage::getTopicObject__index()
{
    return m_TopicObject__index;
}
