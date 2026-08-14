/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_LOG_LOGSETUP_HPP
#define NEVONEX_FCAL_PLATFORM_LOG_LOGSETUP_HPP

#include <iostream>

namespace nevonex
{

namespace log
{
/**
 * INTERNAL
 */
class LogSetup final
{
public:
	static void initLogConfiguration();
public:
	LogSetup() = delete;
	~LogSetup() = delete;
	LogSetup(LogSetup const&) = delete;
	void operator=(LogSetup const&) = delete;

};

} // log
} // nevonex

#endif // NEVONEX_FCAL_PLATFORM_LOG_LOGSETUP_HPP

