/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_DEVICEINFOPROVIDER_DEVICE_INFO_PROVIDER_HPP
#define NEVONEX_FCAL_PLATFORM_DEVICEINFOPROVIDER_DEVICE_INFO_PROVIDER_HPP

#include <string>
#include <mutex>

namespace nevonex
{
namespace deviceinfoprovider
{

class DeviceInfoProvider final
{
public:
	static DeviceInfoProvider& getInstance();
	~DeviceInfoProvider() = default;
	DeviceInfoProvider(DeviceInfoProvider const&) = delete;
	void operator=(DeviceInfoProvider const&) = delete;

	const std::string& getDeviceSerialNumber() const;

private:
	DeviceInfoProvider() = default;

	void load() const;
	void loadDeviceSerialNumber() const;
	void ensureLoaded() const;

	mutable std::mutex mutex;
	mutable bool loaded = false;
	mutable std::string deviceSerialNumber = "UNKNOWN";
};

} // deviceinfoprovider
} // nevonex

#endif // NEVONEX_FCAL_PLATFORM_DEVICEINFOPROVIDER_DEVICE_INFO_PROVIDER_HPP
