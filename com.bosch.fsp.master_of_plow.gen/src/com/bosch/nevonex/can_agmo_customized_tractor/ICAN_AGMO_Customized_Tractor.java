/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_customized_tractor;

import com.bosch.fsp.runtime.feature.exception.CommunicationException;
import com.bosch.fsp.runtime.feature.exception.InterfaceDisabledException;
import com.bosch.fsp.runtime.feature.exception.MachineWriteException;
import com.bosch.nevonex.fcal.IAGMO_GENERIC_SEND_CMD_T_W;
import com.bosch.nevonex.fcal.IRECEIVE_ACC_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_FNR_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_HYD_INFO_T_R;
import com.bosch.nevonex.fcal.IRECEIVE_SFT_INFO_T_R;
import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public interface ICAN_AGMO_Customized_Tractor extends EObject {
	IRECEIVE_ACC_INFO_T_R getReceive_ACC_INFO();

	void setReceive_ACC_INFO(IRECEIVE_ACC_INFO_T_R value);
	IRECEIVE_FNR_INFO_T_R getReceive_FNR_INFO();

	void setReceive_FNR_INFO(IRECEIVE_FNR_INFO_T_R value);
	IRECEIVE_HYD_INFO_T_R getReceive_HYD_INFO();

	void setReceive_HYD_INFO(IRECEIVE_HYD_INFO_T_R value);
	IRECEIVE_SFT_INFO_T_R getReceive_SFT_INFO();

	void setReceive_SFT_INFO(IRECEIVE_SFT_INFO_T_R value);
	IAGMO_GENERIC_SEND_CMD_T_W getSend_ACC_CMD();

	void setSend_ACC_CMD(IAGMO_GENERIC_SEND_CMD_T_W value) throws MachineWriteException, CommunicationException, InterfaceDisabledException;
	IAGMO_GENERIC_SEND_CMD_T_W getSend_FNR_CMD();

	void setSend_FNR_CMD(IAGMO_GENERIC_SEND_CMD_T_W value) throws MachineWriteException, CommunicationException, InterfaceDisabledException;
	IAGMO_GENERIC_SEND_CMD_T_W getSend_HYD_CMD();

	void setSend_HYD_CMD(IAGMO_GENERIC_SEND_CMD_T_W value) throws MachineWriteException, CommunicationException, InterfaceDisabledException;
	IAGMO_GENERIC_SEND_CMD_T_W getSend_SFT_CMD();

	void setSend_SFT_CMD(IAGMO_GENERIC_SEND_CMD_T_W value) throws MachineWriteException, CommunicationException, InterfaceDisabledException;
} // ICAN_AGMO_Customized_Tractor
