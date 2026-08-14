/*
 * nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_Tractor.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTOR_CAN_AGMO_CUSTOMIZED_TRACTOR_HPP
#define NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTOR_CAN_AGMO_CUSTOMIZED_TRACTOR_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/can_agmo_customized_tractor_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachine.hpp>
#include <nevonex/fcal/RECEIVE_ACC_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_FNR_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_HYD_INFO_T_R.hpp>
#include <nevonex/fcal/RECEIVE_SFT_INFO_T_R.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>
#include <nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp>

#include "Can_agmo_customized_tractorPackage.hpp"

namespace nevonex
{
    namespace can_agmo_customized_tractor
    {

    class EXPORT_NEVONEX_DLL CAN_AGMO_Customized_Tractor : public virtual ::nevonex::common::TopicObject,
        public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachine
    {
        /*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        CAN_AGMO_Customized_Tractor();

        virtual ~CAN_AGMO_Customized_Tractor();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::ELong getReceive_ACC_INFO_Timestamp();
        virtual ::ecore::EBoolean isReceive_ACC_INFO_Valid();
        virtual ::ecore::ELong getReceive_FNR_INFO_Timestamp();
        virtual ::ecore::EBoolean isReceive_FNR_INFO_Valid();
        virtual ::ecore::ELong getReceive_HYD_INFO_Timestamp();
        virtual ::ecore::EBoolean isReceive_HYD_INFO_Valid();
        virtual ::ecore::ELong getReceive_SFT_INFO_Timestamp();
        virtual ::ecore::EBoolean isReceive_SFT_INFO_Valid();
        virtual ::ecore::ELong getSend_ACC_CMD_Timestamp();
        virtual ::ecore::EBoolean isSend_ACC_CMD_Valid();
        virtual ::ecore::ELong getSend_FNR_CMD_Timestamp();
        virtual ::ecore::EBoolean isSend_FNR_CMD_Valid();
        virtual ::ecore::ELong getSend_HYD_CMD_Timestamp();
        virtual ::ecore::EBoolean isSend_HYD_CMD_Valid();
        virtual ::ecore::ELong getSend_SFT_CMD_Timestamp();
        virtual ::ecore::EBoolean isSend_SFT_CMD_Valid();

        virtual ::ecore::EBoolean updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp);

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr getReceive_ACC_INFO () const;
        /**
         * \brief 
         */
    public:
        virtual void setReceive_ACC_INFO (::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr _receive_ACC_INFO);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr getReceive_FNR_INFO () const;
        /**
         * \brief 
         */
    public:
        virtual void setReceive_FNR_INFO (::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr _receive_FNR_INFO);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr getReceive_HYD_INFO () const;
        /**
         * \brief 
         */
    public:
        virtual void setReceive_HYD_INFO (::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr _receive_HYD_INFO);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr getReceive_SFT_INFO () const;
        /**
         * \brief 
         */
    public:
        virtual void setReceive_SFT_INFO (::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr _receive_SFT_INFO);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr getSend_ACC_CMD () const;
        /**
         * \brief 
         */
    public:
        virtual void setSend_ACC_CMD (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_ACC_CMD);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr getSend_FNR_CMD () const;
        /**
         * \brief 
         */
    public:
        virtual void setSend_FNR_CMD (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_FNR_CMD);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr getSend_HYD_CMD () const;
        /**
         * \brief 
         */
    public:
        virtual void setSend_HYD_CMD (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_HYD_CMD);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr getSend_SFT_CMD () const;
        /**
         * \brief 
         */
    public:
        virtual void setSend_SFT_CMD (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_SFT_CMD);

    protected:
        void setSend_ACC_CMDInternal (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_ACC_CMD);
        void setSend_FNR_CMDInternal (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_FNR_CMD);
        void setSend_HYD_CMDInternal (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_HYD_CMD);
        void setSend_SFT_CMDInternal (::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr _send_SFT_CMD);


        /*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    protected:
        virtual ::ecore::EJavaObject eGet ( ::ecore::EInt _featureID, ::ecore::EBoolean _resolve) override;
        virtual void eSet ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual ::ecore::EBoolean eIsSet ( ::ecore::EInt _featureID) override;
        virtual void eUnset ( ::ecore::EInt _featureID) override;
        virtual ::ecore::EClass_ptr _eClass () override;
        virtual void _inverseAdd ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual void _inverseRemove ( ::ecore::EInt _featureID, ::ecore::EJavaObject const& _oldValue) override;
        // _setEContainer();
        using ::ecore::EObject::eIsProxy;
        using ::ecore::EObject::eResource;
        using ::ecore::EObject::eContainer;
        using ::ecore::EObject::eContainingFeature;
        using ::ecore::EObject::eContainmentFeature;
        using ::ecore::EObject::eContents;
        using ::ecore::EObject::eAllContents;
        using ::ecore::EObject::eCrossReferences;
        using ::ecore::EObject::eInvoke;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

        /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        CAN_AGMO_Customized_Tractor_ptr _this()
        {   return CAN_AGMO_Customized_Tractor_ptr(this);}

        friend CAN_AGMO_Customized_TractorProvider;

    private:
        // Attributes

        // References
        ::nevonex::fcal::RECEIVE_ACC_INFO_T_R_ptr m_receive_ACC_INFO;
        ::nevonex::fcal::RECEIVE_FNR_INFO_T_R_ptr m_receive_FNR_INFO;
        ::nevonex::fcal::RECEIVE_HYD_INFO_T_R_ptr m_receive_HYD_INFO;
        ::nevonex::fcal::RECEIVE_SFT_INFO_T_R_ptr m_receive_SFT_INFO;
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr m_send_ACC_CMD;
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr m_send_FNR_CMD;
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr m_send_HYD_CMD;
        ::nevonex::fcal::AGMO_GENERIC_SEND_CMD_T_W_ptr m_send_SFT_CMD;

        std::map<int, ::nevonex::types::InterfaceDetails> m_interfaceDetailMap;

        /*PROTECTED REGION ID(CAN_AGMO_Customized_Tractor_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // can_agmo_customized_tractor
}// nevonex

#endif // NEVONEX_CAN_AGMO_CUSTOMIZED_TRACTOR_CAN_AGMO_CUSTOMIZED_TRACTOR_HPP
