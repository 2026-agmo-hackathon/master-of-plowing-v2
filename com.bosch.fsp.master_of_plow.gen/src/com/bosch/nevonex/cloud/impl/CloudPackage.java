/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.cloud.impl;

import com.bosch.fsp.runtime.feature.ICloudDownload;
import com.bosch.nevonex.cloud.ICloud;
import com.bosch.nevonex.cloud.ICloudFactory;
import com.bosch.nevonex.cloud.impl.CloudFactory;

import com.bosch.nevonex.types.impl.TypesPackage;

import org.eclipse.emf.ecore.EAttribute;
import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EFactory;
import org.eclipse.emf.ecore.EOperation;
import org.eclipse.emf.ecore.EPackage;
import org.eclipse.emf.ecore.EParameter;
import org.eclipse.emf.ecore.EReference;
import org.eclipse.emf.ecore.EcoreFactory;
import org.eclipse.emf.ecore.EcorePackage;

import org.eclipse.emf.ecore.impl.EPackageImpl;

/**
 * @generated
 */
public class CloudPackage extends EPackageImpl {
	public static final String eNAME = "cloud";
	public static final String eNS_URI = "com.bosch.nevonex.cloud";
	public static final String eNS_PREFIX = "cloud";
	public static final CloudPackage eINSTANCE = com.bosch.nevonex.cloud.impl.CloudPackage.init();

	public static final int I_CLOUD_DOWNLOAD = 0;
	public static final int I_CLOUD_DOWNLOAD_FEATURE_COUNT = 0;
	public static final int I_CLOUD_DOWNLOAD_OPERATION_COUNT = 0;

	public static final int CLOUD = 1;
	public static final int CLOUD_FEATURE_COUNT = I_CLOUD_DOWNLOAD_FEATURE_COUNT + 0;
	public static final int CLOUD___PROCESS_DOWNLOAD_MESSAGE__STRING = 0;
	public static final int CLOUD___INIT_PLATFORM_SERVICE = 1;
	public static final int CLOUD___START_PLATFORM_SERVICE = 2;
	public static final int CLOUD___STOP_PLATFORM_SERVICE = 3;
	public static final int CLOUD___GET_FILE_PATH = 4;
	public static final int CLOUD___UPLOAD_DATA__STRING__INT = 5;
	public static final int CLOUD___UPLOAD_DATA__STRING__INT__CONNECTIONTYPEENUM = 6;
	public static final int CLOUD___UPLOAD_FILE__FILE__INT = 7;
	public static final int CLOUD___UPLOAD_FILE__STRING__INT = 8;
	public static final int CLOUD___UPLOAD_FILE__FILE__INT__CONNECTIONTYPEENUM = 9;
	public static final int CLOUD___UPLOAD_FILE__STRING__INT__CONNECTIONTYPEENUM = 10;
	public static final int CLOUD_OPERATION_COUNT = I_CLOUD_DOWNLOAD_OPERATION_COUNT + 11;


	private EClass iCloudDownloadEClass = null;
	private EClass cloudEClass = null;

	private static boolean isInited = false;
	private boolean isCreated = false;
	private boolean isInitialized = false;

	public CloudPackage() {
		super();
		setName(eNAME);
		setNsPrefix(eNS_PREFIX);
		setNsURI(eNS_URI);
		setEFactoryInstance(CloudFactory.eINSTANCE);
	}

	public static CloudPackage init() {
		if (isInited) return (CloudPackage) EPackage.Registry.INSTANCE.getEPackage(eNS_URI);
		Object registered = EPackage.Registry.INSTANCE.get(eNS_URI);
		CloudPackage thePackage = (registered instanceof CloudPackage) ? (CloudPackage) registered : new CloudPackage();
		isInited = true;
		EcorePackage.eINSTANCE.eClass();
		TypesPackage.eINSTANCE.eClass();
		thePackage.createPackageContents();
		thePackage.initializePackageContents(thePackage);
		thePackage.freeze();
		EPackage.Registry.INSTANCE.put(eNS_URI, thePackage);
		return thePackage;
	}

	private void createPackageContents() {
		if (isCreated) return;
		isCreated = true;
		createPackageContents_part0();
	}

	private void createPackageContents_part0() {
		iCloudDownloadEClass = createEClass(I_CLOUD_DOWNLOAD);

		cloudEClass = createEClass(CLOUD);
		createEOperation(cloudEClass, CLOUD___PROCESS_DOWNLOAD_MESSAGE__STRING);
		createEOperation(cloudEClass, CLOUD___INIT_PLATFORM_SERVICE);
		createEOperation(cloudEClass, CLOUD___START_PLATFORM_SERVICE);
		createEOperation(cloudEClass, CLOUD___STOP_PLATFORM_SERVICE);
		createEOperation(cloudEClass, CLOUD___GET_FILE_PATH);
		createEOperation(cloudEClass, CLOUD___UPLOAD_DATA__STRING__INT);
		createEOperation(cloudEClass, CLOUD___UPLOAD_DATA__STRING__INT__CONNECTIONTYPEENUM);
		createEOperation(cloudEClass, CLOUD___UPLOAD_FILE__FILE__INT);
		createEOperation(cloudEClass, CLOUD___UPLOAD_FILE__STRING__INT);
		createEOperation(cloudEClass, CLOUD___UPLOAD_FILE__FILE__INT__CONNECTIONTYPEENUM);
		createEOperation(cloudEClass, CLOUD___UPLOAD_FILE__STRING__INT__CONNECTIONTYPEENUM);

	}

	private void initializePackageContents(CloudPackage thePackage) {
		if (isInitialized) return;
		isInitialized = true;
		initializePackageContents_part0(thePackage);
	}

	private void initializePackageContents_part0(CloudPackage thePackage) {
		thePackage.iCloudDownloadEClass.setName("ICloudDownload");
		thePackage.iCloudDownloadEClass.setInstanceClassName("com.bosch.fsp.runtime.feature.ICloudDownload");
		thePackage.iCloudDownloadEClass.setAbstract(true);
		thePackage.iCloudDownloadEClass.setInterface(true);
		thePackage.cloudEClass.setName("Cloud");
		thePackage.cloudEClass.setInstanceClassName("com.bosch.nevonex.cloud.ICloud");
		thePackage.cloudEClass.setAbstract(false);
		thePackage.cloudEClass.setInterface(false);
		thePackage.cloudEClass.getESuperTypes().add((EClass) thePackage.iCloudDownloadEClass);
		thePackage.cloudEClass.getESuperTypes().add((EClass) TypesPackage.eINSTANCE.getPropertyChange());
		thePackage.cloudEClass.getEOperations().get(0).setName("processDownloadMessage");
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("message");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.cloudEClass.getEOperations().get(0).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(1).setName("initPlatformService");
		thePackage.cloudEClass.getEOperations().get(2).setName("startPlatformService");
		thePackage.cloudEClass.getEOperations().get(3).setName("stopPlatformService");
		thePackage.cloudEClass.getEOperations().get(4).setName("getFilePath");
		thePackage.cloudEClass.getEOperations().get(4).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		thePackage.cloudEClass.getEOperations().get(5).setName("uploadData");
		thePackage.cloudEClass.getEOperations().get(5).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("data");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.cloudEClass.getEOperations().get(5).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(5).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(6).setName("uploadData");
		thePackage.cloudEClass.getEOperations().get(6).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("data");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.cloudEClass.getEOperations().get(6).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(6).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("connectionType");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.cloudEClass.getEOperations().get(6).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(7).setName("uploadFile");
		thePackage.cloudEClass.getEOperations().get(7).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("file");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.cloudEClass.getEOperations().get(7).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(7).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(8).setName("uploadFile");
		thePackage.cloudEClass.getEOperations().get(8).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("filePath");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.cloudEClass.getEOperations().get(8).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(8).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(9).setName("uploadFile");
		thePackage.cloudEClass.getEOperations().get(9).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("file");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.cloudEClass.getEOperations().get(9).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(9).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("connectionType");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.cloudEClass.getEOperations().get(9).getEParameters().add(param);
		}
		thePackage.cloudEClass.getEOperations().get(10).setName("uploadFile");
		thePackage.cloudEClass.getEOperations().get(10).setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("filePath");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEString());
			thePackage.cloudEClass.getEOperations().get(10).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("priority");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEInt());
			thePackage.cloudEClass.getEOperations().get(10).getEParameters().add(param);
		}
		{
			EParameter param = EcoreFactory.eINSTANCE.createEParameter();
			param.setName("connectionType");
			param.setEType((EClassifier) EcorePackage.eINSTANCE.getEObject());
			thePackage.cloudEClass.getEOperations().get(10).getEParameters().add(param);
		}
	}

	public EClass getICloudDownload() { return iCloudDownloadEClass; }
	public EClass getCloud() { return cloudEClass; }
	public EOperation getCloud__ProcessDownloadMessage__String() {
		return cloudEClass.getEOperations().get(0);
	}
	public EOperation getCloud__InitPlatformService() {
		return cloudEClass.getEOperations().get(1);
	}
	public EOperation getCloud__StartPlatformService() {
		return cloudEClass.getEOperations().get(2);
	}
	public EOperation getCloud__StopPlatformService() {
		return cloudEClass.getEOperations().get(3);
	}
	public EOperation getCloud__GetFilePath() {
		return cloudEClass.getEOperations().get(4);
	}
	public EOperation getCloud__UploadData__String__int() {
		return cloudEClass.getEOperations().get(5);
	}
	public EOperation getCloud__UploadData__String__int__ConnectionTypeEnum() {
		return cloudEClass.getEOperations().get(6);
	}
	public EOperation getCloud__UploadFile__File__int() {
		return cloudEClass.getEOperations().get(7);
	}
	public EOperation getCloud__UploadFile__String__int() {
		return cloudEClass.getEOperations().get(8);
	}
	public EOperation getCloud__UploadFile__File__int__ConnectionTypeEnum() {
		return cloudEClass.getEOperations().get(9);
	}
	public EOperation getCloud__UploadFile__String__int__ConnectionTypeEnum() {
		return cloudEClass.getEOperations().get(10);
	}

	public ICloudFactory getCloudFactory() {
		return (ICloudFactory) getEFactoryInstance();
	}

	public interface Literals {
		public static final EClass CLOUD = eINSTANCE.getCloud();
	}

} //CloudPackage
