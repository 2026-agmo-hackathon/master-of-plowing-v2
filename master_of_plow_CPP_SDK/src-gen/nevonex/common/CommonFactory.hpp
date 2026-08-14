/*
 * nevonex/common/CommonFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_COMMONFACTORY_HPP
#define _NEVONEX_COMMONFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/common.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace common
    {

        class EXPORT_NEVONEX_DLL CommonFactory : public virtual ::ecore::EFactory
        {
        public:

            static CommonFactory_ptr _instance();

            virtual AbsolutePosition_ptr createAbsolutePosition();
            virtual TopicObject_ptr createTopicObject();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< CommonFactory > s_holder;

            CommonFactory();

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

        template< > inline AbsolutePosition_ptr create< AbsolutePosition >()
        {
            auto eFactory = CommonPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CommonFactory* >(eFactory.get());
            return packageFactory->createAbsolutePosition();
        }

        template< > inline TopicObject_ptr create< TopicObject >()
        {
            auto eFactory = CommonPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CommonFactory* >(eFactory.get());
            return packageFactory->createTopicObject();
        }

    } // common
} // nevonex

#endif // _NEVONEX_COMMONFACTORY_HPP
