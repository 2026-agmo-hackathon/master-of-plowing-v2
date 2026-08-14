/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Package.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_SERIAL_EXT_GPS_NMEA0183PACKAGE_HPP
#define _NEVONEX_SERIAL_EXT_GPS_NMEA0183PACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <nevonex/serial_ext_gps_nmea0183_forward.hpp>
#include <ecorecpp/PackageDeleter.hpp>

#include <nevonex/common/CommonPackage.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace serial_ext_gps_nmea0183
    {

    class EXPORT_NEVONEX_DLL Serial_ext_gps_nmea0183Package : public virtual ::ecore::EPackage
    {
    public:

        static Serial_ext_gps_nmea0183Package_ptr _instance();
        static Serial_ext_gps_nmea0183Package_ptr _getInstanceAndRemoveOwnership();

        // IDs for classifiers

        static const int ISERIAL_EXT_GPS_NMEA0183 = 0;

        static const int ISERIAL_EXT_GPS_NMEA0183FACTORY = 1;

        static const int ISERIAL_EXT_GPS_NMEA0183PROVIDER = 2;

        static const int SERIAL_EXT_GPS_NMEA0183 = 3;

        static const int SERIAL_EXT_GPS_NMEA0183PROVIDER = 4;

        static const int SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA = 4;

        static const int SERIAL_EXT_GPS_NMEA0183PROVIDER__SERIAL_EXT_GPS_NMEA0183 = 5;

        // IDs for classifiers for inherited features

        static const int SERIAL_EXT_GPS_NMEA0183__INDEX = ::nevonex::common::CommonPackage::TOPICOBJECT__INDEX;

        // EClassifiers methods

        virtual ::ecore::EClass_ptr getSerial_Ext_GPS_NMEA0183();
        virtual ::ecore::EClass_ptr getSerial_Ext_GPS_NMEA0183Provider();

        // EStructuralFeatures methods

        virtual ::ecore::EReference_ptr getSerial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data();
        virtual ::ecore::EReference_ptr getSerial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183();

    protected:

        static ::ecore::Ptr< Serial_ext_gps_nmea0183Package > s_instance;

        Serial_ext_gps_nmea0183Package();
        void _initPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_Serial_Ext_GPS_NMEA0183EClass;

        ::ecore::EClass_ptr m_Serial_Ext_GPS_NMEA0183ProviderEClass;

        // EEnuminstances 



        // EDataType instances 



        // EStructuralFeatures instances

        ::ecore::EReference_ptr m_Serial_Ext_GPS_NMEA0183__serial_Ext_GPS_NMEA0183_Data;

        ::ecore::EReference_ptr m_Serial_Ext_GPS_NMEA0183Provider__serial_Ext_GPS_NMEA0183;

    };

}
 // serial_ext_gps_nmea0183
}// nevonex

#endif // _NEVONEX_SERIAL_EXT_GPS_NMEA0183PACKAGE_HPP
