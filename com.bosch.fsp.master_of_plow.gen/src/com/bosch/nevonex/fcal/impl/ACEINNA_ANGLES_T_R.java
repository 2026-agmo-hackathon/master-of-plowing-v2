/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class ACEINNA_ANGLES_T_R extends EObjectImpl implements IACEINNA_ANGLES_T_R {
	protected float pitch = 0.0f;
	protected float roll = 0.0f;
	protected int pitch_Compensation = 0;
	protected int pitch_FigureOfMerit = 0;
	protected int roll_Compensation = 0;
	protected int roll_FigureOfMerit = 0;
	protected float pitchRoll_Latency = 0.0f;

	protected ACEINNA_ANGLES_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.ACEINNA_ANGLES_T_R;
	}

	public float getPitch() {
		return pitch;
	}

	public void setPitch(float newPitch) {
		pitch = newPitch;
	}

	public float getRoll() {
		return roll;
	}

	public void setRoll(float newRoll) {
		roll = newRoll;
	}

	public int getPitch_Compensation() {
		return pitch_Compensation;
	}

	public void setPitch_Compensation(int newPitch_Compensation) {
		pitch_Compensation = newPitch_Compensation;
	}

	public int getPitch_FigureOfMerit() {
		return pitch_FigureOfMerit;
	}

	public void setPitch_FigureOfMerit(int newPitch_FigureOfMerit) {
		pitch_FigureOfMerit = newPitch_FigureOfMerit;
	}

	public int getRoll_Compensation() {
		return roll_Compensation;
	}

	public void setRoll_Compensation(int newRoll_Compensation) {
		roll_Compensation = newRoll_Compensation;
	}

	public int getRoll_FigureOfMerit() {
		return roll_FigureOfMerit;
	}

	public void setRoll_FigureOfMerit(int newRoll_FigureOfMerit) {
		roll_FigureOfMerit = newRoll_FigureOfMerit;
	}

	public float getPitchRoll_Latency() {
		return pitchRoll_Latency;
	}

	public void setPitchRoll_Latency(float newPitchRoll_Latency) {
		pitchRoll_Latency = newPitchRoll_Latency;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH:
			return getPitch();
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL:
			return getRoll();
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
			return getPitch_Compensation();
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT:
			return getPitch_FigureOfMerit();
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
			return getRoll_Compensation();
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT:
			return getRoll_FigureOfMerit();
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY:
			return getPitchRoll_Latency();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH:
			setPitch((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL:
			setRoll((Float) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
			setPitch_Compensation((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT:
			setPitch_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
			setRoll_Compensation((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT:
			setRoll_FigureOfMerit((Integer) newValue);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY:
			setPitchRoll_Latency((Float) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH:
			setPitch(0.0f);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL:
			setRoll(0.0f);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
			setPitch_Compensation(0);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT:
			setPitch_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
			setRoll_Compensation(0);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT:
			setRoll_FigureOfMerit(0);
			return;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY:
			setPitchRoll_Latency(0.0f);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH:
			return pitch != 0.0f;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL:
			return roll != 0.0f;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_COMPENSATION:
			return pitch_Compensation != 0;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT:
			return pitch_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_COMPENSATION:
			return roll_Compensation != 0;
		case FcalPackage.ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT:
			return roll_FigureOfMerit != 0;
		case FcalPackage.ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY:
			return pitchRoll_Latency != 0.0f;
		}
		return super.eIsSet(featureID);
	}
} //ACEINNA_ANGLES_T_R
