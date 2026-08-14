/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>HMIServicesEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.common.impl.CommonPackage#getHMIServicesEnum()
 * @generated
 */
public enum HMIServicesEnum implements InternalHMIServicesEnum {
	/**
	 * The '<em><b>CUSTOMUI</b></em>' literal object.
	 * @see #CUSTOMUI_VALUE
	 * @generated
	 * @ordered
	 */
	CUSTOMUI(0, "CUSTOMUI", "com.bosch.nevonex.customui.impl.UIWebServiceProvider");

	/**
	 * The '<em><b>CUSTOMUI</b></em>' literal value.
	 * @see #CUSTOMUI
	 * @generated
	 * @ordered
	 */
	public static final int CUSTOMUI_VALUE = 0;

	/**
	 * An array of all the '<em><b>HMIServicesEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final HMIServicesEnum[] VALUES_ARRAY = new HMIServicesEnum[] { CUSTOMUI, };

	/**
	 * A public read-only list of all the '<em><b>HMIServicesEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<HMIServicesEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>HMIServicesEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static HMIServicesEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			HMIServicesEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>HMIServicesEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static HMIServicesEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			HMIServicesEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>HMIServicesEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static HMIServicesEnum get(int value) {
		switch (value) {
		case CUSTOMUI_VALUE:
			return CUSTOMUI;
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
	private HMIServicesEnum(int value, String name, String literal) {
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

} //HMIServicesEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalHMIServicesEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
