/*
 * nevonex/exception/ExceptionFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_EXCEPTIONFACTORY_HPP
#define _NEVONEX_EXCEPTIONFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/exception.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace exception
    {

        class EXPORT_NEVONEX_DLL ExceptionFactory : public virtual ::ecore::EFactory
        {
        public:

            static ExceptionFactory_ptr _instance();


            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< ExceptionFactory > s_holder;

            ExceptionFactory();

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

    } // exception
} // nevonex

#endif // _NEVONEX_EXCEPTIONFACTORY_HPP
