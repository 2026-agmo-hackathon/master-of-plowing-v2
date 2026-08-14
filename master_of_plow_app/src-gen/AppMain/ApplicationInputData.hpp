/*
 * AppMain/ApplicationInputData.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_APPLICATIONINPUTDATA_HPP
#define APPMAIN_APPLICATIONINPUTDATA_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

#include <nevonex/can_agmo_customized_tractor_forward.hpp>
#include <nevonex/can_agmo_mtlt305_forward.hpp>
#include <nevonex/can_agmo_steermotor_forward.hpp>
#include <nevonex/isopgn_forward.hpp>
#include <nevonex/serial_ext_gps_nmea0183_forward.hpp>

namespace AppMain
{

    class ApplicationInputData: public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(ApplicationInputData_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        ApplicationInputData();

        virtual ~ApplicationInputData();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider_ptr getCAN_AGMO_Customized_TractorProvider() const;
        /**
         * \brief 
         */
    public:
        virtual void setCAN_AGMO_Customized_TractorProvider(
                ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider_ptr _cAN_AGMO_Customized_TractorProvider);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider_ptr getCAN_AGMO_MTLT305Provider() const;
        /**
         * \brief 
         */
    public:
        virtual void setCAN_AGMO_MTLT305Provider(
                ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider_ptr _cAN_AGMO_MTLT305Provider);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider_ptr getCAN_AGMO_SteerMotorProvider() const;
        /**
         * \brief 
         */
    public:
        virtual void setCAN_AGMO_SteerMotorProvider(
                ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider_ptr _cAN_AGMO_SteerMotorProvider);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::isopgn::ISOPGNProvider_ptr getISOPGNProvider() const;
        /**
         * \brief 
         */
    public:
        virtual void setISOPGNProvider(
                ::nevonex::isopgn::ISOPGNProvider_ptr _iSOPGNProvider);

        /**
         * \brief 
         */
    public:
        virtual ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183Provider_ptr getSerial_Ext_GPS_NMEA0183Provider() const;
        /**
         * \brief 
         */
    public:
        virtual void setSerial_Ext_GPS_NMEA0183Provider(
                ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183Provider_ptr _serial_Ext_GPS_NMEA0183Provider);

    public:

        /*PROTECTED REGION ID(ApplicationInputData) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        /*PROTECTED REGION ID(ApplicationInputDataImpl) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    protected:
        ApplicationInputData_ptr _this()
        {
            return ApplicationInputData_ptr(this);
        }

    private:
        // Attributes

        // References

        ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_TractorProvider_ptr m_cAN_AGMO_Customized_TractorProvider;

        ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305Provider_ptr m_cAN_AGMO_MTLT305Provider;

        ::nevonex::can_agmo_steermotor::CAN_AGMO_SteerMotorProvider_ptr m_cAN_AGMO_SteerMotorProvider;

        ::nevonex::isopgn::ISOPGNProvider_ptr m_iSOPGNProvider;

        ::nevonex::serial_ext_gps_nmea0183::Serial_Ext_GPS_NMEA0183Provider_ptr m_serial_Ext_GPS_NMEA0183Provider;

        /*PROTECTED REGION ID(ApplicationInputData_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_APPLICATIONINPUTDATA_HPP
