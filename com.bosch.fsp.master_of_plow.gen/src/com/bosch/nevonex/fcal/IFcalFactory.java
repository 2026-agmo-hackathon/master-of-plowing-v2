/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

/**
 * @generated
 */
public interface IFcalFactory {
	IFcalFactory INSTANCE = com.bosch.nevonex.fcal.impl.FcalFactory.eINSTANCE;

	IBulkProcessor createBulkProcessor();

	IRECEIVE_ACC_INFO_T_R createRECEIVE_ACC_INFO_T_R();

	IRECEIVE_FNR_INFO_T_R createRECEIVE_FNR_INFO_T_R();

	IRECEIVE_HYD_INFO_T_R createRECEIVE_HYD_INFO_T_R();

	IRECEIVE_SFT_INFO_T_R createRECEIVE_SFT_INFO_T_R();

	IAGMO_GENERIC_SEND_CMD_T_W createAGMO_GENERIC_SEND_CMD_T_W();

	IACEINNA_ACCEL_T_R createACEINNA_ACCEL_T_R();

	IACEINNA_ANGLERATE_T_R createACEINNA_ANGLERATE_T_R();

	IACEINNA_ANGLES_T_R createACEINNA_ANGLES_T_R();

	IMOTOR_HEARTBEAT_T_R createMOTOR_HEARTBEAT_T_R();

	IRAW_CAN_DATA_DLC_8_W createRAW_CAN_DATA_DLC_8_W();

	IGPS_NMEA0183_T_R createGPS_NMEA0183_T_R();

} //IFcalFactory
