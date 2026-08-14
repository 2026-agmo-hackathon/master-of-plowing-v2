/*
 * AppMain/IgnitionStateListener.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_IGNITIONSTATELISTENER_HPP
#define APPMAIN_IGNITIONSTATELISTENER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

#include <nevonex-fcal-platform/notify/AbstractIgnitionStateNotification.hpp>

namespace AppMain
{

    class IgnitionStateListener: public virtual ::ecore::EObject,
            public ::nevonex::notify::AbstractIgnitionStateNotification
    {
        /*PROTECTED REGION ID(IgnitionStateListener_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        IgnitionStateListener();

        virtual ~IgnitionStateListener();

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

        /*PROTECTED REGION ID(IgnitionStateListener) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    protected:

        /**
         * \brief For Ignition On Feature.
         */
        virtual void handleIgnitionOn() override;

        /**
         * \brief For Ignition Off Feature.
         */
        virtual void handleIgnitionOff() override;

    protected:
        IgnitionStateListener_ptr _this()
        {
            return IgnitionStateListener_ptr(this);
        }

    private:
        // Attributes

        // References

        ::AppMain::MainController_ptr m_mainController;

        /*PROTECTED REGION ID(IgnitionStateListener_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_IGNITIONSTATELISTENER_HPP
