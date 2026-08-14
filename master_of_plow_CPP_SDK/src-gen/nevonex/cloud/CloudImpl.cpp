/*
 * nevonex/cloud/CloudImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 * Generator version: 8.6.0.202606260437
 */

#include "Cloud.hpp"
#include <stdexcept>

#include <nevonex/cloud/CloudPackage.hpp>
#include <nevonex/cloud/ICloudDownload.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>
#include <nevonex/fcb.hpp>
#include <nevonex/common.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <nevonex-fcal-platform/common/CommonUtils.hpp>

using namespace ::nevonex::cloud;
using namespace ::nevonex::log;

Cloud_ptr Cloud::s_holder;

Cloud_ptr Cloud::getInstance()
{
    if (!s_holder.get())
        s_holder = Cloud_ptr(new Cloud());

    return s_holder;
}

void Cloud::clearDownloadFiles()
{
    m_availableFilesPair.clear();
}

std::vector< ::ecore::StringPair > const& Cloud::getAvailableFiles() const
{
    return m_availableFilesPair;
}

void Cloud::_initialize()
{
    // Supertypes
    ::nevonex::cloud::ICloudDownload::_initialize();
    ::nevonex::types::PropertyChange::_initialize();

    // References

    /*PROTECTED REGION ID(CloudImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations from Parent(s)

void Cloud::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    // PropertyChange::addPropertyChangeListener (  _listener);
    PropertyChange::addPropertyChangeListener(_listener);

}

void Cloud::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    // PropertyChange::removePropertyChangeListener (  _listener);
    PropertyChange::removePropertyChangeListener(_listener);

}

void Cloud::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    // PropertyChange::notifyPropertyChange (  _name,  _oldValue,  _newValue);
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
    // Notify property change for Cloud

}

// Operations

void Cloud::processDownloadMessage(::ecore::EString const &_message)
{

    try
    {
        using namespace ::ecore;
        using namespace ::nevonex::common::utils;
        using namespace ::rapidjson;
        Document jsonObject(kObjectType);
        CommonUtils::convertStringToJsonObject(_message.c_str(), jsonObject);

        if (jsonObject.HasMember("fileName") && jsonObject.HasMember("path"))
        {
            const EString &fPath = jsonObject["path"].GetString();
            const EString &fName = jsonObject["fileName"].GetString();
            ::nevonex::resource::FilePath filePath = importFile(
                    fPath + "/" + fName);
            notifyPropertyChange("file", nullptr, filePath);
        }
        else if (jsonObject.HasMember("content"))
        {
            const rapidjson::Value &jsonContent = jsonObject["content"];
            const EString &contentString = jsonContent.GetString();
            notifyPropertyChange("content", nullptr, contentString);
        }
        else
        {
            SDK_FCAL_LOG(SeverityLevel::warning)
                    << "Invalid download topic received for cloud.";
        }
    } catch (const std::exception &e)
    {
        SDK_FCAL_LOG(SeverityLevel::error)
                << "Exception while processing the data download message from the processDownloadMessage ."
                << e.what();
    }

}

void Cloud::stopPlatformService()
{
    /*PROTECTED REGION ID(CloudImpl_stopPlatformService) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    throw std::runtime_error(
            "UnsupportedOperationException: nevonex::cloud::Cloud::stopPlatformService");
    /*PROTECTED REGION END*/

}

::ecore::EString Cloud::uploadData(::ecore::EString const &_data,
        ::ecore::EInt _priority)
{
    return uploadData(_data, _priority,
            ::nevonex::common::ConnectionTypeEnum::WIFI);

}

::ecore::EString Cloud::uploadData(::ecore::EString const &_data,
        ::ecore::EInt _priority,
        ::nevonex::common::ConnectionTypeEnum _connectionType)
{
    return ICloud::i_uploadData(_data, _priority,
            ::nevonex::common::getLiteral(_connectionType));

}

::ecore::EString Cloud::uploadFile(::nevonex::types::File const &_file,
        ::ecore::EInt _priority)
{
    return uploadFile(_file, _priority,
            ::nevonex::common::ConnectionTypeEnum::WIFI);

}

::ecore::EString Cloud::uploadFile(::ecore::EString const &_filePath,
        ::ecore::EInt _priority)
{
    return uploadFile(_filePath, _priority,
            ::nevonex::common::ConnectionTypeEnum::WIFI);

}

::ecore::EString Cloud::uploadFile(::nevonex::types::File const &_file,
        ::ecore::EInt _priority,
        ::nevonex::common::ConnectionTypeEnum _connectionType)
{
    return ICloud::i_uploadFile(_file, _priority,
            ::nevonex::common::getLiteral(_connectionType));

}

::ecore::EString Cloud::uploadFile(::ecore::EString const &_filePath,
        ::ecore::EInt _priority,
        ::nevonex::common::ConnectionTypeEnum _connectionType)
{
    return ICloud::i_uploadFile(_filePath, _priority,
            ::nevonex::common::getLiteral(_connectionType));

}

// EObject
::ecore::EJavaObject Cloud::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        return nevonex::types::PropertyChange::eGet(_featureID, _resolve);
    }
    }
    return ::ecore::EObject::eGet(_featureID, _resolve);
}

void Cloud::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        nevonex::types::PropertyChange::eSet(_featureID, _newValue);
        return;
    }
    }
    ::ecore::EObject::eSet(_featureID, _newValue);
}

::ecore::EBoolean Cloud::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::nevonex::types::TypesPackage::PROPERTYCHANGE__LISTENERS:
    {
        return nevonex::types::PropertyChange::eIsSet(_featureID);
    }
    }
    return ::ecore::EObject::eIsSet(_featureID);
}

void Cloud::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    // FIXME: && e.unsettable also should be checked.
    }
    ::ecore::EObject::eUnset(_featureID);
}

::ecore::EClass_ptr Cloud::_eClass()
{
    static ::ecore::EClass_ptr _eclass =
            dynamic_cast< ::nevonex::cloud::CloudPackage* >(::nevonex::cloud::CloudPackage::_instance().get())->getCloud();
    return _eclass;
}

/** Set the local end of a reference with an EOpposite property.
 */
void Cloud::_inverseAdd(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_newValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseAdd(_featureID, _newValue);
}

/** Unset the local end of a reference with an EOpposite property.
 */
void Cloud::_inverseRemove(::ecore::EInt _featureID,
        ::ecore::EJavaObject const &_oldValue)
{
    switch (_featureID)
    {

    }
    ::ecore::EObject::_inverseRemove(_featureID, _oldValue);
}

///

