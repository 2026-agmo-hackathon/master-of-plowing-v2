/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef SOURCE_COMMUNICATOR_COMMUNICATORPROPERTIES_HPP_
#define SOURCE_COMMUNICATOR_COMMUNICATORPROPERTIES_HPP_

#include <string>

namespace nevonex
{

namespace config
{
	enum class COMMUNICATOR_AUTH_TYPE;
}

namespace communicator
{

/**
 * INTERNAL DO NOT USE.
 */
class CommunicatorProperties
{
public:
	CommunicatorProperties() : isFSMEnabled(false), callerId(""), host(""), port(0),
			sslHost(""), sslPort(0), fsmCertFolderPath(""){}
	~CommunicatorProperties() = default;
	CommunicatorProperties(CommunicatorProperties const&) = delete;
	void operator=(CommunicatorProperties const&) = delete;

	bool isFSMEnabled;

	std::string callerId;

	std::string host;
	int port;
	std::string userName;
	std::string password;

	std::string sslHost;
	int sslPort;

	std::string trustStorePath;
	std::string keyStorePath;
	std::string privateKeyPath;

	std::string fsmCertFolderPath;

	::nevonex::config::COMMUNICATOR_AUTH_TYPE communicationAuthType;

};

} // communicator
} // nevonex
#endif /* SOURCE_COMMUNICATOR_COMMUNICATORPROPERTIES_HPP_ */
