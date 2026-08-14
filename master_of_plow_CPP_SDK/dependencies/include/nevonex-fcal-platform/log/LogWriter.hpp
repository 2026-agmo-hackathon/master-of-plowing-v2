/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCAL_PLATFORM_LOG_LOGWRITER_HPP_
#define NEVONEX_FCAL_PLATFORM_LOG_LOGWRITER_HPP_

#include <memory>

namespace nevonex
{

namespace log
{

enum class SeverityLevel;

/**
 * INTERNAL DO NOT USE.
 */
class IWriter
{

#define LOG_TEMPLATE_IWRITER(DataType) virtual IWriter& append(DataType msg) = 0;

public:

	LOG_TEMPLATE_IWRITER(char)
	LOG_TEMPLATE_IWRITER(bool)
	LOG_TEMPLATE_IWRITER(signed short)
	LOG_TEMPLATE_IWRITER(unsigned short)
	LOG_TEMPLATE_IWRITER(signed int)
	LOG_TEMPLATE_IWRITER(unsigned int)
	LOG_TEMPLATE_IWRITER(signed long)
	LOG_TEMPLATE_IWRITER(unsigned long)
	LOG_TEMPLATE_IWRITER(float)
	LOG_TEMPLATE_IWRITER(double)
	LOG_TEMPLATE_IWRITER(char*)
	LOG_TEMPLATE_IWRITER(const char*)
	LOG_TEMPLATE_IWRITER(const void*)
	LOG_TEMPLATE_IWRITER(long double)
	LOG_TEMPLATE_IWRITER(long long int)
	LOG_TEMPLATE_IWRITER(unsigned long long)

	virtual IWriter& append(const std::string& msg) = 0;

	virtual IWriter& append(std::ostream& (*log)(std::ostream&)) = 0;

	IWriter() = default;
	virtual ~IWriter() = default;
	IWriter(IWriter const&) = delete;
	void operator=(IWriter const&) = delete;
};

/**
 * INTERNAL DO NOT USE.
 */
class LogWriter
{

# define LOG_TEMPLATE_WRITER(DataType)\
inline LogWriter& operator<<(DataType msg) {\
	iWriter->append(msg);\
	return *this;\
}

public:
	LogWriter(SeverityLevel  lvl, int type);

	LOG_TEMPLATE_WRITER(char)
	LOG_TEMPLATE_WRITER(bool)
	LOG_TEMPLATE_WRITER(signed short)
	LOG_TEMPLATE_WRITER(unsigned short)
	LOG_TEMPLATE_WRITER(signed int)
	LOG_TEMPLATE_WRITER(unsigned int)
	LOG_TEMPLATE_WRITER(signed long)
	LOG_TEMPLATE_WRITER(unsigned long)
	LOG_TEMPLATE_WRITER(float)
	LOG_TEMPLATE_WRITER(double)
	LOG_TEMPLATE_WRITER(char*)
	LOG_TEMPLATE_WRITER(const char*)
	LOG_TEMPLATE_WRITER(const void*)
	LOG_TEMPLATE_WRITER(long double)
	LOG_TEMPLATE_WRITER(long long int)
	LOG_TEMPLATE_WRITER(unsigned long long)

	inline LogWriter& operator<<(const std::string& msg) {
		iWriter->append(msg);
		return *this;
	}
	inline LogWriter& operator<<(std::ostream& (*log)(std::ostream&))
	{
		iWriter->append(log);
		return *this;
	}

	~LogWriter() = default;
	LogWriter(LogWriter const&) = delete;
	void operator=(LogWriter const&) = delete;
private:
	std::unique_ptr<IWriter> iWriter;
};

}//log
}//nevonex


#endif /* NEVONEX_FCAL_PLATFORM_LOG_LOGWRITER_HPP_ */
