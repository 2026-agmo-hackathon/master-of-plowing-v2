/*
 * nevonex/isopgn/IsopgnPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_ISOPGNPACKAGE_HPP
#define _NEVONEX_ISOPGNPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/isopgn_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/common/CommonPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace isopgn
    {

    class EXPORT_NEVONEX_DLL IsopgnPackage : public virtual ::ecore::EPackage
    {
    public:

        static IsopgnPackage_ptr _instance();
        static IsopgnPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int IISOPGN = 0;

        static const int IISOPGNFACTORY = 1;

        static const int IISOPGNPROVIDER = 2;

        static const int ISOPGN = 3;

        static const int ISOPGNPROVIDER = 4;

        static const int ISOPGN__HITCHPOSPERRE = 4;

        static const int ISOPGNPROVIDER__ISOPGN = 5;

        // IDs for classifiers for inherited features

        static const int ISOPGN__INDEX = ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getISOPGN();
        virtual ::ecore::EClass_ptr getISOPGNProvider();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getISOPGN__hitchPosPerRe();
        virtual ::ecore::EReference_ptr getISOPGNProvider__iSOPGN();

    protected:

        static ::ecore::Ptr< IsopgnPackage > s_instance;

        IsopgnPackage();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_ISOPGNEClass;

        ::ecore::EClass_ptr m_ISOPGNProviderEClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_ISOPGN__hitchPosPerRe;

        ::ecore::EReference_ptr m_ISOPGNProvider__iSOPGN;

    };

}
 // isopgn
}// nevonex

#endif // _NEVONEX_ISOPGNPACKAGE_HPP
