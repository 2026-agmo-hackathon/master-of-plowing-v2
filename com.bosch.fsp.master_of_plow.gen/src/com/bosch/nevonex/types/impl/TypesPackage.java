/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types.impl;

import com.bosch.nevonex.types.IArrayType;
import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.ITypesFactory;
import com.bosch.nevonex.types.impl.TypesFactory;

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
public class TypesPackage extends EPackageImpl {
	public static final String eNAME = "types";
	public static final String eNS_URI = "com.bosch.nevonex.types";
	public static final String eNS_PREFIX = "types";
	public static final TypesPackage eINSTANCE = com.bosch.nevonex.types.impl.TypesPackage.init();

	public static final int I_ARRAY_TYPE = 0;
	public static final int I_ARRAY_TYPE_FEATURE_COUNT = 0;
	public static final int I_ARRAY_TYPE_OPERATION_COUNT = 0;

	public static final int I_PROPERTY_CHANGE = 1;
	public static final int I_PROPERTY_CHANGE_FEATURE_COUNT = 0;
	public static final int I_PROPERTY_CHANGE_OPERATION_COUNT = 0;

	public static final int I_TYPES_FACTORY = 2;
	public static final int I_TYPES_FACTORY_FEATURE_COUNT = 0;
	public static final int I_TYPES_FACTORY_OPERATION_COUNT = 0;

	public static final int PROPERTY_CHANGE = 3;
	public static final int PROPERTY_CHANGE__LISTENERS = I_PROPERTY_CHANGE_FEATURE_COUNT + 0;
	public static final int PROPERTY_CHANGE_FEATURE_COUNT = I_PROPERTY_CHANGE_FEATURE_COUNT + 1;
	public static final int PROPERTY_CHANGE___ADD_PROPERTY_CHANGE_LISTENER__IEJAVAOBJECT = 0;
	public static final int PROPERTY_CHANGE___REMOVE_PROPERTY_CHANGE_LISTENER__IEJAVAOBJECT = 1;
	public static final int PROPERTY_CHANGE___NOTIFY_PROPERTY_CHANGE__STRING__IEJAVAOBJECT__IEJAVAOBJECT = 2;
	public static final int PROPERTY_CHANGE_OPERATION_COUNT = I_PROPERTY_CHANGE_OPERATION_COUNT + 3;


	private EClass iArrayTypeEClass = null;
	private EClass iPropertyChangeEClass = null;
	private EClass iTypesFactoryEClass = null;
	private EClass propertyChangeEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public TypesPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(TypesFactory.eINSTANCE);
	}

	public static TypesPackage init() {
		if (isInited) return (TypesPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		TypesPackage thePackage = (registered instanceof TypesPackage) ? (TypesPackage) registered : new TypesPackage();
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
		iArrayTypeEClass = createEClass(I_ARRAY_TYPE);

		iPropertyChangeEClass = createEClass(I_PROPERTY_CHANGE);

		iTypesFactoryEClass = createEClass(I_TYPES_FACTORY);

		propertyChangeEClass = createEClass(PROPERTY_CHANGE);
		createEAttribute(propertyChangeEClass, PROPERTY_CHANGE__LISTENERS);
		createEOperation(propertyChangeEClass, PROPERTY_CHANGE___ADD_PROPERTY_CHANGE_LISTENER__IEJAVAOBJECT);
		createEOperation(propertyChangeEClass, PROPERTY_CHANGE___REMOVE_PROPERTY_CHANGE_LISTENER__IEJAVAOBJECT);
		createEOperation(propertyChangeEClass, PROPERTY_CHANGE___NOTIFY_PROPERTY_CHANGE__STRING__IEJAVAOBJECT__IEJAVAOBJECT);

	}

	private void initializePackageContents(TypesPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(TypesPackage thePackage) {
		thePackage.iArrayTypeEClass.setName("IArrayType");
		thePackage.iArrayTypeEClass.setInstanceClassName("com.bosch.nevonex.types.IArrayType");
		thePackage.iArrayTypeEClass.setAbstract(true);
		thePackage.iArrayTypeEClass.setInterface(true);
		thePackage.iPropertyChangeEClass.setName("IPropertyChange");
		thePackage.iPropertyChangeEClass.setInstanceClassName("com.bosch.nevonex.types.IPropertyChange");
		thePackage.iPropertyChangeEClass.setAbstract(true);
		thePackage.iPropertyChangeEClass.setInterface(true);
		thePackage.iTypesFactoryEClass.setName("ITypesFactory");
		thePackage.iTypesFactoryEClass.setInstanceClassName("com.bosch.nevonex.types.ITypesFactory");
		thePackage.iTypesFactoryEClass.setAbstract(true);
		thePackage.iTypesFactoryEClass.setInterface(true);
		thePackage.propertyChangeEClass.setName("PropertyChange");
		thePackage.propertyChangeEClass.setInstanceClassName("com.bosch.nevonex.types.IPropertyChange");
		thePackage.propertyChangeEClass.setAbstract(false);
		thePackage.propertyChangeEClass.setInterface(false);
		thePackage.propertyChangeEClass.getESuperTypes().add((EClass) thePackage.iPropertyChangeEClass);
		((EAttribute) thePackage.propertyChangeEClass.getEStructuralFeatures().get(0)).setName("listeners");
		((EAttribute) thePackage.propertyChangeEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
		((EAttribute) thePackage.propertyChangeEClass.getEStructuralFeatures().get(0)).setUpperBound(-1);
		((EAttribute) thePackage.propertyChangeEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		thePackage.propertyChangeEClass.getEOperations().get(0).setName("addPropertyChangeListener");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("_listener");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.propertyChangeEClass.getEOperations().get(0).getEParameters().add(param);
		}
		thePackage.propertyChangeEClass.getEOperations().get(1).setName("removePropertyChangeListener");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("_listener");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.propertyChangeEClass.getEOperations().get(1).getEParameters().add(param);
		}
		thePackage.propertyChangeEClass.getEOperations().get(2).setName("notifyPropertyChange");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("_name");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.propertyChangeEClass.getEOperations().get(2).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("_oldValue");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.propertyChangeEClass.getEOperations().get(2).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("_newValue");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.propertyChangeEClass.getEOperations().get(2).getEParameters().add(param);
		}
	}

	public EClass getIArrayType() { return iArrayTypeEClass; }
	public EClass getIPropertyChange() { return iPropertyChangeEClass; }
	public EClass getITypesFactory() { return iTypesFactoryEClass; }
	public EClass getPropertyChange() { return propertyChangeEClass; }
	public EAttribute getPropertyChange_Listeners() {
		return (EAttribute) propertyChangeEClass.getEStructuralFeature("listeners");
	}
	public EOperation getPropertyChange__AddPropertyChangeListener__EJavaObject() {
		return propertyChangeEClass.getEOperations().get(0);
	}
	public EOperation getPropertyChange__RemovePropertyChangeListener__EJavaObject() {
		return propertyChangeEClass.getEOperations().get(1);
	}
	public EOperation getPropertyChange__NotifyPropertyChange__String__EJavaObject__EJavaObject() {
		return propertyChangeEClass.getEOperations().get(2);
	}

	public ITypesFactory getTypesFactory() {
		return (ITypesFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_ARRAY_TYPE = eINSTANCE.getIArrayType();
		public static final EClass I_PROPERTY_CHANGE = eINSTANCE.getIPropertyChange();
		public static final EClass I_TYPES_FACTORY = eINSTANCE.getITypesFactory();
		public static final EClass PROPERTY_CHANGE = eINSTANCE.getPropertyChange();
	}

} //TypesPackage
