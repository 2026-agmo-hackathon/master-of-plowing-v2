/*
 * nevonex/NevonexFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEXFACTORY_HPP
#define _NEVONEXFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{

    class EXPORT_NEVONEX_DLL NevonexFactory : public virtual ::ecore::EFactory
    {
    public:

        static NevonexFactory_ptr _instance();

        virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
        virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
        virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

    protected:

        static ::ecore::Ptr< NevonexFactory > s_holder;

        NevonexFactory();

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

} // nevonex

#endif // _NEVONEXFACTORY_HPP
