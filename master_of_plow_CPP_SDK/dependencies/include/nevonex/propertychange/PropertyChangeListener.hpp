/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_PropertyChangeListener_H_
#define NEVONEX_PropertyChangeListener_H_

#include <ecore/EObject.hpp>

#include "../propertychange/PropertyChangeEvent.hpp"

namespace nevonex {
namespace propertychange {

/**
 * A "PropertyChange" event gets fired whenever a bean changes a "bound"
 * property.  You can register a PropertyChangeListener with a source
 * bean so as to be notified of any bound property updates.
 * \author per9kor
 * \since 1.0
 */
class PropertyChangeListener : public virtual ::ecore::EObject
{
public:
	/**
	 * This method gets called when a bound property is changed.
     * \param changeEvent A PropertyChangeEvent object describing the event source and the property that has changed.
	 */
	virtual void propertyChange(PropertyChangeEvent<const ::ecore::EJavaObject &, const ::ecore::EJavaObject &> changeEvent) = 0;
	virtual ~PropertyChangeListener() = default;
};

} // propertychange
} // nevonex

#endif /* NEVONEX_PropertyChangeListener_H_ */
