/**
Copyright (c) Robert Bosch GmbH. All rights reserved.
*/
package com.bosch.nevonex.can_agmo_mtlt305;

import com.bosch.nevonex.fcal.IACEINNA_ACCEL_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLERATE_T_R;
import com.bosch.nevonex.fcal.IACEINNA_ANGLES_T_R;
import org.eclipse.emf.ecore.EObject;

/**
 * @generated
 */
public interface ICAN_AGMO_MTLT305 extends EObject {
	IACEINNA_ACCEL_T_R getAceinna_Accel();

	void setAceinna_Accel(IACEINNA_ACCEL_T_R value);
	IACEINNA_ANGLERATE_T_R getAceinna_AngleRate();

	void setAceinna_AngleRate(IACEINNA_ANGLERATE_T_R value);
	IACEINNA_ANGLES_T_R getAceinna_Angles();

	void setAceinna_Angles(IACEINNA_ANGLES_T_R value);
} // ICAN_AGMO_MTLT305
