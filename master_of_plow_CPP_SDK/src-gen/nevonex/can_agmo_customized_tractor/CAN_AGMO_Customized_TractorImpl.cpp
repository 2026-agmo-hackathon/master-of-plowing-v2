/*
 * nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_Customized_Tractor.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/fcal/RECEIVE_ACC_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_FNR_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_HYD_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_SFT_INFO_T_R.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(CAN_AGMO_Customized_TractorImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_customized_tractor;
using namespace ::nevonex::log;

void CAN_AGMO_Customized_Tractor::_initialize()
{
    // Supertypes
    ::nevonex::common::TopicObject::_initialize();

    // References

    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

// EObject
::ecore::EJavaObject CAN_AGMO_Customized_Tractor::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
        if (m_receive_ACC_INFO)
            _any = ::ecore::as < ::ecore::EObject > (m_receive_ACC_INFO);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
        if (m_receive_FNR_INFO)
            _any = ::ecore::as < ::ecore::EObject > (m_receive_FNR_INFO);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
        if (m_receive_HYD_INFO)
            _any = ::ecore::as < ::ecore::EObject > (m_receive_HYD_INFO);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
        if (m_receive_SFT_INFO)
            _any = ::ecore::as < ::ecore::EObject > (m_receive_SFT_INFO);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
        if (m_send_ACC_CMD)
            _any = ::ecore::as < ::ecore::EObject > (m_send_ACC_CMD);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
        if (m_send_FNR_CMD)
            _any = ::ecore::as < ::ecore::EObject > (m_send_FNR_CMD);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
        if (m_send_HYD_CMD)
            _any = ::ecore::as < ::ecore::EObject > (m_send_HYD_CMD);
        return _any;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
        if (m_send_SFT_CMD)
            _any = ::ecore::as < ::ecore::EObject > (m_send_SFT_CMD);
        return _any;
    }
    }
    return ::nevonex::common::TopicObject::eGet(_featureID, _resolve);
}

void CAN_AGMO_Customized_Tractor::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::RECEIVE_ACC_INFO_T_R* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setReceive_ACC_INFO(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::RECEIVE_FNR_INFO_T_R* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setReceive_FNR_INFO(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::RECEIVE_HYD_INFO_T_R* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setReceive_HYD_INFO(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::RECEIVE_SFT_INFO_T_R* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setReceive_SFT_INFO(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setSend_ACC_CMDInternal(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setSend_FNR_CMDInternal(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setSend_HYD_CMDInternal(_t1);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast
                < ::ecore::EObject_ptr > (_newValue);
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _t1 =
                dynamic_cast< ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W* >(_t0.get());
        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor::setSend_SFT_CMDInternal(_t1);
        return;
    }
    }
    ::nevonex::common::TopicObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean CAN_AGMO_Customized_Tractor::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
        return (bool) m_receive_ACC_INFO;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
        return (bool) m_receive_FNR_INFO;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
        return (bool) m_receive_HYD_INFO;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
        return (bool) m_receive_SFT_INFO;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
        return (bool) m_send_ACC_CMD;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
        return (bool) m_send_FNR_CMD;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
        return (bool) m_send_HYD_CMD;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
        return (bool) m_send_SFT_CMD;
    }
    }
    return ::nevonex::common::TopicObject::eIsSet(_featureID);
}

void CAN_AGMO_Customized_Tractor::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
        // nullptr
        setReceive_ACC_INFO(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
        // nullptr
        setReceive_FNR_INFO(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
        // nullptr
        setReceive_HYD_INFO(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
        // nullptr
        setReceive_SFT_INFO(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
        // nullptr
        setSend_ACC_CMD(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
        // nullptr
        setSend_FNR_CMD(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
        // nullptr
        setSend_HYD_CMD(nullptr);
        return;
    }
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
        // nullptr
        setSend_SFT_CMD(nullptr);
        return;
    }
    }
    ::nevonex::common::TopicObject::eUnset(_featureID);
}

::ecore::EClass_ptr CAN_AGMO_Customized_Tractor::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage* >(::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::_instance().get())->getCAN_AGMO_Customized_Tractor();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_Customized_Tractor::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void CAN_AGMO_Customized_Tractor::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
    {
    }
        return;
    case ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
    {
    }
        return;

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///
