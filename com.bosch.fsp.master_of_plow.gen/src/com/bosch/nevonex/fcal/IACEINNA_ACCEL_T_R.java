/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IACEINNA_ACCEL_T_R extends IArrayType {
	float getAccY();

	void setAccY(float value);
	float getAccX();

	void setAccX(float value);
	float getAccZ();

	void setAccZ(float value);
	int getLateralAcc_FigureOfMerit();

	void setLateralAcc_FigureOfMerit(int value);
	int getLongiAcc_FigureOfMerit();

	void setLongiAcc_FigureOfMerit(int value);
	int getVerticAcc_FigureOfMerit();

	void setVerticAcc_FigureOfMerit(int value);
	int getSupport_Rate_Acc();

	void setSupport_Rate_Acc(int value);
} // IACEINNA_ACCEL_T_R
