/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef SRC_FEATURE_ABSTRACTFEATUREMANAGERLISTENER_HPP_
#define SRC_FEATURE_ABSTRACTFEATUREMANAGERLISTENER_HPP_

#include <string>

#include <ecore/EObject.hpp>
#include <nevonex-fcal-platform/notify/AbstractFeatureNotification.hpp>
#include "../../feature/AbstractMachineProvider.hpp"

namespace nevonex
{
namespace app
{
namespace feature
{
class AbstractFeatureManagerListener: public virtual ::nevonex::notify::AbstractFeatureNotification,
		public virtual ::ecore::EObject
{

protected:
	virtual void stopFeatureFramework(const std::string & message) final;
public:
	using ptr_t = ::ecore::Ptr<AbstractFeatureManagerListener>;

	virtual ~AbstractFeatureManagerListener() = default;
};
using AbstractFeatureManagerListener_ptr = AbstractFeatureManagerListener::ptr_t;

} // feature
} // app
} //nevonex
#endif /* SRC_FEATURE_ABSTRACTFEATUREMANAGERLISTENER_HPP_ */
