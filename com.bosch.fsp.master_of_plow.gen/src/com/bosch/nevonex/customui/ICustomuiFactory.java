/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui;

/**
 * @generated
 */
public interface ICustomuiFactory {
	ICustomuiFactory INSTANCE = com.bosch.nevonex.customui.impl.CustomuiFactory.eINSTANCE;

	IUIWebServiceProvider createUIWebServiceProvider();

} //ICustomuiFactory
