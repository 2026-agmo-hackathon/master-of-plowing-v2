/*
 * nevonex/NevonexPackage.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEXPACKAGE_HPP
#define _NEVONEXPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{

class EXPORT_NEVONEX_DLL NevonexPackage : public virtual ::ecore::EPackage
{
public:

    static NevonexPackage_ptr _instance();
    static NevonexPackage_ptr _getInstanceAndRemoveOwnership();

    // IDs for classifiers

    // EClassifiers methods

    // EStructuralFeatures methods

protected:

    static ::ecore::Ptr< NevonexPackage > s_instance;

    NevonexPackage();
    void _initPackage();

    // EClass instances 

    // EEnuminstances 

    // EDataType instances 

    // EStructuralFeatures instances

};

}
 // nevonex

#endif // _NEVONEXPACKAGE_HPP
