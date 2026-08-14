/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>ProviderEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.common.impl.CommonPackage#getProviderEnum()
 * @generated
 */
public enum ProviderEnum implements InternalProviderEnum {
	/**
	 * The '<em><b>CAN_AGMO_Customized_TractorProvider</b></em>' literal object.
	 * @see #CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_VALUE
	 * @generated
	 * @ordered
	 */
	CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER(0, "CAN_AGMO_Customized_TractorProvider", "CAN_AGMO_Customized_TractorProvider"),

	/**
	 * The '<em><b>CAN_AGMO_MTLT305Provider</b></em>' literal object.
	 * @see #CAN_AGMO_MTLT305PROVIDER_VALUE
	 * @generated
	 * @ordered
	 */
	CAN_AGMO_MTLT305PROVIDER(1, "CAN_AGMO_MTLT305Provider", "CAN_AGMO_MTLT305Provider"),

	/**
	 * The '<em><b>CAN_AGMO_SteerMotorProvider</b></em>' literal object.
	 * @see #CAN_AGMO_STEER_MOTOR_PROVIDER_VALUE
	 * @generated
	 * @ordered
	 */
	CAN_AGMO_STEER_MOTOR_PROVIDER(2, "CAN_AGMO_SteerMotorProvider", "CAN_AGMO_SteerMotorProvider"),

	/**
	 * The '<em><b>ISOPGNProvider</b></em>' literal object.
	 * @see #ISOPGN_PROVIDER_VALUE
	 * @generated
	 * @ordered
	 */
	ISOPGN_PROVIDER(3, "ISOPGNProvider", "ISOPGNProvider"),

	/**
	 * The '<em><b>Serial_Ext_GPS_NMEA0183Provider</b></em>' literal object.
	 * @see #SERIAL_EXT_GPS_NMEA0183PROVIDER_VALUE
	 * @generated
	 * @ordered
	 */
	SERIAL_EXT_GPS_NMEA0183PROVIDER(4, "Serial_Ext_GPS_NMEA0183Provider", "Serial_Ext_GPS_NMEA0183Provider");

	/**
	 * The '<em><b>CAN_AGMO_Customized_TractorProvider</b></em>' literal value.
	 * @see #CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER
	 * @generated
	 * @ordered
	 */
	public static final int CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_VALUE = 0;

	/**
	 * The '<em><b>CAN_AGMO_MTLT305Provider</b></em>' literal value.
	 * @see #CAN_AGMO_MTLT305PROVIDER
	 * @generated
	 * @ordered
	 */
	public static final int CAN_AGMO_MTLT305PROVIDER_VALUE = 1;

	/**
	 * The '<em><b>CAN_AGMO_SteerMotorProvider</b></em>' literal value.
	 * @see #CAN_AGMO_STEER_MOTOR_PROVIDER
	 * @generated
	 * @ordered
	 */
	public static final int CAN_AGMO_STEER_MOTOR_PROVIDER_VALUE = 2;

	/**
	 * The '<em><b>ISOPGNProvider</b></em>' literal value.
	 * @see #ISOPGN_PROVIDER
	 * @generated
	 * @ordered
	 */
	public static final int ISOPGN_PROVIDER_VALUE = 3;

	/**
	 * The '<em><b>Serial_Ext_GPS_NMEA0183Provider</b></em>' literal value.
	 * @see #SERIAL_EXT_GPS_NMEA0183PROVIDER
	 * @generated
	 * @ordered
	 */
	public static final int SERIAL_EXT_GPS_NMEA0183PROVIDER_VALUE = 4;

	/**
	 * An array of all the '<em><b>ProviderEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final ProviderEnum[] VALUES_ARRAY = new ProviderEnum[] { CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER, CAN_AGMO_MTLT305PROVIDER, CAN_AGMO_STEER_MOTOR_PROVIDER, ISOPGN_PROVIDER, SERIAL_EXT_GPS_NMEA0183PROVIDER, };

	/**
	 * A public read-only list of all the '<em><b>ProviderEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<ProviderEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>ProviderEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ProviderEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			ProviderEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>ProviderEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ProviderEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			ProviderEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>ProviderEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static ProviderEnum get(int value) {
		switch (value) {
		case CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER_VALUE:
			return CAN_AGMO_CUSTOMIZED_TRACTOR_PROVIDER;
		case CAN_AGMO_MTLT305PROVIDER_VALUE:
			return CAN_AGMO_MTLT305PROVIDER;
		case CAN_AGMO_STEER_MOTOR_PROVIDER_VALUE:
			return CAN_AGMO_STEER_MOTOR_PROVIDER;
		case ISOPGN_PROVIDER_VALUE:
			return ISOPGN_PROVIDER;
		case SERIAL_EXT_GPS_NMEA0183PROVIDER_VALUE:
			return SERIAL_EXT_GPS_NMEA0183PROVIDER;
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
	private ProviderEnum(int value, String name, String literal) {
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

} //ProviderEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalProviderEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
