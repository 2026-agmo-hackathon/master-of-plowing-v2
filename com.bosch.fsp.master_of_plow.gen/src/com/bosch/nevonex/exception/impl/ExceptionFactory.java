/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.exception.impl;

import com.bosch.nevonex.exception.IExceptionFactory;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class ExceptionFactory extends EFactoryImpl implements IExceptionFactory {
	public static final ExceptionFactory eINSTANCE = init();

	public static ExceptionFactory init() {
		try {
			ExceptionFactory theExceptionFactory = (ExceptionFactory) EPackage.Registry.INSTANCE.getEFactory(ExceptionPackage.eNS_URI);
			if (theExceptionFactory != null) {
				return theExceptionFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new ExceptionFactory();
	}

	public ExceptionFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ExceptionPackage getExceptionPackage() {
		return (ExceptionPackage) getEPackage();
	}

	@Deprecated
	public static ExceptionPackage getPackage() {
		return ExceptionPackage.eINSTANCE;
	}

} //ExceptionFactory
