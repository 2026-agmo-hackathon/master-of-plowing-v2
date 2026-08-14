/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>ConnectionTypeEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.common.impl.CommonPackage#getConnectionTypeEnum()
 * @generated
 */
public enum ConnectionTypeEnum implements InternalConnectionTypeEnum {
	/**
	 * The '<em><b>MQTT</b></em>' literal object.
	 * @see #MQTT_VALUE
	 * @generated
	 * @ordered
	 */
	MQTT(0, "MQTT", "MQTT"),

	/**
	 * The '<em><b>CAN</b></em>' literal object.
	 * @see #CAN_VALUE
	 * @generated
	 * @ordered
	 */
	CAN(1, "CAN", "CAN"),

	/**
	 * The '<em><b>GPIO</b></em>' literal object.
	 * @see #GPIO_VALUE
	 * @generated
	 * @ordered
	 */
	GPIO(2, "GPIO", "GPIO"),

	/**
	 * The '<em><b>GPS</b></em>' literal object.
	 * @see #GPS_VALUE
	 * @generated
	 * @ordered
	 */
	GPS(3, "GPS", "GPS"),

	/**
	 * The '<em><b>ISOBUS</b></em>' literal object.
	 * @see #ISOBUS_VALUE
	 * @generated
	 * @ordered
	 */
	ISOBUS(4, "ISOBUS", "ISOBUS"),

	/**
	 * The '<em><b>Serial</b></em>' literal object.
	 * @see #SERIAL_VALUE
	 * @generated
	 * @ordered
	 */
	SERIAL(5, "Serial", "Serial"),

	/**
	 * The '<em><b>WIFI</b></em>' literal object.
	 * @see #WIFI_VALUE
	 * @generated
	 * @ordered
	 */
	WIFI(6, "WIFI", "WIFI"),

	/**
	 * The '<em><b>SATELLITE</b></em>' literal object.
	 * @see #SATELLITE_VALUE
	 * @generated
	 * @ordered
	 */
	SATELLITE(7, "SATELLITE", "SATELLITE");

	/**
	 * The '<em><b>MQTT</b></em>' literal value.
	 * @see #MQTT
	 * @generated
	 * @ordered
	 */
	public static final int MQTT_VALUE = 0;

	/**
	 * The '<em><b>CAN</b></em>' literal value.
	 * @see #CAN
	 * @generated
	 * @ordered
	 */
	public static final int CAN_VALUE = 1;

	/**
	 * The '<em><b>GPIO</b></em>' literal value.
	 * @see #GPIO
	 * @generated
	 * @ordered
	 */
	public static final int GPIO_VALUE = 2;

	/**
	 * The '<em><b>GPS</b></em>' literal value.
	 * @see #GPS
	 * @generated
	 * @ordered
	 */
	public static final int GPS_VALUE = 3;

	/**
	 * The '<em><b>ISOBUS</b></em>' literal value.
	 * @see #ISOBUS
	 * @generated
	 * @ordered
	 */
	public static final int ISOBUS_VALUE = 4;

	/**
	 * The '<em><b>Serial</b></em>' literal value.
	 * @see #SERIAL
	 * @generated
	 * @ordered
	 */
	public static final int SERIAL_VALUE = 5;

	/**
	 * The '<em><b>WIFI</b></em>' literal value.
	 * @see #WIFI
	 * @generated
	 * @ordered
	 */
	public static final int WIFI_VALUE = 6;

	/**
	 * The '<em><b>SATELLITE</b></em>' literal value.
	 * @see #SATELLITE
	 * @generated
	 * @ordered
	 */
	public static final int SATELLITE_VALUE = 7;

	/**
	 * An array of all the '<em><b>ConnectionTypeEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final ConnectionTypeEnum[] VALUES_ARRAY = new ConnectionTypeEnum[] { MQTT, CAN, GPIO, GPS, ISOBUS, SERIAL, WIFI, SATELLITE, };

	/**
	 * A public read-only list of all the '<em><b>ConnectionTypeEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<ConnectionTypeEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>ConnectionTypeEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ConnectionTypeEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			ConnectionTypeEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>ConnectionTypeEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ConnectionTypeEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			ConnectionTypeEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>ConnectionTypeEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ConnectionTypeEnum get(int value) {
		switch (value) {
		case MQTT_VALUE:
			return MQTT;
		case CAN_VALUE:
			return CAN;
		case GPIO_VALUE:
			return GPIO;
		case GPS_VALUE:
			return GPS;
		case ISOBUS_VALUE:
			return ISOBUS;
		case SERIAL_VALUE:
			return SERIAL;
		case WIFI_VALUE:
			return WIFI;
		case SATELLITE_VALUE:
			return SATELLITE;
		}
		return null;
	}

	/**
	 * @generated
	 */
	private final int value;

	/**
	 * @generated
	 */
	private final String name;

	/**
	 * @generated
	 */
	private final String literal;

	/**
	 * Only this class can construct instances.
	 * @generated
	 */
	private ConnectionTypeEnum(int value, String name, String literal) {
		this.value = value;
		this.name = name;
		this.literal = literal;
	}

	/**
	 * @generated
	 */
	@Override
	public int getValue() {
		return value;
	}

	/**
	 * @generated
	 */
	@Override
	public String getName() {
		return name;
	}

	/**
	 * @generated
	 */
	@Override
	public String getLiteral() {
		return literal;
	}

	/**
	 * Returns the literal value of the enumerator, which is its string representation.
	 * @generated
	 */
	@Override
	public String toString() {
		return literal;
	}

} //ConnectionTypeEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalConnectionTypeEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
