package com.bosch.nevonex.sdk.test;

import spark.Request;
import spark.Response;
import spark.Route;
/**
 * An implementation of the model object '<em><b>Nevonex Route</b></em>'.
 *
 *
 */
public abstract class NevonexRoute implements Route {

	/**
	 * @generated
	 */
	protected NevonexRoute() {
		super();
	}

	protected abstract Object getValue(Request request, Response response);

	final public Object handle(Request request, Response response) throws Exception {
		response.header("Access-Control-Allow-Origin", "*");
		response.header("Content-Type", "application/Json");
		return getValue(request, response);
	}

}
