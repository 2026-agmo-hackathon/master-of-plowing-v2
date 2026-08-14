/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IFcalFactory;
import com.bosch.nevonex.fcal.IBulkProcessor;
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

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class FcalFactory extends EFactoryImpl implements IFcalFactory {
	public static final FcalFactory eINSTANCE = init();

	public static FcalFactory init() {
		try {
			FcalFactory theFcalFactory = (FcalFactory) EPackage.Registry.INSTANCE.getEFactory(FcalPackage.eNS_URI);
			if (theFcalFactory != null) {
				return theFcalFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new FcalFactory();
	}

	public FcalFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case FcalPackage.BULK_PROCESSOR:
			return createBulkProcessor();
		case FcalPackage.RECEIVE_ACC_INFO_T_R:
			return createRECEIVE_ACC_INFO_T_R();
		case FcalPackage.RECEIVE_FNR_INFO_T_R:
			return createRECEIVE_FNR_INFO_T_R();
		case FcalPackage.RECEIVE_HYD_INFO_T_R:
			return createRECEIVE_HYD_INFO_T_R();
		case FcalPackage.RECEIVE_SFT_INFO_T_R:
			return createRECEIVE_SFT_INFO_T_R();
		case FcalPackage.AGMO_GENERIC_SEND_CMD_T_W:
			return createAGMO_GENERIC_SEND_CMD_T_W();
		case FcalPackage.ACEINNA_ACCEL_T_R:
			return createACEINNA_ACCEL_T_R();
		case FcalPackage.ACEINNA_ANGLERATE_T_R:
			return createACEINNA_ANGLERATE_T_R();
		case FcalPackage.ACEINNA_ANGLES_T_R:
			return createACEINNA_ANGLES_T_R();
		case FcalPackage.MOTOR_HEARTBEAT_T_R:
			return createMOTOR_HEARTBEAT_T_R();
		case FcalPackage.RAW_CAN_DATA_DLC_8_W:
			return createRAW_CAN_DATA_DLC_8_W();
		case FcalPackage.GPS_NMEA0183_T_R:
			return createGPS_NMEA0183_T_R();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IBulkProcessor createBulkProcessor() {
		BulkProcessor bulkProcessor = new BulkProcessor();
		return bulkProcessor;
	}

	public IRECEIVE_ACC_INFO_T_R createRECEIVE_ACC_INFO_T_R() {
		RECEIVE_ACC_INFO_T_R rECEIVE_ACC_INFO_T_R = new RECEIVE_ACC_INFO_T_R();
		return rECEIVE_ACC_INFO_T_R;
	}

	public IRECEIVE_FNR_INFO_T_R createRECEIVE_FNR_INFO_T_R() {
		RECEIVE_FNR_INFO_T_R rECEIVE_FNR_INFO_T_R = new RECEIVE_FNR_INFO_T_R();
		return rECEIVE_FNR_INFO_T_R;
	}

	public IRECEIVE_HYD_INFO_T_R createRECEIVE_HYD_INFO_T_R() {
		RECEIVE_HYD_INFO_T_R rECEIVE_HYD_INFO_T_R = new RECEIVE_HYD_INFO_T_R();
		return rECEIVE_HYD_INFO_T_R;
	}

	public IRECEIVE_SFT_INFO_T_R createRECEIVE_SFT_INFO_T_R() {
		RECEIVE_SFT_INFO_T_R rECEIVE_SFT_INFO_T_R = new RECEIVE_SFT_INFO_T_R();
		return rECEIVE_SFT_INFO_T_R;
	}

	public IAGMO_GENERIC_SEND_CMD_T_W createAGMO_GENERIC_SEND_CMD_T_W() {
		AGMO_GENERIC_SEND_CMD_T_W aGMO_GENERIC_SEND_CMD_T_W = new AGMO_GENERIC_SEND_CMD_T_W();
		return aGMO_GENERIC_SEND_CMD_T_W;
	}

	public IACEINNA_ACCEL_T_R createACEINNA_ACCEL_T_R() {
		ACEINNA_ACCEL_T_R aCEINNA_ACCEL_T_R = new ACEINNA_ACCEL_T_R();
		return aCEINNA_ACCEL_T_R;
	}

	public IACEINNA_ANGLERATE_T_R createACEINNA_ANGLERATE_T_R() {
		ACEINNA_ANGLERATE_T_R aCEINNA_ANGLERATE_T_R = new ACEINNA_ANGLERATE_T_R();
		return aCEINNA_ANGLERATE_T_R;
	}

	public IACEINNA_ANGLES_T_R createACEINNA_ANGLES_T_R() {
		ACEINNA_ANGLES_T_R aCEINNA_ANGLES_T_R = new ACEINNA_ANGLES_T_R();
		return aCEINNA_ANGLES_T_R;
	}

	public IMOTOR_HEARTBEAT_T_R createMOTOR_HEARTBEAT_T_R() {
		MOTOR_HEARTBEAT_T_R mOTOR_HEARTBEAT_T_R = new MOTOR_HEARTBEAT_T_R();
		return mOTOR_HEARTBEAT_T_R;
	}

	public IRAW_CAN_DATA_DLC_8_W createRAW_CAN_DATA_DLC_8_W() {
		RAW_CAN_DATA_DLC_8_W rAW_CAN_DATA_DLC_8_W = new RAW_CAN_DATA_DLC_8_W();
		return rAW_CAN_DATA_DLC_8_W;
	}

	public IGPS_NMEA0183_T_R createGPS_NMEA0183_T_R() {
		GPS_NMEA0183_T_R gPS_NMEA0183_T_R = new GPS_NMEA0183_T_R();
		return gPS_NMEA0183_T_R;
	}

	public FcalPackage getFcalPackage() {
		return (FcalPackage) getEPackage();
	}

	@Deprecated
	public static FcalPackage getPackage() {
		return FcalPackage.eINSTANCE;
	}

} //FcalFactory
