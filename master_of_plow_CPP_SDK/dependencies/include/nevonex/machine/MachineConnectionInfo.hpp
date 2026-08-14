/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef SOURCE_FEATURE_MACHINECONNECTIONINFO_HPP_
#define SOURCE_FEATURE_MACHINECONNECTIONINFO_HPP_

#include <ecore/EObject.hpp>
#include <string>
#include <nevonex-fcal-platform/log/Logger.hpp>

namespace nevonex
{

namespace machine
{
/**
 * Connection status of machine
 * \since 8.0
 */
enum class CONNECTION_STATUS
{
	CONNECTED,
	DISCONNECTED,
	SAFEMODE
};
/**
 * Error in connection status
 * \since 8.0
 */
enum class ERROR_STATUS
{
	NOERROR_,
	BUSOFF
};

inline const std::string getLiteral(ERROR_STATUS e)
{
	switch (e)
	{
	case ERROR_STATUS::NOERROR_:
		return "NOERROR_";
	case ERROR_STATUS::BUSOFF:
		return "BUSOFF";
	}
	return "INVALID ERROR STATUS";
}

inline const std::string getLiteral(CONNECTION_STATUS e)
{
	switch (e)
	{
	case CONNECTION_STATUS::CONNECTED:
		return "CONNECTED";
	case CONNECTION_STATUS::DISCONNECTED:
		return "DISCONNECTED";
	case CONNECTION_STATUS::SAFEMODE:
		return "SAFEMODE";
	}
	return "INVALID CONNECTION STATUS";
}

/**
 * Context for Machine Connection Status
 * \since 8.0
 */
class MachineConnectionInfo : public virtual ::ecore::EObject
{
private:
	std::int64_t m_timestamp;
	std::int32_t connectionStatusCode = -1;

	void setConnectionStatusCode(int set)
	{
		connectionStatusCode = set;
	}

public:
	CONNECTION_STATUS connectionStatus;
	ERROR_STATUS errorStatus;

	int getConnectionStatusCode() const
	{
		return connectionStatusCode;
	}
	std::int64_t getTimestamp() const
	{
		return m_timestamp;
	}
	CONNECTION_STATUS getConnectionStatus() const
	{
		return connectionStatus;
	}
	ERROR_STATUS getErrorDetails() const
	{
		return errorStatus;
	}

	using ptr_t = ::ecore::Ptr<MachineConnectionInfo>;

	MachineConnectionInfo(int _connectionStatus, int _errorStatus, std::int64_t _timestamp) : m_timestamp(_timestamp)
	{
		switch (_connectionStatus)
		{
		case 0:
			connectionStatus = CONNECTION_STATUS::DISCONNECTED;
			setConnectionStatusCode(202);
			break;
		case 1:
			connectionStatus = CONNECTION_STATUS::CONNECTED;
			setConnectionStatusCode(201);
			break;
		case 2:
			// TO DO
			connectionStatus = CONNECTION_STATUS::SAFEMODE;
			break;
		default:
			using namespace ::nevonex::log;
			RUNTIME_FCAL_LOG(SeverityLevel::warning) << "Invalid connection status. " << _connectionStatus;
		}

		switch (_errorStatus)
		{
		case 0:
			errorStatus = ERROR_STATUS::NOERROR_;
			break;
		case 1:
			errorStatus = ERROR_STATUS::BUSOFF;
			break;
		default:
			using namespace ::nevonex::log;
			RUNTIME_FCAL_LOG(SeverityLevel::warning) << "Invalid error status. " << _errorStatus;
		}
	}
	~MachineConnectionInfo() = default;
	void operator=(MachineConnectionInfo const &) = delete;
};

using MachineConnectionInfo_ptr = MachineConnectionInfo::ptr_t;

} // machine
} // nevonex
#endif /* SOURCE_FEATURE_MACHINECONNECTIONINFO_HPP_ */
