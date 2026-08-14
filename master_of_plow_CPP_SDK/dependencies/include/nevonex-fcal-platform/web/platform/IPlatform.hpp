/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_HPP
#define NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_HPP

#include <string>

namespace nevonex
{
namespace web
{
namespace platform
{
/**
 * INTERNAL
 */
class IPlatform
{
public:
	static const std::string CLOUD_DIR;
	static const std::string DEVICE_DIR;
protected:
	static const std::string tagFeatureId;
	static const std::string tagTimestamp;
	static const std::string tagData;
	static const std::string tagCommand;
	static const std::string tagFilePath;
	static const std::string tagPriority;
	static const std::string tagConnectionType;
};

} // platform
} // web
} // nevonex

#endif /* NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_HPP */
