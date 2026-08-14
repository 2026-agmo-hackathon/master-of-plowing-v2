/*
 * nevonex/cloud/ICloudDownloadImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include "ICloudDownload.hpp"
#include <stdexcept>

#include <nevonex/cloud/CloudPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

#include <nevonex/fcb.hpp>

using namespace ::nevonex::cloud;
using namespace ::nevonex::log;

void ICloudDownload::start()
{
    using namespace ::nevonex::fcb;
    FcbFactory_ptr fcbFactory = FcbFactory::_instance();
    SubscribeConnectionFactory_ptr subConnectionFactory =
            fcbFactory->createSubscribeConnectionFactory();
    subConnectionFactory->initConnectionFactory();
}

void ICloudDownload::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ICloudDownloadImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject ICloudDownload::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void ICloudDownload::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean ICloudDownload::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void ICloudDownload::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr ICloudDownload::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::cloud::CloudPackage* >(::nevonex::cloud::CloudPackage::_instance().get())->getICloudDownload();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void ICloudDownload::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void ICloudDownload::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///

