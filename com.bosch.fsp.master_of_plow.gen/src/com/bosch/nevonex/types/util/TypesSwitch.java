/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types.util;

import com.bosch.nevonex.types.IArrayType;
import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.ITypesFactory;

import com.bosch.nevonex.types.impl.TypesPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;

/**
 * @generated
 */
public class TypesSwitch<T> extends Switch<T> {
	protected static TypesPackage modelPackage;

	public TypesSwitch() {
		if (modelPackage == null) {
			modelPackage = TypesPackage.eINSTANCE;
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
			case TypesPackage.I_ARRAY_TYPE: {
				IArrayType iArrayType = (IArrayType) theEObject;
				T result = caseIArrayType(iArrayType);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case TypesPackage.I_PROPERTY_CHANGE: {
				IPropertyChange iPropertyChange = (IPropertyChange) theEObject;
				T result = caseIPropertyChange(iPropertyChange);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case TypesPackage.I_TYPES_FACTORY: {
				ITypesFactory iTypesFactory = (ITypesFactory) theEObject;
				T result = caseITypesFactory(iTypesFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case TypesPackage.PROPERTY_CHANGE: {
				IPropertyChange propertyChange = (IPropertyChange) theEObject;
				T result = casePropertyChange(propertyChange);
				if (result == null)
					result = caseIPropertyChange((IPropertyChange) propertyChange);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIArrayType(IArrayType object) {
		return null;
	}

	public T caseIPropertyChange(IPropertyChange object) {
		return null;
	}

	public T caseITypesFactory(ITypesFactory object) {
		return null;
	}

	public T casePropertyChange(IPropertyChange object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //TypesSwitch
