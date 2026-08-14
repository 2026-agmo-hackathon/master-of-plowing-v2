/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types;

import java.beans.PropertyChangeListener;
import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public interface IPropertyChange extends EObject {
	void addPropertyChangeListener(PropertyChangeListener listener);

	void removePropertyChangeListener(PropertyChangeListener listener);

	void notifyPropertyChange(String name, Object oldValue, Object newValue);
} // IPropertyChange
