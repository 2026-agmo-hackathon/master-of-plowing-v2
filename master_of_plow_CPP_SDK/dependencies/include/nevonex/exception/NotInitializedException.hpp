/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_NotInitializedException_H_
#define NEVONEX_NotInitializedException_H_

#include <stdexcept>

namespace nevonex
{
namespace exception
{
/**
 * \author per9kor
 * \since 1.0
 */
class NotInitializedException: public std::runtime_error
{
public:
	inline explicit NotInitializedException(const std::string &_message) : std::runtime_error(_message)
	{
	}
	inline explicit NotInitializedException(const std::string &_message,
			const std::exception &_ex) :
			NotInitializedException(_message + _ex.what())
	{
	}
};
} // exception
} // nevonex

#endif // NEVONEX_NotInitializedException_H_
