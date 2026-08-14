package com.bosch.nevonex.fcb.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.ILogger;
import com.bosch.fsp.logger.LoggerConstants;
import com.bosch.fsp.runtime.feature.GracefulFeatureStop;
import com.bosch.fsp.runtime.feature.IMachineProvider;
import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;
import com.bosch.fsp.runtime.feature.notification.SslUtil;
import com.bosch.fsp.runtime.util.internal.FeatureConfig;
import com.bosch.fsp.runtime.util.internal.FeatureConfigConstants;
import com.bosch.fsp.runtime.util.internal.WebserviceUtil;
import com.bosch.nevonex.fcb.IPublishConnectionFactory;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonPrimitive;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.net.ssl.SSLSocketFactory;
import org.apache.commons.lang3.exception.ExceptionUtils;
import org.eclipse.emf.common.util.EList;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.InternalEObject;
import org.eclipse.emf.ecore.impl.EObjectImpl;
import org.eclipse.emf.ecore.util.EObjectResolvingEList;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class PublishConnectionFactory extends EObjectImpl implements IPublishConnectionFactory {

    protected static final MqttAsyncClient CLIENT_EDEFAULT = null;

    protected MqttAsyncClient client = CLIENT_EDEFAULT;
    protected Map<String, String> topicMap;
    protected Map<String, String> payloadMap;
    protected Map<String, String> prefixMap;
    protected Map<String, String> machinePathMap;
    protected EList<IMachineProvider> providers;
    protected Map<Integer, Boolean> enabledDisabledTopicMap;

    private static IPublishConnectionFactory instance;
    private boolean init = false;

    private PublishConnectionFactory() throws CommunicationException {
    }

    public static IPublishConnectionFactory getInstance() {
        if (instance != null) {
            return instance;
        }
        synchronized (PublishConnectionFactory.class) {
            if (instance != null) {
                return instance;
            }
            try {
                instance = new PublishConnectionFactory();
            } catch (CommunicationException e) {
                throw new RuntimeException("Exception while creating communication broker.", e);
            }
            return instance;
        }
    }

    private void createTLSCredConnection() throws NevonexException {
        try {
            this.createTLSConnectionInternal();
        } catch (Exception e) {
            FCALLogs.getInstance().log.warn("[SDK]: TLS connection is not working. Switching back to credentials.");
            this.createPwdConnection();
        }
    }

    private void createTLSConnection() throws NevonexException {
        try {
            this.createTLSConnectionInternal();
        } catch (MqttException e) {
            if (e.getReasonCode() == 32103) {
                FCALLogs.getInstance().log.incident("[SDK]: INC_018_02: " + e.getMessage());
            } else {
                FCALLogs.getInstance().log.incident("[SDK]: INC_011_03: " + e.getMessage());
            }
            throw new NevonexException("Error in establishing connection for notification client", e);
        } catch (Exception e) {
            FCALLogs.getInstance().log.incident("[SDK]: INC_011_03: " + e.getMessage());
            throw new NevonexException("Error in establishing connection for notification client", e);
        }
    }

    private void createTLSConnectionInternal() throws Exception {
        String broker = FeatureConfig.getInstance().getBrokerUrl(FeatureConfig.MqttAuthType.TLS);
        FCALLogs.getInstance().log.debug("[SDK]:  MQTT Connection details: " + broker);
        this.client = new MqttAsyncClient(broker, String.valueOf(System.nanoTime()), new MemoryPersistence());
        FCALLogs.getInstance().log.debug("[SDK]:  FCAL Client Created");

        MqttConnectOptions options = new MqttConnectOptions();
        options.setMaxInflight(100000);
        options.setCleanSession(true);
        options.setKeepAliveInterval(1000);
        options.setAutomaticReconnect(true);

        SSLSocketFactory socketFactory = SslUtil.getSocketFactory(
                FeatureConfig.getInstance().getCaCert(),
                FeatureConfig.getInstance().getClientCert(),
                FeatureConfig.getInstance().getClientKey(),
                "");
        options.setSocketFactory(socketFactory);
        options.setHttpsHostnameVerificationEnabled(false);

        this.tryConnectWithRetry(options, 3);
        this.client.setCallback((MqttCallback) this);
        FCALLogs.getInstance().log.info(
                "[SDK]: " + this.getClass().getSimpleName() + ": Connection using certificates is successful");
    }

    private void createPwdConnection() throws NevonexException {
        try {
            String broker = FeatureConfig.getInstance().getBrokerUrl(FeatureConfig.MqttAuthType.CREDENTIALS);
            FCALLogs.getInstance().log.debug("[SDK]:  MQTT Connection details: " + broker);
            this.client = new MqttAsyncClient(broker, String.valueOf(System.nanoTime()), new MemoryPersistence());
            FCALLogs.getInstance().log.debug("[SDK]: FCAL Client Created");

            MqttConnectOptions options = new MqttConnectOptions();
            options.setMaxInflight(100000);
            options.setCleanSession(true);
            options.setKeepAliveInterval(1000);
            options.setAutomaticReconnect(true);
            options.setUserName(FeatureConfig.getInstance().getMqttUserName());
            options.setPassword(FeatureConfig.getInstance().getMqttPassword().toCharArray());

            this.tryConnectWithRetry(options, 3);
            this.client.setCallback((MqttCallback) this);
            FCALLogs.getInstance().log.info(
                    "[SDK]: " + this.getClass().getSimpleName() + ": Connection using credentials is successful");
        } catch (MqttException e) {
            if (e.getReasonCode() == 32103) {
                FCALLogs.getInstance().log.incident("[SDK]: INC_018_02: " + e.getMessage());
            } else {
                FCALLogs.getInstance().log.incident("[SDK]: INC_011_02: " + e.getMessage());
            }
            throw new NevonexException("Error in establishing connection for notification client", e);
        } catch (Exception e) {
            FCALLogs.getInstance().log.incident("[SDK]: INC_011_02: " + e.getMessage());
            throw new NevonexException("Error in establishing connection for notification client", e);
        }
    }

    private void createNormalConnection() throws NevonexException {
        try {
            String broker = FeatureConfig.getInstance().getBrokerUrl(FeatureConfig.MqttAuthType.NONE);
            FCALLogs.getInstance().log.debug("[SDK]:  MQTT Connection details: " + broker);
            this.client = new MqttAsyncClient(broker, String.valueOf(System.nanoTime()), new MemoryPersistence());
            FCALLogs.getInstance().log.debug("[SDK]: FCAL Client Created");

            MqttConnectOptions options = new MqttConnectOptions();
            options.setMaxInflight(100000);
            options.setCleanSession(true);
            options.setKeepAliveInterval(1000);
            options.setAutomaticReconnect(true);

            this.tryConnectWithRetry(options, 3);
            this.client.setCallback((MqttCallback) this);
            FCALLogs.getInstance().log.info(
                    "[SDK]: " + this.getClass().getSimpleName() + ": Basic connection is successful");
        } catch (Exception e) {
            throw new NevonexException("Error in establishing connection for notification client", e);
        }
    }

    private void tryConnectWithRetry(MqttConnectOptions options, int maxRetries) throws Exception {
        int delay_ms = 5000;
        int timeout = 60000;
        for (int attempt = 0; attempt <= maxRetries; ++attempt) {
            try {
                IMqttToken token = this.client.connect(options);
                token.waitForCompletion(timeout);
                return;
            } catch (Exception e) {
                if (attempt < maxRetries) {
                    FCALLogs.getInstance().log.warn("[SDK]: " + this.getClass().getSimpleName()
                            + "MQTT connect attempt " + (attempt + 1)
                            + " failed, retrying in " + (delay_ms / 1000)
                            + "s. Error: " + e.getMessage());
                    try {
                        Thread.sleep(delay_ms);
                        continue;
                    } catch (InterruptedException ie) {
                        Thread.currentThread().interrupt();
                        throw ie;
                    }
                }
                FCALLogs.getInstance().log.error("[SDK]: " + this.getClass().getSimpleName()
                        + "MQTT connect failed after " + (maxRetries + 1)
                        + " attempts. Error: " + e.getMessage());
                throw e;
            }
        }
    }

    private void initMappings() {
        this.topicMap = new HashMap<>();
		topicMap.put("/1/+", "CAN_AGMO_Customized_Tractor.machineconnect.sub");
		topicMap.put("/0/+", "CAN_AGMO_Customized_Tractor.machinedata.sub");
		topicMap.put("/9455", "CAN_AGMO_Customized_Tractor.receive_ACC_INFO.sub");
		topicMap.put("/9456", "CAN_AGMO_Customized_Tractor.receive_FNR_INFO.sub");
		topicMap.put("/9457", "CAN_AGMO_Customized_Tractor.receive_HYD_INFO.sub");
		topicMap.put("/9459", "CAN_AGMO_Customized_Tractor.receive_SFT_INFO.sub");
		topicMap.put("/9460", "CAN_AGMO_Customized_Tractor.send_ACC_CMD.pub");
		topicMap.put("/9461", "CAN_AGMO_Customized_Tractor.send_FNR_CMD.pub");
		topicMap.put("/9462", "CAN_AGMO_Customized_Tractor.send_HYD_CMD.pub");
		topicMap.put("/9464", "CAN_AGMO_Customized_Tractor.send_SFT_CMD.pub");
		topicMap.put("/1/+", "CAN_AGMO_MTLT305.machineconnect.sub");
		topicMap.put("/0/+", "CAN_AGMO_MTLT305.machinedata.sub");
		topicMap.put("/9440", "CAN_AGMO_MTLT305.aceinna_Accel.sub");
		topicMap.put("/9441", "CAN_AGMO_MTLT305.aceinna_AngleRate.sub");
		topicMap.put("/9442", "CAN_AGMO_MTLT305.aceinna_Angles.sub");
		topicMap.put("/1/+", "CAN_AGMO_SteerMotor.machineconnect.sub");
		topicMap.put("/0/+", "CAN_AGMO_SteerMotor.machinedata.sub");
		topicMap.put("/9428", "CAN_AGMO_SteerMotor.motor_Heartbeat.sub");
		topicMap.put("/9436", "CAN_AGMO_SteerMotor.motor_Request.pub");
		topicMap.put("/9434", "CAN_AGMO_SteerMotor.motor_Response_Encoder_Speed.sub");
		topicMap.put("/9435", "CAN_AGMO_SteerMotor.motor_Response_EncoderCountValue.sub");
		topicMap.put("/1/+", "ISOPGN.machineconnect.sub");
		topicMap.put("/0/+", "ISOPGN.machinedata.sub");
		topicMap.put("/251", "ISOPGN.hitchPosPerRe.sub");
		topicMap.put("/1/+", "Platform_Service.machineconnect.sub");
		topicMap.put("/0/+", "Platform_Service.machinedata.sub");
		topicMap.put("/0", "Platform_Service.download.sub");
		topicMap.put("/0", "Platform_Service.uploadData.sub");
		topicMap.put("/0", "Platform_Service.uploadFile.sub");
		topicMap.put("/1/+", "Serial_Ext_GPS_NMEA0183.machineconnect.sub");
		topicMap.put("/0/+", "Serial_Ext_GPS_NMEA0183.machinedata.sub");
		topicMap.put("/9465", "Serial_Ext_GPS_NMEA0183.serial_Ext_GPS_NMEA0183_Data.sub");

        this.payloadMap = new HashMap<>();


        this.prefixMap = new HashMap<>();
		prefixMap.put("FCAL2FIL_Subscribe", "fek");
		prefixMap.put("FCAL2FIL_Publish", "fek");

        this.machinePathMap = new HashMap<>();
		machinePathMap.put("CAN_AGMO_Customized_Tractor", "CAN_AGMO_Customized_Tractor");
		machinePathMap.put("CAN_AGMO_MTLT305", "CAN_AGMO_MTLT305");
		machinePathMap.put("CAN_AGMO_SteerMotor", "CAN_AGMO_SteerMotor");
		machinePathMap.put("ISOPGN", "ISOPGN");
		machinePathMap.put("Platform_Service", "Platform_Service");
		machinePathMap.put("Serial_Ext_GPS_NMEA0183", "Serial_Ext_GPS_NMEA0183");

        this.initEnableDisableMap();
    }

    private void initEnableDisableMap() {
        this.enabledDisabledTopicMap = new HashMap<>();
        if (!this.topicMap.isEmpty()) {
            for (String key : this.topicMap.keySet()) {
                String[] split = key.split("/");
                if (split.length < 2) {
                    continue;
                }
                Integer id = Integer.valueOf(split[1]);
                this.enabledDisabledTopicMap.put(id, true);
            }
        }
        this.updateMapFromFeatureConfig();
    }

    @SuppressWarnings("unchecked")
    private void updateMapFromFeatureConfig() {
        List<Integer> listOfDisabledInterface = FeatureConfig.getInstance().getListOfDisabledInterface();
        if (!listOfDisabledInterface.isEmpty()) {
            for (Integer id : listOfDisabledInterface) {
                if (this.enabledDisabledTopicMap.containsKey(id)) {
                    this.enabledDisabledTopicMap.put(id, false);
                    continue;
                }
                FCALLogs.getInstance().log.debug("[SDK]: Feature does not contain the interface " + id + ".");
            }
        }
    }

    private void subscribeToEnableDisableTopic() throws MqttException {
        this.client.subscribe("fek/" + WebserviceUtil.getFeatureID() + "/intf/enable", 0);
        this.client.subscribe("fek/" + WebserviceUtil.getFeatureID() + "/intf/disable", 0);
    }

    private void processMultiWriteInterface(MqttMessage message, boolean flag, String topic) {
        JsonObject jsonObject = JsonParser.parseString(message.toString()).getAsJsonObject();
        if (!jsonObject.has("PL")) {
            FCALLogs.getInstance().log.error(
                    "[SDK]: Payload is not available in the MQTT message for write interfaces for the topic : " + topic);
            return;
        }
        JsonElement object = jsonObject.get("PL");
        if (object instanceof JsonArray) {
            JsonArray interfaceID = (JsonArray) object;
            JsonArray arrayOfValues = new JsonArray();
            for (JsonElement id : interfaceID) {
                if (!(id instanceof JsonPrimitive) || !((JsonPrimitive) id).isNumber()) {
                    continue;
                }
                int value = ((JsonPrimitive) id).getAsInt();
                if (this.enabledDisabledTopicMap.containsKey(value)) {
                    this.enabledDisabledTopicMap.put(value, flag);
                    arrayOfValues.add(value);
                    continue;
                }
                FCALLogs.getInstance().log.debug("[SDK]: Interface id " + value + " does not exist");
            }
            this.acknowledgeEnableDisableInterfaces(topic, arrayOfValues);
        }
    }

    private void acknowledgeEnableDisableInterfaces(String topic, JsonArray arrayOfValues) {
        JsonObject jsonObject = new JsonObject();
        jsonObject.add("PL", arrayOfValues);
        jsonObject.addProperty("TS", System.currentTimeMillis());
        MqttMessage message = new MqttMessage(jsonObject.toString().getBytes());
        try {
            this.client.publish(topic, message);
            FCALLogs.getInstance().log.debug(
                    "[SDK]:  Published Topic: " + topic + ", Message: " + message.toString());
        } catch (MqttException e) {
            FCALLogs.getInstance().log.error(
                    "[SDK]: Error while sending acknowledgement to fif topic: " + topic
                            + ", message: " + message.toString());
        }
    }

    @Override
    protected EClass eStaticClass() {
        return FcbPackage.Literals.PUBLISH_CONNECTION_FACTORY;
    }

    @Override
    public MqttAsyncClient getClient() {
        return this.client;
    }

    @Override
    public void setClient(MqttAsyncClient newClient) {
        this.client = newClient;
    }

    @Override
    public Map<String, String> getTopicMap() {
        return this.topicMap;
    }

    @Override
    public void setTopicMap(Map<String, String> newTopicMap) {
        this.topicMap = newTopicMap;
    }

    @Override
    public Map<String, String> getPayloadMap() {
        return this.payloadMap;
    }

    @Override
    public void setPayloadMap(Map<String, String> newPayloadMap) {
        this.payloadMap = newPayloadMap;
    }

    @Override
    public Map<String, String> getPrefixMap() {
        return this.prefixMap;
    }

    @Override
    public void setPrefixMap(Map<String, String> newPrefixMap) {
        this.prefixMap = newPrefixMap;
    }

    @Override
    public Map<String, String> getMachinePathMap() {
        return this.machinePathMap;
    }

    @Override
    public void setMachinePathMap(Map<String, String> newMachinePathMap) {
        this.machinePathMap = newMachinePathMap;
    }

    @Override
    public List<IMachineProvider> getProviders() {
        if (this.providers == null) {
            this.providers = new EObjectResolvingEList<>(IMachineProvider.class, (InternalEObject) this, 5);
        }
        return this.providers;
    }

    @Override
    public Map<Integer, Boolean> getEnabledDisabledTopicMap() {
        return this.enabledDisabledTopicMap;
    }

    @Override
    public void setEnabledDisabledTopicMap(Map<Integer, Boolean> newEnabledDisabledTopicMap) {
        this.enabledDisabledTopicMap = newEnabledDisabledTopicMap;
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) {
        try {
            if (GracefulFeatureStop.getInstance().isFeatureStopped()) {
                FCALLogs.getInstance().log.debug(
                        "[SDK]:  The feature is going to be stopped, so hardware messages cannot be processed.");
                return;
            }
            if (topic == null || message == null || message.toString().isEmpty()) {
                FCALLogs.getInstance().log.error("[SDK]:  Topic or Payload is null");
                return;
            }
            FCALLogs.getInstance().log.debug("[SDK]: Message received : " + topic + " " + message.toString());
            if (topic.endsWith("intf/enable")) {
                this.processMultiWriteInterface(message, true,
                        "fek/" + WebserviceUtil.getFeatureID() + "/intf/enable/ack");
            } else if (topic.endsWith("intf/disable")) {
                this.processMultiWriteInterface(message, false,
                        "fek/" + WebserviceUtil.getFeatureID() + "/intf/disable/ack");
            }
        } catch (Exception e) {
            FCALLogs.getInstance().log.error("[SDK]:  Exception while processing the message "
                    + topic + " : " + message + " : " + ExceptionUtils.getRootCauseMessage(e));
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {
    }

    @Override
    public void connectionLost(Throwable cause) {
        FCALLogs.getInstance().log.info("[SDK]:  FCAL Message Broker connection lost-Trying to reconnect");
    }

    @Override
    public void initConnectionFactory() throws CommunicationException {
        if (!this.init) {
            try {
                this.initMappings();
                if (this.topicMap.isEmpty()) {
                    FCALLogs.getInstance().log.debug("[SDK]: Feature topics are not available");
                }
                if (FeatureConfig.MqttAuthType.TLS_CRED.equals(FeatureConfig.getInstance().getMqttAuthType())) {
                    this.createTLSCredConnection();
                } else if (FeatureConfig.MqttAuthType.CREDENTIALS.equals(FeatureConfig.getInstance().getMqttAuthType())) {
                    this.createPwdConnection();
                } else if (FeatureConfig.MqttAuthType.TLS.equals(FeatureConfig.getInstance().getMqttAuthType())) {
                    this.createTLSConnection();
                } else if (FeatureConfig.MqttAuthType.NONE.equals(FeatureConfig.getInstance().getMqttAuthType())) {
                    if ("PROD".equalsIgnoreCase(FeatureConfig.getInstance().getEnvironment())) {
                        FCALLogs.getInstance().log.incident(
                                "[SDK]: INC_011_04: Basic connection without certificates or credentials is requested, which is not intended.");
                    } else {
                        this.createNormalConnection();
                    }
                }
                this.subscribeToEnableDisableTopic();
                boolean connected = this.client.isConnected();
                if (connected) {
                    FCALLogs.getInstance().log.debug("[SDK]:  FCAL Subscribe Client Connected");
                }
            } catch (Exception e) {
                throw new CommunicationException(
                        "Exception while creating MQTT client to enable communication between Application and device.",
                        e);
            }
            this.init = true;
        }
    }

    @Override
    public com.bosch.nevonex.common.ITopicObject getTopicElement(IMachineProvider provider, String topic, String elementPath) {
        return null;
    }

    @Override
    public void connectComplete(boolean reconnect, String serverURL) {
        if (reconnect) {
            try {
                this.subscribeToEnableDisableTopic();
            } catch (MqttException e) {
                FCALLogs.getInstance().log.error(
                        "[SDK]: Exception while subscribing enable and disable topics for multi write interface to broker after re-connection. "
                                + ExceptionUtils.getRootCauseMessage(e));
            }
            FCALLogs.getInstance().log.info("[SDK]:  FCAL Message Broker connection established again");
        }
    }

    @Override
    public void terminateConnectionFactory() throws NevonexException {
        try {
            if (this.client.isConnected()) {
                this.client.disconnect();
                this.client.close();
            }
        } catch (Exception e) {
            FCALLogs.getInstance().log.debug(
                    "[SDK]: Exception occurred while disconnecting the mqtt client: "
                            + ExceptionUtils.getRootCauseMessage(e));
        }
    }

    @Override
    public Object eGet(int featureID, boolean resolve, boolean coreType) {
        switch (featureID) {
            case 0:
                return this.getClient();
            case 1:
                return this.getTopicMap();
            case 2:
                return this.getPayloadMap();
            case 3:
                return this.getPrefixMap();
            case 4:
                return this.getMachinePathMap();
            case 5:
                return this.getProviders();
            case 6:
                return this.getEnabledDisabledTopicMap();
        }
        return super.eGet(featureID, resolve, coreType);
    }

    @SuppressWarnings("unchecked")
    @Override
    public void eSet(int featureID, Object newValue) {
        switch (featureID) {
            case 0:
                this.setClient((MqttAsyncClient) newValue);
                return;
            case 1:
                this.setTopicMap((Map<String, String>) newValue);
                return;
            case 2:
                this.setPayloadMap((Map<String, String>) newValue);
                return;
            case 3:
                this.setPrefixMap((Map<String, String>) newValue);
                return;
            case 4:
                this.setMachinePathMap((Map<String, String>) newValue);
                return;
            case 5:
                this.getProviders().clear();
                this.getProviders().addAll((java.util.Collection<? extends IMachineProvider>) newValue);
                return;
            case 6:
                this.setEnabledDisabledTopicMap((Map<Integer, Boolean>) newValue);
                return;
        }
        super.eSet(featureID, newValue);
    }

    @Override
    public void eUnset(int featureID) {
        switch (featureID) {
            case 0:
                this.setClient(CLIENT_EDEFAULT);
                return;
            case 1:
                this.setTopicMap(null);
                return;
            case 2:
                this.setPayloadMap(null);
                return;
            case 3:
                this.setPrefixMap(null);
                return;
            case 4:
                this.setMachinePathMap(null);
                return;
            case 5:
                this.getProviders().clear();
                return;
            case 6:
                this.setEnabledDisabledTopicMap(null);
                return;
        }
        super.eUnset(featureID);
    }

    @Override
    public boolean eIsSet(int featureID) {
        switch (featureID) {
            case 0:
                return CLIENT_EDEFAULT == null ? this.client != null : !CLIENT_EDEFAULT.equals(this.client);
            case 1:
                return this.topicMap != null;
            case 2:
                return this.payloadMap != null;
            case 3:
                return this.prefixMap != null;
            case 4:
                return this.machinePathMap != null;
            case 5:
                return this.providers != null && !this.providers.isEmpty();
            case 6:
                return this.enabledDisabledTopicMap != null;
        }
        return super.eIsSet(featureID);
    }

    @Override
    public Object eInvoke(int operationID, EList<?> arguments) throws java.lang.reflect.InvocationTargetException {
        switch (operationID) {
            case 0:
                this.messageArrived((String) arguments.get(0), (MqttMessage) arguments.get(1));
                return null;
            case 1:
                this.deliveryComplete((IMqttDeliveryToken) arguments.get(0));
                return null;
            case 2:
                this.connectionLost((Throwable) arguments.get(0));
                return null;
            case 3:
                try {
                    this.initConnectionFactory();
                    return null;
                } catch (Throwable throwable) {
                    throw new java.lang.reflect.InvocationTargetException(throwable);
                }
            case 4:
                return this.getTopicElement((IMachineProvider) arguments.get(0),
                        (String) arguments.get(1), (String) arguments.get(2));
            case 5:
                this.connectComplete((Boolean) arguments.get(0), (String) arguments.get(1));
                return null;
            case 6:
                try {
                    this.terminateConnectionFactory();
                    return null;
                } catch (Throwable throwable) {
                    throw new java.lang.reflect.InvocationTargetException(throwable);
                }
        }
        return super.eInvoke(operationID, arguments);
    }

    @Override
    public String toString() {
        if (this.eIsProxy()) {
            return super.toString();
        }
        StringBuffer result = new StringBuffer(super.toString());
        result.append(" (client: ");
        result.append(this.client);
        result.append(", topicMap: ");
        result.append(this.topicMap);
        result.append(", payloadMap: ");
        result.append(this.payloadMap);
        result.append(", prefixMap: ");
        result.append(this.prefixMap);
        result.append(", machinePathMap: ");
        result.append(this.machinePathMap);
        result.append(", enabledDisabledTopicMap: ");
        result.append(this.enabledDisabledTopicMap);
        result.append(')');
        return result.toString();
    }

    // ILogger / LoggerConstants symbolic refs (kept for import preservation parity with constpool entries).
    // Referenced in dispatcher source for completeness; do not remove without re-validating constpool delta.
    private static final Class<?> __LOGGER_REF = ILogger.class;
    private static final Class<?> __LOGGER_CONST_REF = LoggerConstants.class;
    private static final Class<?> __FC_CONST_REF = FeatureConfigConstants.class;
}
