/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class ACEINNA_ANGLERATE_T_R extends EObjectImpl implements IACEINNA_ANGLERATE_T_R {
	protected float gyroY = 0.0f;
	protected float gyroX = 0.0f;
	protected float gyroZ = 0.0f;
	protected int gyroY_FigureOfMerit = 0;
	protected int gyroX_FigureOfMerit = 0;
	protected int gyroZ_FigureOfMerit = 0;
	protected float angleRate_Latency = 0.0f;

	protected ACEINNA_ANGLERATE_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.ACEINNA_ANGLERATE_T_R;
	}

	public float getGyroY() {
		return gyroY;
	}

	public void setGyroY(float newGyroY) {
		gyroY = newGyroY;
	}

	public float getGyroX() {
		return gyroX;
	}

	public void setGyroX(float newGyroX) {
		gyroX = newGyroX;
	}

	public float getGyroZ() {
		return gyroZ;
	}

	public void setGyroZ(float newGyroZ) {
		gyroZ = newGyroZ;
	}

	public int getGyroY_FigureOfMerit() {
		return gyroY_FigureOfMerit;
	}

	public void setGyroY_FigureOfMerit(int newGyroY_FigureOfMerit) {
		gyroY_FigureOfMerit = newGyroY_FigureOfMerit;
	}

	public int getGyroX_FigureOfMerit() {
		return gyroX_FigureOfMerit;
	}

	public void setGyroX_FigureOfMerit(int newGyroX_FigureOfMerit) {
		gyroX_FigureOfMerit = newGyroX_FigureOfMerit;
	}

	public int getGyroZ_FigureOfMerit() {
		return gyroZ_FigureOfMerit;
	}

	public void setGyroZ_FigureOfMerit(int newGyroZ_FigureOfMerit) {
		gyroZ_FigureOfMerit = newGyroZ_FigureOfMerit;
	}

	public float getAngleRate_Latency() {
		return angleRate_Latency;
	}

	public void setAngleRate_Latency(float newAngleRate_Latency) {
		angleRate_Latency = newAngleRate_Latency;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y:
			return getGyroY();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X:
			return getGyroX();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z:
			return getGyroZ();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT:
			return getGyroY_FigureOfMerit();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT:
			return getGyroX_FigureOfMerit();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT:
			return getGyroZ_FigureOfMerit();
		case FcalPackage.ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY:
			return getAngleRate_Latency();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y:
			setGyroY((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X:
			setGyroX((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z:
			setGyroZ((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT:
			setGyroY_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT:
			setGyroX_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT:
			setGyroZ_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY:
			setAngleRate_Latency((Float) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y:
			setGyroY(0.0f);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X:
			setGyroX(0.0f);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z:
			setGyroZ(0.0f);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT:
			setGyroY_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT:
			setGyroX_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT:
			setGyroZ_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY:
			setAngleRate_Latency(0.0f);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y:
			return gyroY != 0.0f;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X:
			return gyroX != 0.0f;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z:
			return gyroZ != 0.0f;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT:
			return gyroY_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT:
			return gyroX_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT:
			return gyroZ_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY:
			return angleRate_Latency != 0.0f;
		}
		return super.eIsSet(featureID);
	}
} //ACEINNA_ANGLERATE_T_R
