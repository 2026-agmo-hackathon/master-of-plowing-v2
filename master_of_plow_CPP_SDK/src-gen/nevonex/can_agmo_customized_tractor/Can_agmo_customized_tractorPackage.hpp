/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORPACKAGE_HPP
#define _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/can_agmo_customized_tractor_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/common/CommonPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace can_agmo_customized_tractor
    {

    class EXPORT_NEVONEX_DLL Can_agmo_customized_tractorPackage : public virtual ::ecore::EPackage
    {
    public:

        static Can_agmo_customized_tractorPackage_ptr _instance();
        static Can_agmo_customized_tractorPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR = 0;

        static const int CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER = 1;

        static const int ICAN_AGMO_CUSTOMIZED_TRACTOR = 2;

        static const int ICAN_AGMO_CUSTOMIZED_TRACTORFACTORY = 3;

        static const int ICAN_AGMO_CUSTOMIZED_TRACTORPROVIDER = 4;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO = 4;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO = 5;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO = 6;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO = 7;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD = 8;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD = 9;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD = 10;

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD = 11;

        static const int CAN_AGMO_CUSTOMIZED_TRACTORPROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR = 12;

        // IDs for classifiers for inherited features

        static const int CAN_AGMO_CUSTOMIZED_TRACTOR__INDEX = ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getCAN_AGMO_Customized_Tractor();
        virtual ::ecore::EClass_ptr getCAN_AGMO_Customized_TractorProvider();

        // EStructuralFeatures methods

        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__receive_ACC_INFO();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__receive_FNR_INFO();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__receive_HYD_INFO();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__receive_SFT_INFO();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__send_ACC_CMD();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__send_FNR_CMD();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__send_HYD_CMD();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_Tractor__send_SFT_CMD();
        virtual ::ecore::EReference_ptr getCAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor();

    protected:

        static ::ecore::Ptr< Can_agmo_customized_tractorPackage > s_instance;

        Can_agmo_customized_tractorPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_CAN_AGMO_Customized_TractorEClass;

        ::ecore::EClass_ptr m_CAN_AGMO_Customized_TractorProviderEClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__receive_ACC_INFO;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__receive_FNR_INFO;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__receive_HYD_INFO;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__receive_SFT_INFO;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__send_ACC_CMD;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__send_FNR_CMD;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__send_HYD_CMD;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_Tractor__send_SFT_CMD;

        ::ecore::EReference_ptr m_CAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor;

    };

}
 // can_agmo_customized_tractor
}// nevonex

#endif // _NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTORPACKAGE_HPP
