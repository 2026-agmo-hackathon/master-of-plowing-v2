/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud;

import com.bosch.fsp.runtime.feature.ICloudDownload;
import com.bosch.fsp.runtime.feature.exception.CloudAccessDeniedException;
import com.bosch.fsp.runtime.feature.exception.CloudBadRequestException;
import com.bosch.fsp.runtime.feature.exception.CloudConnectionException;
import com.bosch.fsp.runtime.feature.exception.CloudUnAuthorizedException;
import com.bosch.fsp.runtime.feature.exception.MachineInitException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;
import com.bosch.fsp.runtime.feature.exception.PlatformServiceException;
import com.bosch.nevonex.common.ConnectionTypeEnum;
import com.bosch.nevonex.types.IPropertyChange;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * @generated
 */
public interface ICloud extends ICloudDownload, IPropertyChange {
	void processDownloadMessage(String message);
	void initPlatformService() throws MachineInitException;
	void startPlatformService() throws MachineInitException;
	void stopPlatformService() throws NevonexException;
	String getFilePath();
	String uploadData(String data, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
	String uploadData(String data, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
	String uploadFile(File file, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
	String uploadFile(String filePath, int priority) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
	String uploadFile(File file, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
	String uploadFile(String filePath, int priority, ConnectionTypeEnum connectionType) throws CloudBadRequestException, CloudUnAuthorizedException, CloudAccessDeniedException, CloudConnectionException, PlatformServiceException, FileNotFoundException, IOException;
} // ICloud
