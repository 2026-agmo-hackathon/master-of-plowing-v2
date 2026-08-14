/*
 * nevonex/types/PropertyChange.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_TYPES_PROPERTYCHANGE_HPP
#define NEVONEX_TYPES_PROPERTYCHANGE_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <nevonex/dllNevonex.hpp>
#include <nevonex/types_forward.hpp>

#include <ecore_forward.hpp>
#include <nevonex/propertychange/AbstractPropertyChange.hpp>

#include "TypesPackage.hpp"

/*PROTECTED REGION ID(PropertyChange_commonSection) START*/
/*PROTECTED REGION END*/

namespace nevonex
{
    namespace types
    {

    class EXPORT_NEVONEX_DLL PropertyChange : public virtual ::ecore::EObject, public virtual ::nevonex::propertychange::AbstractPropertyChange
    {
    public:
        PropertyChange();

        virtual ~PropertyChange();

        virtual void _initialize() override;

        // Operations
        virtual void addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener);
        virtual void notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue);

        // Attributes
        virtual std::vector< ::nevonex::types::PropertyChangeListener > const& getListeners() const;
        virtual void addListeners(::nevonex::types::PropertyChangeListener _new_element_in_listeners);
        virtual void setListenersAt(size_t _position, ::nevonex::types::PropertyChangeListener _new_element_in_listeners);
        virtual void deleteListenersAt(size_t _position);
        virtual void removeListeners(::nevonex::types::PropertyChangeListener _remove_element_in_listeners);

        /*PROTECTED REGION ID(PropertyChange) START*/
        /*PROTECTED REGION END*/

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
        PropertyChange_ptr _this()
        {   return PropertyChange_ptr(this);}

    private:
        std::vector< ::nevonex::types::PropertyChangeListener > m_listeners;
    };

}
 // types
}// nevonex

#endif // NEVONEX_TYPES_PROPERTYCHANGE_HPP
