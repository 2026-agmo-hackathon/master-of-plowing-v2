/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.impl;

import com.bosch.nevonex.common.IAbsolutePosition;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class AbsolutePosition extends EObjectImpl implements IAbsolutePosition {
	protected double altitude = 0.0;
	protected double latitude = 0.0;
	protected double longitude = 0.0;

	protected AbsolutePosition() {
		super();
	}

	@Override
	protected EClass eStaticClass() {
		return CommonPackage.Literals.ABSOLUTE_POSITION;
	}

	public double getAltitude() {
		return altitude;
	}

	public void setAltitude(double newAltitude) {
		altitude = newAltitude;
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

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case CommonPackage.ABSOLUTE_POSITION__ALTITUDE:
			return getAltitude();
		case CommonPackage.ABSOLUTE_POSITION__LATITUDE:
			return getLatitude();
		case CommonPackage.ABSOLUTE_POSITION__LONGITUDE:
			return getLongitude();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case CommonPackage.ABSOLUTE_POSITION__ALTITUDE:
			setAltitude((Double) newValue);
			return;
		case CommonPackage.ABSOLUTE_POSITION__LATITUDE:
			setLatitude((Double) newValue);
			return;
		case CommonPackage.ABSOLUTE_POSITION__LONGITUDE:
			setLongitude((Double) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case CommonPackage.ABSOLUTE_POSITION__ALTITUDE:
			setAltitude(0.0);
			return;
		case CommonPackage.ABSOLUTE_POSITION__LATITUDE:
			setLatitude(0.0);
			return;
		case CommonPackage.ABSOLUTE_POSITION__LONGITUDE:
			setLongitude(0.0);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case CommonPackage.ABSOLUTE_POSITION__ALTITUDE:
			return altitude != 0.0;
		case CommonPackage.ABSOLUTE_POSITION__LATITUDE:
			return latitude != 0.0;
		case CommonPackage.ABSOLUTE_POSITION__LONGITUDE:
			return longitude != 0.0;
		}
		return super.eIsSet(featureID);
	}
} //AbsolutePosition
