/*
 * nevonex/types/TypesPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_TYPESPACKAGE_HPP
#define _NEVONEX_TYPESPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/types_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>


#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace types
    {

    class EXPORT_NEVONEX_DLL TypesPackage : public virtual ::ecore::EPackage
    {
    public:

        static TypesPackage_ptr _instance();
        static TypesPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int FILE = 0;

        static const int IARRAYTYPE = 1;

        static const int IMACHINE = 2;

        static const int IMACHINEPROVIDER = 3;

        static const int IPROPERTYCHANGE = 4;

        static const int ITYPESFACTORY = 5;

        static const int PROPERTYCHANGE = 6;

        static const int PROPERTYCHANGE__LISTENERS = 0;

        static const int IMACHINEPROVIDER__CONTROLLER = 1;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getPropertyChange();
        virtual ::ecore::EClass_ptr getIMachine();
        virtual ::ecore::EClass_ptr getIMachineProvider();
        virtual ::ecore::EDataType_ptr getFile();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getPropertyChange__listeners();
        virtual ::ecore::EReference_ptr getIMachineProvider__controller();

    protected:

        static ::ecore::Ptr< TypesPackage > s_instance;

        TypesPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_PropertyChangeEClass;

        ::ecore::EClass_ptr m_IMachineEClass;

        ::ecore::EClass_ptr m_IMachineProviderEClass;

        // EEnuminstances 



        // EDataType instances 

        ::ecore::EDataType_ptr m_FileEDataType;

        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_PropertyChange__listeners;

        ::ecore::EReference_ptr m_IMachineProvider__controller;

    };

}
 // types
}// nevonex

#endif // _NEVONEX_TYPESPACKAGE_HPP
