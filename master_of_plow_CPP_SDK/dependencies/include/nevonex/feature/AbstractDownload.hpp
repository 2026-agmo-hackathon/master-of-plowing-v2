/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractDownload_H_
#define NEVONEX_AbstractDownload_H_

#include <string>

#include <ecore/EObject.hpp>

namespace nevonex {
namespace feature {

/**
 * Abstract Download
 * \author per9kor
 * \since 5.0
 */
class AbstractDownload : public virtual ::ecore::EObject
{
public:
	using ptr_t = ::ecore::Ptr<AbstractDownload>;

	/**
	 * Starts the download subscriber.
	 * \since 5.0
	 */
	virtual void start() = 0;

	virtual ~AbstractDownload() = default;
};
using AbstractDownload_ptr = AbstractDownload::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractDownload_H_

