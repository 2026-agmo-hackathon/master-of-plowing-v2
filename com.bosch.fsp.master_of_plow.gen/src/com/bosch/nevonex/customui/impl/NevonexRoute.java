/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.runtime.feature.GracefulFeatureStop;

import com.bosch.nevonex.customui.INevonexRoute;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.EObjectImpl;

import spark.Request;
import spark.Response;
import spark.Route;

/**
 * An implementation of the model object '<em><b>Nevonex Route</b></em>'.
 *
 * @generated
 */
public abstract class NevonexRoute extends EObjectImpl implements INevonexRoute, Route {

	/**
	 * @generated
	 */
	protected NevonexRoute() {
		super();
	}

	protected abstract Object processService(Request request, Response response);

	@Override
	final public Object handle(Request request, Response response) throws Exception {
		if (GracefulFeatureStop.getInstance().isFeatureStopped()) {
			FCALLogs.getInstance().log.debug(LoggerConstants.LOG_SDK_PREFIX
					+ "The feature is going to be stopped, so rest api services cannot be processed.");
			return "";
		}
		response.header("Content-Type", "text/plain");
		return processService(request, response);
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return CustomuiPackage.Literals.NEVONEX_ROUTE;
	}

} //NevonexRoute
