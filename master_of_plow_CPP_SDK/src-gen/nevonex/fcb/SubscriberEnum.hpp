/*
 * nevonex/fcb/SubscriberEnum.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef NEVONEX_FCB_SUBSCRIBERENUM_HPP
#define NEVONEX_FCB_SUBSCRIBERENUM_HPP

#include <string>

namespace nevonex
{
    namespace fcb
    {

        enum class SubscriberEnum
        {
             FIL = 0 /* FIL*/
            ,FGF = 1 /* FGF*/
            ,ALL = 2 /* ALL*/
        };
// SubscriberEnum Values array.
        static const SubscriberEnum SubscriberEnum_Values[] =
        { SubscriberEnum::FIL, SubscriberEnum::FGF, SubscriberEnum::ALL };
        inline const std::string getLiteral(SubscriberEnum e)
        {
            switch (e)
            {
            case SubscriberEnum::FIL:
                return "FIL";
            case SubscriberEnum::FGF:
                return "FGF";
            case SubscriberEnum::ALL:
                return "ALL";
            }
            return "";
        }
        ;
    } // fcb
} // nevonex

#endif
