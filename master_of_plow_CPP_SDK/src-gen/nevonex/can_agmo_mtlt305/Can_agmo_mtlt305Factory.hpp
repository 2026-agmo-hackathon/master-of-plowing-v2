/*
 * nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_MTLT305FACTORY_HPP
#define _NEVONEX_CAN_AGMO_MTLT305FACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/can_agmo_mtlt305.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_mtlt305
    {

        class EXPORT_NEVONEX_DLL Can_agmo_mtlt305Factory : public virtual ::ecore::EFactory
        {
        public:

            static Can_agmo_mtlt305Factory_ptr _instance();

            virtual CAN_AGMO_MTLT305_ptr createCAN_AGMO_MTLT305();
            virtual CAN_AGMO_MTLT305Provider_ptr createCAN_AGMO_MTLT305Provider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< Can_agmo_mtlt305Factory > s_holder;

            Can_agmo_mtlt305Factory();

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

        template< > inline CAN_AGMO_MTLT305_ptr create< CAN_AGMO_MTLT305 >()
        {
            auto eFactory = Can_agmo_mtlt305Package::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_mtlt305Factory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_MTLT305();
        }

        template< > inline CAN_AGMO_MTLT305Provider_ptr create< CAN_AGMO_MTLT305Provider >()
        {
            auto eFactory = Can_agmo_mtlt305Package::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_mtlt305Factory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_MTLT305Provider();
        }

    } // can_agmo_mtlt305
} // nevonex

#endif // _NEVONEX_CAN_AGMO_MTLT305FACTORY_HPP
