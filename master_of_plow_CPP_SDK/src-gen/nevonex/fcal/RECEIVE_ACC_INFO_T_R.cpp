/*
 * nevonex/fcal/RECEIVE_ACC_INFO_T_R.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "RECEIVE_ACC_INFO_T_R.hpp"
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

/*PROTECTED REGION ID(RECEIVE_ACC_INFO_T_R.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
RECEIVE_ACC_INFO_T_R::RECEIVE_ACC_INFO_T_R() : m_tRZ_ACC_SIG1_V(0.0f), m_tRZ_ACC_SIG2_V(0.0f), m_tRZ_ACC_DIAG(0), m_tRZ_ACC_AUTO(0)
{

    /*PROTECTED REGION ID(RECEIVE_ACC_INFO_T_R__RECEIVE_ACC_INFO_T_R) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

RECEIVE_ACC_INFO_T_R::~RECEIVE_ACC_INFO_T_R()
{

    /*PROTECTED REGION ID(RECEIVE_ACC_INFO_T_R__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EFloat RECEIVE_ACC_INFO_T_R::getTRZ_ACC_SIG1_V() const
{
    return m_tRZ_ACC_SIG1_V;
}

void RECEIVE_ACC_INFO_T_R::setTRZ_ACC_SIG1_V(::ecore::EFloat _tRZ_ACC_SIG1_V)
{

    ::ecore::EFloat _old_tRZ_ACC_SIG1_V = m_tRZ_ACC_SIG1_V;
    m_tRZ_ACC_SIG1_V = _tRZ_ACC_SIG1_V;

}

::ecore::EFloat RECEIVE_ACC_INFO_T_R::getTRZ_ACC_SIG2_V() const
{
    return m_tRZ_ACC_SIG2_V;
}

void RECEIVE_ACC_INFO_T_R::setTRZ_ACC_SIG2_V(::ecore::EFloat _tRZ_ACC_SIG2_V)
{

    ::ecore::EFloat _old_tRZ_ACC_SIG2_V = m_tRZ_ACC_SIG2_V;
    m_tRZ_ACC_SIG2_V = _tRZ_ACC_SIG2_V;

}

::ecore::EInt RECEIVE_ACC_INFO_T_R::getTRZ_ACC_DIAG() const
{
    return m_tRZ_ACC_DIAG;
}

void RECEIVE_ACC_INFO_T_R::setTRZ_ACC_DIAG(::ecore::EInt _tRZ_ACC_DIAG)
{

    ::ecore::EInt _old_tRZ_ACC_DIAG = m_tRZ_ACC_DIAG;
    m_tRZ_ACC_DIAG = _tRZ_ACC_DIAG;

}

::ecore::EInt RECEIVE_ACC_INFO_T_R::getTRZ_ACC_AUTO() const
{
    return m_tRZ_ACC_AUTO;
}

void RECEIVE_ACC_INFO_T_R::setTRZ_ACC_AUTO(::ecore::EInt _tRZ_ACC_AUTO)
{

    ::ecore::EInt _old_tRZ_ACC_AUTO = m_tRZ_ACC_AUTO;
    m_tRZ_ACC_AUTO = _tRZ_ACC_AUTO;

}

// References

