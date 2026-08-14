/*
 * nevonex/fcal/RAW_CAN_DATA_DLC_8_W.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "RAW_CAN_DATA_DLC_8_W.hpp"
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

/*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcal;
using namespace ::nevonex::log;

// Default constructor
RAW_CAN_DATA_DLC_8_W::RAW_CAN_DATA_DLC_8_W() : m_byte0(0), m_byte1(0), m_byte2(0), m_byte3(0), m_byte4(0), m_byte5(0), m_byte6(0), m_byte7(0)
{

    /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W__RAW_CAN_DATA_DLC_8_W) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

RAW_CAN_DATA_DLC_8_W::~RAW_CAN_DATA_DLC_8_W()
{

    /*PROTECTED REGION ID(RAW_CAN_DATA_DLC_8_W__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

void RAW_CAN_DATA_DLC_8_W::initialize(::ecore::EInt _byte0, ::ecore::EInt _byte1, ::ecore::EInt _byte2, ::ecore::EInt _byte3, ::ecore::EInt _byte4, ::ecore::EInt _byte5, ::ecore::EInt _byte6, ::ecore::EInt _byte7)
{
    m_byte0 = _byte0;
    m_byte1 = _byte1;
    m_byte2 = _byte2;
    m_byte3 = _byte3;
    m_byte4 = _byte4;
    m_byte5 = _byte5;
    m_byte6 = _byte6;
    m_byte7 = _byte7;
}

// Attributes

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte0() const
{
    return m_byte0;
}

void RAW_CAN_DATA_DLC_8_W::setByte0(::ecore::EInt _byte0)
{

    ::ecore::EInt _old_byte0 = m_byte0;
    m_byte0 = _byte0;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte1() const
{
    return m_byte1;
}

void RAW_CAN_DATA_DLC_8_W::setByte1(::ecore::EInt _byte1)
{

    ::ecore::EInt _old_byte1 = m_byte1;
    m_byte1 = _byte1;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte2() const
{
    return m_byte2;
}

void RAW_CAN_DATA_DLC_8_W::setByte2(::ecore::EInt _byte2)
{

    ::ecore::EInt _old_byte2 = m_byte2;
    m_byte2 = _byte2;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte3() const
{
    return m_byte3;
}

void RAW_CAN_DATA_DLC_8_W::setByte3(::ecore::EInt _byte3)
{

    ::ecore::EInt _old_byte3 = m_byte3;
    m_byte3 = _byte3;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte4() const
{
    return m_byte4;
}

void RAW_CAN_DATA_DLC_8_W::setByte4(::ecore::EInt _byte4)
{

    ::ecore::EInt _old_byte4 = m_byte4;
    m_byte4 = _byte4;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte5() const
{
    return m_byte5;
}

void RAW_CAN_DATA_DLC_8_W::setByte5(::ecore::EInt _byte5)
{

    ::ecore::EInt _old_byte5 = m_byte5;
    m_byte5 = _byte5;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte6() const
{
    return m_byte6;
}

void RAW_CAN_DATA_DLC_8_W::setByte6(::ecore::EInt _byte6)
{

    ::ecore::EInt _old_byte6 = m_byte6;
    m_byte6 = _byte6;

}

::ecore::EInt RAW_CAN_DATA_DLC_8_W::getByte7() const
{
    return m_byte7;
}

void RAW_CAN_DATA_DLC_8_W::setByte7(::ecore::EInt _byte7)
{

    ::ecore::EInt _old_byte7 = m_byte7;
    m_byte7 = _byte7;

}

// References

