/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor.impl;

import com.bosch.nevonex.can_agmo_customized_tractor.ICan_agmo_customized_tractorFactory;
import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor;
import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_TractorProvider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class Can_agmo_customized_tractorFactory extends EFactoryImpl implements ICan_agmo_customized_tractorFactory {
	public static final Can_agmo_customized_tractorFactory eINSTANCE = init();

	public static Can_agmo_customized_tractorFactory init() {
		try {
			Can_agmo_customized_tractorFactory theCan_agmo_customized_tractorFactory = (Can_agmo_customized_tractorFactory) EPackage.Registry.INSTANCE.getEFactory(Can_agmo_customized_tractorPackage.eNS_URI);
			if (theCan_agmo_customized_tractorFactory != null) {
				return theCan_agmo_customized_tractorFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new Can_agmo_customized_tractorFactory();
	}

	public Can_agmo_customized_tractorFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR:
			return createCAN_AGMO_Customized_Tractor();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER:
			return createCAN_AGMO_Customized_TractorProvider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ICAN_AGMO_Customized_Tractor createCAN_AGMO_Customized_Tractor() {
		CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor = new CAN_AGMO_Customized_Tractor();
		return cAN_AGMO_Customized_Tractor;
	}

	public ICAN_AGMO_Customized_TractorProvider createCAN_AGMO_Customized_TractorProvider() {
		CAN_AGMO_Customized_TractorProvider cAN_AGMO_Customized_TractorProvider = new CAN_AGMO_Customized_TractorProvider();
		return cAN_AGMO_Customized_TractorProvider;
	}

	public Can_agmo_customized_tractorPackage getCan_agmo_customized_tractorPackage() {
		return (Can_agmo_customized_tractorPackage) getEPackage();
	}

	@Deprecated
	public static Can_agmo_customized_tractorPackage getPackage() {
		return Can_agmo_customized_tractorPackage.eINSTANCE;
	}

} //Can_agmo_customized_tractorFactory
