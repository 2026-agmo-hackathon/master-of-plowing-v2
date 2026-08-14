/*
 * nevonex/types/PropertyChangeImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "PropertyChange.hpp"
#include <stdexcept>

#include <nevonex/types/TypesPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(PropertyChangeImpl.cpp) START*/
/*PROTECTED REGION END*/

using namespace ::nevonex::types;
using namespace ::nevonex::log;

void PropertyChange::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(PropertyChangeImpl__initialize) START*/
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject PropertyChange::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        // Return empty any for vector-typed listeners
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void PropertyChange::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean PropertyChange::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        return m_listeners.size() > 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void PropertyChange::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        m_listeners.clear();
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr PropertyChange::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getPropertyChange();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void PropertyChange::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void PropertyChange::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
