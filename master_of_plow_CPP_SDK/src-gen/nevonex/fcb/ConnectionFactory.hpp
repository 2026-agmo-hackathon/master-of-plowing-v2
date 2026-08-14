/*
 * nevonex/fcb/ConnectionFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCB_CONNECTIONFACTORY_HPP
#define NEVONEX_FCB_CONNECTIONFACTORY_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/fcb_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include "FcbPackage.hpp"

namespace nevonex
{
    namespace fcb
    {

    class EXPORT_NEVONEX_DLL ConnectionFactory : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(ConnectionFactory_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ConnectionFactory();

        virtual ~ConnectionFactory();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        // References
    public:

        /*PROTECTED REGION ID(ConnectionFactory) START*/
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

        /*PROTECTED REGION ID(ConnectionFactoryImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        ConnectionFactory_ptr _this()
        {   return ConnectionFactory_ptr(this);}

    private:
        // Attributes


        // References


        /*PROTECTED REGION ID(ConnectionFactory_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcb
}// nevonex

#endif // NEVONEX_FCB_CONNECTIONFACTORY_HPP
