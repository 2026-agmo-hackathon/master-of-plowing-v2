package com.bosch.nevonex.sdk.test;

import java.io.File;
import org.apache.commons.io.FileUtils;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.logging.Level;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.bosch.fsp.logger.FCALLogs;
import spark.Request;
import spark.Response;
import java.io.InputStream;
import java.util.Collection;

import javax.servlet.MultipartConfigElement;
import javax.servlet.ServletException;
import javax.servlet.http.Part;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import com.google.gson.JsonArray;

public class MockFIF {
	private String userDir= System.getProperty("user.home");
	private String msgUpload="msg_upload";
	private String msgDownload="msg_download";
	private String platformCloud="cloud";
	private String platformDevice="device";
	private String emulatorDir=userDir+File.separator +".nevonex" +File.separator +"emulator" +File.separator ;
	private String cloudOutPath= emulatorDir +platformCloud+ File.separator +"out" + File.separator ;
	private String deviceOutPath=emulatorDir +platformDevice+ File.separator +"out" + File.separator ;
	private String tempDir="/temp";
	private String textFileFormat=".txt";
	private String type="type";
	private String data="data";
	private String filePath = "filePath";
	private MqttClient client;
	private String broker = "tcp://localhost:1883";

	public MockFIF() {
		try {
			init();
		} catch (MqttException e) {
			e.printStackTrace();
		}
	}

	/**
	 * @throws MqttException
	 */
	private void init() throws MqttException {
		this.client = new MqttClient(this.broker, String.valueOf(System.nanoTime()), new MemoryPersistence());
		this.client.connect(createConnectionOptions());
		this.client.isConnected();

	}

	private MqttConnectOptions createConnectionOptions() {
		MqttConnectOptions connOpts = new MqttConnectOptions();
		connOpts.setMaxInflight(1000);
		connOpts.setCleanSession(true); // no persistent session
		connOpts.setKeepAliveInterval(1000);
		connOpts.setAutomaticReconnect(true);
		return connOpts;
	}

	public void mockServices() {
		try {
			boolean isServerRunning = UIWebServiceProvider.getInstance().isServerStarted();
			if (isServerRunning) {
				mockFCLLogs();
				mockEnableCloudUploadAccessService();
				mockCloudUploadService();
				mockUIUploadFileService();
				mockUIDownloadFileService();
				mockUIPushFileService();
				mockUIPushMessageService();
				mockUIShowFileService();

				//Ntrip mocks
				mockCurrentNtripProfileService();
				mockNtripConnectService();
				mockNtripDisconnectService();
			} else {
				FCALLogs.getInstance().log.error(
						"Mock FIF Server is not started since the assigned port 6563 is used by some other process.");
			}
		} catch (Exception e) {
			FCALLogs.getInstance().log
					.info("Activating of Mock services has been failed due to below reason " + e.getMessage());

		}
	}
	private void mockEnableCloudUploadAccessService() throws Exception {
		UIWebServiceProvider.getInstance().registerGetService("fcal/uploadaccessavailable/nevonex_feature",
				new NevonexRoute() {
					@Override
					protected Object getValue(Request arg0, Response arg1) {
						return true;
					}
				});
	}

	private void mockCloudUploadService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("fcal/upload", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				request.attribute("org.eclipse.jetty.multipartConfig", new MultipartConfigElement(tempDir));
				try {
					Part dataPart = request.raw().getPart(data);
					if (dataPart != null) {
						// message upload
						String timeStamp = String.valueOf(System.currentTimeMillis());
						String fileLocation = cloudOutPath + msgUpload + timeStamp + textFileFormat;
						try (InputStream is = dataPart.getInputStream()) {
							// Use the input stream to create a file
							FileUtils.copyInputStreamToFile(is, new File(fileLocation));
						} catch (IOException e) {
							FCALLogs.getInstance().log.debug("Cloud upload from feature has been failed.");
						}
					} else {
						// file upload.
						Part filePart = request.raw().getPart(filePath);
						if (filePart != null && !request.raw().getParameterMap().isEmpty()) {
							if (request.raw().getParameterMap().containsKey("filePath")) {
								String[] pathInfo = request.raw().getParameterMap().get("filePath");
								String fileLocation = pathInfo[0];
								String desDir = cloudOutPath;
								FileUtils.copyFileToDirectory(new File(fileLocation), new File(desDir));
							}
						}
					}

				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("Mock Cloud upload service has been failed .");
				}
				return response;
			}
		});
	}
	private void mockUIUploadFileService() throws Exception {
		UIWebServiceProvider.getInstance().registerGetService("uploadFiles", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				String platform = platformCloud;
				JsonArray jsonArray = new JsonArray();
				File uploadDir = new File(cloudOutPath);
				if (uploadDir.exists()) {
					createJsonUploadFileResponseObj(platform, jsonArray, uploadDir);
				}
				platform = platformDevice;
				File deviceuploadDir = new File(deviceOutPath);
				if (deviceuploadDir.exists()) {
					createJsonUploadFileResponseObj(platform, jsonArray, deviceuploadDir);
				}

				return jsonArray;
			}

			private void createJsonUploadFileResponseObj(String platform, JsonArray jsonArray, File deviceuploadDir) {
				Collection<File> listFiles = FileUtils.listFiles(deviceuploadDir, null, true);
				for (File curFile : listFiles) {
					JsonObject curFileJsonObj = new JsonObject();
					String fileName = curFile.getName();
					String filePath = curFile.getAbsolutePath();
					String type;
					if (fileName.startsWith(msgUpload)) {
						// data upload
						type = "message";
					} else {
						// file upload
						type = "file";
					}
					curFileJsonObj.addProperty("name", fileName);
					curFileJsonObj.addProperty("path", filePath);
					curFileJsonObj.addProperty("type", type);
					curFileJsonObj.addProperty("platform", platform);
					jsonArray.add(curFileJsonObj);
				}
			}
		});
	}

	private void mockUIDownloadFileService() throws Exception {
		UIWebServiceProvider.getInstance().registerGetService("downloadedFiles", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				String platform = platformCloud;
				JsonArray jsonArray = new JsonArray();
				File downloadDir = new File(
						emulatorDir + platform + File.separator + "in");
				if (downloadDir.exists()) {
					createJsonFileDownloadResObj(platform, jsonArray, downloadDir);
				}
				platform = platformDevice;
				File deviceDownloadDir = new File(
						emulatorDir + platform + File.separator + "in");
				if (deviceDownloadDir.exists()) {
					createJsonFileDownloadResObj(platform, jsonArray, deviceDownloadDir);
				}
				return jsonArray;
			}

			private void createJsonFileDownloadResObj(String platform, JsonArray jsonArray, File deviceuploadDir) {
				Collection<File> listFiles = FileUtils.listFiles(deviceuploadDir, null, true);
				for (File curFile : listFiles) {
					JsonObject curFileJsonObj = new JsonObject();
					String fileName = curFile.getName();
					String filePath = curFile.getAbsolutePath();
					String type;
					if (fileName.startsWith(msgDownload)) {
						// data upload
						type = "message";
					} else {
						// file upload
						type = "file";
					}
					curFileJsonObj.addProperty("name", fileName);
					curFileJsonObj.addProperty("path", filePath);
					curFileJsonObj.addProperty("type", type);
					curFileJsonObj.addProperty("platform", platform);
					jsonArray.add(curFileJsonObj);
				}
			}
		});
	}

	private void mockUIPushFileService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("uploadfile/*/*", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				request.attribute("org.eclipse.jetty.multipartConfig", new MultipartConfigElement(tempDir));
				try {
					if (request.raw() != null && !request.raw().getParts().isEmpty()) {
						String[] splat = request.splat();
						String fileName = splat[0];
						String platform = splat[1].toLowerCase();
						InputStream is = request.raw().getParts().iterator().next().getInputStream();
						String fileLocation = emulatorDir + platform + File.separator + "in" + File.separator + fileName;
						FileUtils.copyInputStreamToFile(is, new File(fileLocation));
						if (platform.equalsIgnoreCase(platformCloud)) {
							publishCloudDataAvailable(fileName, fileLocation, null);
						} else {
							publishDeviceDataAvailable(fileName, fileLocation, null);
						}
					}
				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("uploading a file from the feature has been failed.");
				}
				JsonObject ob = new JsonObject();
				ob.addProperty("Status", "Success");
				return ob;
			}
		});
	}

	private void mockUIPushMessageService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("pushmsgtofeature/*", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				request.attribute("org.eclipse.jetty.multipartConfig", new MultipartConfigElement(tempDir));

				String message = request.body();
				if (message != null) {
					String[] splat = request.splat();
					String platform = splat[0].toLowerCase();
					String timeStamp = String.valueOf(System.currentTimeMillis());
					String fileName = msgDownload + timeStamp + textFileFormat;
					String fileLocation = emulatorDir + platform + File.separator + "in" + File.separator + fileName;
					File messageFile = new File(fileLocation);
					try {
						FileUtils.writeStringToFile(messageFile, message, Charset.defaultCharset());
						if (platform.equalsIgnoreCase(platformCloud)) {
							publishCloudDataAvailable(fileName, fileLocation, message);
						} else {
							publishDeviceDataAvailable(fileName, fileLocation, message);
						}

					} catch (Exception e) {
						FCALLogs.getInstance().log.debug("message from the platform(Cloud/Device) to the feature has been failed.");
					}
				}
				JsonObject ob = new JsonObject();
				ob.addProperty("status", "success");
				return ob;
			}
		});
	}

	private void mockUIShowFileService() throws Exception {
		UIWebServiceProvider.getInstance().registerGetService("showfile/*", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				String[] values = request.splat();
				File file = new File(File.separator + values[0]);
				String fileName = file.getName();
				try {
					byte[] bytes = FileUtils.readFileToByteArray(file);
					response.header("Content-Disposition", "attachment; filename=" + fileName);
					response.raw().setContentType("application/octet-stream");
					response.raw().getOutputStream().write(bytes);
					response.raw().getOutputStream().flush();
				} catch (IOException e) {
					FCALLogs.getInstance().log.debug("show file service has been failed.  ");
				}
				return response;
			}
		});
	}

	private void publishCloudDataAvailable(String fileName, String filePath, String content) throws Exception {
		JsonObject dataObj = new JsonObject();
		String reqDirPath = filePath.replace(fileName, "");
		if (content != null) {
			dataObj.addProperty("content", content);
		} else {
			dataObj.addProperty("fileName", fileName);
		}
		dataObj.addProperty("path", reqDirPath);
		client.publish("fek/dataavailable/nevonex_feature", dataObj.toString().getBytes(), 2, false);
		FCALLogs.getInstance().log.info("publish to fek/dataavailable/nevonex_feature" + " Payload : " + dataObj.toString());
	}

	private void publishDeviceDataAvailable(String fileName, String filePath, String content) throws Exception {
		JsonObject dataObj = new JsonObject();
		String reqDirPath = filePath.replace(fileName, "");
		if (content != null) {
			dataObj.addProperty("content", content);
		} else {
			dataObj.addProperty("fileName", fileName);
		}
		dataObj.addProperty("path", reqDirPath);
		client.publish("fek/devicedataavailable/nevonex_feature", dataObj.toString().getBytes(), 2, false);
		FCALLogs.getInstance().log.info("publish to fek/devicedataavailable/nevonex_feature" + " Payload : " + dataObj.toString());
	}
	private void mockFCLLogs() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("/fcal_logging", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				try {
					String body = request.body();
					System.out.println(body);
					response.body("OK");
				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("error on printing logging");
				}
				return response;
			}
		});
	}

	private void mockCurrentNtripProfileService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("fcal/current_ntrip_profile", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				try {
					String requestJson = request.body();
					System.out.println("The request json content -> " + requestJson);

					CurrentNtripProfilePojo cnpp = new CurrentNtripProfilePojo();
					String resp = new Gson().toJson(cnpp);
					System.out.println("Resp -> " + resp);
					return resp.toString();
				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("Mock Cloud upload service has been failed .");
				}
				return response;
			}
		});
	}

	private void mockNtripConnectService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("fcal/ntrip_connect", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				try {
					String requestJson = request.body();
					System.out.println("The request json content -> " + requestJson);

					NtripConnectPojo ncp = new NtripConnectPojo();

					String resp = new Gson().toJson(ncp);
					System.out.println("Resp -> " + resp);
					return resp;
				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("Mock Cloud upload service has been failed .");
				}
				return response;
			}
		});
	}

	private void mockNtripDisconnectService() throws Exception {
		UIWebServiceProvider.getInstance().registerPostService("fcal/ntrip_disconnect", new NevonexRoute() {

			@Override
			protected Object getValue(Request request, Response response) {
				try {
					String requestJson = request.body();
					System.out.println("The request json content -> " + requestJson);

					NtripDisconnectPojo ndp = new NtripDisconnectPojo();

					String resp = new Gson().toJson(ndp);
					System.out.println("Resp -> " + resp);
					return resp;
				} catch (Exception e) {
					FCALLogs.getInstance().log.debug("Mock Cloud upload service has been failed .");
				}
				return response;
			}
		});
	}

	public static void main(String[] args) {
		MockFIF mockFif = new MockFIF();
		mockFif.mockServices();
	}
}
