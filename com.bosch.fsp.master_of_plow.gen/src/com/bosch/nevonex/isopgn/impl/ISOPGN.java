/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.IMachine;

import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.impl.TopicObject;

import com.bosch.nevonex.isopgn.IISOPGN;

import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.impl.TypesPackage;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import java.lang.reflect.InvocationTargetException;

import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.emf.common.util.BasicEList;
import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.util.EDataTypeUniqueEList;

/**
 * An implementation of the model object '<em><b>ISOPGN</b></em>'.
 * @generated
 */
public class ISOPGN extends TopicObject implements IISOPGN {
	protected EList<PropertyChangeListener> listeners;
	protected float hitchPosPerRe = 0.0f;

	/**
	 * @generated
	 */
	protected ISOPGN() {
		super();
		initInterfaceDetails();
	}

	private void initInterfaceDetails() {
		interfaceDetailMap.put("fek/251", new InterfaceDetails("fek/251"));
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return IsopgnPackage.Literals.ISOPGN;
	}

	public float getHitchPosPerRe() {
		return hitchPosPerRe;
	}

	public void setHitchPosPerRe(float newHitchPosPerRe) {
		hitchPosPerRe = newHitchPosPerRe;
	}

	/**
	 * @generated
	 */
	public void addPropertyChangeListener(PropertyChangeListener listener) {
		if (listeners == null) {
			listeners = new BasicEList<>();
		}
		listeners.add(listener);
	}

	/**
	 * @generated
	 */
	public void removePropertyChangeListener(PropertyChangeListener listener) {
		if (listeners != null) {
			listeners.remove(listener);
		}
	}

	/**
	 * @generated
	 */
	public void notifyPropertyChange(String name, Object oldValue, Object newValue) {
		if (listeners != null) {
			for (PropertyChangeListener listener : this.listeners) {
				listener.propertyChange(new PropertyChangeEvent(this, name, oldValue, newValue));
			}
		}
	}

	/**
	 * @generated
	 */
	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case IsopgnPackage.ISOPGN__HITCH_POS_PER_RE:
			return getHitchPosPerRe();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	/**
	 * @generated
	 */
	@SuppressWarnings("unchecked")
	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case IsopgnPackage.ISOPGN__HITCH_POS_PER_RE:
			setHitchPosPerRe((Float) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	/**
	 * @generated
	 */
	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case IsopgnPackage.ISOPGN__HITCH_POS_PER_RE:
			setHitchPosPerRe(0.0f);
			return;
		}
		super.eUnset(featureID);
	}

	/**
	 * @generated
	 */
	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case IsopgnPackage.ISOPGN__HITCH_POS_PER_RE:
			return hitchPosPerRe != 0.0f;
		}
		return super.eIsSet(featureID);
	}
} //ISOPGN
