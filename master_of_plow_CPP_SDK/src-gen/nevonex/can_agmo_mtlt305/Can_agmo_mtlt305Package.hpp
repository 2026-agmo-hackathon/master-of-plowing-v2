/*
 * nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_MTLT305PACKAGE_HPP
#define _NEVONEX_CAN_AGMO_MTLT305PACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/can_agmo_mtlt305_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/common/CommonPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_mtlt305
    {

    class EXPORT_NEVONEX_DLL Can_agmo_mtlt305Package : public virtual ::ecore::EPackage
    {
    public:

        static Can_agmo_mtlt305Package_ptr _instance();
        static Can_agmo_mtlt305Package_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int CAN_AGMO_MTLT305 = 0;

        static const int CAN_AGMO_MTLT305PROVIDER = 1;

        static const int ICAN_AGMO_MTLT305 = 2;

        static const int ICAN_AGMO_MTLT305FACTORY = 3;

        static const int ICAN_AGMO_MTLT305PROVIDER = 4;

        static const int CAN_AGMO_MTLT305__ACEINNA_ACCEL = 4;

        static const int CAN_AGMO_MTLT305__ACEINNA_ANGLERATE = 5;

        static const int CAN_AGMO_MTLT305__ACEINNA_ANGLES = 6;

        static const int CAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305 = 7;

        // IDs for classifiers for inherited features

        static const int CAN_AGMO_MTLT305__INDEX = ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getCAN_AGMO_MTLT305();
        virtual ::ecore::EClass_ptr getCAN_AGMO_MTLT305Provider();

        // EStructuralFeatures methods

        virtual ::ecore::EReference_ptr getCAN_AGMO_MTLT305__aceinna_Accel();
        virtual ::ecore::EReference_ptr getCAN_AGMO_MTLT305__aceinna_AngleRate();
        virtual ::ecore::EReference_ptr getCAN_AGMO_MTLT305__aceinna_Angles();
        virtual ::ecore::EReference_ptr getCAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305();

    protected:

        static ::ecore::Ptr< Can_agmo_mtlt305Package > s_instance;

        Can_agmo_mtlt305Package();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_CAN_AGMO_MTLT305EClass;

        ::ecore::EClass_ptr m_CAN_AGMO_MTLT305ProviderEClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EReference_ptr m_CAN_AGMO_MTLT305__aceinna_Accel;

        ::ecore::EReference_ptr m_CAN_AGMO_MTLT305__aceinna_AngleRate;

        ::ecore::EReference_ptr m_CAN_AGMO_MTLT305__aceinna_Angles;

        ::ecore::EReference_ptr m_CAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305;

    };

}
 // can_agmo_mtlt305
}// nevonex

#endif // _NEVONEX_CAN_AGMO_MTLT305PACKAGE_HPP
