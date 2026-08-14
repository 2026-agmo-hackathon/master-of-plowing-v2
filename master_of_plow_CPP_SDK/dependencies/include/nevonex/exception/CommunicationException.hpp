/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_CommunicationException_H_
#define NEVONEX_CommunicationException_H_

#include <stdexcept>

namespace nevonex
{
namespace exception
{
/**
 * \author per9kor
 * \since 1.0
 */
class CommunicationException: public std::runtime_error
{
public:
	inline explicit CommunicationException(const std::string &_message) : std::runtime_error (_message)
	{
	}
	inline explicit CommunicationException(const std::string &_message, const std::exception &_ex) : CommunicationException (_message + _ex.what())
	{
	}
};
} // exception
} // nevonex

#endif // NEVONEX_CommunicationException_H_
