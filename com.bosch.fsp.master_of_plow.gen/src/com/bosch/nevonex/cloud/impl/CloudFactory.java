/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud.impl;

import com.bosch.nevonex.cloud.ICloudFactory;
import com.bosch.nevonex.cloud.ICloud;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class CloudFactory extends EFactoryImpl implements ICloudFactory {
	public static final CloudFactory eINSTANCE = init();

	public static CloudFactory init() {
		try {
			CloudFactory theCloudFactory = (CloudFactory) EPackage.Registry.INSTANCE.getEFactory(CloudPackage.eNS_URI);
			if (theCloudFactory != null) {
				return theCloudFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new CloudFactory();
	}

	public CloudFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case CloudPackage.CLOUD:
			return createCloud();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ICloud createCloud() {
		return Cloud.getInstance();
	}

	public CloudPackage getCloudPackage() {
		return (CloudPackage) getEPackage();
	}

	@Deprecated
	public static CloudPackage getPackage() {
		return CloudPackage.eINSTANCE;
	}

} //CloudFactory
