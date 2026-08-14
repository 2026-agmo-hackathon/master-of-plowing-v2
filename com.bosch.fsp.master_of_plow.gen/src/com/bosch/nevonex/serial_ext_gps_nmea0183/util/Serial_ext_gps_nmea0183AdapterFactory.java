/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.util;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_ext_gps_nmea0183Factory;

import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_ext_gps_nmea0183Package;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class Serial_ext_gps_nmea0183AdapterFactory extends AdapterFactoryImpl {
	protected static Serial_ext_gps_nmea0183Package modelPackage;

	public Serial_ext_gps_nmea0183AdapterFactory() {
		if (modelPackage == null) {
			modelPackage = Serial_ext_gps_nmea0183Package.eINSTANCE;
		}
	}

	@Override
	public boolean isFactoryForType(Object object) {
		if (object == modelPackage) {
			return true;
		}
		if (object instanceof EObject) {
			return ((EObject) object).eClass().getEPackage() == modelPackage;
		}
		return false;
	}

	protected Serial_ext_gps_nmea0183Switch<Adapter> modelSwitch = new Serial_ext_gps_nmea0183Switch<Adapter>() {
		@Override
		public Adapter caseISerial_Ext_GPS_NMEA0183(ISerial_Ext_GPS_NMEA0183 object) {
			return createISerial_Ext_GPS_NMEA0183Adapter();
		}

		@Override
		public Adapter caseSerial_Ext_GPS_NMEA0183(ISerial_Ext_GPS_NMEA0183 object) {
			return createSerial_Ext_GPS_NMEA0183Adapter();
		}

		@Override
		public Adapter caseISerial_Ext_GPS_NMEA0183Provider(ISerial_Ext_GPS_NMEA0183Provider object) {
			return createISerial_Ext_GPS_NMEA0183ProviderAdapter();
		}

		@Override
		public Adapter caseSerial_Ext_GPS_NMEA0183Provider(ISerial_Ext_GPS_NMEA0183Provider object) {
			return createSerial_Ext_GPS_NMEA0183ProviderAdapter();
		}

		@Override
		public Adapter caseISerial_ext_gps_nmea0183Factory(ISerial_ext_gps_nmea0183Factory object) {
			return createISerial_ext_gps_nmea0183FactoryAdapter();
		}

		@Override
		public Adapter defaultCase(EObject object) {
			return createEObjectAdapter();
		}
	};

	@Override
	public Adapter createAdapter(Notifier target) {
		return modelSwitch.doSwitch((EObject) target);
	}

	public Adapter createISerial_Ext_GPS_NMEA0183Adapter() {
		return null;
	}

	public Adapter createSerial_Ext_GPS_NMEA0183Adapter() {
		return null;
	}

	public Adapter createISerial_Ext_GPS_NMEA0183ProviderAdapter() {
		return null;
	}

	public Adapter createSerial_Ext_GPS_NMEA0183ProviderAdapter() {
		return null;
	}

	public Adapter createISerial_ext_gps_nmea0183FactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //Serial_ext_gps_nmea0183AdapterFactory
