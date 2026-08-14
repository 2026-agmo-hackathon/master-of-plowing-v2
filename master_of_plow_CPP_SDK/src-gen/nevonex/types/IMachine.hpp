/*
 * nevonex/types/IMachine.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_TYPES_IMACHINE_HPP
#define NEVONEX_TYPES_IMACHINE_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/types_forward.hpp>

#include <nevonex/feature/AbstractMachine.hpp>

#include "TypesPackage.hpp"

namespace nevonex
{
    namespace types
    {

    class EXPORT_NEVONEX_DLL IMachine : public virtual ::ecore::EObject, public virtual ::nevonex::feature::AbstractMachine
    {
    public:
        IMachine();

        virtual ~IMachine();

        virtual void _initialize() override;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    protected:
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID, ::ecore::EBoolean _resolve) override;
        virtual void eSet(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID) override;
        virtual void eUnset(::ecore::EInt _featureID) override;
        virtual ::ecore::EClass_ptr _eClass() override;
        virtual void _inverseAdd(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue) override;
        virtual void _inverseRemove(::ecore::EInt _featureID, ::ecore::EJavaObject const& _oldValue) override;
#endif

    protected:
        IMachine_ptr _this()
        {   return IMachine_ptr(this);}
    };

}
 // types
}// nevonex

#endif // NEVONEX_TYPES_IMACHINE_HPP
