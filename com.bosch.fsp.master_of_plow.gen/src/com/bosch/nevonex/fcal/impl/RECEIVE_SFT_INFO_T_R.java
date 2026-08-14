/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class RECEIVE_SFT_INFO_T_R extends EObjectImpl implements IRECEIVE_SFT_INFO_T_R {
	protected float tRZ_SFT_SIG1_V = 0.0f;
	protected float tRZ_SFT_SIG2_V = 0.0f;
	protected int tRZ_SFT_DIAG = 0;
	protected int tRZ_SFT_AUTO = 0;
	protected int tRZ_SFT_STATE = 0;

	protected RECEIVE_SFT_INFO_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.RECEIVE_SFT_INFO_T_R;
	}

	public float getTRZ_SFT_SIG1_V() {
		return tRZ_SFT_SIG1_V;
	}

	public void setTRZ_SFT_SIG1_V(float newTRZ_SFT_SIG1_V) {
		tRZ_SFT_SIG1_V = newTRZ_SFT_SIG1_V;
	}

	public float getTRZ_SFT_SIG2_V() {
		return tRZ_SFT_SIG2_V;
	}

	public void setTRZ_SFT_SIG2_V(float newTRZ_SFT_SIG2_V) {
		tRZ_SFT_SIG2_V = newTRZ_SFT_SIG2_V;
	}

	public int getTRZ_SFT_DIAG() {
		return tRZ_SFT_DIAG;
	}

	public void setTRZ_SFT_DIAG(int newTRZ_SFT_DIAG) {
		tRZ_SFT_DIAG = newTRZ_SFT_DIAG;
	}

	public int getTRZ_SFT_AUTO() {
		return tRZ_SFT_AUTO;
	}

	public void setTRZ_SFT_AUTO(int newTRZ_SFT_AUTO) {
		tRZ_SFT_AUTO = newTRZ_SFT_AUTO;
	}

	public int getTRZ_SFT_STATE() {
		return tRZ_SFT_STATE;
	}

	public void setTRZ_SFT_STATE(int newTRZ_SFT_STATE) {
		tRZ_SFT_STATE = newTRZ_SFT_STATE;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V:
			return getTRZ_SFT_SIG1_V();
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V:
			return getTRZ_SFT_SIG2_V();
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG:
			return getTRZ_SFT_DIAG();
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO:
			return getTRZ_SFT_AUTO();
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE:
			return getTRZ_SFT_STATE();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V:
			setTRZ_SFT_SIG1_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V:
			setTRZ_SFT_SIG2_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG:
			setTRZ_SFT_DIAG((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO:
			setTRZ_SFT_AUTO((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE:
			setTRZ_SFT_STATE((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V:
			setTRZ_SFT_SIG1_V(0.0f);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V:
			setTRZ_SFT_SIG2_V(0.0f);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG:
			setTRZ_SFT_DIAG(0);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO:
			setTRZ_SFT_AUTO(0);
			return;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE:
			setTRZ_SFT_STATE(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V:
			return tRZ_SFT_SIG1_V != 0.0f;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V:
			return tRZ_SFT_SIG2_V != 0.0f;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG:
			return tRZ_SFT_DIAG != 0;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO:
			return tRZ_SFT_AUTO != 0;
		case FcalPackage.RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE:
			return tRZ_SFT_STATE != 0;
		}
		return super.eIsSet(featureID);
	}
} //RECEIVE_SFT_INFO_T_R
