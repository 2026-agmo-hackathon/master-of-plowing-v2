/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.IMachine;

import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.impl.TopicObject;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.InterfaceDisabledException;
import com.bosch.fsp.runtime.feature.exception.MachineWriteException;

import com.bosch.fsp.runtime.util.internal.FeatureConfig;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.SubscriberEnum;
import com.bosch.nevonex.fcb.impl.FCALController;

import java.util.ArrayList;

import org.eclipse.emf.ecore.EObject;

import com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R;

import com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R;

import com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R;

import com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R;

import com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W;

import com.bosch.nevonex.can_agmo_customized_tractor.ICAN_AGMO_Customized_Tractor;

import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.impl.TypesPackage;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import java.lang.reflect.InvocationTargetException;

import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.emf.common.util.BasicEList;
import org.eclipse.emf.common.util.EList;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.util.EDataTypeUniqueEList;

/**
 * An implementation of the model object '<em><b>CAN_AGMO_Customized_Tractor</b></em>'.
 * @generated
 */
public class CAN_AGMO_Customized_Tractor extends TopicObject implements ICAN_AGMO_Customized_Tractor {
	protected EList<PropertyChangeListener> listeners;
	private final IFCALController publishController = new FCALController();
	protected IRECEIVE_ACC_INFO_T_R receive_ACC_INFO;
	protected IRECEIVE_FNR_INFO_T_R receive_FNR_INFO;
	protected IRECEIVE_HYD_INFO_T_R receive_HYD_INFO;
	protected IRECEIVE_SFT_INFO_T_R receive_SFT_INFO;
	protected IAGMO_GENERIC_SEND_CMD_T_W send_ACC_CMD;
	protected IAGMO_GENERIC_SEND_CMD_T_W send_FNR_CMD;
	protected IAGMO_GENERIC_SEND_CMD_T_W send_HYD_CMD;
	protected IAGMO_GENERIC_SEND_CMD_T_W send_SFT_CMD;

	/**
	 * @generated
	 */
	protected CAN_AGMO_Customized_Tractor() {
		super();
		initInterfaceDetails();
	}

	private void initInterfaceDetails() {
		interfaceDetailMap.put("fek/9455", new InterfaceDetails("fek/9455"));
		interfaceDetailMap.put("fek/9456", new InterfaceDetails("fek/9456"));
		interfaceDetailMap.put("fek/9457", new InterfaceDetails("fek/9457"));
		interfaceDetailMap.put("fek/9459", new InterfaceDetails("fek/9459"));
		interfaceDetailMap.put("fek/9460", new InterfaceDetails("fek/9460"));
		interfaceDetailMap.put("fek/9461", new InterfaceDetails("fek/9461"));
		interfaceDetailMap.put("fek/9462", new InterfaceDetails("fek/9462"));
		interfaceDetailMap.put("fek/9464", new InterfaceDetails("fek/9464"));
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return Can_agmo_customized_tractorPackage.Literals.CAN_AGMO_CUSTOMIZED_TRACTOR;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_ACC_INFO_T_R getReceive_ACC_INFO() {
		if (receive_ACC_INFO != null && receive_ACC_INFO.eIsProxy()) {
			InternalEObject oldReceive_ACC_INFO = (InternalEObject) receive_ACC_INFO;
			receive_ACC_INFO = (IRECEIVE_ACC_INFO_T_R) eResolveProxy(oldReceive_ACC_INFO);
			if (receive_ACC_INFO != oldReceive_ACC_INFO) {
			}
		}
		return receive_ACC_INFO;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_ACC_INFO_T_R basicGetReceive_ACC_INFO() {
		return receive_ACC_INFO;
	}

	/**
	 * @generated
	 */
	public void setReceive_ACC_INFO(IRECEIVE_ACC_INFO_T_R newReceive_ACC_INFO) {
		IRECEIVE_ACC_INFO_T_R oldReceive_ACC_INFO = receive_ACC_INFO;
		receive_ACC_INFO = newReceive_ACC_INFO;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"receive_ACC_INFO\" changed from  "
						+ oldReceive_ACC_INFO + " to " + newReceive_ACC_INFO);
		notifyPropertyChange("receive_ACC_INFO", oldReceive_ACC_INFO, newReceive_ACC_INFO);
	}

	/**
	 * @generated
	 */
	public long getReceive_ACC_INFO_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9455");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isReceive_ACC_INFO_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9455");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_FNR_INFO_T_R getReceive_FNR_INFO() {
		if (receive_FNR_INFO != null && receive_FNR_INFO.eIsProxy()) {
			InternalEObject oldReceive_FNR_INFO = (InternalEObject) receive_FNR_INFO;
			receive_FNR_INFO = (IRECEIVE_FNR_INFO_T_R) eResolveProxy(oldReceive_FNR_INFO);
			if (receive_FNR_INFO != oldReceive_FNR_INFO) {
			}
		}
		return receive_FNR_INFO;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_FNR_INFO_T_R basicGetReceive_FNR_INFO() {
		return receive_FNR_INFO;
	}

	/**
	 * @generated
	 */
	public void setReceive_FNR_INFO(IRECEIVE_FNR_INFO_T_R newReceive_FNR_INFO) {
		IRECEIVE_FNR_INFO_T_R oldReceive_FNR_INFO = receive_FNR_INFO;
		receive_FNR_INFO = newReceive_FNR_INFO;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"receive_FNR_INFO\" changed from  "
						+ oldReceive_FNR_INFO + " to " + newReceive_FNR_INFO);
		notifyPropertyChange("receive_FNR_INFO", oldReceive_FNR_INFO, newReceive_FNR_INFO);
	}

	/**
	 * @generated
	 */
	public long getReceive_FNR_INFO_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9456");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isReceive_FNR_INFO_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9456");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_HYD_INFO_T_R getReceive_HYD_INFO() {
		if (receive_HYD_INFO != null && receive_HYD_INFO.eIsProxy()) {
			InternalEObject oldReceive_HYD_INFO = (InternalEObject) receive_HYD_INFO;
			receive_HYD_INFO = (IRECEIVE_HYD_INFO_T_R) eResolveProxy(oldReceive_HYD_INFO);
			if (receive_HYD_INFO != oldReceive_HYD_INFO) {
			}
		}
		return receive_HYD_INFO;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_HYD_INFO_T_R basicGetReceive_HYD_INFO() {
		return receive_HYD_INFO;
	}

	/**
	 * @generated
	 */
	public void setReceive_HYD_INFO(IRECEIVE_HYD_INFO_T_R newReceive_HYD_INFO) {
		IRECEIVE_HYD_INFO_T_R oldReceive_HYD_INFO = receive_HYD_INFO;
		receive_HYD_INFO = newReceive_HYD_INFO;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"receive_HYD_INFO\" changed from  "
						+ oldReceive_HYD_INFO + " to " + newReceive_HYD_INFO);
		notifyPropertyChange("receive_HYD_INFO", oldReceive_HYD_INFO, newReceive_HYD_INFO);
	}

	/**
	 * @generated
	 */
	public long getReceive_HYD_INFO_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9457");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isReceive_HYD_INFO_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9457");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_SFT_INFO_T_R getReceive_SFT_INFO() {
		if (receive_SFT_INFO != null && receive_SFT_INFO.eIsProxy()) {
			InternalEObject oldReceive_SFT_INFO = (InternalEObject) receive_SFT_INFO;
			receive_SFT_INFO = (IRECEIVE_SFT_INFO_T_R) eResolveProxy(oldReceive_SFT_INFO);
			if (receive_SFT_INFO != oldReceive_SFT_INFO) {
			}
		}
		return receive_SFT_INFO;
	}

	/**
	 * @generated
	 */
	public IRECEIVE_SFT_INFO_T_R basicGetReceive_SFT_INFO() {
		return receive_SFT_INFO;
	}

	/**
	 * @generated
	 */
	public void setReceive_SFT_INFO(IRECEIVE_SFT_INFO_T_R newReceive_SFT_INFO) {
		IRECEIVE_SFT_INFO_T_R oldReceive_SFT_INFO = receive_SFT_INFO;
		receive_SFT_INFO = newReceive_SFT_INFO;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"receive_SFT_INFO\" changed from  "
						+ oldReceive_SFT_INFO + " to " + newReceive_SFT_INFO);
		notifyPropertyChange("receive_SFT_INFO", oldReceive_SFT_INFO, newReceive_SFT_INFO);
	}

	/**
	 * @generated
	 */
	public long getReceive_SFT_INFO_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9459");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isReceive_SFT_INFO_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9459");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W getSend_ACC_CMD() {
		if (send_ACC_CMD != null && send_ACC_CMD.eIsProxy()) {
			InternalEObject oldSend_ACC_CMD = (InternalEObject) send_ACC_CMD;
			send_ACC_CMD = (IAGMO_GENERIC_SEND_CMD_T_W) eResolveProxy(oldSend_ACC_CMD);
			if (send_ACC_CMD != oldSend_ACC_CMD) {
			}
		}
		return send_ACC_CMD;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W basicGetSend_ACC_CMD() {
		return send_ACC_CMD;
	}

	/**
	 * @generated
	 */
	public synchronized void setSend_ACC_CMD(IAGMO_GENERIC_SEND_CMD_T_W newSend_ACC_CMD) throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		IAGMO_GENERIC_SEND_CMD_T_W oldSend_ACC_CMD = send_ACC_CMD;
		send_ACC_CMD = newSend_ACC_CMD;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"send_ACC_CMD\" changed from  "
						+ oldSend_ACC_CMD + " to " + newSend_ACC_CMD);
		notifyPropertyChange("send_ACC_CMD", oldSend_ACC_CMD, newSend_ACC_CMD);
		// Publish the value to FIL layer, if the value is set from application.
		int publishIndex = FeatureConfig.getInstance().getPublishIndex();
		if (!Thread.currentThread().getStackTrace()[publishIndex].getClassName().contains(getClass().getName())) {
			List<EObject> objectValue = new ArrayList<>();
			objectValue.add(this);
			publishController.publish("CAN_AGMO_Customized_Tractor.send_ACC_CMD.pub", "send_ACC_CMD", objectValue,
					SubscriberEnum.FIL);
		}
	}

	/**
	 * @generated
	 */
	public long getSend_ACC_CMD_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9460");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isSend_ACC_CMD_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9460");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W getSend_FNR_CMD() {
		if (send_FNR_CMD != null && send_FNR_CMD.eIsProxy()) {
			InternalEObject oldSend_FNR_CMD = (InternalEObject) send_FNR_CMD;
			send_FNR_CMD = (IAGMO_GENERIC_SEND_CMD_T_W) eResolveProxy(oldSend_FNR_CMD);
			if (send_FNR_CMD != oldSend_FNR_CMD) {
			}
		}
		return send_FNR_CMD;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W basicGetSend_FNR_CMD() {
		return send_FNR_CMD;
	}

	/**
	 * @generated
	 */
	public synchronized void setSend_FNR_CMD(IAGMO_GENERIC_SEND_CMD_T_W newSend_FNR_CMD) throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		IAGMO_GENERIC_SEND_CMD_T_W oldSend_FNR_CMD = send_FNR_CMD;
		send_FNR_CMD = newSend_FNR_CMD;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"send_FNR_CMD\" changed from  "
						+ oldSend_FNR_CMD + " to " + newSend_FNR_CMD);
		notifyPropertyChange("send_FNR_CMD", oldSend_FNR_CMD, newSend_FNR_CMD);
		// Publish the value to FIL layer, if the value is set from application.
		int publishIndex = FeatureConfig.getInstance().getPublishIndex();
		if (!Thread.currentThread().getStackTrace()[publishIndex].getClassName().contains(getClass().getName())) {
			List<EObject> objectValue = new ArrayList<>();
			objectValue.add(this);
			publishController.publish("CAN_AGMO_Customized_Tractor.send_FNR_CMD.pub", "send_FNR_CMD", objectValue,
					SubscriberEnum.FIL);
		}
	}

	/**
	 * @generated
	 */
	public long getSend_FNR_CMD_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9461");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isSend_FNR_CMD_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9461");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W getSend_HYD_CMD() {
		if (send_HYD_CMD != null && send_HYD_CMD.eIsProxy()) {
			InternalEObject oldSend_HYD_CMD = (InternalEObject) send_HYD_CMD;
			send_HYD_CMD = (IAGMO_GENERIC_SEND_CMD_T_W) eResolveProxy(oldSend_HYD_CMD);
			if (send_HYD_CMD != oldSend_HYD_CMD) {
			}
		}
		return send_HYD_CMD;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W basicGetSend_HYD_CMD() {
		return send_HYD_CMD;
	}

	/**
	 * @generated
	 */
	public synchronized void setSend_HYD_CMD(IAGMO_GENERIC_SEND_CMD_T_W newSend_HYD_CMD) throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		IAGMO_GENERIC_SEND_CMD_T_W oldSend_HYD_CMD = send_HYD_CMD;
		send_HYD_CMD = newSend_HYD_CMD;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"send_HYD_CMD\" changed from  "
						+ oldSend_HYD_CMD + " to " + newSend_HYD_CMD);
		notifyPropertyChange("send_HYD_CMD", oldSend_HYD_CMD, newSend_HYD_CMD);
		// Publish the value to FIL layer, if the value is set from application.
		int publishIndex = FeatureConfig.getInstance().getPublishIndex();
		if (!Thread.currentThread().getStackTrace()[publishIndex].getClassName().contains(getClass().getName())) {
			List<EObject> objectValue = new ArrayList<>();
			objectValue.add(this);
			publishController.publish("CAN_AGMO_Customized_Tractor.send_HYD_CMD.pub", "send_HYD_CMD", objectValue,
					SubscriberEnum.FIL);
		}
	}

	/**
	 * @generated
	 */
	public long getSend_HYD_CMD_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9462");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isSend_HYD_CMD_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9462");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W getSend_SFT_CMD() {
		if (send_SFT_CMD != null && send_SFT_CMD.eIsProxy()) {
			InternalEObject oldSend_SFT_CMD = (InternalEObject) send_SFT_CMD;
			send_SFT_CMD = (IAGMO_GENERIC_SEND_CMD_T_W) eResolveProxy(oldSend_SFT_CMD);
			if (send_SFT_CMD != oldSend_SFT_CMD) {
			}
		}
		return send_SFT_CMD;
	}

	/**
	 * @generated
	 */
	public IAGMO_GENERIC_SEND_CMD_T_W basicGetSend_SFT_CMD() {
		return send_SFT_CMD;
	}

	/**
	 * @generated
	 */
	public synchronized void setSend_SFT_CMD(IAGMO_GENERIC_SEND_CMD_T_W newSend_SFT_CMD) throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		IAGMO_GENERIC_SEND_CMD_T_W oldSend_SFT_CMD = send_SFT_CMD;
		send_SFT_CMD = newSend_SFT_CMD;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_Customized_Tractor property \"send_SFT_CMD\" changed from  "
						+ oldSend_SFT_CMD + " to " + newSend_SFT_CMD);
		notifyPropertyChange("send_SFT_CMD", oldSend_SFT_CMD, newSend_SFT_CMD);
		// Publish the value to FIL layer, if the value is set from application.
		int publishIndex = FeatureConfig.getInstance().getPublishIndex();
		if (!Thread.currentThread().getStackTrace()[publishIndex].getClassName().contains(getClass().getName())) {
			List<EObject> objectValue = new ArrayList<>();
			objectValue.add(this);
			publishController.publish("CAN_AGMO_Customized_Tractor.send_SFT_CMD.pub", "send_SFT_CMD", objectValue,
					SubscriberEnum.FIL);
		}
	}

	/**
	 * @generated
	 */
	public long getSend_SFT_CMD_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9464");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isSend_SFT_CMD_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9464");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public void addPropertyChangeListener(PropertyChangeListener listener) {
		if (listeners == null) {
			listeners = new BasicEList<>();
		}
		listeners.add(listener);
	}

	/**
	 * @generated
	 */
	public void removePropertyChangeListener(PropertyChangeListener listener) {
		if (listeners != null) {
			listeners.remove(listener);
		}
	}

	/**
	 * @generated
	 */
	public void notifyPropertyChange(String name, Object oldValue, Object newValue) {
		if (listeners != null) {
			for (PropertyChangeListener listener : this.listeners) {
				listener.propertyChange(new PropertyChangeEvent(this, name, oldValue, newValue));
			}
		}
	}

	/**
	 * @generated
	 */
	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
			if (resolve)
				return getReceive_ACC_INFO();
			return basicGetReceive_ACC_INFO();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
			if (resolve)
				return getReceive_FNR_INFO();
			return basicGetReceive_FNR_INFO();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
			if (resolve)
				return getReceive_HYD_INFO();
			return basicGetReceive_HYD_INFO();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
			if (resolve)
				return getReceive_SFT_INFO();
			return basicGetReceive_SFT_INFO();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
			if (resolve)
				return getSend_ACC_CMD();
			return basicGetSend_ACC_CMD();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
			if (resolve)
				return getSend_FNR_CMD();
			return basicGetSend_FNR_CMD();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
			if (resolve)
				return getSend_HYD_CMD();
			return basicGetSend_HYD_CMD();
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
			if (resolve)
				return getSend_SFT_CMD();
			return basicGetSend_SFT_CMD();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	/**
	 * @generated
	 */
	@SuppressWarnings("unchecked")
	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
			setReceive_ACC_INFO((IRECEIVE_ACC_INFO_T_R) newValue);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
			setReceive_FNR_INFO((IRECEIVE_FNR_INFO_T_R) newValue);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
			setReceive_HYD_INFO((IRECEIVE_HYD_INFO_T_R) newValue);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
			setReceive_SFT_INFO((IRECEIVE_SFT_INFO_T_R) newValue);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
			try {
				setSend_ACC_CMD((IAGMO_GENERIC_SEND_CMD_T_W) newValue);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
			try {
				setSend_FNR_CMD((IAGMO_GENERIC_SEND_CMD_T_W) newValue);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
			try {
				setSend_HYD_CMD((IAGMO_GENERIC_SEND_CMD_T_W) newValue);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
			try {
				setSend_SFT_CMD((IAGMO_GENERIC_SEND_CMD_T_W) newValue);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		}
		super.eSet(featureID, newValue);
	}

	/**
	 * @generated
	 */
	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
			setReceive_ACC_INFO((IRECEIVE_ACC_INFO_T_R) null);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
			setReceive_FNR_INFO((IRECEIVE_FNR_INFO_T_R) null);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
			setReceive_HYD_INFO((IRECEIVE_HYD_INFO_T_R) null);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
			setReceive_SFT_INFO((IRECEIVE_SFT_INFO_T_R) null);
			return;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
			try {
				setSend_ACC_CMD((IAGMO_GENERIC_SEND_CMD_T_W) null);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
			try {
				setSend_FNR_CMD((IAGMO_GENERIC_SEND_CMD_T_W) null);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
			try {
				setSend_HYD_CMD((IAGMO_GENERIC_SEND_CMD_T_W) null);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
			try {
				setSend_SFT_CMD((IAGMO_GENERIC_SEND_CMD_T_W) null);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		}
		super.eUnset(featureID);
	}

	/**
	 * @generated
	 */
	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_ACC_INFO:
			return receive_ACC_INFO != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_FNR_INFO:
			return receive_FNR_INFO != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_HYD_INFO:
			return receive_HYD_INFO != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__RECEIVE_SFT_INFO:
			return receive_SFT_INFO != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_ACC_CMD:
			return send_ACC_CMD != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_FNR_CMD:
			return send_FNR_CMD != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_HYD_CMD:
			return send_HYD_CMD != null;
		case Can_agmo_customized_tractorPackage.CAN_AGMO_CUSTOMIZED_TRACTOR__SEND_SFT_CMD:
			return send_SFT_CMD != null;
		}
		return super.eIsSet(featureID);
	}
} //CAN_AGMO_Customized_Tractor
