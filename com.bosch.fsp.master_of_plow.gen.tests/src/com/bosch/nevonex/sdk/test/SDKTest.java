package com.bosch.nevonex.sdk.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import org.eclipse.emf.ecore.EStructuralFeature;
import java.util.List;
import java.util.Properties;
import org.junit.Test;
import java.io.FileInputStream;
import java.io.IOException;
import org.eclipse.emf.ecore.EClass;
import java.util.Random;
import java.util.Map;
import java.util.HashMap;
import java.util.concurrent.ThreadLocalRandom;
import org.eclipse.emf.common.util.Enumerator;
import com.bosch.nevonex.common.IAbsolutePosition;
import com.bosch.nevonex.common.impl.CommonFactory;
import com.bosch.nevonex.common.ProviderEnum;
import com.bosch.fsp.logger.FCALLogs;
import com.bosch.fsp.runtime.feature.exception.NevonexException;
import com.bosch.fsp.runtime.registry.FCALRuntime;
import org.junit.BeforeClass;
import com.bosch.nevonex.fcb.impl.FcbPackage;
import com.bosch.nevonex.fcal.impl.FcalPackage;
import com.bosch.nevonex.can_agmo_customized_tractor.impl.Can_agmo_customized_tractorPackage;
import com.bosch.nevonex.can_agmo_customized_tractor.impl.CAN_AGMO_Customized_TractorProvider;
import com.bosch.nevonex.can_agmo_customized_tractor.impl.CAN_AGMO_Customized_Tractor;
import com.bosch.nevonex.can_agmo_mtlt305.impl.Can_agmo_mtlt305Package;
import com.bosch.nevonex.can_agmo_mtlt305.impl.CAN_AGMO_MTLT305Provider;
import com.bosch.nevonex.can_agmo_mtlt305.impl.CAN_AGMO_MTLT305;
import com.bosch.nevonex.can_agmo_steermotor.impl.Can_agmo_steermotorPackage;
import com.bosch.nevonex.can_agmo_steermotor.impl.CAN_AGMO_SteerMotorProvider;
import com.bosch.nevonex.can_agmo_steermotor.impl.CAN_AGMO_SteerMotor;
import com.bosch.nevonex.isopgn.impl.IsopgnPackage;
import com.bosch.nevonex.isopgn.impl.ISOPGNProvider;
import com.bosch.nevonex.isopgn.impl.ISOPGN;
import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_ext_gps_nmea0183Package;
import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_Ext_GPS_NMEA0183Provider;
import com.bosch.nevonex.serial_ext_gps_nmea0183.impl.Serial_Ext_GPS_NMEA0183;


public class SDKTest {
    private static TestFilClient filClient = TestFilClient.getInstance();
    private static Properties prop;
    private static Map<String, String> featureToAddressMap = new HashMap<>();
    private static CAN_AGMO_Customized_TractorProvider can_agmo_customized_tractorProvider;
    private static CAN_AGMO_MTLT305Provider can_agmo_mtlt305Provider;
    private static CAN_AGMO_SteerMotorProvider can_agmo_steermotorProvider;
    private static ISOPGNProvider isopgnProvider;
    private static Serial_Ext_GPS_NMEA0183Provider serial_ext_gps_nmea0183Provider;

    @BeforeClass
    public static void setUp() throws Exception {
        initialize();
        initializeDom();
        prop = readSimulatorPropertiesFile();
        initMaps();
    }

    private static void initialize() throws NevonexException {
        FCALRuntime runtime = new FCALRuntime();
        List<ProviderEnum> providerValues = ProviderEnum.VALUES;
        String[] providerarr = new String[providerValues.size()];
        int index = 0;
        for (ProviderEnum providerEnum : providerValues) {
            providerarr[index] = providerEnum.getName();
            index++;
        }
        runtime.startRuntime(providerarr, new String[0], new String[0]);
        runtime.initialize();
        runtime.startProviders();
        can_agmo_customized_tractorProvider = (CAN_AGMO_Customized_TractorProvider) runtime.getMachineProvider("CAN_AGMO_Customized_TractorProvider");
        can_agmo_mtlt305Provider = (CAN_AGMO_MTLT305Provider) runtime.getMachineProvider("CAN_AGMO_MTLT305Provider");
        can_agmo_steermotorProvider = (CAN_AGMO_SteerMotorProvider) runtime.getMachineProvider("CAN_AGMO_SteerMotorProvider");
        isopgnProvider = (ISOPGNProvider) runtime.getMachineProvider("ISOPGNProvider");
        serial_ext_gps_nmea0183Provider = (Serial_Ext_GPS_NMEA0183Provider) runtime.getMachineProvider("Serial_Ext_GPS_NMEA0183Provider");
        FCALLogs.getInstance().log.info("Runtime started ...");
    }

    private static void initializeDom() throws Exception {
        filClient.createDom(TestFilClient.TOPIC_CREATION, "./data/sample_data.xml");
        while (can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor() == null || can_agmo_mtlt305Provider.getCAN_AGMO_MTLT305() == null || can_agmo_steermotorProvider.getCAN_AGMO_SteerMotor() == null || isopgnProvider.getISOPGN() == null || serial_ext_gps_nmea0183Provider.getSerial_Ext_GPS_NMEA0183() == null) {
            Thread.sleep(5000);
        }
    }

    public static void initMaps() {
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.machineconnect.sub", "/1/+");
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.machinedata.sub", "/0/+");
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.receive_ACC_INFO.sub", "/9455");
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.receive_FNR_INFO.sub", "/9456");
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.receive_HYD_INFO.sub", "/9457");
        featureToAddressMap.put("CAN_AGMO_Customized_Tractor.receive_SFT_INFO.sub", "/9459");
        featureToAddressMap.put("CAN_AGMO_MTLT305.machineconnect.sub", "/1/+");
        featureToAddressMap.put("CAN_AGMO_MTLT305.machinedata.sub", "/0/+");
        featureToAddressMap.put("CAN_AGMO_MTLT305.aceinna_Accel.sub", "/9440");
        featureToAddressMap.put("CAN_AGMO_MTLT305.aceinna_AngleRate.sub", "/9441");
        featureToAddressMap.put("CAN_AGMO_MTLT305.aceinna_Angles.sub", "/9442");
        featureToAddressMap.put("CAN_AGMO_SteerMotor.machineconnect.sub", "/1/+");
        featureToAddressMap.put("CAN_AGMO_SteerMotor.machinedata.sub", "/0/+");
        featureToAddressMap.put("CAN_AGMO_SteerMotor.motor_Heartbeat.sub", "/9428");
        featureToAddressMap.put("CAN_AGMO_SteerMotor.motor_Response_Encoder_Speed.sub", "/9434");
        featureToAddressMap.put("CAN_AGMO_SteerMotor.motor_Response_EncoderCountValue.sub", "/9435");
        featureToAddressMap.put("ISOPGN.machineconnect.sub", "/1/+");
        featureToAddressMap.put("ISOPGN.machinedata.sub", "/0/+");
        featureToAddressMap.put("ISOPGN.hitchPosPerRe.sub", "/251");
        featureToAddressMap.put("Serial_Ext_GPS_NMEA0183.machineconnect.sub", "/1/+");
        featureToAddressMap.put("Serial_Ext_GPS_NMEA0183.machinedata.sub", "/0/+");
        featureToAddressMap.put("Serial_Ext_GPS_NMEA0183.serial_Ext_GPS_NMEA0183_Data.sub", "/9465");
    }

    @Test
    public void testMachineDomBuild() {
        CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor = (CAN_AGMO_Customized_Tractor) can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor();
        assertNotNull(cAN_AGMO_Customized_Tractor);
        CAN_AGMO_MTLT305 cAN_AGMO_MTLT305 = (CAN_AGMO_MTLT305) can_agmo_mtlt305Provider.getCAN_AGMO_MTLT305();
        assertNotNull(cAN_AGMO_MTLT305);
        CAN_AGMO_SteerMotor cAN_AGMO_SteerMotor = (CAN_AGMO_SteerMotor) can_agmo_steermotorProvider.getCAN_AGMO_SteerMotor();
        assertNotNull(cAN_AGMO_SteerMotor);
        ISOPGN iSOPGN = (ISOPGN) isopgnProvider.getISOPGN();
        assertNotNull(iSOPGN);
        Serial_Ext_GPS_NMEA0183 serial_Ext_GPS_NMEA0183 = (Serial_Ext_GPS_NMEA0183) serial_ext_gps_nmea0183Provider.getSerial_Ext_GPS_NMEA0183();
        assertNotNull(serial_Ext_GPS_NMEA0183);
    }

    @Test
    public void testSubscribeCAN_AGMO_Customized_TractorReceive_ACC_INFO() throws Exception {
        String address = "/9455";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R value = (com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R) getRandomValue(feature, prop);
        CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor_ = (CAN_AGMO_Customized_Tractor) can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor();
        cAN_AGMO_Customized_Tractor_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_Customized_Tractor" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_Customized_Tractor_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_Customized_Tractor_.eIsSet(feature));
        com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R testTemp = (com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R) cAN_AGMO_Customized_Tractor_.getReceive_ACC_INFO();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_Customized_TractorReceive_FNR_INFO() throws Exception {
        String address = "/9456";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R value = (com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R) getRandomValue(feature, prop);
        CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor_ = (CAN_AGMO_Customized_Tractor) can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor();
        cAN_AGMO_Customized_Tractor_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_Customized_Tractor" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_Customized_Tractor_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_Customized_Tractor_.eIsSet(feature));
        com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R testTemp = (com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R) cAN_AGMO_Customized_Tractor_.getReceive_FNR_INFO();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_Customized_TractorReceive_HYD_INFO() throws Exception {
        String address = "/9457";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R value = (com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R) getRandomValue(feature, prop);
        CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor_ = (CAN_AGMO_Customized_Tractor) can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor();
        cAN_AGMO_Customized_Tractor_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_Customized_Tractor" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_Customized_Tractor_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_Customized_Tractor_.eIsSet(feature));
        com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R testTemp = (com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R) cAN_AGMO_Customized_Tractor_.getReceive_HYD_INFO();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_Customized_TractorReceive_SFT_INFO() throws Exception {
        String address = "/9459";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R value = (com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R) getRandomValue(feature, prop);
        CAN_AGMO_Customized_Tractor cAN_AGMO_Customized_Tractor_ = (CAN_AGMO_Customized_Tractor) can_agmo_customized_tractorProvider.getCAN_AGMO_Customized_Tractor();
        cAN_AGMO_Customized_Tractor_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_Customized_Tractor" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_Customized_Tractor_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_Customized_Tractor_.eIsSet(feature));
        com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R testTemp = (com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R) cAN_AGMO_Customized_Tractor_.getReceive_SFT_INFO();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_MTLT305Aceinna_Accel() throws Exception {
        String address = "/9440";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R value = (com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R) getRandomValue(feature, prop);
        CAN_AGMO_MTLT305 cAN_AGMO_MTLT305_ = (CAN_AGMO_MTLT305) can_agmo_mtlt305Provider.getCAN_AGMO_MTLT305();
        cAN_AGMO_MTLT305_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_MTLT305" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_MTLT305_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_MTLT305_.eIsSet(feature));
        com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R testTemp = (com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R) cAN_AGMO_MTLT305_.getAceinna_Accel();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_MTLT305Aceinna_AngleRate() throws Exception {
        String address = "/9441";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R value = (com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R) getRandomValue(feature, prop);
        CAN_AGMO_MTLT305 cAN_AGMO_MTLT305_ = (CAN_AGMO_MTLT305) can_agmo_mtlt305Provider.getCAN_AGMO_MTLT305();
        cAN_AGMO_MTLT305_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_MTLT305" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_MTLT305_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_MTLT305_.eIsSet(feature));
        com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R testTemp = (com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R) cAN_AGMO_MTLT305_.getAceinna_AngleRate();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_MTLT305Aceinna_Angles() throws Exception {
        String address = "/9442";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R value = (com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R) getRandomValue(feature, prop);
        CAN_AGMO_MTLT305 cAN_AGMO_MTLT305_ = (CAN_AGMO_MTLT305) can_agmo_mtlt305Provider.getCAN_AGMO_MTLT305();
        cAN_AGMO_MTLT305_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_MTLT305" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_MTLT305_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_MTLT305_.eIsSet(feature));
        com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R testTemp = (com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R) cAN_AGMO_MTLT305_.getAceinna_Angles();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeCAN_AGMO_SteerMotorMotor_Heartbeat() throws Exception {
        String address = "/9428";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R value = (com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R) getRandomValue(feature, prop);
        CAN_AGMO_SteerMotor cAN_AGMO_SteerMotor_ = (CAN_AGMO_SteerMotor) can_agmo_steermotorProvider.getCAN_AGMO_SteerMotor();
        cAN_AGMO_SteerMotor_.eUnset(feature);
        filClient.publishValue("CAN_AGMO_SteerMotor" + address, value, "0", "0");
        int k = 0;
        while (!cAN_AGMO_SteerMotor_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(cAN_AGMO_SteerMotor_.eIsSet(feature));
        com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R testTemp = (com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R) cAN_AGMO_SteerMotor_.getMotor_Heartbeat();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    @Test
    public void testSubscribeSerial_Ext_GPS_NMEA0183Serial_Ext_GPS_NMEA0183_Data() throws Exception {
        String address = "/9465";
        EStructuralFeature feature = getFeatureByInterfaceAddress(address);
        com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R value = (com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R) getRandomValue(feature, prop);
        Serial_Ext_GPS_NMEA0183 serial_Ext_GPS_NMEA0183_ = (Serial_Ext_GPS_NMEA0183) serial_ext_gps_nmea0183Provider.getSerial_Ext_GPS_NMEA0183();
        serial_Ext_GPS_NMEA0183_.eUnset(feature);
        filClient.publishValue("Serial_Ext_GPS_NMEA0183" + address, value, "0", "0");
        int k = 0;
        while (!serial_Ext_GPS_NMEA0183_.eIsSet(feature)) {
            Thread.sleep(250);
            if (++k == 20) break;
        }
        assertTrue(serial_Ext_GPS_NMEA0183_.eIsSet(feature));
        com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R testTemp = (com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R) serial_Ext_GPS_NMEA0183_.getSerial_Ext_GPS_NMEA0183_Data();
        Object[] expected = value.getArrayValues();
        Object[] actual = testTemp.getArrayValues();
        for (int i = 0; i < expected.length; i++) {
            if (expected[i].getClass().isArray() && actual[i].getClass().isArray()) {
                if (expected[i] instanceof int[] && actual[i] instanceof int[]) {
                    org.junit.Assert.assertArrayEquals((int[]) expected[i], (int[]) actual[i]);
                } else if (expected[i] instanceof float[] && actual[i] instanceof float[]) {
                    org.junit.Assert.assertArrayEquals((float[]) expected[i], (float[]) actual[i], 0.001f);
                } else if (expected[i] instanceof double[] && actual[i] instanceof double[]) {
                    org.junit.Assert.assertArrayEquals((double[]) expected[i], (double[]) actual[i], 0.001);
                } else if (expected[i] instanceof long[] && actual[i] instanceof long[]) {
                    org.junit.Assert.assertArrayEquals((long[]) expected[i], (long[]) actual[i]);
                } else if (expected[i] instanceof boolean[] && actual[i] instanceof boolean[]) {
                    org.junit.Assert.assertArrayEquals((boolean[]) expected[i], (boolean[]) actual[i]);
                } else {
                    org.junit.Assert.assertArrayEquals((Object[]) expected[i], (Object[]) actual[i]);
                }
            } else {
                assertEquals(expected[i], actual[i]);
            }
        }
    }

    public static EClass getEClassByName(String name) {
        switch (name) {
        case "can_agmo_customized_tractor":
            return (EClass) Can_agmo_customized_tractorPackage.eINSTANCE.getEClassifier("ICAN_AGMO_Customized_Tractor");
        case "receive_acc_info_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("RECEIVE_ACC_INFO_T_R");
        case "receive_fnr_info_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("RECEIVE_FNR_INFO_T_R");
        case "receive_hyd_info_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("RECEIVE_HYD_INFO_T_R");
        case "receive_sft_info_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("RECEIVE_SFT_INFO_T_R");
        case "can_agmo_mtlt305":
            return (EClass) Can_agmo_mtlt305Package.eINSTANCE.getEClassifier("ICAN_AGMO_MTLT305");
        case "aceinna_accel_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("ACEINNA_ACCEL_T_R");
        case "aceinna_anglerate_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("ACEINNA_ANGLERATE_T_R");
        case "aceinna_angles_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("ACEINNA_ANGLES_T_R");
        case "can_agmo_steermotor":
            return (EClass) Can_agmo_steermotorPackage.eINSTANCE.getEClassifier("ICAN_AGMO_SteerMotor");
        case "motor_heartbeat_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("MOTOR_HEARTBEAT_T_R");
        case "motor_response_encoder_speed":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("Motor_Response_Encoder_Speed");
        case "motor_response_encodercountvalue":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("Motor_Response_EncoderCountValue");
        case "isopgn":
            return (EClass) IsopgnPackage.eINSTANCE.getEClassifier("IISOPGN");
        case "hitchposperre":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("HitchPosPerRe");
        case "serial_ext_gps_nmea0183":
            return (EClass) Serial_ext_gps_nmea0183Package.eINSTANCE.getEClassifier("ISerial_Ext_GPS_NMEA0183");
        case "gps_nmea0183_t_r":
            return (EClass) FcalPackage.eINSTANCE.getEClassifier("GPS_NMEA0183_T_R");
        case "fcalcontroller":
            return FcbPackage.eINSTANCE.getFCALController();
        }
        return null;
    }


    public static EStructuralFeature getFeatureByInterfaceAddress(String address) {
        for (String childKey : featureToAddressMap.keySet()) {
            if (childKey.endsWith(".sub")) {
                if (address.equals(featureToAddressMap.get(childKey))) {
                    String name = childKey.split("\\.")[0];
                    String feature = childKey.split("\\.")[1];
                    EClass eClass = getEClassByName(name.toLowerCase());
                    if (eClass != null && eClass.getEStructuralFeature(feature) != null) {
                        return eClass.getEStructuralFeature(feature);
                    }
                }
            }
        }
        return null;
    }

    public static Object getRandomValue(EStructuralFeature att, Properties prop) {
        Random rd = new Random();
        if (prop == null) {
            throw new IllegalArgumentException("Exception while reading the Simulator.properties file");
        }
        if (att != null) {
            if (att.getEType().getName().equalsIgnoreCase("EFLOAT")) {
                float min = (float) getMinMaxRange(prop, "float", 0);
                float max = (float) getMinMaxRange(prop, "float", 1);
                if (min >= max) throw new IllegalArgumentException("bound must be greater than origin");
                return (rd.nextFloat() * (max - min)) + min;
            } else if (att.getEType().getName().equalsIgnoreCase("EDOUBLE")) {
                double min = (double) getMinMaxRange(prop, "double", 0);
                double max = (double) getMinMaxRange(prop, "double", 1);
                return ThreadLocalRandom.current().nextDouble(min, max);
            } else if (att.getEType().getName().equalsIgnoreCase("EINT")) {
                int min = (int) getMinMaxRange(prop, "int", 0);
                int max = (int) getMinMaxRange(prop, "int", 1);
                return ThreadLocalRandom.current().nextInt(min, max);
            } else if (att.getEType().getName().equalsIgnoreCase("ELONG")) {
                long min = (long) getMinMaxRange(prop, "long", 0);
                long max = (long) getMinMaxRange(prop, "long", 1);
                return ThreadLocalRandom.current().nextLong(min, max);
            } else if (att.getEType().getName().equalsIgnoreCase("ESTRING")) {
                String range = prop.getProperty("string");
                int length = 7;
                if (range != null && !range.isEmpty()) length = Integer.parseInt(range.trim());
                StringBuilder buffer = new StringBuilder(length);
                for (int i = 0; i < length; i++) {
                    int c = 97 + (int) (rd.nextFloat() * (122 - 97 + 1));
                    buffer.append((char) c);
                }
                return buffer.toString();
            } else if (att.getEType().getName().equalsIgnoreCase("EBOOLEAN")) {
                return 1;
            } else if (att.getEType().getName().equalsIgnoreCase("INTARRAY")) {
                int min = (int) getMinMaxRange(prop, "int", 0);
                int max = (int) getMinMaxRange(prop, "int", 1);
                return new int[]{ThreadLocalRandom.current().nextInt(min, max), ThreadLocalRandom.current().nextInt(min, max), ThreadLocalRandom.current().nextInt(min, max)};
            } else if (att.getEType().getName().equalsIgnoreCase("DOUBLEARRAY")) {
                double min = (double) getMinMaxRange(prop, "double", 0);
                double max = (double) getMinMaxRange(prop, "double", 1);
                return new double[]{ThreadLocalRandom.current().nextDouble(min, max), ThreadLocalRandom.current().nextDouble(min, max), ThreadLocalRandom.current().nextDouble(min, max)};
            } else if (att.getEType().getName().equalsIgnoreCase("FLOATARRAY")) {
                float min = (float) getMinMaxRange(prop, "float", 0);
                float max = (float) getMinMaxRange(prop, "float", 1);
                if (min >= max) throw new IllegalArgumentException("bound must be greater than origin");
                return new float[]{(rd.nextFloat() * (max - min)) + min, (rd.nextFloat() * (max - min)) + min, (rd.nextFloat() * (max - min)) + min};
            } else if (att.getEType().getName().equalsIgnoreCase("LONGARRAY")) {
                long min = (long) getMinMaxRange(prop, "long", 0);
                long max = (long) getMinMaxRange(prop, "long", 1);
                return new long[]{ThreadLocalRandom.current().nextLong(min, max), ThreadLocalRandom.current().nextLong(min, max), ThreadLocalRandom.current().nextLong(min, max)};
            } else if (att.getEType().getName().equalsIgnoreCase("BOOLEANARRAY")) {
                return new int[]{1, 0, 1};
            } else if (att.getEType().getName().equalsIgnoreCase("STRINGARRAY")) {
                String range = prop.getProperty("string");
                int length = 7;
                if (range != null && !range.isEmpty()) length = Integer.parseInt(range.trim());
                StringBuilder buffer = new StringBuilder(length);
                for (int i = 0; i < length; i++) {
                    int c = 97 + (int) (rd.nextFloat() * (122 - 97 + 1));
                    buffer.append((char) c);
                }
                return new String[]{buffer.toString()};
            } else if (att.getEType().getName().equalsIgnoreCase("AbsolutePosition")) {
                IAbsolutePosition position = CommonFactory.eINSTANCE.createAbsolutePosition();
                position.setLatitude(49 + rd.nextFloat());
                position.setLongitude(9 + rd.nextFloat());
                return position;
            } else if (Enumerator.class.isAssignableFrom(att.getEType().getInstanceClass())) {
                try {
                    Object object = att.getEType().getInstanceClass().getField("VALUES").get(att.getEType().eClass().getClass());
                    if ((object instanceof List) && !((List) object).isEmpty()) {
                        Object enumLiteral = ((List) object).get(0);
                        if (((List) object).size() > 1) enumLiteral = ((List) object).get(1);
                        if (enumLiteral instanceof Enumerator) return ((Enumerator) enumLiteral).getValue();
                    }
                } catch (Exception e) {
                    FCALLogs.getInstance().log.info("Exception while getting the enum value for the interface " + att.getName(), e);
                }
            } else if (com.bosch.nevonex.types.IArrayType.class.isAssignableFrom(att.getEType().getInstanceClass())) {
                org.eclipse.emf.ecore.EObject eObject = att.getEType().getEPackage().getEFactoryInstance().create((EClass) att.getEType());
                String[] featureNames = ((com.bosch.nevonex.types.IArrayType) eObject).getFeatureNames();
                for (String featureName : featureNames) {
                    try {
                        EStructuralFeature feature = eObject.eClass().getEStructuralFeature(featureName);
                        Object value = getRandomValue(feature, prop);
                        if (Enumerator.class.isAssignableFrom(feature.getEType().getInstanceClass())) {
                            value = feature.getEType().getInstanceClass().getMethod("get", int.class)
                                .invoke(feature.getEType().eClass().getClass(), Integer.valueOf(value.toString()));
                        } else if (feature.getEType().getName().equalsIgnoreCase("EBoolean") && value instanceof Integer) {
                            value = (((Integer) value).intValue() == 1) ? true : false;
                        } else if (feature.getEType().getInstanceClassName().equalsIgnoreCase("boolean[]")) {
                            int[] intArray = (int[]) value;
                            boolean[] booleanArray = new boolean[intArray.length];
                            for (int i = 0; i < booleanArray.length; i++) {
                                booleanArray[i] = (intArray[i] == 1) ? true : false;
                            }
                            value = booleanArray;
                        }
                        eObject.eSet(feature, value);
                    } catch (Exception e) {
                        FCALLogs.getInstance().log.info("Exception while setting the array feature " + featureName, e);
                    }
                }
                return eObject;
            }
        }
        return null;
    }

    public static Properties readSimulatorPropertiesFile() {
        Properties prop = null;
        try {
            FileInputStream fis = new FileInputStream("Simulator.properties");
            prop = new Properties();
            prop.load(fis);
            fis.close();
        } catch (IOException e1) {
            FCALLogs.getInstance().log.info("Exception while reading the Simulator.properties file");
        }
        return prop;
    }

    private static Object getMinMaxRange(Properties prop, String datatype, int index) {
        if (prop != null) {
            String range = prop.getProperty(datatype);
            if (range != null && !range.isEmpty()) {
                String[] values = range.split(",");
                if (values[index] != null) {
                    switch (datatype) {
                    case "int": return Integer.parseInt(values[index].trim());
                    case "long": return Long.parseLong(values[index].trim());
                    case "float": return Float.parseFloat(values[index].trim());
                    case "double": return Double.parseDouble(values[index].trim());
                    }
                }
            }
        }
        return Math.random();
    }

}