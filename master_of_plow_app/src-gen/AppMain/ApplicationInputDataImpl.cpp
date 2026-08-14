/*
 * AppMain/ApplicationInputDataImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ApplicationInputData.hpp"
#include <stdexcept>

#include <nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.hpp>
#include <nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.hpp>
#include <nevonex/can_agmo_steermotor/CAN_AGMO_SteerMotorProvider.hpp>
#include <nevonex/isopgn/ISOPGNProvider.hpp>
#include <nevonex/serial_ext_gps_nmea0183/Serial_Ext_GPS_NMEA0183Provider.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecorecpp/mapping.hpp>

#include <nevonex-fcal-platform/log/Logger.hpp>

/*PROTECTED REGION ID(ApplicationInputDataImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::AppMain;
using namespace ::nevonex::log;

/*PROTECTED REGION ID(ApplicationInputDataImpl_Methods) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
void ApplicationInputData::_initialize()
{
    // Supertypes

    // References

    /*PROTECTED REGION ID(ApplicationInputDataImpl__initialize) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
}

// Operations from Parent(s)

// Operations

/*PROTECTED REGION ID(ApplicationInputDataImpl_MethodsEnd) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
