/*
 * nevonex/fcal/MOTOR_HEARTBEAT_T_R.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_MOTOR_HEARTBEAT_T_R_HPP
#define NEVONEX_FCAL_MOTOR_HEARTBEAT_T_R_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/fcal_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include "FcalPackage.hpp"

namespace nevonex
{
    namespace fcal
    {

    class EXPORT_NEVONEX_DLL MOTOR_HEARTBEAT_T_R : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        MOTOR_HEARTBEAT_T_R();

        virtual ~MOTOR_HEARTBEAT_T_R();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_HallFailure () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_HallFailure (::ecore::EInt _heartbeat_ControlStatus_HallFailure);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_CANdisconnected () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_CANdisconnected (::ecore::EInt _heartbeat_ControlStatus_CANdisconnected);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_MotorStalled () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_MotorStalled (::ecore::EInt _heartbeat_ControlStatus_MotorStalled);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_Disabled () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_Disabled (::ecore::EInt _heartbeat_ControlStatus_Disabled);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_Overvoltage () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_Overvoltage (::ecore::EInt _heartbeat_ControlStatus_Overvoltage);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_HardwareProtection () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_HardwareProtection (::ecore::EInt _heartbeat_ControlStatus_HardwareProtection);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_E2PROM () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_E2PROM (::ecore::EInt _heartbeat_ControlStatus_E2PROM);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_Undervoltage () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_Undervoltage (::ecore::EInt _heartbeat_ControlStatus_Undervoltage);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_Overcurrent () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_Overcurrent (::ecore::EInt _heartbeat_ControlStatus_Overcurrent);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeartbeat_ControlStatus_ModeFailure () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeartbeat_ControlStatus_ModeFailure (::ecore::EInt _heartbeat_ControlStatus_ModeFailure);

        // References
    public:

        /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R) START*/
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

        /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_RImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        MOTOR_HEARTBEAT_T_R_ptr _this()
        {   return MOTOR_HEARTBEAT_T_R_ptr(this);}

    private:
        // Attributes

        ::ecore::EInt m_heartbeat_ControlStatus_HallFailure;

        ::ecore::EInt m_heartbeat_ControlStatus_CANdisconnected;

        ::ecore::EInt m_heartbeat_ControlStatus_MotorStalled;

        ::ecore::EInt m_heartbeat_ControlStatus_Disabled;

        ::ecore::EInt m_heartbeat_ControlStatus_Overvoltage;

        ::ecore::EInt m_heartbeat_ControlStatus_HardwareProtection;

        ::ecore::EInt m_heartbeat_ControlStatus_E2PROM;

        ::ecore::EInt m_heartbeat_ControlStatus_Undervoltage;

        ::ecore::EInt m_heartbeat_ControlStatus_Overcurrent;

        ::ecore::EInt m_heartbeat_ControlStatus_ModeFailure;

        // References


        /*PROTECTED REGION ID(MOTOR_HEARTBEAT_T_R_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_MOTOR_HEARTBEAT_T_R_HPP
