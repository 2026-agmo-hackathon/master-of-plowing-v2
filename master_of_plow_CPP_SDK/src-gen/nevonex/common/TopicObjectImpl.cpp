/*
 * nevonex/common/TopicObjectImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "TopicObject.hpp"
#include <stdexcept>

#include <nevonex/common/CommonPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(TopicObjectImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::common;
using namespace ::nevonex::log;

void TopicObject::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(TopicObjectImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject TopicObject::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX:
    {
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::toAny(_any, m_index);
        return _any;
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void TopicObject::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX:
    {
        ::ecore::EInt _t0;
        ::ecorecpp::mapping::any_traits < ::ecore::EInt
                > ::fromAny(_newValue, _t0);
        ::nevonex::common::TopicObject::setIndex(_t0);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean TopicObject::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX:
    {
        return m_index != 0;
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void TopicObject::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX:
    {
        // 0
        setIndex(0);
        return;
    }
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr TopicObject::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void TopicObject::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void TopicObject::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
