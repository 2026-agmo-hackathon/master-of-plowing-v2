/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * A representation of the literals of the enumeration '<em><b>PlatformServicesEnum</b></em>',
 * and utility methods for working with them.
 * @see com.bosch.nevonex.common.impl.CommonPackage#getPlatformServicesEnum()
 * @generated
 */
public enum PlatformServicesEnum implements InternalPlatformServicesEnum {
	/**
	 * The '<em><b>CLOUD_DOWNLOAD</b></em>' literal object.
	 * @see #CLOUD_DOWNLOAD_VALUE
	 * @generated
	 * @ordered
	 */
	CLOUD_DOWNLOAD(0, "CLOUD_DOWNLOAD", "Cloud_Download"),

	/**
	 * The '<em><b>CLOUD_UPLOAD</b></em>' literal object.
	 * @see #CLOUD_UPLOAD_VALUE
	 * @generated
	 * @ordered
	 */
	CLOUD_UPLOAD(1, "CLOUD_UPLOAD", "Cloud_Upload");

	/**
	 * The '<em><b>CLOUD_DOWNLOAD</b></em>' literal value.
	 * @see #CLOUD_DOWNLOAD
	 * @generated
	 * @ordered
	 */
	public static final int CLOUD_DOWNLOAD_VALUE = 0;

	/**
	 * The '<em><b>CLOUD_UPLOAD</b></em>' literal value.
	 * @see #CLOUD_UPLOAD
	 * @generated
	 * @ordered
	 */
	public static final int CLOUD_UPLOAD_VALUE = 1;

	/**
	 * An array of all the '<em><b>PlatformServicesEnum</b></em>' enumerators.
	 * @generated
	 */
	private static final PlatformServicesEnum[] VALUES_ARRAY = new PlatformServicesEnum[] { CLOUD_DOWNLOAD, CLOUD_UPLOAD, };

	/**
	 * A public read-only list of all the '<em><b>PlatformServicesEnum</b></em>' enumerators.
	 * @generated
	 */
	public static final List<PlatformServicesEnum> VALUES = Collections.unmodifiableList(Arrays.asList(VALUES_ARRAY));

	/**
	 * Returns the '<em><b>PlatformServicesEnum</b></em>' literal with the specified literal value.
	 * @param literal the literal.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PlatformServicesEnum get(String literal) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			PlatformServicesEnum result = VALUES_ARRAY[i];
			if (result.toString().equals(literal)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>PlatformServicesEnum</b></em>' literal with the specified name.
	 * @param name the name.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PlatformServicesEnum getByName(String name) {
		for (int i = 0; i < VALUES_ARRAY.length; ++i) {
			PlatformServicesEnum result = VALUES_ARRAY[i];
			if (result.getName().equals(name)) {
				return result;
			}
		}
		return null;
	}

	/**
	 * Returns the '<em><b>PlatformServicesEnum</b></em>' literal with the specified integer value.
	 * @param value the integer value.
	 * @return the matching enumerator or <code>null</code>.
	 * @generated
	 */
	public static PlatformServicesEnum get(int value) {
		switch (value) {
		case CLOUD_DOWNLOAD_VALUE:
			return CLOUD_DOWNLOAD;
		case CLOUD_UPLOAD_VALUE:
			return CLOUD_UPLOAD;
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
	private PlatformServicesEnum(int value, String name, String literal) {
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

} //PlatformServicesEnum

/**
 * A private implementation interface used to hide the inheritance from Enumerator.
 * @generated
 */
interface InternalPlatformServicesEnum extends org.eclipse.emf.common.util.Enumerator {
	// Empty 
}
