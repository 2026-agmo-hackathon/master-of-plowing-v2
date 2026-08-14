/*
 * nevonex/types/IMachineProvider.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_TYPES_IMACHINEPROVIDER_HPP
#define NEVONEX_TYPES_IMACHINEPROVIDER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/types_forward.hpp>

#include <nevonex/fcb_forward.hpp>
#include <nevonex/fcal_forward.hpp>
#include <ecore_forward.hpp>
#include <nevonex/feature/AbstractMachineProvider.hpp>
#include <nevonex/machine/MachineConnectionInfo.hpp>
#include <nevonex-fcal-platform/web/machine/IMachineDetails.hpp>
#include <nevonex/common.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <rapidjson/document.h>
#include <cstdint>

#include "TypesPackage.hpp"

namespace nevonex
{
    namespace types
    {

    class EXPORT_NEVONEX_DLL IMachineProvider : public virtual ::ecore::EObject,
        public virtual ::nevonex::feature::AbstractMachineProvider,
        public ::nevonex::web::machine::IMachineDetails
    {
    public:
        IMachineProvider();

        virtual ~IMachineProvider();

        virtual void _initialize() override;

        // Operations
        virtual void createMachines(std::istream & _stream);
        virtual void constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path);
        virtual void initMachineProvider() override;
        virtual ::nevonex::common::TopicObject_ptr getTopicElement(::ecore::EString const& _index);
        virtual void stopMachineProvider();

        virtual void start() override;
        virtual void stop() override;
        virtual void restart() override;
        virtual void onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr);
        ::nevonex::common::TopicObject_ptr getTopicObject(const ::ecore::EString & path) const;
        virtual void processMessage(const std::string&, const rapidjson::Value&, std::int64_t) {}

        // References
        virtual ::nevonex::fcb::FCALController_ptr getController() const;
        virtual void setController(::nevonex::fcb::FCALController_ptr _controller);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    protected:
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID, ::ecore::EBoolean _resolve) override;
        virtual void eSet(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID) override;
        virtual void eUnset(::ecore::EInt _featureID) override;
        virtual ::ecore::EClass_ptr _eClass() override;
        virtual void _inverseAdd(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual void _inverseRemove(::ecore::EInt _featureID, ::ecore::EJavaObject const& _oldValue) override;
#endif

    protected:
        IMachineProvider_ptr _this()
        {   return IMachineProvider_ptr(this);}
        virtual bool acceptDom(const ::ecore::EString &) { return false; }

    private:
        ::nevonex::fcb::FCALController_ptr m_controller;
        std::unordered_map< ::ecore::EString, ::nevonex::common::TopicObject_ptr > m_topicObjectMap;
    };

}
 // types
}// nevonex

#endif // NEVONEX_TYPES_IMACHINEPROVIDER_HPP
