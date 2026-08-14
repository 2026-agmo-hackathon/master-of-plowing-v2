/*
 * nevonex/cloud/CloudPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include <nevonex/cloud/CloudPackage.hpp>

using namespace ::nevonex::cloud;

::ecore::Ptr< ::nevonex::cloud::CloudPackage > CloudPackage::s_instance;

::nevonex::cloud::CloudPackage_ptr CloudPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< CloudPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < CloudPackage > (new CloudPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::cloud::CloudPackage_ptr CloudPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}

