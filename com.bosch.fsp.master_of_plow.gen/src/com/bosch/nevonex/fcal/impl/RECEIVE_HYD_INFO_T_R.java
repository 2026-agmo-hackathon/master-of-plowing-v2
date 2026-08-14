/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class RECEIVE_HYD_INFO_T_R extends EObjectImpl implements IRECEIVE_HYD_INFO_T_R {
	protected float tRZ_HYD_SIG1_V = 0.0f;
	protected float tRZ_HYD_SIG2_V = 0.0f;
	protected int tRZ_HYD_DIAG = 0;
	protected int tRZ_HYD_AUTO = 0;

	protected RECEIVE_HYD_INFO_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.RECEIVE_HYD_INFO_T_R;
	}

	public float getTRZ_HYD_SIG1_V() {
		return tRZ_HYD_SIG1_V;
	}

	public void setTRZ_HYD_SIG1_V(float newTRZ_HYD_SIG1_V) {
		tRZ_HYD_SIG1_V = newTRZ_HYD_SIG1_V;
	}

	public float getTRZ_HYD_SIG2_V() {
		return tRZ_HYD_SIG2_V;
	}

	public void setTRZ_HYD_SIG2_V(float newTRZ_HYD_SIG2_V) {
		tRZ_HYD_SIG2_V = newTRZ_HYD_SIG2_V;
	}

	public int getTRZ_HYD_DIAG() {
		return tRZ_HYD_DIAG;
	}

	public void setTRZ_HYD_DIAG(int newTRZ_HYD_DIAG) {
		tRZ_HYD_DIAG = newTRZ_HYD_DIAG;
	}

	public int getTRZ_HYD_AUTO() {
		return tRZ_HYD_AUTO;
	}

	public void setTRZ_HYD_AUTO(int newTRZ_HYD_AUTO) {
		tRZ_HYD_AUTO = newTRZ_HYD_AUTO;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V:
			return getTRZ_HYD_SIG1_V();
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V:
			return getTRZ_HYD_SIG2_V();
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG:
			return getTRZ_HYD_DIAG();
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO:
			return getTRZ_HYD_AUTO();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V:
			setTRZ_HYD_SIG1_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V:
			setTRZ_HYD_SIG2_V((Float) newValue);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG:
			setTRZ_HYD_DIAG((Integer) newValue);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO:
			setTRZ_HYD_AUTO((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V:
			setTRZ_HYD_SIG1_V(0.0f);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V:
			setTRZ_HYD_SIG2_V(0.0f);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG:
			setTRZ_HYD_DIAG(0);
			return;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO:
			setTRZ_HYD_AUTO(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V:
			return tRZ_HYD_SIG1_V != 0.0f;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V:
			return tRZ_HYD_SIG2_V != 0.0f;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG:
			return tRZ_HYD_DIAG != 0;
		case FcalPackage.RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO:
			return tRZ_HYD_AUTO != 0;
		}
		return super.eIsSet(featureID);
	}
} //RECEIVE_HYD_INFO_T_R
