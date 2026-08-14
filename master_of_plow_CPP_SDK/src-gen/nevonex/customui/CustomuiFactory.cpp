/*
 * nevonex/customui/CustomuiFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/customui/CustomuiFactory.hpp>

using namespace ::nevonex::customui;

::ecore::Ptr< ::nevonex::customui::CustomuiFactory > CustomuiFactory::s_holder;

::nevonex::customui::CustomuiFactory_ptr CustomuiFactory::_instance()
{
    if (!s_holder.get())
        s_holder = ::ecore::Ptr < CustomuiFactory > (new CustomuiFactory());

    return s_holder;
}
