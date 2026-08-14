/*
 * nevonex/common/PlatformServicesEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_COMMON_PLATFORMSERVICESENUM_HPP
#define NEVONEX_COMMON_PLATFORMSERVICESENUM_HPP

#include <string>

namespace nevonex
{
    namespace common
    {

        enum class PlatformServicesEnum
        {
             Cloud_Download = 0 /* Cloud_Download*/
            ,Cloud_Upload = 1 /* Cloud_Upload*/
        };
// PlatformServicesEnum Values array.
        static const PlatformServicesEnum PlatformServicesEnum_Values[] =
        { PlatformServicesEnum::Cloud_Download, PlatformServicesEnum::Cloud_Upload };
        inline const std::string getLiteral(PlatformServicesEnum e)
        {
            switch (e)
            {
            case PlatformServicesEnum::Cloud_Download:
                return "Cloud_Download";
            case PlatformServicesEnum::Cloud_Upload:
                return "Cloud_Upload";
            }
            return "";
        }
        ;
    } // common
} // nevonex

#endif
