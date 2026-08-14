/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305.util;

import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305;
import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider;
import com.bosch.nevonex.can_agmo_mtlt305.ICan_agmo_mtlt305Factory;

import com.bosch.nevonex.can_agmo_mtlt305.impl.Can_agmo_mtlt305Package;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class Can_agmo_mtlt305AdapterFactory extends AdapterFactoryImpl {
	protected static Can_agmo_mtlt305Package modelPackage;

	public Can_agmo_mtlt305AdapterFactory() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_mtlt305Package.eINSTANCE;
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

	protected Can_agmo_mtlt305Switch<Adapter> modelSwitch = new Can_agmo_mtlt305Switch<Adapter>() {
		@Override
		public Adapter caseICAN_AGMO_MTLT305(ICAN_AGMO_MTLT305 object) {
			return createICAN_AGMO_MTLT305Adapter();
		}

		@Override
		public Adapter caseCAN_AGMO_MTLT305(ICAN_AGMO_MTLT305 object) {
			return createCAN_AGMO_MTLT305Adapter();
		}

		@Override
		public Adapter caseICAN_AGMO_MTLT305Provider(ICAN_AGMO_MTLT305Provider object) {
			return createICAN_AGMO_MTLT305ProviderAdapter();
		}

		@Override
		public Adapter caseCAN_AGMO_MTLT305Provider(ICAN_AGMO_MTLT305Provider object) {
			return createCAN_AGMO_MTLT305ProviderAdapter();
		}

		@Override
		public Adapter caseICan_agmo_mtlt305Factory(ICan_agmo_mtlt305Factory object) {
			return createICan_agmo_mtlt305FactoryAdapter();
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

	public Adapter createICAN_AGMO_MTLT305Adapter() {
		return null;
	}

	public Adapter createCAN_AGMO_MTLT305Adapter() {
		return null;
	}

	public Adapter createICAN_AGMO_MTLT305ProviderAdapter() {
		return null;
	}

	public Adapter createCAN_AGMO_MTLT305ProviderAdapter() {
		return null;
	}

	public Adapter createICan_agmo_mtlt305FactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //Can_agmo_mtlt305AdapterFactory
