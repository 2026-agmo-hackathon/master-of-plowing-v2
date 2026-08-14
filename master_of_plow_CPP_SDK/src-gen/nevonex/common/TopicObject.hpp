/*
 * nevonex/common/TopicObject.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_TOPICOBJECT_HPP
#define NEVONEX_COMMON_TOPICOBJECT_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/common_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include "CommonPackage.hpp"

namespace nevonex
{
    namespace common
    {

    class EXPORT_NEVONEX_DLL TopicObject : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(TopicObject_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        TopicObject();

        virtual ~TopicObject();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getIndex () const;
        /**
         * \brief 
         */
    public:
        virtual void setIndex (::ecore::EInt _index);

        // References
    public:

        /*PROTECTED REGION ID(TopicObject) START*/
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

        /*PROTECTED REGION ID(TopicObjectImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        TopicObject_ptr _this()
        {   return TopicObject_ptr(this);}

    private:
        // Attributes

        ::ecore::EInt m_index;

        // References


        /*PROTECTED REGION ID(TopicObject_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // common
}// nevonex

#endif // NEVONEX_COMMON_TOPICOBJECT_HPP
