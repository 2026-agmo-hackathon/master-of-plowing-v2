/*
 * nevonex/common/HMIServicesEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_HMISERVICESENUM_HPP
#define NEVONEX_COMMON_HMISERVICESENUM_HPP

#include <string>

namespace nevonex
{
    namespace common
    {

        enum class HMIServicesEnum
        {
             CUSTOMUI = 0 /* CUSTOMUI*/
        };
// HMIServicesEnum Values array.
        static const HMIServicesEnum HMIServicesEnum_Values[] =
        { HMIServicesEnum::CUSTOMUI };
        inline const std::string getLiteral(HMIServicesEnum e)
        {
            switch (e)
            {
            case HMIServicesEnum::CUSTOMUI:
                return "CUSTOMUI";
            }
            return "";
        }
        ;
    } // common
} // nevonex

#endif
