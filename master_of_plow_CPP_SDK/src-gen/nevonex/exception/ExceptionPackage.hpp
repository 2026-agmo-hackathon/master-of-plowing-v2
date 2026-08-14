/*
 * nevonex/exception/ExceptionPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_EXCEPTIONPACKAGE_HPP
#define _NEVONEX_EXCEPTIONPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/exception_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>


#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace exception
    {

    class EXPORT_NEVONEX_DLL ExceptionPackage : public virtual ::ecore::EPackage
    {
    public:

        static ExceptionPackage_ptr _instance();
        static ExceptionPackage_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int IEXCEPTIONFACTORY = 0;



        // EClassifiers methods


        // EStructuralFeatures methods


    protected:

        static ::ecore::Ptr< ExceptionPackage > s_instance;

        ExceptionPackage();
        void _initPackage();

        // EClass instances 



        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances



    };

}
 // exception
}// nevonex

#endif // _NEVONEX_EXCEPTIONPACKAGE_HPP
