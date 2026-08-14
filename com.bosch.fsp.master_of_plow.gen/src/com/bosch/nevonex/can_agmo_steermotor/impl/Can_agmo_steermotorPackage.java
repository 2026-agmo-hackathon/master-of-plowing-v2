/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor.impl;

import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor;
import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider;
import com.bosch.nevonex.can_agmo_steermotor.ICan_agmo_steermotorFactory;
import com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorFactory;

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
public class Can_agmo_steermotorPackage extends EPackageImpl {
	public static final String eNAME = "can_agmo_steermotor";
	public static final String eNS_URI = "com.bosch.nevonex.can_agmo_steermotor";
	public static final String eNS_PREFIX = "can_agmo_steermotor";
	public static final Can_agmo_steermotorPackage eINSTANCE = com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorPackage.init();

	public static final int ICAN_AGMO_STEER_MOTOR = 0;
	public static final int ICAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT = 0;
	public static final int ICAN_AGMO_STEER_MOTOR__MOTOR_REQUEST = 1;
	public static final int ICAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED = 2;
	public static final int ICAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE = 3;
	public static final int ICAN_AGMO_STEER_MOTOR_FEATURE_COUNT = 4;
	public static final int ICAN_AGMO_STEER_MOTOR_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_STEER_MOTOR = 1;
	public static final int CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 2;
	public static final int CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 3;
	public static final int CAN_AGMO_STEER_MOTOR_FEATURE_COUNT = CommonPackage.TOPIC_OBJECT_FEATURE_COUNT + 4;
	public static final int CAN_AGMO_STEER_MOTOR_OPERATION_COUNT = CommonPackage.TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int ICAN_AGMO_STEER_MOTOR_PROVIDER = 2;
	public static final int ICAN_AGMO_STEER_MOTOR_PROVIDER__CAN_AGMO_STEER_MOTOR = 0;
	public static final int ICAN_AGMO_STEER_MOTOR_PROVIDER_FEATURE_COUNT = 1;
	public static final int ICAN_AGMO_STEER_MOTOR_PROVIDER_OPERATION_COUNT = 0;

	public static final int CAN_AGMO_STEER_MOTOR_PROVIDER = 3;
	public static final int CAN_AGMO_STEER_MOTOR_PROVIDER_FEATURE_COUNT = 0;
	public static final int CAN_AGMO_STEER_MOTOR_PROVIDER_OPERATION_COUNT = 0;

	public static final int I_CAN_AGMO_STEERMOTOR_FACTORY = 4;
	public static final int I_CAN_AGMO_STEERMOTOR_FACTORY_FEATURE_COUNT = 0;
	public static final int I_CAN_AGMO_STEERMOTOR_FACTORY_OPERATION_COUNT = 0;


	private EClass iCAN_AGMO_SteerMotorEClass = null;
	private EClass cAN_AGMO_SteerMotorEClass = null;
	private EClass iCAN_AGMO_SteerMotorProviderEClass = null;
	private EClass cAN_AGMO_SteerMotorProviderEClass = null;
	private EClass iCan_agmo_steermotorFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public Can_agmo_steermotorPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(Can_agmo_steermotorFactory.eINSTANCE);
	}

	public static Can_agmo_steermotorPackage init() {
		if (isInited) return (Can_agmo_steermotorPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		Can_agmo_steermotorPackage thePackage = (registered instanceof Can_agmo_steermotorPackage) ? (Can_agmo_steermotorPackage) registered : new Can_agmo_steermotorPackage();
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
		iCAN_AGMO_SteerMotorEClass = createEClass(ICAN_AGMO_STEER_MOTOR);
		createEReference(iCAN_AGMO_SteerMotorEClass, ICAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT);
		createEReference(iCAN_AGMO_SteerMotorEClass, ICAN_AGMO_STEER_MOTOR__MOTOR_REQUEST);
		createEAttribute(iCAN_AGMO_SteerMotorEClass, ICAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED);
		createEAttribute(iCAN_AGMO_SteerMotorEClass, ICAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE);

		cAN_AGMO_SteerMotorEClass = createEClass(CAN_AGMO_STEER_MOTOR);
		createEReference(cAN_AGMO_SteerMotorEClass, CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT);
		createEReference(cAN_AGMO_SteerMotorEClass, CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST);
		createEAttribute(cAN_AGMO_SteerMotorEClass, CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED);
		createEAttribute(cAN_AGMO_SteerMotorEClass, CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE);

		iCAN_AGMO_SteerMotorProviderEClass = createEClass(ICAN_AGMO_STEER_MOTOR_PROVIDER);
		createEReference(iCAN_AGMO_SteerMotorProviderEClass, ICAN_AGMO_STEER_MOTOR_PROVIDER__CAN_AGMO_STEER_MOTOR);

		cAN_AGMO_SteerMotorProviderEClass = createEClass(CAN_AGMO_STEER_MOTOR_PROVIDER);

		iCan_agmo_steermotorFactoryEClass = createEClass(I_CAN_AGMO_STEERMOTOR_FACTORY);

	}

	private void initializePackageContents(Can_agmo_steermotorPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(Can_agmo_steermotorPackage thePackage) {
		thePackage.iCAN_AGMO_SteerMotorEClass.setName("ICAN_AGMO_SteerMotor");
		thePackage.iCAN_AGMO_SteerMotorEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor");
		thePackage.iCAN_AGMO_SteerMotorEClass.setAbstract(true);
		thePackage.iCAN_AGMO_SteerMotorEClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setName("motor_Heartbeat");
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getMOTOR_HEARTBEAT_T_R());
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setName("motor_Request");
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getRAW_CAN_DATA_DLC_8_W());
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setName("motor_Response_Encoder_Speed");
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setName("motor_Response_EncoderCountValue");
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.iCAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0.0");
		thePackage.cAN_AGMO_SteerMotorEClass.setName("CAN_AGMO_SteerMotor");
		thePackage.cAN_AGMO_SteerMotorEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor");
		thePackage.cAN_AGMO_SteerMotorEClass.setAbstract(false);
		thePackage.cAN_AGMO_SteerMotorEClass.setInterface(false);
		thePackage.cAN_AGMO_SteerMotorEClass.getESuperTypes().add((EClass) CommonPackage.eINSTANCE.getTopicObject());
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setName("motor_Heartbeat");
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) FcalPackage.eINSTANCE.getMOTOR_HEARTBEAT_T_R());
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(0))).setContainment(false);
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setName("motor_Request");
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) FcalPackage.eINSTANCE.getRAW_CAN_DATA_DLC_8_W());
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EReference) ((EReference) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(1))).setContainment(false);
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setName("motor_Response_Encoder_Speed");
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setName("motor_Response_EncoderCountValue");
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.cAN_AGMO_SteerMotorEClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0.0");
		thePackage.iCAN_AGMO_SteerMotorProviderEClass.setName("ICAN_AGMO_SteerMotorProvider");
		thePackage.iCAN_AGMO_SteerMotorProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider");
		thePackage.iCAN_AGMO_SteerMotorProviderEClass.setAbstract(true);
		thePackage.iCAN_AGMO_SteerMotorProviderEClass.setInterface(true);
		((EReference) thePackage.iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeatures().get(0)).setName("cAN_AGMO_SteerMotor");
		((EReference) thePackage.iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) thePackage.iCAN_AGMO_SteerMotorEClass);
		((EReference) thePackage.iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EReference) thePackage.iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EReference) ((EReference) thePackage.iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeatures().get(0))).setContainment(true);
		thePackage.cAN_AGMO_SteerMotorProviderEClass.setName("CAN_AGMO_SteerMotorProvider");
		thePackage.cAN_AGMO_SteerMotorProviderEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider");
		thePackage.cAN_AGMO_SteerMotorProviderEClass.setAbstract(false);
		thePackage.cAN_AGMO_SteerMotorProviderEClass.setInterface(false);
		thePackage.iCan_agmo_steermotorFactoryEClass.setName("ICan_agmo_steermotorFactory");
		thePackage.iCan_agmo_steermotorFactoryEClass.setInstanceClassName("com.bosch.nevonex.can_agmo_steermotor.ICan_agmo_steermotorFactory");
		thePackage.iCan_agmo_steermotorFactoryEClass.setAbstract(true);
		thePackage.iCan_agmo_steermotorFactoryEClass.setInterface(true);
	}

	public EClass getICAN_AGMO_SteerMotor() { return iCAN_AGMO_SteerMotorEClass; }
	public EClass getCAN_AGMO_SteerMotor() { return cAN_AGMO_SteerMotorEClass; }
	public EClass getICAN_AGMO_SteerMotorProvider() { return iCAN_AGMO_SteerMotorProviderEClass; }
	public EClass getCAN_AGMO_SteerMotorProvider() { return cAN_AGMO_SteerMotorProviderEClass; }
	public EClass getICan_agmo_steermotorFactory() { return iCan_agmo_steermotorFactoryEClass; }
	public EReference getICAN_AGMO_SteerMotor_Motor_Heartbeat() {
		return (EReference) iCAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Heartbeat");
	}
	public EReference getICAN_AGMO_SteerMotor_Motor_Request() {
		return (EReference) iCAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Request");
	}
	public EAttribute getICAN_AGMO_SteerMotor_Motor_Response_Encoder_Speed() {
		return (EAttribute) iCAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Response_Encoder_Speed");
	}
	public EAttribute getICAN_AGMO_SteerMotor_Motor_Response_EncoderCountValue() {
		return (EAttribute) iCAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Response_EncoderCountValue");
	}
	public EReference getCAN_AGMO_SteerMotor_Motor_Heartbeat() {
		return (EReference) cAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Heartbeat");
	}
	public EReference getCAN_AGMO_SteerMotor_Motor_Request() {
		return (EReference) cAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Request");
	}
	public EAttribute getCAN_AGMO_SteerMotor_Motor_Response_Encoder_Speed() {
		return (EAttribute) cAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Response_Encoder_Speed");
	}
	public EAttribute getCAN_AGMO_SteerMotor_Motor_Response_EncoderCountValue() {
		return (EAttribute) cAN_AGMO_SteerMotorEClass.getEStructuralFeature("motor_Response_EncoderCountValue");
	}
	public EReference getICAN_AGMO_SteerMotorProvider_CAN_AGMO_SteerMotor() {
		return (EReference) iCAN_AGMO_SteerMotorProviderEClass.getEStructuralFeature("cAN_AGMO_SteerMotor");
	}

	public ICan_agmo_steermotorFactory getCan_agmo_steermotorFactory() {
		return (ICan_agmo_steermotorFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass ICAN_AGMO_STEER_MOTOR = eINSTANCE.getICAN_AGMO_SteerMotor();
		public static final EClass CAN_AGMO_STEER_MOTOR = eINSTANCE.getCAN_AGMO_SteerMotor();
		public static final EClass ICAN_AGMO_STEER_MOTOR_PROVIDER = eINSTANCE.getICAN_AGMO_SteerMotorProvider();
		public static final EClass CAN_AGMO_STEER_MOTOR_PROVIDER = eINSTANCE.getCAN_AGMO_SteerMotorProvider();
		public static final EClass I_CAN_AGMO_STEERMOTOR_FACTORY = eINSTANCE.getICan_agmo_steermotorFactory();
	}

} //Can_agmo_steermotorPackage
