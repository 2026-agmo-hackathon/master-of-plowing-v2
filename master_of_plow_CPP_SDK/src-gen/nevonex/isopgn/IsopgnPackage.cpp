/*
 * nevonex/isopgn/IsopgnPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/isopgn/IsopgnPackage.hpp>

using namespace ::nevonex::isopgn;

::ecore::Ptr< ::nevonex::isopgn::IsopgnPackage > IsopgnPackage::s_instance;

::nevonex::isopgn::IsopgnPackage_ptr IsopgnPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< IsopgnPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < IsopgnPackage > (new IsopgnPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::isopgn::IsopgnPackage_ptr IsopgnPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
