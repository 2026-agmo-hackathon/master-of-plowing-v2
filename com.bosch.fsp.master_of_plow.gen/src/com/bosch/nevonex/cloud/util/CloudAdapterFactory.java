/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud.util;

import com.bosch.fsp.runtime.feature.ICloudDownload;
import com.bosch.nevonex.cloud.ICloud;

import com.bosch.nevonex.cloud.impl.CloudPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class CloudAdapterFactory extends AdapterFactoryImpl {
	protected static CloudPackage modelPackage;

	public CloudAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = CloudPackage.eINSTANCE;
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

	protected CloudSwitch<Adapter> modelSwitch = new CloudSwitch<Adapter>() {
		@Override
		public Adapter caseICloudDownload(ICloudDownload object) {
			return createICloudDownloadAdapter();
		}

		@Override
		public Adapter caseCloud(ICloud object) {
			return createCloudAdapter();
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

	public Adapter createICloudDownloadAdapter() {
		return null;
	}

	public Adapter createCloudAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //CloudAdapterFactory
