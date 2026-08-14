/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.impl;

import com.bosch.nevonex.common.ITopicObject;

import com.bosch.fsp.platform.InterfaceDetails;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * @generated
 */
public class TopicObject extends EObjectImpl implements ITopicObject {
	/**
	 * @generated
	 */
	protected static final int INDEX_EDEFAULT = 0;

	/**
	 * @generated
	 */
	protected int index = INDEX_EDEFAULT;

	/**
	 * @generated
	 */
	protected final Map<String, InterfaceDetails> interfaceDetailMap = new HashMap<String, InterfaceDetails>();

	/**
	 * @generated
	 */
	protected TopicObject() {
		super();
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return CommonPackage.Literals.TOPIC_OBJECT;
	}

	/**
	 * @generated
	 */
	public Map<String, InterfaceDetails> getInterfaceDetailMap() {
		return this.interfaceDetailMap;
	}

	/**
	 * @generated
	 */
	public int getIndex() {
		return index;
	}

	/**
	 * @generated
	 */
	public void setIndex(int newIndex) {
		index = newIndex;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case CommonPackage.TOPIC_OBJECT__INDEX:
			return getIndex();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case CommonPackage.TOPIC_OBJECT__INDEX:
			setIndex((Integer) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case CommonPackage.TOPIC_OBJECT__INDEX:
			setIndex(INDEX_EDEFAULT);
			return;
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case CommonPackage.TOPIC_OBJECT__INDEX:
			return index != INDEX_EDEFAULT;
		}
		return super.eIsSet(featureID);
	}
} //TopicObject
