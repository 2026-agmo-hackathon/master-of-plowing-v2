/*
 * nevonex/types/IMachineImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "IMachine.hpp"
#include <stdexcept>

#include <nevonex/types/TypesPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(IMachineImpl.cpp) START*/
/*PROTECTED REGION END*/

using namespace ::nevonex::types;
using namespace ::nevonex::log;

void IMachine::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(IMachineImpl__initialize) START*/
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject IMachine::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void IMachine::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean IMachine::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void IMachine::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr IMachine::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getIMachine();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void IMachine::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void IMachine::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
