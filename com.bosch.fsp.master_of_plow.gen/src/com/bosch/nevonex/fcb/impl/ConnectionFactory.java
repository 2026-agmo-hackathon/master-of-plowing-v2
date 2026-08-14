package com.bosch.nevonex.fcb.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.ILogger;
import com.bosch.fsp.logger.LoggerConstants;
import com.bosch.fsp.platform.InterfaceDetails;
import com.bosch.fsp.runtime.feature.GracefulFeatureStop;
import com.bosch.fsp.runtime.feature.IMachine;
import com.bosch.fsp.runtime.feature.IMachineProvider;
import com.bosch.fsp.runtime.feature.MachineConnectionInfo;
import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.MachineElementNotFoundException;
import com.bosch.fsp.runtime.feature.exception.MachineReadException;
import com.bosch.fsp.runtime.feature.exception.NevonexException;
import com.bosch.fsp.runtime.feature.notification.MqttFeatureNotification;
import com.bosch.fsp.runtime.feature.notification.Notification;
import com.bosch.fsp.runtime.feature.notification.SslUtil;
import com.bosch.fsp.runtime.util.internal.FeatureConfig;
import com.bosch.fsp.runtime.util.internal.FeatureConfigConstants;
import com.bosch.fsp.runtime.util.internal.WebserviceUtil;
import com.bosch.nevonex.common.ITopicObject;
import com.bosch.nevonex.common.ProviderEnum;
import com.bosch.nevonex.fcb.IConnectionFactory;
import com.bosch.nevonex.fcb.IFCALController;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.LinkedBlockingDeque;
import javax.net.ssl.SSLSocketFactory;
import org.apache.commons.lang3.exception.ExceptionUtils;
import org.eclipse.emf.common.util.EList;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EStructuralFeature;
import org.eclipse.emf.ecore.InternalEObject;
import org.eclipse.emf.ecore.impl.EObjectImpl;
import org.eclipse.emf.ecore.util.EObjectResolvingEList;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttMessageListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class ConnectionFactory extends EObjectImpl implements IConnectionFactory {

    protected static final MqttAsyncClient CLIENT_EDEFAULT = null;

    protected MqttAsyncClient client = CLIENT_EDEFAULT;
    protected Map<String, String> topicMap;
    protected Map<String, String> payloadMap;
    protected Map<String, String> prefixMap;
    protected Map<String, String> machinePathMap;
    protected EList<IMachineProvider> providers;

    private static IConnectionFactory instance;
    private boolean init = false;

    private Map<String, Deque<String>> dequeMap = new HashMap<String, Deque<String>>();
    private Map<String, Thread> threadMap = new HashMap<String, Thread>();
    private Map<String, ITopicObject> topicElementMap = new HashMap<String, ITopicObject>();
    private Map<String, IMqttMessageListener> pfServiceMap = new HashMap<String, IMqttMessageListener>();
    private List<String> topicList = new ArrayList<String>();

    private ConnectionFactory() throws CommunicationException {
    }

    public static IConnectionFactory getInstance() {
        if (instance != null) {
            return instance;
        }
        synchronized (ConnectionFactory.class) {
            if (instance != null) {
                return instance;
            }
            try {
                instance = new ConnectionFactory();
            } catch (CommunicationException e) {
                throw new RuntimeException("Exception while creating communication broker.", e);
            }
            return instance;
        }
    }

    public void registerPlatformService(String topic, IMqttMessageListener listener) {
        this.subscribeUsingListener(topic, listener, true);
    }

    private void subscribeUsingListener(String topic, IMqttMessageListener listener, boolean register) {
        if (topic == null) {
            FCALLogs.getInstance().log.debug("[SDK]: Platform service topic cannot be null");
            return;
        }
        try {
            if (listener == null) {
                if (register) {
                    this.topicList.add(topic);
                }
                if (this.client != null) {
                    this.client.subscribe(topic, 0);
                }
            } else {
                if (register) {
                    this.pfServiceMap.put(topic, listener);
                }
                if (this.client != null) {
                    this.client.subscribe(topic, 0, listener);
                }
            }
        } catch (Exception e) {
            FCALLogs.getInstance().log.error("[SDK]: Exception while subscribing to the platform service topic '" + topic + "'. " + e.getMessage());
        }
    }

    private void processMessage(String topic, String message) throws MachineReadException {
        try {
            String inTopicSuffix = topic;
            for (String prefix : this.prefixMap.values()) {
                inTopicSuffix = inTopicSuffix.replace(prefix, "");
            }
            String attribute = this.topicMap.get(inTopicSuffix);
            if (attribute != null) {
                IMachineProvider provider = null;
                String[] keys = attribute.split("\\.");
                String machine = keys[0];
                String machinePath = null;
                for (Map.Entry<String, String> pathEntry : this.machinePathMap.entrySet()) {
                    if (!pathEntry.getKey().equalsIgnoreCase(machine)) {
                        continue;
                    }
                    machinePath = pathEntry.getValue();
                    break;
                }
                if (machinePath == null) {
                    FCALLogs.getInstance().log.error("[SDK]: machine path is null");
                    return;
                }
                String[] pathTokens = machinePath.split("\\.");
                String root = "";
                if (pathTokens.length > 0) {
                    root = pathTokens[0];
                }
                for (IMachineProvider pr : this.providers) {
                    if (!pr.getClass().getSimpleName().equalsIgnoreCase(root + "Provider")) {
                        continue;
                    }
                    provider = pr;
                    break;
                }
                if (provider == null) {
                    FCALLogs.getInstance().log.error("[SDK]:  provider is null");
                    return;
                }
                EStructuralFeature controllerFeature = provider.eClass().getEStructuralFeature("controller");
                IFCALController controller = null;
                if (controllerFeature != null) {
                    controller = (IFCALController) provider.eGet(controllerFeature);
                }
                if (controller == null) {
                    FCALLogs.getInstance().log.error("[SDK]:  Controller is null");
                    return;
                }
                if (root.toLowerCase().equalsIgnoreCase("implement") && !message.contains("\"" + root.toLowerCase() + "\"")) {
                    FCALLogs.getInstance().log.error("[SDK]:  Payload is not proper for the selected interface. Root machine name '" + root.toLowerCase() + "' is not found in the payload.\n" + topic + " : " + message);
                    return;
                }
                JsonObject jsonObject = JsonParser.parseString(message.toString()).getAsJsonObject();
                JsonElement object = jsonObject.get("PL");
                long timestamp = jsonObject.get("TS").getAsLong();
                LinkedHashMap<String, Object> pathToValueMap = new LinkedHashMap<String, Object>();
                this.processPayload(object, pathToValueMap, "", root.toLowerCase());
                for (Map.Entry<String, Object> pathToValue : pathToValueMap.entrySet()) {
                    String path = pathToValue.getKey();
                    Object value = pathToValue.getValue();
                    ITopicObject machineElement = this.getTopicElement(provider, path, machinePath);
                    if (machineElement != null) {
                        InterfaceDetails details = (InterfaceDetails) machineElement.getInterfaceDetailMap().get(topic);
                        if (details.getTimestmap() <= timestamp) {
                            controller.setMachineUpdate(machineElement, keys[keys.length - 2], value, details, timestamp);
                            continue;
                        }
                        FCALLogs.getInstance().log.warn("[SDK]: Old or duplicate value is received for the interface : " + keys[keys.length - 2]);
                        continue;
                    }
                    throw new MachineElementNotFoundException("Machine is not available in the specified path : " + path);
                }
            }
        } catch (Exception e) {
            throw new MachineReadException("Exception Occurred While Processing MqttMessage " + topic + " : " + message, e);
        }
    }

    private void processMachineConnection(String topic, JsonElement jsonElement, long timestamp) {
        if (jsonElement != null && jsonElement.isJsonObject()) {
            JsonObject object = jsonElement.getAsJsonObject();
            for (Map.Entry<String, JsonElement> entry : object.entrySet()) {
                JsonElement element = entry.getValue();
                if (!element.isJsonArray()) {
                    continue;
                }
                JsonArray array = element.getAsJsonArray();
                MachineConnectionInfo info = new MachineConnectionInfo();
                int index = Integer.parseInt(entry.getKey());
                String name = topic.substring("fek/1/".length());
                info.setConnectionStatus(MachineConnectionInfo.CONNECTION_STATUS.values()[array.get(0).getAsInt()]);
                info.setErrorStatus(MachineConnectionInfo.ERROR_STATUS.values()[array.get(1).getAsInt()]);
                for (IMachineProvider provider : this.providers) {
                    String machineName = provider.getClass().getSimpleName().replace("Provider", "");
                    if (!machineName.equalsIgnoreCase(name)) {
                        continue;
                    }
                    Object machineElement = provider.getTopicElement(name.toLowerCase() + "/" + index);
                    if (machineElement instanceof IMachine) {
                        provider.onConnectionStatusChange((IMachine) machineElement, info);
                        JsonObject machines = new JsonObject();
                        machines.addProperty("name", machineName);
                        machines.addProperty("index", index);
                        JsonObject machineConnectionDetails = new JsonObject();
                        machineConnectionDetails.addProperty("code", info.getConnectionStatusCode());
                        machineConnectionDetails.addProperty("featureID", WebserviceUtil.getFeatureID());
                        machineConnectionDetails.add("machine", machines);
                        machineConnectionDetails.addProperty("reason", info.getErrorStatus().toString());
                        MqttMessage message = new MqttMessage(machineConnectionDetails.toString().getBytes());
                        try {
                            this.client.publish("fek/notification", message);
                        } catch (MqttException e) {
                            FCALLogs.getInstance().log.error("[SDK]: Error occurred while sending machine connection notification to fif. " + e.getMessage());
                        }
                        continue;
                    }
                    FCALLogs.getInstance().log.info("[SDK]:  Machine " + name + " with index " + index + " not available to notify connection change: " + info);
                }
            }
        }
    }

    private String removePrefixes(String topic) {
        String suffix = topic;
        for (String value : this.prefixMap.values()) {
            suffix = suffix.replace(value, "");
        }
        if (suffix.startsWith("/")) {
            suffix = suffix.substring(1);
        }
        return suffix;
    }

    private void processPayload(Object object, Map<String, Object> pathToValueMap, String parentKey, String root) {
        if (object instanceof JsonObject) {
            JsonObject jObject = (JsonObject) object;
            this.processPayloadObject(pathToValueMap, jObject, parentKey, root);
        } else if (object instanceof JsonArray) {
            JsonArray array = (JsonArray) object;
            if (this.isValueArray(array)) {
                pathToValueMap.put(parentKey, array);
            } else {
                for (Object arrayElement : array) {
                    this.processPayload(arrayElement, pathToValueMap, parentKey, root);
                }
            }
        } else {
            pathToValueMap.put(parentKey + 0, object);
        }
    }

    private boolean isValueArray(JsonArray array) {
        if (array != null) {
            for (Object object : array) {
                if (!(object instanceof JsonObject)) {
                    continue;
                }
                return false;
            }
            return true;
        }
        return false;
    }

    private void processPayloadObject(Map<String, Object> pathToValueMap, JsonObject jObject, String parentKey, String root) {
        Set<String> keys = jObject.keySet();
        String pkey = parentKey;
        for (String key : keys) {
            JsonElement value;
            if (key.equals("index")) {
                continue;
            }
            if (jObject.get(root) != null && jObject.get(root).isJsonPrimitive()) {
                if (keys.contains(root) && Integer.valueOf(jObject.get(root).getAsInt()) instanceof Integer && !pkey.contains(root)) {
                    pkey = pkey + root + "/" + jObject.get(root).getAsInt();
                }
            } else if (keys.contains("index")) {
                pkey = pkey + "/" + jObject.get("index").getAsInt();
            }
            value = jObject.get(key);
            if (value instanceof JsonObject) {
                this.processPayloadObject(pathToValueMap, (JsonObject) value, pkey + "/" + key, root);
                continue;
            }
            if (value instanceof JsonArray) {
                JsonArray array = (JsonArray) value;
                if (this.isValueArray(array)) {
                    pathToValueMap.put(pkey + "/" + key, value);
                    continue;
                }
                this.processPayload(value, pathToValueMap, pkey + "/" + key, root);
                continue;
            }
            if (key.equals("index") || key.equals(root)) {
                continue;
            }
            pathToValueMap.put(pkey + "/" + key, value);
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
        SSLSocketFactory socketFactory = SslUtil.getSocketFactory(FeatureConfig.getInstance().getCaCert(), FeatureConfig.getInstance().getClientCert(), FeatureConfig.getInstance().getClientKey(), "");
        options.setSocketFactory(socketFactory);
        options.setHttpsHostnameVerificationEnabled(false);
        this.tryConnectWithRetry(options, 3);
        this.client.setCallback(this);
        FCALLogs.getInstance().log.info("[SDK]: " + this.getClass().getSimpleName() + ": Connection using certificates is successful");
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
            this.client.setCallback(this);
            FCALLogs.getInstance().log.info("[SDK]: " + this.getClass().getSimpleName() + ": Connection using credentials is successful");
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
            this.client.setCallback(this);
            FCALLogs.getInstance().log.info("[SDK]: " + this.getClass().getSimpleName() + ": Basic connection is successful");
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
                    FCALLogs.getInstance().log.warn("[SDK]: " + this.getClass().getSimpleName() + "MQTT connect attempt " + (attempt + 1) + " failed, retrying in " + (delay_ms / 1000) + "s. Error: " + e.getMessage());
                    try {
                        Thread.sleep(delay_ms);
                        continue;
                    } catch (InterruptedException ie) {
                        Thread.currentThread().interrupt();
                        throw ie;
                    }
                }
                FCALLogs.getInstance().log.error("[SDK]: " + this.getClass().getSimpleName() + "MQTT connect failed after " + (maxRetries + 1) + " attempts. Error: " + e.getMessage());
                throw e;
            }
        }
    }

    private void initMappings() {
        this.topicMap = new HashMap<String, String>();
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
        this.payloadMap = new HashMap<String, String>();

        this.prefixMap = new HashMap<String, String>();
		prefixMap.put("FCAL2FIL_Subscribe", "fek");
		prefixMap.put("FCAL2FIL_Publish", "fek");
        this.machinePathMap = new HashMap<String, String>();
		machinePathMap.put("CAN_AGMO_Customized_Tractor", "CAN_AGMO_Customized_Tractor");
		machinePathMap.put("CAN_AGMO_MTLT305", "CAN_AGMO_MTLT305");
		machinePathMap.put("CAN_AGMO_SteerMotor", "CAN_AGMO_SteerMotor");
		machinePathMap.put("ISOPGN", "ISOPGN");
		machinePathMap.put("Platform_Service", "Platform_Service");
		machinePathMap.put("Serial_Ext_GPS_NMEA0183", "Serial_Ext_GPS_NMEA0183");
    }

    @Override
    protected EClass eStaticClass() {
        return FcbPackage.Literals.CONNECTION_FACTORY;
    }

    public MqttAsyncClient getClient() {
        return this.client;
    }

    public void setClient(MqttAsyncClient newClient) {
        this.client = newClient;
    }

    public Map<String, String> getTopicMap() {
        return this.topicMap;
    }

    public void setTopicMap(Map<String, String> newTopicMap) {
        this.topicMap = newTopicMap;
    }

    public Map<String, String> getPayloadMap() {
        return this.payloadMap;
    }

    public void setPayloadMap(Map<String, String> newPayloadMap) {
        this.payloadMap = newPayloadMap;
    }

    public Map<String, String> getPrefixMap() {
        return this.prefixMap;
    }

    public void setPrefixMap(Map<String, String> newPrefixMap) {
        this.prefixMap = newPrefixMap;
    }

    public Map<String, String> getMachinePathMap() {
        return this.machinePathMap;
    }

    public void setMachinePathMap(Map<String, String> newMachinePathMap) {
        this.machinePathMap = newMachinePathMap;
    }

    public List<IMachineProvider> getProviders() {
        if (this.providers == null) {
            this.providers = new EObjectResolvingEList<IMachineProvider>(IMachineProvider.class, (InternalEObject) this, 5);
        }
        return this.providers;
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) {
        try {
            if (GracefulFeatureStop.getInstance().isFeatureStopped()) {
                FCALLogs.getInstance().log.debug("[SDK]: The feature is going to be stopped, so hardware messages cannot be processed.");
                return;
            }
            if (topic == null) {
                FCALLogs.getInstance().log.error("[SDK]:  Topic is null");
                return;
            }
            if (message == null) {
                FCALLogs.getInstance().log.warn("[SDK]:  Payload is null for the topic : " + topic);
                return;
            }
            if (message.toString().isEmpty()) {
                FCALLogs.getInstance().log.error("[SDK]:  Payload is empty for the topic : " + topic);
                return;
            }
            JsonElement jsonElement = JsonParser.parseString(message.toString());
            if (jsonElement.isJsonNull()) {
                FCALLogs.getInstance().log.error("[SDK]:  MQTT message is null");
                return;
            }
            JsonObject jsonObject = jsonElement.getAsJsonObject();
            boolean payload = jsonObject.has("PL");
            if (!payload) {
                FCALLogs.getInstance().log.error("[SDK]:  Payload is not available in the MQTT message for the topic : " + topic);
                return;
            }
            JsonElement pl = jsonObject.get("PL");
            if (pl.isJsonNull()) {
                FCALLogs.getInstance().log.warn("[SDK]:  Payload is null for " + topic + " in the MQTT message");
                return;
            }
            if (ProviderEnum.VALUES.isEmpty()) {
                FCALLogs.getInstance().log.info("[SDK]:  Machine Providers are not available.");
            }
            FCALLogs.getInstance().log.debug("[SDK]: Message received : " + topic + " " + message.toString());
            if (topic.startsWith("fek/0/")) {
                String object = jsonObject.get("PL").getAsString();
                byte[] bytes = object.toString().getBytes();
                for (IMachineProvider provider : this.providers) {
                    provider.createMachines(new ByteArrayInputStream(bytes));
                }
                return;
            }
            if (topic.startsWith("fek/1/")) {
                long ts = jsonObject.get("TS").getAsLong();
                this.processMachineConnection(topic, pl, ts);
                return;
            }
            Deque<String> deque = this.dequeMap.get(topic);
            try {
                deque.addLast(message.toString());
                Thread thread = this.threadMap.get(topic);
                if (!thread.isAlive()) {
                    thread.start();
                }
            } catch (Exception e) {
                FCALLogs.getInstance().log.error("[SDK]: Exception while processing the message " + topic + " : " + message + " : " + ExceptionUtils.getRootCauseMessage(e));
            }
        } catch (Exception e) {
            FCALLogs.getInstance().log.error("[SDK]: Exception while processing the message " + topic + " : " + message + " : " + ExceptionUtils.getRootCauseMessage(e));
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {
    }

    @Override
    public void connectionLost(Throwable cause) {
        FCALLogs.getInstance().log.info("[SDK]:  FCAL Message Broker connection lost-Trying to reconnect");
    }

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
                        FCALLogs.getInstance().log.incident("[SDK]: INC_011_04: Basic connection without certificates or credentials is requested, which is not intended.");
                    } else {
                        this.createNormalConnection();
                    }
                }
                ArrayList<String> prefixList = new ArrayList<String>();
                for (Map.Entry<String, String> entry : this.prefixMap.entrySet()) {
                    if (!entry.getKey().toString().endsWith("_Subscribe")) {
                        continue;
                    }
                    prefixList.add(entry.getValue().toString());
                }
                for (Map.Entry<String, String> entry : this.topicMap.entrySet()) {
                    if (!entry.getValue().toString().endsWith(".sub")) {
                        continue;
                    }
                    for (final String prefix : prefixList) {
                        final String suffix = entry.getKey().toString();
                        final String topic = prefix + suffix;
                        this.topicList.add(topic);
                        final LinkedBlockingDeque<String> arrayDeque = new LinkedBlockingDeque<String>();
                        this.dequeMap.put(topic, arrayDeque);
                        Thread thread = new Thread(new Runnable() {
                            @Override
                            public void run() {
                                while (true) {
                                    try {
                                        while (true) {
                                            String message = arrayDeque.takeFirst();
                                            ConnectionFactory.this.processMessage(topic, message);
                                        }
                                    } catch (Exception e) {
                                        FCALLogs.getInstance().log.error("[SDK]:  Exception while processing the message " + ExceptionUtils.getRootCauseMessage(e));
                                        continue;
                                    }
                                }
                            }
                        });
                        this.threadMap.put(topic, thread);
                        thread.setDaemon(true);
                    }
                }
                if (!this.topicList.isEmpty()) {
                    this.client.subscribe(this.topicList.toArray(new String[0]), new int[this.topicList.size()]);
                } else {
                    FCALLogs.getInstance().log.info("[SDK]:  FCAL-FIL topics are not subscribed for the broker.");
                }
                if (!this.pfServiceMap.isEmpty()) {
                    for (Map.Entry<String, IMqttMessageListener> entry : this.pfServiceMap.entrySet()) {
                        String key = entry.getKey();
                        IMqttMessageListener listener = entry.getValue();
                        this.subscribeUsingListener(key, listener, false);
                    }
                }
                boolean connected = this.client.isConnected();
                if (connected) {
                    FCALLogs.getInstance().log.debug("[SDK]:  FCAL Subscribe Client Connected");
                    MqttFeatureNotification.getInstance().publishNotification(Notification.FEATURE_CLIENT_STARTED);
                }
            } catch (Exception e) {
                throw new CommunicationException("Exception while creating MQTT client to enable communication between Application and device.", e);
            }
            this.init = true;
        }
    }

    public ITopicObject getTopicElement(IMachineProvider provider, String topic, String elementPath) {
        if (topic == null || elementPath == null) {
            return null;
        }
        if (topic.startsWith("/")) {
            topic = topic.substring(1);
        }
        Object topicElement = provider.getTopicElement(topic);
        if (topicElement instanceof ITopicObject) {
            return (ITopicObject) topicElement;
        }
        return null;
    }

    @Override
    public void connectComplete(boolean reconnect, String serverURL) {
        if (reconnect) {
            FCALLogs.getInstance().log.info("[SDK]:  FCAL Message Broker connection established again");
            if (!this.topicList.isEmpty()) {
                try {
                    this.client.subscribe(this.topicList.toArray(new String[0]), new int[this.topicList.size()]);
                } catch (MqttException e) {
                    FCALLogs.getInstance().log.error("[SDK]:  Exception while subscribing FCAL-FIL topics to broker after re-connection. " + ExceptionUtils.getRootCauseMessage(e));
                }
            } else {
                FCALLogs.getInstance().log.info("[SDK]:  FCAL-FIL Topics are not subscribed for the broker after re-connection.");
            }
            if (!this.pfServiceMap.isEmpty()) {
                for (Map.Entry<String, IMqttMessageListener> entry : this.pfServiceMap.entrySet()) {
                    String key = entry.getKey();
                    IMqttMessageListener listener = entry.getValue();
                    this.subscribeUsingListener(key, listener, false);
                }
            }
        }
    }

    public void terminateConnectionFactory() throws NevonexException {
        try {
            if (this.client.isConnected()) {
                this.client.disconnect();
                this.client.close();
            }
        } catch (Exception e) {
            FCALLogs.getInstance().log.debug("[SDK]: Exception occurred while disconnecting the mqtt client: " + ExceptionUtils.getRootCauseMessage(e));
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
        }
        return super.eGet(featureID, resolve, coreType);
    }

    @Override
    @SuppressWarnings("unchecked")
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
                this.getProviders().addAll((Collection<? extends IMachineProvider>) newValue);
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
        }
        return super.eIsSet(featureID);
    }

    @Override
    public Object eInvoke(int operationID, EList<?> arguments) throws InvocationTargetException {
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
                    throw new InvocationTargetException(throwable);
                }
            case 4:
                return this.getTopicElement((IMachineProvider) arguments.get(0), (String) arguments.get(1), (String) arguments.get(2));
            case 5:
                this.connectComplete((Boolean) arguments.get(0), (String) arguments.get(1));
                return null;
            case 6:
                try {
                    this.terminateConnectionFactory();
                    return null;
                } catch (Throwable throwable) {
                    throw new InvocationTargetException(throwable);
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
        result.append(')');
        return result.toString();
    }

    // ILogger / LoggerConstants / FeatureConfigConstants / InputStream symbolic refs
    // (kept for import preservation parity with constpool entries).
    private static final Class<?> __LOGGER_REF = ILogger.class;
    private static final Class<?> __LOGGER_CONST_REF = LoggerConstants.class;
    private static final Class<?> __FC_CONST_REF = FeatureConfigConstants.class;
    private static final Class<?> __INPUT_STREAM_REF = InputStream.class;
}
