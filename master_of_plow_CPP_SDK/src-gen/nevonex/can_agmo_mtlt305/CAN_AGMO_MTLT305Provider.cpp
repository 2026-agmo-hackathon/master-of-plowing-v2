/*
 * nevonex/can_agmo_mtlt305/CAN_AGMO_MTLT305Provider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_MTLT305Provider.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_mtlt305/Can_agmo_mtlt305Package.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <sstream>

/*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_mtlt305;
using namespace ::nevonex::log;

CAN_AGMO_MTLT305Provider::CAN_AGMO_MTLT305Provider() : m_cAN_AGMO_MTLT305(0)
{
    _initialize();

    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__CAN_AGMO_MTLT305Provider) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_MTLT305Provider::~CAN_AGMO_MTLT305Provider()
{
    m_cAN_AGMO_MTLT305.reset();
}

// References
::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305_ptr CAN_AGMO_MTLT305Provider::getCAN_AGMO_MTLT305() const
{
    return m_cAN_AGMO_MTLT305;
}

void CAN_AGMO_MTLT305Provider::setCAN_AGMO_MTLT305(::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305_ptr _cAN_AGMO_MTLT305)
{
    if (m_cAN_AGMO_MTLT305)
        m_cAN_AGMO_MTLT305->_setEContainer(CAN_AGMO_MTLT305Provider_ptr(),
                Can_agmo_mtlt305Package::_instance()->getCAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305());
    if (_cAN_AGMO_MTLT305)
        _cAN_AGMO_MTLT305->_setEContainer(_this(),
                Can_agmo_mtlt305Package::_instance()->getCAN_AGMO_MTLT305Provider__cAN_AGMO_MTLT305());
    m_cAN_AGMO_MTLT305 = _cAN_AGMO_MTLT305;
}

// Operations
void CAN_AGMO_MTLT305Provider::createMachines(std::istream & _stream)
{
    bool _newMachineCreated = false;
    if (!m_cAN_AGMO_MTLT305)
    {
        setCAN_AGMO_MTLT305(::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305_ptr(new ::nevonex::can_agmo_mtlt305::CAN_AGMO_MTLT305()));
        _newMachineCreated = true;
    }
    auto _machineObj = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_MTLT305);
    if (_machineObj)
    {
        constructChildTypes(::ecore::EJavaObject(), _machineObj, "CAN_AGMO_MTLT305");
        _machineObj->_initialize();
    }
    if (_newMachineCreated)
    {
        notifyPropertyChange("root", nullptr, m_cAN_AGMO_MTLT305);
    }
    (void)_stream;
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__createMachines) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
    (void)_parent;
    if (!_parentObj) return;
    auto _topicObject = ::ecore::as < ::nevonex::common::TopicObject > (_parentObj);
    if (!_topicObject) return;
    ::ecore::EString _machinePath = _path;
    if (_machinePath.empty())
    {
        _machinePath = "CAN_AGMO_MTLT305";
    }
    m_topicObjectMap[_machinePath] = _topicObject;
    m_topicObjectMap["CAN_AGMO_MTLT305"] = _topicObject;
    m_topicObjectMap["can_agmo_mtlt305"] = _topicObject;
    m_topicObjectMap["can_agmo_mtlt305/0"] = _topicObject;
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__constructChildTypes) START*/
    /*PROTECTED REGION END*/
}

::nevonex::common::TopicObject_ptr CAN_AGMO_MTLT305Provider::getTopicElement(::ecore::EString const& _index)
{
    if (!m_cAN_AGMO_MTLT305)
    {
        std::istringstream _emptyStream;
        createMachines(_emptyStream);
    }
    auto _it = m_topicObjectMap.find(_index);
    if (_it != m_topicObjectMap.end())
        return _it->second;
    if (boost::iequals(_index, "CAN_AGMO_MTLT305")
            || boost::iequals(_index, "can_agmo_mtlt305")
            || boost::iequals(_index, "can_agmo_mtlt305/0"))
    {
        return ::ecore::as < ::nevonex::common::TopicObject > (m_cAN_AGMO_MTLT305);
    }
    return nullptr;
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__getTopicElement) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::initMachineProvider()
{
    ::nevonex::types::IMachineProvider::initMachineProvider();
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__initMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::stopMachineProvider()
{
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__stopMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::start()
{
    ::nevonex::types::IMachineProvider::start();
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__start) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::stop()
{
    ::nevonex::types::IMachineProvider::stop();
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__stop) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::restart()
{
    ::nevonex::types::IMachineProvider::restart();
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__restart) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp)
{
    if (_topic == "CAN_AGMO_MTLT305.Aceinna_Accel.sub" || _topic == "/9440")
    {
        if (!m_cAN_AGMO_MTLT305)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_MTLT305->updateInterfaceDetail(9440, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0 && _jsonPlData[0].IsObject())
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject() && !(_item->HasMember("accY") || _item->HasMember("accy") || _item->HasMember("ACCY") || _item->HasMember("accX") || _item->HasMember("accx") || _item->HasMember("ACCX") || _item->HasMember("accZ") || _item->HasMember("accz") || _item->HasMember("ACCZ") || _item->HasMember("lateralAcc_FigureOfMerit") || _item->HasMember("lateralacc_figureofmerit") || _item->HasMember("LATERALACC_FIGUREOFMERIT") || _item->HasMember("longiAcc_FigureOfMerit") || _item->HasMember("longiacc_figureofmerit") || _item->HasMember("LONGIACC_FIGUREOFMERIT") || _item->HasMember("verticAcc_FigureOfMerit") || _item->HasMember("verticacc_figureofmerit") || _item->HasMember("VERTICACC_FIGUREOFMERIT") || _item->HasMember("support_Rate_Acc") || _item->HasMember("support_rate_acc") || _item->HasMember("SUPPORT_RATE_ACC")))
        {
            if (_item->HasMember("aceinna_Accel"))
            {
                _item = &(*_item)["aceinna_Accel"];
            }
            else if (_item->HasMember("Aceinna_Accel"))
            {
                _item = &(*_item)["Aceinna_Accel"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("accY") || _item->HasMember("accy") || _item->HasMember("ACCY") || _item->HasMember("accX") || _item->HasMember("accx") || _item->HasMember("ACCX") || _item->HasMember("accZ") || _item->HasMember("accz") || _item->HasMember("ACCZ") || _item->HasMember("lateralAcc_FigureOfMerit") || _item->HasMember("lateralacc_figureofmerit") || _item->HasMember("LATERALACC_FIGUREOFMERIT") || _item->HasMember("longiAcc_FigureOfMerit") || _item->HasMember("longiacc_figureofmerit") || _item->HasMember("LONGIACC_FIGUREOFMERIT") || _item->HasMember("verticAcc_FigureOfMerit") || _item->HasMember("verticacc_figureofmerit") || _item->HasMember("VERTICACC_FIGUREOFMERIT") || _item->HasMember("support_Rate_Acc") || _item->HasMember("support_rate_acc") || _item->HasMember("SUPPORT_RATE_ACC")))
        {
            if (_item->HasMember("aceinna_Accel"))
            {
                _item = &(*_item)["aceinna_Accel"];
            }
            else if (_item->HasMember("Aceinna_Accel"))
            {
                _item = &(*_item)["Aceinna_Accel"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::ACEINNA_ACCEL_T_R > (new ::nevonex::fcal::ACEINNA_ACCEL_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _accYField = nullptr;
        if ((*_item).HasMember("accY"))
        {
            _accYField = &(*_item)["accY"];
        }
        else if ((*_item).HasMember("accy"))
        {
            _accYField = &(*_item)["accy"];
        }
        else if ((*_item).HasMember("ACCY"))
        {
            _accYField = &(*_item)["ACCY"];
        }
        if (_accYField && _accYField->IsNumber())
        {
            _value->setAccY(static_cast< ::ecore::EFloat >(_accYField->GetFloat()));
        }
        const rapidjson::Value* _accXField = nullptr;
        if ((*_item).HasMember("accX"))
        {
            _accXField = &(*_item)["accX"];
        }
        else if ((*_item).HasMember("accx"))
        {
            _accXField = &(*_item)["accx"];
        }
        else if ((*_item).HasMember("ACCX"))
        {
            _accXField = &(*_item)["ACCX"];
        }
        if (_accXField && _accXField->IsNumber())
        {
            _value->setAccX(static_cast< ::ecore::EFloat >(_accXField->GetFloat()));
        }
        const rapidjson::Value* _accZField = nullptr;
        if ((*_item).HasMember("accZ"))
        {
            _accZField = &(*_item)["accZ"];
        }
        else if ((*_item).HasMember("accz"))
        {
            _accZField = &(*_item)["accz"];
        }
        else if ((*_item).HasMember("ACCZ"))
        {
            _accZField = &(*_item)["ACCZ"];
        }
        if (_accZField && _accZField->IsNumber())
        {
            _value->setAccZ(static_cast< ::ecore::EFloat >(_accZField->GetFloat()));
        }
        const rapidjson::Value* _lateralAcc_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("lateralAcc_FigureOfMerit"))
        {
            _lateralAcc_FigureOfMeritField = &(*_item)["lateralAcc_FigureOfMerit"];
        }
        else if ((*_item).HasMember("lateralacc_figureofmerit"))
        {
            _lateralAcc_FigureOfMeritField = &(*_item)["lateralacc_figureofmerit"];
        }
        else if ((*_item).HasMember("LATERALACC_FIGUREOFMERIT"))
        {
            _lateralAcc_FigureOfMeritField = &(*_item)["LATERALACC_FIGUREOFMERIT"];
        }
        if (_lateralAcc_FigureOfMeritField && _lateralAcc_FigureOfMeritField->IsNumber())
        {
            _value->setLateralAcc_FigureOfMerit(static_cast< ::ecore::EInt >(_lateralAcc_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _longiAcc_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("longiAcc_FigureOfMerit"))
        {
            _longiAcc_FigureOfMeritField = &(*_item)["longiAcc_FigureOfMerit"];
        }
        else if ((*_item).HasMember("longiacc_figureofmerit"))
        {
            _longiAcc_FigureOfMeritField = &(*_item)["longiacc_figureofmerit"];
        }
        else if ((*_item).HasMember("LONGIACC_FIGUREOFMERIT"))
        {
            _longiAcc_FigureOfMeritField = &(*_item)["LONGIACC_FIGUREOFMERIT"];
        }
        if (_longiAcc_FigureOfMeritField && _longiAcc_FigureOfMeritField->IsNumber())
        {
            _value->setLongiAcc_FigureOfMerit(static_cast< ::ecore::EInt >(_longiAcc_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _verticAcc_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("verticAcc_FigureOfMerit"))
        {
            _verticAcc_FigureOfMeritField = &(*_item)["verticAcc_FigureOfMerit"];
        }
        else if ((*_item).HasMember("verticacc_figureofmerit"))
        {
            _verticAcc_FigureOfMeritField = &(*_item)["verticacc_figureofmerit"];
        }
        else if ((*_item).HasMember("VERTICACC_FIGUREOFMERIT"))
        {
            _verticAcc_FigureOfMeritField = &(*_item)["VERTICACC_FIGUREOFMERIT"];
        }
        if (_verticAcc_FigureOfMeritField && _verticAcc_FigureOfMeritField->IsNumber())
        {
            _value->setVerticAcc_FigureOfMerit(static_cast< ::ecore::EInt >(_verticAcc_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _support_Rate_AccField = nullptr;
        if ((*_item).HasMember("support_Rate_Acc"))
        {
            _support_Rate_AccField = &(*_item)["support_Rate_Acc"];
        }
        else if ((*_item).HasMember("support_rate_acc"))
        {
            _support_Rate_AccField = &(*_item)["support_rate_acc"];
        }
        else if ((*_item).HasMember("SUPPORT_RATE_ACC"))
        {
            _support_Rate_AccField = &(*_item)["SUPPORT_RATE_ACC"];
        }
        if (_support_Rate_AccField && _support_Rate_AccField->IsNumber())
        {
            _value->setSupport_Rate_Acc(static_cast< ::ecore::EInt >(_support_Rate_AccField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setAccY(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setAccX(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setAccZ(static_cast< ::ecore::EFloat >((*_item)[2].GetFloat()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setLateralAcc_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setLongiAcc_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
            if ((*_item).Size() > 5 && (*_item)[5].IsNumber())
            {
                _value->setVerticAcc_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[5].GetDouble()));
            }
            if ((*_item).Size() > 6 && (*_item)[6].IsNumber())
            {
                _value->setSupport_Rate_Acc(static_cast< ::ecore::EInt >((*_item)[6].GetDouble()));
            }
        }
        m_cAN_AGMO_MTLT305->setAceinna_Accel(_value);
        return;
    }

    if (_topic == "CAN_AGMO_MTLT305.Aceinna_AngleRate.sub" || _topic == "/9441")
    {
        if (!m_cAN_AGMO_MTLT305)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_MTLT305->updateInterfaceDetail(9441, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0 && _jsonPlData[0].IsObject())
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject() && !(_item->HasMember("gyroY") || _item->HasMember("gyroy") || _item->HasMember("GYROY") || _item->HasMember("gyroX") || _item->HasMember("gyrox") || _item->HasMember("GYROX") || _item->HasMember("gyroZ") || _item->HasMember("gyroz") || _item->HasMember("GYROZ") || _item->HasMember("gyroY_FigureOfMerit") || _item->HasMember("gyroy_figureofmerit") || _item->HasMember("GYROY_FIGUREOFMERIT") || _item->HasMember("gyroX_FigureOfMerit") || _item->HasMember("gyrox_figureofmerit") || _item->HasMember("GYROX_FIGUREOFMERIT") || _item->HasMember("gyroZ_FigureOfMerit") || _item->HasMember("gyroz_figureofmerit") || _item->HasMember("GYROZ_FIGUREOFMERIT") || _item->HasMember("angleRate_Latency") || _item->HasMember("anglerate_latency") || _item->HasMember("ANGLERATE_LATENCY")))
        {
            if (_item->HasMember("aceinna_AngleRate"))
            {
                _item = &(*_item)["aceinna_AngleRate"];
            }
            else if (_item->HasMember("Aceinna_AngleRate"))
            {
                _item = &(*_item)["Aceinna_AngleRate"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("gyroY") || _item->HasMember("gyroy") || _item->HasMember("GYROY") || _item->HasMember("gyroX") || _item->HasMember("gyrox") || _item->HasMember("GYROX") || _item->HasMember("gyroZ") || _item->HasMember("gyroz") || _item->HasMember("GYROZ") || _item->HasMember("gyroY_FigureOfMerit") || _item->HasMember("gyroy_figureofmerit") || _item->HasMember("GYROY_FIGUREOFMERIT") || _item->HasMember("gyroX_FigureOfMerit") || _item->HasMember("gyrox_figureofmerit") || _item->HasMember("GYROX_FIGUREOFMERIT") || _item->HasMember("gyroZ_FigureOfMerit") || _item->HasMember("gyroz_figureofmerit") || _item->HasMember("GYROZ_FIGUREOFMERIT") || _item->HasMember("angleRate_Latency") || _item->HasMember("anglerate_latency") || _item->HasMember("ANGLERATE_LATENCY")))
        {
            if (_item->HasMember("aceinna_AngleRate"))
            {
                _item = &(*_item)["aceinna_AngleRate"];
            }
            else if (_item->HasMember("Aceinna_AngleRate"))
            {
                _item = &(*_item)["Aceinna_AngleRate"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::ACEINNA_ANGLERATE_T_R > (new ::nevonex::fcal::ACEINNA_ANGLERATE_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _gyroYField = nullptr;
        if ((*_item).HasMember("gyroY"))
        {
            _gyroYField = &(*_item)["gyroY"];
        }
        else if ((*_item).HasMember("gyroy"))
        {
            _gyroYField = &(*_item)["gyroy"];
        }
        else if ((*_item).HasMember("GYROY"))
        {
            _gyroYField = &(*_item)["GYROY"];
        }
        if (_gyroYField && _gyroYField->IsNumber())
        {
            _value->setGyroY(static_cast< ::ecore::EFloat >(_gyroYField->GetFloat()));
        }
        const rapidjson::Value* _gyroXField = nullptr;
        if ((*_item).HasMember("gyroX"))
        {
            _gyroXField = &(*_item)["gyroX"];
        }
        else if ((*_item).HasMember("gyrox"))
        {
            _gyroXField = &(*_item)["gyrox"];
        }
        else if ((*_item).HasMember("GYROX"))
        {
            _gyroXField = &(*_item)["GYROX"];
        }
        if (_gyroXField && _gyroXField->IsNumber())
        {
            _value->setGyroX(static_cast< ::ecore::EFloat >(_gyroXField->GetFloat()));
        }
        const rapidjson::Value* _gyroZField = nullptr;
        if ((*_item).HasMember("gyroZ"))
        {
            _gyroZField = &(*_item)["gyroZ"];
        }
        else if ((*_item).HasMember("gyroz"))
        {
            _gyroZField = &(*_item)["gyroz"];
        }
        else if ((*_item).HasMember("GYROZ"))
        {
            _gyroZField = &(*_item)["GYROZ"];
        }
        if (_gyroZField && _gyroZField->IsNumber())
        {
            _value->setGyroZ(static_cast< ::ecore::EFloat >(_gyroZField->GetFloat()));
        }
        const rapidjson::Value* _gyroY_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("gyroY_FigureOfMerit"))
        {
            _gyroY_FigureOfMeritField = &(*_item)["gyroY_FigureOfMerit"];
        }
        else if ((*_item).HasMember("gyroy_figureofmerit"))
        {
            _gyroY_FigureOfMeritField = &(*_item)["gyroy_figureofmerit"];
        }
        else if ((*_item).HasMember("GYROY_FIGUREOFMERIT"))
        {
            _gyroY_FigureOfMeritField = &(*_item)["GYROY_FIGUREOFMERIT"];
        }
        if (_gyroY_FigureOfMeritField && _gyroY_FigureOfMeritField->IsNumber())
        {
            _value->setGyroY_FigureOfMerit(static_cast< ::ecore::EInt >(_gyroY_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _gyroX_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("gyroX_FigureOfMerit"))
        {
            _gyroX_FigureOfMeritField = &(*_item)["gyroX_FigureOfMerit"];
        }
        else if ((*_item).HasMember("gyrox_figureofmerit"))
        {
            _gyroX_FigureOfMeritField = &(*_item)["gyrox_figureofmerit"];
        }
        else if ((*_item).HasMember("GYROX_FIGUREOFMERIT"))
        {
            _gyroX_FigureOfMeritField = &(*_item)["GYROX_FIGUREOFMERIT"];
        }
        if (_gyroX_FigureOfMeritField && _gyroX_FigureOfMeritField->IsNumber())
        {
            _value->setGyroX_FigureOfMerit(static_cast< ::ecore::EInt >(_gyroX_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _gyroZ_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("gyroZ_FigureOfMerit"))
        {
            _gyroZ_FigureOfMeritField = &(*_item)["gyroZ_FigureOfMerit"];
        }
        else if ((*_item).HasMember("gyroz_figureofmerit"))
        {
            _gyroZ_FigureOfMeritField = &(*_item)["gyroz_figureofmerit"];
        }
        else if ((*_item).HasMember("GYROZ_FIGUREOFMERIT"))
        {
            _gyroZ_FigureOfMeritField = &(*_item)["GYROZ_FIGUREOFMERIT"];
        }
        if (_gyroZ_FigureOfMeritField && _gyroZ_FigureOfMeritField->IsNumber())
        {
            _value->setGyroZ_FigureOfMerit(static_cast< ::ecore::EInt >(_gyroZ_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _angleRate_LatencyField = nullptr;
        if ((*_item).HasMember("angleRate_Latency"))
        {
            _angleRate_LatencyField = &(*_item)["angleRate_Latency"];
        }
        else if ((*_item).HasMember("anglerate_latency"))
        {
            _angleRate_LatencyField = &(*_item)["anglerate_latency"];
        }
        else if ((*_item).HasMember("ANGLERATE_LATENCY"))
        {
            _angleRate_LatencyField = &(*_item)["ANGLERATE_LATENCY"];
        }
        if (_angleRate_LatencyField && _angleRate_LatencyField->IsNumber())
        {
            _value->setAngleRate_Latency(static_cast< ::ecore::EFloat >(_angleRate_LatencyField->GetFloat()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setGyroY(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setGyroX(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setGyroZ(static_cast< ::ecore::EFloat >((*_item)[2].GetFloat()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setGyroY_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setGyroX_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
            if ((*_item).Size() > 5 && (*_item)[5].IsNumber())
            {
                _value->setGyroZ_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[5].GetDouble()));
            }
            if ((*_item).Size() > 6 && (*_item)[6].IsNumber())
            {
                _value->setAngleRate_Latency(static_cast< ::ecore::EFloat >((*_item)[6].GetFloat()));
            }
        }
        m_cAN_AGMO_MTLT305->setAceinna_AngleRate(_value);
        return;
    }

    if (_topic == "CAN_AGMO_MTLT305.Aceinna_Angles.sub" || _topic == "/9442")
    {
        if (!m_cAN_AGMO_MTLT305)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_MTLT305->updateInterfaceDetail(9442, !_jsonPlData.IsNull(), _timestamp))
        {
            return;
        }
        if (_jsonPlData.IsNull())
        {
            return;
        }

        const rapidjson::Value* _item = &_jsonPlData;
        if (_jsonPlData.IsArray() && _jsonPlData.Size() > 0 && _jsonPlData[0].IsObject())
        {
            _item = &_jsonPlData[0];
        }
        if (_item->IsObject() && !(_item->HasMember("pitch") || _item->HasMember("PITCH") || _item->HasMember("roll") || _item->HasMember("ROLL") || _item->HasMember("pitch_Compensation") || _item->HasMember("pitch_compensation") || _item->HasMember("PITCH_COMPENSATION") || _item->HasMember("pitch_FigureOfMerit") || _item->HasMember("pitch_figureofmerit") || _item->HasMember("PITCH_FIGUREOFMERIT") || _item->HasMember("roll_Compensation") || _item->HasMember("roll_compensation") || _item->HasMember("ROLL_COMPENSATION") || _item->HasMember("roll_FigureOfMerit") || _item->HasMember("roll_figureofmerit") || _item->HasMember("ROLL_FIGUREOFMERIT") || _item->HasMember("pitchRoll_Latency") || _item->HasMember("pitchroll_latency") || _item->HasMember("PITCHROLL_LATENCY")))
        {
            if (_item->HasMember("aceinna_Angles"))
            {
                _item = &(*_item)["aceinna_Angles"];
            }
            else if (_item->HasMember("Aceinna_Angles"))
            {
                _item = &(*_item)["Aceinna_Angles"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("pitch") || _item->HasMember("PITCH") || _item->HasMember("roll") || _item->HasMember("ROLL") || _item->HasMember("pitch_Compensation") || _item->HasMember("pitch_compensation") || _item->HasMember("PITCH_COMPENSATION") || _item->HasMember("pitch_FigureOfMerit") || _item->HasMember("pitch_figureofmerit") || _item->HasMember("PITCH_FIGUREOFMERIT") || _item->HasMember("roll_Compensation") || _item->HasMember("roll_compensation") || _item->HasMember("ROLL_COMPENSATION") || _item->HasMember("roll_FigureOfMerit") || _item->HasMember("roll_figureofmerit") || _item->HasMember("ROLL_FIGUREOFMERIT") || _item->HasMember("pitchRoll_Latency") || _item->HasMember("pitchroll_latency") || _item->HasMember("PITCHROLL_LATENCY")))
        {
            if (_item->HasMember("aceinna_Angles"))
            {
                _item = &(*_item)["aceinna_Angles"];
            }
            else if (_item->HasMember("Aceinna_Angles"))
            {
                _item = &(*_item)["Aceinna_Angles"];
            }
            else if (_item->HasMember("CAN_AGMO_MTLT305"))
            {
                _item = &(*_item)["CAN_AGMO_MTLT305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305"))
            {
                _item = &(*_item)["can_agmo_mtlt305"];
            }
            else if (_item->HasMember("can_agmo_mtlt305/0"))
            {
                _item = &(*_item)["can_agmo_mtlt305/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::ACEINNA_ANGLES_T_R > (new ::nevonex::fcal::ACEINNA_ANGLES_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _pitchField = nullptr;
        if ((*_item).HasMember("pitch"))
        {
            _pitchField = &(*_item)["pitch"];
        }
        else if ((*_item).HasMember("PITCH"))
        {
            _pitchField = &(*_item)["PITCH"];
        }
        if (_pitchField && _pitchField->IsNumber())
        {
            _value->setPitch(static_cast< ::ecore::EFloat >(_pitchField->GetFloat()));
        }
        const rapidjson::Value* _rollField = nullptr;
        if ((*_item).HasMember("roll"))
        {
            _rollField = &(*_item)["roll"];
        }
        else if ((*_item).HasMember("ROLL"))
        {
            _rollField = &(*_item)["ROLL"];
        }
        if (_rollField && _rollField->IsNumber())
        {
            _value->setRoll(static_cast< ::ecore::EFloat >(_rollField->GetFloat()));
        }
        const rapidjson::Value* _pitch_CompensationField = nullptr;
        if ((*_item).HasMember("pitch_Compensation"))
        {
            _pitch_CompensationField = &(*_item)["pitch_Compensation"];
        }
        else if ((*_item).HasMember("pitch_compensation"))
        {
            _pitch_CompensationField = &(*_item)["pitch_compensation"];
        }
        else if ((*_item).HasMember("PITCH_COMPENSATION"))
        {
            _pitch_CompensationField = &(*_item)["PITCH_COMPENSATION"];
        }
        if (_pitch_CompensationField && _pitch_CompensationField->IsNumber())
        {
            _value->setPitch_Compensation(static_cast< ::ecore::EInt >(_pitch_CompensationField->GetDouble()));
        }
        const rapidjson::Value* _pitch_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("pitch_FigureOfMerit"))
        {
            _pitch_FigureOfMeritField = &(*_item)["pitch_FigureOfMerit"];
        }
        else if ((*_item).HasMember("pitch_figureofmerit"))
        {
            _pitch_FigureOfMeritField = &(*_item)["pitch_figureofmerit"];
        }
        else if ((*_item).HasMember("PITCH_FIGUREOFMERIT"))
        {
            _pitch_FigureOfMeritField = &(*_item)["PITCH_FIGUREOFMERIT"];
        }
        if (_pitch_FigureOfMeritField && _pitch_FigureOfMeritField->IsNumber())
        {
            _value->setPitch_FigureOfMerit(static_cast< ::ecore::EInt >(_pitch_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _roll_CompensationField = nullptr;
        if ((*_item).HasMember("roll_Compensation"))
        {
            _roll_CompensationField = &(*_item)["roll_Compensation"];
        }
        else if ((*_item).HasMember("roll_compensation"))
        {
            _roll_CompensationField = &(*_item)["roll_compensation"];
        }
        else if ((*_item).HasMember("ROLL_COMPENSATION"))
        {
            _roll_CompensationField = &(*_item)["ROLL_COMPENSATION"];
        }
        if (_roll_CompensationField && _roll_CompensationField->IsNumber())
        {
            _value->setRoll_Compensation(static_cast< ::ecore::EInt >(_roll_CompensationField->GetDouble()));
        }
        const rapidjson::Value* _roll_FigureOfMeritField = nullptr;
        if ((*_item).HasMember("roll_FigureOfMerit"))
        {
            _roll_FigureOfMeritField = &(*_item)["roll_FigureOfMerit"];
        }
        else if ((*_item).HasMember("roll_figureofmerit"))
        {
            _roll_FigureOfMeritField = &(*_item)["roll_figureofmerit"];
        }
        else if ((*_item).HasMember("ROLL_FIGUREOFMERIT"))
        {
            _roll_FigureOfMeritField = &(*_item)["ROLL_FIGUREOFMERIT"];
        }
        if (_roll_FigureOfMeritField && _roll_FigureOfMeritField->IsNumber())
        {
            _value->setRoll_FigureOfMerit(static_cast< ::ecore::EInt >(_roll_FigureOfMeritField->GetDouble()));
        }
        const rapidjson::Value* _pitchRoll_LatencyField = nullptr;
        if ((*_item).HasMember("pitchRoll_Latency"))
        {
            _pitchRoll_LatencyField = &(*_item)["pitchRoll_Latency"];
        }
        else if ((*_item).HasMember("pitchroll_latency"))
        {
            _pitchRoll_LatencyField = &(*_item)["pitchroll_latency"];
        }
        else if ((*_item).HasMember("PITCHROLL_LATENCY"))
        {
            _pitchRoll_LatencyField = &(*_item)["PITCHROLL_LATENCY"];
        }
        if (_pitchRoll_LatencyField && _pitchRoll_LatencyField->IsNumber())
        {
            _value->setPitchRoll_Latency(static_cast< ::ecore::EFloat >(_pitchRoll_LatencyField->GetFloat()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setPitch(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setRoll(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setPitch_Compensation(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setPitch_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setRoll_Compensation(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
            if ((*_item).Size() > 5 && (*_item)[5].IsNumber())
            {
                _value->setRoll_FigureOfMerit(static_cast< ::ecore::EInt >((*_item)[5].GetDouble()));
            }
            if ((*_item).Size() > 6 && (*_item)[6].IsNumber())
            {
                _value->setPitchRoll_Latency(static_cast< ::ecore::EFloat >((*_item)[6].GetFloat()));
            }
        }
        m_cAN_AGMO_MTLT305->setAceinna_Angles(_value);
        return;
    }

    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__processMessage) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_MTLT305Provider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
    /*PROTECTED REGION ID(CAN_AGMO_MTLT305Provider__onConnectionStatusChange) START*/
    /*PROTECTED REGION END*/
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_MTLT305Provider::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_MTLT305Provider::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_MTLT305Provider::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
