/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb.util;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.IConnectionFactory;
import com.bosch.nevonex.fcb.IPublishConnectionFactory;
import com.bosch.nevonex.fcb.ISubscribeConnectionFactory;
import com.bosch.nevonex.fcb.IFcbFactory;

import com.bosch.nevonex.fcb.impl.FcbPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;

/**
 * @generated
 */
public class FcbSwitch<T> extends Switch<T> {
	protected static FcbPackage modelPackage;

	public FcbSwitch() {
		if (modelPackage == null) {
			modelPackage = FcbPackage.eINSTANCE;
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
			case FcbPackage.IFCAL_CONTROLLER: {
				IFCALController iFCALController = (IFCALController) theEObject;
				T result = caseIFCALController(iFCALController);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.FCAL_CONTROLLER: {
				IFCALController fCALController = (IFCALController) theEObject;
				T result = caseFCALController(fCALController);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.I_CONNECTION_FACTORY: {
				IConnectionFactory iConnectionFactory = (IConnectionFactory) theEObject;
				T result = caseIConnectionFactory(iConnectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.CONNECTION_FACTORY: {
				IConnectionFactory connectionFactory = (IConnectionFactory) theEObject;
				T result = caseConnectionFactory(connectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.I_PUBLISH_CONNECTION_FACTORY: {
				IPublishConnectionFactory iPublishConnectionFactory = (IPublishConnectionFactory) theEObject;
				T result = caseIPublishConnectionFactory(iPublishConnectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.PUBLISH_CONNECTION_FACTORY: {
				IPublishConnectionFactory publishConnectionFactory = (IPublishConnectionFactory) theEObject;
				T result = casePublishConnectionFactory(publishConnectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.I_SUBSCRIBE_CONNECTION_FACTORY: {
				ISubscribeConnectionFactory iSubscribeConnectionFactory = (ISubscribeConnectionFactory) theEObject;
				T result = caseISubscribeConnectionFactory(iSubscribeConnectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.SUBSCRIBE_CONNECTION_FACTORY: {
				ISubscribeConnectionFactory subscribeConnectionFactory = (ISubscribeConnectionFactory) theEObject;
				T result = caseSubscribeConnectionFactory(subscribeConnectionFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case FcbPackage.I_FCB_FACTORY: {
				IFcbFactory iFcbFactory = (IFcbFactory) theEObject;
				T result = caseIFcbFactory(iFcbFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIFCALController(IFCALController object) {
		return null;
	}

	public T caseFCALController(IFCALController object) {
		return null;
	}

	public T caseIConnectionFactory(IConnectionFactory object) {
		return null;
	}

	public T caseConnectionFactory(IConnectionFactory object) {
		return null;
	}

	public T caseIPublishConnectionFactory(IPublishConnectionFactory object) {
		return null;
	}

	public T casePublishConnectionFactory(IPublishConnectionFactory object) {
		return null;
	}

	public T caseISubscribeConnectionFactory(ISubscribeConnectionFactory object) {
		return null;
	}

	public T caseSubscribeConnectionFactory(ISubscribeConnectionFactory object) {
		return null;
	}

	public T caseIFcbFactory(IFcbFactory object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //FcbSwitch
