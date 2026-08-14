/*
 * nevonex/fcal/RECEIVE_FNR_INFO_T_R.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_RECEIVE_FNR_INFO_T_R_HPP
#define NEVONEX_FCAL_RECEIVE_FNR_INFO_T_R_HPP

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

    class EXPORT_NEVONEX_DLL RECEIVE_FNR_INFO_T_R : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(RECEIVE_FNR_INFO_T_R_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        RECEIVE_FNR_INFO_T_R();

        virtual ~RECEIVE_FNR_INFO_T_R();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getTRZ_FNR_SIG1_V () const;
        /**
         * \brief 
         */
    public:
        virtual void setTRZ_FNR_SIG1_V (::ecore::EFloat _tRZ_FNR_SIG1_V);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EFloat getTRZ_FNR_SIG2_V () const;
        /**
         * \brief 
         */
    public:
        virtual void setTRZ_FNR_SIG2_V (::ecore::EFloat _tRZ_FNR_SIG2_V);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getTRZ_FNR_DIAG () const;
        /**
         * \brief 
         */
    public:
        virtual void setTRZ_FNR_DIAG (::ecore::EInt _tRZ_FNR_DIAG);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getTRZ_FNR_AUTO () const;
        /**
         * \brief 
         */
    public:
        virtual void setTRZ_FNR_AUTO (::ecore::EInt _tRZ_FNR_AUTO);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getTRZ_FNR_STATE () const;
        /**
         * \brief 
         */
    public:
        virtual void setTRZ_FNR_STATE (::ecore::EInt _tRZ_FNR_STATE);

        // References
    public:

        /*PROTECTED REGION ID(RECEIVE_FNR_INFO_T_R) START*/
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

        /*PROTECTED REGION ID(RECEIVE_FNR_INFO_T_RImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        RECEIVE_FNR_INFO_T_R_ptr _this()
        {   return RECEIVE_FNR_INFO_T_R_ptr(this);}

    private:
        // Attributes

        ::ecore::EFloat m_tRZ_FNR_SIG1_V;

        ::ecore::EFloat m_tRZ_FNR_SIG2_V;

        ::ecore::EInt m_tRZ_FNR_DIAG;

        ::ecore::EInt m_tRZ_FNR_AUTO;

        ::ecore::EInt m_tRZ_FNR_STATE;

        // References


        /*PROTECTED REGION ID(RECEIVE_FNR_INFO_T_R_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_RECEIVE_FNR_INFO_T_R_HPP
