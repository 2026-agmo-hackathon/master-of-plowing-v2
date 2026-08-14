/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_CONFIG_GLOBAL_CONFIG_HPP
#define NEVONEX_FCAL_PLATFORM_CONFIG_GLOBAL_CONFIG_HPP

#include "../common/CommonUtils.hpp"
#include "../log/LogSetup.hpp"
#include "../log/Logger.hpp"
#include "../web/client/WebServiceUtil.hpp"
#include "../web/cloud/ICloud.hpp"
#include "../web/device/IDevice.hpp"
#include "../web/machine/IMachineDetails.hpp"
#include "../web/platform/IPlatformDownload.hpp"
#include "../web/server/AbstractHttpServerProvider.hpp"
#include "../web/hmi/AbstractHmiServerProvider.hpp"
#include "../resource/FileProvider.hpp"
#include "../resource/FilePath.hpp"
#include "../resource/usb/UsbContent.hpp"
#include "../hmi/AbstractUserControls.hpp"
#include "../notify/NotificationManager.hpp"
#include "../communicator/ICommunicator.hpp"

#include <string>
#include <rapidjson/document.h>

namespace nevonex::deviceinfoprovider
{
	class DeviceInfoProvider;
};

namespace nevonex::ntrip
{
	class NtripConnectionHandler;
	class NtripMqttClient;
};

namespace nevonex
{
namespace fcb
{
// TODO check;
class SubscribeConnectionFactory;
class PublishConnectionFactory;

}
}

namespace nevonex
{
namespace config
{
/**
 * INTERNAL DO NOT USE.
 */
enum class CONFIG_ENV
{
	NONE, DEV, PROD
};

/**
 * INTERNAL DO NOT USE.
 */
enum class LOG_TYPE
{
	NONE, FILE, CENTRAL, MQTT
};

/**
 * INTERNAL DO NOT USE.
 */
enum class STACKTRACE
{
	YES, NO
};

/**
 * INTERNAL DO NOT USE.
 */
class GlobalConfig final // TODO: featureConfig
{
public:
	static GlobalConfig& getInstance()
	{
		static GlobalConfig instance;
		return instance;
	}

private:
	GlobalConfig();
public:
	~GlobalConfig() = default;
	GlobalConfig(GlobalConfig const&) = delete;
	void operator=(GlobalConfig const&) = delete;

public:
	// TODO: restrict following visibility to respective classes:
	const std::string& getFeatureId() const;
	bool getIsSSLWsgiEnabled() const;


private:
	const std::vector<std::string>& getDisabledFeatures() const;

	const std::string& getMqttHost() const;
	boost::uint16_t getMqttPort() const;
	const std::string& getMqttSSLHost() const;
	boost::uint16_t getMqttSSLPort() const;
	const COMMUNICATOR_AUTH_TYPE& getCommunicatorAuthType() const;
	const std::string& getMqttUserName() const;
	const std::string& getMqttPassword() const;
	boost::uint32_t getMqttConnectionTimeout() const;
	const std::string& getCaCertPath() const;
	const std::string& getClientCertPath() const;
	const std::string& getClientKeyPath() const;
	const std::string& getCertBasePath() const;

	const std::string& getDeviceIp() const;
	const std::string& getDeviceStaticIp() const;
	friend class ::nevonex::fcb::SubscribeConnectionFactory;
	friend class ::nevonex::fcb::PublishConnectionFactory;


	boost::uint16_t getFgfPort() const;
	friend class ::nevonex::web::hmi::AbstractHmiServerProvider;

	boost::uint16_t getCustomUiPort() const;
	friend class ::nevonex::web::server::AbstractHttpServerProvider;
	friend class ::nevonex::notify::NotificationManager;

	const std::string& getLogPath() const;
	::nevonex::log::SeverityLevel getLogLevel() const;
	boost::uint32_t getMaxLogFileSize() const;
	boost::uint8_t getMaxLogFileCount() const;
	CONFIG_ENV getConfigEnv() const;
	const std::string& getRestLoggerUrl() const;


	friend class ::nevonex::log::LogSetup;
	friend class ::nevonex::log::Logger;

	const std::string& getWebServiceUserName() const;
	const std::string& getWebServicePassword() const;
	friend class ::nevonex::web::client::WebServiceUtil;

	const std::string& getServiceUrl() const;
	friend class ::nevonex::cloud::ICloud;
	friend class ::nevonex::device::IDevice;
	friend class ::nevonex::deviceinfoprovider::DeviceInfoProvider;
	friend class ::nevonex::web::platform::IPlatformDownload;
	friend class ::nevonex::web::machine::IMachineDetails;

	const std::string& getResourcePath() const;
	const uint64_t& getAllowedMaxResourceSizeInBytes() const;
	const std::string& getFifInPath() const;
	const std::string& getFifOutPath() const;

	const std::string& getResourcePathAbs() const;
	const std::string& getFifInPathAbs() const;
	const std::string& getFifOutPathAbs() const;
	friend class ::nevonex::resource::FilePath;
	friend class ::nevonex::resource::FileProvider;
	friend class ::nevonex::resource::usb::UsbContent;

	friend class ::nevonex::hmi::AbstractUserControls;
	friend class nevonex::ntrip::NtripConnectionHandler;
	friend class nevonex::ntrip::NtripMqttClient;

private:
	void initialize();
	void retrieveConfigFile(rapidjson::Document &root);
	void loadJsonConfig(const rapidjson::Document &configFile);
	void loadEnvironment(const rapidjson::Document &config);
	void loadMqttConfig(const rapidjson::Document &config);
	void loadCredentials(const rapidjson::Document &config);
	void validateCredentials();
	void loadLoggerDetails(const rapidjson::Document &config);
	void loadUiConfiguration(const rapidjson::Document &config);
	void loadStateRetentionConfiguration(const rapidjson::Document &config);
	void loadFifPathDetails(const rapidjson::Document &config);
	void loadAuthWebSocketToken();
	void loadDisabledFeature(const rapidjson::Document &config);
	void loadFIFDetails(const rapidjson::Document &config);

	void validatePath(const std::string &path, const std::string &errMessage);
	void validateFile(const std::string &file, const std::string &errMessage);

	std::string featureId;
	std::string deviceIp;
	std::string deviceStaticIp;
	boost::uint16_t fifPort;
	boost::uint16_t fsmPort;

	std::string serviceUrl;

	std::string mqttHost;
	boost::uint16_t mqttPort;
	std::string mqttSSLHost;
	boost::uint16_t mqttSSLPort;
	COMMUNICATOR_AUTH_TYPE communicatorAuthType;
	std::string mqttUserName;
	std::string mqttPassword;
	boost::uint32_t mqttConnectionTimeout;
	std::string caCertPath;
	std::string clientCertPath;
	std::string clientKeyPath;
	std::string certBasePath;

	boost::uint16_t customUiPort;
	boost::uint16_t fgfPort;

	std::string logPath;
	std::string restLoggerUrl;
	::nevonex::log::SeverityLevel logLevel;
	boost::uint32_t maxBytes;
	boost::uint8_t maxLogFileCount;
	LOG_TYPE logType;
	STACKTRACE stackTrace;

	boost::uint64_t allowedMaxResourceSizeInBytes;
	std::string resourcePath;
	std::string resourcePathAbs;
	std::string fifInPath;
	std::string fifInPathAbs;
	std::string fifOutPath;
	std::string fifOutPathAbs;

	std::string webServiceUserName;
	std::string webServicePassword;

	std::string authWebSocketToken;

	boost::uint8_t tableMaxCount;
	boost::uint8_t chartMaxCount;
	boost::uint8_t inputRateMaxCount;

	CONFIG_ENV configEnv;

	std::vector<std::string> disabledFeatures;

	bool isFSMConfigEnabled;
	bool isSSLWsgiEnabled;
};

} // config
} // nevonex
#endif // NEVONEX_FCAL_PLATFORM_CONFIG_GLOBAL_CONFIG_HPP
