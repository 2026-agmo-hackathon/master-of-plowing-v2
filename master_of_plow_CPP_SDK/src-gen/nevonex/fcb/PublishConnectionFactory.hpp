/*
 * nevonex/fcb/PublishConnectionFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCB_PUBLISHCONNECTIONFACTORY_HPP
#define NEVONEX_FCB_PUBLISHCONNECTIONFACTORY_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/fcb_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>
#include <nevonex-fcal-platform/config/GlobalConfig.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <nevonex-fcal-platform/communicator/ICommunicator.hpp>
#include <nevonex-fcal-platform/communicator/CommunicatorProperties.hpp>
#include <rapidjson/document.h>
#include <nevonex-fcal-platform/common/CommonUtils.hpp>
#include <unordered_map>
#include <map>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EClassifier.hpp>
#include <ecorecpp/mapping/any.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <chrono>
#include <cstdint>

#include "FcbPackage.hpp"

namespace nevonex
{
    namespace fcb
    {

    class EXPORT_NEVONEX_DLL PublishConnectionFactory : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(PublishConnectionFactory_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        PublishConnectionFactory();

        virtual ~PublishConnectionFactory();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        static ::nevonex::fcb::PublishConnectionFactory_ptr getInstance();
        virtual void initConnectionFactory();
        virtual void stopPublishClient();
        void publish(const ::ecore::EString& _interfaceKey, const ::ecore::EString& _interfaceName, const std::vector< ::ecore::EObject_ptr >& _payload, ::nevonex::fcb::SubscriberEnum _subscriber);
        void publish(const ::ecore::EString& _interfaceName, ::ecore::EObject_ptr _obj, rapidjson::Value& _val, rapidjson::Document& _doc, ::nevonex::fcb::SubscriberEnum _subscriber);
        void publishFinal(::nevonex::fcb::SubscriberEnum _subscriber, const ::ecore::EString& _topic, rapidjson::Document& _doc);
        void terminateConnectionFactory();
        void setTopicMap(const std::unordered_map<std::string, std::string>& _map);
        void setPayloadMap(const std::unordered_map<std::string, std::string>& _map);
        void setPrefixMap(const std::unordered_map<std::string, std::string>& _map);
        void setMachinePathMap(const std::unordered_map<std::string, std::string>& _map);
        void setEnabledDisabledTopicMap(const std::unordered_map<std::string, bool>& _map);
        bool isWriteEnabled(const ::ecore::EString& _interfaceKey) const;
        std::vector< ::ecore::EObject_ptr > getProviders() const;
        void addProvider(::ecore::EObject_ptr _prov);
        void connection_lost(const ::ecore::EString& _cause);
        void message_arrived(const std::string& _topic, const std::string& _message);

        // Attributes
        // References
    public:

        /*PROTECTED REGION ID(PublishConnectionFactory) START*/
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

        /*PROTECTED REGION ID(PublishConnectionFactoryImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        PublishConnectionFactory_ptr _this()
        {   return PublishConnectionFactory_ptr(this);}

    private:
        // Attributes


        // References


        static ::ecore::Ptr< ::nevonex::fcb::PublishConnectionFactory > s_instance;
        std::unique_ptr< ::nevonex::communicator::ICommunicator > m_client;
        std::unordered_map<std::string, std::string> m_topicMap;
        std::unordered_map<std::string, std::string> m_payloadMap;
        std::unordered_map<std::string, std::string> m_prefixMap;
        std::unordered_map<std::string, std::string> m_machinePathMap;
        std::unordered_map<std::string, bool> m_enabledDisabledTopicMap;
        std::vector< ::ecore::EObject_ptr > m_providers;
        bool m_initialized { false };

        /*PROTECTED REGION ID(PublishConnectionFactory_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcb
}// nevonex

#endif // NEVONEX_FCB_PUBLISHCONNECTIONFACTORY_HPP
