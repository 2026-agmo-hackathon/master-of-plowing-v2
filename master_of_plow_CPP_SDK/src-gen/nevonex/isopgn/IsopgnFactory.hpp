/*
 * nevonex/isopgn/IsopgnFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_ISOPGNFACTORY_HPP
#define _NEVONEX_ISOPGNFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/isopgn.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace isopgn
    {

        class EXPORT_NEVONEX_DLL IsopgnFactory : public virtual ::ecore::EFactory
        {
        public:

            static IsopgnFactory_ptr _instance();

            virtual ISOPGN_ptr createISOPGN();
            virtual ISOPGNProvider_ptr createISOPGNProvider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< IsopgnFactory > s_holder;

            IsopgnFactory();

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

        template< > inline ISOPGN_ptr create< ISOPGN >()
        {
            auto eFactory = IsopgnPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< IsopgnFactory* >(eFactory.get());
            return packageFactory->createISOPGN();
        }

        template< > inline ISOPGNProvider_ptr create< ISOPGNProvider >()
        {
            auto eFactory = IsopgnPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< IsopgnFactory* >(eFactory.get());
            return packageFactory->createISOPGNProvider();
        }

    } // isopgn
} // nevonex

#endif // _NEVONEX_ISOPGNFACTORY_HPP
