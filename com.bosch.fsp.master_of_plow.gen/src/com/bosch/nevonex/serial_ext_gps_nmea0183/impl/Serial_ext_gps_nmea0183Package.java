/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.impl;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_ext_gps_nmea0183Factory;
import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_ext_gps_nmea0183Factory;

import com.bosch.nevonex.fcal.impl.FcalPackage;
import com.bosch.nevonex.common.impl.CommonPackage;

import org.eclipse.emf.ecore.EAttribute;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EFactory;
import org.eclipse.emf.ecore.EOperation;
import org.eclipse.emf.ecore.EPackage;
import org.eclipse.emf.ecore.EParameter;
import org.eclipse.emf.ecore.EReference;
import org.eclipse.emf.ecore.EcoreFactory;
import org.eclipse.emf.ecore.EcorePackage;

import org.eclipse.emf.ecore.impl.EPackageImpl;

/**
 * @generated
 */
public class Serial_ext_gps_nmea0183Package extends EPackageImpl {
	public static final String eNAME = "serial_ext_gps_nmea0183";
	public static final String eNS_URI = "com.bosch.nevonex.serial_ext_gps_nmea0183";
	public static final String eNS_PREFIX = "serial_ext_gps_nmea0183";
	public static final Serial_ext_gps_nmea0183Package eINSTANCE = com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_ext_gps_nmea0183Package.init();

	public static final int I_SERIAL_EXT_GPS_NMEA0183 = 0;
	public static final int I_SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA = 0;
	public static final int I_SERIAL_EXT_GPS_NMEA0183_FEATURE_COUNT = 1;
	public static final int I_SERIAL_EXT_GPS_NMEA0183_OPERATION_COUNT = 0;

	public static final int SERIAL_EXT_GPS_NMEA0183 = 1;
	public static final int SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int SERIAL_EXT_GPS_NMEA0183_FEATURE_COUNT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int SERIAL_EXT_GPS_NMEA0183_OPERATION_COUNT = CommonPackage.TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int I_SERIAL_EXT_GPS_NMEA0183PROVIDER = 2;
	public static final int I_SERIAL_EXT_GPS_NMEA0183PROVIDER__SERIAL_EXT_GPS_NMEA0183 = 0;
	public static final int I_SERIAL_EXT_GPS_NMEA0183PROVIDER_FEATURE_COUNT = 1;
	public static final int I_SERIAL_EXT_GPS_NMEA0183PROVIDER_OPERATION_COUNT = 0;

	public static final int SERIAL_EXT_GPS_NMEA0183PROVIDER = 3;
	public static final int SERIAL_EXT_GPS_NMEA0183PROVIDER_FEATURE_COUNT = 0;
	public static final int SERIAL_EXT_GPS_NMEA0183PROVIDER_OPERATION_COUNT = 0;

	public static final int I_SERIAL_EXT_GPS_NMEA0183FACTORY = 4;
	public static final int I_SERIAL_EXT_GPS_NMEA0183FACTORY_FEATURE_COUNT = 0;
	public static final int I_SERIAL_EXT_GPS_NMEA0183FACTORY_OPERATION_COUNT = 0;


	private EClass iSerial_Ext_GPS_NMEA0183EClass = null;
	private EClass serial_Ext_GPS_NMEA0183EClass = null;
	private EClass iSerial_Ext_GPS_NMEA0183ProviderEClass = null;
	private EClass serial_Ext_GPS_NMEA0183ProviderEClass = null;
	private EClass iSerial_ext_gps_nmea0183FactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public Serial_ext_gps_nmea0183Package() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(Serial_ext_gps_nmea0183Factory.eINSTANCE);
	}

	public static Serial_ext_gps_nmea0183Package init() {
		if (isInited) return (Serial_ext_gps_nmea0183Package) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		Serial_ext_gps_nmea0183Package thePackage = (registered instanceof Serial_ext_gps_nmea0183Package) ? (Serial_ext_gps_nmea0183Package) registered : new Serial_ext_gps_nmea0183Package();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
		FcalPackage.eINSTANCE.eClass();
		CommonPackage.eINSTANCE.eClass();
		thePackage.createPackageContents();
		thePackage.initializePackageContents(thePackage);
		thePackage.freeze();
		EPackage.Registry.INSTANCE.put(eNS_URI, thePackage);
		return thePackage;
	}

	private void createPackageContents() {
		if (isCreated) return;
		isCreated = true;
		createPackageContents_part0();
	}

	private void createPackageContents_part0() {
		iSerial_Ext_GPS_NMEA0183EClass = createEClass(I_SERIAL_EXT_GPS_NMEA0183);
		createEReference(iSerial_Ext_GPS_NMEA0183EClass, I_SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA);

		serial_Ext_GPS_NMEA0183EClass = createEClass(SERIAL_EXT_GPS_NMEA0183);
		createEReference(serial_Ext_GPS_NMEA0183EClass, SERIAL_EXT_GPS_NMEA0183__SERIAL_EXT_GPS_NMEA0183_DATA);

		iSerial_Ext_GPS_NMEA0183ProviderEClass = createEClass(I_SERIAL_EXT_GPS_NMEA0183PROVIDER);
		createEReference(iSerial_Ext_GPS_NMEA0183ProviderEClass, I_SERIAL_EXT_GPS_NMEA0183PROVIDER__SERIAL_EXT_GPS_NMEA0183);

		serial_Ext_GPS_NMEA0183ProviderEClass = createEClass(SERIAL_EXT_GPS_NMEA0183PROVIDER);

		iSerial_ext_gps_nmea0183FactoryEClass = createEClass(I_SERIAL_EXT_GPS_NMEA0183FACTORY);

	}

	private void initializePackageContents(Serial_ext_gps_nmea0183Package thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(Serial_ext_gps_nmea0183Package thePackage) {
		thePackage.iSerial_Ext_GPS_NMEA0183EClass.setName("ISerial_Ext_GPS_NMEA0183");
		thePackage.iSerial_Ext_GPS_NMEA0183EClass.setInstanceClassName("com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183");
		thePackage.iSerial_Ext_GPS_NMEA0183EClass.setAbstract(true);
		thePackage.iSerial_Ext_GPS_NMEA0183EClass.setInterface(true);
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setName("serial_Ext_GPS_NMEA0183_Data");
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getGPS_NMEA0183_T_R());
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0))).setContainment(false);
		thePackage.serial_Ext_GPS_NMEA0183EClass.setName("Serial_Ext_GPS_NMEA0183");
		thePackage.serial_Ext_GPS_NMEA0183EClass.setInstanceClassName("com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183");
		thePackage.serial_Ext_GPS_NMEA0183EClass.setAbstract(false);
		thePackage.serial_Ext_GPS_NMEA0183EClass.setInterface(false);
		thePackage.serial_Ext_GPS_NMEA0183EClass.getESuperTypes().add((EClass) CommonPackage.eINSTANCE.getTopicObject());
		((EReference) thePackage.serial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setName("serial_Ext_GPS_NMEA0183_Data");
		((EReference) thePackage.serial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getGPS_NMEA0183_T_R());
		((EReference) thePackage.serial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.serial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.serial_Ext_GPS_NMEA0183EClass.getEStructuralFeatures().get(0))).setContainment(false);
		thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.setName("ISerial_Ext_GPS_NMEA0183Provider");
		thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.setInstanceClassName("com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider");
		thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.setAbstract(true);
		thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.setInterface(true);
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeatures().get(0)).setName("serial_Ext_GPS_NMEA0183");
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) thePackage.iSerial_Ext_GPS_NMEA0183EClass);
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeatures().get(0))).setContainment(true);
		thePackage.serial_Ext_GPS_NMEA0183ProviderEClass.setName("Serial_Ext_GPS_NMEA0183Provider");
		thePackage.serial_Ext_GPS_NMEA0183ProviderEClass.setInstanceClassName("com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider");
		thePackage.serial_Ext_GPS_NMEA0183ProviderEClass.setAbstract(false);
		thePackage.serial_Ext_GPS_NMEA0183ProviderEClass.setInterface(false);
		thePackage.iSerial_ext_gps_nmea0183FactoryEClass.setName("ISerial_ext_gps_nmea0183Factory");
		thePackage.iSerial_ext_gps_nmea0183FactoryEClass.setInstanceClassName("com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_ext_gps_nmea0183Factory");
		thePackage.iSerial_ext_gps_nmea0183FactoryEClass.setAbstract(true);
		thePackage.iSerial_ext_gps_nmea0183FactoryEClass.setInterface(true);
	}

	public EClass getISerial_Ext_GPS_NMEA0183() { return iSerial_Ext_GPS_NMEA0183EClass; }
	public EClass getSerial_Ext_GPS_NMEA0183() { return serial_Ext_GPS_NMEA0183EClass; }
	public EClass getISerial_Ext_GPS_NMEA0183Provider() { return iSerial_Ext_GPS_NMEA0183ProviderEClass; }
	public EClass getSerial_Ext_GPS_NMEA0183Provider() { return serial_Ext_GPS_NMEA0183ProviderEClass; }
	public EClass getISerial_ext_gps_nmea0183Factory() { return iSerial_ext_gps_nmea0183FactoryEClass; }
	public EReference getISerial_Ext_GPS_NMEA0183_Serial_Ext_GPS_NMEA0183_Data() {
		return (EReference) iSerial_Ext_GPS_NMEA0183EClass.getEStructuralFeature("serial_Ext_GPS_NMEA0183_Data");
	}
	public EReference getSerial_Ext_GPS_NMEA0183_Serial_Ext_GPS_NMEA0183_Data() {
		return (EReference) serial_Ext_GPS_NMEA0183EClass.getEStructuralFeature("serial_Ext_GPS_NMEA0183_Data");
	}
	public EReference getISerial_Ext_GPS_NMEA0183Provider_Serial_Ext_GPS_NMEA0183() {
		return (EReference) iSerial_Ext_GPS_NMEA0183ProviderEClass.getEStructuralFeature("serial_Ext_GPS_NMEA0183");
	}

	public ISerial_ext_gps_nmea0183Factory getSerial_ext_gps_nmea0183Factory() {
		return (ISerial_ext_gps_nmea0183Factory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_SERIAL_EXT_GPS_NMEA0183 = eINSTANCE.getISerial_Ext_GPS_NMEA0183();
		public static final EClass SERIAL_EXT_GPS_NMEA0183 = eINSTANCE.getSerial_Ext_GPS_NMEA0183();
		public static final EClass I_SERIAL_EXT_GPS_NMEA0183PROVIDER = eINSTANCE.getISerial_Ext_GPS_NMEA0183Provider();
		public static final EClass SERIAL_EXT_GPS_NMEA0183PROVIDER = eINSTANCE.getSerial_Ext_GPS_NMEA0183Provider();
		public static final EClass I_SERIAL_EXT_GPS_NMEA0183FACTORY = eINSTANCE.getISerial_ext_gps_nmea0183Factory();
	}

} //Serial_ext_gps_nmea0183Package
