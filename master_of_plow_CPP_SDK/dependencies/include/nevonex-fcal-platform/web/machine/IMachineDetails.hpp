/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_MACHINE_IMACHINEDETAILS_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_MACHINE_IMACHINEDETAILS_HPP_

#include "../../web/platform/IPlatform.hpp"

#include <string>
#include <rapidjson/document.h>

namespace nevonex
{
namespace web
{
namespace machine
{
/**
 * INTERNAL
 */
class IMachineDetails: public ::nevonex::web::platform::IPlatform
{
public:
	IMachineDetails();
	virtual ~IMachineDetails() = default;
protected:
	std::string uploadMachineDetails(const rapidjson::Document &machineDetails);

private:
	std::string machineDetailsUrl;
};

} /* namespace machine */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_MACHINE_IMACHINEDETAILS_HPP_ */
