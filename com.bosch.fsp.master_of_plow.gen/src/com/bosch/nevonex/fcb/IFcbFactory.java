/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb;

/**
 * @generated
 */
public interface IFcbFactory {
	IFcbFactory INSTANCE = com.bosch.nevonex.fcb.impl.FcbFactory.eINSTANCE;

	IFCALController createFCALController();

	ISubscribeConnectionFactory createSubscribeConnectionFactory();

} //IFcbFactory
