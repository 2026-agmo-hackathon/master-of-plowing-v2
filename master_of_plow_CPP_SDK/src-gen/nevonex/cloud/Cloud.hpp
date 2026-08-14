/*
 * nevonex/cloud/Cloud.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#ifndef NEVONEX_CLOUD_CLOUD_HPP
#define NEVONEX_CLOUD_CLOUD_HPP

#include <ecorecpp/mapping_forward.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/cloud_forward.hpp>

#include <nevonex/types_forward.hpp>
#include <ecore_forward.hpp>
#include <nevonex/common_forward.hpp>
#include <nevonex/cloud/ICloudDownload.hpp>
#include <nevonex/types/PropertyChange.hpp>

#include "CloudPackage.hpp"

#include <nevonex-fcal-platform/web/cloud/ICloud.hpp>

namespace nevonex
{
    namespace cloud
    {

    class EXPORT_NEVONEX_DLL Cloud : public virtual ::nevonex::cloud::ICloudDownload, public virtual ::nevonex::types::PropertyChange , public ICloud
    {
        /*PROTECTED REGION ID(Cloud_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        Cloud();

        virtual ~Cloud();

        virtual void _initialize() override;

        // Operations from Parent(s)

        virtual void addPropertyChangeListener ( ::nevonex::types::PropertyChangeListener _listener) override;

        virtual void removePropertyChangeListener ( ::nevonex::types::PropertyChangeListener _listener) override;

        virtual void notifyPropertyChange ( ::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue) override;

        // Operations

        /**
         * \brief It processes the message/file received from cloud.
         *
         * \param _message 
         * \return void 
         */
        virtual void processDownloadMessage ( ::ecore::EString const& _message);

        /**
         * \brief 
         *
         * \return void 
         */
        virtual void stopPlatformService ();

        /**
         * \brief Uploads given data to Nevonex Cloud
         *
         * \param _data Data to be uploaded
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadData ( ::ecore::EString const& _data, ::ecore::EInt _priority);

        /**
         * \brief Uploads given data to Nevonex Cloud using wifi or satellite based on the selected connection type.
         *
         * \param _data Data to be uploaded
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \param _connectionType Type of communication which should be used for uploading data. The allowable connection types are WIFI,SATELLITE. If this value is null WIFI is selected by default.
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadData ( ::ecore::EString const& _data, ::ecore::EInt _priority, ::nevonex::common::ConnectionTypeEnum _connectionType);

        /**
         * \brief Uploads given file to Nevonex Cloud
         *
         * \param _file File which has to be uploaded
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadFile ( ::nevonex::types::File const& _file, ::ecore::EInt _priority);

        /**
         * \brief Uploads given file to Nevonex Cloud
         *
         * \param _filePath Absolute Path of the file
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadFile ( ::ecore::EString const& _filePath, ::ecore::EInt _priority);

        /**
         * \brief Uploads given file to Nevonex Cloud
         *
         * \param _file File which has to be uploaded
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \param _connectionType Type of communication which should be used for uploading File. The allowable connection types are WIFI,SATELLITE. If this value is null WIFI is selected by default.
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadFile ( ::nevonex::types::File const& _file, ::ecore::EInt _priority, ::nevonex::common::ConnectionTypeEnum _connectionType);

        /**
         * \brief Uploads given file to Nevonex Cloud
         *
         * \param _filePath Absolute Path of the file
         * \param _priority Priority of the upload 1. High, 2. Medium, 3.Low
         * \param _connectionType Type of communication which should be used for uploading File. The allowable connection types are WIFI,SATELLITE. If this value is null WIFI is selected by default.
         * \return ::ecore::EString 
         */
        virtual ::ecore::EString uploadFile ( ::ecore::EString const& _filePath, ::ecore::EInt _priority, ::nevonex::common::ConnectionTypeEnum _connectionType);

        // Attributes

        // References
    public:

        /*PROTECTED REGION ID(Cloud) START*/
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

    public:
        static Cloud_ptr getInstance();
    protected:
        static Cloud_ptr s_holder;

    public:
        /**
         *
         * \deprecated since 6.0 and has no effect and FIF will handle internally removal of downloaded files.
         */
        void clearDownloadFiles() __attribute__((__deprecated__));
        /**
         * Use ::nevonex::resource::FileProvider::getInstance().retrieveAllDonwloadedFiles(::nevonex::resource::ResourceType::CLOUD);
         * \deprecated since 6.0 and has no effect.
         */
        std::vector<::ecore::StringPair> const & getAvailableFiles() const __attribute__((__deprecated__));
    private:
        std::vector< ::ecore::StringPair > m_availableFilesPair;

    protected:
        Cloud_ptr _this()
        {   return Cloud_ptr(this);}

    private:
        // Attributes

        // References

        /*PROTECTED REGION ID(Cloud_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // cloud
}// nevonex

#endif // NEVONEX_CLOUD_CLOUD_HPP

