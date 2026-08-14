/*
 * nevonex/customui/CustomuiPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CUSTOMUIPACKAGE_HPP
#define _NEVONEX_CUSTOMUIPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/customui_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>


#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace customui
    {

    class EXPORT_NEVONEX_DLL CustomuiPackage : public virtual ::ecore::EPackage
    {
    public:

        static CustomuiPackage_ptr _instance();
        static CustomuiPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int ABSTRACTWEBSOCKETENDPOINT = 0;

        static const int IABSTRACTWEBSOCKETENDPOINT = 1;

        static const int ICUSTOMUIFACTORY = 2;

        static const int INEVONEXROUTE = 3;

        static const int IUIWEBSERVICEPROVIDER = 4;

        static const int NEVONEXROUTE = 5;

        static const int PORTENUM = 6;

        static const int UIWEBSERVICEPROVIDER = 7;



        // EClassifiers methods

        virtual ::ecore::EClass_ptr getAbstractWebsocketEndPoint();
        virtual ::ecore::EClass_ptr getNevonexRoute();
        virtual ::ecore::EClass_ptr getUIWebServiceProvider();
        virtual ::ecore::EEnum_ptr getPortEnum();

        // EStructuralFeatures methods


    protected:

        static ::ecore::Ptr< CustomuiPackage > s_instance;

        CustomuiPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_AbstractWebsocketEndPointEClass;

        ::ecore::EClass_ptr m_NevonexRouteEClass;

        ::ecore::EClass_ptr m_UIWebServiceProviderEClass;

        // EEnuminstances 

        ::ecore::EEnum_ptr m_PortEnumEEnum;

        // EDataType instances 



        // EStructuralFeatures instances



    };

}
 // customui
}// nevonex

#endif // _NEVONEX_CUSTOMUIPACKAGE_HPP
