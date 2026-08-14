/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#pragma once

#include <string>

namespace nevonex
{

namespace machine
{

/**
 * Context for Interface Details
 * \since 8.3.0.rc10
 */
class InterfaceDetails
{
private:
	// const std::string m_name;
	// const std::uint32_t m_id;
	const std::string m_id_str;
	std::int64_t m_timestamp;
	bool m_valid;

public:
	// InterfaceDetails(const std::string & interfaceName, std::uint32_t interfaceId) : m_name(interfaceName), m_id(interfaceId) 
	InterfaceDetails(const std::string & interfaceId) : m_id_str(interfaceId), m_timestamp(0), m_valid(false)
	{

	}
	~InterfaceDetails() = default;
	void operator=(InterfaceDetails const&) = delete;

	inline std::int64_t getTimestamp() const
	{
		return m_timestamp;
	}

	inline bool isValid() const
	{
		return m_valid;
	}

	inline void setValid(bool valid, std::int64_t t)
	{
		m_valid = valid;
		m_timestamp = t;
	}
};

} // machine
} // nevonex
