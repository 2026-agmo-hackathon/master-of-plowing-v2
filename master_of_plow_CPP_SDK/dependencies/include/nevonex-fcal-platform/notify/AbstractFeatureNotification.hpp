/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SOURCE_NOTIFY_ABSTRACTFEATURENOTIFICATION_HPP_
#define SOURCE_NOTIFY_ABSTRACTFEATURENOTIFICATION_HPP_

#include <string>

namespace nevonex
{
namespace notify
{
/**
 * INTERNAL DO NOT USE.
 */

class AbstractFeatureNotification
{

public:
	AbstractFeatureNotification() = default;
	virtual ~AbstractFeatureNotification() = default;
	/**
	 * \brief For Starting Feature.
	 * This method is notified once the feature is started.
	 * Feature developer can use this to do post started operations.
	 * \param message
	 * \return void
	 */
	virtual void handleFeatureStart(const std::string & message) = 0;

	/**
	 * \brief For Stopping Feature.
	 * This method is notified once the feature is being stopped.
	 * Feature developer can use this to do pre closure operations.
	 * \param message
	 * \return void
	 */
	virtual void handleFeatureStop(const std::string & message) = 0;

	virtual void stopFeatureFramework(const std::string & message) = 0;
};
} /* namespace feature */
} /* namespace nevonex */

#endif /* SOURCE_NOTIFY_ABSTRACTFEATURENOTIFICATION_HPP_ */
