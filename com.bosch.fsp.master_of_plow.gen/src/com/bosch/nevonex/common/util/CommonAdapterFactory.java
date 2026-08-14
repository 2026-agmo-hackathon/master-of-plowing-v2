/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.util;

import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.ICommonFactory;

import com.bosch.nevonex.common.impl.CommonPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class CommonAdapterFactory extends AdapterFactoryImpl {
	protected static CommonPackage modelPackage;

	public CommonAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = CommonPackage.eINSTANCE;
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

	protected CommonSwitch<Adapter> modelSwitch = new CommonSwitch<Adapter>() {
		@Override
		public Adapter caseIAbsolutePosition(IAbsolutePosition object) {
			return createIAbsolutePositionAdapter();
		}

		@Override
		public Adapter caseITopicObject(ITopicObject object) {
			return createITopicObjectAdapter();
		}

		@Override
		public Adapter caseAbsolutePosition(IAbsolutePosition object) {
			return createAbsolutePositionAdapter();
		}

		@Override
		public Adapter caseTopicObject(ITopicObject object) {
			return createTopicObjectAdapter();
		}

		@Override
		public Adapter caseICommonFactory(ICommonFactory object) {
			return createICommonFactoryAdapter();
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

	public Adapter createIAbsolutePositionAdapter() {
		return null;
	}

	public Adapter createITopicObjectAdapter() {
		return null;
	}

	public Adapter createAbsolutePositionAdapter() {
		return null;
	}

	public Adapter createTopicObjectAdapter() {
		return null;
	}

	public Adapter createICommonFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //CommonAdapterFactory
