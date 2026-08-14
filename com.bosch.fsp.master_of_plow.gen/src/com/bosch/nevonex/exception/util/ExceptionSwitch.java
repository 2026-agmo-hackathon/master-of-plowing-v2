/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.exception.util;

import com.bosch.nevonex.exception.IExceptionFactory;

import com.bosch.nevonex.exception.impl.ExceptionPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;

/**
 * @generated
 */
public class ExceptionSwitch<T> extends Switch<T> {
	protected static ExceptionPackage modelPackage;

	public ExceptionSwitch() {
		if (modelPackage == null) {
			modelPackage = ExceptionPackage.eINSTANCE;
		}
	}

	@Override
	protected boolean isSwitchFor(EPackage ePackage) {
		return ePackage == modelPackage;
	}

	@Override
	protected T doSwitch(int classifierID, EObject theEObject) {
		if (classifierID < 80) return doSwitch_chunk0(classifierID, theEObject);
		return defaultCase(theEObject);
	}

	private T doSwitch_chunk0(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case ExceptionPackage.I_EXCEPTION_FACTORY: {
				IExceptionFactory iExceptionFactory = (IExceptionFactory) theEObject;
				T result = caseIExceptionFactory(iExceptionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIExceptionFactory(IExceptionFactory object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //ExceptionSwitch
