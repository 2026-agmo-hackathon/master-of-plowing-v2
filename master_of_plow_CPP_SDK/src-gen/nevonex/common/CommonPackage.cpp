/*
 * nevonex/common/CommonPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/common/CommonPackage.hpp>

using namespace ::nevonex::common;

::ecore::Ptr< ::nevonex::common::CommonPackage > CommonPackage::s_instance;

::nevonex::common::CommonPackage_ptr CommonPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< CommonPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < CommonPackage > (new CommonPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::common::CommonPackage_ptr CommonPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
