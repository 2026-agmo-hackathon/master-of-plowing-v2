/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor.util;

import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor;
import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider;
import com.bosch.nevonex.can_agmo_steermotor.ICan_agmo_steermotorFactory;

import com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class Can_agmo_steermotorAdapterFactory extends AdapterFactoryImpl {
	protected static Can_agmo_steermotorPackage modelPackage;

	public Can_agmo_steermotorAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_steermotorPackage.eINSTANCE;
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

	protected Can_agmo_steermotorSwitch<Adapter> modelSwitch = new Can_agmo_steermotorSwitch<Adapter>() {
		@Override
		public Adapter caseICAN_AGMO_SteerMotor(ICAN_AGMO_SteerMotor object) {
			return createICAN_AGMO_SteerMotorAdapter();
		}

		@Override
		public Adapter caseCAN_AGMO_SteerMotor(ICAN_AGMO_SteerMotor object) {
			return createCAN_AGMO_SteerMotorAdapter();
		}

		@Override
		public Adapter caseICAN_AGMO_SteerMotorProvider(ICAN_AGMO_SteerMotorProvider object) {
			return createICAN_AGMO_SteerMotorProviderAdapter();
		}

		@Override
		public Adapter caseCAN_AGMO_SteerMotorProvider(ICAN_AGMO_SteerMotorProvider object) {
			return createCAN_AGMO_SteerMotorProviderAdapter();
		}

		@Override
		public Adapter caseICan_agmo_steermotorFactory(ICan_agmo_steermotorFactory object) {
			return createICan_agmo_steermotorFactoryAdapter();
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

	public Adapter createICAN_AGMO_SteerMotorAdapter() {
		return null;
	}

	public Adapter createCAN_AGMO_SteerMotorAdapter() {
		return null;
	}

	public Adapter createICAN_AGMO_SteerMotorProviderAdapter() {
		return null;
	}

	public Adapter createCAN_AGMO_SteerMotorProviderAdapter() {
		return null;
	}

	public Adapter createICan_agmo_steermotorFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //Can_agmo_steermotorAdapterFactory
