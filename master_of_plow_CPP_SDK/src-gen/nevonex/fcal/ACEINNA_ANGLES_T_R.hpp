/*
 * nevonex/fcal/ACEINNA_ANGLES_T_R.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_ACEINNA_ANGLES_T_R_HPP
#define NEVONEX_FCAL_ACEINNA_ANGLES_T_R_HPP

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

    class EXPORT_NEVONEX_DLL ACEINNA_ANGLES_T_R : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(ACEINNA_ANGLES_T_R_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ACEINNA_ANGLES_T_R();

        virtual ~ACEINNA_ANGLES_T_R();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getPitch () const;
        /**
         * \brief 
         */
    public:
        virtual void setPitch (::ecore::EFloat _pitch);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getRoll () const;
        /**
         * \brief 
         */
    public:
        virtual void setRoll (::ecore::EFloat _roll);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getPitch_Compensation () const;
        /**
         * \brief 
         */
    public:
        virtual void setPitch_Compensation (::ecore::EInt _pitch_Compensation);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getPitch_FigureOfMerit () const;
        /**
         * \brief 
         */
    public:
        virtual void setPitch_FigureOfMerit (::ecore::EInt _pitch_FigureOfMerit);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getRoll_Compensation () const;
        /**
         * \brief 
         */
    public:
        virtual void setRoll_Compensation (::ecore::EInt _roll_Compensation);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getRoll_FigureOfMerit () const;
        /**
         * \brief 
         */
    public:
        virtual void setRoll_FigureOfMerit (::ecore::EInt _roll_FigureOfMerit);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getPitchRoll_Latency () const;
        /**
         * \brief 
         */
    public:
        virtual void setPitchRoll_Latency (::ecore::EFloat _pitchRoll_Latency);

        // References
    public:

        /*PROTECTED REGION ID(ACEINNA_ANGLES_T_R) START*/
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

        /*PROTECTED REGION ID(ACEINNA_ANGLES_T_RImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        ACEINNA_ANGLES_T_R_ptr _this()
        {   return ACEINNA_ANGLES_T_R_ptr(this);}

    private:
        // Attributes

        ::ecore::EFloat m_pitch;

        ::ecore::EFloat m_roll;

        ::ecore::EInt m_pitch_Compensation;

        ::ecore::EInt m_pitch_FigureOfMerit;

        ::ecore::EInt m_roll_Compensation;

        ::ecore::EInt m_roll_FigureOfMerit;

        ::ecore::EFloat m_pitchRoll_Latency;

        // References


        /*PROTECTED REGION ID(ACEINNA_ANGLES_T_R_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_ACEINNA_ANGLES_T_R_HPP
