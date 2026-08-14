/*
 * AppMain/FeatureManagerListener.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_FEATUREMANAGERLISTENER_HPP
#define APPMAIN_FEATUREMANAGERLISTENER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

#include <nevonex/application/feature/AbstractFeatureManagerListener.hpp>

namespace AppMain
{

    class FeatureManagerListener: public virtual ::ecore::EObject,
            public ::nevonex::app::feature::AbstractFeatureManagerListener
    {
        /*PROTECTED REGION ID(FeatureManagerListener_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        FeatureManagerListener();

        virtual ~FeatureManagerListener();

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

        /*PROTECTED REGION ID(FeatureManagerListener) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    protected:

        /**
         * \brief For Starting Feature.
         */
        virtual void handleFeatureStart(const std::string &message) override;

        /**
         * \brief For Stoping Feature.
         */
        virtual void handleFeatureStop(const std::string &message) override;

    protected:
        FeatureManagerListener_ptr _this()
        {
            return FeatureManagerListener_ptr(this);
        }

    private:
        // Attributes

        // References

        ::AppMain::MainController_ptr m_mainController;

        /*PROTECTED REGION ID(FeatureManagerListener_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_FEATUREMANAGERLISTENER_HPP
