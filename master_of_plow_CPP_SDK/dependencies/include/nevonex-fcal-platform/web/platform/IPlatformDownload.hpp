/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_DOWNLOAD_HPP
#define NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_DOWNLOAD_HPP

#include "../../resource/FileProvider.hpp"
#include "../../resource/FilePath.hpp"

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
class IPlatformDownload
{
public:
	virtual ~IPlatformDownload() = default;
protected:
	IPlatformDownload() = default;
protected:
	virtual const std::string getDownloadDirectory() = 0;
	::nevonex::resource::FilePath importFile(const ::nevonex::resource::FilePath & sourcePath);
};

} // platform
} // web
} // nevonex

#endif /* NEVONEX_FCAL_PLATFORM__WEB_PLATFORM_DOWNLOAD_HPP */
