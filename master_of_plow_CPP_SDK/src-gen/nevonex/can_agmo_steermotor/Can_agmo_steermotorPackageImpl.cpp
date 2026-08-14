/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>
#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.hpp>
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

using namespace ::nevonex::can_agmo_steermotor;

Can_agmo_steermotorPackage::Can_agmo_steermotorPackage()
{

    // Feature definitions of CAN_AGMO_SteerMotor
    m_CAN_AGMO_SteerMotor__motor_Heartbeat = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_SteerMotor__motor_Request = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);
    m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);
    m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of CAN_AGMO_SteerMotorProvider
    m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void Can_agmo_steermotorPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = Can_agmo_steermotorFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // CAN_AGMO_SteerMotor
    {
        m_CAN_AGMO_SteerMotorEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_SteerMotorEClass->setClassifierID(CAN_AGMO_STEERMOTOR);
        m_CAN_AGMO_SteerMotorEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_SteerMotorEClass);
    }
    // m_CAN_AGMO_SteerMotor__motor_Heartbeat has already been allocated above
    m_CAN_AGMO_SteerMotor__motor_Heartbeat->setFeatureID(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT);
    m_CAN_AGMO_SteerMotorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_SteerMotor__motor_Heartbeat);
    // m_CAN_AGMO_SteerMotor__motor_Request has already been allocated above
    m_CAN_AGMO_SteerMotor__motor_Request->setFeatureID(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_REQUEST);
    m_CAN_AGMO_SteerMotorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_SteerMotor__motor_Request);
    // m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed has already been allocated above
    m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed->setFeatureID(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED);
    m_CAN_AGMO_SteerMotorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed);
    // m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue has already been allocated above
    m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue->setFeatureID(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE);
    m_CAN_AGMO_SteerMotorEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue);

    // CAN_AGMO_SteerMotorProvider
    {
        m_CAN_AGMO_SteerMotorProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_CAN_AGMO_SteerMotorProviderEClass->setClassifierID(CAN_AGMO_STEERMOTORPROVIDER);
        m_CAN_AGMO_SteerMotorProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_CAN_AGMO_SteerMotorProviderEClass);
    }
    // m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor has already been allocated above
    m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor->setFeatureID(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR);
    m_CAN_AGMO_SteerMotorProviderEClass->getEStructuralFeatures().push_back(
            m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor);

    // Create enums

    // Create data types

    // Initialize package
    setName("can_agmo_steermotor");
    setNsPrefix("can_agmo_steermotor");
    setNsURI("com.bosch.nevonex.can_agmo_steermotor");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_CAN_AGMO_SteerMotorEClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // CAN_AGMO_SteerMotor
    m_CAN_AGMO_SteerMotorEClass->setName("CAN_AGMO_SteerMotor");
    m_CAN_AGMO_SteerMotorEClass->setAbstract(false);
    m_CAN_AGMO_SteerMotorEClass->setInterface(false);
    m_CAN_AGMO_SteerMotor__motor_Heartbeat->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getMOTOR_HEARTBEAT_T_R());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_SteerMotor__motor_Heartbeat),
            "motor_Heartbeat", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_SteerMotor__motor_Request->setEType(
            dynamic_cast< ::nevonex::fcal::FcalPackage* >(::nevonex::fcal::FcalPackage::_instance().get())->getRAW_CAN_DATA_DLC_8_W());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_SteerMotor__motor_Request),
            "motor_Request", false, false, false, "", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEInt());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed),
            "motor_Response_Encoder_Speed", true, false, false, "0", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed->setID(false);
    m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue),
            "motor_Response_EncoderCountValue", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue->setID(false);
    // CAN_AGMO_SteerMotorProvider
    m_CAN_AGMO_SteerMotorProviderEClass->setName("CAN_AGMO_SteerMotorProvider");
    m_CAN_AGMO_SteerMotorProviderEClass->setAbstract(false);
    m_CAN_AGMO_SteerMotorProviderEClass->setInterface(false);
    m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor->setEType(
            dynamic_cast< ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage* >(::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::_instance().get())->getCAN_AGMO_SteerMotor());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor),
            "cAN_AGMO_SteerMotor", false, false, false, "", false, true, true, true, 0,
            1, false, false);

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotor()
{
    return m_CAN_AGMO_SteerMotorEClass;
}
::ecore::EClass_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotorProvider()
{
    return m_CAN_AGMO_SteerMotorProviderEClass;
}

::ecore::EReference_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotor__motor_Heartbeat()
{
    return m_CAN_AGMO_SteerMotor__motor_Heartbeat;
}
::ecore::EReference_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotor__motor_Request()
{
    return m_CAN_AGMO_SteerMotor__motor_Request;
}
::ecore::EAttribute_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotor__motor_Response_Encoder_Speed()
{
    return m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed;
}
::ecore::EAttribute_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotor__motor_Response_EncoderCountValue()
{
    return m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue;
}
::ecore::EReference_ptr Can_agmo_steermotorPackage::getCAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor()
{
    return m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor;
}
