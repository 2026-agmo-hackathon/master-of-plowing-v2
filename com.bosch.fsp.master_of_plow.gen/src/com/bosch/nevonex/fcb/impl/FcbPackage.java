/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb.impl;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.IConnectionFactory;
import com.bosch.nevonex.fcb.IPublishConnectionFactory;
import com.bosch.nevonex.fcb.ISubscribeConnectionFactory;
import com.bosch.nevonex.fcb.IFcbFactory;
import com.bosch.nevonex.fcb.impl.FcbFactory;

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
public class FcbPackage extends EPackageImpl {
	public static final String eNAME = "fcb";
	public static final String eNS_URI = "com.bosch.nevonex.fcb";
	public static final String eNS_PREFIX = "fcb";
	public static final FcbPackage eINSTANCE = com.bosch.nevonex.fcb.impl.FcbPackage.init();

	public static final int IFCAL_CONTROLLER = 0;
	public static final int IFCAL_CONTROLLER_FEATURE_COUNT = 0;
	public static final int IFCAL_CONTROLLER___SET_MACHINE_UPDATE__IEOBJECT__STRING__IEJAVAOBJECT__IEJAVAOBJECT__LONG = 0;
	public static final int IFCAL_CONTROLLER___PUBLISH__STRING__STRING__IEJAVAOBJECT__SUBSCRIBERENUM = 1;
	public static final int IFCAL_CONTROLLER_OPERATION_COUNT = 2;

	public static final int FCAL_CONTROLLER = 1;
	public static final int FCAL_CONTROLLER_FEATURE_COUNT = 0;
	public static final int FCAL_CONTROLLER_OPERATION_COUNT = 0;

	public static final int I_CONNECTION_FACTORY = 2;
	public static final int I_CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int I_CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int CONNECTION_FACTORY = 3;
	public static final int CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int I_PUBLISH_CONNECTION_FACTORY = 4;
	public static final int I_PUBLISH_CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int I_PUBLISH_CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int PUBLISH_CONNECTION_FACTORY = 5;
	public static final int PUBLISH_CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int PUBLISH_CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int I_SUBSCRIBE_CONNECTION_FACTORY = 6;
	public static final int I_SUBSCRIBE_CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int I_SUBSCRIBE_CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int SUBSCRIBE_CONNECTION_FACTORY = 7;
	public static final int SUBSCRIBE_CONNECTION_FACTORY_FEATURE_COUNT = 0;
	public static final int SUBSCRIBE_CONNECTION_FACTORY_OPERATION_COUNT = 0;

	public static final int I_FCB_FACTORY = 8;
	public static final int I_FCB_FACTORY_FEATURE_COUNT = 0;
	public static final int I_FCB_FACTORY_OPERATION_COUNT = 0;


	private EClass iFCALControllerEClass = null;
	private EClass fCALControllerEClass = null;
	private EClass iConnectionFactoryEClass = null;
	private EClass connectionFactoryEClass = null;
	private EClass iPublishConnectionFactoryEClass = null;
	private EClass publishConnectionFactoryEClass = null;
	private EClass iSubscribeConnectionFactoryEClass = null;
	private EClass subscribeConnectionFactoryEClass = null;
	private EClass iFcbFactoryEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public FcbPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(FcbFactory.eINSTANCE);
	}

	public static FcbPackage init() {
		if (isInited) return (FcbPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		FcbPackage thePackage = (registered instanceof FcbPackage) ? (FcbPackage) registered : new FcbPackage();
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
		iFCALControllerEClass = createEClass(IFCAL_CONTROLLER);
		createEOperation(iFCALControllerEClass, IFCAL_CONTROLLER___SET_MACHINE_UPDATE__IEOBJECT__STRING__IEJAVAOBJECT__IEJAVAOBJECT__LONG);
		createEOperation(iFCALControllerEClass, IFCAL_CONTROLLER___PUBLISH__STRING__STRING__IEJAVAOBJECT__SUBSCRIBERENUM);

		fCALControllerEClass = createEClass(FCAL_CONTROLLER);

		iConnectionFactoryEClass = createEClass(I_CONNECTION_FACTORY);

		connectionFactoryEClass = createEClass(CONNECTION_FACTORY);

		iPublishConnectionFactoryEClass = createEClass(I_PUBLISH_CONNECTION_FACTORY);

		publishConnectionFactoryEClass = createEClass(PUBLISH_CONNECTION_FACTORY);

		iSubscribeConnectionFactoryEClass = createEClass(I_SUBSCRIBE_CONNECTION_FACTORY);

		subscribeConnectionFactoryEClass = createEClass(SUBSCRIBE_CONNECTION_FACTORY);

		iFcbFactoryEClass = createEClass(I_FCB_FACTORY);

	}

	private void initializePackageContents(FcbPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(FcbPackage thePackage) {
		thePackage.iFCALControllerEClass.setName("IFCALController");
		thePackage.iFCALControllerEClass.setInstanceClassName("com.bosch.nevonex.fcb.IFCALController");
		thePackage.iFCALControllerEClass.setAbstract(true);
		thePackage.iFCALControllerEClass.setInterface(true);
		thePackage.iFCALControllerEClass.getEOperations().get(0).setName("setMachineUpdate");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("machine");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.iFCALControllerEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("attribute");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.iFCALControllerEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("payload");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.iFCALControllerEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("details");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.iFCALControllerEClass.getEOperations().get(0).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("timestamp");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getELong());
			thePackage.iFCALControllerEClass.getEOperations().get(0).getEParameters().add(param);
		}
		thePackage.iFCALControllerEClass.getEOperations().get(1).setName("publish");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("interfaceKey");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.iFCALControllerEClass.getEOperations().get(1).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("interfaceName");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.iFCALControllerEClass.getEOperations().get(1).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("payloadValue");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.iFCALControllerEClass.getEOperations().get(1).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("subscriber");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.iFCALControllerEClass.getEOperations().get(1).getEParameters().add(param);
		}
		thePackage.fCALControllerEClass.setName("FCALController");
		thePackage.fCALControllerEClass.setInstanceClassName("com.bosch.nevonex.fcb.IFCALController");
		thePackage.fCALControllerEClass.setAbstract(false);
		thePackage.fCALControllerEClass.setInterface(false);
		thePackage.iConnectionFactoryEClass.setName("IConnectionFactory");
		thePackage.iConnectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.IConnectionFactory");
		thePackage.iConnectionFactoryEClass.setAbstract(true);
		thePackage.iConnectionFactoryEClass.setInterface(true);
		thePackage.connectionFactoryEClass.setName("ConnectionFactory");
		thePackage.connectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.IConnectionFactory");
		thePackage.connectionFactoryEClass.setAbstract(false);
		thePackage.connectionFactoryEClass.setInterface(false);
		thePackage.iPublishConnectionFactoryEClass.setName("IPublishConnectionFactory");
		thePackage.iPublishConnectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.IPublishConnectionFactory");
		thePackage.iPublishConnectionFactoryEClass.setAbstract(true);
		thePackage.iPublishConnectionFactoryEClass.setInterface(true);
		thePackage.publishConnectionFactoryEClass.setName("PublishConnectionFactory");
		thePackage.publishConnectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.IPublishConnectionFactory");
		thePackage.publishConnectionFactoryEClass.setAbstract(false);
		thePackage.publishConnectionFactoryEClass.setInterface(false);
		thePackage.iSubscribeConnectionFactoryEClass.setName("ISubscribeConnectionFactory");
		thePackage.iSubscribeConnectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.ISubscribeConnectionFactory");
		thePackage.iSubscribeConnectionFactoryEClass.setAbstract(true);
		thePackage.iSubscribeConnectionFactoryEClass.setInterface(true);
		thePackage.subscribeConnectionFactoryEClass.setName("SubscribeConnectionFactory");
		thePackage.subscribeConnectionFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.ISubscribeConnectionFactory");
		thePackage.subscribeConnectionFactoryEClass.setAbstract(false);
		thePackage.subscribeConnectionFactoryEClass.setInterface(false);
		thePackage.iFcbFactoryEClass.setName("IFcbFactory");
		thePackage.iFcbFactoryEClass.setInstanceClassName("com.bosch.nevonex.fcb.IFcbFactory");
		thePackage.iFcbFactoryEClass.setAbstract(true);
		thePackage.iFcbFactoryEClass.setInterface(true);
	}

	public EClass getIFCALController() { return iFCALControllerEClass; }
	public EClass getFCALController() { return fCALControllerEClass; }
	public EClass getIConnectionFactory() { return iConnectionFactoryEClass; }
	public EClass getConnectionFactory() { return connectionFactoryEClass; }
	public EClass getIPublishConnectionFactory() { return iPublishConnectionFactoryEClass; }
	public EClass getPublishConnectionFactory() { return publishConnectionFactoryEClass; }
	public EClass getISubscribeConnectionFactory() { return iSubscribeConnectionFactoryEClass; }
	public EClass getSubscribeConnectionFactory() { return subscribeConnectionFactoryEClass; }
	public EClass getIFcbFactory() { return iFcbFactoryEClass; }
	public EOperation getIFCALController__SetMachineUpdate__EObject__String__EJavaObject__EJavaObject__long() {
		return iFCALControllerEClass.getEOperations().get(0);
	}
	public EOperation getIFCALController__Publish__String__String__EJavaObject__SubscriberEnum() {
		return iFCALControllerEClass.getEOperations().get(1);
	}

	public IFcbFactory getFcbFactory() {
		return (IFcbFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass IFCAL_CONTROLLER = eINSTANCE.getIFCALController();
		public static final EClass FCAL_CONTROLLER = eINSTANCE.getFCALController();
		public static final EClass I_CONNECTION_FACTORY = eINSTANCE.getIConnectionFactory();
		public static final EClass CONNECTION_FACTORY = eINSTANCE.getConnectionFactory();
		public static final EClass I_PUBLISH_CONNECTION_FACTORY = eINSTANCE.getIPublishConnectionFactory();
		public static final EClass PUBLISH_CONNECTION_FACTORY = eINSTANCE.getPublishConnectionFactory();
		public static final EClass I_SUBSCRIBE_CONNECTION_FACTORY = eINSTANCE.getISubscribeConnectionFactory();
		public static final EClass SUBSCRIBE_CONNECTION_FACTORY = eINSTANCE.getSubscribeConnectionFactory();
		public static final EClass I_FCB_FACTORY = eINSTANCE.getIFcbFactory();
	}

} //FcbPackage
