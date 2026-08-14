/*
 * nevonex/fcb/FcbPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcb/FcbPackage.hpp>

using namespace ::nevonex::fcb;

::ecore::Ptr< ::nevonex::fcb::FcbPackage > FcbPackage::s_instance;

::nevonex::fcb::FcbPackage_ptr FcbPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< FcbPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < FcbPackage > (new FcbPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::fcb::FcbPackage_ptr FcbPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
