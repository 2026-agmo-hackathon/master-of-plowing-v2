/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IRAW_CAN_DATA_DLC_8_W extends IArrayType {
	int getByte0();

	void setByte0(int value);
	int getByte1();

	void setByte1(int value);
	int getByte2();

	void setByte2(int value);
	int getByte3();

	void setByte3(int value);
	int getByte4();

	void setByte4(int value);
	int getByte5();

	void setByte5(int value);
	int getByte6();

	void setByte6(int value);
	int getByte7();

	void setByte7(int value);
	void initialize(int byte0, int byte1, int byte2, int byte3, int byte4, int byte5, int byte6, int byte7);
} // IRAW_CAN_DATA_DLC_8_W
