/*
 * nevonex/fcb/FCALController.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "FCALController.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/fcb/FcbPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <algorithm>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <rapidjson/document.h>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EClassifier.hpp>
#include <ecore/EObject.hpp>
#include <nevonex/fcb/PublishConnectionFactory.hpp>
#include <nevonex/fcb/SubscriberEnum.hpp>
#include <vector>
#include <string>

#if BOOST_VERSION >= 106501
#define BOOST_STACKTRACE_HEADERS_FOUND
#include <boost/stacktrace.hpp>
#endif

/*PROTECTED REGION ID(FCALController.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::fcb;
using namespace ::nevonex::log;

// Default constructor
FCALController::FCALController()
{

    /*PROTECTED REGION ID(FCALController__FCALController) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

FCALController::~FCALController()
{

    /*PROTECTED REGION ID(FCALController__Destructor) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/

}

// Attributes

// References

// FCALController framework operations

void FCALController::publish(const ::ecore::EString& _interfaceKey,
    const ::ecore::EString& _interfaceName,
    const std::vector< ::ecore::EObject_ptr >& _payload,
    ::nevonex::fcb::SubscriberEnum _subscriber)
{
    ::nevonex::fcb::PublishConnectionFactory::getInstance()->publish(
        _interfaceKey, _interfaceName, _payload, _subscriber);
}

void FCALController::setValue(::ecore::EObject_ptr _obj,
    ::ecore::EStructuralFeature_ptr _feature,
    const rapidjson::Document& _doc)
{
    auto _value = jsonToETypeObject(_obj, _feature, _doc);
    _obj->eSet(_feature, _value);
}

void FCALController::setMachineUpdate(::ecore::EObject_ptr /*_machine*/,
    const ::ecore::EString& /*_attribute*/,
    const ::ecore::EJavaObject& /*_payload*/,
    const ::ecore::EJavaObject& /*_details*/,
    ::ecore::ELong /*_timestamp*/)
{
}

::ecore::EJavaObject FCALController::jsonToETypeObject(::ecore::EObject_ptr /*_obj*/,
    ::ecore::EStructuralFeature_ptr _feature,
    const rapidjson::Document& _doc)
{
    ::ecore::EJavaObject _result;
    auto _eType = _feature->getEType();
    if (!_eType) return _result;
    const auto& _typeName = _eType->getName();
    if (_typeName == "EString" && _doc.IsString())
    {
        _result = ::ecore::EString(_doc.GetString());
    }
    else if (_typeName == "EInt" && _doc.IsInt())
    {
        _result = static_cast< ::ecore::EInt >(_doc.GetInt());
    }
    else if (_typeName == "EDouble" && _doc.IsDouble())
    {
        _result = static_cast< ::ecore::EDouble >(_doc.GetDouble());
    }
    else if (_typeName == "EBoolean" && _doc.IsBool())
    {
        _result = static_cast< ::ecore::EBoolean >(_doc.GetBool());
    }
    else if (_typeName == "ELong" && _doc.IsInt64())
    {
        _result = static_cast< ::ecore::ELong >(_doc.GetInt64());
    }
    else if (_typeName == "EFloat" && _doc.IsFloat())
    {
        _result = static_cast< ::ecore::EFloat >(_doc.GetFloat());
    }
    return _result;
}

