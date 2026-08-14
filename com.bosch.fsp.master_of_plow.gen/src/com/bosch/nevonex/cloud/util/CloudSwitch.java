/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud.util;

import com.bosch.fsp.runtime.feature.ICloudDownload;
import com.bosch.nevonex.cloud.ICloud;

import com.bosch.nevonex.cloud.impl.CloudPackage;

import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.util.Switch;
import com.bosch.nevonex.types.IPropertyChange;

/**
 * @generated
 */
public class CloudSwitch<T> extends Switch<T> {
	protected static CloudPackage modelPackage;

	public CloudSwitch() {
		if (modelPackage == null) {
			modelPackage = CloudPackage.eINSTANCE;
		}
	}

	@Override
	protected boolean isSwitchFor(EPackage ePackage) {
		return ePackage == modelPackage;
	}

	@Override
	protected T doSwitch(int classifierID, EObject theEObject) {
		if (classifierID < 80) return doSwitch_chunk0(classifierID, theEObject);
		return defaultCase(theEObject);
	}

	private T doSwitch_chunk0(int classifierID, EObject theEObject) {
		switch (classifierID) {
			case CloudPackage.I_CLOUD_DOWNLOAD: {
				ICloudDownload iCloudDownload = (ICloudDownload) theEObject;
				T result = caseICloudDownload(iCloudDownload);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			case CloudPackage.CLOUD: {
				ICloud cloud = (ICloud) theEObject;
				T result = caseCloud(cloud);
				if (result == null)
					result = caseICloudDownload((ICloudDownload) cloud);
				if (result == null)
					result = casePropertyChange((IPropertyChange) cloud);
				if (result == null)
					result = defaultCase(theEObject);
				return result;
			}
			default:
				return defaultCase(theEObject);
		}
	}

	public T caseICloudDownload(ICloudDownload object) {
		return null;
	}

	public T caseCloud(ICloud object) {
		return null;
	}

	public T casePropertyChange(IPropertyChange object) {
		return null;
	}

	@Override
	public T defaultCase(EObject object) {
		return null;
	}

} //CloudSwitch
