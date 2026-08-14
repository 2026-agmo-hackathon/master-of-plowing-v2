/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class AGMO_GENERIC_SEND_CMD_T_W extends EObjectImpl implements IAGMO_GENERIC_SEND_CMD_T_W {
	protected int aD_Generic_Cmd = 0;
	protected int aD_Generic_Mode_Cmd = 0;

	protected AGMO_GENERIC_SEND_CMD_T_W() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.AGMO_GENERIC_SEND_CMD_T_W;
	}

	public int getAD_Generic_Cmd() {
		return aD_Generic_Cmd;
	}

	public void setAD_Generic_Cmd(int newAD_Generic_Cmd) {
		aD_Generic_Cmd = newAD_Generic_Cmd;
	}

	public int getAD_Generic_Mode_Cmd() {
		return aD_Generic_Mode_Cmd;
	}

	public void setAD_Generic_Mode_Cmd(int newAD_Generic_Mode_Cmd) {
		aD_Generic_Mode_Cmd = newAD_Generic_Mode_Cmd;
	}

	/**
	 * @generated
	 */
	@Override
	public void initialize(int aD_Generic_Cmd, int aD_Generic_Mode_Cmd) {
		this.aD_Generic_Cmd = aD_Generic_Cmd;
		this.aD_Generic_Mode_Cmd = aD_Generic_Mode_Cmd;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
			return getAD_Generic_Cmd();
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
			return getAD_Generic_Mode_Cmd();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
			setAD_Generic_Cmd((Integer) newValue);
			return;
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
			setAD_Generic_Mode_Cmd((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
			setAD_Generic_Cmd(0);
			return;
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
			setAD_Generic_Mode_Cmd(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD:
			return aD_Generic_Cmd != 0;
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD:
			return aD_Generic_Mode_Cmd != 0;
		}
		return super.eIsSet(featureID);
	}
} //AGMO_GENERIC_SEND_CMD_T_W
