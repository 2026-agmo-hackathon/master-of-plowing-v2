/*
 * nevonex/types/TypesFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/types/TypesFactory.hpp>

using namespace ::nevonex::types;

::ecore::Ptr< ::nevonex::types::TypesFactory > TypesFactory::s_holder;

::nevonex::types::TypesFactory_ptr TypesFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < TypesFactory > (new TypesFactory());

    return s_holder;
}
