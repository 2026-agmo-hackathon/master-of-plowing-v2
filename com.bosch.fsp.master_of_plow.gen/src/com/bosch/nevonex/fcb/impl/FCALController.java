/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.platform.InterfaceDetails;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.InterfaceDisabledException;
import com.bosch.fsp.runtime.feature.exception.MachineReadException;
import com.bosch.fsp.runtime.feature.exception.MachineWriteException;

import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.ITopicObject;

import com.bosch.nevonex.fcb.IFCALController;
import com.bosch.nevonex.fcb.SubscriberEnum;

import com.bosch.nevonex.types.IArrayType;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonPrimitive;

import com.google.gson.internal.LinkedTreeMap;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;

import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;

import java.util.Map.Entry;

import java.util.Set;

import org.eclipse.emf.common.util.Enumerator;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EStructuralFeature;

import org.eclipse.emf.ecore.impl.EObjectImpl;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttPersistenceException;

/**
 * An implementation of the model object '<em><b>FCALController</b></em>'.
 * @generated
 */
public class FCALController extends EObjectImpl implements IFCALController {
	/**
	 * @generated
	 */
	public FCALController() {
		super();
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return FcbPackage.Literals.FCAL_CONTROLLER;
	}

	private boolean canPublishWriteInterfaces(String topicInterface) {
		boolean canPublish = true;
		Map<Integer, Boolean> multiWriteInterfaceMap = PublishConnectionFactory.getInstance()
				.getEnabledDisabledTopicMap();
		if (!multiWriteInterfaceMap.isEmpty() && topicInterface != null) {
			String[] split = topicInterface.split("/");
			if (split.length >= 2) {
				String id = split[1];
				if (multiWriteInterfaceMap.containsKey(Integer.valueOf(id))) {
					canPublish = multiWriteInterfaceMap.get(Integer.valueOf(id));
				}
			} else {
				FCALLogs.getInstance().log
						.error(LoggerConstants.LOG_SDK_PREFIX + " Interface key " + topicInterface + " is not proper");
			}
		}
		return canPublish;
	}

	private int getMachineIndex(EObject eObject) {
		if (eObject instanceof ITopicObject) {
			return ((ITopicObject) eObject).getIndex();
		}
		return 0;
	}

	private Object getPayloadValue(EObject machine, String featureName) {
		Object object = null;
		if (machine != null && featureName != null) {
			EStructuralFeature feature = machine.eClass().getEStructuralFeature(featureName);
			if (feature != null) {
				Object value = machine.eGet(feature);
				if (value instanceof IAbsolutePosition) {
					JsonArray posObject = new JsonArray();
					posObject.add(((IAbsolutePosition) value).getLatitude());
					posObject.add(((IAbsolutePosition) value).getLongitude());
					object = posObject;
				} else if (value instanceof IArrayType) {
					Object[] arrayValues = ((IArrayType) value).getArrayValues();
					JsonArray arrayObject = new JsonArray();
					for (Object obj : arrayValues) {
						arrayObject.add(new Gson().toJsonTree(obj, Object.class));
					}
					object = arrayObject;
				} else if (value instanceof Enumerator) {
					object = ((Enumerator) value).getValue();
				} else if (value instanceof Boolean) {
					object = ((Boolean) value) ? 1 : 0;
				} else if (value instanceof boolean[]) {
					boolean[] booleanArray = (boolean[]) value;
					int[] intArray = new int[booleanArray.length];
					for (int i = 0; i < intArray.length; i++) {
						boolean boolObject = booleanArray[i];
						intArray[i] = ((boolObject) == true) ? 1 : 0;
					}
					object = intArray;
				} else {
					object = value;
				}
			}
		}
		return object;
	}

	// FD 는 getInstanceClass() 를 널 검사 없이 바로 쓴다. IDE 가 만든 EMF 패키지는
	// setInstanceClassName(...) 으로 인스턴스 클래스를 늦게 묶으므로, 클래스로더가
	// 아직 못 푼 분류자에서는 null 이 나올 수 있다. 널이면 판정을 false 로 떨어뜨려
	// 아래 processInterfaceValue 의 기본 분기(값 그대로)로 보낸다 — FD 대비 동작 차이는
	// "NPE 대신 원본 값" 뿐이다.
	private boolean isArrayType(EStructuralFeature feature) {
		if (feature != null && feature.getEType() != null) {
			Class<?> clazz = feature.getEType().getInstanceClass();
			if (clazz != null && IArrayType.class.isAssignableFrom(clazz)) {
				return true;
			}
		}
		return false;
	}

	private Object processInterfaceValue(Object newObj, Object obj, EStructuralFeature feature)
			throws MachineReadException, IllegalAccessException, InvocationTargetException, NoSuchMethodException {
		Class<?> instanceClass = feature.getEType().getInstanceClass();
		String typeName = (instanceClass == null) ? "" : instanceClass.getName();
		if (typeName.equals(IAbsolutePosition.class.getName())) {
			IAbsolutePosition position = (IAbsolutePosition) feature.getEType().getEPackage().getEFactoryInstance()
					.create((EClass) feature.getEType());
			if (obj instanceof JsonArray) {
				position.setLatitude(Float.valueOf(((JsonArray) obj).get(0).toString()));
				position.setLongitude(Float.valueOf(((JsonArray) obj).get(1).toString()));
				newObj = position;
			}
		} else if (isArrayType(feature)) {
			EObject eObject = feature.getEType().getEPackage().getEFactoryInstance()
					.create((EClass) feature.getEType());
			if (eObject instanceof IArrayType && obj instanceof JsonArray) {
				int index = 0;
				for (Object arrayElement : (JsonArray) obj) {
					((IArrayType) eObject).setArrayFeature(index, arrayElement);
					index++;
				}
			}
			newObj = eObject;
		} else if (typeName.equals("float")) {
			newObj = Float.valueOf(obj.toString());
		} else if (typeName.equals("double")) {
			newObj = Double.valueOf(obj.toString());
		} else if (typeName.equals("int")) {
			newObj = Integer.valueOf(obj.toString());
		} else if (typeName.equals("long")) {
			newObj = Long.valueOf(obj.toString());
		} else if (typeName.equals("java.lang.String")) {
			if (obj instanceof JsonPrimitive && ((JsonPrimitive) obj).isString()) {
				newObj = ((JsonPrimitive) obj).getAsString();
			} else {
				FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX + "Value " + obj.toString()
						+ " is not proper for the String datatype ");
			}
		} else if (typeName.equals("boolean")) {
			if (obj instanceof JsonPrimitive) {
				JsonPrimitive primitive = (JsonPrimitive) obj;
				if (primitive.isBoolean()) {
					newObj = primitive.getAsBoolean();
				} else if (primitive.isNumber()) {
					newObj = (primitive.getAsInt() == 1) ? true : false;
				} else {
					newObj = Boolean.valueOf(obj.toString());
				}
			} else {
				newObj = Boolean.valueOf(obj.toString());
			}
		} else if (instanceClass != null && Enumerator.class.isAssignableFrom(instanceClass)) {
			newObj = instanceClass.getMethod("get", int.class)
					.invoke(feature.getEType().eClass().getClass(), Integer.valueOf(obj.toString()));
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("float[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				float[] floatArray = new float[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					floatArray[i] = Float.valueOf(jsonArray.get(i).toString());
				}
				newObj = floatArray;
			}
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("long[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				long[] longArray = new long[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					longArray[i] = Long.valueOf(jsonArray.get(i).getAsLong());
				}
				newObj = longArray;
			}
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("int[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				int[] intArray = new int[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					intArray[i] = Integer.valueOf(jsonArray.get(i).getAsInt());
				}
				newObj = intArray;
			}
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("double[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				double[] doubleArray = new double[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					doubleArray[i] = Double.valueOf(jsonArray.get(i).getAsDouble());
				}
				newObj = doubleArray;
			}
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("java.lang.String[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				String[] stringArray = new String[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					stringArray[i] = jsonArray.get(i).getAsString();
				}
				newObj = stringArray;
			}
		} else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("boolean[]")) {
			if (obj instanceof JsonArray) {
				JsonArray jsonArray = (JsonArray) obj;
				boolean[] booleanArray = new boolean[jsonArray.size()];
				for (int i = 0; i < jsonArray.size(); i++) {
					try {
						int intObject = jsonArray.get(i).getAsInt();
						booleanArray[i] = (((Integer) intObject).intValue() == 1) ? true : false;
					} catch (NumberFormatException nfe) {
						booleanArray[i] = jsonArray.get(i).getAsBoolean();
					}
				}
				newObj = booleanArray;
			}
		} else {
			newObj = obj;
		}
		return newObj;
	}

	private void constructPayload(Map<EObject, Set<EObject>> map, JsonArray array, String machineId) {
		for (Entry<EObject, Set<EObject>> entry : map.entrySet()) {
			EObject eObject = entry.getKey();
			Set<EObject> set = entry.getValue();
			if (!(eObject.eContainer() instanceof ITopicObject) && !set.isEmpty()) {
				// Implement
				JsonObject object = createNonSortedJSONObject();
				object.addProperty(eObject.eClass().getName().toLowerCase(), getMachineIndex(eObject));
				Object values = constructChildren(map, set, machineId);
				if (values != null) {
					object.add(set.iterator().next().getClass().getSimpleName().toLowerCase(),
							new Gson().toJsonTree(values, Object.class));
				}
				array.add(object);
			}
		}
	}

	private Object constructChildren(Map<EObject, Set<EObject>> map, Set<EObject> set, String machineId) {
		if (!set.isEmpty()) {
			if (map.containsKey(set.iterator().next())) {
				// intermediate objects - construct array
				JsonArray childArray = new JsonArray();
				for (EObject obj : set) {
					JsonObject childObj = createNonSortedJSONObject();
					childObj.addProperty("index", getMachineIndex(obj));
					Set<EObject> set2 = map.get(obj);
					if (!set2.isEmpty()) {
						Object children = constructChildren(map, set2, machineId);
						if (children != null) {
							childObj.add(set2.iterator().next().getClass().getSimpleName().toLowerCase(),
									new Gson().toJsonTree(children, Object.class));
						}
					}
					childArray.add(childObj);
				}
				return childArray;
			} else {
				// final children - construct values object
				JsonObject values = createNonSortedJSONObject();
				for (EObject child : set) {
					values.add("" + getMachineIndex(child),
							new Gson().toJsonTree(getPayloadValue(child, machineId), Object.class));
				}
				return values;
			}

		}
		return null;
	}

	// Gson 의 JsonObject 는 기본이 정렬 맵이라 키 순서가 뒤집힌다. FIL 은 순서를 보지
	// 않지만 골든과 바이트 비교가 가능해야 하므로 FD 와 동일하게 LinkedTreeMap 으로 갈아끼운다.
	private JsonObject createNonSortedJSONObject() {
		JsonObject jsonObject = new JsonObject();
		try {
			Field map = jsonObject.getClass().getDeclaredField("members");
			map.setAccessible(true);
			map.set(jsonObject, new LinkedTreeMap<>());
			map.setAccessible(false);
		} catch (NoSuchFieldException | SecurityException | IllegalArgumentException | IllegalAccessException e) {
			FCALLogs.getInstance().log.error(
					LoggerConstants.LOG_SDK_PREFIX + "Json Object creation failed due to access restriction " + e);
		}
		return jsonObject;
	}

	private Map<EObject, Set<EObject>> getParentChildMap(List<EObject> objects) {
		Map<EObject, Set<EObject>> map = new HashMap<EObject, Set<EObject>>();
		for (EObject object : objects) {
			EObject obj = object;
			while (obj instanceof ITopicObject && obj.eContainer() instanceof ITopicObject) {
				Set<EObject> set = map.get(obj.eContainer());
				if (set == null) {
					set = new LinkedHashSet<>();
					map.put(obj.eContainer(), set);
				}
				set.add(obj);
				obj = obj.eContainer();
			}
		}
		return map;
	}

	/**
	 * @generated
	 */
	public void setMachineUpdate(ITopicObject machine, String attribute, Object payload, InterfaceDetails details,
			long timestamp) throws MachineReadException {
		try {
			if (machine != null) {
				Object newObj = null;
				Object obj = payload;
				EStructuralFeature feature = machine.eClass().getEStructuralFeature(attribute);
				if (feature != null) {
					// For payload null value
					if (payload instanceof JsonNull || payload == null) {
						details.setValid(false, timestamp);
						machine.eUnset(feature);
						FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX
								+ " Payload is null for the topic : " + details.getTopicId());
						return;
					}
					if (payload instanceof JsonArray) {
						JsonArray array = (JsonArray) payload;
						if (array.contains(JsonNull.INSTANCE)) {
							details.setValid(false, timestamp);
							machine.eUnset(feature);
							FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX
									+ " Payload is null for the topic : " + details.getTopicId());
							return;
						}
					}
					newObj = processInterfaceValue(newObj, obj, feature);
					if (newObj != null) {
						details.setValid(true, timestamp);
						machine.eSet(feature, newObj);
					} else {
						// payload mismatch value
						details.setValid(false, timestamp);
						machine.eUnset(feature);
						throw new MachineReadException("Payload is not matching with the selected interface");
					}
				} else {
					throw new MachineReadException("The Interface " + attribute + " is not present in machine '"
							+ machine.getClass().getSimpleName() + "'.");
				}
			}
		} catch (Exception e) {
			details.setValid(false, timestamp);
			throw new MachineReadException("Exception while updating machine data from FIL layer.", e);
		}
	}

	/**
	 * @generated
	 */
	public void publish(String interfaceKey, String interfaceName, List<EObject> payloadValue,
			SubscriberEnum subscriber)
			throws MachineWriteException, CommunicationException, InterfaceDisabledException {
		if ((interfaceKey == null) || (payloadValue == null) || payloadValue.isEmpty()) {
			throw new MachineWriteException("interfacekey or payloadvalue is null");
		}
		String topicInterface = null;
		Map<String, String> topicMap = PublishConnectionFactory.getInstance().getTopicMap();
		for (Entry<String, String> entry : topicMap.entrySet()) {
			if (interfaceKey.equalsIgnoreCase(entry.getValue().toString())) {
				topicInterface = entry.getKey().toString();
				break;
			}
		}
		if (!canPublishWriteInterfaces(topicInterface)) {
			FCALLogs.getInstance().log
					.error(LoggerConstants.LOG_SDK_PREFIX + "Interface " + interfaceName + " is disabled");
			throw new InterfaceDisabledException("Interface " + interfaceName + " is disabled");
		}
		String className = null;
		// check whether all are of same type
		for (EObject eObject : payloadValue) {
			if (className == null) {
				className = eObject.getClass().getName();
			} else if (!className.equalsIgnoreCase(eObject.getClass().getName())) {
				FCALLogs.getInstance().log
						.debug(LoggerConstants.LOG_SDK_PREFIX + "All payload objects are not of same type");
			}
		}
		JsonObject main = createNonSortedJSONObject();
		if (topicInterface != null) {
			// check the machine path
			String machine = interfaceKey.split("\\.")[0];
			String machinePath = PublishConnectionFactory.getInstance().getMachinePathMap().get(machine);
			if (machinePath == null) {
				FCALLogs.getInstance().log.error(
						LoggerConstants.LOG_SDK_PREFIX + "machinePathMap has no entry for machine " + machine);
				return;
			}
			int length = machinePath.split("\\.").length;
			if (length == 1) {
				// if implement
				JsonObject values = createNonSortedJSONObject();
				String root = "implement";
				for (EObject eObject : payloadValue) {
					values.add("" + getMachineIndex(eObject),
							new Gson().toJsonTree(getPayloadValue(eObject, interfaceName), Object.class));
					root = eObject.eClass().getName().toLowerCase();
				}
				JsonObject imp = createNonSortedJSONObject();
				imp.add(root, values);
				main.add("PL", imp);
				main.addProperty("TS", System.currentTimeMillis());
			} else if (length >= 2) {
				// if boom
				JsonArray array = new JsonArray();
				Map<EObject, Set<EObject>> map = getParentChildMap(payloadValue);
				constructPayload(map, array, interfaceName);
				main.add("PL", array);
				main.addProperty("TS", System.currentTimeMillis());
			}
			for (Entry<String, String> entry : PublishConnectionFactory.getInstance().getPrefixMap().entrySet()) {
				String string = entry.getKey();
				if (string.toLowerCase().endsWith("publish") && (subscriber.equals(SubscriberEnum.FIL)
						|| string.toLowerCase().contains(subscriber.name().toLowerCase()))) {
					String finalTopic = entry.getValue() + topicInterface + "/process";
					try {
						MqttMessage message = new MqttMessage(main.toString().getBytes());
						message.setQos(0);
						PublishConnectionFactory.getInstance().getClient().publish(finalTopic, message);
						FCALLogs.getInstance().log.debug(LoggerConstants.LOG_SDK_PREFIX
								+ "Topic published from FCAL Layer : " + finalTopic + "\n" + message);
					} catch (MqttPersistenceException e) {
						throw new CommunicationException("Exception while sending data to the message broker.", e);
					} catch (MqttException e) {
						throw new CommunicationException("Exception while sending data to the message broker.", e);
					}
				}
			}
		}
	}
} //FCALController
