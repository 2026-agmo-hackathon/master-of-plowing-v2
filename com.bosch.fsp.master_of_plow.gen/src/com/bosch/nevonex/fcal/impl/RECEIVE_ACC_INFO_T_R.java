/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class RECEIVE_ACC_INFO_T_R extends EObjectImpl implements IRECEIVE_ACC_INFO_T_R {
	protected float tRZ_ACC_SIG1_V = 0.0f;
	protected float tRZ_ACC_SIG2_V = 0.0f;
	protected int tRZ_ACC_DIAG = 0;
	protected int tRZ_ACC_AUTO = 0;

	protected RECEIVE_ACC_INFO_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.RECEIVE_ACC_INFO_T_R;
	}

	public float getTRZ_ACC_SIG1_V() {
		return tRZ_ACC_SIG1_V;
	}

	public void setTRZ_ACC_SIG1_V(float newTRZ_ACC_SIG1_V) {
		tRZ_ACC_SIG1_V = newTRZ_ACC_SIG1_V;
	}

	public float getTRZ_ACC_SIG2_V() {
		return tRZ_ACC_SIG2_V;
	}

	public void setTRZ_ACC_SIG2_V(float newTRZ_ACC_SIG2_V) {
		tRZ_ACC_SIG2_V = newTRZ_ACC_SIG2_V;
	}

	public int getTRZ_ACC_DIAG() {
		return tRZ_ACC_DIAG;
	}

	public void setTRZ_ACC_DIAG(int newTRZ_ACC_DIAG) {
		tRZ_ACC_DIAG = newTRZ_ACC_DIAG;
	}

	public int getTRZ_ACC_AUTO() {
		return tRZ_ACC_AUTO;
	}

	public void setTRZ_ACC_AUTO(int newTRZ_ACC_AUTO) {
		tRZ_ACC_AUTO = newTRZ_ACC_AUTO;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
			return getTRZ_ACC_SIG1_V();
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
			return getTRZ_ACC_SIG2_V();
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
			return getTRZ_ACC_DIAG();
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
			return getTRZ_ACC_AUTO();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
			setTRZ_ACC_SIG1_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
			setTRZ_ACC_SIG2_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
			setTRZ_ACC_DIAG((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
			setTRZ_ACC_AUTO((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
			setTRZ_ACC_SIG1_V(0.0f);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
			setTRZ_ACC_SIG2_V(0.0f);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
			setTRZ_ACC_DIAG(0);
			return;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
			setTRZ_ACC_AUTO(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V:
			return tRZ_ACC_SIG1_V != 0.0f;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V:
			return tRZ_ACC_SIG2_V != 0.0f;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG:
			return tRZ_ACC_DIAG != 0;
		case FcalPackage.RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO:
			return tRZ_ACC_AUTO != 0;
		}
		return super.eIsSet(featureID);
	}
} //RECEIVE_ACC_INFO_T_R
