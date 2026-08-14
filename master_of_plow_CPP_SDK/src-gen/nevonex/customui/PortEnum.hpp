/*
 * nevonex/customui/PortEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_CUSTOMUI_PORTENUM_HPP
#define NEVONEX_CUSTOMUI_PORTENUM_HPP

#include <string>

namespace nevonex
{
    namespace customui
    {

        enum class PortEnum
        {
             HTTP = 8080 /* HTTP*/
            ,WS = 8081 /* WS*/
        };
// PortEnum Values array.
        static const PortEnum PortEnum_Values[] =
        { PortEnum::HTTP, PortEnum::WS };
        inline const std::string getLiteral(PortEnum e)
        {
            switch (e)
            {
            case PortEnum::HTTP:
                return "HTTP";
            case PortEnum::WS:
                return "WS";
            }
            return "";
        }
        ;
    } // customui
} // nevonex

#endif
