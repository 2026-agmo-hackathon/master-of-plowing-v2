/*
 * nevonex/isopgn/ISOPGN.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_ISOPGN_ISOPGN_HPP
#define NEVONEX_ISOPGN_ISOPGN_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/isopgn_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachine.hpp>

#include "IsopgnPackage.hpp"

namespace nevonex
{
    namespace isopgn
    {

    class EXPORT_NEVONEX_DLL ISOPGN : public virtual ::nevonex::common::TopicObject,
        public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachine
    {
        /*PROTECTED REGION ID(ISOPGN_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ISOPGN();

        virtual ~ISOPGN();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::ELong getHitchPosPerRe_Timestamp();
        virtual ::ecore::EBoolean isHitchPosPerRe_Valid();

        virtual ::ecore::EBoolean updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp);

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getHitchPosPerRe () const;
        /**
         * \brief 
         */
    public:
        virtual void setHitchPosPerRe (::ecore::EFloat _hitchPosPerRe);


        /*PROTECTED REGION ID(ISOPGN) START*/
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

        /*PROTECTED REGION ID(ISOPGNImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        ISOPGN_ptr _this()
        {   return ISOPGN_ptr(this);}

        friend ISOPGNProvider;

    private:
        // Attributes

        // References
        ::ecore::EFloat m_hitchPosPerRe;

        std::map<int, ::nevonex::types::InterfaceDetails> m_interfaceDetailMap;

        /*PROTECTED REGION ID(ISOPGN_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // isopgn
}// nevonex

#endif // NEVONEX_ISOPGN_ISOPGN_HPP
