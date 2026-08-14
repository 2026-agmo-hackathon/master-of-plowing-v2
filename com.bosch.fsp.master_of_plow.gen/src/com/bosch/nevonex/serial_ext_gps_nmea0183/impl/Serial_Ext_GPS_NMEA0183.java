/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.IMachine;

import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.impl.TopicObject;

import com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;

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
 * An implementation of the model object '<em><b>Serial_Ext_GPS_NMEA0183</b></em>'.
 * @generated
 */
public class Serial_Ext_GPS_NMEA0183 extends TopicObject implements ISerial_Ext_GPS_NMEA0183 {
	protected EList<PropertyChangeListener> listeners;
	protected IGPS_NMEA0183_T_R serial_Ext_GPS_NMEA0183_Data;

	/**
	 * @generated
	 */
	protected Serial_Ext_GPS_NMEA0183() {
		super();
		initInterfaceDetails();
	}

	private void initInterfaceDetails() {
		interfaceDetailMap.put("fek/9465", new InterfaceDetails("fek/9465"));
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return Serial_ext_gps_nmea0183Package.Literals.SERIAL_EXT_GPS_NMEA0183;
	}

	/**
	 * @generated
	 */
	public IGPS_NMEA0183_T_R getSerial_Ext_GPS_NMEA0183_Data() {
		if (serial_Ext_GPS_NMEA0183_Data != null && serial_Ext_GPS_NMEA0183_Data.eIsProxy()) {
			InternalEObject oldSerial_Ext_GPS_NMEA0183_Data = (InternalEObject) serial_Ext_GPS_NMEA0183_Data;
			serial_Ext_GPS_NMEA0183_Data = (IGPS_NMEA0183_T_R) eResolveProxy(oldSerial_Ext_GPS_NMEA0183_Data);
			if (serial_Ext_GPS_NMEA0183_Data != oldSerial_Ext_GPS_NMEA0183_Data) {
			}
		}
		return serial_Ext_GPS_NMEA0183_Data;
	}

	/**
	 * @generated
	 */
	public IGPS_NMEA0183_T_R basicGetSerial_Ext_GPS_NMEA0183_Data() {
		return serial_Ext_GPS_NMEA0183_Data;
	}

	/**
	 * @generated
	 */
	public void setSerial_Ext_GPS_NMEA0183_Data(IGPS_NMEA0183_T_R newSerial_Ext_GPS_NMEA0183_Data) {
		IGPS_NMEA0183_T_R oldSerial_Ext_GPS_NMEA0183_Data = serial_Ext_GPS_NMEA0183_Data;
		serial_Ext_GPS_NMEA0183_Data = newSerial_Ext_GPS_NMEA0183_Data;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " Serial_Ext_GPS_NMEA0183 property \"serial_Ext_GPS_NMEA0183_Data\" changed from  "
						+ oldSerial_Ext_GPS_NMEA0183_Data + " to " + newSerial_Ext_GPS_NMEA0183_Data);
		notifyPropertyChange("serial_Ext_GPS_NMEA0183_Data", oldSerial_Ext_GPS_NMEA0183_Data, newSerial_Ext_GPS_NMEA0183_Data);
	}

	/**
	 * @generated
	 */
	public long getSerial_Ext_GPS_NMEA0183_Data_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9465");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isSerial_Ext_GPS_NMEA0183_Data_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9465");
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
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
			if (resolve)
				return getSerial_Ext_GPS_NMEA0183_Data();
			return basicGetSerial_Ext_GPS_NMEA0183_Data();
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
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
			setSerial_Ext_GPS_NMEA0183_Data((IGPS_NMEA0183_T_R) newValue);
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
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
			setSerial_Ext_GPS_NMEA0183_Data((IGPS_NMEA0183_T_R) null);
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
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA:
			return serial_Ext_GPS_NMEA0183_Data != null;
		}
		return super.eIsSet(featureID);
	}
} //Serial_Ext_GPS_NMEA0183
