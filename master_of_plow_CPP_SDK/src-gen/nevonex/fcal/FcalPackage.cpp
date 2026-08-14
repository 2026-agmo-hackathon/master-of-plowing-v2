/*
 * nevonex/fcal/FcalPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcal/FcalPackage.hpp>

using namespace ::nevonex::fcal;

::ecore::Ptr< ::nevonex::fcal::FcalPackage > FcalPackage::s_instance;

::nevonex::fcal::FcalPackage_ptr FcalPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< FcalPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < FcalPackage > (new FcalPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::fcal::FcalPackage_ptr FcalPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
