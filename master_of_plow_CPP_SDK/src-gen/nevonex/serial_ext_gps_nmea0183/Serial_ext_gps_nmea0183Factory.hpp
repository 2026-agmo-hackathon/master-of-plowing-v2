/*
 * nevonex/serial_ext_gps_nmea0183/Serial_ext_gps_nmea0183Factory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_SERIAL_EXT_GPS_NMEA0183FACTORY_HPP
#define _NEVONEX_SERIAL_EXT_GPS_NMEA0183FACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/serial_ext_gps_nmea0183.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace serial_ext_gps_nmea0183
    {

        class EXPORT_NEVONEX_DLL Serial_ext_gps_nmea0183Factory : public virtual ::ecore::EFactory
        {
        public:

            static Serial_ext_gps_nmea0183Factory_ptr _instance();

            virtual Serial_Ext_GPS_NMEA0183_ptr createSerial_Ext_GPS_NMEA0183();
            virtual Serial_Ext_GPS_NMEA0183Provider_ptr createSerial_Ext_GPS_NMEA0183Provider();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< Serial_ext_gps_nmea0183Factory > s_holder;

            Serial_ext_gps_nmea0183Factory();

        };

        /** An object creation helper
         *
         * Usage (add namespaces as required):
         *   auto p = create<MyClass>();
         *
         */
        template< class T > inline ::ecore::Ptr< T > create()
        {
            return ::ecore::Ptr< T >();
        }

        template< > inline Serial_Ext_GPS_NMEA0183_ptr create< Serial_Ext_GPS_NMEA0183 >()
        {
            auto eFactory = Serial_ext_gps_nmea0183Package::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Serial_ext_gps_nmea0183Factory* >(eFactory.get());
            return packageFactory->createSerial_Ext_GPS_NMEA0183();
        }

        template< > inline Serial_Ext_GPS_NMEA0183Provider_ptr create< Serial_Ext_GPS_NMEA0183Provider >()
        {
            auto eFactory = Serial_ext_gps_nmea0183Package::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< Serial_ext_gps_nmea0183Factory* >(eFactory.get());
            return packageFactory->createSerial_Ext_GPS_NMEA0183Provider();
        }

    } // serial_ext_gps_nmea0183
} // nevonex

#endif // _NEVONEX_SERIAL_EXT_GPS_NMEA0183FACTORY_HPP
