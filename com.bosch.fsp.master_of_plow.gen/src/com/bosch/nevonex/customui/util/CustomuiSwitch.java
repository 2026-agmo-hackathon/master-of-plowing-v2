/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.util;

import com.bosch.nevonex.customui.IAbstractWebsocketEndPoint;
import com.bosch.nevonex.customui.INevonexRoute;
import com.bosch.nevonex.customui.IUIWebServiceProvider;
import com.bosch.nevonex.customui.ICustomuiFactory;

import com.bosch.nevonex.customui.impl.CustomuiPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;

/**
 * @generated
 */
public class CustomuiSwitch<T> extends Switch<T> {
	protected static CustomuiPackage modelPackage;

	public CustomuiSwitch() {
		if (modelPackage == null) {
			modelPackage = CustomuiPackage.eINSTANCE;
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
			case CustomuiPackage.I_ABSTRACT_WEBSOCKET_END_POINT: {
				IAbstractWebsocketEndPoint iAbstractWebsocketEndPoint = (IAbstractWebsocketEndPoint) theEObject;
				T result = caseIAbstractWebsocketEndPoint(iAbstractWebsocketEndPoint);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.ABSTRACT_WEBSOCKET_END_POINT: {
				IAbstractWebsocketEndPoint abstractWebsocketEndPoint = (IAbstractWebsocketEndPoint) theEObject;
				T result = caseAbstractWebsocketEndPoint(abstractWebsocketEndPoint);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.I_NEVONEX_ROUTE: {
				INevonexRoute iNevonexRoute = (INevonexRoute) theEObject;
				T result = caseINevonexRoute(iNevonexRoute);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.NEVONEX_ROUTE: {
				INevonexRoute nevonexRoute = (INevonexRoute) theEObject;
				T result = caseNevonexRoute(nevonexRoute);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.IUI_WEB_SERVICE_PROVIDER: {
				IUIWebServiceProvider iUIWebServiceProvider = (IUIWebServiceProvider) theEObject;
				T result = caseIUIWebServiceProvider(iUIWebServiceProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.UI_WEB_SERVICE_PROVIDER: {
				IUIWebServiceProvider uIWebServiceProvider = (IUIWebServiceProvider) theEObject;
				T result = caseUIWebServiceProvider(uIWebServiceProvider);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CustomuiPackage.I_CUSTOMUI_FACTORY: {
				ICustomuiFactory iCustomuiFactory = (ICustomuiFactory) theEObject;
				T result = caseICustomuiFactory(iCustomuiFactory);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseIAbstractWebsocketEndPoint(IAbstractWebsocketEndPoint object) {
		return null;
	}

	public T caseAbstractWebsocketEndPoint(IAbstractWebsocketEndPoint object) {
		return null;
	}

	public T caseINevonexRoute(INevonexRoute object) {
		return null;
	}

	public T caseNevonexRoute(INevonexRoute object) {
		return null;
	}

	public T caseIUIWebServiceProvider(IUIWebServiceProvider object) {
		return null;
	}

	public T caseUIWebServiceProvider(IUIWebServiceProvider object) {
		return null;
	}

	public T caseICustomuiFactory(ICustomuiFactory object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //CustomuiSwitch
