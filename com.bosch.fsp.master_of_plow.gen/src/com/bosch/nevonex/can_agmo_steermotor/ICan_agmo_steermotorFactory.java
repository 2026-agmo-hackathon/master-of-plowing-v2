/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor;

/**
 * @generated
 */
public interface ICan_agmo_steermotorFactory {
	ICan_agmo_steermotorFactory INSTANCE = com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorFactory.eINSTANCE;

	ICAN_AGMO_SteerMotor createCAN_AGMO_SteerMotor();

	ICAN_AGMO_SteerMotorProvider createCAN_AGMO_SteerMotorProvider();

} //ICan_agmo_steermotorFactory
