/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_AbstractMachineProvider_H_
#define NEVONEX_AbstractMachineProvider_H_

#include "../machine/MachineConnectionInfo.hpp"

#include <ecore/EObject.hpp>

namespace nevonex {
namespace feature {

/**
 * Root interface for Machine Provider
 * \author per9kor
 * \since 1.0
 */
class AbstractMachineProvider : public virtual ::ecore::EObject
{
public:
	/**
	 * \since 4.1.0
	 */
	using ptr_t = ::ecore::Ptr<AbstractMachineProvider>;

	virtual const std::string& getProviderName() = 0;
	/**
	 * Initializes the machine provider. This shall be called after creation of machine provider instance.
	 */
	virtual void initMachineProvider() = 0;
	/**
	 * Start the machine provider.
	 * \since 4.1.0
	 */
	virtual void start() = 0;
	/**
	 * Stops the machine provider.
	 * \since 4.1.0
	 */
	virtual void stop() = 0;
	/**
	 * Start the machine provider.
	 * \since 4.1.0
	 */
	virtual void restart() = 0;
	/**
	 * Connect/Disconnect the machine provider.
	 * \since 8.0.0
	 * \param index Machine index which is disconnected.
	 * \param machineConnectionInfo Machine Connection Info.
	 */
	virtual void onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr machineConnectionInfo) = 0;

	/**
	 * Sets communication server properties.
	 * \deprecated since 5.0
	 * \param props Properties to be set.
	 */
	virtual inline void setConnectionProps(const ::ecore::EStringMap & props)
	{
		(void) props;
	};
	virtual ~AbstractMachineProvider() = default;
};
using AbstractMachineProvider_ptr  = AbstractMachineProvider::ptr_t;

} // feature
} // nevonex

#endif // NEVONEX_AbstractMachineProvider_H_
