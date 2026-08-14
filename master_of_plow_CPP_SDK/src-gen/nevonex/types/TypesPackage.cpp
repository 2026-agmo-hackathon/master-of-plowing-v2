/*
 * nevonex/types/TypesPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/types/TypesPackage.hpp>

using namespace ::nevonex::types;

::ecore::Ptr< ::nevonex::types::TypesPackage > TypesPackage::s_instance;

::nevonex::types::TypesPackage_ptr TypesPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< TypesPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < TypesPackage > (new TypesPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::types::TypesPackage_ptr TypesPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
