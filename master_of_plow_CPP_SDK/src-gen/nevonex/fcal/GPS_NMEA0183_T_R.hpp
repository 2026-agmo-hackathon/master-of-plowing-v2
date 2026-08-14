/*
 * nevonex/fcal/GPS_NMEA0183_T_R.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_GPS_NMEA0183_T_R_HPP
#define NEVONEX_FCAL_GPS_NMEA0183_T_R_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/fcal_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include "FcalPackage.hpp"

namespace nevonex
{
    namespace fcal
    {

    class EXPORT_NEVONEX_DLL GPS_NMEA0183_T_R : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(GPS_NMEA0183_T_R_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        GPS_NMEA0183_T_R();

        virtual ~GPS_NMEA0183_T_R();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EString getUTC_ms () const;
        /**
         * \brief 
         */
    public:
        virtual void setUTC_ms (::ecore::EString _uTC_ms);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EDouble getLatitude () const;
        /**
         * \brief 
         */
    public:
        virtual void setLatitude (::ecore::EDouble _latitude);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EDouble getLongitude () const;
        /**
         * \brief 
         */
    public:
        virtual void setLongitude (::ecore::EDouble _longitude);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getGPS_Quality_Indicator () const;
        /**
         * \brief 
         */
    public:
        virtual void setGPS_Quality_Indicator (::ecore::EInt _gPS_Quality_Indicator);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getNo_Satellites () const;
        /**
         * \brief 
         */
    public:
        virtual void setNo_Satellites (::ecore::EInt _no_Satellites);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getAltitude () const;
        /**
         * \brief 
         */
    public:
        virtual void setAltitude (::ecore::EFloat _altitude);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getGeoidal_Separation () const;
        /**
         * \brief 
         */
    public:
        virtual void setGeoidal_Separation (::ecore::EFloat _geoidal_Separation);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getDGPS_Age () const;
        /**
         * \brief 
         */
    public:
        virtual void setDGPS_Age (::ecore::EFloat _dGPS_Age);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getDifferential_Ref_Station () const;
        /**
         * \brief 
         */
    public:
        virtual void setDifferential_Ref_Station (::ecore::EInt _differential_Ref_Station);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EString getSelection_Mode () const;
        /**
         * \brief 
         */
    public:
        virtual void setSelection_Mode (::ecore::EString _selection_Mode);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getFix_Type () const;
        /**
         * \brief 
         */
    public:
        virtual void setFix_Type (::ecore::EInt _fix_Type);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getPDOP () const;
        /**
         * \brief 
         */
    public:
        virtual void setPDOP (::ecore::EFloat _pDOP);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getHDOP () const;
        /**
         * \brief 
         */
    public:
        virtual void setHDOP (::ecore::EFloat _hDOP);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getVDOP () const;
        /**
         * \brief 
         */
    public:
        virtual void setVDOP (::ecore::EFloat _vDOP);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getStatus () const;
        /**
         * \brief 
         */
    public:
        virtual void setStatus (::ecore::EInt _status);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getSpeed () const;
        /**
         * \brief 
         */
    public:
        virtual void setSpeed (::ecore::EFloat _speed);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getCourse () const;
        /**
         * \brief 
         */
    public:
        virtual void setCourse (::ecore::EFloat _course);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getMagnetic_variation () const;
        /**
         * \brief 
         */
    public:
        virtual void setMagnetic_variation (::ecore::EFloat _magnetic_variation);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EString getFAA_Mode_Indicator () const;
        /**
         * \brief 
         */
    public:
        virtual void setFAA_Mode_Indicator (::ecore::EString _fAA_Mode_Indicator);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getHeading () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeading (::ecore::EFloat _heading);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getHeadingTrue () const;
        /**
         * \brief 
         */
    public:
        virtual void setHeadingTrue (::ecore::EInt _headingTrue);

        // References
    public:

        /*PROTECTED REGION ID(GPS_NMEA0183_T_R) START*/
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

        /*PROTECTED REGION ID(GPS_NMEA0183_T_RImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        GPS_NMEA0183_T_R_ptr _this()
        {   return GPS_NMEA0183_T_R_ptr(this);}

    private:
        // Attributes

        ::ecore::EString m_uTC_ms;

        ::ecore::EDouble m_latitude;

        ::ecore::EDouble m_longitude;

        ::ecore::EInt m_gPS_Quality_Indicator;

        ::ecore::EInt m_no_Satellites;

        ::ecore::EFloat m_altitude;

        ::ecore::EFloat m_geoidal_Separation;

        ::ecore::EFloat m_dGPS_Age;

        ::ecore::EInt m_differential_Ref_Station;

        ::ecore::EString m_selection_Mode;

        ::ecore::EInt m_fix_Type;

        ::ecore::EFloat m_pDOP;

        ::ecore::EFloat m_hDOP;

        ::ecore::EFloat m_vDOP;

        ::ecore::EInt m_status;

        ::ecore::EFloat m_speed;

        ::ecore::EFloat m_course;

        ::ecore::EFloat m_magnetic_variation;

        ::ecore::EString m_fAA_Mode_Indicator;

        ::ecore::EFloat m_heading;

        ::ecore::EInt m_headingTrue;

        // References


        /*PROTECTED REGION ID(GPS_NMEA0183_T_R_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_GPS_NMEA0183_T_R_HPP
