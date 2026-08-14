/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.sdk.test;

import com.bosch.fsp.runtime.util.internal.Util;
import com.bosch.fsp.logger.FCALLogs;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.lang.reflect.Field;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.TimerTask;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import com.bosch.nevonex.common.impl.AbsolutePosition;

import org.eclipse.emf.ecore.EStructuralFeature;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class UpdateTask extends TimerTask {
    
    private TestFilClient client;
    private Map<String, String> machinePath = new HashMap<>();
	private Map<String, Set<Integer>> machineIndices = new HashMap<>();
	private Properties filSimulatorProperty;
    
    public UpdateTask(TestFilClient filClient) throws Exception {
	    client = filClient;
	    filSimulatorProperty=SDKTest.readSimulatorPropertiesFile();
	    SDKTest.initMaps();
			File file = new File("./data/sample_data.xml");
			DocumentBuilder buider = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			Document document = buider.parse(file);
			List<String> machines = new ArrayList<>();
			machinePath.put("CAN_AGMO_Customized_Tractor", "CAN_AGMO_Customized_Tractor");
			machinePath.put("CAN_AGMO_MTLT305", "CAN_AGMO_MTLT305");
			machinePath.put("CAN_AGMO_SteerMotor", "CAN_AGMO_SteerMotor");
			machinePath.put("ISOPGN", "ISOPGN");
			machinePath.put("Serial_Ext_GPS_NMEA0183", "Serial_Ext_GPS_NMEA0183");
         machines.addAll(machinePath.keySet());
			for (String machine : machines) {
				NodeList nodelist = document.getElementsByTagName(machine);
				for (int i = 0; i < nodelist.getLength(); i++) {
					int index = 0;
					Node item = nodelist.item(i);
					String value = Util.getChildNodeValue((Element) item, "index");
					try {
						index = Integer.valueOf(value);
	
					} catch (NumberFormatException e) {
						FCALLogs.getInstance().log.debug("Index is not available for the machine " + item.getNodeName());
					}
					Set<Integer> set = machineIndices.get(item.getNodeName());
					if (set == null) {
						set = new HashSet<>();
						machineIndices.put(item.getNodeName(), set);
					}
					set.add(index);
				}
			}
    }
    
    @Override
    public void run() {
        try {
            String []  interfaceArray = {"CAN_AGMO_Customized_Tractor/9455", "CAN_AGMO_Customized_Tractor/9456", "CAN_AGMO_Customized_Tractor/9457", "CAN_AGMO_Customized_Tractor/9459", "CAN_AGMO_MTLT305/9440", "CAN_AGMO_MTLT305/9441", "CAN_AGMO_MTLT305/9442", "CAN_AGMO_SteerMotor/9428", "CAN_AGMO_SteerMotor/9434", "CAN_AGMO_SteerMotor/9435", "ISOPGN/251", "Serial_Ext_GPS_NMEA0183/9465"};
    			for (String interfaceAddress : interfaceArray) {
					String name = interfaceAddress.split("/")[0];
					String address = "/" + interfaceAddress.split("/")[1];
					EStructuralFeature feature = SDKTest.getFeatureByInterfaceAddress(address);
					publish(name, feature, "fek" + address);
				}
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

	private void publish(String machine, EStructuralFeature feature, String topic) throws Exception {
		String machinePathString = machinePath.get(machine);
		String[] split = machinePathString.split("\\.");

		int length = split.length;
		Set<Integer> set = machineIndices.get(split[0]);
		if (length == 1) {
			JsonObject main = createNonSortedJSONObject();
			JsonObject values = createNonSortedJSONObject();
			for (Integer index : set) {
				values.add("" + index, new Gson().toJsonTree(getRandomValueObject(feature), Object.class));
			}
			JsonObject imp = createNonSortedJSONObject();
			imp.add(machine.toLowerCase(), values);
			main.add("PL", imp);
			main.addProperty("TS", System.currentTimeMillis());
			FCALLogs.getInstance().log.info(topic + ":" + main.toString());
			client.publish(topic, main.toString());
		} else if (length >= 2) {
			JsonObject main = createNonSortedJSONObject();
			JsonArray array = new JsonArray();
			String m = split[0];
			Set<Integer> indeces = machineIndices.get(m);
			for (Integer integer : indeces) {
				JsonObject obj = createNonSortedJSONObject();
				obj.addProperty(m.toLowerCase(), integer);
				computeChild(obj, 1, split, machineIndices, feature);
				array.add(obj);
			}
			main.add("PL", array);
			main.addProperty("TS", System.currentTimeMillis());
			client.publish(topic, main.toString());
			FCALLogs.getInstance().log.info(topic + ":" + main.toString());
		} else {
			FCALLogs.getInstance().log.info("Machine path details are not proper for " + machine);
		}

	}

	private void computeChild(JsonObject parent, int i, String[] split, Map<String, Set<Integer>> machineIndices,
			EStructuralFeature feature) {
		if (i < split.length - 1) {
			JsonArray array = new JsonArray();
			for (Integer index : machineIndices.get(split[i])) {
				JsonObject obj = createNonSortedJSONObject();
				computeChild(obj, i + 1, split, machineIndices, feature);
				obj.addProperty("index", index);
				array.add(obj);
			}
			parent.add(split[i].toLowerCase(), array);
		} else if (i == split.length - 1) {
			JsonObject values = createNonSortedJSONObject();
			for (Integer index : machineIndices.get(split[i])) {
				values.add("" + index, new Gson().toJsonTree(getRandomValueObject(feature), Object.class));
			}
			parent.add(split[i].toLowerCase(), values);
		}
	}

	private JsonObject createNonSortedJSONObject() {
		JsonObject jsonObject = new JsonObject();
		try {
			Field map = jsonObject.getClass().getDeclaredField("members");
			map.setAccessible(true);
			map.set(jsonObject, new com.google.gson.internal.LinkedTreeMap<>());
			map.setAccessible(false);
		} catch (NoSuchFieldException | SecurityException | IllegalArgumentException | IllegalAccessException e) {
			FCALLogs.getInstance().log.error("JSON Object creation failed due to access restriction " + e);
		}
		return jsonObject;
	}

	private Object getRandomValueObject(EStructuralFeature feature) {
		Object value = SDKTest.getRandomValue(feature,filSimulatorProperty);
        if (value instanceof AbsolutePosition) {
            AbsolutePosition gpsposition = (AbsolutePosition) value;
            JsonArray gps = new JsonArray();
            gps.add(gpsposition.getLongitude());
            gps.add(gpsposition.getLatitude());
            return gps;
        } else if (value instanceof com.bosch.nevonex.types.IArrayType) {
	        	return ((com.bosch.nevonex.types.IArrayType) value).getArrayValues();
        } else {
            return  value;
        }
	}
}