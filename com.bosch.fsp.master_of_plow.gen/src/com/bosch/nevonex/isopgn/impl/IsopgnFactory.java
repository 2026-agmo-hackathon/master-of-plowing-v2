/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn.impl;

import com.bosch.nevonex.isopgn.IIsopgnFactory;
import com.bosch.nevonex.isopgn.IISOPGN;
import com.bosch.nevonex.isopgn.IISOPGNProvider;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class IsopgnFactory extends EFactoryImpl implements IIsopgnFactory {
	public static final IsopgnFactory eINSTANCE = init();

	public static IsopgnFactory init() {
		try {
			IsopgnFactory theIsopgnFactory = (IsopgnFactory) EPackage.Registry.INSTANCE.getEFactory(IsopgnPackage.eNS_URI);
			if (theIsopgnFactory != null) {
				return theIsopgnFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new IsopgnFactory();
	}

	public IsopgnFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case IsopgnPackage.ISOPGN:
			return createISOPGN();
		case IsopgnPackage.ISOPGN_PROVIDER:
			return createISOPGNProvider();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IISOPGN createISOPGN() {
		ISOPGN iSOPGN = new ISOPGN();
		return iSOPGN;
	}

	public IISOPGNProvider createISOPGNProvider() {
		ISOPGNProvider iSOPGNProvider = new ISOPGNProvider();
		return iSOPGNProvider;
	}

	public IsopgnPackage getIsopgnPackage() {
		return (IsopgnPackage) getEPackage();
	}

	@Deprecated
	public static IsopgnPackage getPackage() {
		return IsopgnPackage.eINSTANCE;
	}

} //IsopgnFactory
