/*
 * nevonex/cloud/ICloudDownload.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#ifndef NEVONEX_CLOUD_ICLOUDDOWNLOAD_HPP
#define NEVONEX_CLOUD_ICLOUDDOWNLOAD_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/cloud_forward.hpp>

#include "CloudPackage.hpp"

#include <nevonex/feature/AbstractCloudDownload.hpp>
#include <nevonex-fcal-platform/web/cloud/ICloudDownload.hpp>

namespace nevonex
{
    namespace cloud
    {

    class EXPORT_NEVONEX_DLL ICloudDownload : public virtual ::ecore::EObject , public virtual ::nevonex::feature::AbstractCloudDownload, public virtual ::nevonex::web::cloud::ICloudDownload
    {
        /*PROTECTED REGION ID(ICloudDownload_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ICloudDownload();

        virtual ~ICloudDownload();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
    public:

        /*PROTECTED REGION ID(ICloudDownload) START*/
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

    protected:
        virtual void start () override;

    protected:
        ICloudDownload_ptr _this()
        {   return ICloudDownload_ptr(this);}

    private:
        // Attributes

        // References

        /*PROTECTED REGION ID(ICloudDownload_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // cloud
}// nevonex

#endif // NEVONEX_CLOUD_ICLOUDDOWNLOAD_HPP

