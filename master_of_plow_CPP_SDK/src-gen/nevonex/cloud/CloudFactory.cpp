/*
 * nevonex/cloud/CloudFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include <nevonex/cloud/CloudFactory.hpp>

using namespace ::nevonex::cloud;

::ecore::Ptr< ::nevonex::cloud::CloudFactory > CloudFactory::s_holder;

::nevonex::cloud::CloudFactory_ptr CloudFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < CloudFactory > (new CloudFactory());

    return s_holder;
}

