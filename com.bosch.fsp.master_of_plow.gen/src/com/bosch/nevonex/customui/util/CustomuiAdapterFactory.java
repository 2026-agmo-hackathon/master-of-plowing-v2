/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.util;

import com.bosch.nevonex.customui.IAbstractWebsocketEndPoint;
import com.bosch.nevonex.customui.INevonexRoute;
import com.bosch.nevonex.customui.IUIWebServiceProvider;
import com.bosch.nevonex.customui.ICustomuiFactory;

import com.bosch.nevonex.customui.impl.CustomuiPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class CustomuiAdapterFactory extends AdapterFactoryImpl {
	protected static CustomuiPackage modelPackage;

	public CustomuiAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = CustomuiPackage.eINSTANCE;
		}
	}

	@Override
	public boolean isFactoryForType(Object object) {
		if (object == modelPackage) {
			return true;
		}
		if (object instanceof EObject) {
			return ((EObject) object).eClass().getEPackage() == modelPackage;
		}
		return false;
	}

	protected CustomuiSwitch<Adapter> modelSwitch = new CustomuiSwitch<Adapter>() {
		@Override
		public Adapter caseIAbstractWebsocketEndPoint(IAbstractWebsocketEndPoint object) {
			return createIAbstractWebsocketEndPointAdapter();
		}

		@Override
		public Adapter caseAbstractWebsocketEndPoint(IAbstractWebsocketEndPoint object) {
			return createAbstractWebsocketEndPointAdapter();
		}

		@Override
		public Adapter caseINevonexRoute(INevonexRoute object) {
			return createINevonexRouteAdapter();
		}

		@Override
		public Adapter caseNevonexRoute(INevonexRoute object) {
			return createNevonexRouteAdapter();
		}

		@Override
		public Adapter caseIUIWebServiceProvider(IUIWebServiceProvider object) {
			return createIUIWebServiceProviderAdapter();
		}

		@Override
		public Adapter caseUIWebServiceProvider(IUIWebServiceProvider object) {
			return createUIWebServiceProviderAdapter();
		}

		@Override
		public Adapter caseICustomuiFactory(ICustomuiFactory object) {
			return createICustomuiFactoryAdapter();
		}

		@Override
		public Adapter defaultCase(EObject object) {
			return createEObjectAdapter();
		}
	};

	@Override
	public Adapter createAdapter(Notifier target) {
		return modelSwitch.doSwitch((EObject) target);
	}

	public Adapter createIAbstractWebsocketEndPointAdapter() {
		return null;
	}

	public Adapter createAbstractWebsocketEndPointAdapter() {
		return null;
	}

	public Adapter createINevonexRouteAdapter() {
		return null;
	}

	public Adapter createNevonexRouteAdapter() {
		return null;
	}

	public Adapter createIUIWebServiceProviderAdapter() {
		return null;
	}

	public Adapter createUIWebServiceProviderAdapter() {
		return null;
	}

	public Adapter createICustomuiFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //CustomuiAdapterFactory
