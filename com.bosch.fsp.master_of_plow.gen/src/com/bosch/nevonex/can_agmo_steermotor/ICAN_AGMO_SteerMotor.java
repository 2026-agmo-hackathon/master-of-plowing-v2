/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.InterfaceDisabledException;
import com.bosch.fsp.runtime.feature.exception.MachineWriteException;
import com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R;
import com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W;
import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public interface ICAN_AGMO_SteerMotor extends EObject {
	IMOTOR_HEARTBEAT_T_R getMotor_Heartbeat();

	void setMotor_Heartbeat(IMOTOR_HEARTBEAT_T_R value);
	IRAW_CAN_DATA_DLC_8_W getMotor_Request();

	void setMotor_Request(IRAW_CAN_DATA_DLC_8_W value) throws MachineWriteException, CommunicationException, InterfaceDisabledException;
	int getMotor_Response_Encoder_Speed();

	void setMotor_Response_Encoder_Speed(int value);
	float getMotor_Response_EncoderCountValue();

	void setMotor_Response_EncoderCountValue(float value);
} // ICAN_AGMO_SteerMotor
