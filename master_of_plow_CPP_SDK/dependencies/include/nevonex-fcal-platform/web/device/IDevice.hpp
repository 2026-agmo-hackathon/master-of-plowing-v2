/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_DEVICE_IDEVICE_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_DEVICE_IDEVICE_HPP_

#include "../../resource/FilePath.hpp"
#include "../../web/platform/IPlatform.hpp"

namespace nevonex
{
namespace device
{
/**
 * INTERNAL
 */
class IDevice: public ::nevonex::web::platform::IPlatform
{
public:
	virtual ~IDevice() = default;
protected:
	IDevice();

protected:
	std::string i_SendCommand(const std::string &command) const;
	std::string i_SendFile(const ::nevonex::resource::FilePath &filePath) const;
	bool i_IsDeviceConnected() const;

private:
	std::string deviceUploadCommandUrl;
	std::string deviceUploadFileUrl;
	std::string deviceIsConnectedUrl;

	boost::filesystem::path destPath;
};

} /* namespace device */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_DEVICE_IDEVICE_HPP_ */
