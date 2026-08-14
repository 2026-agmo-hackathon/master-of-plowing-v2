/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IAGMO_GENERIC_SEND_CMD_T_W extends IArrayType {
	int getAD_Generic_Cmd();

	void setAD_Generic_Cmd(int value);
	int getAD_Generic_Mode_Cmd();

	void setAD_Generic_Mode_Cmd(int value);
	void initialize(int aD_Generic_Cmd, int aD_Generic_Mode_Cmd);
} // IAGMO_GENERIC_SEND_CMD_T_W
