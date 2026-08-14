/*
 * nevonex/exception/ExceptionPackage.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/exception/ExceptionPackage.hpp>

using namespace ::nevonex::exception;

::ecore::Ptr< ::nevonex::exception::ExceptionPackage > ExceptionPackage::s_instance;

::nevonex::exception::ExceptionPackage_ptr ExceptionPackage::_instance()
{
    static bool duringConstruction = false;
    if (!s_instance.get())
    {
        if (duringConstruction)
            return ::ecore::Ptr< ExceptionPackage >();
        duringConstruction = true;
        s_instance = ::ecore::Ptr < ExceptionPackage > (new ExceptionPackage());
        s_instance->_initPackage();
        duringConstruction = false;
    }

    return s_instance;
}

::nevonex::exception::ExceptionPackage_ptr ExceptionPackage::_getInstanceAndRemoveOwnership()
{
    return _instance();
}
