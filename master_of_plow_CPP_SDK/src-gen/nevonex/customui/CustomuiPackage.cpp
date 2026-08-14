/*
 * nevonex/customui/CustomuiPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/customui/CustomuiPackage.hpp>

using namespace ::nevonex::customui;

::ecore::Ptr< ::nevonex::customui::CustomuiPackage > CustomuiPackage::s_instance;

::nevonex::customui::CustomuiPackage_ptr CustomuiPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< CustomuiPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < CustomuiPackage > (new CustomuiPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::customui::CustomuiPackage_ptr CustomuiPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
