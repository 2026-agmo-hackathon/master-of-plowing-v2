/*
 * nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTORPROVIDER_HPP
#define NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTORPROVIDER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/can_agmo_steermotor_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachineProvider.hpp>
#include <nevonex/common.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <rapidjson/document.h>
#include <cstdint>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotor.hpp>

#include "Can_agmo_steermotorPackage.hpp"

namespace nevonex
{
    namespace can_agmo_steermotor
    {

    class EXPORT_NEVONEX_DLL CAN_AGMO_SteerMotorProvider : public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachineProvider
    {
        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        CAN_AGMO_SteerMotorProvider();

        virtual ~CAN_AGMO_SteerMotorProvider();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);

        virtual void createMachines(std::istream & _stream) override;
        virtual void constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path) override;
        virtual ::nevonex::common::TopicObject_ptr getTopicElement(::ecore::EString const& _index) override;
        virtual void initMachineProvider() override;
        virtual void stopMachineProvider() override;

        virtual void start() override;
        virtual void stop() override;
        virtual void restart() override;
        virtual void onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr) override;
        virtual void processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp) override;

        inline const std::string& getProviderName() override
        {
            static const std::string name("CAN_AGMO_SteerMotorProvider");
            return name;
        }

        inline bool acceptDom(const ::ecore::EString & root) override
        {
            return boost::iequals("can_agmo_steermotor", root);
        }

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr getCAN_AGMO_SteerMotor () const;
        /**
         * \brief 
         */
    public:
        virtual void setCAN_AGMO_SteerMotor (::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr _cAN_AGMO_SteerMotor);


        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider) START*/
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
#endif

    protected:
        CAN_AGMO_SteerMotorProvider_ptr _this()
        {   return CAN_AGMO_SteerMotorProvider_ptr(this);}

    private:
        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotor_ptr m_cAN_AGMO_SteerMotor;

        std::unordered_map< ::ecore::EString, ::ecore::EClassifier_ptr > m_classifierMap;
        std::unordered_map< ::ecore::EString, ::ecore::EStructuralFeature_ptr > m_featuresMap;
        std::unordered_map< ::ecore::EString, ::nevonex::common::TopicObject_ptr > m_topicObjectMap;

        /*PROTECTED REGION ID(CAN_AGMO_SteerMotorProvider_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // can_agmo_steermotor
}// nevonex

#endif // NEVONEX_CAN_AGMO_STEERMOTOR_CAN_AGMO_STEERMOTORPROVIDER_HPP
