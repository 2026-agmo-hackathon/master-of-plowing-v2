/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SRC_APPLICATION_MACHINE_ABSTRACTMACHINECONNECTIONLISTENER_HPP_
#define SRC_APPLICATION_MACHINE_ABSTRACTMACHINECONNECTIONLISTENER_HPP_

#include "../../propertychange/PropertyChangeListener.hpp"
#include "../../feature/AbstractMachine.hpp"
#include "../../machine/MachineConnectionInfo.hpp"

namespace nevonex
{
namespace app
{
namespace machine
{

/**
 * Abstract listener for machine connection
 * \since 8.0
 */
class AbstractMachineConnectionListener : public virtual ::nevonex::propertychange::PropertyChangeListener
{
public:
	AbstractMachineConnectionListener() = default;
	virtual ~AbstractMachineConnectionListener() = default;
protected:
	/**
	 * \brief For Handling Machine Connect
	 *
	 * \param machine which is Connected
	 * \param machineConnectionInfo briefed connection details. Connection Status and Error Status
	 * \return void
	 */
	virtual void machineConnected(::nevonex::feature::AbstractMachine_ptr machine) = 0;
	/**
	 * \brief For Handling Machine Disconnect
	 *
	 * \param machine which is disconnected
	 * \param machineConnectionInfo briefed connection details. Connection Status and Error Status
	 * \return void
	 */
	virtual void machineDisconnected(::nevonex::feature::AbstractMachine_ptr machine, ::nevonex::machine::MachineConnectionInfo_ptr machineConnectionInfo) = 0;
private:
	virtual void propertyChange(::nevonex::propertychange::PropertyChangeEvent<const ::ecore::EJavaObject&, const ::ecore::EJavaObject& > changeEvent) override;
};

} /* namespace machine */
} /* namespace app */
} /* namespace nevonex */

#endif /* SRC_APPLICATION_MACHINE_ABSTRACTMACHINECONNECTIONLISTENER_HPP_ */
