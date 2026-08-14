/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_STEERMOTORFACTORY_HPP
#define _NEVONEX_CAN_AGMO_STEERMOTORFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/can_agmo_steermotor.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_steermotor
    {

        class EXPORT_NEVONEX_DLL Can_agmo_steermotorFactory : public virtual ::ecore::EFactory
        {
        public:

            static Can_agmo_steermotorFactory_ptr _instance();

            virtual CAN_AGMO_SteerMotor_ptr createCAN_AGMO_SteerMotor();
            virtual CAN_AGMO_SteerMotorProvider_ptr createCAN_AGMO_SteerMotorProvider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< Can_agmo_steermotorFactory > s_holder;

            Can_agmo_steermotorFactory();

        };

        /** An object creation helper
         *
         * Usage (add namespaces as required):
         *   auto p = create<MyClass>();
         *
         */
        template< class T > inline ::ecore::Ptr< T > create()
        {
            return ::ecore::Ptr< T >();
        }

        template< > inline CAN_AGMO_SteerMotor_ptr create< CAN_AGMO_SteerMotor >()
        {
            auto eFactory = Can_agmo_steermotorPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_steermotorFactory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_SteerMotor();
        }

        template< > inline CAN_AGMO_SteerMotorProvider_ptr create< CAN_AGMO_SteerMotorProvider >()
        {
            auto eFactory = Can_agmo_steermotorPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_steermotorFactory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_SteerMotorProvider();
        }

    } // can_agmo_steermotor
} // nevonex

#endif // _NEVONEX_CAN_AGMO_STEERMOTORFACTORY_HPP
