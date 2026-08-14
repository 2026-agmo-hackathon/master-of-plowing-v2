/*
 * nevonex/common/AbsolutePosition.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_ABSOLUTEPOSITION_HPP
#define NEVONEX_COMMON_ABSOLUTEPOSITION_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/common_forward.hpp>

#include <ecore/EObject.hpp>
#include <ecore_forward.hpp>

#include "CommonPackage.hpp"

namespace nevonex
{
    namespace common
    {

    class EXPORT_NEVONEX_DLL AbsolutePosition : public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(AbsolutePosition_commonSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    public:
        AbsolutePosition();

        virtual ~AbsolutePosition();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes
        /**
         * \brief 
         */
    public:
        virtual ::ecore::EDouble getAltitude () const;
        /**
         * \brief 
         */
    public:
        virtual void setAltitude (::ecore::EDouble _altitude);

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

        // References
    public:

        /*PROTECTED REGION ID(AbsolutePosition) START*/
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

        /*PROTECTED REGION ID(AbsolutePositionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        AbsolutePosition_ptr _this()
        {   return AbsolutePosition_ptr(this);}

    private:
        // Attributes

        ::ecore::EDouble m_altitude;

        ::ecore::EDouble m_latitude;

        ::ecore::EDouble m_longitude;

        // References


        /*PROTECTED REGION ID(AbsolutePosition_privateSection) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/
    };

}
 // common
}// nevonex

#endif // NEVONEX_COMMON_ABSOLUTEPOSITION_HPP
