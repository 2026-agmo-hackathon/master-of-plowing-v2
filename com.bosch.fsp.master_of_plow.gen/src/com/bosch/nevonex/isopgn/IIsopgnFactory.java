/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.isopgn;

/**
 * @generated
 */
public interface IIsopgnFactory {
	IIsopgnFactory INSTANCE = com.bosch.nevonex.isopgn.impl.IsopgnFactory.eINSTANCE;

	IISOPGN createISOPGN();

	IISOPGNProvider createISOPGNProvider();

} //IIsopgnFactory
