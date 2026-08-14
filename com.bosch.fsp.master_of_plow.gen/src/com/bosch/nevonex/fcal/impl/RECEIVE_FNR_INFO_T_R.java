/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class RECEIVE_FNR_INFO_T_R extends EObjectImpl implements IRECEIVE_FNR_INFO_T_R {
	protected float tRZ_FNR_SIG1_V = 0.0f;
	protected float tRZ_FNR_SIG2_V = 0.0f;
	protected int tRZ_FNR_DIAG = 0;
	protected int tRZ_FNR_AUTO = 0;
	protected int tRZ_FNR_STATE = 0;

	protected RECEIVE_FNR_INFO_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.RECEIVE_FNR_INFO_T_R;
	}

	public float getTRZ_FNR_SIG1_V() {
		return tRZ_FNR_SIG1_V;
	}

	public void setTRZ_FNR_SIG1_V(float newTRZ_FNR_SIG1_V) {
		tRZ_FNR_SIG1_V = newTRZ_FNR_SIG1_V;
	}

	public float getTRZ_FNR_SIG2_V() {
		return tRZ_FNR_SIG2_V;
	}

	public void setTRZ_FNR_SIG2_V(float newTRZ_FNR_SIG2_V) {
		tRZ_FNR_SIG2_V = newTRZ_FNR_SIG2_V;
	}

	public int getTRZ_FNR_DIAG() {
		return tRZ_FNR_DIAG;
	}

	public void setTRZ_FNR_DIAG(int newTRZ_FNR_DIAG) {
		tRZ_FNR_DIAG = newTRZ_FNR_DIAG;
	}

	public int getTRZ_FNR_AUTO() {
		return tRZ_FNR_AUTO;
	}

	public void setTRZ_FNR_AUTO(int newTRZ_FNR_AUTO) {
		tRZ_FNR_AUTO = newTRZ_FNR_AUTO;
	}

	public int getTRZ_FNR_STATE() {
		return tRZ_FNR_STATE;
	}

	public void setTRZ_FNR_STATE(int newTRZ_FNR_STATE) {
		tRZ_FNR_STATE = newTRZ_FNR_STATE;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V:
			return getTRZ_FNR_SIG1_V();
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V:
			return getTRZ_FNR_SIG2_V();
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG:
			return getTRZ_FNR_DIAG();
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO:
			return getTRZ_FNR_AUTO();
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE:
			return getTRZ_FNR_STATE();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V:
			setTRZ_FNR_SIG1_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V:
			setTRZ_FNR_SIG2_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG:
			setTRZ_FNR_DIAG((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO:
			setTRZ_FNR_AUTO((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE:
			setTRZ_FNR_STATE((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V:
			setTRZ_FNR_SIG1_V(0.0f);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V:
			setTRZ_FNR_SIG2_V(0.0f);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG:
			setTRZ_FNR_DIAG(0);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO:
			setTRZ_FNR_AUTO(0);
			return;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE:
			setTRZ_FNR_STATE(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V:
			return tRZ_FNR_SIG1_V != 0.0f;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V:
			return tRZ_FNR_SIG2_V != 0.0f;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG:
			return tRZ_FNR_DIAG != 0;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO:
			return tRZ_FNR_AUTO != 0;
		case FcalPackage.RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE:
			return tRZ_FNR_STATE != 0;
		}
		return super.eIsSet(featureID);
	}
} //RECEIVE_FNR_INFO_T_R
