/*
 * nevonex/types/IMachineProviderImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "IMachineProvider.hpp"
#include <stdexcept>

#include <nevonex/types/TypesPackage.hpp>
#include <nevonex/fcb/FCALController.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(IMachineProviderImpl.cpp) START*/
/*PROTECTED REGION END*/

using namespace ::nevonex::types;
using namespace ::nevonex::log;

void IMachineProvider::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(IMachineProviderImpl__initialize) START*/
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject IMachineProvider::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
        if (m_controller)
            _any = ::ecore::as < ::ecore::EObject > (m_controller);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void IMachineProvider::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcb::FCALController_ptr _t1 =
                dynamic_cast< ::nevonex::fcb::FCALController* >(_t0.get());
        ::nevonex::types::IMachineProvider::setController(_t1);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean IMachineProvider::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
        return (bool) m_controller;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void IMachineProvider::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
        // nullptr
        setController(nullptr);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr IMachineProvider::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::types::TypesPackage* >(::nevonex::types::TypesPackage::_instance().get())->getIMachineProvider();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void IMachineProvider::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void IMachineProvider::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::IMACHINEPROVIDER__CONTROLLER:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
