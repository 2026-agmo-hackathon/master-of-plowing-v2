/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORFACTORY_HPP
#define _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/can_agmo_customized_tractor.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_customized_tractor
    {

        class EXPORT_NEVONEX_DLL Can_agmo_customized_tractorFactory : public virtual ::ecore::EFactory
        {
        public:

            static Can_agmo_customized_tractorFactory_ptr _instance();

            virtual CAN_AGMO_Customized_Tractor_ptr createCAN_AGMO_Customized_Tractor();
            virtual CAN_AGMO_Customized_TractorProvider_ptr createCAN_AGMO_Customized_TractorProvider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< Can_agmo_customized_tractorFactory > s_holder;

            Can_agmo_customized_tractorFactory();

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

        template< > inline CAN_AGMO_Customized_Tractor_ptr create< CAN_AGMO_Customized_Tractor >()
        {
            auto eFactory = Can_agmo_customized_tractorPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_customized_tractorFactory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_Customized_Tractor();
        }

        template< > inline CAN_AGMO_Customized_TractorProvider_ptr create< CAN_AGMO_Customized_TractorProvider >()
        {
            auto eFactory = Can_agmo_customized_tractorPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Can_agmo_customized_tractorFactory* >(eFactory.get());
            return packageFactory->createCAN_AGMO_Customized_TractorProvider();
        }

    } // can_agmo_customized_tractor
} // nevonex

#endif // _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORFACTORY_HPP
