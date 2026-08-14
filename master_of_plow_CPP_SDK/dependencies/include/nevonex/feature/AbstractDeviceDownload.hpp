/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractDeviceDownload_H_
#define NEVONEX_AbstractDeviceDownload_H_

#include <string>

#include <ecore/EObject.hpp>

#include "../feature/AbstractDownload.hpp"
#include "../propertychange/AbstractPropertyChange.hpp"

namespace nevonex {
namespace feature {

/**
 * Abstract Device Download
 * \author per9kor
 * \since 5.0
 */
class AbstractDeviceDownload : public virtual ::nevonex::propertychange::AbstractPropertyChange, public virtual ::nevonex::feature::AbstractDownload
{
public:
	using ptr_t = ::ecore::Ptr<AbstractDeviceDownload>;

	virtual ~AbstractDeviceDownload() = default;
};
using AbstractDeviceDownload_ptr = AbstractDeviceDownload::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractDeviceDownload_H_

