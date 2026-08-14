/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_WEBSERVICE_HPP
#define NEVONEX_FCAL_PLATFORM_WEB_WEBSERVICE_HPP

#include <string>
#include <chrono>

#include <curlpp/Form.hpp>
#include <curlpp/Options.hpp>

namespace nevonex
{
namespace web
{
namespace client
{
/**
 * INTERNAL
 */
class WebServiceUtil final
{

public:
	static WebServiceUtil& getInstance()
	{
		static WebServiceUtil instance;
		return instance;
	}

private:
	WebServiceUtil();
public:
	~WebServiceUtil();
	WebServiceUtil(WebServiceUtil const&) = delete;
	void operator=(WebServiceUtil const&) = delete;

	std::string updateAuthWebSocketToken();

	std::string get(const std::string &url,
			const std::chrono::milliseconds &timeOut = std::chrono::milliseconds(DEFAULT_TIMEOUT));
	std::string perform(const curlpp::Forms &formParts, const std::string &url);
	std::string perform(const std::string &url,
			const curlpp::options::UserPwd &userPwd,
			const curlpp::Forms &formParts,
			const std::chrono::milliseconds &timeOut  =
					std::chrono::milliseconds(DEFAULT_TIMEOUT));
	std::string performJsonPost(const std::string &url, const std::string & postData,
			const std::chrono::milliseconds &timeOut  =
								std::chrono::milliseconds(DEFAULT_TIMEOUT));

	std::string performJsonPostPoco(std::string &baseUrl, std::string &endPoint, std::string &postData,
		const std::chrono::milliseconds &timeOut =
								std::chrono::milliseconds(DEFAULT_TIMEOUT));

private:
	void initialize();
	void applyMtls(curlpp::Easy &request) const;

	std::string authUserPwd;
	std::string trustStorePath;
	std::string keyStorePath;
	std::string privateKeyPath;
protected:
	static const int DEFAULT_TIMEOUT;
};

} // client

} // web

} // nevonex

#endif // NEVONEX_FCAL_PLATFORM_WEB_WEBSERVICE_HPP
