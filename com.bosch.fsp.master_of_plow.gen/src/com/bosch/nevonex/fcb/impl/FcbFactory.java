/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb.impl;

import com.bosch.nevonex.fcb.IFcbFactory;
import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.ISubscribeConnectionFactory;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class FcbFactory extends EFactoryImpl implements IFcbFactory {
	public static final FcbFactory eINSTANCE = init();

	public static FcbFactory init() {
		try {
			FcbFactory theFcbFactory = (FcbFactory) EPackage.Registry.INSTANCE.getEFactory(FcbPackage.eNS_URI);
			if (theFcbFactory != null) {
				return theFcbFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new FcbFactory();
	}

	public FcbFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case FcbPackage.FCAL_CONTROLLER:
			return createFCALController();
		case FcbPackage.SUBSCRIBE_CONNECTION_FACTORY:
			return createSubscribeConnectionFactory();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IFCALController createFCALController() {
		FCALController fCALController = new FCALController();
		return fCALController;
	}

	public ISubscribeConnectionFactory createSubscribeConnectionFactory() {
		SubscribeConnectionFactory subscribeConnectionFactory = new SubscribeConnectionFactory();
		return subscribeConnectionFactory;
	}

	public FcbPackage getFcbPackage() {
		return (FcbPackage) getEPackage();
	}

	@Deprecated
	public static FcbPackage getPackage() {
		return FcbPackage.eINSTANCE;
	}

} //FcbFactory
