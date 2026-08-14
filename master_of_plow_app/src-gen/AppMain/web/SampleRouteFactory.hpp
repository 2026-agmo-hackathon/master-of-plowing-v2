/*
 * AppMain/web/SampleRouteFactory.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_WEB_SAMPLEROUTEFACTORY_HPP
#define APPMAIN_WEB_SAMPLEROUTEFACTORY_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain/web_forward.hpp>

#include <AppMain_forward.hpp>

#include "SampleRoute.hpp"
#include <nevonex-fcal-platform/web/server/NevonexRouteFactory.hpp>

/*PROTECTED REGION ID(SampleRouteFactory_additional_headers) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace AppMain
{
    namespace web
    {

        class SampleRouteFactory: public virtual ::ecore::EObject,
                public ::nevonex::web::server::NevonexRouteFactory
        {
            /*PROTECTED REGION ID(SampleRouteFactory_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        public:
            SampleRouteFactory();

            virtual ~SampleRouteFactory();

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

            /*PROTECTED REGION ID(SampleRouteFactory) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        public:
            virtual ::nevonex::web::server::NevonexRoute* createRoute(
                    const Poco::Net::HTTPServerRequest &request) override;
            virtual void disconnect() override;

        protected:
            SampleRouteFactory_ptr _this()
            {
                return SampleRouteFactory_ptr(this);
            }

        private:
            // Attributes

            // References

            ::AppMain::MainController_ptr m_mainController;

            /*PROTECTED REGION ID(SampleRouteFactory_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
        };

    } // web
} // AppMain

#endif // APPMAIN_WEB_SAMPLEROUTEFACTORY_HPP
