/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb.util;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.IConnectionFactory;
import com.bosch.nevonex.fcb.IPublishConnectionFactory;
import com.bosch.nevonex.fcb.ISubscribeConnectionFactory;
import com.bosch.nevonex.fcb.IFcbFactory;

import com.bosch.nevonex.fcb.impl.FcbPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class FcbAdapterFactory extends AdapterFactoryImpl {
	protected static FcbPackage modelPackage;

	public FcbAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = FcbPackage.eINSTANCE;
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

	protected FcbSwitch<Adapter> modelSwitch = new FcbSwitch<Adapter>() {
		@Override
		public Adapter caseIFCALController(IFCALController object) {
			return createIFCALControllerAdapter();
		}

		@Override
		public Adapter caseFCALController(IFCALController object) {
			return createFCALControllerAdapter();
		}

		@Override
		public Adapter caseIConnectionFactory(IConnectionFactory object) {
			return createIConnectionFactoryAdapter();
		}

		@Override
		public Adapter caseConnectionFactory(IConnectionFactory object) {
			return createConnectionFactoryAdapter();
		}

		@Override
		public Adapter caseIPublishConnectionFactory(IPublishConnectionFactory object) {
			return createIPublishConnectionFactoryAdapter();
		}

		@Override
		public Adapter casePublishConnectionFactory(IPublishConnectionFactory object) {
			return createPublishConnectionFactoryAdapter();
		}

		@Override
		public Adapter caseISubscribeConnectionFactory(ISubscribeConnectionFactory object) {
			return createISubscribeConnectionFactoryAdapter();
		}

		@Override
		public Adapter caseSubscribeConnectionFactory(ISubscribeConnectionFactory object) {
			return createSubscribeConnectionFactoryAdapter();
		}

		@Override
		public Adapter caseIFcbFactory(IFcbFactory object) {
			return createIFcbFactoryAdapter();
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

	public Adapter createIFCALControllerAdapter() {
		return null;
	}

	public Adapter createFCALControllerAdapter() {
		return null;
	}

	public Adapter createIConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createIPublishConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createPublishConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createISubscribeConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createSubscribeConnectionFactoryAdapter() {
		return null;
	}

	public Adapter createIFcbFactoryAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //FcbAdapterFactory
