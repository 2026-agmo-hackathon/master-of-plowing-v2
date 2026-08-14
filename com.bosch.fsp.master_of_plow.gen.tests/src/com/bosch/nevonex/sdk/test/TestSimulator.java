package com.bosch.nevonex.sdk.test;

import java.io.IOException;
import com.bosch.fsp.runtime.util.internal.FeatureConfig;


public class TestSimulator {

	public static void main(String[] args) throws IOException, Exception {
		FeatureConfig.getInstance().loadFeatureConfiguration();
		MockFIF mockFif = new MockFIF();
		mockFif.mockServices();
		TestCustomUI customUi = new TestCustomUI();
		customUi.mockServices();
		TestFilClient filClient = new TestFilClient();
		filClient.createDom(TestFilClient.TOPIC_CREATION, "./data/sample_data.xml");
		Thread.sleep(2 * 1000);
		filClient.simulateData(filClient);
	}
}
