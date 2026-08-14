/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal;

import com.bosch.nevonex.types.IArrayType;

/**
 * @generated
 */
public interface IGPS_NMEA0183_T_R extends IArrayType {
	String getUTC_ms();

	void setUTC_ms(String value);
	double getLatitude();

	void setLatitude(double value);
	double getLongitude();

	void setLongitude(double value);
	int getGPS_Quality_Indicator();

	void setGPS_Quality_Indicator(int value);
	int getNo_Satellites();

	void setNo_Satellites(int value);
	float getAltitude();

	void setAltitude(float value);
	float getGeoidal_Separation();

	void setGeoidal_Separation(float value);
	float getDGPS_Age();

	void setDGPS_Age(float value);
	int getDifferential_Ref_Station();

	void setDifferential_Ref_Station(int value);
	String getSelection_Mode();

	void setSelection_Mode(String value);
	int getFix_Type();

	void setFix_Type(int value);
	float getPDOP();

	void setPDOP(float value);
	float getHDOP();

	void setHDOP(float value);
	float getVDOP();

	void setVDOP(float value);
	int getStatus();

	void setStatus(int value);
	float getSpeed();

	void setSpeed(float value);
	float getCourse();

	void setCourse(float value);
	float getMagnetic_variation();

	void setMagnetic_variation(float value);
	String getFAA_Mode_Indicator();

	void setFAA_Mode_Indicator(String value);
	float getHeading();

	void setHeading(float value);
	int getHeadingTrue();

	void setHeadingTrue(int value);
} // IGPS_NMEA0183_T_R
