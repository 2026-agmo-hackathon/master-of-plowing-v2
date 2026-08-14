/*
 * nevonex/fcal/ACEINNA_ACCEL_T_R.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_ACEINNA_ACCEL_T_R_HPP
#define NEVONEX_FCAL_ACEINNA_ACCEL_T_R_HPP

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

    class EXPORT_NEVONEX_DLL ACEINNA_ACCEL_T_R : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(ACEINNA_ACCEL_T_R_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        ACEINNA_ACCEL_T_R();

        virtual ~ACEINNA_ACCEL_T_R();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getAccY () const;
        /**
         * \brief 
         */
    public:
        virtual void setAccY (::ecore::EFloat _accY);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getAccX () const;
        /**
         * \brief 
         */
    public:
        virtual void setAccX (::ecore::EFloat _accX);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getAccZ () const;
        /**
         * \brief 
         */
    public:
        virtual void setAccZ (::ecore::EFloat _accZ);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getLateralAcc_FigureOfMerit () const;
        /**
         * \brief 
         */
    public:
        virtual void setLateralAcc_FigureOfMerit (::ecore::EInt _lateralAcc_FigureOfMerit);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getLongiAcc_FigureOfMerit () const;
        /**
         * \brief 
         */
    public:
        virtual void setLongiAcc_FigureOfMerit (::ecore::EInt _longiAcc_FigureOfMerit);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getVerticAcc_FigureOfMerit () const;
        /**
         * \brief 
         */
    public:
        virtual void setVerticAcc_FigureOfMerit (::ecore::EInt _verticAcc_FigureOfMerit);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getSupport_Rate_Acc () const;
        /**
         * \brief 
         */
    public:
        virtual void setSupport_Rate_Acc (::ecore::EInt _support_Rate_Acc);

        // References
    public:

        /*PROTECTED REGION ID(ACEINNA_ACCEL_T_R) START*/
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

        /*PROTECTED REGION ID(ACEINNA_ACCEL_T_RImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        ACEINNA_ACCEL_T_R_ptr _this()
        {   return ACEINNA_ACCEL_T_R_ptr(this);}

    private:
        // Attributes

        ::ecore::EFloat m_accY;

        ::ecore::EFloat m_accX;

        ::ecore::EFloat m_accZ;

        ::ecore::EInt m_lateralAcc_FigureOfMerit;

        ::ecore::EInt m_longiAcc_FigureOfMerit;

        ::ecore::EInt m_verticAcc_FigureOfMerit;

        ::ecore::EInt m_support_Rate_Acc;

        // References


        /*PROTECTED REGION ID(ACEINNA_ACCEL_T_R_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_ACEINNA_ACCEL_T_R_HPP
