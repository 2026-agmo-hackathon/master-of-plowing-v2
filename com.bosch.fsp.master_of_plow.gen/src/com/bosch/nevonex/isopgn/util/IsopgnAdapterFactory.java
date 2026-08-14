/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn.util;

import com.bosch.nevonex.isopgn.IISOPGN;
import com.bosch.nevonex.isopgn.IISOPGNProvider;
import com.bosch.nevonex.isopgn.IIsopgnFactory;

import com.bosch.nevonex.isopgn.impl.IsopgnPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class IsopgnAdapterFactory extends AdapterFactoryImpl {
	protected static IsopgnPackage modelPackage;

	public IsopgnAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = IsopgnPackage.eINSTANCE;
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

	protected IsopgnSwitch<Adapter> modelSwitch = new IsopgnSwitch<Adapter>() {
		@Override
		public Adapter caseIISOPGN(IISOPGN object) {
			return createIISOPGNAdapter();
		}

		@Override
		public Adapter caseISOPGN(IISOPGN object) {
			return createISOPGNAdapter();
		}

		@Override
		public Adapter caseIISOPGNProvider(IISOPGNProvider object) {
			return createIISOPGNProviderAdapter();
		}

		@Override
		public Adapter caseISOPGNProvider(IISOPGNProvider object) {
			return createISOPGNProviderAdapter();
		}

		@Override
		public Adapter caseIIsopgnFactory(IIsopgnFactory object) {
			return createIIsopgnFactoryAdapter();
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

	public Adapter createIISOPGNAdapter() {
		return null;
	}

	public Adapter createISOPGNAdapter() {
		return null;
	}

	public Adapter createIISOPGNProviderAdapter() {
		return null;
	}

	public Adapter createISOPGNProviderAdapter() {
		return null;
	}

	public Adapter createIIsopgnFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //IsopgnAdapterFactory
