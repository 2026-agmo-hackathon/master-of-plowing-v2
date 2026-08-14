/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.exception.impl;

import com.bosch.nevonex.exception.IExceptionFactory;
import com.bosch.nevonex.exception.impl.ExceptionFactory;

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
public class ExceptionPackage extends EPackageImpl {
	public static final String eNAME = "exception";
	public static final String eNS_URI = "com.bosch.nevonex.exception";
	public static final String eNS_PREFIX = "exception";
	public static final ExceptionPackage eINSTANCE = com.bosch.nevonex.exception.impl.ExceptionPackage.init();

	public static final int I_EXCEPTION_FACTORY = 0;
	public static final int I_EXCEPTION_FACTORY_FEATURE_COUNT = 0;
	public static final int I_EXCEPTION_FACTORY_OPERATION_COUNT = 0;


	private EClass iExceptionFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public ExceptionPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(ExceptionFactory.eINSTANCE);
	}

	public static ExceptionPackage init() {
		if (isInited) return (ExceptionPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		ExceptionPackage thePackage = (registered instanceof ExceptionPackage) ? (ExceptionPackage) registered : new ExceptionPackage();
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
		iExceptionFactoryEClass = createEClass(I_EXCEPTION_FACTORY);

	}

	private void initializePackageContents(ExceptionPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(ExceptionPackage thePackage) {
		thePackage.iExceptionFactoryEClass.setName("IExceptionFactory");
		thePackage.iExceptionFactoryEClass.setInstanceClassName("com.bosch.nevonex.exception.IExceptionFactory");
		thePackage.iExceptionFactoryEClass.setAbstract(true);
		thePackage.iExceptionFactoryEClass.setInterface(true);
	}

	public EClass getIExceptionFactory() { return iExceptionFactoryEClass; }

	public IExceptionFactory getExceptionFactory() {
		return (IExceptionFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_EXCEPTION_FACTORY = eINSTANCE.getIExceptionFactory();
	}

} //ExceptionPackage
