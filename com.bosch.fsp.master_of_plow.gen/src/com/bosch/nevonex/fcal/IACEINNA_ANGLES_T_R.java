/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IACEINNA_ANGLES_T_R extends IArrayType {
	float getPitch();

	void setPitch(float value);
	float getRoll();

	void setRoll(float value);
	int getPitch_Compensation();

	void setPitch_Compensation(int value);
	int getPitch_FigureOfMerit();

	void setPitch_FigureOfMerit(int value);
	int getRoll_Compensation();

	void setRoll_Compensation(int value);
	int getRoll_FigureOfMerit();

	void setRoll_FigureOfMerit(int value);
	float getPitchRoll_Latency();

	void setPitchRoll_Latency(float value);
} // IACEINNA_ANGLES_T_R
