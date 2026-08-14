/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.util;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_ext_gps_nmea0183Factory;

import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_ext_gps_nmea0183Package;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.common.ITopicObject;

/**
 * @generated
 */
public class Serial_ext_gps_nmea0183Switch<T> extends Switch<T> {
	protected static Serial_ext_gps_nmea0183Package modelPackage;

	public Serial_ext_gps_nmea0183Switch() {
		if (modelPackage == null) {
			modelPackage = Serial_ext_gps_nmea0183Package.eINSTANCE;
		}
	}

	@Override
	protected boolean isSwitchFor(EPackage ePackage) {
		return ePackage == modelPackage;
	}

	@Override
	protected T doSwitch(int classifierID, EObject theEObject) {
		if (classifierID < 80) return doSwitch_chunk0(classifierID, theEObject);
		return defaultCase(theEObject);
	}

	private T doSwitch_chunk0(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case Serial_ext_gps_nmea0183Package.I_SERIAL_EXT_GPS_NMEA0183: {
				ISerial_Ext_GPS_NMEA0183 iSerial_Ext_GPS_NMEA0183 = (ISerial_Ext_GPS_NMEA0183) theEObject;
				T result = caseISerial_Ext_GPS_NMEA0183(iSerial_Ext_GPS_NMEA0183);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183: {
				ISerial_Ext_GPS_NMEA0183 serial_Ext_GPS_NMEA0183 = (ISerial_Ext_GPS_NMEA0183) theEObject;
				T result = caseSerial_Ext_GPS_NMEA0183(serial_Ext_GPS_NMEA0183);
				if (result == null)
					result = caseTopicObject((ITopicObject) serial_Ext_GPS_NMEA0183);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Serial_ext_gps_nmea0183Package.I_SERIAL_EXT_GPS_NMEA0183PROVIDER: {
				ISerial_Ext_GPS_NMEA0183Provider iSerial_Ext_GPS_NMEA0183Provider = (ISerial_Ext_GPS_NMEA0183Provider) theEObject;
				T result = caseISerial_Ext_GPS_NMEA0183Provider(iSerial_Ext_GPS_NMEA0183Provider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183PROVIDER: {
				ISerial_Ext_GPS_NMEA0183Provider serial_Ext_GPS_NMEA0183Provider = (ISerial_Ext_GPS_NMEA0183Provider) theEObject;
				T result = caseSerial_Ext_GPS_NMEA0183Provider(serial_Ext_GPS_NMEA0183Provider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Serial_ext_gps_nmea0183Package.I_SERIAL_EXT_GPS_NMEA0183FACTORY: {
				ISerial_ext_gps_nmea0183Factory iSerial_ext_gps_nmea0183Factory = (ISerial_ext_gps_nmea0183Factory) theEObject;
				T result = caseISerial_ext_gps_nmea0183Factory(iSerial_ext_gps_nmea0183Factory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseISerial_Ext_GPS_NMEA0183(ISerial_Ext_GPS_NMEA0183 object) {
		return null;
	}

	public T caseSerial_Ext_GPS_NMEA0183(ISerial_Ext_GPS_NMEA0183 object) {
		return null;
	}

	public T caseISerial_Ext_GPS_NMEA0183Provider(ISerial_Ext_GPS_NMEA0183Provider object) {
		return null;
	}

	public T caseSerial_Ext_GPS_NMEA0183Provider(ISerial_Ext_GPS_NMEA0183Provider object) {
		return null;
	}

	public T caseISerial_ext_gps_nmea0183Factory(ISerial_ext_gps_nmea0183Factory object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //Serial_ext_gps_nmea0183Switch
