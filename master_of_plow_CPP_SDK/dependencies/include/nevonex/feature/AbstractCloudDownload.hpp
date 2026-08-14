/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractCloudDownload_H_
#define NEVONEX_AbstractCloudDownload_H_

#include <string>

#include <ecore/EObject.hpp>

#include "../feature/AbstractDownload.hpp"
#include "../propertychange/AbstractPropertyChange.hpp"

namespace nevonex {
namespace feature {

/**
 * Cloud Download
 * \author per9kor
 * \since 5.0
 */
class AbstractCloudDownload : public virtual ::nevonex::propertychange::AbstractPropertyChange, public virtual ::nevonex::feature::AbstractDownload,  public virtual ::ecore::EObject
{
public:
	using ptr_t = ::ecore::Ptr<AbstractCloudDownload>;

	virtual ~AbstractCloudDownload() = default;
};
using AbstractCloudDownload_ptr = AbstractCloudDownload::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractCloudDownload_H_

