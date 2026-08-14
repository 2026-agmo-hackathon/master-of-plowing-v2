/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor.util;

import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor;
import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider;
import com.bosch.nevonex.can_agmo_customized_tractor.ICan_agmo_customized_tractorFactory;

import com.bosch.nevonex.can_agmo_customized_tractor.impl.Can_agmo_customized_tractorPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.common.ITopicObject;

/**
 * @generated
 */
public class Can_agmo_customized_tractorSwitch<T> extends Switch<T> {
	protected static Can_agmo_customized_tractorPackage modelPackage;

	public Can_agmo_customized_tractorSwitch() {
		if (modelPackage == null) {
			modelPackage = Can_agmo_customized_tractorPackage.eINSTANCE;
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
			case Can_agmo_customized_tractorPackage.ICAN_AGMO_CUSTOMIZED_TRACTOR: {
				ICAN_AGMO_Customized_Tractor iCAN_AGMO_Customized_Tractor = (ICAN_AGMO_Customized_Tractor) theEObject;
				T result = caseICAN_AGMO_Customized_Tractor(iCAN_AGMO_Customized_Tractor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR: {
				ICAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor = (ICAN_AGMO_Customized_Tractor) theEObject;
				T result = caseCAN_AGMO_Customized_Tractor(cAN_AGMO_Customized_Tractor);
				if (result == null)
					result = caseTopicObject((ITopicObject) cAN_AGMO_Customized_Tractor);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_customized_tractorPackage.ICAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER: {
				ICAN_AGMO_Customized_TractorProvider iCAN_AGMO_Customized_TractorProvider = (ICAN_AGMO_Customized_TractorProvider) theEObject;
				T result = caseICAN_AGMO_Customized_TractorProvider(iCAN_AGMO_Customized_TractorProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER: {
				ICAN_AGMO_Customized_TractorProvider cAN_AGMO_Customized_TractorProvider = (ICAN_AGMO_Customized_TractorProvider) theEObject;
				T result = caseCAN_AGMO_Customized_TractorProvider(cAN_AGMO_Customized_TractorProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case Can_agmo_customized_tractorPackage.I_CAN_AGMO_CUSTOMIZED_TRACTOR_FACTORY: {
				ICan_agmo_customized_tractorFactory iCan_agmo_customized_tractorFactory = (ICan_agmo_customized_tractorFactory) theEObject;
				T result = caseICan_agmo_customized_tractorFactory(iCan_agmo_customized_tractorFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseICAN_AGMO_Customized_Tractor(ICAN_AGMO_Customized_Tractor object) {
		return null;
	}

	public T caseCAN_AGMO_Customized_Tractor(ICAN_AGMO_Customized_Tractor object) {
		return null;
	}

	public T caseICAN_AGMO_Customized_TractorProvider(ICAN_AGMO_Customized_TractorProvider object) {
		return null;
	}

	public T caseCAN_AGMO_Customized_TractorProvider(ICAN_AGMO_Customized_TractorProvider object) {
		return null;
	}

	public T caseICan_agmo_customized_tractorFactory(ICan_agmo_customized_tractorFactory object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //Can_agmo_customized_tractorSwitch
