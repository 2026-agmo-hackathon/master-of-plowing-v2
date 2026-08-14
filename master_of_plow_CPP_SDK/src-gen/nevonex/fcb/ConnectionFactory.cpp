/*
 * nevonex/fcb/ConnectionFactory.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "ConnectionFactory.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcb/FcbPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(ConnectionFactory.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcb;
using namespace ::nevonex::log;

// Default constructor
ConnectionFactory::ConnectionFactory()
{

    /*PROTECTED REGION ID(ConnectionFactory__ConnectionFactory) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

ConnectionFactory::~ConnectionFactory()
{

    /*PROTECTED REGION ID(ConnectionFactory__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

// References

