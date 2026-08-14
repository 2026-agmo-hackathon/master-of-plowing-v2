/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.serial_ext_gps_nmea0183.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.runtime.feature.IMachine;
import com.bosch.fsp.runtime.feature.IMachineProvider;
import com.bosch.fsp.runtime.feature.MachineConnectionInfo;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.MachineInitException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;

import com.bosch.fsp.runtime.util.internal.Util;

import com.bosch.nevonex.common.ITopicObject;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.impl.ConnectionFactory;
import com.bosch.nevonex.fcb.impl.FCALController;
import com.bosch.nevonex.fcb.impl.PublishConnectionFactory;

import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183;
import com.bosch.nevonex.serial_ext_gps_nmea0183.ISerial_Ext_GPS_NMEA0183Provider;

import com.bosch.nevonex.types.IPropertyChange;
import com.bosch.nevonex.types.impl.TypesPackage;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import java.io.InputStream;

import java.lang.reflect.InvocationTargetException;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang3.exception.ExceptionUtils;

import org.eclipse.emf.common.notify.NotificationChain;
import org.eclipse.emf.common.util.BasicEList;
import org.eclipse.emf.common.util.EList;
import org.eclipse.emf.common.util.Enumerator;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EReference;
import org.eclipse.emf.ecore.EStructuralFeature;
import org.eclipse.emf.ecore.InternalEObject;

import org.eclipse.emf.ecore.impl.EObjectImpl;

import org.eclipse.emf.ecore.util.EDataTypeUniqueEList;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * An implementation of the model object '<em><b>Serial_Ext_GPS_NMEA0183Provider</b></em>'.
 * @generated
 */
public class Serial_Ext_GPS_NMEA0183Provider extends EObjectImpl implements ISerial_Ext_GPS_NMEA0183Provider, IMachineProvider {
	protected EList<PropertyChangeListener> listeners;
	protected IFCALController controller;
	protected ISerial_Ext_GPS_NMEA0183 serial_Ext_GPS_NMEA0183;

	private Map<String, ITopicObject> indexToObjectMap = new HashMap<>();

	/**
	 * @generated
	 */
	public Serial_Ext_GPS_NMEA0183Provider() {
		super();
	}

	public ISerial_Ext_GPS_NMEA0183 getSerial_Ext_GPS_NMEA0183() {
		if (serial_Ext_GPS_NMEA0183 != null && ((org.eclipse.emf.ecore.EObject) serial_Ext_GPS_NMEA0183).eIsProxy()) {
			org.eclipse.emf.ecore.InternalEObject old = (org.eclipse.emf.ecore.InternalEObject) serial_Ext_GPS_NMEA0183;
			serial_Ext_GPS_NMEA0183 = (ISerial_Ext_GPS_NMEA0183) eResolveProxy(old);
		}
		return serial_Ext_GPS_NMEA0183;
	}

	public ISerial_Ext_GPS_NMEA0183 basicGetSerial_Ext_GPS_NMEA0183() {
		return serial_Ext_GPS_NMEA0183;
	}

	public void setSerial_Ext_GPS_NMEA0183(ISerial_Ext_GPS_NMEA0183 newSerial_Ext_GPS_NMEA0183) {
		serial_Ext_GPS_NMEA0183 = newSerial_Ext_GPS_NMEA0183;
	}

	public boolean acceptDom(String root) {
		return "serial_ext_gps_nmea0183".equalsIgnoreCase(root);
	}

	/**
	 * @generated
	 */
	public void createMachines(InputStream stream) throws MachineInitException {
		ensureMachineCreated();
		if (stream == null) {
			return;
		}
		try {
			Element root = Util.getDomRootElement(stream);
			List<Node> secondLevelNodeList = new ArrayList<Node>();
			if ("root".equalsIgnoreCase(root.getNodeName())) {
				NodeList childNodes = root.getChildNodes();
				for (int i = 0; i < childNodes.getLength(); i++) {
					Node secondLevelNode = childNodes.item(i);
					if (secondLevelNode.getNodeType() == Node.ELEMENT_NODE
							&& acceptDom(secondLevelNode.getNodeName())) {
						secondLevelNodeList.add(secondLevelNode);
					}
				}
			} else {
				if (root.getNodeType() == Node.ELEMENT_NODE && acceptDom(root.getNodeName())) {
					secondLevelNodeList.add(root);
				}
			}
			if (secondLevelNodeList.isEmpty()) {
				return;
			}
			constructChildTypes(secondLevelNodeList.get(0), serial_Ext_GPS_NMEA0183, "Serial_Ext_GPS_NMEA0183");
		} catch (Exception e) {
			throw new MachineInitException("Exception while initializing machine data", e);
		}
	}

	/**
	 * 머신 인스턴스를 만들고 인덱스 맵에 올린다.
	 *
	 * <p>FD 는 FIL 이 보낸 DOM 을 파싱해 머신을 만든다. IDE 프로젝트는 플러그인당
	 * 머신이 하나로 고정이라 DOM 내용에 의존할 필요가 없고, C++ 제너레이터도 같은
	 * 방식으로 직접 만든다. 그래서 DOM 이 안 와도 첫 수신 메시지에서 복구된다 —
	 * 예전에는 여기가 비어 있어 인덱스 맵이 영원히 비었고, getTopicElement 가 항상
	 * null 을 돌려줘 구독한 값이 전부 유실됐다.
	 * @generated
	 */
	private synchronized void ensureMachineCreated() {
		if (serial_Ext_GPS_NMEA0183 != null) {
			return;
		}
		setSerial_Ext_GPS_NMEA0183(Serial_ext_gps_nmea0183Factory.eINSTANCE.createSerial_Ext_GPS_NMEA0183());
		if (serial_Ext_GPS_NMEA0183 != null) {
			constructChildTypes(null, serial_Ext_GPS_NMEA0183, "Serial_Ext_GPS_NMEA0183");
			notifyPropertyChange("root", null, serial_Ext_GPS_NMEA0183);
		}
	}

	/**
	 * @generated
	 */
	public void constructChildTypes(Node parent, Object parentObj, String path) {
		if (!(parentObj instanceof ITopicObject)) {
			return;
		}
		ITopicObject topicObject = (ITopicObject) parentObj;
		// 같은 객체를 네 가지 키로 올린다. 수신 경로가 넘겨주는 인덱스가 페이로드
		// 모양에 따라 달라지기 때문이다 (머신명 / 소문자 머신명 / "<머신>/0" / 경로).
		if (path != null && !path.isEmpty()) {
			indexToObjectMap.put(path, topicObject);
		}
		indexToObjectMap.put("Serial_Ext_GPS_NMEA0183", topicObject);
		indexToObjectMap.put("serial_ext_gps_nmea0183", topicObject);
		indexToObjectMap.put("serial_ext_gps_nmea0183/0", topicObject);
		indexToObjectMap.put("0", topicObject);
	}

	/**
	 * @generated
	 */
	public void initMachineProvider() throws MachineInitException {
		try {
			this.controller = new FCALController();
			ConnectionFactory.getInstance().getProviders().add(this);
			PublishConnectionFactory.getInstance();
		} catch (Exception e) {
			throw new MachineInitException("Exception while initializing the machine provider.", e);
		}
	}

	/**
	 * @generated
	 */
	public void startMachineProvider() throws MachineInitException {
		try {
			ConnectionFactory.getInstance().initConnectionFactory();
			PublishConnectionFactory.getInstance().initConnectionFactory();
		} catch (CommunicationException e) {
			throw new MachineInitException(e.getMessage(), e);
		}
	}

	/**
	 * @generated
	 */
	public void stopMachineProvider() throws NevonexException {
		try {
			ConnectionFactory.getInstance().terminateConnectionFactory();
			PublishConnectionFactory.getInstance().terminateConnectionFactory();
		} catch (Exception e) {
			throw new NevonexException(e.getMessage(), e);
		}
	}

	/**
	 * @generated
	 */
	public ITopicObject getTopicElement(String index) {
		ensureMachineCreated();
		if (index == null) {
			return null;
		}
		ITopicObject topicObject = indexToObjectMap.get(index);
		if (topicObject != null) {
			return topicObject;
		}
		// 대소문자만 다른 인덱스로도 찾는다 — FIL 페이로드의 머신 키 표기가 일정하지 않다.
		for (Map.Entry<String, ITopicObject> entry : indexToObjectMap.entrySet()) {
			if (entry.getKey().equalsIgnoreCase(index)) {
				return entry.getValue();
			}
		}
		return null;
	}

	/**
	 * @generated
	 */
	public void onConnectionStatusChange(IMachine machine, MachineConnectionInfo info) {
		notifyPropertyChange(machine, MachineConnectionInfo.CONNECT_PROPERTY, null, info);
	}

	public void notifyPropertyChange(Object source, String name, Object oldValue, Object newValue) {
		if (listeners != null) {
			for (PropertyChangeListener listener : this.listeners) {
				listener.propertyChange(new PropertyChangeEvent(source, name, oldValue, newValue));
			}
		}
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
	protected EClass eStaticClass() {
		return Serial_ext_gps_nmea0183Package.Literals.SERIAL_EXT_GPS_NMEA0183PROVIDER;
	}
} //Serial_Ext_GPS_NMEA0183Provider
