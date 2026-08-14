/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.sdk.test;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.StringWriter;
import java.util.Properties;
import java.util.Timer;
import java.util.HashSet;
import java.util.Set;
import java.util.Arrays;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.runtime.util.internal.FeatureConfig;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import com.google.gson.JsonObject;
import com.google.gson.JsonArray;
import com.google.gson.Gson;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

import java.util.logging.Level;
import com.bosch.nevonex.common.impl.AbsolutePosition;
import com.bosch.nevonex.sdk.test.TestCustomUI; 

public class TestFilClient implements MqttCallback, MqttCallbackExtended {
    
  /**
   *
   */
  public static final String FCAL_F = "fek/#";
  /**
   *
   */
  public static final String TOPIC_CREATION = "fek/0/";
  
  
  public static String payload = "";
  
  
  public static final Set<String> interfacesSet = new HashSet<>(Arrays.asList(new String[] {"CAN_AGMO_Customized_Tractor", "CAN_AGMO_MTLT305", "CAN_AGMO_SteerMotor", "ISOPGN", "Serial_Ext_GPS_NMEA0183"}));
   /**
   * <!-- begin-user-doc --> <!-- end-user-doc -->
   *
   * @generated
   */
  protected TestFilClient() {
    super();

    try {
      init();
    }
    catch (MqttException e) {
      e.printStackTrace();
    }
  }
    
    private String broker = "tcp://localhost:1883";
    
    private static TestFilClient factory;
    
    static {
        factory = new TestFilClient();
    }
    
    private MqttAsyncClient client;
    private final String[] topicArray = { FCAL_F };
    
    /**
     * @throws MqttException
     */
    private void init() throws MqttException {
    
        Properties pros = new Properties();
        FileInputStream inStream = null;
        try {
          File file = new File("./connection.props");
          if (file.exists()) {
            inStream = new FileInputStream(file);
            pros.load(inStream);
          }
        }
        catch (Exception e) {
			  FCALLogs.getInstance().log.info("Exception while reading the connection properties. " + e.getMessage());
        }
        finally {
          if (inStream != null) {
            try {
              inStream.close();
            }
            catch (IOException e) {
              // Ignore exception
            }
          }
        }
    
        this.broker = pros.getProperty("broker");
        if (this.broker == null) {
          this.broker = "tcp://localhost:1883";
        }
        this.client = new MqttAsyncClient(this.broker, String.valueOf(System.nanoTime()), new MemoryPersistence());
    
        IMqttToken token = this.client.connect(createConnectionOptions());
        token.waitForCompletion((long) 60 * 1000); // wait for 60 s = 1 min
        this.client.setCallback(this);
        this.client.subscribe(this.topicArray, new int[topicArray.length]);
    }
    
    private MqttConnectOptions createConnectionOptions() {
        MqttConnectOptions connOpts = new MqttConnectOptions();
        connOpts.setMaxInflight(1000);
        connOpts.setCleanSession(true); // no persistent session
        connOpts.setKeepAliveInterval(1000);
        connOpts.setAutomaticReconnect(true);
        return connOpts;
    }
    
    public static synchronized TestFilClient getInstance() {
        return factory;
    }
    
    /**
     * {@inheritDoc}
     */
    @Override
    public void connectionLost(final Throwable arg0) {
		FCALLogs.getInstance().log.info("Test FIl Client message broker connection is lost. Trying to re-connect.");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void deliveryComplete(final IMqttDeliveryToken arg0) {
        // Nothing to implement

    }
    
    	@Override
	public void connectComplete(boolean reconnect, String serverUrl) {
		if (reconnect) {
				FCALLogs.getInstance().log.info("Test FIl Client message broker connection established again.");
			try {
	        this.client.subscribe(this.topicArray, new int[topicArray.length]);
			} catch (MqttException e) {
			  FCALLogs.getInstance().log.error("Exception while sunscribing test fil client topics to the broker after re-connection. " + e.getMessage());
			}
		}
	}
    

    /**
     * {@inheritDoc}
     */
    @Override
    public void messageArrived(final String arg0, final MqttMessage arg1) throws Exception {
        payload = arg1.toString();
    }

    /**
     * @param string
     */
    public void publish(final String string, final String payload, final int qos, final boolean retained)
            throws Exception {
        this.client.publish(string, payload.getBytes(), qos, retained);
    }

    /**
     * @param string
     */
    public void publish(final String string, final String payload) throws Exception {
        this.client.publish(string, new MqttMessage(payload.getBytes()));
    } 

    
    public void publishCAN_AGMO_Customized_Tractorreceive_ACC_INFO(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject receive_ACC_INFO = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            receive_ACC_INFO.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	receive_ACC_INFO.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            receive_ACC_INFO.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_customized_tractor = new JsonObject();
        can_agmo_customized_tractor.add("can_agmo_customized_tractor", receive_ACC_INFO);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_customized_tractor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_Customized_Tractorreceive_FNR_INFO(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject receive_FNR_INFO = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            receive_FNR_INFO.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	receive_FNR_INFO.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            receive_FNR_INFO.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_customized_tractor = new JsonObject();
        can_agmo_customized_tractor.add("can_agmo_customized_tractor", receive_FNR_INFO);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_customized_tractor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_Customized_Tractorreceive_HYD_INFO(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject receive_HYD_INFO = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            receive_HYD_INFO.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	receive_HYD_INFO.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            receive_HYD_INFO.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_customized_tractor = new JsonObject();
        can_agmo_customized_tractor.add("can_agmo_customized_tractor", receive_HYD_INFO);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_customized_tractor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_Customized_Tractorreceive_SFT_INFO(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject receive_SFT_INFO = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            receive_SFT_INFO.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	receive_SFT_INFO.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            receive_SFT_INFO.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_customized_tractor = new JsonObject();
        can_agmo_customized_tractor.add("can_agmo_customized_tractor", receive_SFT_INFO);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_customized_tractor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_MTLT305aceinna_Accel(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject aceinna_Accel = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            aceinna_Accel.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	aceinna_Accel.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            aceinna_Accel.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_mtlt305 = new JsonObject();
        can_agmo_mtlt305.add("can_agmo_mtlt305", aceinna_Accel);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_mtlt305);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_MTLT305aceinna_AngleRate(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject aceinna_AngleRate = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            aceinna_AngleRate.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	aceinna_AngleRate.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            aceinna_AngleRate.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_mtlt305 = new JsonObject();
        can_agmo_mtlt305.add("can_agmo_mtlt305", aceinna_AngleRate);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_mtlt305);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_MTLT305aceinna_Angles(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject aceinna_Angles = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            aceinna_Angles.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	aceinna_Angles.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            aceinna_Angles.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_mtlt305 = new JsonObject();
        can_agmo_mtlt305.add("can_agmo_mtlt305", aceinna_Angles);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_mtlt305);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_SteerMotormotor_Heartbeat(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject motor_Heartbeat = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            motor_Heartbeat.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	motor_Heartbeat.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            motor_Heartbeat.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_steermotor = new JsonObject();
        can_agmo_steermotor.add("can_agmo_steermotor", motor_Heartbeat);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_steermotor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_SteerMotormotor_Response_Encoder_Speed(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject motor_Response_Encoder_Speed = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            motor_Response_Encoder_Speed.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	motor_Response_Encoder_Speed.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            motor_Response_Encoder_Speed.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_steermotor = new JsonObject();
        can_agmo_steermotor.add("can_agmo_steermotor", motor_Response_Encoder_Speed);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_steermotor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishCAN_AGMO_SteerMotormotor_Response_EncoderCountValue(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject motor_Response_EncoderCountValue = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            motor_Response_EncoderCountValue.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	motor_Response_EncoderCountValue.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            motor_Response_EncoderCountValue.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject can_agmo_steermotor = new JsonObject();
        can_agmo_steermotor.add("can_agmo_steermotor", motor_Response_EncoderCountValue);     
        JsonObject root = new JsonObject();
        root.add("PL", can_agmo_steermotor);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishISOPGNhitchPosPerRe(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject hitchPosPerRe = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            hitchPosPerRe.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	hitchPosPerRe.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            hitchPosPerRe.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject isopgn = new JsonObject();
        isopgn.add("isopgn", hitchPosPerRe);     
        JsonObject root = new JsonObject();
        root.add("PL", isopgn);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    public void publishSerial_Ext_GPS_NMEA0183serial_Ext_GPS_NMEA0183_Data(String interfaceAddress, Object value, String index, String parentIndex) throws Exception {
        String address = interfaceAddress.split("/")[1];
        JsonObject serial_Ext_GPS_NMEA0183_Data = new JsonObject();
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            serial_Ext_GPS_NMEA0183_Data.add(index, gps);
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	Object[] values = ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
	        	serial_Ext_GPS_NMEA0183_Data.add(index,  new Gson().toJsonTree(values, Object.class));
        } else {
            serial_Ext_GPS_NMEA0183_Data.add(index, new Gson().toJsonTree(value, Object.class));
        }
        JsonObject serial_ext_gps_nmea0183 = new JsonObject();
        serial_ext_gps_nmea0183.add("serial_ext_gps_nmea0183", serial_Ext_GPS_NMEA0183_Data);     
        JsonObject root = new JsonObject();
        root.add("PL", serial_ext_gps_nmea0183);
        root.addProperty("TS", System.currentTimeMillis());
        publish("fek/" + address, root.toString(), 2, false);
        FCALLogs.getInstance().log.info("publish to fek/" + address + " Payload : " + root.toString());
    }
    
    
    public void publishValue(String interfaceAdress, Object value, String index, String parentIndex) throws Exception {
        switch (interfaceAdress) {
        case "CAN_AGMO_Customized_Tractor/9455":
           factory.publishCAN_AGMO_Customized_Tractorreceive_ACC_INFO(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_Customized_Tractor/9456":
           factory.publishCAN_AGMO_Customized_Tractorreceive_FNR_INFO(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_Customized_Tractor/9457":
           factory.publishCAN_AGMO_Customized_Tractorreceive_HYD_INFO(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_Customized_Tractor/9459":
           factory.publishCAN_AGMO_Customized_Tractorreceive_SFT_INFO(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_MTLT305/9440":
           factory.publishCAN_AGMO_MTLT305aceinna_Accel(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_MTLT305/9441":
           factory.publishCAN_AGMO_MTLT305aceinna_AngleRate(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_MTLT305/9442":
           factory.publishCAN_AGMO_MTLT305aceinna_Angles(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_SteerMotor/9428":
           factory.publishCAN_AGMO_SteerMotormotor_Heartbeat(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_SteerMotor/9434":
           factory.publishCAN_AGMO_SteerMotormotor_Response_Encoder_Speed(interfaceAdress, value, index, parentIndex);
           break;
        case "CAN_AGMO_SteerMotor/9435":
           factory.publishCAN_AGMO_SteerMotormotor_Response_EncoderCountValue(interfaceAdress, value, index, parentIndex);
           break;
        case "ISOPGN/251":
           factory.publishISOPGNhitchPosPerRe(interfaceAdress, value, index, parentIndex);
           break;
        case "Serial_Ext_GPS_NMEA0183/9465":
           factory.publishSerial_Ext_GPS_NMEA0183serial_Ext_GPS_NMEA0183_Data(interfaceAdress, value, index, parentIndex);
           break;

        default:
            break;
        }
    }

    public synchronized void createDom(String topic, String fileName) throws IOException, Exception {
        System.out.println("Initiated Creation of SDK");
        File file = new File(fileName);
        StringWriter st = new StringWriter();
        DocumentBuilder buider = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document document =  buider.parse(file);
        Node firstNode = document.getFirstChild();
        if (!interfacesSet.contains(firstNode.getNodeName())) {
            document.renameNode(document.getFirstChild(), null, "root");            
        }
        Transformer trans = TransformerFactory.newInstance().newTransformer();
        trans.transform(new DOMSource(document), new StreamResult(st));
        JsonObject obj = new JsonObject();
        obj.addProperty("PL", st.toString());
        publish(topic, obj.toString(), 2, false);
    }


    public void simulateData(TestFilClient obj) throws Exception {
        UpdateTask updateTask = new UpdateTask(obj);
        Timer timer = new Timer();
        timer.schedule(updateTask, 1000, 10000);
    }
    
    public static void main(final String[] args) throws Exception {
        TestFilClient filClient = new TestFilClient();
        FeatureConfig.getInstance().loadFeatureConfiguration();        
        TestCustomUI customUITest=new TestCustomUI();
        customUITest.mockServices(); 
        filClient.createDom(TestFilClient.TOPIC_CREATION, "./data/sample_data.xml");
        Thread.sleep(2 * 1000);
        filClient.simulateData(filClient);
    }
    
}