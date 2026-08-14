/*
 * nevonex/fcb/FcbFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCBFACTORY_HPP
#define _NEVONEX_FCBFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/fcb.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace fcb
    {

        class EXPORT_NEVONEX_DLL FcbFactory : public virtual ::ecore::EFactory
        {
        public:

            static FcbFactory_ptr _instance();

            virtual FCALController_ptr createFCALController();
            virtual ConnectionFactory_ptr createConnectionFactory();
            virtual PublishConnectionFactory_ptr createPublishConnectionFactory();
            virtual SubscribeConnectionFactory_ptr createSubscribeConnectionFactory();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< FcbFactory > s_holder;

            FcbFactory();

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

        template< > inline FCALController_ptr create< FCALController >()
        {
            auto eFactory = FcbPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcbFactory* >(eFactory.get());
            return packageFactory->createFCALController();
        }

        template< > inline ConnectionFactory_ptr create< ConnectionFactory >()
        {
            auto eFactory = FcbPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcbFactory* >(eFactory.get());
            return packageFactory->createConnectionFactory();
        }

        template< > inline PublishConnectionFactory_ptr create< PublishConnectionFactory >()
        {
            auto eFactory = FcbPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcbFactory* >(eFactory.get());
            return packageFactory->createPublishConnectionFactory();
        }

        template< > inline SubscribeConnectionFactory_ptr create< SubscribeConnectionFactory >()
        {
            auto eFactory = FcbPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcbFactory* >(eFactory.get());
            return packageFactory->createSubscribeConnectionFactory();
        }

    } // fcb
} // nevonex

#endif // _NEVONEX_FCBFACTORY_HPP
