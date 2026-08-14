/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.impl;

import com.bosch.fsp.runtime.feature.exception.BulkProcessorException;

import com.bosch.nevonex.fcal.IBulkProcessor;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.SubscriberEnum;

import java.lang.reflect.InvocationTargetException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EStructuralFeature;

import org.eclipse.emf.ecore.impl.EObjectImpl;

/**
 * An implementation of the model object '<em><b>BulkProcessor</b></em>'.
 * @generated
 */
public class BulkProcessor extends EObjectImpl implements IBulkProcessor {
	/**
	 * @generated
	 */
	public BulkProcessor() {
		super();
	}

	private final Map<String, Map<EObject, Object>> ifToMachineValueMap = new HashMap<>();

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return FcalPackage.Literals.BULK_PROCESSOR;
	}

	/**
	 * @generated
	 */
	public void executeBulkOperations() throws BulkProcessorException {
		try {
			for (Entry<String, Map<EObject, Object>> entry : ifToMachineValueMap.entrySet()) {
				String interfaceName = entry.getKey();
				Map<EObject, Object> map = entry.getValue();
				EObject machine = null;
				for (Entry<EObject, Object> innerMap : map.entrySet()) {
					machine = innerMap.getKey();
					Object value = innerMap.getValue();
					EStructuralFeature feature = machine.eClass().getEStructuralFeature(interfaceName);
					if (machine != null && feature != null) {
						machine.eSet(feature, value);
					}
				}
				if (machine != null) {
					EStructuralFeature feature = machine.eClass().getEStructuralFeature("controller");
					if (feature != null) {
						IFCALController controller = (IFCALController) machine.eGet(feature);
						controller.publish(machine.getClass().getSimpleName() + "." + interfaceName + ".pub",
								interfaceName, new ArrayList(map.keySet()), SubscriberEnum.FIL);
					}
				}
			}
			ifToMachineValueMap.clear();
		} catch (Exception e) {
			throw new BulkProcessorException("Exception occurred while processing data in bulk operation.", e);
		}
	}

	/**
	 * @generated
	 */
	@Override
	public Object eInvoke(int operationID, EList<?> arguments) throws InvocationTargetException {
		switch (operationID) {
		case FcalPackage.I_BULK_PROCESSOR___EXECUTE_BULK_OPERATIONS:
			try {
				executeBulkOperations();
				return null;
			} catch (Throwable throwable) {
				throw new InvocationTargetException(throwable);
			}
		}
		return super.eInvoke(operationID, arguments);
	}
} //BulkProcessor
