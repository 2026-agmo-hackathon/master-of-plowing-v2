/*
 * nevonex/common/CommonFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/common/CommonFactory.hpp>

using namespace ::nevonex::common;

::ecore::Ptr< ::nevonex::common::CommonFactory > CommonFactory::s_holder;

::nevonex::common::CommonFactory_ptr CommonFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < CommonFactory > (new CommonFactory());

    return s_holder;
}
