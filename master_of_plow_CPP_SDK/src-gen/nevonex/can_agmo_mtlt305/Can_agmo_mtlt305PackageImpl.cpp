/*
 * nevonex/can_agmo_mtlt305/Can_agmo_mtlt305PackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp>
#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.hpp>
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

using namespace ::nevonex::can_agmo_mtlt305;

Can_agmo_mtlt305Package::Can_agmo_mtlt305Package()
{

    // Feature definitions of CAN_AGMO_MTLT305
    m_CAN_AGMO_MTLT305__aceinna_Accel = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_MTLT305__aceinna_AngleRate = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_MTLT305__aceinna_Angles = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

    // Feature definitions of CAN_AGMO_MTLT305Provider
    m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305 = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void Can_agmo_mtlt305Package::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = Can_agmo_mtlt305Factory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // CAN_AGMO_MTLT305
    {
        m_CAN_AGMO_MTLT305EClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_MTLT305EClass->setClassifierID(CAN_AGMO_MTLT305);
        m_CAN_AGMO_MTLT305EClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_MTLT305EClass);
    }
    // m_CAN_AGMO_MTLT305__aceinna_Accel has already been allocated above
    m_CAN_AGMO_MTLT305__aceinna_Accel->setFeatureID(
            ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL);
    m_CAN_AGMO_MTLT305EClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_MTLT305__aceinna_Accel);
    // m_CAN_AGMO_MTLT305__aceinna_AngleRate has already been allocated above
    m_CAN_AGMO_MTLT305__aceinna_AngleRate->setFeatureID(
            ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE);
    m_CAN_AGMO_MTLT305EClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_MTLT305__aceinna_AngleRate);
    // m_CAN_AGMO_MTLT305__aceinna_Angles has already been allocated above
    m_CAN_AGMO_MTLT305__aceinna_Angles->setFeatureID(
            ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES);
    m_CAN_AGMO_MTLT305EClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_MTLT305__aceinna_Angles);

    // CAN_AGMO_MTLT305Provider
    {
        m_CAN_AGMO_MTLT305ProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_MTLT305ProviderEClass->setClassifierID(CAN_AGMO_MTLT305PROVIDER);
        m_CAN_AGMO_MTLT305ProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_MTLT305ProviderEClass);
    }
    // m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305 has already been allocated above
    m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305->setFeatureID(
            ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305);
    m_CAN_AGMO_MTLT305ProviderEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305);

    // Create enums

    // Create data types

    // Initialize package
    setName("can_agmo_mtlt305");
    setNsPrefix("can_agmo_mtlt305");
    setNsURI("com.bosch.nevonex.can_agmo_mtlt305");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_CAN_AGMO_MTLT305EClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // CAN_AGMO_MTLT305
    m_CAN_AGMO_MTLT305EClass->setName("CAN_AGMO_MTLT305");
    m_CAN_AGMO_MTLT305EClass->setAbstract(false);
    m_CAN_AGMO_MTLT305EClass->setInterface(false);
    m_CAN_AGMO_MTLT305__aceinna_Accel->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ACCEL_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_MTLT305__aceinna_Accel),
            "aceinna_Accel", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_MTLT305__aceinna_AngleRate->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ANGLERATE_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_MTLT305__aceinna_AngleRate),
            "aceinna_AngleRate", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_MTLT305__aceinna_Angles->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getACEINNA_ANGLES_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_MTLT305__aceinna_Angles),
            "aceinna_Angles", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    // CAN_AGMO_MTLT305Provider
    m_CAN_AGMO_MTLT305ProviderEClass->setName("CAN_AGMO_MTLT305Provider");
    m_CAN_AGMO_MTLT305ProviderEClass->setAbstract(false);
    m_CAN_AGMO_MTLT305ProviderEClass->setInterface(false);
    m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305->setEType(
            dynamic_cast< ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package* >(::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::_instance().get())->getCAN_AGMO_MTLT305());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305),
            "cAN_AGMO_MTLT305", false, false, false, "", false, true, true, true, 0,
            1, false, false);

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305()
{
    return m_CAN_AGMO_MTLT305EClass;
}
::ecore::EClass_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305Provider()
{
    return m_CAN_AGMO_MTLT305ProviderEClass;
}

::ecore::EReference_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305__aceinna_Accel()
{
    return m_CAN_AGMO_MTLT305__aceinna_Accel;
}
::ecore::EReference_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305__aceinna_AngleRate()
{
    return m_CAN_AGMO_MTLT305__aceinna_AngleRate;
}
::ecore::EReference_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305__aceinna_Angles()
{
    return m_CAN_AGMO_MTLT305__aceinna_Angles;
}
::ecore::EReference_ptr Can_agmo_mtlt305Package::getCAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305()
{
    return m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305;
}
