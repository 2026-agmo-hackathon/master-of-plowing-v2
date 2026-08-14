/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>

using namespace ::nevonex::can_agmo_customized_tractor;

::ecore::Ptr< ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage > Can_agmo_customized_tractorPackage::s_instance;

::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage_ptr Can_agmo_customized_tractorPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< Can_agmo_customized_tractorPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < Can_agmo_customized_tractorPackage > (new Can_agmo_customized_tractorPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage_ptr Can_agmo_customized_tractorPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
