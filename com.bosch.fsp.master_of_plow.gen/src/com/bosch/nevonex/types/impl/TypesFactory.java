/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types.impl;

import com.bosch.nevonex.types.ITypesFactory;
import com.bosch.nevonex.types.IPropertyChange;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class TypesFactory extends EFactoryImpl implements ITypesFactory {
	public static final TypesFactory eINSTANCE = init();

	public static TypesFactory init() {
		try {
			TypesFactory theTypesFactory = (TypesFactory) EPackage.Registry.INSTANCE.getEFactory(TypesPackage.eNS_URI);
			if (theTypesFactory != null) {
				return theTypesFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new TypesFactory();
	}

	public TypesFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case TypesPackage.PROPERTY_CHANGE:
			return createPropertyChange();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IPropertyChange createPropertyChange() {
		PropertyChange propertyChange = new PropertyChange();
		return propertyChange;
	}

	public TypesPackage getTypesPackage() {
		return (TypesPackage) getEPackage();
	}

	@Deprecated
	public static TypesPackage getPackage() {
		return TypesPackage.eINSTANCE;
	}

} //TypesFactory
