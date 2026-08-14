/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcb;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>SubscriberEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.fcb.impl.FcbPackage#getSubscriberEnum()
 * @generated
 */
public enum SubscriberEnum implements InternalSubscriberEnum {
	/**
	 * The '<em><b>FIL</b></em>' literal object.
	 * @see #FIL_VALUE
	 * @generated
	 * @ordered
	 */
	FIL(0, "FIL", "FIL"),

	/**
	 * The '<em><b>FGF</b></em>' literal object.
	 * @see #FGF_VALUE
	 * @generated
	 * @ordered
	 */
	FGF(1, "FGF", "FGF"),

	/**
	 * The '<em><b>ALL</b></em>' literal object.
	 * @see #ALL_VALUE
	 * @generated
	 * @ordered
	 */
	ALL(2, "ALL", "ALL");

	/**
	 * The '<em><b>FIL</b></em>' literal value.
	 * @see #FIL
	 * @generated
	 * @ordered
	 */
	public static final int FIL_VALUE = 0;

	/**
	 * The '<em><b>FGF</b></em>' literal value.
	 * @see #FGF
	 * @generated
	 * @ordered
	 */
	public static final int FGF_VALUE = 1;

	/**
	 * The '<em><b>ALL</b></em>' literal value.
	 * @see #ALL
	 * @generated
	 * @ordered
	 */
	public static final int ALL_VALUE = 2;

	/**
	 * An array of all the '<em><b>SubscriberEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final SubscriberEnum[] VALUES_ARRAY = new SubscriberEnum[] { FIL, FGF, ALL, };

	/**
	 * A public read-only list of all the '<em><b>SubscriberEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<SubscriberEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>SubscriberEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static SubscriberEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			SubscriberEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>SubscriberEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static SubscriberEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			SubscriberEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>SubscriberEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static SubscriberEnum get(int value) {
		switch (value) {
		case FIL_VALUE:
			return FIL;
		case FGF_VALUE:
			return FGF;
		case ALL_VALUE:
			return ALL;
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
	private SubscriberEnum(int value, String name, String literal) {
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

} //SubscriberEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalSubscriberEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
