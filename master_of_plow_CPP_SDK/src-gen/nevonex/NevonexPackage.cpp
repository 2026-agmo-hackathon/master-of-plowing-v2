/*
 * nevonex/NevonexPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/NevonexPackage.hpp>

using namespace ::nevonex;

::ecore::Ptr< ::nevonex::NevonexPackage > NevonexPackage::s_instance;

::nevonex::NevonexPackage_ptr NevonexPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< NevonexPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < NevonexPackage > (new NevonexPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::NevonexPackage_ptr NevonexPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
