/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>
#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.hpp>
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

using namespace ::nevonex::can_agmo_customized_tractor;

Can_agmo_customized_tractorPackage::Can_agmo_customized_tractorPackage()
{

    // Feature definitions of CAN_AGMO_Customized_Tractor
    m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__send_ACC_CMD = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__send_FNR_CMD = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__send_HYD_CMD = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_Customized_Tractor__send_SFT_CMD = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

    // Feature definitions of CAN_AGMO_Customized_TractorProvider
    m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void Can_agmo_customized_tractorPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = Can_agmo_customized_tractorFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // CAN_AGMO_Customized_Tractor
    {
        m_CAN_AGMO_Customized_TractorEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_Customized_TractorEClass->setClassifierID(CAN_AGMO_CUSTOMIZED_TRACTOR);
        m_CAN_AGMO_Customized_TractorEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_Customized_TractorEClass);
    }
    // m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO has already been allocated above
    m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO);
    // m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO has already been allocated above
    m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO);
    // m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO has already been allocated above
    m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO);
    // m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO has already been allocated above
    m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO);
    // m_CAN_AGMO_Customized_Tractor__send_ACC_CMD has already been allocated above
    m_CAN_AGMO_Customized_Tractor__send_ACC_CMD->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__send_ACC_CMD);
    // m_CAN_AGMO_Customized_Tractor__send_FNR_CMD has already been allocated above
    m_CAN_AGMO_Customized_Tractor__send_FNR_CMD->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__send_FNR_CMD);
    // m_CAN_AGMO_Customized_Tractor__send_HYD_CMD has already been allocated above
    m_CAN_AGMO_Customized_Tractor__send_HYD_CMD->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__send_HYD_CMD);
    // m_CAN_AGMO_Customized_Tractor__send_SFT_CMD has already been allocated above
    m_CAN_AGMO_Customized_Tractor__send_SFT_CMD->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD);
    m_CAN_AGMO_Customized_TractorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_Tractor__send_SFT_CMD);

    // CAN_AGMO_Customized_TractorProvider
    {
        m_CAN_AGMO_Customized_TractorProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_Customized_TractorProviderEClass->setClassifierID(CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER);
        m_CAN_AGMO_Customized_TractorProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_Customized_TractorProviderEClass);
    }
    // m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor has already been allocated above
    m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor->setFeatureID(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR);
    m_CAN_AGMO_Customized_TractorProviderEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor);

    // Create enums

    // Create data types

    // Initialize package
    setName("can_agmo_customized_tractor");
    setNsPrefix("can_agmo_customized_tractor");
    setNsURI("com.bosch.nevonex.can_agmo_customized_tractor");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_CAN_AGMO_Customized_TractorEClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // CAN_AGMO_Customized_Tractor
    m_CAN_AGMO_Customized_TractorEClass->setName("CAN_AGMO_Customized_Tractor");
    m_CAN_AGMO_Customized_TractorEClass->setAbstract(false);
    m_CAN_AGMO_Customized_TractorEClass->setInterface(false);
    m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRECEIVE_ACC_INFO_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO),
            "receive_ACC_INFO", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRECEIVE_FNR_INFO_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO),
            "receive_FNR_INFO", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRECEIVE_HYD_INFO_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO),
            "receive_HYD_INFO", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRECEIVE_SFT_INFO_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO),
            "receive_SFT_INFO", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__send_ACC_CMD->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getAGMO_GENERIC_SEND_CMD_T_W());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__send_ACC_CMD),
            "send_ACC_CMD", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__send_FNR_CMD->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getAGMO_GENERIC_SEND_CMD_T_W());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__send_FNR_CMD),
            "send_FNR_CMD", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__send_HYD_CMD->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getAGMO_GENERIC_SEND_CMD_T_W());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__send_HYD_CMD),
            "send_HYD_CMD", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_Customized_Tractor__send_SFT_CMD->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getAGMO_GENERIC_SEND_CMD_T_W());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_Tractor__send_SFT_CMD),
            "send_SFT_CMD", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    // CAN_AGMO_Customized_TractorProvider
    m_CAN_AGMO_Customized_TractorProviderEClass->setName("CAN_AGMO_Customized_TractorProvider");
    m_CAN_AGMO_Customized_TractorProviderEClass->setAbstract(false);
    m_CAN_AGMO_Customized_TractorProviderEClass->setInterface(false);
    m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor->setEType(
            dynamic_cast< ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage* >(::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::_instance().get())->getCAN_AGMO_Customized_Tractor());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor),
            "cAN_AGMO_Customized_Tractor", false, false, false, "", false, true, true, true, 0,
            1, false, false);

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor()
{
    return m_CAN_AGMO_Customized_TractorEClass;
}
::ecore::EClass_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_TractorProvider()
{
    return m_CAN_AGMO_Customized_TractorProviderEClass;
}

::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__receive_ACC_INFO()
{
    return m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__receive_FNR_INFO()
{
    return m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__receive_HYD_INFO()
{
    return m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__receive_SFT_INFO()
{
    return m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__send_ACC_CMD()
{
    return m_CAN_AGMO_Customized_Tractor__send_ACC_CMD;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__send_FNR_CMD()
{
    return m_CAN_AGMO_Customized_Tractor__send_FNR_CMD;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__send_HYD_CMD()
{
    return m_CAN_AGMO_Customized_Tractor__send_HYD_CMD;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_Tractor__send_SFT_CMD()
{
    return m_CAN_AGMO_Customized_Tractor__send_SFT_CMD;
}
::ecore::EReference_ptr Can_agmo_customized_tractorPackage::getCAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor()
{
    return m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor;
}
