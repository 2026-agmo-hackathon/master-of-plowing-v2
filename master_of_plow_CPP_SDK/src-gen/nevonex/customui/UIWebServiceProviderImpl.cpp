/*
 * nevonex/customui/UIWebServiceProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "UIWebServiceProvider.hpp"
#include <stdexcept>

#include <nevonex/customui/CustomuiPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(UIWebServiceProviderImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::customui;
using namespace ::nevonex::log;

// Singleton storage
UIWebServiceProvider_ptr UIWebServiceProvider::s_holder;

UIWebServiceProvider_ptr UIWebServiceProvider::getInstance()
{
    if (!s_holder.get())
    {
        s_holder = UIWebServiceProvider_ptr(new UIWebServiceProvider());
    }
    return s_holder;
}

void UIWebServiceProvider::start()
{
    ::nevonex::web::server::AbstractHttpServerProvider::start();
}

void UIWebServiceProvider::stop()
{
    ::nevonex::web::server::AbstractHttpServerProvider::stop();
}

void UIWebServiceProvider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(UIWebServiceProviderImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject UIWebServiceProvider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void UIWebServiceProvider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean UIWebServiceProvider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void UIWebServiceProvider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr UIWebServiceProvider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::customui::CustomuiPackage* >(::nevonex::customui::CustomuiPackage::_instance().get())->getUIWebServiceProvider();
    return _eclass;
}

void UIWebServiceProvider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

void UIWebServiceProvider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}
