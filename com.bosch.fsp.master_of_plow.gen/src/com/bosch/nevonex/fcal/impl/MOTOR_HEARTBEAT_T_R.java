/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class MOTOR_HEARTBEAT_T_R extends EObjectImpl implements IMOTOR_HEARTBEAT_T_R {
	protected int heartbeat_ControlStatus_HallFailure = 0;
	protected int heartbeat_ControlStatus_CANdisconnected = 0;
	protected int heartbeat_ControlStatus_MotorStalled = 0;
	protected int heartbeat_ControlStatus_Disabled = 0;
	protected int heartbeat_ControlStatus_Overvoltage = 0;
	protected int heartbeat_ControlStatus_HardwareProtection = 0;
	protected int heartbeat_ControlStatus_E2PROM = 0;
	protected int heartbeat_ControlStatus_Undervoltage = 0;
	protected int heartbeat_ControlStatus_Overcurrent = 0;
	protected int heartbeat_ControlStatus_ModeFailure = 0;

	protected MOTOR_HEARTBEAT_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.MOTOR_HEARTBEAT_T_R;
	}

	public int getHeartbeat_ControlStatus_HallFailure() {
		return heartbeat_ControlStatus_HallFailure;
	}

	public void setHeartbeat_ControlStatus_HallFailure(int newHeartbeat_ControlStatus_HallFailure) {
		heartbeat_ControlStatus_HallFailure = newHeartbeat_ControlStatus_HallFailure;
	}

	public int getHeartbeat_ControlStatus_CANdisconnected() {
		return heartbeat_ControlStatus_CANdisconnected;
	}

	public void setHeartbeat_ControlStatus_CANdisconnected(int newHeartbeat_ControlStatus_CANdisconnected) {
		heartbeat_ControlStatus_CANdisconnected = newHeartbeat_ControlStatus_CANdisconnected;
	}

	public int getHeartbeat_ControlStatus_MotorStalled() {
		return heartbeat_ControlStatus_MotorStalled;
	}

	public void setHeartbeat_ControlStatus_MotorStalled(int newHeartbeat_ControlStatus_MotorStalled) {
		heartbeat_ControlStatus_MotorStalled = newHeartbeat_ControlStatus_MotorStalled;
	}

	public int getHeartbeat_ControlStatus_Disabled() {
		return heartbeat_ControlStatus_Disabled;
	}

	public void setHeartbeat_ControlStatus_Disabled(int newHeartbeat_ControlStatus_Disabled) {
		heartbeat_ControlStatus_Disabled = newHeartbeat_ControlStatus_Disabled;
	}

	public int getHeartbeat_ControlStatus_Overvoltage() {
		return heartbeat_ControlStatus_Overvoltage;
	}

	public void setHeartbeat_ControlStatus_Overvoltage(int newHeartbeat_ControlStatus_Overvoltage) {
		heartbeat_ControlStatus_Overvoltage = newHeartbeat_ControlStatus_Overvoltage;
	}

	public int getHeartbeat_ControlStatus_HardwareProtection() {
		return heartbeat_ControlStatus_HardwareProtection;
	}

	public void setHeartbeat_ControlStatus_HardwareProtection(int newHeartbeat_ControlStatus_HardwareProtection) {
		heartbeat_ControlStatus_HardwareProtection = newHeartbeat_ControlStatus_HardwareProtection;
	}

	public int getHeartbeat_ControlStatus_E2PROM() {
		return heartbeat_ControlStatus_E2PROM;
	}

	public void setHeartbeat_ControlStatus_E2PROM(int newHeartbeat_ControlStatus_E2PROM) {
		heartbeat_ControlStatus_E2PROM = newHeartbeat_ControlStatus_E2PROM;
	}

	public int getHeartbeat_ControlStatus_Undervoltage() {
		return heartbeat_ControlStatus_Undervoltage;
	}

	public void setHeartbeat_ControlStatus_Undervoltage(int newHeartbeat_ControlStatus_Undervoltage) {
		heartbeat_ControlStatus_Undervoltage = newHeartbeat_ControlStatus_Undervoltage;
	}

	public int getHeartbeat_ControlStatus_Overcurrent() {
		return heartbeat_ControlStatus_Overcurrent;
	}

	public void setHeartbeat_ControlStatus_Overcurrent(int newHeartbeat_ControlStatus_Overcurrent) {
		heartbeat_ControlStatus_Overcurrent = newHeartbeat_ControlStatus_Overcurrent;
	}

	public int getHeartbeat_ControlStatus_ModeFailure() {
		return heartbeat_ControlStatus_ModeFailure;
	}

	public void setHeartbeat_ControlStatus_ModeFailure(int newHeartbeat_ControlStatus_ModeFailure) {
		heartbeat_ControlStatus_ModeFailure = newHeartbeat_ControlStatus_ModeFailure;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE:
			return getHeartbeat_ControlStatus_HallFailure();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED:
			return getHeartbeat_ControlStatus_CANdisconnected();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED:
			return getHeartbeat_ControlStatus_MotorStalled();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED:
			return getHeartbeat_ControlStatus_Disabled();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE:
			return getHeartbeat_ControlStatus_Overvoltage();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION:
			return getHeartbeat_ControlStatus_HardwareProtection();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM:
			return getHeartbeat_ControlStatus_E2PROM();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE:
			return getHeartbeat_ControlStatus_Undervoltage();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT:
			return getHeartbeat_ControlStatus_Overcurrent();
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE:
			return getHeartbeat_ControlStatus_ModeFailure();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE:
			setHeartbeat_ControlStatus_HallFailure((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED:
			setHeartbeat_ControlStatus_CANdisconnected((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED:
			setHeartbeat_ControlStatus_MotorStalled((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED:
			setHeartbeat_ControlStatus_Disabled((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE:
			setHeartbeat_ControlStatus_Overvoltage((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION:
			setHeartbeat_ControlStatus_HardwareProtection((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM:
			setHeartbeat_ControlStatus_E2PROM((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE:
			setHeartbeat_ControlStatus_Undervoltage((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT:
			setHeartbeat_ControlStatus_Overcurrent((Integer) newValue);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE:
			setHeartbeat_ControlStatus_ModeFailure((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE:
			setHeartbeat_ControlStatus_HallFailure(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED:
			setHeartbeat_ControlStatus_CANdisconnected(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED:
			setHeartbeat_ControlStatus_MotorStalled(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED:
			setHeartbeat_ControlStatus_Disabled(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE:
			setHeartbeat_ControlStatus_Overvoltage(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION:
			setHeartbeat_ControlStatus_HardwareProtection(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM:
			setHeartbeat_ControlStatus_E2PROM(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE:
			setHeartbeat_ControlStatus_Undervoltage(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT:
			setHeartbeat_ControlStatus_Overcurrent(0);
			return;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE:
			setHeartbeat_ControlStatus_ModeFailure(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE:
			return heartbeat_ControlStatus_HallFailure != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED:
			return heartbeat_ControlStatus_CANdisconnected != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED:
			return heartbeat_ControlStatus_MotorStalled != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED:
			return heartbeat_ControlStatus_Disabled != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE:
			return heartbeat_ControlStatus_Overvoltage != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION:
			return heartbeat_ControlStatus_HardwareProtection != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM:
			return heartbeat_ControlStatus_E2PROM != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE:
			return heartbeat_ControlStatus_Undervoltage != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT:
			return heartbeat_ControlStatus_Overcurrent != 0;
		case FcalPackage.MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE:
			return heartbeat_ControlStatus_ModeFailure != 0;
		}
		return super.eIsSet(featureID);
	}
} //MOTOR_HEARTBEAT_T_R
