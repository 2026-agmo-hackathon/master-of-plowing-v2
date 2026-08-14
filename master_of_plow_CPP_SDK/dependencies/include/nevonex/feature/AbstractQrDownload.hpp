/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractQrDownload_H_
#define NEVONEX_AbstractQrDownload_H_

#include <string>

#include <ecore/EObject.hpp>

#include "../feature/AbstractDownload.hpp"
#include "../propertychange/AbstractPropertyChange.hpp"

namespace nevonex {
namespace feature {

class AbstractQrDownload : public virtual ::nevonex::propertychange::AbstractPropertyChange, public virtual ::nevonex::feature::AbstractDownload,  public virtual ::ecore::EObject
{
public:
	using ptr_t = ::ecore::Ptr<AbstractQrDownload>;

	virtual ~AbstractQrDownload() = default;
};
using AbstractQrDownload_ptr = AbstractQrDownload::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractQrDownload_H_

