/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common;

/**
 * @generated
 */
public interface ICommonFactory {
	ICommonFactory INSTANCE = com.bosch.nevonex.common.impl.CommonFactory.eINSTANCE;

	IAbsolutePosition createAbsolutePosition();

	ITopicObject createTopicObject();

} //ICommonFactory
