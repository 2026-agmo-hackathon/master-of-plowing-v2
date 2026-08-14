/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_NevonexException_H_
#define NEVONEX_NevonexException_H_

#include <stdexcept>

namespace nevonex
{
namespace exception
{

class NevonexException: public std::runtime_error
{
public:
	inline explicit NevonexException(const std::string &_message) : std::runtime_error(_message)
	{
	}
	inline explicit NevonexException(const std::string &_message,
			const std::exception &_ex) :
		NevonexException(_message + _ex.what())
	{
	}
};
} // exception
} // nevonex

#endif // NEVONEX_NevonexException_H_
