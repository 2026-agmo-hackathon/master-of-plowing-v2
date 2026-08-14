/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_PROVIDER_HPP
#define NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_PROVIDER_HPP

#include "../resource/FilePath.hpp"
#include "../resource/usb/UsbContent.hpp"

namespace nevonex
{
namespace resource
{

/**
 * Enums which will be used for retrieve all download API:
 *  - Enum values : Cloud / Device2Device
 */
enum class ResourceType { CLOUD = 0, D2D = 1 };

/**
 * Utilities for the following functionalities:
 *  - Access FileSystem
 *  - Persist files on feature re-start
 *  - Get files from Cloud / Device2Device.
 */
class FileProvider final
{
public:
	static FileProvider& getInstance()
	{
		static FileProvider instance;
		return instance;
	}

private:
	FileProvider() = default;
public:
	~FileProvider() = default;
	FileProvider(FileProvider const&) = delete;
	void operator=(FileProvider const&) = delete;
	std::uintmax_t getFolderSizeInBytes(const std::string &folderPath) const;

	// DISK API:
	/**
	 * Saves file to the resource folder mounted to the device which is available even if the feature is restarted.
	 * @param sourceFile The file which needs to be saved to the resource folder.
	 * @param overwrite  If true existing file will be overwritten,false otherwise.
	 * @return true if the file is successfully saved to the resources folder,false otherwise.
	 */

	void saveToDisk(const FilePath &sourceFile, bool overwrite = false) const;

	/**
	 * Retrieves file which is stored in the resources folder
	 *
	 * @param fileName name of the file to be retrieved from the resources folder.
	 * @return returns the file if it is present.
	 */
	FilePath retrieveFileFromDisk(const std::string &fileName) const;

	/**
	 * Deletes the file from the resources folder.
	 *
	 * @see FilePath.remove();
	 *
	 * @param fileName name of the file to be deleted from the resources folder
	 * @return true if the file is deleted,false otherwise.
	 */
	bool deleteFileFromDisk(const std::string & fileName) const;

	/**
	 * Returns the list of files present in the resources folder
	 *
	 * @return returns the list of files and folders present in the resources folder
	*/
	std::vector<FilePath> retrieveAllFilesFromDisk() const;

	// DOWNLOAD API:
	/**
	 * Retrieves file which is downloaded to the feature.
	 *
	 * @param resourceType CLOUD / D2D file
	 * @param fileName name of the file to be retrieved.
	 * @return returns the file path if it is present.
	 */
	FilePath retriveDownloadedFile(const ResourceType & resourceType, const std::string & fileName) const;

	/**
	 * Returns the list of files downloaded to the feature.
	 *
	 * @param resourceType CLOUD / D2D file
	 * @return list of downloaded files
	 */
	std::vector<FilePath> retrieveAllDownloadedFiles(const ResourceType & resourceType) const;

	// FIL Usage:
	/**
	 * Returns absolute path of the resources folder mounted to the device. This folder is accessible outside the docker
	 * container even the feature is stopped.
	 *
	 * @return Resources folder path.
	 */
	FilePath getResourcesFolderPath() const;

	/**
	 * Returns absolute path of the incoming folder mounted to the device.
	 * This folder contains all the downloaded files to the feature.
	 * @return Incoming folder path.
	 */
	FilePath getIncomingFolderPath() const;

	/**
	 * Returns absolute path of the outgoing folder mounted to the device.
	 * This folder contains all the files which will be uploaded to Cloud/Device.
	 *
	 * @return Outgoing folder path.
	 */
	FilePath getOutgoingFolderPath() const;

	/**
	 * Returns list of the files from the usb.
	 *
	 * @throws std::exception if any internal errors.
	 *
	 * @return List of Files.
	 */
	std::unique_ptr<::nevonex::resource::usb::UsbContent> getListOfFilesFromUSB(const std::string &path = "/") const;
private:
	void retrieveAllFiles(std::vector<FilePath> &files, const boost::filesystem::path &path) const;

	static const std::string GET_FILES_SD_URL;
};

} // resource

} // nevonex

#endif // NEVONEX_FCAL_PLATFORM__RESOURCE_FILE_PROVIDER_HPP
