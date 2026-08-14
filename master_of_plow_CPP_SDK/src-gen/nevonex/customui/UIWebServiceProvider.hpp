/*
 * nevonex/customui/UIWebServiceProvider.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CUSTOMUI_UIWEBSERVICEPROVIDER_HPP
#define NEVONEX_CUSTOMUI_UIWEBSERVICEPROVIDER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/customui_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include <nevonex-fcal-platform/web/server/AbstractHttpServerProvider.hpp>

#include "CustomuiPackage.hpp"

namespace nevonex
{
    namespace customui
    {

    class EXPORT_NEVONEX_DLL UIWebServiceProvider
        : public virtual ::ecore::EObject
        , public ::nevonex::web::server::AbstractHttpServerProvider
    {
        /*PROTECTED REGION ID(UIWebServiceProvider_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        UIWebServiceProvider();

        virtual ~UIWebServiceProvider();

        virtual void _initialize() override;

        // Singleton accessor
        static UIWebServiceProvider_ptr getInstance();

        // Operations from Parent(s) — AbstractHttpServerProvider
        virtual void start() override;
        virtual void stop() override;

        // Attributes
        // References
    public:

        /*PROTECTED REGION ID(UIWebServiceProvider) START*/
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

        /*PROTECTED REGION ID(UIWebServiceProviderImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        UIWebServiceProvider_ptr _this()
        {   return UIWebServiceProvider_ptr(this);}

        static UIWebServiceProvider_ptr s_holder;

    private:
        // Attributes

        // References

        /*PROTECTED REGION ID(UIWebServiceProvider_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // customui
}// nevonex

#endif // NEVONEX_CUSTOMUI_UIWEBSERVICEPROVIDER_HPP
