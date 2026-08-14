/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor.impl;

import com.bosch.nevonex.can_agmo_steermotor.ICan_agmo_steermotorFactory;
import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor;
import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class Can_agmo_steermotorFactory extends EFactoryImpl implements ICan_agmo_steermotorFactory {
	public static final Can_agmo_steermotorFactory eINSTANCE = init();

	public static Can_agmo_steermotorFactory init() {
		try {
			Can_agmo_steermotorFactory theCan_agmo_steermotorFactory = (Can_agmo_steermotorFactory) EPackage.Registry.INSTANCE.getEFactory(Can_agmo_steermotorPackage.eNS_URI);
			if (theCan_agmo_steermotorFactory != null) {
				return theCan_agmo_steermotorFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new Can_agmo_steermotorFactory();
	}

	public Can_agmo_steermotorFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR:
			return createCAN_AGMO_SteerMotor();
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR_PROVIDER:
			return createCAN_AGMO_SteerMotorProvider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ICAN_AGMO_SteerMotor createCAN_AGMO_SteerMotor() {
		CAN_AGMO_SteerMotor cAN_AGMO_SteerMotor = new CAN_AGMO_SteerMotor();
		return cAN_AGMO_SteerMotor;
	}

	public ICAN_AGMO_SteerMotorProvider createCAN_AGMO_SteerMotorProvider() {
		CAN_AGMO_SteerMotorProvider cAN_AGMO_SteerMotorProvider = new CAN_AGMO_SteerMotorProvider();
		return cAN_AGMO_SteerMotorProvider;
	}

	public Can_agmo_steermotorPackage getCan_agmo_steermotorPackage() {
		return (Can_agmo_steermotorPackage) getEPackage();
	}

	@Deprecated
	public static Can_agmo_steermotorPackage getPackage() {
		return Can_agmo_steermotorPackage.eINSTANCE;
	}

} //Can_agmo_steermotorFactory
