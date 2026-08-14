/*
 * nevonex/NevonexFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/NevonexFactory.hpp>

using namespace ::nevonex;

::ecore::Ptr< ::nevonex::NevonexFactory > NevonexFactory::s_holder;

::nevonex::NevonexFactory_ptr NevonexFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < NevonexFactory > (new NevonexFactory());

    return s_holder;
}
