/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_InterfaceDisabledException_H_
#define NEVONEX_InterfaceDisabledException_H_

#include "../exception/NevonexException.hpp"
namespace nevonex
{
namespace exception
{

class InterfaceDisabledException: public NevonexException
{
public:
	inline explicit InterfaceDisabledException(const std::string &_message) : NevonexException (_message)
	{
	}
	inline explicit InterfaceDisabledException(const std::string &_message, const std::string &_key) : NevonexException (_message + _key)
	{
	}
	inline explicit InterfaceDisabledException(const std::string &_message, const std::exception &_ex) : InterfaceDisabledException (_message + _ex.what())
	{
	}
};
} // exception
} // nevonex

#endif // NEVONEX_InterfaceDisabledException_H_
