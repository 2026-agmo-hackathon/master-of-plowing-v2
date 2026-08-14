/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_ABSTRACT_APPLICATION_HPP_
#define NEVONEX_ABSTRACT_APPLICATION_HPP_

#include <string>
#include <unordered_map>

#include "../feature/AbstractMachine.hpp"
#include "../feature/AbstractMachineProvider.hpp"
#include "../propertychange/PropertyChangeListener.hpp"

#include <nevonex-fcal-platform/notify/AbstractFeatureNotification.hpp>
#include <nevonex-fcal-platform/notify/AbstractIgnitionStateNotification.hpp>
using namespace ::nevonex::propertychange; // NOSONAR - cpp:S1003 since this is a header and if change can affect multple components

namespace nevonex
{

/**
 * Main class for the Nevonex application.
 *
 * Any application Main class has to extend this for improved usage.
 * \author per9kor
 */
class NEVONEXApplication
{
public:
	/**
	 * Call back for the user once the Machine provider is initialized.
	 * i.e: Once the Machine DOM is received from the FIL to FCAL this will method will be invoked.
	 *
	 * \since 4.1.0
	 *
	 * \param provider
	 * \return
	 */
	virtual bool onStart(::nevonex::feature::AbstractMachineProvider_ptr provider) = 0;
	/**
	 *
	 * \since 5.0
	 */
	virtual void onStart(::nevonex::feature::AbstractMachine_ptr machine) = 0;
	/**
	 * Initializes the SDK based on your configuration generated with Feature Designer Project.
	 * \since 6.0
	 */
	virtual void initialize(const std::vector<std::string> & providernames) final;
	/**
	 * Starts the application and
	 * \since 6.0
	 */
	virtual void start() final;
	/**
	 *
	 * \param name Name of the Machine provider.
	 * \return machine provider instance;
	 * \since 4.1.0
	 */
	virtual ::nevonex::feature::AbstractMachineProvider_ptr getMachineProvider(const std::string &name);
	/**
	 *
	 * \param abstractFeatureNotification AbstractFeatureNotification which will used as a callback for notifying feature start and stop.
	 * \since 8.3.0
	 */
	virtual void setFeatureManager(std::unique_ptr<::nevonex::notify::AbstractFeatureNotification> abstractFeatureNotification) final;
	/**
     *
     * \param abstractIgnitionStateNotification AbstractIgnitionStateNotification which will used as a callback for ignition on and off..
     * \since 8.4.0
     */
    virtual void setIgnitionStateManager(std::unique_ptr<::nevonex::notify::AbstractIgnitionStateNotification> abstractIgnitionStateNotification) final;
protected:
	NEVONEXApplication();
	virtual ~NEVONEXApplication() = default;

private:
	/**
	 * Made private in v6.0
	 * use start() instead.
	 * \since 1.0
	 */
	virtual void monitorEnd();

	/**
	 * Property change listener
	 */
	class PropertyListener: public virtual PropertyChangeListener
	{	
	public:
		explicit PropertyListener(NEVONEXApplication& parent);
		virtual ~PropertyListener() = default;
		virtual void propertyChange(PropertyChangeEvent<const ::ecore::EJavaObject&, const ::ecore::EJavaObject&> changeEvent) override;
	private:
		NEVONEXApplication& parent_;
	};
private:
	void setMachineprovider(const std::string & providerkey, ::nevonex::feature::AbstractMachineProvider_ptr machineprovider);

	std::unordered_map<std::string, ::nevonex::feature::AbstractMachineProvider_ptr> machineprovidermap;
};

}
#endif /* NEVONEX_ABSTRACT_APPLICATION_HPP_ */
