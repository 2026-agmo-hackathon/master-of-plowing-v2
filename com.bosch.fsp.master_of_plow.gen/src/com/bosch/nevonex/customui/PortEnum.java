/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>PortEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.customui.impl.CustomuiPackage#getPortEnum()
 * @generated
 */
public enum PortEnum implements InternalPortEnum {
	/**
	 * The '<em><b>HTTP</b></em>' literal object.
	 * @see #HTTP_VALUE
	 * @generated
	 * @ordered
	 */
	HTTP(8080, "HTTP", "HTTP"),

	/**
	 * The '<em><b>WS</b></em>' literal object.
	 * @see #WS_VALUE
	 * @generated
	 * @ordered
	 */
	WS(8081, "WS", "WS");

	/**
	 * The '<em><b>HTTP</b></em>' literal value.
	 * @see #HTTP
	 * @generated
	 * @ordered
	 */
	public static final int HTTP_VALUE = 8080;

	/**
	 * The '<em><b>WS</b></em>' literal value.
	 * @see #WS
	 * @generated
	 * @ordered
	 */
	public static final int WS_VALUE = 8081;

	/**
	 * An array of all the '<em><b>PortEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final PortEnum[] VALUES_ARRAY = new PortEnum[] { HTTP, WS, };

	/**
	 * A public read-only list of all the '<em><b>PortEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<PortEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>PortEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PortEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			PortEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>PortEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PortEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			PortEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>PortEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PortEnum get(int value) {
		switch (value) {
		case HTTP_VALUE:
			return HTTP;
		case WS_VALUE:
			return WS;
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
	private PortEnum(int value, String name, String literal) {
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

} //PortEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalPortEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
