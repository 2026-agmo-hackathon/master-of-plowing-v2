/*
 * nevonex/isopgn/IsopgnFactoryImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/isopgn/IsopgnFactory.hpp>
#include <nevonex/isopgn/IsopgnPackage.hpp>
#include <nevonex/isopgn/ISOPGN.hpp>
#include <nevonex/isopgn/ISOPGNProvider.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::nevonex::isopgn;

IsopgnFactory::IsopgnFactory()
{
}

::ecore::EObject_ptr IsopgnFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case IsopgnPackage::ISOPGN:
        return createISOPGN();
    case IsopgnPackage::ISOPGNPROVIDER:
        return createISOPGNProvider();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject IsopgnFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const &_literalValue)
{
    (void)_literalValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString IsopgnFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const &_instanceValue)
{
    (void)_instanceValue;
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

ISOPGN_ptr IsopgnFactory::createISOPGN()
{
    return ::ecore::Ptr < ISOPGN > (new ISOPGN);
}
ISOPGNProvider_ptr IsopgnFactory::createISOPGNProvider()
{
    return ::ecore::Ptr < ISOPGNProvider > (new ISOPGNProvider);
}
