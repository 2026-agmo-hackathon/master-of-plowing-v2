/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_STEERMOTORPACKAGE_HPP
#define _NEVONEX_CAN_AGMO_STEERMOTORPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/can_agmo_steermotor_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/common/CommonPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_steermotor
    {

    class EXPORT_NEVONEX_DLL Can_agmo_steermotorPackage : public virtual ::ecore::EPackage
    {
    public:

        static Can_agmo_steermotorPackage_ptr _instance();
        static Can_agmo_steermotorPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int CAN_AGMO_STEERMOTOR = 0;

        static const int CAN_AGMO_STEERMOTORPROVIDER = 1;

        static const int ICAN_AGMO_STEERMOTOR = 2;

        static const int ICAN_AGMO_STEERMOTORFACTORY = 3;

        static const int ICAN_AGMO_STEERMOTORPROVIDER = 4;

        static const int CAN_AGMO_STEERMOTOR__MOTOR_HEARTBEAT = 4;

        static const int CAN_AGMO_STEERMOTOR__MOTOR_REQUEST = 5;

        static const int CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODER_SPEED = 6;

        static const int CAN_AGMO_STEERMOTOR__MOTOR_RESPONSE_ENCODERCOUNTVALUE = 7;

        static const int CAN_AGMO_STEERMOTORPROVIDER__CAN_AGMO_STEERMOTOR = 8;

        // IDs for classifiers for inherited features

        static const int CAN_AGMO_STEERMOTOR__INDEX = ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getCAN_AGMO_SteerMotor();
        virtual ::ecore::EClass_ptr getCAN_AGMO_SteerMotorProvider();

        // EStructuralFeatures methods

        virtual ::ecore::EReference_ptr getCAN_AGMO_SteerMotor__motor_Heartbeat();
        virtual ::ecore::EReference_ptr getCAN_AGMO_SteerMotor__motor_Request();
        virtual ::ecore::EAttribute_ptr getCAN_AGMO_SteerMotor__motor_Response_Encoder_Speed();
        virtual ::ecore::EAttribute_ptr getCAN_AGMO_SteerMotor__motor_Response_EncoderCountValue();
        virtual ::ecore::EReference_ptr getCAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor();

    protected:

        static ::ecore::Ptr< Can_agmo_steermotorPackage > s_instance;

        Can_agmo_steermotorPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_CAN_AGMO_SteerMotorEClass;

        ::ecore::EClass_ptr m_CAN_AGMO_SteerMotorProviderEClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EReference_ptr m_CAN_AGMO_SteerMotor__motor_Heartbeat;

        ::ecore::EReference_ptr m_CAN_AGMO_SteerMotor__motor_Request;

        ::ecore::EAttribute_ptr m_CAN_AGMO_SteerMotor__motor_Response_Encoder_Speed;

        ::ecore::EAttribute_ptr m_CAN_AGMO_SteerMotor__motor_Response_EncoderCountValue;

        ::ecore::EReference_ptr m_CAN_AGMO_SteerMotorProvider__cAN_AGMO_SteerMotor;

    };

}
 // can_agmo_steermotor
}// nevonex

#endif // _NEVONEX_CAN_AGMO_STEERMOTORPACKAGE_HPP
