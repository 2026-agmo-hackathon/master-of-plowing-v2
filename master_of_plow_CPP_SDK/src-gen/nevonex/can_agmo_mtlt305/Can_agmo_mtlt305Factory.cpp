/*
 * nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.hpp>

using namespace ::nevonex::can_agmo_mtlt305;

::ecore::Ptr< ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Factory > Can_agmo_mtlt305Factory::s_holder;

::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Factory_ptr Can_agmo_mtlt305Factory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < Can_agmo_mtlt305Factory > (new Can_agmo_mtlt305Factory());

    return s_holder;
}
