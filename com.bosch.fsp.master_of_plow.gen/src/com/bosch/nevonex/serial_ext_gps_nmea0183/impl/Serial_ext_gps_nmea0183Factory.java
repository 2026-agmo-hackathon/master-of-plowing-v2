/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.impl;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_ext_gps_nmea0183Factory;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class Serial_ext_gps_nmea0183Factory extends EFactoryImpl implements ISerial_ext_gps_nmea0183Factory {
	public static final Serial_ext_gps_nmea0183Factory eINSTANCE = init();

	public static Serial_ext_gps_nmea0183Factory init() {
		try {
			Serial_ext_gps_nmea0183Factory theSerial_ext_gps_nmea0183Factory = (Serial_ext_gps_nmea0183Factory) EPackage.Registry.INSTANCE.getEFactory(Serial_ext_gps_nmea0183Package.eNS_URI);
			if (theSerial_ext_gps_nmea0183Factory != null) {
				return theSerial_ext_gps_nmea0183Factory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new Serial_ext_gps_nmea0183Factory();
	}

	public Serial_ext_gps_nmea0183Factory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183:
			return createSerial_Ext_GPS_NMEA0183();
		case Serial_ext_gps_nmea0183Package.SERIAL_EXT_GPS_NMEA0183PROVIDER:
			return createSerial_Ext_GPS_NMEA0183Provider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ISerial_Ext_GPS_NMEA0183 createSerial_Ext_GPS_NMEA0183() {
		Serial_Ext_GPS_NMEA0183 serial_Ext_GPS_NMEA0183 = new Serial_Ext_GPS_NMEA0183();
		return serial_Ext_GPS_NMEA0183;
	}

	public ISerial_Ext_GPS_NMEA0183Provider createSerial_Ext_GPS_NMEA0183Provider() {
		Serial_Ext_GPS_NMEA0183Provider serial_Ext_GPS_NMEA0183Provider = new Serial_Ext_GPS_NMEA0183Provider();
		return serial_Ext_GPS_NMEA0183Provider;
	}

	public Serial_ext_gps_nmea0183Package getSerial_ext_gps_nmea0183Package() {
		return (Serial_ext_gps_nmea0183Package) getEPackage();
	}

	@Deprecated
	public static Serial_ext_gps_nmea0183Package getPackage() {
		return Serial_ext_gps_nmea0183Package.eINSTANCE;
	}

} //Serial_ext_gps_nmea0183Factory
