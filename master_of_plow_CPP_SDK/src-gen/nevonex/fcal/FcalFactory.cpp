/*
 * nevonex/fcal/FcalFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/fcal/FcalFactory.hpp>

using namespace ::nevonex::fcal;

::ecore::Ptr< ::nevonex::fcal::FcalFactory > FcalFactory::s_holder;

::nevonex::fcal::FcalFactory_ptr FcalFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < FcalFactory > (new FcalFactory());

    return s_holder;
}
