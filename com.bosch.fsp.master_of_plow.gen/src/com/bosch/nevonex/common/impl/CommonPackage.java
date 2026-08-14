/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.impl;

import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.ICommonFactory;
import com.bosch.nevonex.common.impl.CommonFactory;

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
public class CommonPackage extends EPackageImpl {
	public static final String eNAME = "common";
	public static final String eNS_URI = "com.bosch.nevonex.common";
	public static final String eNS_PREFIX = "common";
	public static final CommonPackage eINSTANCE = com.bosch.nevonex.common.impl.CommonPackage.init();

	public static final int I_ABSOLUTE_POSITION = 0;
	public static final int I_ABSOLUTE_POSITION_FEATURE_COUNT = 0;
	public static final int I_ABSOLUTE_POSITION_OPERATION_COUNT = 0;

	public static final int I_TOPIC_OBJECT = 1;
	public static final int I_TOPIC_OBJECT_FEATURE_COUNT = 0;
	public static final int I_TOPIC_OBJECT_OPERATION_COUNT = 0;

	public static final int ABSOLUTE_POSITION = 2;
	public static final int ABSOLUTE_POSITION__ALTITUDE = I_ABSOLUTE_POSITION_FEATURE_COUNT + 0;
	public static final int ABSOLUTE_POSITION__LATITUDE = I_ABSOLUTE_POSITION_FEATURE_COUNT + 1;
	public static final int ABSOLUTE_POSITION__LONGITUDE = I_ABSOLUTE_POSITION_FEATURE_COUNT + 2;
	public static final int ABSOLUTE_POSITION_FEATURE_COUNT = I_ABSOLUTE_POSITION_FEATURE_COUNT + 3;
	public static final int ABSOLUTE_POSITION_OPERATION_COUNT = I_ABSOLUTE_POSITION_OPERATION_COUNT + 0;

	public static final int TOPIC_OBJECT = 3;
	public static final int TOPIC_OBJECT__INDEX = I_TOPIC_OBJECT_FEATURE_COUNT + 0;
	public static final int TOPIC_OBJECT_FEATURE_COUNT = I_TOPIC_OBJECT_FEATURE_COUNT + 1;
	public static final int TOPIC_OBJECT_OPERATION_COUNT = I_TOPIC_OBJECT_OPERATION_COUNT + 0;

	public static final int I_COMMON_FACTORY = 4;
	public static final int I_COMMON_FACTORY_FEATURE_COUNT = 0;
	public static final int I_COMMON_FACTORY_OPERATION_COUNT = 0;


	private EClass iAbsolutePositionEClass = null;
	private EClass iTopicObjectEClass = null;
	private EClass absolutePositionEClass = null;
	private EClass topicObjectEClass = null;
	private EClass iCommonFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public CommonPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(CommonFactory.eINSTANCE);
	}

	public static CommonPackage init() {
		if (isInited) return (CommonPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		CommonPackage thePackage = (registered instanceof CommonPackage) ? (CommonPackage) registered : new CommonPackage();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
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
		iAbsolutePositionEClass = createEClass(I_ABSOLUTE_POSITION);

		iTopicObjectEClass = createEClass(I_TOPIC_OBJECT);

		absolutePositionEClass = createEClass(ABSOLUTE_POSITION);
		createEAttribute(absolutePositionEClass, ABSOLUTE_POSITION__ALTITUDE);
		createEAttribute(absolutePositionEClass, ABSOLUTE_POSITION__LATITUDE);
		createEAttribute(absolutePositionEClass, ABSOLUTE_POSITION__LONGITUDE);

		topicObjectEClass = createEClass(TOPIC_OBJECT);
		createEAttribute(topicObjectEClass, TOPIC_OBJECT__INDEX);

		iCommonFactoryEClass = createEClass(I_COMMON_FACTORY);

	}

	private void initializePackageContents(CommonPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(CommonPackage thePackage) {
		thePackage.iAbsolutePositionEClass.setName("IAbsolutePosition");
		thePackage.iAbsolutePositionEClass.setInstanceClassName("com.bosch.nevonex.common.IAbsolutePosition");
		thePackage.iAbsolutePositionEClass.setAbstract(true);
		thePackage.iAbsolutePositionEClass.setInterface(true);
		thePackage.iTopicObjectEClass.setName("ITopicObject");
		thePackage.iTopicObjectEClass.setInstanceClassName("com.bosch.nevonex.common.ITopicObject");
		thePackage.iTopicObjectEClass.setAbstract(true);
		thePackage.iTopicObjectEClass.setInterface(true);
		thePackage.absolutePositionEClass.setName("AbsolutePosition");
		thePackage.absolutePositionEClass.setInstanceClassName("com.bosch.nevonex.common.IAbsolutePosition");
		thePackage.absolutePositionEClass.setAbstract(false);
		thePackage.absolutePositionEClass.setInterface(false);
		thePackage.absolutePositionEClass.getESuperTypes().add((EClass) thePackage.iAbsolutePositionEClass);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(0)).setName("altitude");
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEDouble());
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0");
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(1)).setName("latitude");
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEDouble());
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0");
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(2)).setName("longitude");
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEDouble());
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.absolutePositionEClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0.0");
		thePackage.topicObjectEClass.setName("TopicObject");
		thePackage.topicObjectEClass.setInstanceClassName("com.bosch.nevonex.common.ITopicObject");
		thePackage.topicObjectEClass.setAbstract(false);
		thePackage.topicObjectEClass.setInterface(false);
		thePackage.topicObjectEClass.getESuperTypes().add((EClass) thePackage.iTopicObjectEClass);
		((EAttribute) thePackage.topicObjectEClass.getEStructuralFeatures().get(0)).setName("index");
		((EAttribute) thePackage.topicObjectEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.topicObjectEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.topicObjectEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.topicObjectEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0");
		thePackage.iCommonFactoryEClass.setName("ICommonFactory");
		thePackage.iCommonFactoryEClass.setInstanceClassName("com.bosch.nevonex.common.ICommonFactory");
		thePackage.iCommonFactoryEClass.setAbstract(true);
		thePackage.iCommonFactoryEClass.setInterface(true);
	}

	public EClass getIAbsolutePosition() { return iAbsolutePositionEClass; }
	public EClass getITopicObject() { return iTopicObjectEClass; }
	public EClass getAbsolutePosition() { return absolutePositionEClass; }
	public EClass getTopicObject() { return topicObjectEClass; }
	public EClass getICommonFactory() { return iCommonFactoryEClass; }
	public EAttribute getAbsolutePosition_Altitude() {
		return (EAttribute) absolutePositionEClass.getEStructuralFeature("altitude");
	}
	public EAttribute getAbsolutePosition_Latitude() {
		return (EAttribute) absolutePositionEClass.getEStructuralFeature("latitude");
	}
	public EAttribute getAbsolutePosition_Longitude() {
		return (EAttribute) absolutePositionEClass.getEStructuralFeature("longitude");
	}
	public EAttribute getTopicObject_Index() {
		return (EAttribute) topicObjectEClass.getEStructuralFeature("index");
	}

	public ICommonFactory getCommonFactory() {
		return (ICommonFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_ABSOLUTE_POSITION = eINSTANCE.getIAbsolutePosition();
		public static final EClass I_TOPIC_OBJECT = eINSTANCE.getITopicObject();
		public static final EClass ABSOLUTE_POSITION = eINSTANCE.getAbsolutePosition();
		public static final EClass TOPIC_OBJECT = eINSTANCE.getTopicObject();
		public static final EClass I_COMMON_FACTORY = eINSTANCE.getICommonFactory();
	}

} //CommonPackage
