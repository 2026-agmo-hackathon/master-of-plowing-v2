/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.sdk.test;

import java.io.IOException;
import java.net.URI;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutionException;

import org.eclipse.jetty.websocket.api.Session;
import org.eclipse.jetty.websocket.client.ClientUpgradeRequest;
import org.eclipse.jetty.websocket.client.WebSocketClient;
import com.google.gson.JsonObject;


public class MockWebsocketClient {

  WebSocketClient client;
  MockWebsocketEndpoint socket;
  Session session;
  static Map<String, JsonObject> TopicPayloadMap = new HashMap<>();

  public void stopClient() throws Exception {
    this.client.stop();
  }

  public void sendMessage(final String message, Session session2) throws InterruptedException, ExecutionException, IOException {
    MockWebsocketEndpoint.getInstance().sendMessage(session2, message);
  }


}
