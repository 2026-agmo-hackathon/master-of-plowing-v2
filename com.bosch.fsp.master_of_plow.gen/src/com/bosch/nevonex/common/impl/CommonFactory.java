/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.common.impl;

import com.bosch.nevonex.common.ICommonFactory;
import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.ITopicObject;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EObject;
import org.eclipse.emf.ecore.EPackage;

import org.eclipse.emf.ecore.impl.EFactoryImpl;

import org.eclipse.emf.ecore.plugin.EcorePlugin;

/**
 * @generated
 */
public class CommonFactory extends EFactoryImpl implements ICommonFactory {
	public static final CommonFactory eINSTANCE = init();

	public static CommonFactory init() {
		try {
			CommonFactory theCommonFactory = (CommonFactory) EPackage.Registry.INSTANCE.getEFactory(CommonPackage.eNS_URI);
			if (theCommonFactory != null) {
				return theCommonFactory;
			}
		} catch (Exception exception) {
			EcorePlugin.INSTANCE.log(exception);
		}
		return new CommonFactory();
	}

	public CommonFactory() {
		super();
	}

	@Override
	public EObject create(EClass eClass) {
		switch (eClass.getClassifierID()) {
		case CommonPackage.ABSOLUTE_POSITION:
			return createAbsolutePosition();
		case CommonPackage.TOPIC_OBJECT:
			return createTopicObject();
		default:
			throw new IllegalArgumentException("The class '" + eClass.getName() + "' is not a valid classifier");
		}
	}

	public IAbsolutePosition createAbsolutePosition() {
		AbsolutePosition absolutePosition = new AbsolutePosition();
		return absolutePosition;
	}

	public ITopicObject createTopicObject() {
		TopicObject topicObject = new TopicObject();
		return topicObject;
	}

	public CommonPackage getCommonPackage() {
		return (CommonPackage) getEPackage();
	}

	@Deprecated
	public static CommonPackage getPackage() {
		return CommonPackage.eINSTANCE;
	}

} //CommonFactory
