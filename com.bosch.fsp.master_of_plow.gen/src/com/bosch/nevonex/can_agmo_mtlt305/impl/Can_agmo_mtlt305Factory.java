/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305.impl;

import com.bosch.nevonex.can_agmo_mtlt305.ICan_agmo_mtlt305Factory;
import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305;
import com.bosch.nevonex.can_agmo_mtlt305.ICAN_AGMO_MTLT305Provider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class Can_agmo_mtlt305Factory extends EFactoryImpl implements ICan_agmo_mtlt305Factory {
	public static final Can_agmo_mtlt305Factory eINSTANCE = init();

	public static Can_agmo_mtlt305Factory init() {
		try {
			Can_agmo_mtlt305Factory theCan_agmo_mtlt305Factory = (Can_agmo_mtlt305Factory) EPackage.Registry.INSTANCE.getEFactory(Can_agmo_mtlt305Package.eNS_URI);
			if (theCan_agmo_mtlt305Factory != null) {
				return theCan_agmo_mtlt305Factory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new Can_agmo_mtlt305Factory();
	}

	public Can_agmo_mtlt305Factory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305:
			return createCAN_AGMO_MTLT305();
		case Can_agmo_mtlt305Package.CAN_AGMO_MTLT305PROVIDER:
			return createCAN_AGMO_MTLT305Provider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public ICAN_AGMO_MTLT305 createCAN_AGMO_MTLT305() {
		CAN_AGMO_MTLT305 cAN_AGMO_MTLT305 = new CAN_AGMO_MTLT305();
		return cAN_AGMO_MTLT305;
	}

	public ICAN_AGMO_MTLT305Provider createCAN_AGMO_MTLT305Provider() {
		CAN_AGMO_MTLT305Provider cAN_AGMO_MTLT305Provider = new CAN_AGMO_MTLT305Provider();
		return cAN_AGMO_MTLT305Provider;
	}

	public Can_agmo_mtlt305Package getCan_agmo_mtlt305Package() {
		return (Can_agmo_mtlt305Package) getEPackage();
	}

	@Deprecated
	public static Can_agmo_mtlt305Package getPackage() {
		return Can_agmo_mtlt305Package.eINSTANCE;
	}

} //Can_agmo_mtlt305Factory
