/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.impl;

import com.bosch.nevonex.customui.ICustomuiFactory;
import com.bosch.nevonex.customui.IUIWebServiceProvider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class CustomuiFactory extends EFactoryImpl implements ICustomuiFactory {
	public static final CustomuiFactory eINSTANCE = init();

	public static CustomuiFactory init() {
		try {
			CustomuiFactory theCustomuiFactory = (CustomuiFactory) EPackage.Registry.INSTANCE.getEFactory(CustomuiPackage.eNS_URI);
			if (theCustomuiFactory != null) {
				return theCustomuiFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new CustomuiFactory();
	}

	public CustomuiFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case CustomuiPackage.UI_WEB_SERVICE_PROVIDER:
			return createUIWebServiceProvider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IUIWebServiceProvider createUIWebServiceProvider() {
		return UIWebServiceProvider.getInstance();
	}

	public CustomuiPackage getCustomuiPackage() {
		return (CustomuiPackage) getEPackage();
	}

	@Deprecated
	public static CustomuiPackage getPackage() {
		return CustomuiPackage.eINSTANCE;
	}

} //CustomuiFactory
