/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305.impl;

import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305;
import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider;
import com.bosch.nevonex.can_agmo_mtlt305.ICan_agmo_mtlt305Factory;
import com.bosch.nevonex.can_agmo_mtlt305.impl.Can_agmo_mtlt305Factory;

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
public class Can_agmo_mtlt305Package extends EPackageImpl {
	public static final String eNAME = "can_agmo_mtlt305";
	public static final String eNS_URI = "com.bosch.nevonex.can_agmo_mtlt305";
	public static final String eNS_PREFIX = "can_agmo_mtlt305";
	public static final Can_agmo_mtlt305Package eINSTANCE = com.bosch.nevonex.can_agmo_mtlt305.impl.Can_agmo_mtlt305Package.init();

	public static final int ICAN_AGMO_MTLT305 = 0;
	public static final int ICAN_AGMO_MTLT305__ACEINNA_ACCEL = 0;
	public static final int ICAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE = 1;
	public static final int ICAN_AGMO_MTLT305__ACEINNA_ANGLES = 2;
	public static final int ICAN_AGMO_MTLT305_FEATURE_COUNT = 3;
	public static final int ICAN_AGMO_MTLT305_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_MTLT305 = 1;
	public static final int CAN_AGMO_MTLT305__ACEINNA_ACCEL = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int CAN_AGMO_MTLT305__ACEINNA_ANGLES = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 2;
	public static final int CAN_AGMO_MTLT305_FEATURE_COUNT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 3;
	public static final int CAN_AGMO_MTLT305_OPERATION_COUNT = CommonPackage.TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int ICAN_AGMO_MTLT305PROVIDER = 2;
	public static final int ICAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305 = 0;
	public static final int ICAN_AGMO_MTLT305PROVIDER_FEATURE_COUNT = 1;
	public static final int ICAN_AGMO_MTLT305PROVIDER_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_MTLT305PROVIDER = 3;
	public static final int CAN_AGMO_MTLT305PROVIDER_FEATURE_COUNT = 0;
	public static final int CAN_AGMO_MTLT305PROVIDER_OPERATION_COUNT = 0;

	public static final int I_CAN_AGMO_MTLT305FACTORY = 4;
	public static final int I_CAN_AGMO_MTLT305FACTORY_FEATURE_COUNT = 0;
	public static final int I_CAN_AGMO_MTLT305FACTORY_OPERATION_COUNT = 0;


	private EClass iCAN_AGMO_MTLT305EClass = null;
	private EClass cAN_AGMO_MTLT305EClass = null;
	private EClass iCAN_AGMO_MTLT305ProviderEClass = null;
	private EClass cAN_AGMO_MTLT305ProviderEClass = null;
	private EClass iCan_agmo_mtlt305FactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public Can_agmo_mtlt305Package() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(Can_agmo_mtlt305Factory.eINSTANCE);
	}

	public static Can_agmo_mtlt305Package init() {
		if (isInited) return (Can_agmo_mtlt305Package) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		Can_agmo_mtlt305Package thePackage = (registered instanceof Can_agmo_mtlt305Package) ? (Can_agmo_mtlt305Package) registered : new Can_agmo_mtlt305Package();
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
		iCAN_AGMO_MTLT305EClass = createEClass(ICAN_AGMO_MTLT305);
		createEReference(iCAN_AGMO_MTLT305EClass, ICAN_AGMO_MTLT305__ACEINNA_ACCEL);
		createEReference(iCAN_AGMO_MTLT305EClass, ICAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE);
		createEReference(iCAN_AGMO_MTLT305EClass, ICAN_AGMO_MTLT305__ACEINNA_ANGLES);

		cAN_AGMO_MTLT305EClass = createEClass(CAN_AGMO_MTLT305);
		createEReference(cAN_AGMO_MTLT305EClass, CAN_AGMO_MTLT305__ACEINNA_ACCEL);
		createEReference(cAN_AGMO_MTLT305EClass, CAN_AGMO_MTLT305__ACEINNA_ANGLE_RATE);
		createEReference(cAN_AGMO_MTLT305EClass, CAN_AGMO_MTLT305__ACEINNA_ANGLES);

		iCAN_AGMO_MTLT305ProviderEClass = createEClass(ICAN_AGMO_MTLT305PROVIDER);
		createEReference(iCAN_AGMO_MTLT305ProviderEClass, ICAN_AGMO_MTLT305PROVIDER__CAN_AGMO_MTLT305);

		cAN_AGMO_MTLT305ProviderEClass = createEClass(CAN_AGMO_MTLT305PROVIDER);

		iCan_agmo_mtlt305FactoryEClass = createEClass(I_CAN_AGMO_MTLT305FACTORY);

	}

	private void initializePackageContents(Can_agmo_mtlt305Package thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(Can_agmo_mtlt305Package thePackage) {
		thePackage.iCAN_AGMO_MTLT305EClass.setName("ICAN_AGMO_MTLT305");
		thePackage.iCAN_AGMO_MTLT305EClass.setInstanceClassName("com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305");
		thePackage.iCAN_AGMO_MTLT305EClass.setAbstract(true);
		thePackage.iCAN_AGMO_MTLT305EClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setName("aceinna_Accel");
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ACCEL_T_R());
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setName("aceinna_AngleRate");
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ANGLERATE_T_R());
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setName("aceinna_Angles");
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ANGLES_T_R());
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2))).setContainment(false);
		thePackage.cAN_AGMO_MTLT305EClass.setName("CAN_AGMO_MTLT305");
		thePackage.cAN_AGMO_MTLT305EClass.setInstanceClassName("com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305");
		thePackage.cAN_AGMO_MTLT305EClass.setAbstract(false);
		thePackage.cAN_AGMO_MTLT305EClass.setInterface(false);
		thePackage.cAN_AGMO_MTLT305EClass.getESuperTypes().add((EClass) CommonPackage.eINSTANCE.getTopicObject());
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setName("aceinna_Accel");
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ACCEL_T_R());
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setName("aceinna_AngleRate");
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ANGLERATE_T_R());
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setName("aceinna_Angles");
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setEType((EClassifier) FcalPackage.eINSTANCE.getACEINNA_ANGLES_T_R());
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_MTLT305EClass.getEStructuralFeatures().get(2))).setContainment(false);
		thePackage.iCAN_AGMO_MTLT305ProviderEClass.setName("ICAN_AGMO_MTLT305Provider");
		thePackage.iCAN_AGMO_MTLT305ProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider");
		thePackage.iCAN_AGMO_MTLT305ProviderEClass.setAbstract(true);
		thePackage.iCAN_AGMO_MTLT305ProviderEClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeatures().get(0)).setName("cAN_AGMO_MTLT305");
		((EReference) thePackage.iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) thePackage.iCAN_AGMO_MTLT305EClass);
		((EReference) thePackage.iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeatures().get(0))).setContainment(true);
		thePackage.cAN_AGMO_MTLT305ProviderEClass.setName("CAN_AGMO_MTLT305Provider");
		thePackage.cAN_AGMO_MTLT305ProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider");
		thePackage.cAN_AGMO_MTLT305ProviderEClass.setAbstract(false);
		thePackage.cAN_AGMO_MTLT305ProviderEClass.setInterface(false);
		thePackage.iCan_agmo_mtlt305FactoryEClass.setName("ICan_agmo_mtlt305Factory");
		thePackage.iCan_agmo_mtlt305FactoryEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_mtlt305.ICan_agmo_mtlt305Factory");
		thePackage.iCan_agmo_mtlt305FactoryEClass.setAbstract(true);
		thePackage.iCan_agmo_mtlt305FactoryEClass.setInterface(true);
	}

	public EClass getICAN_AGMO_MTLT305() { return iCAN_AGMO_MTLT305EClass; }
	public EClass getCAN_AGMO_MTLT305() { return cAN_AGMO_MTLT305EClass; }
	public EClass getICAN_AGMO_MTLT305Provider() { return iCAN_AGMO_MTLT305ProviderEClass; }
	public EClass getCAN_AGMO_MTLT305Provider() { return cAN_AGMO_MTLT305ProviderEClass; }
	public EClass getICan_agmo_mtlt305Factory() { return iCan_agmo_mtlt305FactoryEClass; }
	public EReference getICAN_AGMO_MTLT305_Aceinna_Accel() {
		return (EReference) iCAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_Accel");
	}
	public EReference getICAN_AGMO_MTLT305_Aceinna_AngleRate() {
		return (EReference) iCAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_AngleRate");
	}
	public EReference getICAN_AGMO_MTLT305_Aceinna_Angles() {
		return (EReference) iCAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_Angles");
	}
	public EReference getCAN_AGMO_MTLT305_Aceinna_Accel() {
		return (EReference) cAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_Accel");
	}
	public EReference getCAN_AGMO_MTLT305_Aceinna_AngleRate() {
		return (EReference) cAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_AngleRate");
	}
	public EReference getCAN_AGMO_MTLT305_Aceinna_Angles() {
		return (EReference) cAN_AGMO_MTLT305EClass.getEStructuralFeature("aceinna_Angles");
	}
	public EReference getICAN_AGMO_MTLT305Provider_CAN_AGMO_MTLT305() {
		return (EReference) iCAN_AGMO_MTLT305ProviderEClass.getEStructuralFeature("cAN_AGMO_MTLT305");
	}

	public ICan_agmo_mtlt305Factory getCan_agmo_mtlt305Factory() {
		return (ICan_agmo_mtlt305Factory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass ICAN_AGMO_MTLT305 = eINSTANCE.getICAN_AGMO_MTLT305();
		public static final EClass CAN_AGMO_MTLT305 = eINSTANCE.getCAN_AGMO_MTLT305();
		public static final EClass ICAN_AGMO_MTLT305PROVIDER = eINSTANCE.getICAN_AGMO_MTLT305Provider();
		public static final EClass CAN_AGMO_MTLT305PROVIDER = eINSTANCE.getCAN_AGMO_MTLT305Provider();
		public static final EClass I_CAN_AGMO_MTLT305FACTORY = eINSTANCE.getICan_agmo_mtlt305Factory();
	}

} //Can_agmo_mtlt305Package
