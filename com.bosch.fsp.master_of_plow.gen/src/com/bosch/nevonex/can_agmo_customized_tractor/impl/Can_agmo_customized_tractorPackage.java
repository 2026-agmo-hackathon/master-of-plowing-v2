/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor.impl;

import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor;
import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider;
import com.bosch.nevonex.can_agmo_customized_tractor.ICan_agmo_customized_tractorFactory;
import com.bosch.nevonex.can_agmo_customized_tractor.impl.Can_agmo_customized_tractorFactory;

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
public class Can_agmo_customized_tractorPackage extends EPackageImpl {
	public static final String eNAME = "can_agmo_customized_tractor";
	public static final String eNS_URI = "com.bosch.nevonex.can_agmo_customized_tractor";
	public static final String eNS_PREFIX = "can_agmo_customized_tractor";
	public static final Can_agmo_customized_tractorPackage eINSTANCE = com.bosch.nevonex.can_agmo_customized_tractor.impl.Can_agmo_customized_tractorPackage.init();

	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR = 0;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO = 0;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO = 1;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO = 2;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO = 3;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD = 4;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD = 5;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD = 6;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD = 7;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_FEATURE_COUNT = 8;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR = 1;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 2;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 3;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 4;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 5;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 6;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 7;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_FEATURE_COUNT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 8;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_OPERATION_COUNT = CommonPackage.TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER = 2;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR = 0;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_FEATURE_COUNT = 1;
	public static final int ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER = 3;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_FEATURE_COUNT = 0;
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_OPERATION_COUNT = 0;

	public static final int I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY = 4;
	public static final int I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY_FEATURE_COUNT = 0;
	public static final int I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY_OPERATION_COUNT = 0;


	private EClass iCAN_AGMO_Customized_TractorEClass = null;
	private EClass cAN_AGMO_Customized_TractorEClass = null;
	private EClass iCAN_AGMO_Customized_TractorProviderEClass = null;
	private EClass cAN_AGMO_Customized_TractorProviderEClass = null;
	private EClass iCan_agmo_customized_tractorFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public Can_agmo_customized_tractorPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(Can_agmo_customized_tractorFactory.eINSTANCE);
	}

	public static Can_agmo_customized_tractorPackage init() {
		if (isInited) return (Can_agmo_customized_tractorPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		Can_agmo_customized_tractorPackage thePackage = (registered instanceof Can_agmo_customized_tractorPackage) ? (Can_agmo_customized_tractorPackage) registered : new Can_agmo_customized_tractorPackage();
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
		iCAN_AGMO_Customized_TractorEClass = createEClass(ICAN_AGMO_CUSTOMIZED_TRACTOR);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD);
		createEReference(iCAN_AGMO_Customized_TractorEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD);

		cAN_AGMO_Customized_TractorEClass = createEClass(CAN_AGMO_CUSTOMIZED_TRACTOR);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD);
		createEReference(cAN_AGMO_Customized_TractorEClass, CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD);

		iCAN_AGMO_Customized_TractorProviderEClass = createEClass(ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER);
		createEReference(iCAN_AGMO_Customized_TractorProviderEClass, ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER__CAN_AGMO_CUSTOMIZED_TRACTOR);

		cAN_AGMO_Customized_TractorProviderEClass = createEClass(CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER);

		iCan_agmo_customized_tractorFactoryEClass = createEClass(I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY);

	}

	private void initializePackageContents(Can_agmo_customized_tractorPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(Can_agmo_customized_tractorPackage thePackage) {
		thePackage.iCAN_AGMO_Customized_TractorEClass.setName("ICAN_AGMO_Customized_Tractor");
		thePackage.iCAN_AGMO_Customized_TractorEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor");
		thePackage.iCAN_AGMO_Customized_TractorEClass.setAbstract(true);
		thePackage.iCAN_AGMO_Customized_TractorEClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setName("receive_ACC_INFO");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_ACC_INFO_T_R());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setName("receive_FNR_INFO");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_FNR_INFO_T_R());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setName("receive_HYD_INFO");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_HYD_INFO_T_R());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setName("receive_SFT_INFO");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_SFT_INFO_T_R());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setName("send_ACC_CMD");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setName("send_FNR_CMD");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setName("send_HYD_CMD");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setName("send_SFT_CMD");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7))).setContainment(false);
		thePackage.cAN_AGMO_Customized_TractorEClass.setName("CAN_AGMO_Customized_Tractor");
		thePackage.cAN_AGMO_Customized_TractorEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor");
		thePackage.cAN_AGMO_Customized_TractorEClass.setAbstract(false);
		thePackage.cAN_AGMO_Customized_TractorEClass.setInterface(false);
		thePackage.cAN_AGMO_Customized_TractorEClass.getESuperTypes().add((EClass) CommonPackage.eINSTANCE.getTopicObject());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setName("receive_ACC_INFO");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_ACC_INFO_T_R());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setName("receive_FNR_INFO");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_FNR_INFO_T_R());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setName("receive_HYD_INFO");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_HYD_INFO_T_R());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(2))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setName("receive_SFT_INFO");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setEType((EClassifier) FcalPackage.eINSTANCE.getRECEIVE_SFT_INFO_T_R());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(3))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setName("send_ACC_CMD");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(4))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setName("send_FNR_CMD");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(5))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setName("send_HYD_CMD");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(6))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setName("send_SFT_CMD");
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setEType((EClassifier) FcalPackage.eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W());
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_Customized_TractorEClass.getEStructuralFeatures().get(7))).setContainment(false);
		thePackage.iCAN_AGMO_Customized_TractorProviderEClass.setName("ICAN_AGMO_Customized_TractorProvider");
		thePackage.iCAN_AGMO_Customized_TractorProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider");
		thePackage.iCAN_AGMO_Customized_TractorProviderEClass.setAbstract(true);
		thePackage.iCAN_AGMO_Customized_TractorProviderEClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeatures().get(0)).setName("cAN_AGMO_Customized_Tractor");
		((EReference) thePackage.iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) thePackage.iCAN_AGMO_Customized_TractorEClass);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeatures().get(0))).setContainment(true);
		thePackage.cAN_AGMO_Customized_TractorProviderEClass.setName("CAN_AGMO_Customized_TractorProvider");
		thePackage.cAN_AGMO_Customized_TractorProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider");
		thePackage.cAN_AGMO_Customized_TractorProviderEClass.setAbstract(false);
		thePackage.cAN_AGMO_Customized_TractorProviderEClass.setInterface(false);
		thePackage.iCan_agmo_customized_tractorFactoryEClass.setName("ICan_agmo_customized_tractorFactory");
		thePackage.iCan_agmo_customized_tractorFactoryEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_customized_tractor.ICan_agmo_customized_tractorFactory");
		thePackage.iCan_agmo_customized_tractorFactoryEClass.setAbstract(true);
		thePackage.iCan_agmo_customized_tractorFactoryEClass.setInterface(true);
	}

	public EClass getICAN_AGMO_Customized_Tractor() { return iCAN_AGMO_Customized_TractorEClass; }
	public EClass getCAN_AGMO_Customized_Tractor() { return cAN_AGMO_Customized_TractorEClass; }
	public EClass getICAN_AGMO_Customized_TractorProvider() { return iCAN_AGMO_Customized_TractorProviderEClass; }
	public EClass getCAN_AGMO_Customized_TractorProvider() { return cAN_AGMO_Customized_TractorProviderEClass; }
	public EClass getICan_agmo_customized_tractorFactory() { return iCan_agmo_customized_tractorFactoryEClass; }
	public EReference getICAN_AGMO_Customized_Tractor_Receive_ACC_INFO() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_ACC_INFO");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Receive_FNR_INFO() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_FNR_INFO");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Receive_HYD_INFO() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_HYD_INFO");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Receive_SFT_INFO() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_SFT_INFO");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Send_ACC_CMD() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_ACC_CMD");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Send_FNR_CMD() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_FNR_CMD");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Send_HYD_CMD() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_HYD_CMD");
	}
	public EReference getICAN_AGMO_Customized_Tractor_Send_SFT_CMD() {
		return (EReference) iCAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_SFT_CMD");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Receive_ACC_INFO() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_ACC_INFO");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Receive_FNR_INFO() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_FNR_INFO");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Receive_HYD_INFO() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_HYD_INFO");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Receive_SFT_INFO() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("receive_SFT_INFO");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Send_ACC_CMD() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_ACC_CMD");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Send_FNR_CMD() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_FNR_CMD");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Send_HYD_CMD() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_HYD_CMD");
	}
	public EReference getCAN_AGMO_Customized_Tractor_Send_SFT_CMD() {
		return (EReference) cAN_AGMO_Customized_TractorEClass.getEStructuralFeature("send_SFT_CMD");
	}
	public EReference getICAN_AGMO_Customized_TractorProvider_CAN_AGMO_Customized_Tractor() {
		return (EReference) iCAN_AGMO_Customized_TractorProviderEClass.getEStructuralFeature("cAN_AGMO_Customized_Tractor");
	}

	public ICan_agmo_customized_tractorFactory getCan_agmo_customized_tractorFactory() {
		return (ICan_agmo_customized_tractorFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass ICAN_AGMO_CUSTOMIZED_TRACTOR = eINSTANCE.getICAN_AGMO_Customized_Tractor();
		public static final EClass CAN_AGMO_CUSTOMIZED_TRACTOR = eINSTANCE.getCAN_AGMO_Customized_Tractor();
		public static final EClass ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER = eINSTANCE.getICAN_AGMO_Customized_TractorProvider();
		public static final EClass CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER = eINSTANCE.getCAN_AGMO_Customized_TractorProvider();
		public static final EClass I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY = eINSTANCE.getICan_agmo_customized_tractorFactory();
	}

} //Can_agmo_customized_tractorPackage
