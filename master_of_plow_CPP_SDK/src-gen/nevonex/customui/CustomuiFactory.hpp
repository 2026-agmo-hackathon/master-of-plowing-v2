/*
 * nevonex/customui/CustomuiFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CUSTOMUIFACTORY_HPP
#define _NEVONEX_CUSTOMUIFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/customui.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace customui
    {

        class EXPORT_NEVONEX_DLL CustomuiFactory : public virtual ::ecore::EFactory
        {
        public:

            static CustomuiFactory_ptr _instance();

            virtual AbstractWebsocketEndPoint_ptr createAbstractWebsocketEndPoint();
            virtual NevonexRoute_ptr createNevonexRoute();
            virtual UIWebServiceProvider_ptr createUIWebServiceProvider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< CustomuiFactory > s_holder;

            CustomuiFactory();

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

        template< > inline AbstractWebsocketEndPoint_ptr create< AbstractWebsocketEndPoint >()
        {
            auto eFactory = CustomuiPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CustomuiFactory* >(eFactory.get());
            return packageFactory->createAbstractWebsocketEndPoint();
        }

        template< > inline NevonexRoute_ptr create< NevonexRoute >()
        {
            auto eFactory = CustomuiPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CustomuiFactory* >(eFactory.get());
            return packageFactory->createNevonexRoute();
        }

        template< > inline UIWebServiceProvider_ptr create< UIWebServiceProvider >()
        {
            auto eFactory = CustomuiPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< CustomuiFactory* >(eFactory.get());
            return packageFactory->createUIWebServiceProvider();
        }

    } // customui
} // nevonex

#endif // _NEVONEX_CUSTOMUIFACTORY_HPP
