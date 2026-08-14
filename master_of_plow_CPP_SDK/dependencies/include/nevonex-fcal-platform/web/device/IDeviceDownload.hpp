/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__WEB_DEVICE_IDEVICE_DOWNLOAD_HPP_
#define NEVONEX_FCAL_PLATFORM__WEB_DEVICE_IDEVICE_DOWNLOAD_HPP_

#include "../../resource/FilePath.hpp"

#include "../../web/platform/IPlatformDownload.hpp"

namespace nevonex
{
namespace web
{
namespace device
{
/**
 * INTERNAL
 */
class IDeviceDownload : public ::nevonex::web::platform::IPlatformDownload
{
public:
	virtual ~IDeviceDownload() = default;
protected:
	IDeviceDownload() = default;

protected:
	// TODO: change to boost::filepath
	virtual const std::string getDownloadDirectory() override;
};

} /* namespace device */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM__WEB_DEVICE_IDEVICE_DOWNLOAD_HPP_ */
