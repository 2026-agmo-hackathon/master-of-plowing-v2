/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.util;

import com.bosch.nevonex.fcal.IBulkProcessor;
import com.bosch.nevonex.fcal.IFcalFactory;
import com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R;
import com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W;
import com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R;
import com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R;
import com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W;
import com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R;

import com.bosch.nevonex.fcal.impl.FcalPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public class FcalSwitch<T> extends Switch<T> {
	protected static FcalPackage modelPackage;

	public FcalSwitch() {
		if (modelPackage == null) {
			modelPackage = FcalPackage.eINSTANCE;
		}
	}

	@Override
	protected boolean isSwitchFor(EPackage ePackage) {
		return ePackage == modelPackage;
	}

	@Override
	protected T doSwitch(int classifierID, EObject theEObject) {
		if (classifierID < 80) return doSwitch_chunk0(classifierID, theEObject);
		return defaultCase(theEObject);
	}

	private T doSwitch_chunk0(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case FcalPackage.I_BULK_PROCESSOR: {
				IBulkProcessor iBulkProcessor = (IBulkProcessor) theEObject;
				T result = caseIBulkProcessor(iBulkProcessor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.BULK_PROCESSOR: {
				IBulkProcessor bulkProcessor = (IBulkProcessor) theEObject;
				T result = caseBulkProcessor(bulkProcessor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.I_FCAL_FACTORY: {
				IFcalFactory iFcalFactory = (IFcalFactory) theEObject;
				T result = caseIFcalFactory(iFcalFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.RECEIVE_ACC_INFO_T_R: {
				IRECEIVE_ACC_INFO_T_R rECEIVE_ACC_INFO_T_R = (IRECEIVE_ACC_INFO_T_R) theEObject;
				T result = caseRECEIVE_ACC_INFO_T_R(rECEIVE_ACC_INFO_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) rECEIVE_ACC_INFO_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.RECEIVE_FNR_INFO_T_R: {
				IRECEIVE_FNR_INFO_T_R rECEIVE_FNR_INFO_T_R = (IRECEIVE_FNR_INFO_T_R) theEObject;
				T result = caseRECEIVE_FNR_INFO_T_R(rECEIVE_FNR_INFO_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) rECEIVE_FNR_INFO_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.RECEIVE_HYD_INFO_T_R: {
				IRECEIVE_HYD_INFO_T_R rECEIVE_HYD_INFO_T_R = (IRECEIVE_HYD_INFO_T_R) theEObject;
				T result = caseRECEIVE_HYD_INFO_T_R(rECEIVE_HYD_INFO_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) rECEIVE_HYD_INFO_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.RECEIVE_SFT_INFO_T_R: {
				IRECEIVE_SFT_INFO_T_R rECEIVE_SFT_INFO_T_R = (IRECEIVE_SFT_INFO_T_R) theEObject;
				T result = caseRECEIVE_SFT_INFO_T_R(rECEIVE_SFT_INFO_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) rECEIVE_SFT_INFO_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W: {
				IAGMO_GENERIC_SEND_CMD_T_W aGMO_GENERIC_SEND_CMD_T_W = (IAGMO_GENERIC_SEND_CMD_T_W) theEObject;
				T result = caseAGMO_GENERIC_SEND_CMD_T_W(aGMO_GENERIC_SEND_CMD_T_W);
				if (result == null)
					result = caseIArrayType((IArrayType) aGMO_GENERIC_SEND_CMD_T_W);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.ACEINNA_ACCEL_T_R: {
				IACEINNA_ACCEL_T_R aCEINNA_ACCEL_T_R = (IACEINNA_ACCEL_T_R) theEObject;
				T result = caseACEINNA_ACCEL_T_R(aCEINNA_ACCEL_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) aCEINNA_ACCEL_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.ACEINNA_ANGLERATE_T_R: {
				IACEINNA_ANGLERATE_T_R aCEINNA_ANGLERATE_T_R = (IACEINNA_ANGLERATE_T_R) theEObject;
				T result = caseACEINNA_ANGLERATE_T_R(aCEINNA_ANGLERATE_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) aCEINNA_ANGLERATE_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.ACEINNA_ANGLES_T_R: {
				IACEINNA_ANGLES_T_R aCEINNA_ANGLES_T_R = (IACEINNA_ANGLES_T_R) theEObject;
				T result = caseACEINNA_ANGLES_T_R(aCEINNA_ANGLES_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) aCEINNA_ANGLES_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.MOTOR_HEARTBEAT_T_R: {
				IMOTOR_HEARTBEAT_T_R mOTOR_HEARTBEAT_T_R = (IMOTOR_HEARTBEAT_T_R) theEObject;
				T result = caseMOTOR_HEARTBEAT_T_R(mOTOR_HEARTBEAT_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) mOTOR_HEARTBEAT_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.RAW_CAN_DATA_DLC_8_W: {
				IRAW_CAN_DATA_DLC_8_W rAW_CAN_DATA_DLC_8_W = (IRAW_CAN_DATA_DLC_8_W) theEObject;
				T result = caseRAW_CAN_DATA_DLC_8_W(rAW_CAN_DATA_DLC_8_W);
				if (result == null)
					result = caseIArrayType((IArrayType) rAW_CAN_DATA_DLC_8_W);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcalPackage.GPS_NMEA0183_T_R: {
				IGPS_NMEA0183_T_R gPS_NMEA0183_T_R = (IGPS_NMEA0183_T_R) theEObject;
				T result = caseGPS_NMEA0183_T_R(gPS_NMEA0183_T_R);
				if (result == null)
					result = caseIArrayType((IArrayType) gPS_NMEA0183_T_R);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIBulkProcessor(IBulkProcessor object) {
		return null;
	}

	public T caseBulkProcessor(IBulkProcessor object) {
		return null;
	}

	public T caseIFcalFactory(IFcalFactory object) {
		return null;
	}

	public T caseRECEIVE_ACC_INFO_T_R(IRECEIVE_ACC_INFO_T_R object) {
		return null;
	}

	public T caseRECEIVE_FNR_INFO_T_R(IRECEIVE_FNR_INFO_T_R object) {
		return null;
	}

	public T caseRECEIVE_HYD_INFO_T_R(IRECEIVE_HYD_INFO_T_R object) {
		return null;
	}

	public T caseRECEIVE_SFT_INFO_T_R(IRECEIVE_SFT_INFO_T_R object) {
		return null;
	}

	public T caseAGMO_GENERIC_SEND_CMD_T_W(IAGMO_GENERIC_SEND_CMD_T_W object) {
		return null;
	}

	public T caseACEINNA_ACCEL_T_R(IACEINNA_ACCEL_T_R object) {
		return null;
	}

	public T caseACEINNA_ANGLERATE_T_R(IACEINNA_ANGLERATE_T_R object) {
		return null;
	}

	public T caseACEINNA_ANGLES_T_R(IACEINNA_ANGLES_T_R object) {
		return null;
	}

	public T caseMOTOR_HEARTBEAT_T_R(IMOTOR_HEARTBEAT_T_R object) {
		return null;
	}

	public T caseRAW_CAN_DATA_DLC_8_W(IRAW_CAN_DATA_DLC_8_W object) {
		return null;
	}

	public T caseGPS_NMEA0183_T_R(IGPS_NMEA0183_T_R object) {
		return null;
	}

	public T caseIArrayType(IArrayType object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //FcalSwitch
