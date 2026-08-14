/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud.impl;

import com.bosch.nevonex.common.ConnectionTypeEnum;
import java.io.File;
import com.bosch.nevonex.cloud.ICloud;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.InternalEObject;

import com.bosch.nevonex.types.impl.PropertyChange;
import com.bosch.fsp.runtime.feature.exception.MachineInitException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;
import com.bosch.fsp.runtime.feature.exception.CloudBadRequestException;
import com.bosch.fsp.runtime.feature.exception.CloudUnAuthorizedException;
import com.bosch.fsp.runtime.feature.exception.CloudAccessDeniedException;
import com.bosch.fsp.runtime.feature.exception.CloudConnectionException;
import com.bosch.fsp.runtime.feature.exception.PlatformServiceException;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * @generated
 */
public class Cloud extends PropertyChange implements ICloud {
	private static ICloud instance;

	private Cloud() {
		super();
	}

	public static ICloud getInstance() {
		if (instance == null)
			synchronized (Cloud.class) {
				if (instance == null) {
					instance = new Cloud();
				}
			}
		return instance;
	}

	@Override
	protected EClass eStaticClass() {
		return CloudPackage.Literals.CLOUD;
	}

	/**
	 * @generated
	 */
	@Override
	public void processDownloadMessage(String message) {
	}

	/**
	 * @generated
	 */
	@Override
	public void initPlatformService() throws MachineInitException {
	}

	/**
	 * @generated
	 */
	@Override
	public void startPlatformService() throws MachineInitException {
	}

	/**
	 * @generated
	 */
	@Override
	public void stopPlatformService() throws NevonexException {
	}

	/**
	 * @generated
	 */
	@Override
	public String getFilePath() {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadData(String data, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadData(String data, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadFile(File file, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadFile(String filePath, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadFile(File file, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	/**
	 * @generated
	 */
	@Override
	public String uploadFile(String filePath, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException {
		return null;
	}

	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		}
		return super.eGet(featureID, resolve, coreType);
	}

	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		}
		super.eSet(featureID, newValue);
	}

	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		}
		super.eUnset(featureID);
	}

	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		}
		return super.eIsSet(featureID);
	}
} //Cloud
