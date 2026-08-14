/*
 * nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Impl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_MTLT305.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/fcal/ACEINNA_ACCEL_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLERATE_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLES_T_R.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_MTLT305Impl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_mtlt305;
using namespace ::nevonex::log;

void CAN_AGMO_MTLT305::_initialize()
{
    // Supertypes
    ::nevonex::common::TopicObject::_initialize();

    // References

    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Impl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_MTLT305::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
        if (m_aceinna_Accel)
            _any = ::ecore::as < ::ecore::EObject > (m_aceinna_Accel);
        return _any;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
        if (m_aceinna_AngleRate)
            _any = ::ecore::as < ::ecore::EObject > (m_aceinna_AngleRate);
        return _any;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
        if (m_aceinna_Angles)
            _any = ::ecore::as < ::ecore::EObject > (m_aceinna_Angles);
        return _any;
    }
    }
    return ::nevonex::common::TopicObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_MTLT305::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::ACEINNA_ACCEL_T_R* >(_t0.get());
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305::setAceinna_Accel(_t1);
        return;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::ACEINNA_ANGLERATE_T_R* >(_t0.get());
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305::setAceinna_AngleRate(_t1);
        return;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::ACEINNA_ANGLES_T_R* >(_t0.get());
        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305::setAceinna_Angles(_t1);
        return;
    }
    }
    ::nevonex::common::TopicObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_MTLT305::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
        return (bool) m_aceinna_Accel;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
        return (bool) m_aceinna_AngleRate;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
        return (bool) m_aceinna_Angles;
    }
    }
    return ::nevonex::common::TopicObject::eIsSet(_featureID);
}

void CAN_AGMO_MTLT305::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
        // nullptr
        setAceinna_Accel(nullptr);
        return;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
        // nullptr
        setAceinna_AngleRate(nullptr);
        return;
    }
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
        // nullptr
        setAceinna_Angles(nullptr);
        return;
    }
    }
    ::nevonex::common::TopicObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_MTLT305::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package* >(::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::_instance().get())->getCAN_AGMO_MTLT305();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_MTLT305::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
    }
        return;
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
    }
        return;
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_MTLT305::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ACCEL:
    {
    }
        return;
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLERATE:
    {
    }
        return;
    case ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::CAN_AGMO_MTLT305__ACEINNA_ANGLES:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
