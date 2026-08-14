/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud;

/**
 * @generated
 */
public interface ICloudFactory {
	ICloudFactory INSTANCE = com.bosch.nevonex.cloud.impl.CloudFactory.eINSTANCE;

	ICloud createCloud();

} //ICloudFactory
