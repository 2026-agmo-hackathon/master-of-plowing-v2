/*
 * nevonex/isopgn_forward.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _NEVONEX_ISOPGN_FORWARD_HPP
#define _NEVONEX_ISOPGN_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(nevonex_isopgn_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage

#include <ecore_forward.hpp> // for EDataTypes

namespace nevonex
{
    namespace isopgn
    {

// EDataType

// EClass

// ISOPGN
        class ISOPGN;
        using ISOPGN_ptr = ::ecore::Ptr<ISOPGN>;

// ISOPGNProvider
        class ISOPGNProvider;
        using ISOPGNProvider_ptr = ::ecore::Ptr<ISOPGNProvider>;

// EEnum

// Package & Factory
        class IsopgnFactory;
        using IsopgnFactory_ptr = ::ecore::Ptr<IsopgnFactory>;
        class IsopgnPackage;
        using IsopgnPackage_ptr = ::ecore::Ptr<IsopgnPackage>;

    } // isopgn
} // nevonex

#endif // _NEVONEX_ISOPGN_FORWARD_HPP
