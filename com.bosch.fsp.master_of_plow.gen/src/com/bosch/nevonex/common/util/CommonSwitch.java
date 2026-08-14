/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.util;

import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.ICommonFactory;

import com.bosch.nevonex.common.impl.CommonPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;

/**
 * @generated
 */
public class CommonSwitch<T> extends Switch<T> {
	protected static CommonPackage modelPackage;

	public CommonSwitch() {
		if (modelPackage == null) {
			modelPackage = CommonPackage.eINSTANCE;
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
			case CommonPackage.I_ABSOLUTE_POSITION: {
				IAbsolutePosition iAbsolutePosition = (IAbsolutePosition) theEObject;
				T result = caseIAbsolutePosition(iAbsolutePosition);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CommonPackage.I_TOPIC_OBJECT: {
				ITopicObject iTopicObject = (ITopicObject) theEObject;
				T result = caseITopicObject(iTopicObject);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CommonPackage.ABSOLUTE_POSITION: {
				IAbsolutePosition absolutePosition = (IAbsolutePosition) theEObject;
				T result = caseAbsolutePosition(absolutePosition);
				if (result == null)
					result = caseIAbsolutePosition((IAbsolutePosition) absolutePosition);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CommonPackage.TOPIC_OBJECT: {
				ITopicObject topicObject = (ITopicObject) theEObject;
				T result = caseTopicObject(topicObject);
				if (result == null)
					result = caseITopicObject((ITopicObject) topicObject);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CommonPackage.I_COMMON_FACTORY: {
				ICommonFactory iCommonFactory = (ICommonFactory) theEObject;
				T result = caseICommonFactory(iCommonFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIAbsolutePosition(IAbsolutePosition object) {
		return null;
	}

	public T caseITopicObject(ITopicObject object) {
		return null;
	}

	public T caseAbsolutePosition(IAbsolutePosition object) {
		return null;
	}

	public T caseTopicObject(ITopicObject object) {
		return null;
	}

	public T caseICommonFactory(ICommonFactory object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //CommonSwitch
