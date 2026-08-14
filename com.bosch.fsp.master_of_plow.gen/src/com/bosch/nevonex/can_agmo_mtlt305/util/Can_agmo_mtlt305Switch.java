/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305.util;

import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305;
import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider;
import com.bosch.nevonex.can_agmo_mtlt305.ICan_agmo_mtlt305Factory;

import com.bosch.nevonex.can_agmo_mtlt305.impl.Can_agmo_mtlt305Package;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.common.ITopicObject;

/**
 * @generated
 */
public class Can_agmo_mtlt305Switch<T> extends Switch<T> {
	protected static Can_agmo_mtlt305Package modelPackage;

	public Can_agmo_mtlt305Switch() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_mtlt305Package.eINSTANCE;
		}
	}

	@Override
	protected boolean isSwitchFor(EPackage ePackage) {
		return ePackage == modelPackage;
	}

	@Override
	protected T doSwitch(int classifierID, EObject theEObject) {
		if (classifierID < 80) return doSwitch_chunk0(classifierID, theEObject);
		return defaultCase(theEObject);
	}

	private T doSwitch_chunk0(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case Can_agmo_mtlt305Package.ICAN_AGMO_MTLT305: {
				ICAN_AGMO_MTLT305 iCAN_AGMO_MTLT305 = (ICAN_AGMO_MTLT305) theEObject;
				T result = caseICAN_AGMO_MTLT305(iCAN_AGMO_MTLT305);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305: {
				ICAN_AGMO_MTLT305 cAN_AGMO_MTLT305 = (ICAN_AGMO_MTLT305) theEObject;
				T result = caseCAN_AGMO_MTLT305(cAN_AGMO_MTLT305);
				if (result == null)
					result = caseTopicObject((ITopicObject) cAN_AGMO_MTLT305);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_mtlt305Package.ICAN_AGMO_MTLT305PROVIDER: {
				ICAN_AGMO_MTLT305Provider iCAN_AGMO_MTLT305Provider = (ICAN_AGMO_MTLT305Provider) theEObject;
				T result = caseICAN_AGMO_MTLT305Provider(iCAN_AGMO_MTLT305Provider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305PROVIDER: {
				ICAN_AGMO_MTLT305Provider cAN_AGMO_MTLT305Provider = (ICAN_AGMO_MTLT305Provider) theEObject;
				T result = caseCAN_AGMO_MTLT305Provider(cAN_AGMO_MTLT305Provider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_mtlt305Package.I_CAN_AGMO_MTLT305FACTORY: {
				ICan_agmo_mtlt305Factory iCan_agmo_mtlt305Factory = (ICan_agmo_mtlt305Factory) theEObject;
				T result = caseICan_agmo_mtlt305Factory(iCan_agmo_mtlt305Factory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseICAN_AGMO_MTLT305(ICAN_AGMO_MTLT305 object) {
		return null;
	}

	public T caseCAN_AGMO_MTLT305(ICAN_AGMO_MTLT305 object) {
		return null;
	}

	public T caseICAN_AGMO_MTLT305Provider(ICAN_AGMO_MTLT305Provider object) {
		return null;
	}

	public T caseCAN_AGMO_MTLT305Provider(ICAN_AGMO_MTLT305Provider object) {
		return null;
	}

	public T caseICan_agmo_mtlt305Factory(ICan_agmo_mtlt305Factory object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //Can_agmo_mtlt305Switch
