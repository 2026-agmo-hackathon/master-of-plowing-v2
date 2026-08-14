/*
 * nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CAN_AGMO_MTLT305_CAN_AGMO_MTLT305_HPP
#define NEVONEX_CAN_AGMO_MTLT305_CAN_AGMO_MTLT305_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/can_agmo_mtlt305_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachine.hpp>
#include <nevonex/fcal/ACEINNA_ACCEL_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLERATE_T_R.hpp>
#include <nevonex/fcal/ACEINNA_ANGLES_T_R.hpp>

#include "Can_agmo_mtlt305Package.hpp"

namespace nevonex
{
    namespace can_agmo_mtlt305
    {

    class EXPORT_NEVONEX_DLL CAN_AGMO_MTLT305 : public virtual ::nevonex::common::TopicObject,
        public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachine
    {
        /*PROTECTED REGION ID(CAN_AGMO_MTLT305_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        CAN_AGMO_MTLT305();

        virtual ~CAN_AGMO_MTLT305();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::ELong getAceinna_Accel_Timestamp();
        virtual ::ecore::EBoolean isAceinna_Accel_Valid();
        virtual ::ecore::ELong getAceinna_AngleRate_Timestamp();
        virtual ::ecore::EBoolean isAceinna_AngleRate_Valid();
        virtual ::ecore::ELong getAceinna_Angles_Timestamp();
        virtual ::ecore::EBoolean isAceinna_Angles_Valid();

        virtual ::ecore::EBoolean updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp);

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr getAceinna_Accel () const;
        /**
         * \brief 
         */
    public:
        virtual void setAceinna_Accel (::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr _aceinna_Accel);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr getAceinna_AngleRate () const;
        /**
         * \brief 
         */
    public:
        virtual void setAceinna_AngleRate (::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr _aceinna_AngleRate);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr getAceinna_Angles () const;
        /**
         * \brief 
         */
    public:
        virtual void setAceinna_Angles (::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr _aceinna_Angles);


        /*PROTECTED REGION ID(CAN_AGMO_MTLT305) START*/
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

        /*PROTECTED REGION ID(CAN_AGMO_MTLT305Impl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        CAN_AGMO_MTLT305_ptr _this()
        {   return CAN_AGMO_MTLT305_ptr(this);}

        friend CAN_AGMO_MTLT305Provider;

    private:
        // Attributes

        // References
        ::nevonex::fcal::ACEINNA_ACCEL_T_R_ptr m_aceinna_Accel;
        ::nevonex::fcal::ACEINNA_ANGLERATE_T_R_ptr m_aceinna_AngleRate;
        ::nevonex::fcal::ACEINNA_ANGLES_T_R_ptr m_aceinna_Angles;

        std::map<int, ::nevonex::types::InterfaceDetails> m_interfaceDetailMap;

        /*PROTECTED REGION ID(CAN_AGMO_MTLT305_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // can_agmo_mtlt305
}// nevonex

#endif // NEVONEX_CAN_AGMO_MTLT305_CAN_AGMO_MTLT305_HPP
