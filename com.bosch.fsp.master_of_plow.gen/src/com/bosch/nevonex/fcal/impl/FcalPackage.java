/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

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
import com.bosch.nevonex.fcal.impl.FcalFactory;

import com.bosch.nevonex.types.impl.TypesPackage;

import org.eclipse.emf.ecore.EAttribute;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EFactory;
import org.eclipse.emf.ecore.EOperation;
import org.eclipse.emf.ecore.EPackage;
import org.eclipse.emf.ecore.EParameter;
import org.eclipse.emf.ecore.EReference;
import org.eclipse.emf.ecore.EcoreFactory;
import org.eclipse.emf.ecore.EcorePackage;

import org.eclipse.emf.ecore.impl.EPackageImpl;

/**
 * @generated
 */
public class FcalPackage extends EPackageImpl {
	public static final String eNAME = "fcal";
	public static final String eNS_URI = "com.bosch.nevonex.fcal";
	public static final String eNS_PREFIX = "fcal";
	public static final FcalPackage eINSTANCE = com.bosch.nevonex.fcal.impl.FcalPackage.init();

	public static final int I_BULK_PROCESSOR = 0;
	public static final int I_BULK_PROCESSOR_FEATURE_COUNT = 0;
	public static final int I_BULK_PROCESSOR___EXECUTE_BULK_OPERATIONS = 0;
	public static final int I_BULK_PROCESSOR_OPERATION_COUNT = 1;

	public static final int BULK_PROCESSOR = 1;
	public static final int BULK_PROCESSOR_FEATURE_COUNT = 0;
	public static final int BULK_PROCESSOR_OPERATION_COUNT = 0;

	public static final int I_FCAL_FACTORY = 2;
	public static final int I_FCAL_FACTORY_FEATURE_COUNT = 0;
	public static final int I_FCAL_FACTORY_OPERATION_COUNT = 0;

	public static final int RECEIVE_ACC_INFO_T_R = 3;
	public static final int RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int RECEIVE_ACC_INFO_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int RECEIVE_ACC_INFO_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int RECEIVE_FNR_INFO_T_R = 4;
	public static final int RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int RECEIVE_FNR_INFO_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int RECEIVE_FNR_INFO_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int RECEIVE_HYD_INFO_T_R = 5;
	public static final int RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int RECEIVE_HYD_INFO_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int RECEIVE_HYD_INFO_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int RECEIVE_SFT_INFO_T_R = 6;
	public static final int RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int RECEIVE_SFT_INFO_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int RECEIVE_SFT_INFO_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int AGMO_GENERIC_SEND_CMD_T_W = 7;
	public static final int AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int AGMO_GENERIC_SEND_CMD_T_W_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int AGMO_GENERIC_SEND_CMD_T_W___INITIALIZE__INT__INT = 0;
	public static final int AGMO_GENERIC_SEND_CMD_T_W_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 1;

	public static final int ACEINNA_ACCEL_T_R = 8;
	public static final int ACEINNA_ACCEL_T_R__ACC_Y = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int ACEINNA_ACCEL_T_R__ACC_X = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int ACEINNA_ACCEL_T_R__ACC_Z = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int ACEINNA_ACCEL_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int ACEINNA_ACCEL_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int ACEINNA_ANGLERATE_T_R = 9;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_Y = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_X = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_Z = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int ACEINNA_ANGLERATE_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int ACEINNA_ANGLERATE_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int ACEINNA_ANGLES_T_R = 10;
	public static final int ACEINNA_ANGLES_T_R__PITCH = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int ACEINNA_ANGLES_T_R__ROLL = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int ACEINNA_ANGLES_T_R__PITCH_COMPENSATION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int ACEINNA_ANGLES_T_R__ROLL_COMPENSATION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int ACEINNA_ANGLES_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int ACEINNA_ANGLES_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int MOTOR_HEARTBEAT_T_R = 11;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 8;
	public static final int MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 9;
	public static final int MOTOR_HEARTBEAT_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 10;
	public static final int MOTOR_HEARTBEAT_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;

	public static final int RAW_CAN_DATA_DLC_8_W = 12;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE0 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE1 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE2 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE3 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE4 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE5 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE6 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int RAW_CAN_DATA_DLC_8_W__BYTE7 = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int RAW_CAN_DATA_DLC_8_W_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 8;
	public static final int RAW_CAN_DATA_DLC_8_W___INITIALIZE__INT__INT__INT__INT__INT__INT__INT__INT = 0;
	public static final int RAW_CAN_DATA_DLC_8_W_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 1;

	public static final int GPS_NMEA0183_T_R = 13;
	public static final int GPS_NMEA0183_T_R__UTC_MS = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 0;
	public static final int GPS_NMEA0183_T_R__LATITUDE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 1;
	public static final int GPS_NMEA0183_T_R__LONGITUDE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 2;
	public static final int GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 3;
	public static final int GPS_NMEA0183_T_R__NO_SATELLITES = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 4;
	public static final int GPS_NMEA0183_T_R__ALTITUDE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 5;
	public static final int GPS_NMEA0183_T_R__GEOIDAL_SEPARATION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 6;
	public static final int GPS_NMEA0183_T_R__DGPS_AGE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 7;
	public static final int GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 8;
	public static final int GPS_NMEA0183_T_R__SELECTION_MODE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 9;
	public static final int GPS_NMEA0183_T_R__FIX_TYPE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 10;
	public static final int GPS_NMEA0183_T_R__PDOP = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 11;
	public static final int GPS_NMEA0183_T_R__HDOP = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 12;
	public static final int GPS_NMEA0183_T_R__VDOP = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 13;
	public static final int GPS_NMEA0183_T_R__STATUS = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 14;
	public static final int GPS_NMEA0183_T_R__SPEED = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 15;
	public static final int GPS_NMEA0183_T_R__COURSE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 16;
	public static final int GPS_NMEA0183_T_R__MAGNETIC_VARIATION = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 17;
	public static final int GPS_NMEA0183_T_R__FAA_MODE_INDICATOR = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 18;
	public static final int GPS_NMEA0183_T_R__HEADING = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 19;
	public static final int GPS_NMEA0183_T_R__HEADING_TRUE = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 20;
	public static final int GPS_NMEA0183_T_R_FEATURE_COUNT = TypesPackage.I_ARRAY_TYPE_FEATURE_COUNT + 21;
	public static final int GPS_NMEA0183_T_R_OPERATION_COUNT = TypesPackage.I_ARRAY_TYPE_OPERATION_COUNT + 0;


	private EClass iBulkProcessorEClass = null;
	private EClass bulkProcessorEClass = null;
	private EClass iFcalFactoryEClass = null;
	private EClass rECEIVE_ACC_INFO_T_REClass = null;
	private EClass rECEIVE_FNR_INFO_T_REClass = null;
	private EClass rECEIVE_HYD_INFO_T_REClass = null;
	private EClass rECEIVE_SFT_INFO_T_REClass = null;
	private EClass aGMO_GENERIC_SEND_CMD_T_WEClass = null;
	private EClass aCEINNA_ACCEL_T_REClass = null;
	private EClass aCEINNA_ANGLERATE_T_REClass = null;
	private EClass aCEINNA_ANGLES_T_REClass = null;
	private EClass mOTOR_HEARTBEAT_T_REClass = null;
	private EClass rAW_CAN_DATA_DLC_8_WEClass = null;
	private EClass gPS_NMEA0183_T_REClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public FcalPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(FcalFactory.eINSTANCE);
	}

	public static FcalPackage init() {
		if (isInited) return (FcalPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		FcalPackage thePackage = (registered instanceof FcalPackage) ? (FcalPackage) registered : new FcalPackage();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
		TypesPackage.eINSTANCE.eClass();
		thePackage.createPackageContents();
		thePackage.initializePackageContents(thePackage);
		thePackage.freeze();
		EPackage.Registry.INSTANCE.put(eNS_URI, thePackage);
		return thePackage;
	}

	private void createPackageContents() {
		if (isCreated) return;
		isCreated = true;
		createPackageContents_part0();
	}

	private void createPackageContents_part0() {
		iBulkProcessorEClass = createEClass(I_BULK_PROCESSOR);
		createEOperation(iBulkProcessorEClass, I_BULK_PROCESSOR___EXECUTE_BULK_OPERATIONS);

		bulkProcessorEClass = createEClass(BULK_PROCESSOR);

		iFcalFactoryEClass = createEClass(I_FCAL_FACTORY);

		rECEIVE_ACC_INFO_T_REClass = createEClass(RECEIVE_ACC_INFO_T_R);
		createEAttribute(rECEIVE_ACC_INFO_T_REClass, RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG1_V);
		createEAttribute(rECEIVE_ACC_INFO_T_REClass, RECEIVE_ACC_INFO_T_R__TRZ_ACC_SIG2_V);
		createEAttribute(rECEIVE_ACC_INFO_T_REClass, RECEIVE_ACC_INFO_T_R__TRZ_ACC_DIAG);
		createEAttribute(rECEIVE_ACC_INFO_T_REClass, RECEIVE_ACC_INFO_T_R__TRZ_ACC_AUTO);

		rECEIVE_FNR_INFO_T_REClass = createEClass(RECEIVE_FNR_INFO_T_R);
		createEAttribute(rECEIVE_FNR_INFO_T_REClass, RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG1_V);
		createEAttribute(rECEIVE_FNR_INFO_T_REClass, RECEIVE_FNR_INFO_T_R__TRZ_FNR_SIG2_V);
		createEAttribute(rECEIVE_FNR_INFO_T_REClass, RECEIVE_FNR_INFO_T_R__TRZ_FNR_DIAG);
		createEAttribute(rECEIVE_FNR_INFO_T_REClass, RECEIVE_FNR_INFO_T_R__TRZ_FNR_AUTO);
		createEAttribute(rECEIVE_FNR_INFO_T_REClass, RECEIVE_FNR_INFO_T_R__TRZ_FNR_STATE);

		rECEIVE_HYD_INFO_T_REClass = createEClass(RECEIVE_HYD_INFO_T_R);
		createEAttribute(rECEIVE_HYD_INFO_T_REClass, RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG1_V);
		createEAttribute(rECEIVE_HYD_INFO_T_REClass, RECEIVE_HYD_INFO_T_R__TRZ_HYD_SIG2_V);
		createEAttribute(rECEIVE_HYD_INFO_T_REClass, RECEIVE_HYD_INFO_T_R__TRZ_HYD_DIAG);
		createEAttribute(rECEIVE_HYD_INFO_T_REClass, RECEIVE_HYD_INFO_T_R__TRZ_HYD_AUTO);

		rECEIVE_SFT_INFO_T_REClass = createEClass(RECEIVE_SFT_INFO_T_R);
		createEAttribute(rECEIVE_SFT_INFO_T_REClass, RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG1_V);
		createEAttribute(rECEIVE_SFT_INFO_T_REClass, RECEIVE_SFT_INFO_T_R__TRZ_SFT_SIG2_V);
		createEAttribute(rECEIVE_SFT_INFO_T_REClass, RECEIVE_SFT_INFO_T_R__TRZ_SFT_DIAG);
		createEAttribute(rECEIVE_SFT_INFO_T_REClass, RECEIVE_SFT_INFO_T_R__TRZ_SFT_AUTO);
		createEAttribute(rECEIVE_SFT_INFO_T_REClass, RECEIVE_SFT_INFO_T_R__TRZ_SFT_STATE);

		aGMO_GENERIC_SEND_CMD_T_WEClass = createEClass(AGMO_GENERIC_SEND_CMD_T_W);
		createEAttribute(aGMO_GENERIC_SEND_CMD_T_WEClass, AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_CMD);
		createEAttribute(aGMO_GENERIC_SEND_CMD_T_WEClass, AGMO_GENERIC_SEND_CMD_T_W__AD_GENERIC_MODE_CMD);
		createEOperation(aGMO_GENERIC_SEND_CMD_T_WEClass, AGMO_GENERIC_SEND_CMD_T_W___INITIALIZE__INT__INT);

		aCEINNA_ACCEL_T_REClass = createEClass(ACEINNA_ACCEL_T_R);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__ACC_Y);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__ACC_X);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__ACC_Z);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__LATERAL_ACC_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__LONGI_ACC_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__VERTIC_ACC_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ACCEL_T_REClass, ACEINNA_ACCEL_T_R__SUPPORT_RATE_ACC);

		aCEINNA_ANGLERATE_T_REClass = createEClass(ACEINNA_ANGLERATE_T_R);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_Y);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_X);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_Z);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_Y_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_X_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__GYRO_Z_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ANGLERATE_T_REClass, ACEINNA_ANGLERATE_T_R__ANGLE_RATE_LATENCY);

		aCEINNA_ANGLES_T_REClass = createEClass(ACEINNA_ANGLES_T_R);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__PITCH);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__ROLL);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__PITCH_COMPENSATION);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__PITCH_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__ROLL_COMPENSATION);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__ROLL_FIGURE_OF_MERIT);
		createEAttribute(aCEINNA_ANGLES_T_REClass, ACEINNA_ANGLES_T_R__PITCH_ROLL_LATENCY);

		mOTOR_HEARTBEAT_T_REClass = createEClass(MOTOR_HEARTBEAT_T_R);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HALL_FAILURE);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_CA_NDISCONNECTED);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MOTOR_STALLED);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_DISABLED);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERVOLTAGE);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_HARDWARE_PROTECTION);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_E2PROM);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_UNDERVOLTAGE);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_OVERCURRENT);
		createEAttribute(mOTOR_HEARTBEAT_T_REClass, MOTOR_HEARTBEAT_T_R__HEARTBEAT_CONTROL_STATUS_MODE_FAILURE);

		rAW_CAN_DATA_DLC_8_WEClass = createEClass(RAW_CAN_DATA_DLC_8_W);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE0);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE1);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE2);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE3);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE4);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE5);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE6);
		createEAttribute(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W__BYTE7);
		createEOperation(rAW_CAN_DATA_DLC_8_WEClass, RAW_CAN_DATA_DLC_8_W___INITIALIZE__INT__INT__INT__INT__INT__INT__INT__INT);

		gPS_NMEA0183_T_REClass = createEClass(GPS_NMEA0183_T_R);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__UTC_MS);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__LATITUDE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__LONGITUDE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__NO_SATELLITES);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__ALTITUDE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__GEOIDAL_SEPARATION);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__DGPS_AGE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__SELECTION_MODE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__FIX_TYPE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__PDOP);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__HDOP);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__VDOP);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__STATUS);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__SPEED);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__COURSE);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__MAGNETIC_VARIATION);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__FAA_MODE_INDICATOR);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__HEADING);
		createEAttribute(gPS_NMEA0183_T_REClass, GPS_NMEA0183_T_R__HEADING_TRUE);

	}

	private void initializePackageContents(FcalPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
		initializePackageContents_part1(thePackage);
	}

	private void initializePackageContents_part0(FcalPackage thePackage) {
		thePackage.iBulkProcessorEClass.setName("IBulkProcessor");
		thePackage.iBulkProcessorEClass.setInstanceClassName("com.bosch.nevonex.fcal.IBulkProcessor");
		thePackage.iBulkProcessorEClass.setAbstract(true);
		thePackage.iBulkProcessorEClass.setInterface(true);
		thePackage.iBulkProcessorEClass.getEOperations().get(0).setName("executeBulkOperations");
		thePackage.bulkProcessorEClass.setName("BulkProcessor");
		thePackage.bulkProcessorEClass.setInstanceClassName("com.bosch.nevonex.fcal.IBulkProcessor");
		thePackage.bulkProcessorEClass.setAbstract(false);
		thePackage.bulkProcessorEClass.setInterface(false);
		thePackage.iFcalFactoryEClass.setName("IFcalFactory");
		thePackage.iFcalFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcal.IFcalFactory");
		thePackage.iFcalFactoryEClass.setAbstract(true);
		thePackage.iFcalFactoryEClass.setInterface(true);
		thePackage.rECEIVE_ACC_INFO_T_REClass.setName("RECEIVE_ACC_INFO_T_R");
		thePackage.rECEIVE_ACC_INFO_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R");
		thePackage.rECEIVE_ACC_INFO_T_REClass.setAbstract(false);
		thePackage.rECEIVE_ACC_INFO_T_REClass.setInterface(false);
		thePackage.rECEIVE_ACC_INFO_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(0)).setName("tRZ_ACC_SIG1_V");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(1)).setName("tRZ_ACC_SIG2_V");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(2)).setName("tRZ_ACC_DIAG");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(3)).setName("tRZ_ACC_AUTO");
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_ACC_INFO_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		thePackage.rECEIVE_FNR_INFO_T_REClass.setName("RECEIVE_FNR_INFO_T_R");
		thePackage.rECEIVE_FNR_INFO_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R");
		thePackage.rECEIVE_FNR_INFO_T_REClass.setAbstract(false);
		thePackage.rECEIVE_FNR_INFO_T_REClass.setInterface(false);
		thePackage.rECEIVE_FNR_INFO_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(0)).setName("tRZ_FNR_SIG1_V");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(1)).setName("tRZ_FNR_SIG2_V");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(2)).setName("tRZ_FNR_DIAG");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(3)).setName("tRZ_FNR_AUTO");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(4)).setName("tRZ_FNR_STATE");
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_FNR_INFO_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		thePackage.rECEIVE_HYD_INFO_T_REClass.setName("RECEIVE_HYD_INFO_T_R");
		thePackage.rECEIVE_HYD_INFO_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R");
		thePackage.rECEIVE_HYD_INFO_T_REClass.setAbstract(false);
		thePackage.rECEIVE_HYD_INFO_T_REClass.setInterface(false);
		thePackage.rECEIVE_HYD_INFO_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(0)).setName("tRZ_HYD_SIG1_V");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(1)).setName("tRZ_HYD_SIG2_V");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(2)).setName("tRZ_HYD_DIAG");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(3)).setName("tRZ_HYD_AUTO");
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_HYD_INFO_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		thePackage.rECEIVE_SFT_INFO_T_REClass.setName("RECEIVE_SFT_INFO_T_R");
		thePackage.rECEIVE_SFT_INFO_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R");
		thePackage.rECEIVE_SFT_INFO_T_REClass.setAbstract(false);
		thePackage.rECEIVE_SFT_INFO_T_REClass.setInterface(false);
		thePackage.rECEIVE_SFT_INFO_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(0)).setName("tRZ_SFT_SIG1_V");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(1)).setName("tRZ_SFT_SIG2_V");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(2)).setName("tRZ_SFT_DIAG");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(3)).setName("tRZ_SFT_AUTO");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(4)).setName("tRZ_SFT_STATE");
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.rECEIVE_SFT_INFO_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.setName("AGMO_GENERIC_SEND_CMD_T_W");
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.setInstanceClassName("com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W");
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.setAbstract(false);
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.setInterface(false);
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(0)).setName("aD_Generic_Cmd");
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(1)).setName("aD_Generic_Mode_Cmd");
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0");
		thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEOperations().get(0).setName("initialize");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("aD_Generic_Cmd");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("aD_Generic_Mode_Cmd");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.aGMO_GENERIC_SEND_CMD_T_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		thePackage.aCEINNA_ACCEL_T_REClass.setName("ACEINNA_ACCEL_T_R");
		thePackage.aCEINNA_ACCEL_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R");
		thePackage.aCEINNA_ACCEL_T_REClass.setAbstract(false);
		thePackage.aCEINNA_ACCEL_T_REClass.setInterface(false);
		thePackage.aCEINNA_ACCEL_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(0)).setName("accY");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(1)).setName("accX");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(2)).setName("accZ");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(3)).setName("lateralAcc_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(4)).setName("longiAcc_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(5)).setName("verticAcc_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(6)).setName("support_Rate_Acc");
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ACCEL_T_REClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0");
		thePackage.aCEINNA_ANGLERATE_T_REClass.setName("ACEINNA_ANGLERATE_T_R");
		thePackage.aCEINNA_ANGLERATE_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R");
		thePackage.aCEINNA_ANGLERATE_T_REClass.setAbstract(false);
		thePackage.aCEINNA_ANGLERATE_T_REClass.setInterface(false);
		thePackage.aCEINNA_ANGLERATE_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(0)).setName("gyroY");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(1)).setName("gyroX");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(2)).setName("gyroZ");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(3)).setName("gyroY_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(4)).setName("gyroX_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(5)).setName("gyroZ_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(6)).setName("angleRate_Latency");
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLERATE_T_REClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0.0f");
		thePackage.aCEINNA_ANGLES_T_REClass.setName("ACEINNA_ANGLES_T_R");
		thePackage.aCEINNA_ANGLES_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R");
		thePackage.aCEINNA_ANGLES_T_REClass.setAbstract(false);
		thePackage.aCEINNA_ANGLES_T_REClass.setInterface(false);
		thePackage.aCEINNA_ANGLES_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(0)).setName("pitch");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(1)).setName("roll");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(2)).setName("pitch_Compensation");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(3)).setName("pitch_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(4)).setName("roll_Compensation");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(5)).setName("roll_FigureOfMerit");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(6)).setName("pitchRoll_Latency");
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.aCEINNA_ANGLES_T_REClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0.0f");
		thePackage.mOTOR_HEARTBEAT_T_REClass.setName("MOTOR_HEARTBEAT_T_R");
		thePackage.mOTOR_HEARTBEAT_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R");
		thePackage.mOTOR_HEARTBEAT_T_REClass.setAbstract(false);
		thePackage.mOTOR_HEARTBEAT_T_REClass.setInterface(false);
		thePackage.mOTOR_HEARTBEAT_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(0)).setName("heartbeat_ControlStatus_HallFailure");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(1)).setName("heartbeat_ControlStatus_CANdisconnected");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(2)).setName("heartbeat_ControlStatus_MotorStalled");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(3)).setName("heartbeat_ControlStatus_Disabled");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(4)).setName("heartbeat_ControlStatus_Overvoltage");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(5)).setName("heartbeat_ControlStatus_HardwareProtection");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(6)).setName("heartbeat_ControlStatus_E2PROM");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(7)).setName("heartbeat_ControlStatus_Undervoltage");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(7)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(7)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(7)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(7)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(8)).setName("heartbeat_ControlStatus_Overcurrent");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(8)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(8)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(8)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(8)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(9)).setName("heartbeat_ControlStatus_ModeFailure");
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(9)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(9)).setUpperBound(1);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(9)).setLowerBound(0);
		((EAttribute) thePackage.mOTOR_HEARTBEAT_T_REClass.getEStructuralFeatures().get(9)).setDefaultValueLiteral("0");
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.setName("RAW_CAN_DATA_DLC_8_W");
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.setInstanceClassName("com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W");
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.setAbstract(false);
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.setInterface(false);
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(0)).setName("byte0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(0)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(1)).setName("byte1");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(2)).setName("byte2");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(3)).setName("byte3");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(4)).setName("byte4");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(5)).setName("byte5");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(6)).setName("byte6");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(7)).setName("byte7");
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(7)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(7)).setUpperBound(1);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(7)).setLowerBound(0);
		((EAttribute) thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeatures().get(7)).setDefaultValueLiteral("0");
		thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).setName("initialize");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte0");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte1");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte2");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte3");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte4");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
	}

	private void initializePackageContents_part1(FcalPackage thePackage) {
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte5");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte6");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("byte7");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0).getEParameters().add(param);
		}
		thePackage.gPS_NMEA0183_T_REClass.setName("GPS_NMEA0183_T_R");
		thePackage.gPS_NMEA0183_T_REClass.setInstanceClassName("com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R");
		thePackage.gPS_NMEA0183_T_REClass.setAbstract(false);
		thePackage.gPS_NMEA0183_T_REClass.setInterface(false);
		thePackage.gPS_NMEA0183_T_REClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getIArrayType());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(0)).setName("uTC_ms");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(0)).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(0)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(0)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(1)).setName("latitude");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(1)).setEType((EClassifier) EcorePackage.eINSTANCE.getEDouble());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(1)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(1)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(1)).setDefaultValueLiteral("0.0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(2)).setName("longitude");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(2)).setEType((EClassifier) EcorePackage.eINSTANCE.getEDouble());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(2)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(2)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(2)).setDefaultValueLiteral("0.0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(3)).setName("gPS_Quality_Indicator");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(3)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(3)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(3)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(3)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(4)).setName("no_Satellites");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(4)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(4)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(4)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(4)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(5)).setName("altitude");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(5)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(5)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(5)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(5)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(6)).setName("geoidal_Separation");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(6)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(6)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(6)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(6)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(7)).setName("dGPS_Age");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(7)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(7)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(7)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(7)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(8)).setName("differential_Ref_Station");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(8)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(8)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(8)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(8)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(9)).setName("selection_Mode");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(9)).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(9)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(9)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(10)).setName("fix_Type");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(10)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(10)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(10)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(10)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(11)).setName("pDOP");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(11)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(11)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(11)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(11)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(12)).setName("hDOP");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(12)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(12)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(12)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(12)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(13)).setName("vDOP");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(13)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(13)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(13)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(13)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(14)).setName("status");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(14)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(14)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(14)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(14)).setDefaultValueLiteral("0");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(15)).setName("speed");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(15)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(15)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(15)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(15)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(16)).setName("course");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(16)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(16)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(16)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(16)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(17)).setName("magnetic_variation");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(17)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(17)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(17)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(17)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(18)).setName("fAA_Mode_Indicator");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(18)).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(18)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(18)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(19)).setName("heading");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(19)).setEType((EClassifier) EcorePackage.eINSTANCE.getEFloat());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(19)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(19)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(19)).setDefaultValueLiteral("0.0f");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(20)).setName("headingTrue");
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(20)).setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(20)).setUpperBound(1);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(20)).setLowerBound(0);
		((EAttribute) thePackage.gPS_NMEA0183_T_REClass.getEStructuralFeatures().get(20)).setDefaultValueLiteral("0");
	}

	public EClass getIBulkProcessor() { return iBulkProcessorEClass; }
	public EClass getBulkProcessor() { return bulkProcessorEClass; }
	public EClass getIFcalFactory() { return iFcalFactoryEClass; }
	public EClass getRECEIVE_ACC_INFO_T_R() { return rECEIVE_ACC_INFO_T_REClass; }
	public EClass getRECEIVE_FNR_INFO_T_R() { return rECEIVE_FNR_INFO_T_REClass; }
	public EClass getRECEIVE_HYD_INFO_T_R() { return rECEIVE_HYD_INFO_T_REClass; }
	public EClass getRECEIVE_SFT_INFO_T_R() { return rECEIVE_SFT_INFO_T_REClass; }
	public EClass getAGMO_GENERIC_SEND_CMD_T_W() { return aGMO_GENERIC_SEND_CMD_T_WEClass; }
	public EClass getACEINNA_ACCEL_T_R() { return aCEINNA_ACCEL_T_REClass; }
	public EClass getACEINNA_ANGLERATE_T_R() { return aCEINNA_ANGLERATE_T_REClass; }
	public EClass getACEINNA_ANGLES_T_R() { return aCEINNA_ANGLES_T_REClass; }
	public EClass getMOTOR_HEARTBEAT_T_R() { return mOTOR_HEARTBEAT_T_REClass; }
	public EClass getRAW_CAN_DATA_DLC_8_W() { return rAW_CAN_DATA_DLC_8_WEClass; }
	public EClass getGPS_NMEA0183_T_R() { return gPS_NMEA0183_T_REClass; }
	public EAttribute getRECEIVE_ACC_INFO_T_R_TRZ_ACC_SIG1_V() {
		return (EAttribute) rECEIVE_ACC_INFO_T_REClass.getEStructuralFeature("tRZ_ACC_SIG1_V");
	}
	public EAttribute getRECEIVE_ACC_INFO_T_R_TRZ_ACC_SIG2_V() {
		return (EAttribute) rECEIVE_ACC_INFO_T_REClass.getEStructuralFeature("tRZ_ACC_SIG2_V");
	}
	public EAttribute getRECEIVE_ACC_INFO_T_R_TRZ_ACC_DIAG() {
		return (EAttribute) rECEIVE_ACC_INFO_T_REClass.getEStructuralFeature("tRZ_ACC_DIAG");
	}
	public EAttribute getRECEIVE_ACC_INFO_T_R_TRZ_ACC_AUTO() {
		return (EAttribute) rECEIVE_ACC_INFO_T_REClass.getEStructuralFeature("tRZ_ACC_AUTO");
	}
	public EAttribute getRECEIVE_FNR_INFO_T_R_TRZ_FNR_SIG1_V() {
		return (EAttribute) rECEIVE_FNR_INFO_T_REClass.getEStructuralFeature("tRZ_FNR_SIG1_V");
	}
	public EAttribute getRECEIVE_FNR_INFO_T_R_TRZ_FNR_SIG2_V() {
		return (EAttribute) rECEIVE_FNR_INFO_T_REClass.getEStructuralFeature("tRZ_FNR_SIG2_V");
	}
	public EAttribute getRECEIVE_FNR_INFO_T_R_TRZ_FNR_DIAG() {
		return (EAttribute) rECEIVE_FNR_INFO_T_REClass.getEStructuralFeature("tRZ_FNR_DIAG");
	}
	public EAttribute getRECEIVE_FNR_INFO_T_R_TRZ_FNR_AUTO() {
		return (EAttribute) rECEIVE_FNR_INFO_T_REClass.getEStructuralFeature("tRZ_FNR_AUTO");
	}
	public EAttribute getRECEIVE_FNR_INFO_T_R_TRZ_FNR_STATE() {
		return (EAttribute) rECEIVE_FNR_INFO_T_REClass.getEStructuralFeature("tRZ_FNR_STATE");
	}
	public EAttribute getRECEIVE_HYD_INFO_T_R_TRZ_HYD_SIG1_V() {
		return (EAttribute) rECEIVE_HYD_INFO_T_REClass.getEStructuralFeature("tRZ_HYD_SIG1_V");
	}
	public EAttribute getRECEIVE_HYD_INFO_T_R_TRZ_HYD_SIG2_V() {
		return (EAttribute) rECEIVE_HYD_INFO_T_REClass.getEStructuralFeature("tRZ_HYD_SIG2_V");
	}
	public EAttribute getRECEIVE_HYD_INFO_T_R_TRZ_HYD_DIAG() {
		return (EAttribute) rECEIVE_HYD_INFO_T_REClass.getEStructuralFeature("tRZ_HYD_DIAG");
	}
	public EAttribute getRECEIVE_HYD_INFO_T_R_TRZ_HYD_AUTO() {
		return (EAttribute) rECEIVE_HYD_INFO_T_REClass.getEStructuralFeature("tRZ_HYD_AUTO");
	}
	public EAttribute getRECEIVE_SFT_INFO_T_R_TRZ_SFT_SIG1_V() {
		return (EAttribute) rECEIVE_SFT_INFO_T_REClass.getEStructuralFeature("tRZ_SFT_SIG1_V");
	}
	public EAttribute getRECEIVE_SFT_INFO_T_R_TRZ_SFT_SIG2_V() {
		return (EAttribute) rECEIVE_SFT_INFO_T_REClass.getEStructuralFeature("tRZ_SFT_SIG2_V");
	}
	public EAttribute getRECEIVE_SFT_INFO_T_R_TRZ_SFT_DIAG() {
		return (EAttribute) rECEIVE_SFT_INFO_T_REClass.getEStructuralFeature("tRZ_SFT_DIAG");
	}
	public EAttribute getRECEIVE_SFT_INFO_T_R_TRZ_SFT_AUTO() {
		return (EAttribute) rECEIVE_SFT_INFO_T_REClass.getEStructuralFeature("tRZ_SFT_AUTO");
	}
	public EAttribute getRECEIVE_SFT_INFO_T_R_TRZ_SFT_STATE() {
		return (EAttribute) rECEIVE_SFT_INFO_T_REClass.getEStructuralFeature("tRZ_SFT_STATE");
	}
	public EAttribute getAGMO_GENERIC_SEND_CMD_T_W_AD_Generic_Cmd() {
		return (EAttribute) aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeature("aD_Generic_Cmd");
	}
	public EAttribute getAGMO_GENERIC_SEND_CMD_T_W_AD_Generic_Mode_Cmd() {
		return (EAttribute) aGMO_GENERIC_SEND_CMD_T_WEClass.getEStructuralFeature("aD_Generic_Mode_Cmd");
	}
	public EAttribute getACEINNA_ACCEL_T_R_AccY() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("accY");
	}
	public EAttribute getACEINNA_ACCEL_T_R_AccX() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("accX");
	}
	public EAttribute getACEINNA_ACCEL_T_R_AccZ() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("accZ");
	}
	public EAttribute getACEINNA_ACCEL_T_R_LateralAcc_FigureOfMerit() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("lateralAcc_FigureOfMerit");
	}
	public EAttribute getACEINNA_ACCEL_T_R_LongiAcc_FigureOfMerit() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("longiAcc_FigureOfMerit");
	}
	public EAttribute getACEINNA_ACCEL_T_R_VerticAcc_FigureOfMerit() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("verticAcc_FigureOfMerit");
	}
	public EAttribute getACEINNA_ACCEL_T_R_Support_Rate_Acc() {
		return (EAttribute) aCEINNA_ACCEL_T_REClass.getEStructuralFeature("support_Rate_Acc");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroY() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroY");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroX() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroX");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroZ() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroZ");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroY_FigureOfMerit() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroY_FigureOfMerit");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroX_FigureOfMerit() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroX_FigureOfMerit");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_GyroZ_FigureOfMerit() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("gyroZ_FigureOfMerit");
	}
	public EAttribute getACEINNA_ANGLERATE_T_R_AngleRate_Latency() {
		return (EAttribute) aCEINNA_ANGLERATE_T_REClass.getEStructuralFeature("angleRate_Latency");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Pitch() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("pitch");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Roll() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("roll");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Pitch_Compensation() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("pitch_Compensation");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Pitch_FigureOfMerit() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("pitch_FigureOfMerit");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Roll_Compensation() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("roll_Compensation");
	}
	public EAttribute getACEINNA_ANGLES_T_R_Roll_FigureOfMerit() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("roll_FigureOfMerit");
	}
	public EAttribute getACEINNA_ANGLES_T_R_PitchRoll_Latency() {
		return (EAttribute) aCEINNA_ANGLES_T_REClass.getEStructuralFeature("pitchRoll_Latency");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_HallFailure() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_HallFailure");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_CANdisconnected() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_CANdisconnected");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_MotorStalled() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_MotorStalled");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_Disabled() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_Disabled");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_Overvoltage() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_Overvoltage");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_HardwareProtection() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_HardwareProtection");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_E2PROM() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_E2PROM");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_Undervoltage() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_Undervoltage");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_Overcurrent() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_Overcurrent");
	}
	public EAttribute getMOTOR_HEARTBEAT_T_R_Heartbeat_ControlStatus_ModeFailure() {
		return (EAttribute) mOTOR_HEARTBEAT_T_REClass.getEStructuralFeature("heartbeat_ControlStatus_ModeFailure");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte0() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte0");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte1() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte1");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte2() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte2");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte3() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte3");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte4() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte4");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte5() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte5");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte6() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte6");
	}
	public EAttribute getRAW_CAN_DATA_DLC_8_W_Byte7() {
		return (EAttribute) rAW_CAN_DATA_DLC_8_WEClass.getEStructuralFeature("byte7");
	}
	public EAttribute getGPS_NMEA0183_T_R_UTC_ms() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("uTC_ms");
	}
	public EAttribute getGPS_NMEA0183_T_R_Latitude() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("latitude");
	}
	public EAttribute getGPS_NMEA0183_T_R_Longitude() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("longitude");
	}
	public EAttribute getGPS_NMEA0183_T_R_GPS_Quality_Indicator() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("gPS_Quality_Indicator");
	}
	public EAttribute getGPS_NMEA0183_T_R_No_Satellites() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("no_Satellites");
	}
	public EAttribute getGPS_NMEA0183_T_R_Altitude() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("altitude");
	}
	public EAttribute getGPS_NMEA0183_T_R_Geoidal_Separation() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("geoidal_Separation");
	}
	public EAttribute getGPS_NMEA0183_T_R_DGPS_Age() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("dGPS_Age");
	}
	public EAttribute getGPS_NMEA0183_T_R_Differential_Ref_Station() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("differential_Ref_Station");
	}
	public EAttribute getGPS_NMEA0183_T_R_Selection_Mode() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("selection_Mode");
	}
	public EAttribute getGPS_NMEA0183_T_R_Fix_Type() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("fix_Type");
	}
	public EAttribute getGPS_NMEA0183_T_R_PDOP() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("pDOP");
	}
	public EAttribute getGPS_NMEA0183_T_R_HDOP() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("hDOP");
	}
	public EAttribute getGPS_NMEA0183_T_R_VDOP() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("vDOP");
	}
	public EAttribute getGPS_NMEA0183_T_R_Status() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("status");
	}
	public EAttribute getGPS_NMEA0183_T_R_Speed() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("speed");
	}
	public EAttribute getGPS_NMEA0183_T_R_Course() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("course");
	}
	public EAttribute getGPS_NMEA0183_T_R_Magnetic_variation() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("magnetic_variation");
	}
	public EAttribute getGPS_NMEA0183_T_R_FAA_Mode_Indicator() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("fAA_Mode_Indicator");
	}
	public EAttribute getGPS_NMEA0183_T_R_Heading() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("heading");
	}
	public EAttribute getGPS_NMEA0183_T_R_HeadingTrue() {
		return (EAttribute) gPS_NMEA0183_T_REClass.getEStructuralFeature("headingTrue");
	}
	public EOperation getIBulkProcessor__ExecuteBulkOperations() {
		return iBulkProcessorEClass.getEOperations().get(0);
	}
	public EOperation getAGMO_GENERIC_SEND_CMD_T_W__Initialize__int__int() {
		return aGMO_GENERIC_SEND_CMD_T_WEClass.getEOperations().get(0);
	}
	public EOperation getRAW_CAN_DATA_DLC_8_W__Initialize__int__int__int__int__int__int__int__int() {
		return rAW_CAN_DATA_DLC_8_WEClass.getEOperations().get(0);
	}

	public IFcalFactory getFcalFactory() {
		return (IFcalFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_BULK_PROCESSOR = eINSTANCE.getIBulkProcessor();
		public static final EClass BULK_PROCESSOR = eINSTANCE.getBulkProcessor();
		public static final EClass I_FCAL_FACTORY = eINSTANCE.getIFcalFactory();
		public static final EClass RECEIVE_ACC_INFO_T_R = eINSTANCE.getRECEIVE_ACC_INFO_T_R();
		public static final EClass RECEIVE_FNR_INFO_T_R = eINSTANCE.getRECEIVE_FNR_INFO_T_R();
		public static final EClass RECEIVE_HYD_INFO_T_R = eINSTANCE.getRECEIVE_HYD_INFO_T_R();
		public static final EClass RECEIVE_SFT_INFO_T_R = eINSTANCE.getRECEIVE_SFT_INFO_T_R();
		public static final EClass AGMO_GENERIC_SEND_CMD_T_W = eINSTANCE.getAGMO_GENERIC_SEND_CMD_T_W();
		public static final EClass ACEINNA_ACCEL_T_R = eINSTANCE.getACEINNA_ACCEL_T_R();
		public static final EClass ACEINNA_ANGLERATE_T_R = eINSTANCE.getACEINNA_ANGLERATE_T_R();
		public static final EClass ACEINNA_ANGLES_T_R = eINSTANCE.getACEINNA_ANGLES_T_R();
		public static final EClass MOTOR_HEARTBEAT_T_R = eINSTANCE.getMOTOR_HEARTBEAT_T_R();
		public static final EClass RAW_CAN_DATA_DLC_8_W = eINSTANCE.getRAW_CAN_DATA_DLC_8_W();
		public static final EClass GPS_NMEA0183_T_R = eINSTANCE.getGPS_NMEA0183_T_R();
	}

} //FcalPackage
