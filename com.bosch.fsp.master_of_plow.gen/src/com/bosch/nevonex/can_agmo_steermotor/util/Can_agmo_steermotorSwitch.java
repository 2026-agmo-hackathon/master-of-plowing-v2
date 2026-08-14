/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor.util;

import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor;
import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotorProvider;
import com.bosch.nevonex.can_agmo_steermotor.ICan_agmo_steermotorFactory;

import com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.common.ITopicObject;

/**
 * @generated
 */
public class Can_agmo_steermotorSwitch<T> extends Switch<T> {
	protected static Can_agmo_steermotorPackage modelPackage;

	public Can_agmo_steermotorSwitch() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_steermotorPackage.eINSTANCE;
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
			case Can_agmo_steermotorPackage.ICAN_AGMO_STEER_MOTOR: {
				ICAN_AGMO_SteerMotor iCAN_AGMO_SteerMotor = (ICAN_AGMO_SteerMotor) theEObject;
				T result = caseICAN_AGMO_SteerMotor(iCAN_AGMO_SteerMotor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR: {
				ICAN_AGMO_SteerMotor cAN_AGMO_SteerMotor = (ICAN_AGMO_SteerMotor) theEObject;
				T result = caseCAN_AGMO_SteerMotor(cAN_AGMO_SteerMotor);
				if (result == null)
					result = caseTopicObject((ITopicObject) cAN_AGMO_SteerMotor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_steermotorPackage.ICAN_AGMO_STEER_MOTOR_PROVIDER: {
				ICAN_AGMO_SteerMotorProvider iCAN_AGMO_SteerMotorProvider = (ICAN_AGMO_SteerMotorProvider) theEObject;
				T result = caseICAN_AGMO_SteerMotorProvider(iCAN_AGMO_SteerMotorProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR_PROVIDER: {
				ICAN_AGMO_SteerMotorProvider cAN_AGMO_SteerMotorProvider = (ICAN_AGMO_SteerMotorProvider) theEObject;
				T result = caseCAN_AGMO_SteerMotorProvider(cAN_AGMO_SteerMotorProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_steermotorPackage.I_CAN_AGMO_STEERMOTOR_FACTORY: {
				ICan_agmo_steermotorFactory iCan_agmo_steermotorFactory = (ICan_agmo_steermotorFactory) theEObject;
				T result = caseICan_agmo_steermotorFactory(iCan_agmo_steermotorFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseICAN_AGMO_SteerMotor(ICAN_AGMO_SteerMotor object) {
		return null;
	}

	public T caseCAN_AGMO_SteerMotor(ICAN_AGMO_SteerMotor object) {
		return null;
	}

	public T caseICAN_AGMO_SteerMotorProvider(ICAN_AGMO_SteerMotorProvider object) {
		return null;
	}

	public T caseCAN_AGMO_SteerMotorProvider(ICAN_AGMO_SteerMotorProvider object) {
		return null;
	}

	public T caseICan_agmo_steermotorFactory(ICan_agmo_steermotorFactory object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //Can_agmo_steermotorSwitch
