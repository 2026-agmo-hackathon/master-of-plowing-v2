/*
 * nevonex/fcal/FcalFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_FCALFACTORY_HPP
#define _NEVONEX_FCALFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <nevonex/fcal.hpp>

#include <nevonex/dllNevonex.hpp>

namespace nevonex
{
    namespace fcal
    {

        class EXPORT_NEVONEX_DLL FcalFactory : public virtual ::ecore::EFactory
        {
        public:

            static FcalFactory_ptr _instance();

            virtual BulkProcessor_ptr createBulkProcessor();
            virtual RECEIVE_ACC_INFO_T_R_ptr createRECEIVE_ACC_INFO_T_R();
            virtual RECEIVE_FNR_INFO_T_R_ptr createRECEIVE_FNR_INFO_T_R();
            virtual RECEIVE_HYD_INFO_T_R_ptr createRECEIVE_HYD_INFO_T_R();
            virtual RECEIVE_SFT_INFO_T_R_ptr createRECEIVE_SFT_INFO_T_R();
            virtual AGMO_GENERIC_SEND_CMD_T_W_ptr createAGMO_GENERIC_SEND_CMD_T_W();
            virtual ACEINNA_ACCEL_T_R_ptr createACEINNA_ACCEL_T_R();
            virtual ACEINNA_ANGLERATE_T_R_ptr createACEINNA_ANGLERATE_T_R();
            virtual ACEINNA_ANGLES_T_R_ptr createACEINNA_ANGLES_T_R();
            virtual MOTOR_HEARTBEAT_T_R_ptr createMOTOR_HEARTBEAT_T_R();
            virtual RAW_CAN_DATA_DLC_8_W_ptr createRAW_CAN_DATA_DLC_8_W();
            virtual GPS_NMEA0183_T_R_ptr createGPS_NMEA0183_T_R();

            virtual ::ecore::EObject_ptr create ( ::ecore::EClass_ptr _eClass);
            virtual ::ecore::EJavaObject createFromString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EString const& _literalValue);
            virtual ::ecore::EString convertToString ( ::ecore::EDataType_ptr _eDataType, ::ecore::EJavaObject const& _instanceValue);

        protected:

            static ::ecore::Ptr< FcalFactory > s_holder;

            FcalFactory();

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

        template< > inline BulkProcessor_ptr create< BulkProcessor >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createBulkProcessor();
        }

        template< > inline RECEIVE_ACC_INFO_T_R_ptr create< RECEIVE_ACC_INFO_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createRECEIVE_ACC_INFO_T_R();
        }

        template< > inline RECEIVE_FNR_INFO_T_R_ptr create< RECEIVE_FNR_INFO_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createRECEIVE_FNR_INFO_T_R();
        }

        template< > inline RECEIVE_HYD_INFO_T_R_ptr create< RECEIVE_HYD_INFO_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createRECEIVE_HYD_INFO_T_R();
        }

        template< > inline RECEIVE_SFT_INFO_T_R_ptr create< RECEIVE_SFT_INFO_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createRECEIVE_SFT_INFO_T_R();
        }

        template< > inline AGMO_GENERIC_SEND_CMD_T_W_ptr create< AGMO_GENERIC_SEND_CMD_T_W >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createAGMO_GENERIC_SEND_CMD_T_W();
        }

        template< > inline ACEINNA_ACCEL_T_R_ptr create< ACEINNA_ACCEL_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createACEINNA_ACCEL_T_R();
        }

        template< > inline ACEINNA_ANGLERATE_T_R_ptr create< ACEINNA_ANGLERATE_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createACEINNA_ANGLERATE_T_R();
        }

        template< > inline ACEINNA_ANGLES_T_R_ptr create< ACEINNA_ANGLES_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createACEINNA_ANGLES_T_R();
        }

        template< > inline MOTOR_HEARTBEAT_T_R_ptr create< MOTOR_HEARTBEAT_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createMOTOR_HEARTBEAT_T_R();
        }

        template< > inline RAW_CAN_DATA_DLC_8_W_ptr create< RAW_CAN_DATA_DLC_8_W >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createRAW_CAN_DATA_DLC_8_W();
        }

        template< > inline GPS_NMEA0183_T_R_ptr create< GPS_NMEA0183_T_R >()
        {
            auto eFactory = FcalPackage::_instance()->getEFactoryInstance();
            auto packageFactory = dynamic_cast< FcalFactory* >(eFactory.get());
            return packageFactory->createGPS_NMEA0183_T_R();
        }

    } // fcal
} // nevonex

#endif // _NEVONEX_FCALFACTORY_HPP
