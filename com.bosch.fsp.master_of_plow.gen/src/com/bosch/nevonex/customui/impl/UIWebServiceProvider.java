/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.runtime.feature.notification.MqttFeatureNotification;
import com.bosch.fsp.runtime.feature.notification.Notification;

import com.bosch.fsp.runtime.util.internal.FeatureConfig;

import com.bosch.nevonex.customui.IUIWebServiceProvider;
import com.bosch.nevonex.customui.PortEnum;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.EObjectImpl;

import spark.Filter;
import spark.Request;
import spark.Response;
import spark.Service;

/**
 * An implementation of the model object '<em><b>UI Web Service Provider</b></em>'.
 *
 * @generated
 */
public class UIWebServiceProvider extends EObjectImpl implements IUIWebServiceProvider {

	/**
	 * @generated
	 */
	private UIWebServiceProvider() {
		super();
	}

	private static UIWebServiceProvider instance = null;
	private Service uiServer = null;
	private boolean isDefaultFilterEnabled = true;
	private boolean isDefaultOptionsEnabled = true;

	public static UIWebServiceProvider getInstance() {
		if (instance == null) {
			instance = new UIWebServiceProvider();
		}
		return instance;
	}

	public void startHMIControls() {
		ensureInitialized();
		uiServer.init();
		uiServer.awaitInitialization();

		if (isDefaultFilterEnabled) {
			FCALLogs.getInstance().log.info(LoggerConstants.LOG_SDK_PREFIX + "Registring the default route filter");
			registerBeforeFilter((request, response) -> {
				response.header("Access-Control-Allow-Origin", "*");
			});
		} else {
			FCALLogs.getInstance().log.info(LoggerConstants.LOG_SDK_PREFIX
					+ "Ignoring default route filter, since user-defined filter is enabled");
		}

		if (isDefaultOptionsEnabled) {
			try {
				FCALLogs.getInstance().log.info(LoggerConstants.LOG_SDK_PREFIX + "Registring the default route option");
				registerOptionsService("*", new NevonexRoute() {
					@Override
					protected Object processService(Request request, Response response) {
						response.header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
						response.header("Access-Control-Allow-Headers", "Content-Type, Authorization, Accept");
						response.header("Access-Control-Allow-Credentials", "true");
						return "OK";
					}
				});
			} catch (Exception e) {
				FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX + e.getMessage(), e);
			}
		} else {
			FCALLogs.getInstance().log.info(LoggerConstants.LOG_SDK_PREFIX
					+ "Ignoring default route option, since user-defined option is enabled");
		}

		MqttFeatureNotification.getInstance().publishNotification(Notification.FEATURE_BEFORE_READY);
	}

	public void stopHMIControls() {
		uiServer.stop();
		uiServer.awaitStop();
	}

	public void initHMIControls() {
		if (uiServer == null) {
			uiServer = Service.ignite().port(FeatureConfig.getInstance().getCustomUIPort());
		}
	}

	private void ensureInitialized() {
		if (uiServer == null) {
			initHMIControls();
		}
	}

	public void openWebsocket(final String path, Object handler) {
		ensureInitialized();
		uiServer.webSocket(path, handler);
	}

	public void registerBeforeFilter(Filter filter) {
		ensureInitialized();
		if (isDefaultFilterEnabled) {
			isDefaultFilterEnabled = false;
		}
		uiServer.before(filter);
	}

	public void registerOptionsService(String serviceName, NevonexRoute provider) throws Exception {
		ensureInitialized();
		if (isDefaultOptionsEnabled) {
			isDefaultOptionsEnabled = false;
		}
		uiServer.options("/" + serviceName, provider);
	}

	public void enableCORS() {
		ensureInitialized();
		uiServer.options("/*", (request, response) -> {
			response.header("Access-Control-Allow-Origin", "*");
			response.header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
			String accessControlRequestHeaders = request.headers("Access-Control-Request-Headers");
			if (accessControlRequestHeaders != null) {
				response.header("Access-Control-Allow-Headers", accessControlRequestHeaders);
			}
			return "OK";
		});
	}

	public void registerGetService(String serviceName, NevonexRoute provider) throws Exception {
		ensureInitialized();
		uiServer.get("/" + serviceName, provider);
	}

	public void registerPostService(String serviceName, NevonexRoute provider) throws Exception {
		ensureInitialized();
		uiServer.post("/" + serviceName, provider);
	}

	public void registerPutService(String serviceName, NevonexRoute provider) throws Exception {
		ensureInitialized();
		uiServer.put("/" + serviceName, provider);
	}

	public void registerDeleteService(String serviceName, NevonexRoute provider) throws Exception {
		ensureInitialized();
		uiServer.delete("/" + serviceName, provider);
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return CustomuiPackage.Literals.UI_WEB_SERVICE_PROVIDER;
	}

} //UIWebServiceProvider
