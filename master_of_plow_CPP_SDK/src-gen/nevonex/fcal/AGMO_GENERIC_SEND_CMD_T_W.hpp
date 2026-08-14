/*
 * nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_AGMO_GENERIC_SEND_CMD_T_W_HPP
#define NEVONEX_FCAL_AGMO_GENERIC_SEND_CMD_T_W_HPP

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

    class EXPORT_NEVONEX_DLL AGMO_GENERIC_SEND_CMD_T_W : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        AGMO_GENERIC_SEND_CMD_T_W();

        virtual ~AGMO_GENERIC_SEND_CMD_T_W();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        /**
         * \brief 
         */
    public:
        virtual void initialize (::ecore::EInt _aD_Generic_Cmd, ::ecore::EInt _aD_Generic_Mode_Cmd);


        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getAD_Generic_Cmd () const;
        /**
         * \brief 
         */
    public:
        virtual void setAD_Generic_Cmd (::ecore::EInt _aD_Generic_Cmd);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getAD_Generic_Mode_Cmd () const;
        /**
         * \brief 
         */
    public:
        virtual void setAD_Generic_Mode_Cmd (::ecore::EInt _aD_Generic_Mode_Cmd);

        // References
    public:

        /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W) START*/
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

        /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_WImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        AGMO_GENERIC_SEND_CMD_T_W_ptr _this()
        {   return AGMO_GENERIC_SEND_CMD_T_W_ptr(this);}

    private:
        // Attributes

        ::ecore::EInt m_aD_Generic_Cmd;

        ::ecore::EInt m_aD_Generic_Mode_Cmd;

        // References


        /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_AGMO_GENERIC_SEND_CMD_T_W_HPP
