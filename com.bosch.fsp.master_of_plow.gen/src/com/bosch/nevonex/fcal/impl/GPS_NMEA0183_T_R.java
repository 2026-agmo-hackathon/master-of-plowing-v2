/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class GPS_NMEA0183_T_R extends EObjectImpl implements IGPS_NMEA0183_T_R {
	protected String uTC_ms = null;
	protected double latitude = 0.0;
	protected double longitude = 0.0;
	protected int gPS_Quality_Indicator = 0;
	protected int no_Satellites = 0;
	protected float altitude = 0.0f;
	protected float geoidal_Separation = 0.0f;
	protected float dGPS_Age = 0.0f;
	protected int differential_Ref_Station = 0;
	protected String selection_Mode = null;
	protected int fix_Type = 0;
	protected float pDOP = 0.0f;
	protected float hDOP = 0.0f;
	protected float vDOP = 0.0f;
	protected int status = 0;
	protected float speed = 0.0f;
	protected float course = 0.0f;
	protected float magnetic_variation = 0.0f;
	protected String fAA_Mode_Indicator = null;
	protected float heading = 0.0f;
	protected int headingTrue = 0;

	protected GPS_NMEA0183_T_R() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.GPS_NMEA0183_T_R;
	}

	public String getUTC_ms() {
		return uTC_ms;
	}

	public void setUTC_ms(String newUTC_ms) {
		uTC_ms = newUTC_ms;
	}

	public double getLatitude() {
		return latitude;
	}

	public void setLatitude(double newLatitude) {
		latitude = newLatitude;
	}

	public double getLongitude() {
		return longitude;
	}

	public void setLongitude(double newLongitude) {
		longitude = newLongitude;
	}

	public int getGPS_Quality_Indicator() {
		return gPS_Quality_Indicator;
	}

	public void setGPS_Quality_Indicator(int newGPS_Quality_Indicator) {
		gPS_Quality_Indicator = newGPS_Quality_Indicator;
	}

	public int getNo_Satellites() {
		return no_Satellites;
	}

	public void setNo_Satellites(int newNo_Satellites) {
		no_Satellites = newNo_Satellites;
	}

	public float getAltitude() {
		return altitude;
	}

	public void setAltitude(float newAltitude) {
		altitude = newAltitude;
	}

	public float getGeoidal_Separation() {
		return geoidal_Separation;
	}

	public void setGeoidal_Separation(float newGeoidal_Separation) {
		geoidal_Separation = newGeoidal_Separation;
	}

	public float getDGPS_Age() {
		return dGPS_Age;
	}

	public void setDGPS_Age(float newDGPS_Age) {
		dGPS_Age = newDGPS_Age;
	}

	public int getDifferential_Ref_Station() {
		return differential_Ref_Station;
	}

	public void setDifferential_Ref_Station(int newDifferential_Ref_Station) {
		differential_Ref_Station = newDifferential_Ref_Station;
	}

	public String getSelection_Mode() {
		return selection_Mode;
	}

	public void setSelection_Mode(String newSelection_Mode) {
		selection_Mode = newSelection_Mode;
	}

	public int getFix_Type() {
		return fix_Type;
	}

	public void setFix_Type(int newFix_Type) {
		fix_Type = newFix_Type;
	}

	public float getPDOP() {
		return pDOP;
	}

	public void setPDOP(float newPDOP) {
		pDOP = newPDOP;
	}

	public float getHDOP() {
		return hDOP;
	}

	public void setHDOP(float newHDOP) {
		hDOP = newHDOP;
	}

	public float getVDOP() {
		return vDOP;
	}

	public void setVDOP(float newVDOP) {
		vDOP = newVDOP;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int newStatus) {
		status = newStatus;
	}

	public float getSpeed() {
		return speed;
	}

	public void setSpeed(float newSpeed) {
		speed = newSpeed;
	}

	public float getCourse() {
		return course;
	}

	public void setCourse(float newCourse) {
		course = newCourse;
	}

	public float getMagnetic_variation() {
		return magnetic_variation;
	}

	public void setMagnetic_variation(float newMagnetic_variation) {
		magnetic_variation = newMagnetic_variation;
	}

	public String getFAA_Mode_Indicator() {
		return fAA_Mode_Indicator;
	}

	public void setFAA_Mode_Indicator(String newFAA_Mode_Indicator) {
		fAA_Mode_Indicator = newFAA_Mode_Indicator;
	}

	public float getHeading() {
		return heading;
	}

	public void setHeading(float newHeading) {
		heading = newHeading;
	}

	public int getHeadingTrue() {
		return headingTrue;
	}

	public void setHeadingTrue(int newHeadingTrue) {
		headingTrue = newHeadingTrue;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case FcalPackage.GPS_NMEA0183_T_R__UTC_MS:
			return getUTC_ms();
		case FcalPackage.GPS_NMEA0183_T_R__LATITUDE:
			return getLatitude();
		case FcalPackage.GPS_NMEA0183_T_R__LONGITUDE:
			return getLongitude();
		case FcalPackage.GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
			return getGPS_Quality_Indicator();
		case FcalPackage.GPS_NMEA0183_T_R__NO_SATELLITES:
			return getNo_Satellites();
		case FcalPackage.GPS_NMEA0183_T_R__ALTITUDE:
			return getAltitude();
		case FcalPackage.GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
			return getGeoidal_Separation();
		case FcalPackage.GPS_NMEA0183_T_R__DGPS_AGE:
			return getDGPS_Age();
		case FcalPackage.GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
			return getDifferential_Ref_Station();
		case FcalPackage.GPS_NMEA0183_T_R__SELECTION_MODE:
			return getSelection_Mode();
		case FcalPackage.GPS_NMEA0183_T_R__FIX_TYPE:
			return getFix_Type();
		case FcalPackage.GPS_NMEA0183_T_R__PDOP:
			return getPDOP();
		case FcalPackage.GPS_NMEA0183_T_R__HDOP:
			return getHDOP();
		case FcalPackage.GPS_NMEA0183_T_R__VDOP:
			return getVDOP();
		case FcalPackage.GPS_NMEA0183_T_R__STATUS:
			return getStatus();
		case FcalPackage.GPS_NMEA0183_T_R__SPEED:
			return getSpeed();
		case FcalPackage.GPS_NMEA0183_T_R__COURSE:
			return getCourse();
		case FcalPackage.GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
			return getMagnetic_variation();
		case FcalPackage.GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
			return getFAA_Mode_Indicator();
		case FcalPackage.GPS_NMEA0183_T_R__HEADING:
			return getHeading();
		case FcalPackage.GPS_NMEA0183_T_R__HEADING_TRUE:
			return getHeadingTrue();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case FcalPackage.GPS_NMEA0183_T_R__UTC_MS:
			setUTC_ms((String) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__LATITUDE:
			setLatitude((Double) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__LONGITUDE:
			setLongitude((Double) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
			setGPS_Quality_Indicator((Integer) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__NO_SATELLITES:
			setNo_Satellites((Integer) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__ALTITUDE:
			setAltitude((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
			setGeoidal_Separation((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__DGPS_AGE:
			setDGPS_Age((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
			setDifferential_Ref_Station((Integer) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__SELECTION_MODE:
			setSelection_Mode((String) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__FIX_TYPE:
			setFix_Type((Integer) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__PDOP:
			setPDOP((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HDOP:
			setHDOP((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__VDOP:
			setVDOP((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__STATUS:
			setStatus((Integer) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__SPEED:
			setSpeed((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__COURSE:
			setCourse((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
			setMagnetic_variation((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
			setFAA_Mode_Indicator((String) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING:
			setHeading((Float) newValue);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING_TRUE:
			setHeadingTrue((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case FcalPackage.GPS_NMEA0183_T_R__UTC_MS:
			setUTC_ms(null);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__LATITUDE:
			setLatitude(0.0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__LONGITUDE:
			setLongitude(0.0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
			setGPS_Quality_Indicator(0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__NO_SATELLITES:
			setNo_Satellites(0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__ALTITUDE:
			setAltitude(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
			setGeoidal_Separation(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__DGPS_AGE:
			setDGPS_Age(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
			setDifferential_Ref_Station(0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__SELECTION_MODE:
			setSelection_Mode(null);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__FIX_TYPE:
			setFix_Type(0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__PDOP:
			setPDOP(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HDOP:
			setHDOP(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__VDOP:
			setVDOP(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__STATUS:
			setStatus(0);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__SPEED:
			setSpeed(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__COURSE:
			setCourse(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
			setMagnetic_variation(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
			setFAA_Mode_Indicator(null);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING:
			setHeading(0.0f);
			return;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING_TRUE:
			setHeadingTrue(0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case FcalPackage.GPS_NMEA0183_T_R__UTC_MS:
			return uTC_ms != null;
		case FcalPackage.GPS_NMEA0183_T_R__LATITUDE:
			return latitude != 0.0;
		case FcalPackage.GPS_NMEA0183_T_R__LONGITUDE:
			return longitude != 0.0;
		case FcalPackage.GPS_NMEA0183_T_R__GPS_QUALITY_INDICATOR:
			return gPS_Quality_Indicator != 0;
		case FcalPackage.GPS_NMEA0183_T_R__NO_SATELLITES:
			return no_Satellites != 0;
		case FcalPackage.GPS_NMEA0183_T_R__ALTITUDE:
			return altitude != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__GEOIDAL_SEPARATION:
			return geoidal_Separation != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__DGPS_AGE:
			return dGPS_Age != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__DIFFERENTIAL_REF_STATION:
			return differential_Ref_Station != 0;
		case FcalPackage.GPS_NMEA0183_T_R__SELECTION_MODE:
			return selection_Mode != null;
		case FcalPackage.GPS_NMEA0183_T_R__FIX_TYPE:
			return fix_Type != 0;
		case FcalPackage.GPS_NMEA0183_T_R__PDOP:
			return pDOP != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__HDOP:
			return hDOP != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__VDOP:
			return vDOP != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__STATUS:
			return status != 0;
		case FcalPackage.GPS_NMEA0183_T_R__SPEED:
			return speed != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__COURSE:
			return course != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__MAGNETIC_VARIATION:
			return magnetic_variation != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__FAA_MODE_INDICATOR:
			return fAA_Mode_Indicator != null;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING:
			return heading != 0.0f;
		case FcalPackage.GPS_NMEA0183_T_R__HEADING_TRUE:
			return headingTrue != 0;
		}
		return super.eIsSet(featureID);
	}
} //GPS_NMEA0183_T_R
