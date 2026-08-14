/*
 * nevonex/fcb/FcbFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcb/FcbFactory.hpp>

using namespace ::nevonex::fcb;

::ecore::Ptr< ::nevonex::fcb::FcbFactory > FcbFactory::s_holder;

::nevonex::fcb::FcbFactory_ptr FcbFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < FcbFactory > (new FcbFactory());

    return s_holder;
}
