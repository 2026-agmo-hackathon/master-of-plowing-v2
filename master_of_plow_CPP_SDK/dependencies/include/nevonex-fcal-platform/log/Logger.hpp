/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_PLATFORM_LOG_LOGGER_HPP_
#define NEVONEX_FCAL_PLATFORM_LOG_LOGGER_HPP_

#include "../log/LogWriter.hpp"

#include <memory>
#include <sstream>

/*
 *  * Example:
 *
 * #include <nevonex-fcal-platform/log/Logger.hpp>
 * using namespace ::nevonex::log;
 * NEVONEX_LOG(SeverityLevel::Trace) << "Hi its printing";
 */
#define NEVONEX_LOG(lvl) ::nevonex::log::LogWriter(lvl, 0)

/**
 * INTERNAL DO NOT USE.
 */
#define PLATFORM_FCAL_LOG(lvl) ::nevonex::log::LogWriter(lvl, 1)

/**
 * INTERNAL DO NOT USE.
 */
#define RUNTIME_FCAL_LOG(lvl) ::nevonex::log::LogWriter(lvl, 2)

/**
 * INTERNAL DO NOT USE.
 */
#define SDK_FCAL_LOG(lvl) ::nevonex::log::LogWriter(lvl, 3)

namespace nevonex
{

namespace log
{

class ILogger;

enum class SeverityLevel
{
    trace,
    debug,
    info,
    warning,
    error,
	critical,
    fatal,
	incident	
};

/**
 * \since 8.0
 * Nevonex Logging Class.
 */
class Logger
{
public:
	/**
	 * Main application logger
	 */
	void log(SeverityLevel lvl, const std::string & msg);

	static Logger& getInstance()
	{
		static Logger instance;
		return instance;
	}
	/**
	 * Initializing logger.
	 */
	void initialize();

	~Logger();
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

	static std::string INC_018_02;

	static std::string INC_018_04;

	static std::string INC_011_02;

	static std::string INC_011_03;

	static std::string INC_011_04;



	/**
	 * Check whether log is enabled or not
	 */
	bool isLogEnabled(const SeverityLevel lvl);
	/**
	 * Trace log level.
	 * \param msg message
	 */
	void trace(const std::string & msg);
	/**
	 * Debug log level.
	 * \param msg message
	 */
	void debug(const std::string & msg);
	/**
	 * Info log level.
	 * \param msg message
	 */
	void info(const std::string & msg);
	/**
	 * Warn log level.
	 * \param msg message
	 */
	void warn(const std::string & msg);
	/**
	 * Error log level.
	 * \param msg message
	 */
	void error(const std::string & msg);
	/**
	 * Fatal log level.
	 * \param msg message
	 */
	void fatal(const std::string & msg);
	/**
	 * Critical log level.
	 * \param msg message
	 */
	void critical(const std::string & msg);
	/**
	 * Incident log level.
	 * \param msg message
	 */
	void incident(const std::string & msg);
	/**
	 * Internal Use Only.
	 * \lvl level of log message
	 * \param msg message
	 */
	void fallbackLog(const SeverityLevel lvl, const std::string & msg);

private:
	std::unique_ptr<ILogger> _log;
	std::unique_ptr<ILogger> _fallbackLog;
	static bool isInited;

	Logger();
};

}//log
}//nevonex


#endif /* NEVONEX_FCAL_PLATFORM_LOG_LOGGER_HPP_ */
