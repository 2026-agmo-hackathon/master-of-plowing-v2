/*
 * nevonex/common/TopicObject.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "TopicObject.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/common/CommonPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(TopicObject.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::common;
using namespace ::nevonex::log;

// Default constructor
TopicObject::TopicObject() : m_index(0)
{

    /*PROTECTED REGION ID(TopicObject__TopicObject) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

TopicObject::~TopicObject()
{

    /*PROTECTED REGION ID(TopicObject__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

::ecore::EInt TopicObject::getIndex() const
{
    return m_index;
}

void TopicObject::setIndex(::ecore::EInt _index)
{

    ::ecore::EInt _old_index = m_index;
    m_index = _index;

}

// References

