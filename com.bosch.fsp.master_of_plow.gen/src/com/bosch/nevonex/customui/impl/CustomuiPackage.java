/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.impl;

import com.bosch.nevonex.customui.IAbstractWebsocketEndPoint;
import com.bosch.nevonex.customui.INevonexRoute;
import com.bosch.nevonex.customui.IUIWebServiceProvider;
import com.bosch.nevonex.customui.ICustomuiFactory;
import com.bosch.nevonex.customui.impl.CustomuiFactory;

import org.eclipse.emf.ecore.EAttribute;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EFactory;
import org.eclipse.emf.ecore.EOperation;
import org.eclipse.emf.ecore.EPackage;
import org.eclipse.emf.ecore.EParameter;
import org.eclipse.emf.ecore.EReference;
import org.eclipse.emf.ecore.EcoreFactory;
import org.eclipse.emf.ecore.EcorePackage;

import org.eclipse.emf.ecore.impl.EPackageImpl;

/**
 * @generated
 */
public class CustomuiPackage extends EPackageImpl {
	public static final String eNAME = "customui";
	public static final String eNS_URI = "com.bosch.nevonex.customui";
	public static final String eNS_PREFIX = "customui";
	public static final CustomuiPackage eINSTANCE = com.bosch.nevonex.customui.impl.CustomuiPackage.init();

	public static final int I_ABSTRACT_WEBSOCKET_END_POINT = 0;
	public static final int I_ABSTRACT_WEBSOCKET_END_POINT_FEATURE_COUNT = 0;
	public static final int I_ABSTRACT_WEBSOCKET_END_POINT_OPERATION_COUNT = 0;

	public static final int ABSTRACT_WEBSOCKET_END_POINT = 1;
	public static final int ABSTRACT_WEBSOCKET_END_POINT_FEATURE_COUNT = 0;
	public static final int ABSTRACT_WEBSOCKET_END_POINT_OPERATION_COUNT = 0;

	public static final int I_NEVONEX_ROUTE = 2;
	public static final int I_NEVONEX_ROUTE_FEATURE_COUNT = 0;
	public static final int I_NEVONEX_ROUTE_OPERATION_COUNT = 0;

	public static final int NEVONEX_ROUTE = 3;
	public static final int NEVONEX_ROUTE_FEATURE_COUNT = 0;
	public static final int NEVONEX_ROUTE_OPERATION_COUNT = 0;

	public static final int IUI_WEB_SERVICE_PROVIDER = 4;
	public static final int IUI_WEB_SERVICE_PROVIDER_FEATURE_COUNT = 0;
	public static final int IUI_WEB_SERVICE_PROVIDER_OPERATION_COUNT = 0;

	public static final int UI_WEB_SERVICE_PROVIDER = 5;
	public static final int UI_WEB_SERVICE_PROVIDER_FEATURE_COUNT = 0;
	public static final int UI_WEB_SERVICE_PROVIDER_OPERATION_COUNT = 0;

	public static final int I_CUSTOMUI_FACTORY = 6;
	public static final int I_CUSTOMUI_FACTORY_FEATURE_COUNT = 0;
	public static final int I_CUSTOMUI_FACTORY_OPERATION_COUNT = 0;


	private EClass iAbstractWebsocketEndPointEClass = null;
	private EClass abstractWebsocketEndPointEClass = null;
	private EClass iNevonexRouteEClass = null;
	private EClass nevonexRouteEClass = null;
	private EClass iUIWebServiceProviderEClass = null;
	private EClass uIWebServiceProviderEClass = null;
	private EClass iCustomuiFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public CustomuiPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(CustomuiFactory.eINSTANCE);
	}

	public static CustomuiPackage init() {
		if (isInited) return (CustomuiPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		CustomuiPackage thePackage = (registered instanceof CustomuiPackage) ? (CustomuiPackage) registered : new CustomuiPackage();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
		thePackage.createPackageContents();
		thePackage.initializePackageContents(thePackage);
		thePackage.freeze();
		EPackage.Registry.INSTANCE.put(eNS_URI, thePackage);
		return thePackage;
	}

	private void createPackageContents() {
		if (isCreated) return;
		isCreated = true;
		createPackageContents_part0();
	}

	private void createPackageContents_part0() {
		iAbstractWebsocketEndPointEClass = createEClass(I_ABSTRACT_WEBSOCKET_END_POINT);

		abstractWebsocketEndPointEClass = createEClass(ABSTRACT_WEBSOCKET_END_POINT);

		iNevonexRouteEClass = createEClass(I_NEVONEX_ROUTE);

		nevonexRouteEClass = createEClass(NEVONEX_ROUTE);

		iUIWebServiceProviderEClass = createEClass(IUI_WEB_SERVICE_PROVIDER);

		uIWebServiceProviderEClass = createEClass(UI_WEB_SERVICE_PROVIDER);

		iCustomuiFactoryEClass = createEClass(I_CUSTOMUI_FACTORY);

	}

	private void initializePackageContents(CustomuiPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(CustomuiPackage thePackage) {
		thePackage.iAbstractWebsocketEndPointEClass.setName("IAbstractWebsocketEndPoint");
		thePackage.iAbstractWebsocketEndPointEClass.setInstanceClassName("com.bosch.nevonex.customui.IAbstractWebsocketEndPoint");
		thePackage.iAbstractWebsocketEndPointEClass.setAbstract(true);
		thePackage.iAbstractWebsocketEndPointEClass.setInterface(true);
		thePackage.abstractWebsocketEndPointEClass.setName("AbstractWebsocketEndPoint");
		thePackage.abstractWebsocketEndPointEClass.setInstanceClassName("com.bosch.nevonex.customui.IAbstractWebsocketEndPoint");
		thePackage.abstractWebsocketEndPointEClass.setAbstract(false);
		thePackage.abstractWebsocketEndPointEClass.setInterface(false);
		thePackage.iNevonexRouteEClass.setName("INevonexRoute");
		thePackage.iNevonexRouteEClass.setInstanceClassName("com.bosch.nevonex.customui.INevonexRoute");
		thePackage.iNevonexRouteEClass.setAbstract(true);
		thePackage.iNevonexRouteEClass.setInterface(true);
		thePackage.nevonexRouteEClass.setName("NevonexRoute");
		thePackage.nevonexRouteEClass.setInstanceClassName("com.bosch.nevonex.customui.INevonexRoute");
		thePackage.nevonexRouteEClass.setAbstract(false);
		thePackage.nevonexRouteEClass.setInterface(false);
		thePackage.iUIWebServiceProviderEClass.setName("IUIWebServiceProvider");
		thePackage.iUIWebServiceProviderEClass.setInstanceClassName("com.bosch.nevonex.customui.IUIWebServiceProvider");
		thePackage.iUIWebServiceProviderEClass.setAbstract(true);
		thePackage.iUIWebServiceProviderEClass.setInterface(true);
		thePackage.uIWebServiceProviderEClass.setName("UIWebServiceProvider");
		thePackage.uIWebServiceProviderEClass.setInstanceClassName("com.bosch.nevonex.customui.IUIWebServiceProvider");
		thePackage.uIWebServiceProviderEClass.setAbstract(false);
		thePackage.uIWebServiceProviderEClass.setInterface(false);
		thePackage.iCustomuiFactoryEClass.setName("ICustomuiFactory");
		thePackage.iCustomuiFactoryEClass.setInstanceClassName("com.bosch.nevonex.customui.ICustomuiFactory");
		thePackage.iCustomuiFactoryEClass.setAbstract(true);
		thePackage.iCustomuiFactoryEClass.setInterface(true);
	}

	public EClass getIAbstractWebsocketEndPoint() { return iAbstractWebsocketEndPointEClass; }
	public EClass getAbstractWebsocketEndPoint() { return abstractWebsocketEndPointEClass; }
	public EClass getINevonexRoute() { return iNevonexRouteEClass; }
	public EClass getNevonexRoute() { return nevonexRouteEClass; }
	public EClass getIUIWebServiceProvider() { return iUIWebServiceProviderEClass; }
	public EClass getUIWebServiceProvider() { return uIWebServiceProviderEClass; }
	public EClass getICustomuiFactory() { return iCustomuiFactoryEClass; }

	public ICustomuiFactory getCustomuiFactory() {
		return (ICustomuiFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass I_ABSTRACT_WEBSOCKET_END_POINT = eINSTANCE.getIAbstractWebsocketEndPoint();
		public static final EClass ABSTRACT_WEBSOCKET_END_POINT = eINSTANCE.getAbstractWebsocketEndPoint();
		public static final EClass I_NEVONEX_ROUTE = eINSTANCE.getINevonexRoute();
		public static final EClass NEVONEX_ROUTE = eINSTANCE.getNevonexRoute();
		public static final EClass IUI_WEB_SERVICE_PROVIDER = eINSTANCE.getIUIWebServiceProvider();
		public static final EClass UI_WEB_SERVICE_PROVIDER = eINSTANCE.getUIWebServiceProvider();
		public static final EClass I_CUSTOMUI_FACTORY = eINSTANCE.getICustomuiFactory();
	}

} //CustomuiPackage
