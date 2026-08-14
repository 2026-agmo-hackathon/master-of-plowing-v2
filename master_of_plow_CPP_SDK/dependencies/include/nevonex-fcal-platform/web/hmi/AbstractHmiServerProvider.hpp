/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_FCAL_PLATFORM_WEB_HMI_ABSTRACTHMISERVERPROVIDER_HPP_
#define NEVONEX_FCAL_PLATFORM_WEB_HMI_ABSTRACTHMISERVERPROVIDER_HPP_

#include "../../web/server/AbstractHttpServerProvider.hpp"
#include "../../web/hmi/HmiWebsocketRouteFactory.hpp"

namespace nevonex
{
namespace web
{
namespace hmi
{
/**
 * Internal
 */
class AbstractHmiServerProvider : public ::nevonex::web::server::AbstractHttpServerProvider
{
public:
	AbstractHmiServerProvider() = default;
	virtual ~AbstractHmiServerProvider() = default;
public:
	virtual void start() override; // NOSONAR - cpp:S1242 Because this is overriden here and also derived classes of this can override.

protected:
	virtual void registerWebsocketRoute(const std::string &path, // NOSONAR - cpp:S1242 This is different from base class method and also derived classes of this can override.
				std::shared_ptr<::nevonex::web::hmi::HmiWebsocketRouteFactory> router);
	virtual void registerRoute(const std::string &path,
			std::shared_ptr<::nevonex::web::server::NevonexRouteFactory> router);
};

} /* namespace hmi */
} /* namespace web */
} /* namespace nevonex */

#endif /* NEVONEX_FCAL_PLATFORM_WEB_HMI_ABSTRACTHMISERVERPROVIDER_HPP_ */
