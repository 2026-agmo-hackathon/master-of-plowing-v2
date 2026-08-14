/*
 * nevonex/types/TypesFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_TYPESFACTORY_HPP
#define _NEVONEX_TYPESFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/types.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace types
    {

        class EXPORT_NEVONEX_DLL TypesFactory : public virtual ::ecore::EFactory
        {
        public:

            static TypesFactory_ptr _instance();


            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< TypesFactory > s_holder;

            TypesFactory();

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

    } // types
} // nevonex

#endif // _NEVONEX_TYPESFACTORY_HPP
