/*
 * nevonex/cloud/CloudFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#ifndef _NEVONEX_CLOUDFACTORY_HPP
#define _NEVONEX_CLOUDFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/cloud.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace cloud
    {

        class EXPORT_NEVONEX_DLL CloudFactory : public virtual ::ecore::EFactory
        {
        public:

            static CloudFactory_ptr _instance();

            virtual Cloud_ptr createCloud();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< CloudFactory > s_holder;

            CloudFactory();

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

        template< > inline Cloud_ptr create< Cloud >()
        {
            auto eFactory = CloudPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CloudFactory* >(eFactory.get());
            return packageFactory->createCloud();
        }

    } // cloud
} // nevonex

#endif // _NEVONEX_CLOUDFACTORY_HPP

