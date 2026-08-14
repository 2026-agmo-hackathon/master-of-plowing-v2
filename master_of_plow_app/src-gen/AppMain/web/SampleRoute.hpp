/*
 * AppMain/web/SampleRoute.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_WEB_SAMPLEROUTE_HPP
#define APPMAIN_WEB_SAMPLEROUTE_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain/web_forward.hpp>

#include <AppMain_forward.hpp>

#include <nevonex-fcal-platform/web/server/NevonexRoute.hpp>

/*PROTECTED REGION ID(SampleRoute_additional_headers) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace AppMain
{
    namespace web
    {

        class SampleRoute: public virtual ::ecore::EObject,
                public ::nevonex::web::server::NevonexRoute
        {
            /*PROTECTED REGION ID(SampleRoute_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        public:
            SampleRoute();

            virtual ~SampleRoute();

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

            /*PROTECTED REGION ID(SampleRoute) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

        protected:
            SampleRoute_ptr _this()
            {
                return SampleRoute_ptr(this);
            }

        private:
            // Attributes

            // References

            ::AppMain::MainController_ptr m_mainController;

            /*PROTECTED REGION ID(SampleRoute_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
        };

    } // web
} // AppMain

#endif // APPMAIN_WEB_SAMPLEROUTE_HPP
