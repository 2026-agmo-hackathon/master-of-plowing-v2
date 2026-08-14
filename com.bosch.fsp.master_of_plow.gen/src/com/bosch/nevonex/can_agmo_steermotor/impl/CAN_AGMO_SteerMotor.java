/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_steermotor.impl;

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

import com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R;

import com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W;

import com.bosch.nevonex.can_agmo_steermotor.ICAN_AGMO_SteerMotor;

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
 * An implementation of the model object '<em><b>CAN_AGMO_SteerMotor</b></em>'.
 * @generated
 */
public class CAN_AGMO_SteerMotor extends TopicObject implements ICAN_AGMO_SteerMotor {
	protected EList<PropertyChangeListener> listeners;
	private final IFCALController publishController = new FCALController();
	protected IMOTOR_HEARTBEAT_T_R motor_Heartbeat;
	protected IRAW_CAN_DATA_DLC_8_W motor_Request;
	protected int motor_Response_Encoder_Speed = 0;
	protected float motor_Response_EncoderCountValue = 0.0f;

	/**
	 * @generated
	 */
	protected CAN_AGMO_SteerMotor() {
		super();
		initInterfaceDetails();
	}

	private void initInterfaceDetails() {
		interfaceDetailMap.put("fek/9428", new InterfaceDetails("fek/9428"));
		interfaceDetailMap.put("fek/9434", new InterfaceDetails("fek/9434"));
		interfaceDetailMap.put("fek/9435", new InterfaceDetails("fek/9435"));
		interfaceDetailMap.put("fek/9436", new InterfaceDetails("fek/9436"));
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return Can_agmo_steermotorPackage.Literals.CAN_AGMO_STEER_MOTOR;
	}

	/**
	 * @generated
	 */
	public IMOTOR_HEARTBEAT_T_R getMotor_Heartbeat() {
		if (motor_Heartbeat != null && motor_Heartbeat.eIsProxy()) {
			InternalEObject oldMotor_Heartbeat = (InternalEObject) motor_Heartbeat;
			motor_Heartbeat = (IMOTOR_HEARTBEAT_T_R) eResolveProxy(oldMotor_Heartbeat);
			if (motor_Heartbeat != oldMotor_Heartbeat) {
			}
		}
		return motor_Heartbeat;
	}

	/**
	 * @generated
	 */
	public IMOTOR_HEARTBEAT_T_R basicGetMotor_Heartbeat() {
		return motor_Heartbeat;
	}

	/**
	 * @generated
	 */
	public void setMotor_Heartbeat(IMOTOR_HEARTBEAT_T_R newMotor_Heartbeat) {
		IMOTOR_HEARTBEAT_T_R oldMotor_Heartbeat = motor_Heartbeat;
		motor_Heartbeat = newMotor_Heartbeat;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_SteerMotor property \"motor_Heartbeat\" changed from  "
						+ oldMotor_Heartbeat + " to " + newMotor_Heartbeat);
		notifyPropertyChange("motor_Heartbeat", oldMotor_Heartbeat, newMotor_Heartbeat);
	}

	/**
	 * @generated
	 */
	public long getMotor_Heartbeat_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9428");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isMotor_Heartbeat_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9428");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	/**
	 * @generated
	 */
	public IRAW_CAN_DATA_DLC_8_W getMotor_Request() {
		if (motor_Request != null && motor_Request.eIsProxy()) {
			InternalEObject oldMotor_Request = (InternalEObject) motor_Request;
			motor_Request = (IRAW_CAN_DATA_DLC_8_W) eResolveProxy(oldMotor_Request);
			if (motor_Request != oldMotor_Request) {
			}
		}
		return motor_Request;
	}

	/**
	 * @generated
	 */
	public IRAW_CAN_DATA_DLC_8_W basicGetMotor_Request() {
		return motor_Request;
	}

	/**
	 * @generated
	 */
	public synchronized void setMotor_Request(IRAW_CAN_DATA_DLC_8_W newMotor_Request) throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		IRAW_CAN_DATA_DLC_8_W oldMotor_Request = motor_Request;
		motor_Request = newMotor_Request;
		FCALLogs.getInstance().log
				.debug(LoggerConstants.LOG_SDK_PREFIX + " CAN_AGMO_SteerMotor property \"motor_Request\" changed from  "
						+ oldMotor_Request + " to " + newMotor_Request);
		notifyPropertyChange("motor_Request", oldMotor_Request, newMotor_Request);
		// Publish the value to FIL layer, if the value is set from application.
		int publishIndex = FeatureConfig.getInstance().getPublishIndex();
		if (!Thread.currentThread().getStackTrace()[publishIndex].getClassName().contains(getClass().getName())) {
			List<EObject> objectValue = new ArrayList<>();
			objectValue.add(this);
			publishController.publish("CAN_AGMO_SteerMotor.motor_Request.pub", "motor_Request", objectValue,
					SubscriberEnum.FIL);
		}
	}

	/**
	 * @generated
	 */
	public long getMotor_Request_Timestamp() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9436");
		if (interfaceDetails != null) {
			return interfaceDetails.getTimestmap();
		}
		return -1;
	}

	/**
	 * @generated
	 */
	public boolean isMotor_Request_Valid() {
		InterfaceDetails interfaceDetails = interfaceDetailMap.get("fek/9436");
		if (interfaceDetails != null) {
			return interfaceDetails.isValid();
		}
		return false;
	}

	public int getMotor_Response_Encoder_Speed() {
		return motor_Response_Encoder_Speed;
	}

	public void setMotor_Response_Encoder_Speed(int newMotor_Response_Encoder_Speed) {
		motor_Response_Encoder_Speed = newMotor_Response_Encoder_Speed;
	}

	public float getMotor_Response_EncoderCountValue() {
		return motor_Response_EncoderCountValue;
	}

	public void setMotor_Response_EncoderCountValue(float newMotor_Response_EncoderCountValue) {
		motor_Response_EncoderCountValue = newMotor_Response_EncoderCountValue;
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
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT:
			if (resolve)
				return getMotor_Heartbeat();
			return basicGetMotor_Heartbeat();
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST:
			if (resolve)
				return getMotor_Request();
			return basicGetMotor_Request();
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
			return getMotor_Response_Encoder_Speed();
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE:
			return getMotor_Response_EncoderCountValue();
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
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT:
			setMotor_Heartbeat((IMOTOR_HEARTBEAT_T_R) newValue);
			return;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST:
			try {
				setMotor_Request((IRAW_CAN_DATA_DLC_8_W) newValue);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
			setMotor_Response_Encoder_Speed((Integer) newValue);
			return;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE:
			setMotor_Response_EncoderCountValue((Float) newValue);
			return;
		}
		super.eSet(featureID, newValue);
	}

	/**
	 * @generated
	 */
	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT:
			setMotor_Heartbeat((IMOTOR_HEARTBEAT_T_R) null);
			return;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST:
			try {
				setMotor_Request((IRAW_CAN_DATA_DLC_8_W) null);
				return;
			} catch (Throwable throwable) {
				throw new RuntimeException(throwable);
			}
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
			setMotor_Response_Encoder_Speed(0);
			return;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE:
			setMotor_Response_EncoderCountValue(0.0f);
			return;
		}
		super.eUnset(featureID);
	}

	/**
	 * @generated
	 */
	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_HEARTBEAT:
			return motor_Heartbeat != null;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_REQUEST:
			return motor_Request != null;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_SPEED:
			return motor_Response_Encoder_Speed != 0;
		case Can_agmo_steermotorPackage.CAN_AGMO_STEER_MOTOR__MOTOR_RESPONSE_ENCODER_COUNT_VALUE:
			return motor_Response_EncoderCountValue != 0.0f;
		}
		return super.eIsSet(featureID);
	}
} //CAN_AGMO_SteerMotor
