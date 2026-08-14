/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IMOTOR_HEARTBEAT_T_R extends IArrayType {
	int getHeartbeat_ControlStatus_HallFailure();

	void setHeartbeat_ControlStatus_HallFailure(int value);
	int getHeartbeat_ControlStatus_CANdisconnected();

	void setHeartbeat_ControlStatus_CANdisconnected(int value);
	int getHeartbeat_ControlStatus_MotorStalled();

	void setHeartbeat_ControlStatus_MotorStalled(int value);
	int getHeartbeat_ControlStatus_Disabled();

	void setHeartbeat_ControlStatus_Disabled(int value);
	int getHeartbeat_ControlStatus_Overvoltage();

	void setHeartbeat_ControlStatus_Overvoltage(int value);
	int getHeartbeat_ControlStatus_HardwareProtection();

	void setHeartbeat_ControlStatus_HardwareProtection(int value);
	int getHeartbeat_ControlStatus_E2PROM();

	void setHeartbeat_ControlStatus_E2PROM(int value);
	int getHeartbeat_ControlStatus_Undervoltage();

	void setHeartbeat_ControlStatus_Undervoltage(int value);
	int getHeartbeat_ControlStatus_Overcurrent();

	void setHeartbeat_ControlStatus_Overcurrent(int value);
	int getHeartbeat_ControlStatus_ModeFailure();

	void setHeartbeat_ControlStatus_ModeFailure(int value);
} // IMOTOR_HEARTBEAT_T_R
