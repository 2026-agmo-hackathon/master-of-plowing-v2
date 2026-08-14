/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_COMMON_UTILS_HPP
#define NEVONEX_FCAL_PLATFORM_COMMON_UTILS_HPP

#include "../log/Logger.hpp"

#include <string>

#include <jsoncpp/json/json.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include <boost/log/trivial.hpp>

namespace nevonex
{
namespace common
{
namespace utils
{
class CommonUtils
{
public:
	/**
	 * Encode a given string with SHA256 algorithm.
	 * \param[in] normalString normal string without any encryption
	 * \param[out] encodedString Encoded String.
	 *
	 * \return Nothing
	 */
	static void encodeSHA256(const std::string &normalString, std::string &encodedString);
	/**
	 * Convert String To Boost Log Level.
	 * \param logLevelStr normal string to boost level enum
	 *
	 * \return Returning enum value of the given string
	 */
	static ::nevonex::log::SeverityLevel convertStringToNVLogLevel(const std::string &logLevelStr);
	/**
	 * Convert String To Boost Log Level.
	 * \param logLevelStr normal string to boost level enum
	 *
	 * \return Returning string value of the given string
	 */
	static const std::string convertNVLogLevelToString(const ::nevonex::log::SeverityLevel nvLogLvl);
	/**
	 * Convert NVX Log Level To Boost Log Level.
	 * \param logLevelStr normal string to boost level enum
	 *
	 * \return Returning enum value of the given string
	 */
	static ::boost::log::trivial::severity_level convertNVLogLevelToBoostLogLevel(const ::nevonex::log::SeverityLevel nvLogLvl);
	/**
	 * Convert String To JsonObject.
	 * \param[in] inputString Normal string which is in json format
	 * \param[out] outputJsonObject Holds the json object
	 *
	 * \return Nothing
	 */
	static void convertStringToJsonObject(const std::string & inputString, Json::Value & outputJsonObject);
	/**
	 * Convert String To JsonObject.
	 * \param[in] inputString Normal string which is in json format
	 * \param[out] outputJsonObject Holds the json object
	 *
	 * \return Nothing
	 */
	static void convertStringToJsonObject(const char* inputString, rapidjson::Document & outputJsonObject);
	/**
	 * Convert JsonObject To String.
	 * \param[in] inputJsonObject Holds the json object
	 * \param[out] outputString Normal string which is in json format
	 *
	 * \return Nothing
	 */
	static void convertJsonObjectToString(const Json::Value & inputJsonObject,  std::string & outputString);
	/**
	 * Convert Json To String.
	 *
	 * \param[in] inputJson Holds the json object
	 * \param[out] stringBuffer buffer to be used for conversion.
	 *
	 * \return pointer to the json string
	 */
	static void convertJsonToString(const ::rapidjson::Document & inputJson, ::rapidjson::StringBuffer & stringBuffer);
	/**
	 * Convert Json To String.
	 *
	 * \param[in] inputJson Holds the json object
	 *
	 * \return pointer to the json string
	 */
	static const std::string convertJsonToString(const ::rapidjson::Document & inputJson);
	/**
	 * Convert Json To String.
	 *
	 * \param[in] inputJson Holds the json object
	 *
	 * \return pointer to the json string
	 */
	static const std::string convertJsonToString(const ::rapidjson::Value & inputJson);
	/**
	 *
	 * \return current milliseconds
	 */
	static int64_t getCurrentMilliseconds();
	/**
	 * Write files to file content.
	 * \param[in] filePath File path where to write
	 * \param[in] contents contents which should be written to file
	 *
	 * \return Nothing
	 */
	static void writeToFile(const std::string & filePath, const std::string & contents);
	/**
	 * Delete Files from the given path.
	 * \param[in] filePath File path where to delete
	 *
	 * \return Nothing
	 */
	static void deleteFile(const std::string & filePath);

	~CommonUtils() = default;
	CommonUtils(CommonUtils const&) = delete;
	void operator=(CommonUtils const&) = delete;

private:
	CommonUtils() = default;
	static Json::CharReaderBuilder charReaderBuilder;
	static Json::StreamWriterBuilder streamWrtierBuilder;

};
} // utils
} // common
} // nevonex

#endif // NEVONEX_FCAL_PLATFORM_COMMON_UTILS_HPP
