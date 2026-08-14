/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SOURCE_NOTIFY_ABSTRACTIGNITIONSTATENOTIFICATION_HPP_
#define SOURCE_NOTIFY_ABSTRACTIGNITIONSTATENOTIFICATION_HPP_

#include <string>

namespace nevonex
{
namespace notify
{
/**
 * INTERNAL DO NOT USE.
 */

class AbstractIgnitionStateNotification
{

public:
    AbstractIgnitionStateNotification() = default;
    virtual ~AbstractIgnitionStateNotification() = default;
    /**
     * \brief For Ignition On Feature.
     * This method is notified once the feature is started.
     * Feature developer can use this to do post started operations.
     * \param message
     * \return void
     */
    virtual void handleIgnitionOn() = 0;

    /**
     * \brief For Ignition Off Feature.
     * This method is notified once the feature is being stopped.
     * Feature developer can use this to do pre closure operations.
     * \param message
     * \return void
     */
    virtual void handleIgnitionOff() = 0;
};
} /* namespace feature */
} /* namespace nevonex */

#endif /* SOURCE_NOTIFY_ABSTRACTIGNITIONSTATENOTIFICATION_HPP_ */