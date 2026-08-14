/*
 * nevonex/isopgn/IsopgnPackageImpl.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include <nevonex/isopgn/IsopgnPackage.hpp>
#include <nevonex/isopgn/IsopgnFactory.hpp>
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
#include <ecore/EcorePackage.hpp>
#include <nevonex/common/CommonPackage.hpp>

using namespace ::nevonex::isopgn;

IsopgnPackage::IsopgnPackage()
{

    // Feature definitions of ISOPGN
    m_ISOPGN__hitchPosPerRe = ::ecore::Ptr < ::ecore::EAttribute
            > (new ::ecore::EAttribute);

    // Feature definitions of ISOPGNProvider
    m_ISOPGNProvider__iSOPGN = ::ecore::Ptr < ::ecore::EReference
            > (new ::ecore::EReference);

}

void IsopgnPackage::_initPackage()
{
    // Factory
    {
        ::ecore::EFactory_ptr _fa = IsopgnFactory::_instance();
        setEFactoryInstance(_fa);
        _fa->setEPackage(_this());
    }

    // Create classes and their features

    // ISOPGN
    {
        m_ISOPGNEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ISOPGNEClass->setClassifierID(ISOPGN);
        m_ISOPGNEClass->setEPackage(_this());
        getEClassifiers().push_back(m_ISOPGNEClass);
    }
    // m_ISOPGN__hitchPosPerRe has already been allocated above
    m_ISOPGN__hitchPosPerRe->setFeatureID(
            ::nevonex::isopgn::IsopgnPackage::ISOPGN__HITCHPOSPERRE);
    m_ISOPGNEClass->getEStructuralFeatures().push_back(
            m_ISOPGN__hitchPosPerRe);

    // ISOPGNProvider
    {
        m_ISOPGNProviderEClass = ::ecore::Ptr < ::ecore::EClass
                > (new ::ecore::EClass);
        m_ISOPGNProviderEClass->setClassifierID(ISOPGNPROVIDER);
        m_ISOPGNProviderEClass->setEPackage(_this());
        getEClassifiers().push_back(m_ISOPGNProviderEClass);
    }
    // m_ISOPGNProvider__iSOPGN has already been allocated above
    m_ISOPGNProvider__iSOPGN->setFeatureID(
            ::nevonex::isopgn::IsopgnPackage::ISOPGNPROVIDER__ISOPGN);
    m_ISOPGNProviderEClass->getEStructuralFeatures().push_back(
            m_ISOPGNProvider__iSOPGN);

    // Create enums

    // Create data types

    // Initialize package
    setName("isopgn");
    setNsPrefix("isopgn");
    setNsURI("com.bosch.nevonex.isopgn");

    // TODO: bounds for type parameters

    // Add supertypes to classes
    m_ISOPGNEClass->getESuperTypes().push_back(
            dynamic_cast< ::nevonex::common::CommonPackage* >(::nevonex::common::CommonPackage::_instance().get())->getTopicObject());

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // ISOPGN
    m_ISOPGNEClass->setName("ISOPGN");
    m_ISOPGNEClass->setAbstract(false);
    m_ISOPGNEClass->setInterface(false);
    m_ISOPGN__hitchPosPerRe->setEType(
            dynamic_cast< ::ecore::EcorePackage* >(::ecore::EcorePackage::_instance().get())->getEFloat());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EAttribute > (m_ISOPGN__hitchPosPerRe),
            "hitchPosPerRe", true, false, false, "0.0", false, false, true, true, 0,
            1, false, false);
    m_ISOPGN__hitchPosPerRe->setID(false);
    // ISOPGNProvider
    m_ISOPGNProviderEClass->setName("ISOPGNProvider");
    m_ISOPGNProviderEClass->setAbstract(false);
    m_ISOPGNProviderEClass->setInterface(false);
    m_ISOPGNProvider__iSOPGN->setEType(
            dynamic_cast< ::nevonex::isopgn::IsopgnPackage* >(::nevonex::isopgn::IsopgnPackage::_instance().get())->getISOPGN());
    initEStructuralFeature(
            ::ecore::as < ::ecore::EReference > (m_ISOPGNProvider__iSOPGN),
            "iSOPGN", false, false, false, "", false, true, true, true, 0,
            1, false, false);

    // TODO: Initialize data types

    _initialize();
}

::ecore::EClass_ptr IsopgnPackage::getISOPGN()
{
    return m_ISOPGNEClass;
}
::ecore::EClass_ptr IsopgnPackage::getISOPGNProvider()
{
    return m_ISOPGNProviderEClass;
}

::ecore::EAttribute_ptr IsopgnPackage::getISOPGN__hitchPosPerRe()
{
    return m_ISOPGN__hitchPosPerRe;
}
::ecore::EReference_ptr IsopgnPackage::getISOPGNProvider__iSOPGN()
{
    return m_ISOPGNProvider__iSOPGN;
}
