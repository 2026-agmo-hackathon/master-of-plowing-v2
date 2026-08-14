/*
 * nevonex/customui/AbstractWebsocketEndPointImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "AbstractWebsocketEndPoint.hpp"
#include <stdexcept>

#include <nevonex/customui/CustomuiPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(AbstractWebsocketEndPointImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::customui;
using namespace ::nevonex::log;

void AbstractWebsocketEndPoint::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(AbstractWebsocketEndPointImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject AbstractWebsocketEndPoint::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void AbstractWebsocketEndPoint::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean AbstractWebsocketEndPoint::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void AbstractWebsocketEndPoint::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr AbstractWebsocketEndPoint::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::customui::CustomuiPackage* >(::nevonex::customui::CustomuiPackage::_instance().get())->getAbstractWebsocketEndPoint();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void AbstractWebsocketEndPoint::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void AbstractWebsocketEndPoint::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
