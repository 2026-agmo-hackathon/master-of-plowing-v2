/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor.util;

import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor;
import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider;
import com.bosch.nevonex.can_agmo_customized_tractor.ICan_agmo_customized_tractorFactory;

import com.bosch.nevonex.can_agmo_customized_tractor.impl.Can_agmo_customized_tractorPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class Can_agmo_customized_tractorAdapterFactory extends AdapterFactoryImpl {
	protected static Can_agmo_customized_tractorPackage modelPackage;

	public Can_agmo_customized_tractorAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_customized_tractorPackage.eINSTANCE;
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

	protected Can_agmo_customized_tractorSwitch<Adapter> modelSwitch = new Can_agmo_customized_tractorSwitch<Adapter>() {
		@Override
		public Adapter caseICAN_AGMO_Customized_Tractor(ICAN_AGMO_Customized_Tractor object) {
			return createICAN_AGMO_Customized_TractorAdapter();
		}

		@Override
		public Adapter caseCAN_AGMO_Customized_Tractor(ICAN_AGMO_Customized_Tractor object) {
			return createCAN_AGMO_Customized_TractorAdapter();
		}

		@Override
		public Adapter caseICAN_AGMO_Customized_TractorProvider(ICAN_AGMO_Customized_TractorProvider object) {
			return createICAN_AGMO_Customized_TractorProviderAdapter();
		}

		@Override
		public Adapter caseCAN_AGMO_Customized_TractorProvider(ICAN_AGMO_Customized_TractorProvider object) {
			return createCAN_AGMO_Customized_TractorProviderAdapter();
		}

		@Override
		public Adapter caseICan_agmo_customized_tractorFactory(ICan_agmo_customized_tractorFactory object) {
			return createICan_agmo_customized_tractorFactoryAdapter();
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

	public Adapter createICAN_AGMO_Customized_TractorAdapter() {
		return null;
	}

	public Adapter createCAN_AGMO_Customized_TractorAdapter() {
		return null;
	}

	public Adapter createICAN_AGMO_Customized_TractorProviderAdapter() {
		return null;
	}

	public Adapter createCAN_AGMO_Customized_TractorProviderAdapter() {
		return null;
	}

	public Adapter createICan_agmo_customized_tractorFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //Can_agmo_customized_tractorAdapterFactory
