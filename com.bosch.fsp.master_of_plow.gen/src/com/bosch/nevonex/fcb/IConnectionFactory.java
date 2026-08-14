/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb;

import com.bosch.fsp.runtime.feature.IMachineProvider;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;

import com.bosch.nevonex.common.ITopicObject;

import java.util.List;
import java.util.Map;

import org.eclipse.emf.ecore.EObject;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttMessageListener;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @generated
 */
public interface IConnectionFactory extends EObject, MqttCallback, MqttCallbackExtended {
	public void registerPlatformService(String topic, IMqttMessageListener listener);

	MqttAsyncClient getClient();

	void setClient(MqttAsyncClient value);

	Map<String, String> getTopicMap();

	void setTopicMap(Map<String, String> value);

	Map<String, String> getPayloadMap();

	void setPayloadMap(Map<String, String> value);

	Map<String, String> getPrefixMap();

	void setPrefixMap(Map<String, String> value);

	Map<String, String> getMachinePathMap();

	void setMachinePathMap(Map<String, String> value);

	List<IMachineProvider> getProviders();

	void messageArrived(String topic, MqttMessage message);

	void deliveryComplete(IMqttDeliveryToken token);

	void connectionLost(Throwable cause);

	void initConnectionFactory() throws CommunicationException;

	ITopicObject getTopicElement(IMachineProvider provider, String topic, String elementPath);

	void connectComplete(boolean reconnect, String serverURL);

	void terminateConnectionFactory() throws NevonexException;

} // IConnectionFactory
