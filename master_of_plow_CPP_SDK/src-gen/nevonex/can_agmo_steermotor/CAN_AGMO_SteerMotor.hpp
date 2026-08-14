/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTOR_HPP
#define NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTOR_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/can_agmo_steermotor_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachine.hpp>
#include <nevonex/fcal/MOTOR_HEARTBEAT_T_R.hpp>
#include <nevonex/fcal/RAW_CAN_DATA_DLC_8_W.hpp>

#include "Can_agmo_steermotorPackage.hpp"

namespace nevonex
{
    namespace can_agmo_steermotor
    {

    class EXPORT_NEVONEX_DLL CAN_AGMO_SteerMotor : public virtual ::nevonex::common::TopicObject,
        public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachine
    {
        /*PROTECTED REGION ID(CAN_AGMO_SteerMotor_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        CAN_AGMO_SteerMotor();

        virtual ~CAN_AGMO_SteerMotor();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::ELong getMotor_Heartbeat_Timestamp();
        virtual ::ecore::EBoolean isMotor_Heartbeat_Valid();
        virtual ::ecore::ELong getMotor_Request_Timestamp();
        virtual ::ecore::EBoolean isMotor_Request_Valid();
        virtual ::ecore::ELong getMotor_Response_Encoder_Speed_Timestamp();
        virtual ::ecore::EBoolean isMotor_Response_Encoder_Speed_Valid();
        virtual ::ecore::ELong getMotor_Response_EncoderCountValue_Timestamp();
        virtual ::ecore::EBoolean isMotor_Response_EncoderCountValue_Valid();

        virtual ::ecore::EBoolean updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp);

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr getMotor_Heartbeat () const;
        /**
         * \brief 
         */
    public:
        virtual void setMotor_Heartbeat (::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr _motor_Heartbeat);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr getMotor_Request () const;
        /**
         * \brief 
         */
    public:
        virtual void setMotor_Request (::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _motor_Request);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getMotor_Response_Encoder_Speed () const;
        /**
         * \brief 
         */
    public:
        virtual void setMotor_Response_Encoder_Speed (::ecore::EInt _motor_Response_Encoder_Speed);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getMotor_Response_EncoderCountValue () const;
        /**
         * \brief 
         */
    public:
        virtual void setMotor_Response_EncoderCountValue (::ecore::EFloat _motor_Response_EncoderCountValue);

    protected:
        void setMotor_RequestInternal (::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr _motor_Request);


        /*PROTECTED REGION ID(CAN_AGMO_SteerMotor) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    protected:
        virtual ::ecore::EJavaObject eGet ( ::ecore::EInt _featureID, ::ecore::EBoolean _resolve) override;
        virtual void eSet ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual ::ecore::EBoolean eIsSet ( ::ecore::EInt _featureID) override;
        virtual void eUnset ( ::ecore::EInt _featureID) override;
        virtual ::ecore::EClass_ptr _eClass () override;
        virtual void _inverseAdd ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual void _inverseRemove ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _oldValue) override;
        // _setEContainer();
        using ::ecore::EObject::eIsProxy;
        using ::ecore::EObject::eResource;
        using ::ecore::EObject::eContainer;
        using ::ecore::EObject::eContainingFeature;
        using ::ecore::EObject::eContainmentFeature;
        using ::ecore::EObject::eContents;
        using ::ecore::EObject::eAllContents;
        using ::ecore::EObject::eCrossReferences;
        using ::ecore::EObject::eInvoke;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        CAN_AGMO_SteerMotor_ptr _this()
        {   return CAN_AGMO_SteerMotor_ptr(this);}

        friend CAN_AGMO_SteerMotorProvider;

    private:
        // Attributes

        // References
        ::nevonex::fcal::MOTOR_HEARTBEAT_T_R_ptr m_motor_Heartbeat;
        ::nevonex::fcal::RAW_CAN_DATA_DLC_8_W_ptr m_motor_Request;
        ::ecore::EInt m_motor_Response_Encoder_Speed;
        ::ecore::EFloat m_motor_Response_EncoderCountValue;

        std::map<int, ::nevonex::types::InterfaceDetails> m_interfaceDetailMap;

        /*PROTECTED REGION ID(CAN_AGMO_SteerMotor_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // can_agmo_steermotor
}// nevonex

#endif // NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTOR_HPP
