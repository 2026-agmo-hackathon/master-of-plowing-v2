/*
 * nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.hpp>

using namespace ::nevonex::can_agmo_steermotor;

::ecore::Ptr< ::nevonex::can_agmo_steermotor::Can_agmo_steermotorFactory > Can_agmo_steermotorFactory::s_holder;

::nevonex::can_agmo_steermotor::Can_agmo_steermotorFactory_ptr Can_agmo_steermotorFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < Can_agmo_steermotorFactory > (new Can_agmo_steermotorFactory());

    return s_holder;
}
