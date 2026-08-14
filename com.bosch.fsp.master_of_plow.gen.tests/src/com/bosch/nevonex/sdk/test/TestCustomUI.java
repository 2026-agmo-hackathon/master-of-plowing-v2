package com.bosch.nevonex.sdk.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import com.bosch.fsp.logger.FCALLogs;

import spark.Request;
import spark.Response;
public class TestCustomUI {

	private int configuredPort;
	
	public int getConfiguredPort() {
		return configuredPort;
	}

	public void setConfiguredPort(int configuredPort) {
		this.configuredPort = configuredPort;
	}

	private void fetchConfiguredPort() {
		try {
			File featureConfigFile = new File("./feature.config");
			BufferedReader reader = new BufferedReader(new FileReader(featureConfigFile));
	            Gson gson = new Gson();
	            JsonObject root = gson.fromJson(reader, JsonObject.class);
			String uiTag="ui";
			boolean uiObj = root.has(uiTag);
			if (uiObj) {
				int portValue = root.get(uiTag).getAsJsonObject().get("customui_port").getAsInt();
				setConfiguredPort(portValue);
			}
		} catch (Exception e) {
			// nothing to do.
		}
	}

	public void mockServices() {
		try {
		   fetchConfiguredPort();
			boolean isServerRunning = UIWebServiceProvider.getInstance().isServerStarted();
			if (isServerRunning) {
				mockGetAssignedPorts();
				mockFeatureGatewayService();
			}else {
				FCALLogs.getInstance().log.error(
						"Mock Custom UI Server is not started since the assigned port 6563 is used by some other process.");
			}
		} catch (Exception e) {
			FCALLogs.getInstance().log.info(
					"Activating of Custom UI Mock services has been failed due to below reason \n" + e.getMessage());
		}
	}

	private void mockGetAssignedPorts() throws Exception {
		UIWebServiceProvider.getInstance().registerGetService("/get_assigned_ports", new NevonexRoute() {
			@Override
			protected Object getValue(Request arg0, Response arg1) {
				JsonObject ob = new JsonObject();
				// read port info from feature.config file.
				ob.addProperty("1456", getConfiguredPort());
				return ob;
			}
		});
	}

	private void mockFeatureGatewayService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("/feature_gateway", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				String body = request.body();
				String reposneVal = "";
				JsonObject objBody = JsonParser.parseString(body).getAsJsonObject();
				String methodType = objBody.get("method").getAsString();
	                String url = objBody.get("url").getAsString();
				String portKey = String.valueOf(getConfiguredPort());
				String link = "http://localhost:" + portKey + "/" + url;
				if ("GET".equalsIgnoreCase(methodType)) {
					reposneVal = getData(link);
				} else  {
					JsonObject dataJson = objBody.get("data").getAsJsonObject();
					reposneVal = getData(link,methodType.toUpperCase(), dataJson.toString());
				}
				return reposneVal;
			}
		});
	}

	/**
	 * to get the Data from webservice according to the URL that is passed
	 *
	 * @param finalUrl
	 * @return
	 */
	private String getData(final String finalUrl) {
		String output;
		StringBuilder res = new StringBuilder();
		try {
			URL url = new URL(finalUrl);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestMethod("GET");
			conn.setRequestProperty("Accept", "application/json");
			if (conn.getResponseCode() != 200) {
				throw new RuntimeException("Failed : HTTP error code : " + conn.getResponseCode());
			}
			BufferedReader br = new BufferedReader(
					new InputStreamReader((conn.getInputStream()), StandardCharsets.UTF_8));
			while ((output = br.readLine()) != null) {
				res.append(output);
			}
			conn.disconnect();
		} catch (Exception e) {
		} //
		return res.toString();
	}

	/**
	 * to get the Data from webservice according to the URL that is passed
	 *
	 * @param finalUrl
	 * @return
	 */
	private String getData(final String finalUrl, final String type, final String data) {
		String output;
		StringBuilder res = new StringBuilder();
		try {
			URL url = new URL(finalUrl);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestMethod(type);
			conn.setRequestProperty("Content-Type", "application/json; utf-8");
			conn.setRequestProperty("Accept", "application/json");
			conn.setDoOutput(true);
			try (OutputStream os = conn.getOutputStream()) {
				if (data != null) {
					byte[] input = data.getBytes("utf-8");
					os.write(input, 0, input.length);
				}
			}
			if (conn.getResponseCode() != 200) {
				throw new RuntimeException("Failed : HTTP error code : " + conn.getResponseCode());
			}
			BufferedReader br = new BufferedReader(
					new InputStreamReader((conn.getInputStream()), StandardCharsets.UTF_8));
			while ((output = br.readLine()) != null) {
				res.append(output);
			}
			conn.disconnect();
		} catch (Exception e) {
		} //
		return res.toString();
	}
}
