/*
 * nevonex/common/CommonPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_COMMONPACKAGE_HPP
#define _NEVONEX_COMMONPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/common_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>


#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace common
    {

    class EXPORT_NEVONEX_DLL CommonPackage : public virtual ::ecore::EPackage
    {
    public:

        static CommonPackage_ptr _instance();
        static CommonPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int ABSOLUTEPOSITION = 0;

        static const int CONNECTIONTYPEENUM = 1;

        static const int HMISERVICESENUM = 2;

        static const int IABSOLUTEPOSITION = 3;

        static const int ICOMMONFACTORY = 4;

        static const int ITOPICOBJECT = 5;

        static const int PLATFORMSERVICESENUM = 6;

        static const int PROVIDERENUM = 7;

        static const int TOPICOBJECT = 8;

        static const int ABSOLUTEPOSITION__ALTITUDE = 0;

        static const int ABSOLUTEPOSITION__LATITUDE = 1;

        static const int ABSOLUTEPOSITION__LONGITUDE = 2;

        static const int TOPICOBJECT__INDEX = 3;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getAbsolutePosition();
        virtual ::ecore::EClass_ptr getTopicObject();
        virtual ::ecore::EEnum_ptr getProviderEnum();
        virtual ::ecore::EEnum_ptr getConnectionTypeEnum();
        virtual ::ecore::EEnum_ptr getPlatformServicesEnum();
        virtual ::ecore::EEnum_ptr getHMIServicesEnum();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getAbsolutePosition__altitude();
        virtual ::ecore::EAttribute_ptr getAbsolutePosition__latitude();
        virtual ::ecore::EAttribute_ptr getAbsolutePosition__longitude();
        virtual ::ecore::EAttribute_ptr getTopicObject__index();

    protected:

        static ::ecore::Ptr< CommonPackage > s_instance;

        CommonPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_AbsolutePositionEClass;

        ::ecore::EClass_ptr m_TopicObjectEClass;

        // EEnuminstances 

        ::ecore::EEnum_ptr m_ProviderEnumEEnum;

        ::ecore::EEnum_ptr m_ConnectionTypeEnumEEnum;

        ::ecore::EEnum_ptr m_PlatformServicesEnumEEnum;

        ::ecore::EEnum_ptr m_HMIServicesEnumEEnum;

        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_AbsolutePosition__altitude;

        ::ecore::EAttribute_ptr m_AbsolutePosition__latitude;

        ::ecore::EAttribute_ptr m_AbsolutePosition__longitude;

        ::ecore::EAttribute_ptr m_TopicObject__index;

    };

}
 // common
}// nevonex

#endif // _NEVONEX_COMMONPACKAGE_HPP
