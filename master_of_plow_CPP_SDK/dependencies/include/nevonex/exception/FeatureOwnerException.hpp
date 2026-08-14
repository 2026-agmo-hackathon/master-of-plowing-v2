/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FeatureOwnerException_H_
#define NEVONEX_FeatureOwnerException_H_

#include <stdexcept>

namespace nevonex
{
namespace exception
{
/**
 * \author per9kor
 * \since 1.0
 */
class FeatureOwnerException: public std::runtime_error
{
public:
	explicit FeatureOwnerException(const std::string &_message, const std::exception &_ex)  : std::runtime_error (_message +_ex.what())
	{
	}
};
} // exception
} // nevonex

#endif // NEVONEX_FeatureOwnerException_H_
