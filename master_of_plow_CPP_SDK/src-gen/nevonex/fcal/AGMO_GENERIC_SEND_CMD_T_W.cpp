/*
 * nevonex/fcal/AGMO_GENERIC_SEND_CMD_T_W.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "AGMO_GENERIC_SEND_CMD_T_W.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcal/FcalPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
AGMO_GENERIC_SEND_CMD_T_W::AGMO_GENERIC_SEND_CMD_T_W() : m_aD_Generic_Cmd(0), m_aD_Generic_Mode_Cmd(0)
{

    /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W__AGMO_GENERIC_SEND_CMD_T_W) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

AGMO_GENERIC_SEND_CMD_T_W::~AGMO_GENERIC_SEND_CMD_T_W()
{

    /*PROTECTED REGION ID(AGMO_GENERIC_SEND_CMD_T_W__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

void AGMO_GENERIC_SEND_CMD_T_W::initialize(::ecore::EInt _aD_Generic_Cmd, ::ecore::EInt _aD_Generic_Mode_Cmd)
{
    m_aD_Generic_Cmd = _aD_Generic_Cmd;
    m_aD_Generic_Mode_Cmd = _aD_Generic_Mode_Cmd;
}

// Attributes

::ecore::EInt AGMO_GENERIC_SEND_CMD_T_W::getAD_Generic_Cmd() const
{
    return m_aD_Generic_Cmd;
}

void AGMO_GENERIC_SEND_CMD_T_W::setAD_Generic_Cmd(::ecore::EInt _aD_Generic_Cmd)
{

    ::ecore::EInt _old_aD_Generic_Cmd = m_aD_Generic_Cmd;
    m_aD_Generic_Cmd = _aD_Generic_Cmd;

}

::ecore::EInt AGMO_GENERIC_SEND_CMD_T_W::getAD_Generic_Mode_Cmd() const
{
    return m_aD_Generic_Mode_Cmd;
}

void AGMO_GENERIC_SEND_CMD_T_W::setAD_Generic_Mode_Cmd(::ecore::EInt _aD_Generic_Mode_Cmd)
{

    ::ecore::EInt _old_aD_Generic_Mode_Cmd = m_aD_Generic_Mode_Cmd;
    m_aD_Generic_Mode_Cmd = _aD_Generic_Mode_Cmd;

}

// References

