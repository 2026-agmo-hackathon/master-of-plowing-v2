/*
 * AppMain/IController.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_ICONTROLLER_HPP
#define APPMAIN_ICONTROLLER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

namespace AppMain
{
    class ProcessTimer
    {
    public:
        ProcessTimer(IController_ptr controller,
                const int intervalInMilliSecond);
    };
} // AppMain

namespace AppMain
{

    class IController: public virtual ::ecore::EObject
    {
        /*PROTECTED REGION ID(IController_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        IController();

        virtual ~IController();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
    public:

        /*PROTECTED REGION ID(IController) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    protected:
        virtual void start(const int intervalInMilliSecond) = 0;
        virtual int nextProcessIntervalMs(const int configuredIntervalMs) const
        {
            return configuredIntervalMs;
        }
    private:
        friend class ProcessTimer;

    protected:
        IController_ptr _this()
        {
            return IController_ptr(this);
        }

    private:
        // Attributes

        // References

        /*PROTECTED REGION ID(IController_privateSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_ICONTROLLER_HPP
