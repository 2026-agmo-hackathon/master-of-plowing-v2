/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.exception.util;

import com.bosch.nevonex.exception.IExceptionFactory;

import com.bosch.nevonex.exception.impl.ExceptionPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class ExceptionAdapterFactory extends AdapterFactoryImpl {
	protected static ExceptionPackage modelPackage;

	public ExceptionAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = ExceptionPackage.eINSTANCE;
		}
	}

	@Override
	public boolean isFactoryForType(Object object) {
		if (object == modelPackage) {
			return true;
		}
		if (object instanceof EObject) {
			return ((EObject) object).eClass().getEPackage() == modelPackage;
		}
		return false;
	}

	protected ExceptionSwitch<Adapter> modelSwitch = new ExceptionSwitch<Adapter>() {
		@Override
		public Adapter caseIExceptionFactory(IExceptionFactory object) {
			return createIExceptionFactoryAdapter();
		}

		@Override
		public Adapter defaultCase(EObject object) {
			return createEObjectAdapter();
		}
	};

	@Override
	public Adapter createAdapter(Notifier target) {
		return modelSwitch.doSwitch((EObject) target);
	}

	public Adapter createIExceptionFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //ExceptionAdapterFactory
