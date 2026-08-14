/*
 * nevonex/fcal/RECEIVE_SFT_INFO_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "RECEIVE_SFT_INFO_T_R.hpp"
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

/*PROTECTED REGION ID(RECEIVE_SFT_INFO_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
RECEIVE_SFT_INFO_T_R::RECEIVE_SFT_INFO_T_R() : m_tRZ_SFT_SIG1_V(0.0f), m_tRZ_SFT_SIG2_V(0.0f), m_tRZ_SFT_DIAG(0), m_tRZ_SFT_AUTO(0), m_tRZ_SFT_STATE(0)
{

    /*PROTECTED REGION ID(RECEIVE_SFT_INFO_T_R__RECEIVE_SFT_INFO_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

RECEIVE_SFT_INFO_T_R::~RECEIVE_SFT_INFO_T_R()
{

    /*PROTECTED REGION ID(RECEIVE_SFT_INFO_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EFloat RECEIVE_SFT_INFO_T_R::getTRZ_SFT_SIG1_V() const
{
    return m_tRZ_SFT_SIG1_V;
}

void RECEIVE_SFT_INFO_T_R::setTRZ_SFT_SIG1_V(::ecore::EFloat _tRZ_SFT_SIG1_V)
{

    ::ecore::EFloat _old_tRZ_SFT_SIG1_V = m_tRZ_SFT_SIG1_V;
    m_tRZ_SFT_SIG1_V = _tRZ_SFT_SIG1_V;

}

::ecore::EFloat RECEIVE_SFT_INFO_T_R::getTRZ_SFT_SIG2_V() const
{
    return m_tRZ_SFT_SIG2_V;
}

void RECEIVE_SFT_INFO_T_R::setTRZ_SFT_SIG2_V(::ecore::EFloat _tRZ_SFT_SIG2_V)
{

    ::ecore::EFloat _old_tRZ_SFT_SIG2_V = m_tRZ_SFT_SIG2_V;
    m_tRZ_SFT_SIG2_V = _tRZ_SFT_SIG2_V;

}

::ecore::EInt RECEIVE_SFT_INFO_T_R::getTRZ_SFT_DIAG() const
{
    return m_tRZ_SFT_DIAG;
}

void RECEIVE_SFT_INFO_T_R::setTRZ_SFT_DIAG(::ecore::EInt _tRZ_SFT_DIAG)
{

    ::ecore::EInt _old_tRZ_SFT_DIAG = m_tRZ_SFT_DIAG;
    m_tRZ_SFT_DIAG = _tRZ_SFT_DIAG;

}

::ecore::EInt RECEIVE_SFT_INFO_T_R::getTRZ_SFT_AUTO() const
{
    return m_tRZ_SFT_AUTO;
}

void RECEIVE_SFT_INFO_T_R::setTRZ_SFT_AUTO(::ecore::EInt _tRZ_SFT_AUTO)
{

    ::ecore::EInt _old_tRZ_SFT_AUTO = m_tRZ_SFT_AUTO;
    m_tRZ_SFT_AUTO = _tRZ_SFT_AUTO;

}

::ecore::EInt RECEIVE_SFT_INFO_T_R::getTRZ_SFT_STATE() const
{
    return m_tRZ_SFT_STATE;
}

void RECEIVE_SFT_INFO_T_R::setTRZ_SFT_STATE(::ecore::EInt _tRZ_SFT_STATE)
{

    ::ecore::EInt _old_tRZ_SFT_STATE = m_tRZ_SFT_STATE;
    m_tRZ_SFT_STATE = _tRZ_SFT_STATE;

}

// References

