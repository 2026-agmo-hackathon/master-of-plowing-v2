/*
 * nevonex.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "nevonex.hpp"
#include <nevonex/dllNevonex.hpp>
#include <nevonex/NevonexPackage.hpp>
#include <nevonex/NevonexFactory.hpp>
#include <nevonex/common/CommonPackage.hpp>
#include <nevonex/common/CommonFactory.hpp>
#include <nevonex/common/PlatformServicesEnum.hpp>
#include <nevonex/types/IMachineProvider.hpp>
#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp>
#include <nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorFactory.hpp>
#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp>
#include <nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Factory.hpp>
#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorPackage.hpp>
#include <nevonex/can_agmo_steermotor/Can_agmo_steermotorFactory.hpp>
#include <nevonex/isopgn/IsopgnPackage.hpp>
#include <nevonex/isopgn/IsopgnFactory.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.hpp>
#include <nevonex/customui/UIWebServiceProvider.hpp>

#include <ecore/EPackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EClassifier.hpp>
#include <ecore/EObject.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vector>
#include <string>
#include <set>

typedef std::vector< ::ecore::EObject_ptr > PROVIDER_LIST;

using namespace ::ecore;

EObject_ptr getNevonexPackageClass(const std::string & searchClassName)
{
    ::nevonex::NevonexPackage_ptr nevonex = ::nevonex::NevonexPackage::_instance();
    auto & subPackages = nevonex->getESubpackages();
    for (const auto & pkg : subPackages)
    {
        auto & classifiers = pkg->getEClassifiers();
        for (const auto & cls : classifiers)
        {
            if (boost::iequals(searchClassName, cls->getName()))
            {
                return pkg->getEFactoryInstance()->create(
                        ::ecore::as< EClass >(cls));
            }
        }
    }
    return nullptr;
}

extern "C" EXPORT_NEVONEX_DLL PROVIDER_LIST & getProviders(void)
{
    static PROVIDER_LIST providers;
    if (providers.empty())
    {
        {
            EObject_ptr _obj = getNevonexPackageClass("CAN_AGMO_Customized_TractorProvider");
            if (_obj && ::ecore::as< ::nevonex::types::IMachineProvider >(_obj))
            {
                providers.push_back(_obj);
            }
        }
        {
            EObject_ptr _obj = getNevonexPackageClass("CAN_AGMO_MTLT305Provider");
            if (_obj && ::ecore::as< ::nevonex::types::IMachineProvider >(_obj))
            {
                providers.push_back(_obj);
            }
        }
        {
            EObject_ptr _obj = getNevonexPackageClass("CAN_AGMO_SteerMotorProvider");
            if (_obj && ::ecore::as< ::nevonex::types::IMachineProvider >(_obj))
            {
                providers.push_back(_obj);
            }
        }
        {
            EObject_ptr _obj = getNevonexPackageClass("ISOPGNProvider");
            if (_obj && ::ecore::as< ::nevonex::types::IMachineProvider >(_obj))
            {
                providers.push_back(_obj);
            }
        }
        {
            EObject_ptr _obj = getNevonexPackageClass("Serial_Ext_GPS_NMEA0183Provider");
            if (_obj && ::ecore::as< ::nevonex::types::IMachineProvider >(_obj))
            {
                providers.push_back(_obj);
            }
        }
    }
    return providers;
}

typedef std::vector< ::ecore::EObject_ptr > WEB_SERVICE_PROVIDER_LIST;

extern "C" EXPORT_NEVONEX_DLL WEB_SERVICE_PROVIDER_LIST & getWebServiceProvidersInstance(void)
{
    static WEB_SERVICE_PROVIDER_LIST webServiceProvidersList;
    static bool webServiceProvidersInitialized = false;
    if (webServiceProvidersInitialized)
    {
        return webServiceProvidersList;
    }
    webServiceProvidersList.clear();
    {
        using namespace ::nevonex::customui;
        webServiceProvidersList.push_back(UIWebServiceProvider::getInstance());
    }
    webServiceProvidersInitialized = true;
    return webServiceProvidersList;
}

typedef std::vector< ::ecore::EObject_ptr > PLATFORM_SERVICE_PROVIDER_LIST;

extern "C" EXPORT_NEVONEX_DLL PLATFORM_SERVICE_PROVIDER_LIST & getCloudDownloadInstance(void)
{
    static PLATFORM_SERVICE_PROVIDER_LIST platformServiceProviders;
    static bool platformServiceProvidersInitialized = false;
    if (platformServiceProvidersInitialized)
    {
        return platformServiceProviders;
    }
    using namespace ::nevonex;

    platformServiceProviders.clear();

    std::set<std::string> platformServicesSet;
    for (common::PlatformServicesEnum e : common::PlatformServicesEnum_Values)
    {
        std::vector<std::string> splitted;
        boost::split(splitted, common::getLiteral(e), boost::is_any_of("_"));
        if (!splitted.empty())
        {
            platformServicesSet.emplace(splitted.at(0));
        }
    }

    for (auto platformServiceClassName : platformServicesSet)
    {
        ::ecore::EObject_ptr eObj = getNevonexPackageClass(platformServiceClassName);
        if (eObj != nullptr)
        {
            platformServiceProviders.push_back(eObj);
        }
    }

    platformServiceProvidersInitialized = true;
    return platformServiceProviders;
}
