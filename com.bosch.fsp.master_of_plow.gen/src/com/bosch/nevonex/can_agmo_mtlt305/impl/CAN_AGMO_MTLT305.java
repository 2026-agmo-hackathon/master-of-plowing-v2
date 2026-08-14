/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.IMachine;

import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.impl.TopicObject;

import com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R;

import com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R;

import com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R;

import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305;

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
 * An implementation of the model object '<em><b>CAN_AGMO_MTLT305</b></em>'.
 * @generated
 */
public class CAN_AGMO_MTLT305 extends TopicObject implements ICAN_AGMO_MTLT305 {
	protected EList<PropertyChangeListener> listeners;
	protected IACEINNA_ACCEL_T_R aceinna_Accel;
	protected IACEINNA_ANGLERATE_T_R aceinna_AngleRate;
	protected IACEINNA_ANGLES_T_R aceinna_Angles;

	/**
	 * @generated
	 */
	protected CAN_AGMO_MTLT305() {
		super();
		initInterfaceDetails();
	}

	private void initInterfaceDetails() {
		interfaceDetailMap.put("fek/9440", new InterfaceDetails("fek/9440"));
		interfaceDetailMap.put("fek/9441", new InterfaceDetails("fek/9441"));
		interfaceDetailMap.put("fek/9442", new InterfaceDetails("fek/9442"));
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return Can_agmo_mtlt305Package.Literals.CAN_AGMO_MTLT305;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ACCEL_T_R getAceinna_Accel() {
		if (aceinna_Accel != null && aceinna_Accel.eIsProxy()) {
			InternalEObject oldAceinna_Accel = (InternalEObject) aceinna_Accel;
			aceinna_Accel = (IACEINNA_ACCEL_T_R) eResolveProxy(oldAceinna_Accel);
			if (aceinna_Accel != oldAceinna_Accel) {
			}
		}
		return aceinna_Accel;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ACCEL_T_R basicGetAceinna_Accel() {
		return aceinna_Accel;
	}

	/**
	 * @generated
	 */
	public void setAceinna_Accel(IACEINNA_ACCEL_T_R newAceinna_Accel) {
		IACEINNA_ACCEL_T_R oldAceinna_Accel = aceinna_Accel;
		aceinna_Accel = newAceinna_Accel;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_MTLT305 property \"aceinna_Accel\" changed from  "
						+ oldAceinna_Accel + " to " + newAceinna_Accel);
		notifyPropertyChange("aceinna_Accel", oldAceinna_Accel, newAceinna_Accel);
	}

	/**
	 * @generated
	 */
	public long getAceinna_Accel_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9440");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isAceinna_Accel_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9440");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ANGLERATE_T_R getAceinna_AngleRate() {
		if (aceinna_AngleRate != null && aceinna_AngleRate.eIsProxy()) {
			InternalEObject oldAceinna_AngleRate = (InternalEObject) aceinna_AngleRate;
			aceinna_AngleRate = (IACEINNA_ANGLERATE_T_R) eResolveProxy(oldAceinna_AngleRate);
			if (aceinna_AngleRate != oldAceinna_AngleRate) {
			}
		}
		return aceinna_AngleRate;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ANGLERATE_T_R basicGetAceinna_AngleRate() {
		return aceinna_AngleRate;
	}

	/**
	 * @generated
	 */
	public void setAceinna_AngleRate(IACEINNA_ANGLERATE_T_R newAceinna_AngleRate) {
		IACEINNA_ANGLERATE_T_R oldAceinna_AngleRate = aceinna_AngleRate;
		aceinna_AngleRate = newAceinna_AngleRate;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_MTLT305 property \"aceinna_AngleRate\" changed from  "
						+ oldAceinna_AngleRate + " to " + newAceinna_AngleRate);
		notifyPropertyChange("aceinna_AngleRate", oldAceinna_AngleRate, newAceinna_AngleRate);
	}

	/**
	 * @generated
	 */
	public long getAceinna_AngleRate_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9441");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isAceinna_AngleRate_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9441");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ANGLES_T_R getAceinna_Angles() {
		if (aceinna_Angles != null && aceinna_Angles.eIsProxy()) {
			InternalEObject oldAceinna_Angles = (InternalEObject) aceinna_Angles;
			aceinna_Angles = (IACEINNA_ANGLES_T_R) eResolveProxy(oldAceinna_Angles);
			if (aceinna_Angles != oldAceinna_Angles) {
			}
		}
		return aceinna_Angles;
	}

	/**
	 * @generated
	 */
	public IACEINNA_ANGLES_T_R basicGetAceinna_Angles() {
		return aceinna_Angles;
	}

	/**
	 * @generated
	 */
	public void setAceinna_Angles(IACEINNA_ANGLES_T_R newAceinna_Angles) {
		IACEINNA_ANGLES_T_R oldAceinna_Angles = aceinna_Angles;
		aceinna_Angles = newAceinna_Angles;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_MTLT305 property \"aceinna_Angles\" changed from  "
						+ oldAceinna_Angles + " to " + newAceinna_Angles);
		notifyPropertyChange("aceinna_Angles", oldAceinna_Angles, newAceinna_Angles);
	}

	/**
	 * @generated
	 */
	public long getAceinna_Angles_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9442");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isAceinna_Angles_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9442");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
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
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ACCEL:
			if (resolve)
				return getAceinna_Accel();
			return basicGetAceinna_Accel();
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE:
			if (resolve)
				return getAceinna_AngleRate();
			return basicGetAceinna_AngleRate();
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLES:
			if (resolve)
				return getAceinna_Angles();
			return basicGetAceinna_Angles();
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
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ACCEL:
			setAceinna_Accel((IACEINNA_ACCEL_T_R) newValue);
			return;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE:
			setAceinna_AngleRate((IACEINNA_ANGLERATE_T_R) newValue);
			return;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLES:
			setAceinna_Angles((IACEINNA_ANGLES_T_R) newValue);
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
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ACCEL:
			setAceinna_Accel((IACEINNA_ACCEL_T_R) null);
			return;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE:
			setAceinna_AngleRate((IACEINNA_ANGLERATE_T_R) null);
			return;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLES:
			setAceinna_Angles((IACEINNA_ANGLES_T_R) null);
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
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ACCEL:
			return aceinna_Accel != null;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE:
			return aceinna_AngleRate != null;
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305__ACEINNA_ANGLES:
			return aceinna_Angles != null;
		}
		return super.eIsSet(featureID);
	}
} //CAN_AGMO_MTLT305
