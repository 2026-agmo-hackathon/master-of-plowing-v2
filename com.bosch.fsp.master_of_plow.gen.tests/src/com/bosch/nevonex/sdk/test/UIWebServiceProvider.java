package com.bosch.nevonex.sdk.test;

import static spark.Spark.get;
import static spark.Spark.port;
import static spark.Spark.post;

import spark.Spark;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.io.File;
import java.io.IOException;

/**
 * An implementation of the model object '<em><b>UI Web Service Provider</b></em>'.
 *
 * @generated
 */
public class UIWebServiceProvider  {

	private UIWebServiceProvider() {
		super();
	}

	private static UIWebServiceProvider instance = null;
	private boolean serverStarted = false;

	public boolean isServerStarted() {
		return serverStarted;
	}
	
	public static UIWebServiceProvider getInstance() throws IOException{
		if (instance == null) {
			instance = new UIWebServiceProvider();
			instance.registerPort();
		}
		return instance;
	}
	
	private void registerPort() throws java.io.IOException{
		int port = 6563;
		boolean isPortOpen = portIsOpen("127.0.0.1", port);
		if (!isPortOpen) {
			Spark.ipAddress("127.0.0.1");
			Spark.staticFileLocation("/public");
		    boolean isPathExist = addExternalFileLocation();
			if (isPathExist) {
				port(port);
				Spark.init();
				serverStarted = true;
			} else {
				throw new IOException(
						"UI Folder location of an application is not valid. Please configure correct valid path in Simulator.properties file.");
			}
		}
	}
	
	   private boolean portIsOpen(final String ip, final int port) {
		try (Socket socket = new Socket()) {
			socket.connect(new InetSocketAddress(ip, port));
			return true;
		} catch (Exception ex) {
			return false;
		}
	}

	public void registerGetService(String serviceName, NevonexRoute provider) throws Exception {
		get("/" + serviceName, provider);
	}

	public void registerPostService(String serviceName, NevonexRoute provider) throws Exception {
		post("/" + serviceName, provider);
	}

	   private boolean addExternalFileLocation() {
	      boolean isPathExist = true;
		try {
			java.util.Properties props = com.bosch.nevonex.sdk.test.SDKTest.readSimulatorPropertiesFile();
		    String uiPath = (String) props.get("uiFolderLocation");
			if (uiPath != null) {
				File path = new File(uiPath);
				if (!path.exists()) {
					isPathExist = false;
					return isPathExist;
				}
				Spark.externalStaticFileLocation(uiPath);
			}
		} catch (Exception e) {
			com.bosch.fsp.logger.FCALLogs.getInstance().log.error("External File location of UI content is not found.");
		}
		return isPathExist;
	}
	
}
