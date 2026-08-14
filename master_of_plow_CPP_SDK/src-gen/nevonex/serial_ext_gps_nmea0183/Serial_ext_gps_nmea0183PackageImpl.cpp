/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183PackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.hpp>
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
#include <nevonex/common/CommonPackage.hpp>
#include <nevonex/fcal/FcalPackage.hpp>

using namespace ::nevonex::serial_ext_gps_nmea0183;

Serial_ext_gps_nmea0183Package::Serial_ext_gps_nmea0183Package()
{

    // Feature definitions of Serial_Ext_GPS_NMEA0183
    m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

    // Feature definitions of Serial_Ext_GPS_NMEA0183Provider
    m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183 = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void Serial_ext_gps_nmea0183Package::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = Serial_ext_gps_nmea0183Factory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // Serial_Ext_GPS_NMEA0183
    {
        m_Serial_Ext_GPS_NMEA0183EClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_Serial_Ext_GPS_NMEA0183EClass->setClassifierID(SERIAL_EXT_GPS_NMEA0183);
        m_Serial_Ext_GPS_NMEA0183EClass->setEPackage(_this());
        getEClassifiers().push_back(m_Serial_Ext_GPS_NMEA0183EClass);
    }
    // m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data has already been allocated above
    m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data->setFeatureID(
            ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA);
    m_Serial_Ext_GPS_NMEA0183EClass->getEStructuralFeatures().push_back(
            m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data);

    // Serial_Ext_GPS_NMEA0183Provider
    {
        m_Serial_Ext_GPS_NMEA0183ProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_Serial_Ext_GPS_NMEA0183ProviderEClass->setClassifierID(SERIAL_EXT_GPS_NMEA0183PROVIDER);
        m_Serial_Ext_GPS_NMEA0183ProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_Serial_Ext_GPS_NMEA0183ProviderEClass);
    }
    // m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183 has already been allocated above
    m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183->setFeatureID(
            ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::SERIAL_EXT_GPS_NMEA0183PROVIDER__SERIAL_EXT_GPS_NMEA0183);
    m_Serial_Ext_GPS_NMEA0183ProviderEClass->getEStructuralFeatures().push_back(
            m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183);

    // Create enums

    // Create data types

    // Initialize package
    setName("serial_ext_gps_nmea0183");
    setNsPrefix("serial_ext_gps_nmea0183");
    setNsURI("com.bosch.nevonex.serial_ext_gps_nmea0183");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_Serial_Ext_GPS_NMEA0183EClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // Serial_Ext_GPS_NMEA0183
    m_Serial_Ext_GPS_NMEA0183EClass->setName("Serial_Ext_GPS_NMEA0183");
    m_Serial_Ext_GPS_NMEA0183EClass->setAbstract(false);
    m_Serial_Ext_GPS_NMEA0183EClass->setInterface(false);
    m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getGPS_NMEA0183_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data),
            "serial_Ext_GPS_NMEA0183_Data", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    // Serial_Ext_GPS_NMEA0183Provider
    m_Serial_Ext_GPS_NMEA0183ProviderEClass->setName("Serial_Ext_GPS_NMEA0183Provider");
    m_Serial_Ext_GPS_NMEA0183ProviderEClass->setAbstract(false);
    m_Serial_Ext_GPS_NMEA0183ProviderEClass->setInterface(false);
    m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183->setEType(
            dynamic_cast< ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package* >(::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::_instance().get())->getSerial_Ext_GPS_NMEA0183());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183),
            "serial_Ext_GPS_NMEA0183", false, false, false, "", false, true, true, true, 0,
            1, false, false);

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr Serial_ext_gps_nmea0183Package::getSerial_Ext_GPS_NMEA0183()
{
    return m_Serial_Ext_GPS_NMEA0183EClass;
}
::ecore::EClass_ptr Serial_ext_gps_nmea0183Package::getSerial_Ext_GPS_NMEA0183Provider()
{
    return m_Serial_Ext_GPS_NMEA0183ProviderEClass;
}

::ecore::EReference_ptr Serial_ext_gps_nmea0183Package::getSerial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data()
{
    return m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data;
}
::ecore::EReference_ptr Serial_ext_gps_nmea0183Package::getSerial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183()
{
    return m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183;
}
