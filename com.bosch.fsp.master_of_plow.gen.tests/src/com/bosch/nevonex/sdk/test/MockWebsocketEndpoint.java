/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.sdk.test;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;

import org.eclipse.jetty.websocket.api.Session;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketClose;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketConnect;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketError;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketMessage;
import org.eclipse.jetty.websocket.api.annotations.WebSocket;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

/**
 * @author RME2COB
 */
@WebSocket(maxTextMessageSize = 64 * 1024)
public class MockWebsocketEndpoint {

  private static MockWebsocketEndpoint instance = null;

  public static MockWebsocketEndpoint getInstance() {
    if (instance == null) {
      instance = new MockWebsocketEndpoint();
    }
    return instance;
  }

  private final CountDownLatch closeLatch;

  @SuppressWarnings("unused")
  private Session session;

  public MockWebsocketEndpoint() {
    this.closeLatch = new CountDownLatch(1);
  }

  public boolean awaitClose(final int duration, final TimeUnit unit) throws InterruptedException {
    return this.closeLatch.await(duration, unit);
  }

  @OnWebSocketClose
  public void onClose(final int statusCode, final String reason) {
    System.out.printf("Connection closed: %d - %s%n", statusCode, reason);
    this.session = null;
    this.closeLatch.countDown();
  }

  @OnWebSocketConnect
  public void onConnect(final Session session) {
    System.out.printf("Got connect: %s%n", session);
    this.session = session;
  }

  public void sendMessage(final Session sessionfromclient, final String message)
      throws InterruptedException, ExecutionException, IOException {
    sessionfromclient.getRemote().sendString(message);
  }

  @OnWebSocketMessage
  public void onMessage(final String msg) {
    JsonObject json = JsonParser.parseString(msg).getAsJsonObject();
    MockWebsocketClient.TopicPayloadMap.put(json.get("topic").getAsString(), json.get("payload").getAsJsonObject());
  }

  @OnWebSocketError
  public void onError(final Throwable error) {
    System.out.println(error);
  }
}
