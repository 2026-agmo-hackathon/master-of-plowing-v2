/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types.util;

import com.bosch.nevonex.types.IArrayType;
import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.ITypesFactory;

import com.bosch.nevonex.types.impl.TypesPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class TypesAdapterFactory extends AdapterFactoryImpl {
	protected static TypesPackage modelPackage;

	public TypesAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = TypesPackage.eINSTANCE;
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

	protected TypesSwitch<Adapter> modelSwitch = new TypesSwitch<Adapter>() {
		@Override
		public Adapter caseIArrayType(IArrayType object) {
			return createIArrayTypeAdapter();
		}

		@Override
		public Adapter caseIPropertyChange(IPropertyChange object) {
			return createIPropertyChangeAdapter();
		}

		@Override
		public Adapter caseITypesFactory(ITypesFactory object) {
			return createITypesFactoryAdapter();
		}

		@Override
		public Adapter casePropertyChange(IPropertyChange object) {
			return createPropertyChangeAdapter();
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

	public Adapter createIArrayTypeAdapter() {
		return null;
	}

	public Adapter createIPropertyChangeAdapter() {
		return null;
	}

	public Adapter createITypesFactoryAdapter() {
		return null;
	}

	public Adapter createPropertyChangeAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //TypesAdapterFactory
