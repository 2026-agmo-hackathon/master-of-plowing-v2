/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.fcal.util;

import com.bosch.nevonex.fcal.IBulkProcessor;
import com.bosch.nevonex.fcal.IFcalFactory;
import com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R;
import com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W;
import com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R;
import com.bosch.nevonex.fcal.IMOTOR_HEARTBEAT_T_R;
import com.bosch.nevonex.fcal.IRAW_CAN_DATA_DLC_8_W;
import com.bosch.nevonex.fcal.IGPS_NMEA0183_T_R;

import com.bosch.nevonex.fcal.impl.FcalPackage;

import org.eclipse.emf.common.notify.Adapter;
import org.eclipse.emf.common.notify.Notifier;

import org.eclipse.emf.common.notify.impl.AdapterFactoryImpl;

import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public class FcalAdapterFactory extends AdapterFactoryImpl {
	protected static FcalPackage modelPackage;

	public FcalAdapterFactory() {
		if (modelPackage == null) {
			modelPackage = FcalPackage.eINSTANCE;
		}
	}

	@Override
	public boolean isFactoryForType(Object object) {
		if (object == modelPackage) {
			return true;
		}
		if (object instanceof EObject) {
			return ((EObject) object).eClass().getEPackage() == modelPackage;
		}
		return false;
	}

	protected FcalSwitch<Adapter> modelSwitch = new FcalSwitch<Adapter>() {
		@Override
		public Adapter caseIBulkProcessor(IBulkProcessor object) {
			return createIBulkProcessorAdapter();
		}

		@Override
		public Adapter caseBulkProcessor(IBulkProcessor object) {
			return createBulkProcessorAdapter();
		}

		@Override
		public Adapter caseIFcalFactory(IFcalFactory object) {
			return createIFcalFactoryAdapter();
		}

		@Override
		public Adapter caseRECEIVE_ACC_INFO_T_R(IRECEIVE_ACC_INFO_T_R object) {
			return createRECEIVE_ACC_INFO_T_RAdapter();
		}

		@Override
		public Adapter caseRECEIVE_FNR_INFO_T_R(IRECEIVE_FNR_INFO_T_R object) {
			return createRECEIVE_FNR_INFO_T_RAdapter();
		}

		@Override
		public Adapter caseRECEIVE_HYD_INFO_T_R(IRECEIVE_HYD_INFO_T_R object) {
			return createRECEIVE_HYD_INFO_T_RAdapter();
		}

		@Override
		public Adapter caseRECEIVE_SFT_INFO_T_R(IRECEIVE_SFT_INFO_T_R object) {
			return createRECEIVE_SFT_INFO_T_RAdapter();
		}

		@Override
		public Adapter caseAGMO_GENERIC_SEND_CMD_T_W(IAGMO_GENERIC_SEND_CMD_T_W object) {
			return createAGMO_GENERIC_SEND_CMD_T_WAdapter();
		}

		@Override
		public Adapter caseACEINNA_ACCEL_T_R(IACEINNA_ACCEL_T_R object) {
			return createACEINNA_ACCEL_T_RAdapter();
		}

		@Override
		public Adapter caseACEINNA_ANGLERATE_T_R(IACEINNA_ANGLERATE_T_R object) {
			return createACEINNA_ANGLERATE_T_RAdapter();
		}

		@Override
		public Adapter caseACEINNA_ANGLES_T_R(IACEINNA_ANGLES_T_R object) {
			return createACEINNA_ANGLES_T_RAdapter();
		}

		@Override
		public Adapter caseMOTOR_HEARTBEAT_T_R(IMOTOR_HEARTBEAT_T_R object) {
			return createMOTOR_HEARTBEAT_T_RAdapter();
		}

		@Override
		public Adapter caseRAW_CAN_DATA_DLC_8_W(IRAW_CAN_DATA_DLC_8_W object) {
			return createRAW_CAN_DATA_DLC_8_WAdapter();
		}

		@Override
		public Adapter caseGPS_NMEA0183_T_R(IGPS_NMEA0183_T_R object) {
			return createGPS_NMEA0183_T_RAdapter();
		}

		@Override
		public Adapter defaultCase(EObject object) {
			return createEObjectAdapter();
		}
	};

	@Override
	public Adapter createAdapter(Notifier target) {
		return modelSwitch.doSwitch((EObject) target);
	}

	public Adapter createIBulkProcessorAdapter() {
		return null;
	}

	public Adapter createBulkProcessorAdapter() {
		return null;
	}

	public Adapter createIFcalFactoryAdapter() {
		return null;
	}

	public Adapter createRECEIVE_ACC_INFO_T_RAdapter() {
		return null;
	}

	public Adapter createRECEIVE_FNR_INFO_T_RAdapter() {
		return null;
	}

	public Adapter createRECEIVE_HYD_INFO_T_RAdapter() {
		return null;
	}

	public Adapter createRECEIVE_SFT_INFO_T_RAdapter() {
		return null;
	}

	public Adapter createAGMO_GENERIC_SEND_CMD_T_WAdapter() {
		return null;
	}

	public Adapter createACEINNA_ACCEL_T_RAdapter() {
		return null;
	}

	public Adapter createACEINNA_ANGLERATE_T_RAdapter() {
		return null;
	}

	public Adapter createACEINNA_ANGLES_T_RAdapter() {
		return null;
	}

	public Adapter createMOTOR_HEARTBEAT_T_RAdapter() {
		return null;
	}

	public Adapter createRAW_CAN_DATA_DLC_8_WAdapter() {
		return null;
	}

	public Adapter createGPS_NMEA0183_T_RAdapter() {
		return null;
	}

	public Adapter createEObjectAdapter() {
		return null;
	}

} //FcalAdapterFactory
