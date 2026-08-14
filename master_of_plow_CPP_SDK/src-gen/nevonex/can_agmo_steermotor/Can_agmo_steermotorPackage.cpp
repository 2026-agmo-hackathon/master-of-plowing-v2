/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>

using namespace ::nevonex::can_agmo_steermotor;

::ecore::Ptr< ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage > Can_agmo_steermotorPackage::s_instance;

::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage_ptr Can_agmo_steermotorPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< Can_agmo_steermotorPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < Can_agmo_steermotorPackage > (new Can_agmo_steermotorPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage_ptr Can_agmo_steermotorPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
