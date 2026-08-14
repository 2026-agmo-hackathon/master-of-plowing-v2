/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class RAW_CAN_DATA_DLC_8_W extends EObjectImpl implements IRAW_CAN_DATA_DLC_8_W {
	protected int byte0 = 0;
	protected int byte1 = 0;
	protected int byte2 = 0;
	protected int byte3 = 0;
	protected int byte4 = 0;
	protected int byte5 = 0;
	protected int byte6 = 0;
	protected int byte7 = 0;

	protected RAW_CAN_DATA_DLC_8_W() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.RAW_CAN_DATA_DLC_8_W;
	}

	public int getByte0() {
		return byte0;
	}

	public void setByte0(int newByte0) {
		byte0 = newByte0;
	}

	public int getByte1() {
		return byte1;
	}

	public void setByte1(int newByte1) {
		byte1 = newByte1;
	}

	public int getByte2() {
		return byte2;
	}

	public void setByte2(int newByte2) {
		byte2 = newByte2;
	}

	public int getByte3() {
		return byte3;
	}

	public void setByte3(int newByte3) {
		byte3 = newByte3;
	}

	public int getByte4() {
		return byte4;
	}

	public void setByte4(int newByte4) {
		byte4 = newByte4;
	}

	public int getByte5() {
		return byte5;
	}

	public void setByte5(int newByte5) {
		byte5 = newByte5;
	}

	public int getByte6() {
		return byte6;
	}

	public void setByte6(int newByte6) {
		byte6 = newByte6;
	}

	public int getByte7() {
		return byte7;
	}

	public void setByte7(int newByte7) {
		byte7 = newByte7;
	}

	/**
	 * @generated
	 */
	@Override
	public void initialize(int byte0, int byte1, int byte2, int byte3, int byte4, int byte5, int byte6, int byte7) {
		this.byte0 = byte0;
		this.byte1 = byte1;
		this.byte2 = byte2;
		this.byte3 = byte3;
		this.byte4 = byte4;
		this.byte5 = byte5;
		this.byte6 = byte6;
		this.byte7 = byte7;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE0:
			return getByte0();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE1:
			return getByte1();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE2:
			return getByte2();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE3:
			return getByte3();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE4:
			return getByte4();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE5:
			return getByte5();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE6:
			return getByte6();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE7:
			return getByte7();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE0:
			setByte0((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE1:
			setByte1((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE2:
			setByte2((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE3:
			setByte3((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE4:
			setByte4((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE5:
			setByte5((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE6:
			setByte6((Integer) newValue);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE7:
			setByte7((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE0:
			setByte0(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE1:
			setByte1(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE2:
			setByte2(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE3:
			setByte3(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE4:
			setByte4(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE5:
			setByte5(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE6:
			setByte6(0);
			return;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE7:
			setByte7(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE0:
			return byte0 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE1:
			return byte1 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE2:
			return byte2 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE3:
			return byte3 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE4:
			return byte4 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE5:
			return byte5 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE6:
			return byte6 != 0;
		case FcalPackage.RAW_CAN_DATA_DLC_8_W__BYTE7:
			return byte7 != 0;
		}
		return super.eIsSet(featureID);
	}
} //RAW_CAN_DATA_DLC_8_W
