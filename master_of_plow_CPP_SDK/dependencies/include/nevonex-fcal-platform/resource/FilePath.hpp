/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_HPP
#define NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_HPP

#include <string>
#include <boost/filesystem/path.hpp>

namespace nevonex
{
namespace resource
{

/**
 * Encapsulates a boost::filesystem::path.
 *
 * @see https://www.boost.org/doc/libs/1_73_0/libs/filesystem/doc/reference.html#class-path for more details.
 */

class FilePath
{
public:
	virtual ~FilePath() = default;
	FilePath(const std::string &filePath); // NOSONAR - cpp:S1709 Since changing this would require changes to be done in nevonex-sdk, not critical issue TODO: fileName -> enforce Resource Folder.
	FilePath(const boost::filesystem::path &path);  // NOSONAR - cpp:S1709 Since changing this would require changes to be done in nevonex-sdk, not critical issue

	FilePath persist(bool overwrite = false) const;

	bool remove();

	const boost::filesystem::path& get() const;

	FilePath copyFile(const boost::filesystem::path &destPath,
			bool overwrite) const;

	FilePath copyFile(const boost::filesystem::path &destPath,
			const std::string & destFileName, bool overwrite) const;

protected:
	boost::filesystem::path m_filePath;
};

} // resource

} // nevonex

#endif /* NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_HPP */
