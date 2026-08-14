/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractPropertyChange_H_
#define NEVONEX_AbstractPropertyChange_H_

#include <ecore/EObject.hpp>

#include "../propertychange/PropertyChangeListener.hpp"

namespace nevonex {
namespace propertychange {

/**
 * Abstract implementation of the property change listeners.
 * \author per9kor
 * \since 1.0
 */
class AbstractPropertyChange  : public virtual ::ecore::EObject
{

public:
	/**
	 * \since 4.1.0
	 */
	using ptr_t = ::ecore::Ptr<AbstractPropertyChange>;

	/**
	 * Adds given property change listener to the machine provider. This listener will be fired whenever new value is set to the property.
	 * \param listener Listener to be added.
	 */
	virtual void addPropertyChangeListener(PropertyChangeListener * listener) = 0;
	/**
	 * Removes given property change listener from the machine provider.
	 *
	 * \param listener Listener to be removed.
	 */
	virtual void removePropertyChangeListener(PropertyChangeListener * listener) = 0;
	virtual inline ~AbstractPropertyChange() = default;
};
using AbstractPropertyChange_ptr  = AbstractPropertyChange::ptr_t;

} // propertychange
} // nevonex

#endif // NEVONEX_AbstractPropertyChange_H_
