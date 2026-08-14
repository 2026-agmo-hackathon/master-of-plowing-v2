/*
 * nevonex/isopgn/IsopgnFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/isopgn/IsopgnFactory.hpp>

using namespace ::nevonex::isopgn;

::ecore::Ptr< ::nevonex::isopgn::IsopgnFactory > IsopgnFactory::s_holder;

::nevonex::isopgn::IsopgnFactory_ptr IsopgnFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < IsopgnFactory > (new IsopgnFactory());

    return s_holder;
}
