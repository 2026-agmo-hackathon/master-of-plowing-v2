/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.types;

import com.bosch.fsp.runtime.feature.exception.MachineInitException;
import com.bosch.fsp.runtime.feature.exception.MachineReadException;

import org.eclipse.emf.common.util.Enumerator;

import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EStructuralFeature;

/**
 * @generated
 */
public interface IArrayType extends EObject {
	/**
	 * @generated
	 */
	/**
	 * 배열 잎의 이름을 선언 순서대로.
	 *
	 * <p>상속 피처(TopicObject::index)는 빼야 한다 — FD 의 ArrayType 은 피처가
	 * 없고 잎에는 인터페이스 값만 들어간다. IDE 는 배열 EClass 를
	 * {@code #//common/TopicObject} 에서 상속시키므로 getEAllStructuralFeatures 를
	 * 쓰면 index 가 0번 잎으로 끼어든다. 그러면 송신은 잎이 하나 늘고, 수신은
	 * 첫 잎이 index 로 삼켜진 뒤 나머지가 한 칸씩 밀려 마지막 필드가 영영 기본값이
	 * 된다 — 컴파일도 되고 에러도 안 난다. C++ 생성기는 같은 이유로 이미
	 * getEStructuralFeatures(자기 피처만) 를 쓴다.
	 */
	default String[] getFeatureNames() {
		java.util.List<String> names = new java.util.ArrayList<>();
		for (EStructuralFeature f : eClass().getEStructuralFeatures()) {
			names.add(f.getName());
		}
		return names.toArray(new String[0]);
	}

	/**
	 * @generated
	 */
	default void setArrayFeature(int index, Object value) throws MachineReadException {
		String[] featureNames = getFeatureNames();
		if (index < featureNames.length) {
			EStructuralFeature feature = eClass().getEStructuralFeature(featureNames[index]);
			if (feature != null) {
				eSet(feature, coerceArrayFeatureValue(feature, value));
			}
		}
	}

	/**
	 * @generated
	 */
	default Object[] getArrayValues() {
		String[] featureNames = getFeatureNames();
		Object[] values = new Object[featureNames.length];
		int index = 0;
		for (String featureName : featureNames) {
			EStructuralFeature feature = eClass().getEStructuralFeature(featureName);
			if (feature != null) {
				Object value = eGet(feature);
				if (value instanceof Enumerator) {
					values[index] = ((Enumerator) value).getValue();
				} else if (value instanceof Boolean) {
					values[index] = ((Boolean) value) ? 1 : 0;
				} else if (value instanceof boolean[]) {
					boolean[] booleanArray = (boolean[]) value;
					int[] intArray = new int[booleanArray.length];
					for (int i = 0; i < intArray.length; i++) {
						intArray[i] = booleanArray[i] ? 1 : 0;
					}
					values[index] = intArray;
				} else {
					values[index] = value;
				}
				index++;
			}
		}
		return values;
	}

	/**
	 * @generated
	 */
	default void setArrayFeature(String name, String value) throws MachineInitException {
		String[] featureNames = getFeatureNames();
		for (String featureName : featureNames) {
			if (featureName.equalsIgnoreCase(name)) {
				EStructuralFeature feature = eClass().getEStructuralFeature(featureName);
				if (feature != null) {
					eSet(feature, coerceArrayFeatureValue(feature, value));
				}
				break;
			}
		}
	}

	/**
	 * 들어온 값을 피처의 선언 타입으로 맞춘다.
	 *
	 * <p>이게 없으면 수신이 통째로 깨진다. FCALController 는 MQTT 페이로드를 파싱한
	 * 결과를 <b>그대로</b> setArrayFeature 로 넘기므로 값은 JSON 원소(JsonPrimitive)나
	 * DOM 문자열이고, EMF 가 생성한 eSet 은 (Double)/(Float) 같은 직접 캐스트라
	 * ClassCastException 이 난다. FD 산출물은 같은 변환을 클래스마다 손으로 넣는데,
	 * 여기 default 에 한 번 두면 모든 ArrayType 이 함께 고쳐진다.
	 *
	 * <p>Gson 타입을 import 하지 않는다 — 문자열 표현과 Iterable 만 보면 되고,
	 * 그래야 이 인터페이스가 JSON 라이브러리에 묶이지 않는다.
	 * @generated
	 */
	static Object coerceArrayFeatureValue(EStructuralFeature feature, Object value) {
		if (value == null) {
			return null;
		}
		EClassifier eType = feature.getEType();
		Class<?> instanceClass = eType == null ? null : eType.getInstanceClass();
		if (instanceClass == null) {
			return value;
		}
		// 이미 목표 타입이면 건드리지 않는다 (박싱 타입/EObject 재대입 경로).
		if (!instanceClass.isPrimitive() && instanceClass.isInstance(value)) {
			return value;
		}
		if (instanceClass.isArray() && value instanceof Iterable) {
			return coerceArrayValue(instanceClass, (Iterable<?>) value);
		}
		String text = unwrapJsonText(value);
		if (text == null || "null".equals(text)) {
			return null;
		}
		try {
			String typeName = instanceClass.getName();
			if ("double".equals(typeName) || "java.lang.Double".equals(typeName)) {
				return Double.valueOf(text);
			}
			if ("float".equals(typeName) || "java.lang.Float".equals(typeName)) {
				return Float.valueOf(text);
			}
			// 정수 계열은 "12.0" 처럼 소수점이 붙어 와도 받아야 한다 —
			// JSON 숫자는 정수/실수 구분이 없어서 실제로 그렇게 온다.
			if ("int".equals(typeName) || "java.lang.Integer".equals(typeName)) {
				return Integer.valueOf((int) Double.parseDouble(text));
			}
			if ("long".equals(typeName) || "java.lang.Long".equals(typeName)) {
				return Long.valueOf((long) Double.parseDouble(text));
			}
			if ("short".equals(typeName) || "java.lang.Short".equals(typeName)) {
				return Short.valueOf((short) Double.parseDouble(text));
			}
			if ("byte".equals(typeName) || "java.lang.Byte".equals(typeName)) {
				return Byte.valueOf((byte) Double.parseDouble(text));
			}
			if ("boolean".equals(typeName) || "java.lang.Boolean".equals(typeName)) {
				// FIL 은 BOOLEAN 을 1/0 으로 싣는다.
				if ("1".equals(text)) {
					return Boolean.TRUE;
				}
				if ("0".equals(text)) {
					return Boolean.FALSE;
				}
				return Boolean.valueOf(text);
			}
			if ("java.lang.String".equals(typeName)) {
				return text;
			}
			if (Enumerator.class.isAssignableFrom(instanceClass)) {
				return instanceClass.getMethod("get", int.class)
						.invoke(null, Integer.valueOf((int) Double.parseDouble(text)));
			}
		} catch (Exception e) {
			// 변환 실패는 원본을 그대로 돌려준다. 여기서 던지면 같은 메시지 안의
			// 다른 잎까지 통째로 버려진다.
			return value;
		}
		return value;
	}

	/**
	 * @generated
	 */
	static Object coerceArrayValue(Class<?> arrayClass, Iterable<?> elements) {
		java.util.List<Object> items = new java.util.ArrayList<>();
		for (Object element : elements) {
			items.add(element);
		}
		Class<?> componentType = arrayClass.getComponentType();
		Object array = java.lang.reflect.Array.newInstance(componentType, items.size());
		for (int i = 0; i < items.size(); i++) {
			String text = unwrapJsonText(items.get(i));
			try {
				if (componentType == double.class) {
					java.lang.reflect.Array.setDouble(array, i, Double.parseDouble(text));
				} else if (componentType == float.class) {
					java.lang.reflect.Array.setFloat(array, i, Float.parseFloat(text));
				} else if (componentType == int.class) {
					java.lang.reflect.Array.setInt(array, i, (int) Double.parseDouble(text));
				} else if (componentType == long.class) {
					java.lang.reflect.Array.setLong(array, i, (long) Double.parseDouble(text));
				} else if (componentType == boolean.class) {
					java.lang.reflect.Array.setBoolean(array, i,
							"1".equals(text) || Boolean.parseBoolean(text));
				} else {
					java.lang.reflect.Array.set(array, i, text);
				}
			} catch (Exception e) {
				// 원소 하나가 깨져도 나머지는 살린다.
			}
		}
		return array;
	}

	/**
	 * JSON 문자열 리터럴은 toString() 이 따옴표를 붙여 돌려준다 — 벗겨낸다.
	 * @generated
	 */
	static String unwrapJsonText(Object value) {
		if (value == null) {
			return null;
		}
		String text = String.valueOf(value);
		if (text.length() >= 2 && text.charAt(0) == '"' && text.charAt(text.length() - 1) == '"') {
			return text.substring(1, text.length() - 1);
		}
		return text;
	}

} // IArrayType
