/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SOURCE_RESOURCE_USB_USBCONTENT_HPP_
#define SOURCE_RESOURCE_USB_USBCONTENT_HPP_

#include <string>
#include <vector>
#include <memory>

#include <rapidjson/document.h>
#include <boost/filesystem.hpp>

namespace nevonex
{
namespace resource
{

class FileProvider;

namespace usb
{

/**
 * Enums which will be used for checking the type of content:
 *  - Enum values : FILE / DIRECTORY
 */
enum class ContentType { FILE = 0, DIRECTORY = 1 };
/**
 * UsbContent
 * Stores all the USB related informations
 * Usage :
 * Sample Example
 * using namespace ::nevonex::resource;
 * usb::UsbContent usbContent = FileProvider::getInstance().getListOfFilesFromUSB();
 * usbContent.updateName("updatedName");			// To Update Name
 * usbContent.createDirectory("testDir");			// Create Directory inside root folder
 * usbContent.copyFile("PathFilePresentInFEU");		// Copy File from FEU to USB
 * usbContent.download("PathToSaveInFEU");			// Copy File to FEU from USB
 * \since 8.4
 */
class UsbContent
{
public:
	/**
	 * Returns list of the files from the usb.
	 *
	 * @param path Path inside the USB. [optional] if root path in the USB.
	 *
	 * @throws std::exception if any internal errors.
	 *
	 * @return Root USB content starting from the given path.
	 */
	static std::unique_ptr<::nevonex::resource::usb::UsbContent> getFiles(const std::string &path = "/");

	/**
	 * Returns USB contents to JSON representation but in a string datatype.
	 *
	 * @return stringContent holds the USB content(JSON).
	 */
	std::string toString() const;

	/**
	 * Returns USB contents to JSON representation (RapidJson).
	 *
	 * \code
	 * const std::unique_ptr<UsbContent> & usbContent = FileProvider::getInstance().getListOfFilesFromUSB();
	 * rapidjson::Document jsonDoc(rapidjson::kObjectType);
	 * usbContent->toJson(jsonDoc, jsonDoc.GetAllocator());
	 * \endcode
	 *
	 * @param[out] jsonDoc holds the USB content(JSON).
	 * @param	allocator Rapidjson document allocator.
	 */
	void toJson(rapidjson::Document & jsonDoc, rapidjson::Document::AllocatorType & allocator) const;

	/**
	 * Returns the name of the file/directory.
	 *
	 * @return name
	 */
	const std::string & getName() const;

	/**
	 * Returns the content type of the USB (file/directory).
	 *
	 * @return type : FILE/DIRECTORY
	 */
	ContentType getType() const;

	/**
	 * Returns the path of the file/directory in from the immediate parent directory.
	 *
	 * @return path
	 */
	const std::string & getPath() const;

	/**
	 * Returns the absolute path of the USB content in the USB.
	 *
	 * @return absolutePath
	 */
	std::string getAbsolutePath() const;

	/**
	 * Returns the whether file/directory is read only or not.
	 *
	 * @return readOnly : true/false
	 */
	bool isReadOnly() const;

	/**
	 * Returns the list of children(s) under the current directory.
	 *
	 * @return children : vector<UsbContent>
	 */
	std::vector<std::shared_ptr<UsbContent>> & getChildren();

	/**
	 * Returns the child at given path.
	 *
	 * @param path the path of usbContent
	 *
	 * @return children : UsbContent if found. null_ptr otherwise.
	 */
	std::shared_ptr<UsbContent> getChildAt(const std::string & path) const;

	/**
	 * Copy a file from FEATURE to USB.
	 *
	 * @param filePathInTheFeature Input file path to be copied to USB.
	 *
	 * @return the path where the file has been copied in the USB.
	 */
	boost::filesystem::path copyFile(const std::string & filePathInTheFeature);

	/**
	 * Copy a file from USB to Feature.
	 *
	 * @param aDirectoryInTheFeature Path where the particular file should be copied. [optional] can be empty. look for the return value where it has copied.
	 *
	 * @return the path where the file has been copied onto the feature.
	 */
	boost::filesystem::path download(const std::string & aDirectoryInTheFeature = {});

	/**
	 * Creates a directory with the name under the current UsbContent object.
	 *
	 * - root
	 *   - secondlevel
	 *     - *create a new directory here*
	 *
	 *
	 * the code snippet looks like:
	 * \code
	 * const std::unique_ptr<UsbContent> & usbContent = FileProvider::getInstance().getListOfFilesFromUSB();
	 * auto firstLevelUsbDir = usbContent->getChildAt("\root\firstlevel");
	 * firstLevelUsbDir->createDirectory("ThridLevelDir");
	 * \endcode
	 *
	 *
	 * @param directoryName The directory name
	 *
	 * @return true/false whether the operation is success or not.
	 */
	std::shared_ptr<UsbContent> createDirectory(const std::string & directoryName);

	/**
	 * Updates the name of an USBContent (directory or a file).
	 *
	 * @param updateName The name of the directory/file to be changed.
	 *
	 * @return true/false whether the operation is success or not.
	 */
	bool updateName(const std::string & updateName);

	/**
	 * Mount an OTG device onto the RCU
	 */
	static bool mountOTGDrive();

	/**
	 * Unmount a mounted OTG device from RCU
	 */
	static bool unMountOTGDrive();

private:
	UsbContent() = delete;
	UsbContent(const std::string & _prefix, const std::string & _root, const std::string & usbStringContent);
	UsbContent(const std::string & _prefix, const std::string & _name, const std::string & _type, const std::string & _path, bool _isReadOnly);
	friend class ::nevonex::resource::FileProvider;

	std::shared_ptr<UsbContent> parseChildUsbContent(const std::string & _prefix, const ::rapidjson::Value & childVal);
	void parseUsbContent(const std::string & _prefix, const std::string & _root, const std::string & _stringContent);
	boost::filesystem::path copyFileFromUSB(const ::nevonex::resource::usb::UsbContent & usbContent, const std::string & sourceFeuPath) const;
	boost::filesystem::path copyFileToUSB(const ::nevonex::resource::usb::UsbContent & usbContent, const std::string & destinationFeuPath) const;
	bool editUSBDirectoryStructure(const ::rapidjson::Document & responsePayload) const;
	static bool mountUnmountOTG(const ::rapidjson::Document &commandPayload);

	std::string name;
	ContentType type;
	std::string path;
	std::string prefix;
	bool readOnly;
	std::vector<std::shared_ptr<UsbContent>> children;

	static const char* NAME;
	static const char* TYPE;
	static const char* PATH;
	static const char* ABSOLUTE_PATH;
	static const char* CHILDREN;
	static const char* READ_ONLY;
	static const char* CREATE;
	static const char* RENAME;
	static const char* MESSAGE;

	static const std::string FILE;
	static const std::string DIRECTORY;

	static const std::string TAG_FEATURE_ID;
	static const std::string TAG_TIMESTAMP;
	static const std::string TAG_DESTINATION_PATH;
	static const std::string TAG_FILE_PATH;
	static const std::string TAG_SD_PATH;

	static const std::string DOWNLOAD_FROM_SD_URL;
	static const std::string UPLOAD_TO_SD_URL;
	static const std::string EDIT_DIRECTORIES_SD_URL;
	static const std::string USB_MOUNT_OTG_URL;

};

} //usb
} //resource
} //nevonex
#endif /* SOURCE_RESOURCE_USB_USBCONTENT_HPP_ */
