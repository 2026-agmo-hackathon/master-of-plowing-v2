/*
 * nevonex/types/PropertyChange.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "PropertyChange.hpp"
#include <algorithm>
#include <nevonex/propertychange/PropertyChangeEvent.hpp>

using namespace ::nevonex::types;

// Default constructor
PropertyChange::PropertyChange()
{
}

PropertyChange::~PropertyChange()
{
}

// Attributes

std::vector< ::nevonex::types::PropertyChangeListener > const& PropertyChange::getListeners() const
{
    return m_listeners;
}

void PropertyChange::addListeners(::nevonex::types::PropertyChangeListener _new_element_in_listeners)
{
    m_listeners.push_back(_new_element_in_listeners);
}

void PropertyChange::setListenersAt(size_t _position, ::nevonex::types::PropertyChangeListener _new_element_in_listeners)
{
    if (_position >= m_listeners.size()) m_listeners.resize(_position + 1, 0);
    m_listeners[_position] = _new_element_in_listeners;
}

void PropertyChange::deleteListenersAt(size_t _position)
{
    if (_position >= m_listeners.size()) return;
    m_listeners.erase(m_listeners.begin() + _position);
}

void PropertyChange::removeListeners(::nevonex::types::PropertyChangeListener _remove)
{
    m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), _remove), m_listeners.end());
}

// Framework operations

void PropertyChange::addPropertyChangeListener(::nevonex::types::PropertyChangeListener _listener)
{
    addListeners(_listener);
}

void PropertyChange::removePropertyChangeListener(::nevonex::types::PropertyChangeListener _listener)
{
    removeListeners(_listener);
}

void PropertyChange::notifyPropertyChange(::ecore::EString const& _name, ::ecore::EJavaObject const& _oldValue, ::ecore::EJavaObject const& _newValue)
{
    /*PROTECTED REGION ID(PropertyChange_notifyPropertyChange) START*/
    ::ecore::EJavaObject _source = ::ecore::as < ::ecore::EObject > (_this());
    ::nevonex::propertychange::PropertyChangeEvent<
            const ::ecore::EJavaObject &,
            const ::ecore::EJavaObject & > _event(_source, _name, _oldValue, _newValue);
    for (auto _listener : m_listeners)
    {
        if (_listener)
        {
            _listener->propertyChange(_event);
        }
    }
    /*PROTECTED REGION END*/
}
