/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_HPP_
#define NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_HPP_

#include "../../resource/FilePath.hpp"
#include "../../web/platform/IPlatform.hpp"

namespace nevonex
{
namespace cloud
{
/**
 * INTERNAL
 */
class ICloud: public ::nevonex::web::platform::IPlatform
{
public:
	virtual ~ICloud() = default;
protected:
	ICloud();

protected:
	std::string i_uploadData(const std::string &data, boost::uint16_t priority,
			const std::string &connectionType) const;
	std::string i_uploadFile(const ::nevonex::resource::FilePath &fileToUpload,
			boost::uint16_t priority, const std::string &connectionType) const;
	std::string i_uploadFile(const ::nevonex::resource::FilePath &fileToUpload, const std::string & destFileName,
				boost::uint16_t priority, const std::string &connectionType) const;
private:
	std::string cloudUploadUrl;
	boost::filesystem::path destPath;
};

} /* namespace cloud */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM__WEB_CLOUD_ICLOUD_HPP_ */
