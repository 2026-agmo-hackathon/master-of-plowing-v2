/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_DOWNLOAD_HPP_
#define NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_DOWNLOAD_HPP_

#include "../../resource/FilePath.hpp"
#include "../../web/platform/IPlatformDownload.hpp"

namespace nevonex
{
namespace web
{
namespace cloud
{
/**
 * INTERNAL
 */
class ICloudDownload : public ::nevonex::web::platform::IPlatformDownload
{
public:
	virtual ~ICloudDownload() = default;
protected:
	ICloudDownload() = default;

protected:
	// TODO: change to boost::filepath
	virtual const std::string getDownloadDirectory() override;
};

} /* namespace cloud */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_DOWNLOAD_HPP_ */
