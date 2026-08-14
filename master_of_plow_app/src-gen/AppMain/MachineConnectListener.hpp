/*
 * AppMain/MachineConnectListener.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_MACHINECONNECTLISTENER_HPP
#define APPMAIN_MACHINECONNECTLISTENER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

#include <nevonex/machine/MachineConnectionInfo.hpp>
#include <nevonex/application/machine/AbstractMachineConnectionListener.hpp>

namespace AppMain
{

    class MachineConnectListener: public virtual ::ecore::EObject,
            public ::nevonex::app::machine::AbstractMachineConnectionListener
    {
        /*PROTECTED REGION ID(MachineConnectListener_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        MachineConnectListener();

        virtual ~MachineConnectListener();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
        /**
         * \brief 
         */
    public:
        virtual ::AppMain::MainController_ptr getMainController() const;
        /**
         * \brief 
         */
    public:
        virtual void setMainController(
                ::AppMain::MainController_ptr _mainController);

    public:

        /*PROTECTED REGION ID(MachineConnectListener) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    protected:
        virtual void machineConnected(
                ::nevonex::feature::AbstractMachine_ptr machine) override;
        virtual void machineDisconnected(
                ::nevonex::feature::AbstractMachine_ptr machine,
                ::nevonex::machine::MachineConnectionInfo_ptr machineConnectionInfo)
                        override;

    private:
        virtual void propertyChange(
                ::nevonex::propertychange::PropertyChangeEvent<
                        const ::ecore::EJavaObject &,
                        const ::ecore::EJavaObject & > changeEvent) override;

    protected:
        MachineConnectListener_ptr _this()
        {
            return MachineConnectListener_ptr(this);
        }

    private:
        // Attributes

        // References

        ::AppMain::MainController_ptr m_mainController;

        /*PROTECTED REGION ID(MachineConnectListener_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_MACHINECONNECTLISTENER_HPP
