/*
 * nevonex/fcb/SubscribeConnectionFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCB_SUBSCRIBECONNECTIONFACTORY_HPP
#define NEVONEX_FCB_SUBSCRIBECONNECTIONFACTORY_HPP

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
#include <mqtt/async_client.h>
#include <mqtt/callback.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>
#include <mqtt/message.h>
#include <nevonex-fcal-platform/notify/NotificationManager.hpp>
#include <nevonex/types/IMachineProvider.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <rapidjson/document.h>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <sstream>
#include <unistd.h>
#include <nevonex/cloud/Cloud.hpp>

#include "FcbPackage.hpp"

namespace nevonex
{
    namespace fcb
    {

    class EXPORT_NEVONEX_DLL SubscribeConnectionFactory : public virtual ::ecore::EObject, public virtual ::mqtt::callback
    {
        /*PROTECTED REGION ID(SubscribeConnectionFactory_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        SubscribeConnectionFactory();

        virtual ~SubscribeConnectionFactory();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        static ::nevonex::fcb::SubscribeConnectionFactory_ptr getInstance();
        virtual void initConnectionFactory();
        virtual void stopSubscribeClient();
        virtual void connected(const std::string& _cause) override;
        virtual void connection_lost(const std::string& _cause) override;
        virtual void message_arrived(::mqtt::const_message_ptr _msg) override;
        virtual void delivery_complete(::mqtt::delivery_token_ptr _tok) override;
        void setTopicMap(const std::unordered_map<std::string, std::string>& _map);
        void setPrefixMap(const std::unordered_map<std::string, std::string>& _map);
        void setMachinePathMap(const std::unordered_map<std::string, std::string>& _map);
        std::vector< ::ecore::EObject_ptr > getProviders() const;
        void addProvider(::ecore::EObject_ptr _prov);
        void processMessage(const std::string& _topic, const rapidjson::Value& _payload, std::int64_t _timestamp);

        // Attributes
        // References
    public:

        /*PROTECTED REGION ID(SubscribeConnectionFactory) START*/
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

        /*PROTECTED REGION ID(SubscribeConnectionFactoryImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        SubscribeConnectionFactory_ptr _this()
        {   return SubscribeConnectionFactory_ptr(this);}

    private:
        // Attributes


        // References


        static ::ecore::Ptr< ::nevonex::fcb::SubscribeConnectionFactory > s_instance;
        std::unique_ptr< ::mqtt::async_client > m_client;
        ::mqtt::connect_options m_connOpts;
        ::mqtt::ssl_options m_sslOpts;
        std::mutex m_connectMutex;
        std::condition_variable m_connectCv;
        std::atomic<bool> m_connected { false };
        std::unordered_map<std::string, std::string> m_topicMap;
        std::unordered_map<std::string, std::string> m_prefixMap;
        std::unordered_map<std::string, std::string> m_machinePathMap;
        std::vector< ::ecore::EObject_ptr > m_providers;
        bool m_initialized { false };

        /*PROTECTED REGION ID(SubscribeConnectionFactory_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcb
}// nevonex

#endif // NEVONEX_FCB_SUBSCRIBECONNECTIONFACTORY_HPP
