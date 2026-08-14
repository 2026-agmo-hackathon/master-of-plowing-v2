/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 *
 * @Project Name  : NEVONEX
 * @File Name     : NtripConnectionHandlerCallback.hpp
 * @Author        : Adhith Gopal
 * @Created       : 2025
 * @Last modified : 
 * @Description   : This is the callback interface that should be implemented and provided as input to NtripConnectionHandler::enableNtrip 
 * by the NTRIP app users.
 */
#ifndef F344AA95_414E_4D71_8586_A11807C84FE8
#define F344AA95_414E_4D71_8586_A11807C84FE8

#include <string>
#include "NtripCommon.hpp"

namespace nevonex::ntrip
{
    /**
     * @brief This is the callback interface that should be implemented and provided as input to NtripConnectionHandler::enableNtrip 
     * by the NTRIP app users. The NtripConnectionHandler would call the respective methods in an async manner to update the status
     * of NTRIP caster connection whenever a change in connection is noticed.
     * 
     */
    class NtripConnectionHandlerCallback
    {
    public:
        NtripConnectionHandlerCallback() = default;
        virtual ~NtripConnectionHandlerCallback();

        virtual void connectionStatusUpdate(NTRIP_CONNECTION_ENUM status, std::string error) = 0;
    };
};

#endif /* F344AA95_414E_4D71_8586_A11807C84FE8 */
