/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn.impl;

import com.bosch.nevonex.isopgn.IISOPGN;
import com.bosch.nevonex.isopgn.IISOPGNProvider;
import com.bosch.nevonex.isopgn.IIsopgnFactory;
import com.bosch.nevonex.isopgn.impl.IsopgnFactory;

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
public class IsopgnPackage extends EPackageImpl {
	public static final String eNAME = "isopgn";
	public static final String eNS_URI = "com.bosch.nevonex.isopgn";
	public static final String eNS_PREFIX = "isopgn";
	public static final IsopgnPackage eINSTANCE = com.bosch.nevonex.isopgn.impl.IsopgnPackage.init();

	public static final int IISOPGN = 0;
	public static final int IISOPGN__HITCH_POS_PER_RE = 0;
	public static final int IISOPGN_FEATURE_COUNT = 1;
	public static final int IISOPGN_OPERATION_COUNT = 0;

	public static final int ISOPGN = 1;
	public static final int ISOPGN__HITCH_POS_PER_RE = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int ISOPGN_FEATURE_COUNT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int ISOPGN_OPERATION_COUNT = CommonPackage.TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int IISOPGN_PROVIDER = 2;
	public static final int IISOPGN_PROVIDER__ISOPGN = 0;
	public static final int IISOPGN_PROVIDER_FEATURE_COUNT = 1;
	public static final int IISOPGN_PROVIDER_OPERATION_COUNT = 0;

	public static final int ISOPGN_PROVIDER = 3;
	public static final int ISOPGN_PROVIDER_FEATURE_COUNT = 0;
	public static final int ISOPGN_PROVIDER_OPERATION_COUNT = 0;

	public static final int I_ISOPGN_FACTORY = 4;
	public static final int I_ISOPGN_FACTORY_FEATURE_COUNT = 0;
	public static final int I_ISOPGN_FACTORY_OPERATION_COUNT = 0;


	private EClass iISOPGNEClass = null;
	private EClass iSOPGNEClass = null;
	private EClass iISOPGNProviderEClass = null;
	private EClass iSOPGNProviderEClass = null;
	private EClass iIsopgnFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public IsopgnPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(IsopgnFactory.eINSTANCE);
	}

	public static IsopgnPackage init() {
		if (isInited) return (IsopgnPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		IsopgnPackage thePackage = (registered instanceof IsopgnPackage) ? (IsopgnPackage) registered : new IsopgnPackage();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
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
		iISOPGNEClass = createEClass(IISOPGN);
		createEAttribute(iISOPGNEClass, IISOPGN__HITCH_POS_PER_RE);

		iSOPGNEClass = createEClass(ISOPGN);
		createEAttribute(iSOPGNEClass, ISOPGN__HITCH_POS_PER_RE);

		iISOPGNProviderEClass = createEClass(IISOPGN_PROVIDER);
		createEReference(iISOPGNProviderEClass, IISOPGN_PROVIDER__ISOPGN);

		iSOPGNProviderEClass = createEClass(ISOPGN_PROVIDER);

		iIsopgnFactoryEClass = createEClass(I_ISOPGN_FACTORY);

	}

	private void initializePackageContents(IsopgnPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(IsopgnPackage thePackage) {
		thePackage.iISOPGNEClass.setName("IISOPGN");
		thePackage.iISOPGNEClass.setInstanceClassName("com.bosch.nevonex.isopgn.IISOPGN");
		thePackage.iISOPGNEClass.setAbstract(true);
		thePackage.iISOPGNEClass.setInterface(true);
		((EAttribute) thePackage.iISOPGNEClass.getEStructuralFeatures().get(0)).setName("hitchPosPerRe");
		((EAttribute) thePackage.iISOPGNEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.iISOPGNEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.iISOPGNEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.iISOPGNEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0");
		thePackage.iSOPGNEClass.setName("ISOPGN");
		thePackage.iSOPGNEClass.setInstanceClassName("com.bosch.nevonex.isopgn.IISOPGN");
		thePackage.iSOPGNEClass.setAbstract(false);
		thePackage.iSOPGNEClass.setInterface(false);
		thePackage.iSOPGNEClass.getESuperTypes().add((EClass) CommonPackage.eINSTANCE.getTopicObject());
		((EAttribute) thePackage.iSOPGNEClass.getEStructuralFeatures().get(0)).setName("hitchPosPerRe");
		((EAttribute) thePackage.iSOPGNEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.iSOPGNEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.iSOPGNEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.iSOPGNEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0");
		thePackage.iISOPGNProviderEClass.setName("IISOPGNProvider");
		thePackage.iISOPGNProviderEClass.setInstanceClassName("com.bosch.nevonex.isopgn.IISOPGNProvider");
		thePackage.iISOPGNProviderEClass.setAbstract(true);
		thePackage.iISOPGNProviderEClass.setInterface(true);
		((EReference) thePackage.iISOPGNProviderEClass.getEStructuralFeatures().get(0)).setName("iSOPGN");
		((EReference) thePackage.iISOPGNProviderEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) thePackage.iISOPGNEClass);
		((EReference) thePackage.iISOPGNProviderEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iISOPGNProviderEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iISOPGNProviderEClass.getEStructuralFeatures().get(0))).setContainment(true);
		thePackage.iSOPGNProviderEClass.setName("ISOPGNProvider");
		thePackage.iSOPGNProviderEClass.setInstanceClassName("com.bosch.nevonex.isopgn.IISOPGNProvider");
		thePackage.iSOPGNProviderEClass.setAbstract(false);
		thePackage.iSOPGNProviderEClass.setInterface(false);
		thePackage.iIsopgnFactoryEClass.setName("IIsopgnFactory");
		thePackage.iIsopgnFactoryEClass.setInstanceClassName("com.bosch.nevonex.isopgn.IIsopgnFactory");
		thePackage.iIsopgnFactoryEClass.setAbstract(true);
		thePackage.iIsopgnFactoryEClass.setInterface(true);
	}

	public EClass getIISOPGN() { return iISOPGNEClass; }
	public EClass getISOPGN() { return iSOPGNEClass; }
	public EClass getIISOPGNProvider() { return iISOPGNProviderEClass; }
	public EClass getISOPGNProvider() { return iSOPGNProviderEClass; }
	public EClass getIIsopgnFactory() { return iIsopgnFactoryEClass; }
	public EAttribute getIISOPGN_HitchPosPerRe() {
		return (EAttribute) iISOPGNEClass.getEStructuralFeature("hitchPosPerRe");
	}
	public EAttribute getISOPGN_HitchPosPerRe() {
		return (EAttribute) iSOPGNEClass.getEStructuralFeature("hitchPosPerRe");
	}
	public EReference getIISOPGNProvider_ISOPGN() {
		return (EReference) iISOPGNProviderEClass.getEStructuralFeature("iSOPGN");
	}

	public IIsopgnFactory getIsopgnFactory() {
		return (IIsopgnFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass IISOPGN = eINSTANCE.getIISOPGN();
		public static final EClass ISOPGN = eINSTANCE.getISOPGN();
		public static final EClass IISOPGN_PROVIDER = eINSTANCE.getIISOPGNProvider();
		public static final EClass ISOPGN_PROVIDER = eINSTANCE.getISOPGNProvider();
		public static final EClass I_ISOPGN_FACTORY = eINSTANCE.getIIsopgnFactory();
	}

} //IsopgnPackage
