/*
 * nevonex/NevonexPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/NevonexPackage.hpp>
#include <nevonex/NevonexFactory.hpp>
#include <ecore.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EAttribute.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecore/EParameter.hpp>
#include <ecore/EEnum.hpp>
#include <ecore/EEnumLiteral.hpp>
#include <ecore/EDataType.hpp>
#include <ecore/EGenericType.hpp>
#include <ecore/ETypeParameter.hpp>

using namespace ::nevonex;

NevonexPackage::NevonexPackage()
{

}

void NevonexPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = NevonexFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // Create enums

    // Create data types

    getESubpackages().push_back(
            ::nevonex::common::CommonPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::exception::ExceptionPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::fcb::FcbPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::fcal::FcalPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::types::TypesPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::customui::CustomuiPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::cloud::CloudPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::can_agmo_customized_tractor::Can_agmo_customized_tractorPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::can_agmo_mtlt305::Can_agmo_mtlt305Package::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::can_agmo_steermotor::Can_agmo_steermotorPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::isopgn::IsopgnPackage::_getInstanceAndRemoveOwnership());
    getESubpackages().push_back(
            ::nevonex::serial_ext_gps_nmea0183::Serial_ext_gps_nmea0183Package::_getInstanceAndRemoveOwnership());

    // Initialize package
    setName("nevonex");
    setNsPrefix("nevonex");
    setNsURI("www.bosch.com/nevonex");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes

    // TODO: Initialize data types

    _initialize();
}
