/*
 * nevonex/fcal.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_HPP
#define NEVONEX_FCAL_HPP

#include "fcal_forward.hpp"
#include <boost/intrusive_ptr.hpp>
#include <ecorecpp/mapping_forward.hpp>

// EPackage

// EClass
#include "fcal/BulkProcessor.hpp"
#include "fcal/RECEIVE_ACC_INFO_T_R.hpp"
#include "fcal/RECEIVE_FNR_INFO_T_R.hpp"
#include "fcal/RECEIVE_HYD_INFO_T_R.hpp"
#include "fcal/RECEIVE_SFT_INFO_T_R.hpp"
#include "fcal/AGMO_GENERIC_SEND_CMD_T_W.hpp"
#include "fcal/ACEINNA_ACCEL_T_R.hpp"
#include "fcal/ACEINNA_ANGLERATE_T_R.hpp"
#include "fcal/ACEINNA_ANGLES_T_R.hpp"
#include "fcal/MOTOR_HEARTBEAT_T_R.hpp"
#include "fcal/RAW_CAN_DATA_DLC_8_W.hpp"
#include "fcal/GPS_NMEA0183_T_R.hpp"

// EEnum

// Package & Factory
#include "nevonex/fcal/FcalPackage.hpp"
#include "nevonex/fcal/FcalFactory.hpp"

#endif // NEVONEX_FCAL_HPP
