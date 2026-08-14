/*
 * nevonex/exception/ExceptionFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/exception/ExceptionFactory.hpp>

using namespace ::nevonex::exception;

::ecore::Ptr< ::nevonex::exception::ExceptionFactory > ExceptionFactory::s_holder;

::nevonex::exception::ExceptionFactory_ptr ExceptionFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < ExceptionFactory > (new ExceptionFactory());

    return s_holder;
}
