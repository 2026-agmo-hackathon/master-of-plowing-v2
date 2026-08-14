/*
 * nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.hpp>

using namespace ::nevonex::can_agmo_customized_tractor;

::ecore::Ptr< ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorFactory > Can_agmo_customized_tractorFactory::s_holder;

::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorFactory_ptr Can_agmo_customized_tractorFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < Can_agmo_customized_tractorFactory > (new Can_agmo_customized_tractorFactory());

    return s_holder;
}
