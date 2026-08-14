/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.InterfaceDisabledException;
import com.bosch.fsp.runtime.feature.exception.MachineReadException;
import com.bosch.fsp.runtime.feature.exception.MachineWriteException;

import com.bosch.nevonex.common.ITopicObject;

import java.util.List;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public interface IFCALController extends EObject {
	/**
	 * @generated
	 */
	void setMachineUpdate(ITopicObject machine, String attribute, Object payload, InterfaceDetails details,
			long timestamp) throws MachineReadException;

	/**
	 * @generated
	 */
	void publish(String interfaceKey, String interfaceName, List<EObject> payloadValue, SubscriberEnum subscriber)
			throws MachineWriteException, CommunicationException, InterfaceDisabledException;

} // IFCALController
