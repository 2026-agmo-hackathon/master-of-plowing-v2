/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn.util;

import com.bosch.nevonex.isopgn.IISOPGN;
import com.bosch.nevonex.isopgn.IISOPGNProvider;
import com.bosch.nevonex.isopgn.IIsopgnFactory;

import com.bosch.nevonex.isopgn.impl.IsopgnPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.common.ITopicObject;

/**
 * @generated
 */
public class IsopgnSwitch<T> extends Switch<T> {
	protected static IsopgnPackage modelPackage;

	public IsopgnSwitch() {
		if (modelPackage == null) {
			modelPackage = IsopgnPackage.eINSTANCE;
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
			case IsopgnPackage.IISOPGN: {
				IISOPGN iISOPGN = (IISOPGN) theEObject;
				T result = caseIISOPGN(iISOPGN);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case IsopgnPackage.ISOPGN: {
				IISOPGN iSOPGN = (IISOPGN) theEObject;
				T result = caseISOPGN(iSOPGN);
				if (result == null)
					result = caseTopicObject((ITopicObject) iSOPGN);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case IsopgnPackage.IISOPGN_PROVIDER: {
				IISOPGNProvider iISOPGNProvider = (IISOPGNProvider) theEObject;
				T result = caseIISOPGNProvider(iISOPGNProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case IsopgnPackage.ISOPGN_PROVIDER: {
				IISOPGNProvider iSOPGNProvider = (IISOPGNProvider) theEObject;
				T result = caseISOPGNProvider(iSOPGNProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case IsopgnPackage.I_ISOPGN_FACTORY: {
				IIsopgnFactory iIsopgnFactory = (IIsopgnFactory) theEObject;
				T result = caseIIsopgnFactory(iIsopgnFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIISOPGN(IISOPGN object) {
		return null;
	}

	public T caseISOPGN(IISOPGN object) {
		return null;
	}

	public T caseIISOPGNProvider(IISOPGNProvider object) {
		return null;
	}

	public T caseISOPGNProvider(IISOPGNProvider object) {
		return null;
	}

	public T caseIIsopgnFactory(IIsopgnFactory object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //IsopgnSwitch
