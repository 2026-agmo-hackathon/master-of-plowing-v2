/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class ACEINNA_ACCEL_T_R extends EObjectImpl implements IACEINNA_ACCEL_T_R {
	protected float accY = 0.0f;
	protected float accX = 0.0f;
	protected float accZ = 0.0f;
	protected int lateralAcc_FigureOfMerit = 0;
	protected int longiAcc_FigureOfMerit = 0;
	protected int verticAcc_FigureOfMerit = 0;
	protected int support_Rate_Acc = 0;

	protected ACEINNA_ACCEL_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.ACEINNA_ACCEL_T_R;
	}

	public float getAccY() {
		return accY;
	}

	public void setAccY(float newAccY) {
		accY = newAccY;
	}

	public float getAccX() {
		return accX;
	}

	public void setAccX(float newAccX) {
		accX = newAccX;
	}

	public float getAccZ() {
		return accZ;
	}

	public void setAccZ(float newAccZ) {
		accZ = newAccZ;
	}

	public int getLateralAcc_FigureOfMerit() {
		return lateralAcc_FigureOfMerit;
	}

	public void setLateralAcc_FigureOfMerit(int newLateralAcc_FigureOfMerit) {
		lateralAcc_FigureOfMerit = newLateralAcc_FigureOfMerit;
	}

	public int getLongiAcc_FigureOfMerit() {
		return longiAcc_FigureOfMerit;
	}

	public void setLongiAcc_FigureOfMerit(int newLongiAcc_FigureOfMerit) {
		longiAcc_FigureOfMerit = newLongiAcc_FigureOfMerit;
	}

	public int getVerticAcc_FigureOfMerit() {
		return verticAcc_FigureOfMerit;
	}

	public void setVerticAcc_FigureOfMerit(int newVerticAcc_FigureOfMerit) {
		verticAcc_FigureOfMerit = newVerticAcc_FigureOfMerit;
	}

	public int getSupport_Rate_Acc() {
		return support_Rate_Acc;
	}

	public void setSupport_Rate_Acc(int newSupport_Rate_Acc) {
		support_Rate_Acc = newSupport_Rate_Acc;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Y:
			return getAccY();
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_X:
			return getAccX();
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Z:
			return getAccZ();
		case FcalPackage.ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT:
			return getLateralAcc_FigureOfMerit();
		case FcalPackage.ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT:
			return getLongiAcc_FigureOfMerit();
		case FcalPackage.ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT:
			return getVerticAcc_FigureOfMerit();
		case FcalPackage.ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
			return getSupport_Rate_Acc();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Y:
			setAccY((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_X:
			setAccX((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Z:
			setAccZ((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT:
			setLateralAcc_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT:
			setLongiAcc_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT:
			setVerticAcc_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
			setSupport_Rate_Acc((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Y:
			setAccY(0.0f);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_X:
			setAccX(0.0f);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Z:
			setAccZ(0.0f);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT:
			setLateralAcc_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT:
			setLongiAcc_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT:
			setVerticAcc_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
			setSupport_Rate_Acc(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Y:
			return accY != 0.0f;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_X:
			return accX != 0.0f;
		case FcalPackage.ACEINNA_ACCEL_T_R__ACC_Z:
			return accZ != 0.0f;
		case FcalPackage.ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT:
			return lateralAcc_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT:
			return longiAcc_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT:
			return verticAcc_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC:
			return support_Rate_Acc != 0;
		}
		return super.eIsSet(featureID);
	}
} //ACEINNA_ACCEL_T_R
