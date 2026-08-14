/*
 * nevonex/fcb/FcbPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCBPACKAGE_HPP
#define _NEVONEX_FCBPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/fcb_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>


#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace fcb
    {

    class EXPORT_NEVONEX_DLL FcbPackage : public virtual ::ecore::EPackage
    {
    public:

        static FcbPackage_ptr _instance();
        static FcbPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int CONNECTIONFACTORY = 0;

        static const int FCALCONTROLLER = 1;

        static const int ICONNECTIONFACTORY = 2;

        static const int IFCALCONTROLLER = 3;

        static const int IFCBFACTORY = 4;

        static const int IPUBLISHCONNECTIONFACTORY = 5;

        static const int ISUBSCRIBECONNECTIONFACTORY = 6;

        static const int PUBLISHCONNECTIONFACTORY = 7;

        static const int SUBSCRIBECONNECTIONFACTORY = 8;

        static const int SUBSCRIBERENUM = 9;



        // EClassifiers methods

        virtual ::ecore::EClass_ptr getFCALController();
        virtual ::ecore::EClass_ptr getConnectionFactory();
        virtual ::ecore::EClass_ptr getPublishConnectionFactory();
        virtual ::ecore::EClass_ptr getSubscribeConnectionFactory();
        virtual ::ecore::EEnum_ptr getSubscriberEnum();

        // EStructuralFeatures methods


    protected:

        static ::ecore::Ptr< FcbPackage > s_instance;

        FcbPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_FCALControllerEClass;

        ::ecore::EClass_ptr m_ConnectionFactoryEClass;

        ::ecore::EClass_ptr m_PublishConnectionFactoryEClass;

        ::ecore::EClass_ptr m_SubscribeConnectionFactoryEClass;

        // EEnuminstances 

        ::ecore::EEnum_ptr m_SubscriberEnumEEnum;

        // EDataType instances 



        // EStructuralFeatures instances



    };

}
 // fcb
}// nevonex

#endif // _NEVONEX_FCBPACKAGE_HPP
