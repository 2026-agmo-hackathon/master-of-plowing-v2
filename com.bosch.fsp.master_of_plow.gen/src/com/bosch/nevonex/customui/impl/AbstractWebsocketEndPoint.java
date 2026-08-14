/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.customui.impl;

import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.logger.LoggerConstants;

import com.bosch.fsp.runtime.feature.GracefulFeatureStop;

import com.bosch.nevonex.customui.IAbstractWebsocketEndPoint;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.io.IOException;

import java.sql.Timestamp;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang3.exception.ExceptionUtils;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.EObjectImpl;

import org.eclipse.jetty.websocket.api.Session;

import org.eclipse.jetty.websocket.api.annotations.OnWebSocketClose;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketConnect;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketError;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketMessage;

/**
 * An implementation of the model object '<em><b>Abstract Websocket End Point</b></em>'.
 *
 * @generated
 */
public abstract class AbstractWebsocketEndPoint extends EObjectImpl implements IAbstractWebsocketEndPoint {

	/**
	 * @generated
	 */
	protected AbstractWebsocketEndPoint() {
		super();
	}

	private boolean logSessionNull = true;
	private Session wsSession;

	@OnWebSocketConnect
	public void connected(Session session) {
		this.setWsSession(session);
		FCALLogs.getInstance().log.info(LoggerConstants.LOG_SDK_PREFIX + "WebSocket Client is connected");
		publishCachedMessages();
	}

	@OnWebSocketClose
	public void closed(Session session, int statusCode, String reason) {
		if (GracefulFeatureStop.getInstance().isFeatureStopped())
			FCALLogs.getInstance().log.debug(LoggerConstants.LOG_SDK_PREFIX + " WebSocket Client is disconnected");
		else
			FCALLogs.getInstance().log.warn(LoggerConstants.LOG_SDK_PREFIX + " WebSocket Client is disconnected");
	}

	@OnWebSocketError
	public void throwError(Throwable error) {
		FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX + ExceptionUtils.getRootCauseMessage(error));
	}

	private void sendHeartBeat() {
		JsonObject message = new JsonObject();
		message.add("heart_beat", new Gson().toJsonTree(new Timestamp(System.currentTimeMillis()), Timestamp.class));
		broadcastMessage(message);
	}

	protected void createTimerForHeartBeat() {
		ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
		scheduler.scheduleAtFixedRate(new Runnable() {

			@Override
			public void run() {
				if (wsSession == null || !wsSession.isOpen()) {
					return;
				}
				sendHeartBeat();
			}
		}, 0, 60, TimeUnit.SECONDS);
	}

	public Session getWsSession() {
		return wsSession;
	}

	public void setWsSession(Session wsSession) {
		this.wsSession = wsSession;
	}

	protected void cacheIncomingScalarMessages(String message, String type, String controlId) {
		// Subclasses can implement cache mechanism.
	}

	protected void cacheIncomingMessages(String message, String type, String controlId) throws IOException {
		// Subclasses can implement cache mechanism.
	}

	protected synchronized void publishCachedMessages() {
		// Subclasses can implement cache mechanism.
	}

	/**
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return CustomuiPackage.Literals.ABSTRACT_WEBSOCKET_END_POINT;
	}

	/**
	 * @generated
	 */
	public synchronized void broadcastMessage(JsonObject jsonMessage) {
		if (jsonMessage == null || jsonMessage.isJsonNull() || jsonMessage.toString().isEmpty()) {
			FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX + "Message is null");
			return;
		}
		broadcastMessage(jsonMessage.toString());
	}

	/**
	 * @generated
	 */
	public synchronized void broadcastMessage(String message) {
		if (GracefulFeatureStop.getInstance().isFeatureStopped()) {
			FCALLogs.getInstance().log.debug(LoggerConstants.LOG_SDK_PREFIX
					+ " The feature is going to be stopped, so HMI messages cannot be processed.");
			return;
		}
		if (message == null) {
			FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX + " Message is null");
			return;
		}
		JsonObject jsonMessage = JsonParser.parseString(message).getAsJsonObject();
		jsonMessage.addProperty("TS", System.currentTimeMillis());
		try {
			if (wsSession == null || !wsSession.isOpen()) {
				if (logSessionNull) {
					FCALLogs.getInstance().log
							.warn(LoggerConstants.LOG_SDK_PREFIX + "WebSocket session is not yet established.");
					logSessionNull = false;
				}
			} else {
				wsSession.getRemote().sendString(message);
				logSessionNull = true;
				FCALLogs.getInstance().log
						.debug(LoggerConstants.LOG_SDK_PREFIX + " topic published: " + jsonMessage.get("topic") + " : "
								+ jsonMessage.get("payload") + " , " + "TS: " + jsonMessage.get("TS"));
			}
		} catch (IOException e) {
			FCALLogs.getInstance().log.error(LoggerConstants.LOG_SDK_PREFIX
					+ " Message cannot be sent to the websocket session. " + ExceptionUtils.getRootCauseMessage(e));
			FCALLogs.getInstance().log
					.debug(LoggerConstants.LOG_SDK_PREFIX + "Message cannot be sent to the websocket session.", e);
		}
	}

} //AbstractWebsocketEndPoint
