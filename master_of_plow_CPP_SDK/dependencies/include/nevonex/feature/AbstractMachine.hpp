/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractMachine_H_
#define NEVONEX_AbstractMachine_H_

#include <ecore/EObject.hpp>

namespace nevonex {
namespace feature {

/**
 * Root interface for Machine type
 * \author per9kor
 * \since 5.0
 */
class AbstractMachine : public virtual ::ecore::EObject
{
public:
	/**
	 * \since 4.1.0
	 */
	using ptr_t = ::ecore::Ptr<AbstractMachine>;

	virtual inline bool isInitialized()
	{
		return this->m_initialized;
	}
	virtual inline void setInitalized(bool initialized)
	{
		this->m_initialized = initialized;
	}
	virtual ~AbstractMachine() = default;
private:
	bool m_initialized = {false};
};
using AbstractMachine_ptr  = AbstractMachine::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractMachine_H_
