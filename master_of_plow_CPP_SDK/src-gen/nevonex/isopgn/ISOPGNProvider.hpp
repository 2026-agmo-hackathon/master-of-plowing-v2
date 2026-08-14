/*
 * nevonex/isopgn/ISOPGNProvider.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_ISOPGN_ISOPGNPROVIDER_HPP
#define NEVONEX_ISOPGN_ISOPGNPROVIDER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/isopgn_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachineProvider.hpp>
#include <nevonex/common.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <rapidjson/document.h>
#include <cstdint>
#include <nevonex/isopgn/ISOPGN.hpp>

#include "IsopgnPackage.hpp"

namespace nevonex
{
    namespace isopgn
    {

    class EXPORT_NEVONEX_DLL ISOPGNProvider : public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachineProvider
    {
        /*PROTECTED REGION ID(ISOPGNProvider_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ISOPGNProvider();

        virtual ~ISOPGNProvider();

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
            static const std::string name("ISOPGNProvider");
            return name;
        }

        inline bool acceptDom(const ::ecore::EString & root) override
        {
            return boost::iequals("isopgn", root);
        }

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::isopgn::ISOPGN_ptr getISOPGN () const;
        /**
         * \brief 
         */
    public:
        virtual void setISOPGN (::nevonex::isopgn::ISOPGN_ptr _iSOPGN);


        /*PROTECTED REGION ID(ISOPGNProvider) START*/
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
        ISOPGNProvider_ptr _this()
        {   return ISOPGNProvider_ptr(this);}

    private:
        ::nevonex::isopgn::ISOPGN_ptr m_iSOPGN;

        std::unordered_map< ::ecore::EString, ::ecore::EClassifier_ptr > m_classifierMap;
        std::unordered_map< ::ecore::EString, ::ecore::EStructuralFeature_ptr > m_featuresMap;
        std::unordered_map< ::ecore::EString, ::nevonex::common::TopicObject_ptr > m_topicObjectMap;

        /*PROTECTED REGION ID(ISOPGNProvider_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // isopgn
}// nevonex

#endif // NEVONEX_ISOPGN_ISOPGNPROVIDER_HPP
