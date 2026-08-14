/*
 * nevonex/fcal/RAW_CAN_DATA_DLC_8_W.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_RAW_CAN_DATA_DLC_8_W_HPP
#define NEVONEX_FCAL_RAW_CAN_DATA_DLC_8_W_HPP

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

    class EXPORT_NEVONEX_DLL RAW_CAN_DATA_DLC_8_W : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        RAW_CAN_DATA_DLC_8_W();

        virtual ~RAW_CAN_DATA_DLC_8_W();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations
        /**
         * \brief 
         */
    public:
        virtual void initialize (::ecore::EInt _byte0, ::ecore::EInt _byte1, ::ecore::EInt _byte2, ::ecore::EInt _byte3, ::ecore::EInt _byte4, ::ecore::EInt _byte5, ::ecore::EInt _byte6, ::ecore::EInt _byte7);


        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte0 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte0 (::ecore::EInt _byte0);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte1 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte1 (::ecore::EInt _byte1);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte2 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte2 (::ecore::EInt _byte2);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte3 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte3 (::ecore::EInt _byte3);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte4 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte4 (::ecore::EInt _byte4);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte5 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte5 (::ecore::EInt _byte5);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte6 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte6 (::ecore::EInt _byte6);

        /**
         * \brief 
         */
    public:
        virtual ::ecore::EInt getByte7 () const;
        /**
         * \brief 
         */
    public:
        virtual void setByte7 (::ecore::EInt _byte7);

        // References
    public:

        /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W) START*/
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

        /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_WImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        RAW_CAN_DATA_DLC_8_W_ptr _this()
        {   return RAW_CAN_DATA_DLC_8_W_ptr(this);}

    private:
        // Attributes

        ::ecore::EInt m_byte0;

        ::ecore::EInt m_byte1;

        ::ecore::EInt m_byte2;

        ::ecore::EInt m_byte3;

        ::ecore::EInt m_byte4;

        ::ecore::EInt m_byte5;

        ::ecore::EInt m_byte6;

        ::ecore::EInt m_byte7;

        // References


        /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // fcal
}// nevonex

#endif // NEVONEX_FCAL_RAW_CAN_DATA_DLC_8_W_HPP
