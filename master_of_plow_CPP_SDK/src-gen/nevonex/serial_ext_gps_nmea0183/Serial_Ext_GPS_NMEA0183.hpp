/*
 * nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_SERIAL_EXT_GPS_NMEA0183_SERIAL_EXT_GPS_NMEA0183_HPP
#define NEVONEX_SERIAL_EXT_GPS_NMEA0183_SERIAL_EXT_GPS_NMEA0183_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/serial_ext_gps_nmea0183_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/common/TopicObject.hpp>
#include <nevonex/types/PropertyChange.hpp>
#include <nevonex/types/IMachine.hpp>
#include <nevonex/fcal/GPS_NMEA0183_T_R.hpp>

#include "Serial_ext_gps_nmea0183Package.hpp"

namespace nevonex
{
    namespace serial_ext_gps_nmea0183
    {

    class EXPORT_NEVONEX_DLL Serial_Ext_GPS_NMEA0183 : public virtual ::nevonex::common::TopicObject,
        public virtual ::nevonex::types::PropertyChange,
        public virtual ::nevonex::types::IMachine
    {
        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        Serial_Ext_GPS_NMEA0183();

        virtual ~Serial_Ext_GPS_NMEA0183();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::ELong getSerial_Ext_GPS_NMEA0183_Data_Timestamp();
        virtual ::ecore::EBoolean isSerial_Ext_GPS_NMEA0183_Data_Valid();

        virtual ::ecore::EBoolean updateInterfaceDetail(int _topicKey, ::ecore::EBoolean _valid, ::ecore::ELong _timestamp);

        // Attributes
        // References
        /**
         * \brief 
         */
    public:
        virtual ::nevonex::fcal::GPS_NMEA0183_T_R_ptr getSerial_Ext_GPS_NMEA0183_Data () const;
        /**
         * \brief 
         */
    public:
        virtual void setSerial_Ext_GPS_NMEA0183_Data (::nevonex::fcal::GPS_NMEA0183_T_R_ptr _serial_Ext_GPS_NMEA0183_Data);


        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183) START*/
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

        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183Impl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        Serial_Ext_GPS_NMEA0183_ptr _this()
        {   return Serial_Ext_GPS_NMEA0183_ptr(this);}

        friend Serial_Ext_GPS_NMEA0183Provider;

    private:
        // Attributes

        // References
        ::nevonex::fcal::GPS_NMEA0183_T_R_ptr m_serial_Ext_GPS_NMEA0183_Data;

        std::map<int, ::nevonex::types::InterfaceDetails> m_interfaceDetailMap;

        /*PROTECTED REGION ID(Serial_Ext_GPS_NMEA0183_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // serial_ext_gps_nmea0183
}// nevonex

#endif // NEVONEX_SERIAL_EXT_GPS_NMEA0183_SERIAL_EXT_GPS_NMEA0183_HPP
