/*
 * nevonex/can_agmo_customized_tractor/CAN_AGMO_Customized_TractorProvider.cpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#include "CAN_AGMO_Customized_TractorProvider.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "nevonex/can_agmo_customized_tractor/Can_agmo_customized_tractorPackage.hpp"
#include <ecorecpp/mapping.hpp>
#include <nevonex-fcal-platform/log/Logger.hpp>
#include <sstream>

/*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

using namespace ::nevonex::can_agmo_customized_tractor;
using namespace ::nevonex::log;

CAN_AGMO_Customized_TractorProvider::CAN_AGMO_Customized_TractorProvider() : m_cAN_AGMO_Customized_Tractor(0)
{
    _initialize();

    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__CAN_AGMO_Customized_TractorProvider) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CAN_AGMO_Customized_TractorProvider::~CAN_AGMO_Customized_TractorProvider()
{
    m_cAN_AGMO_Customized_Tractor.reset();
}

// References
::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor_ptr CAN_AGMO_Customized_TractorProvider::getCAN_AGMO_Customized_Tractor() const
{
    return m_cAN_AGMO_Customized_Tractor;
}

void CAN_AGMO_Customized_TractorProvider::setCAN_AGMO_Customized_Tractor(::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor_ptr _cAN_AGMO_Customized_Tractor)
{
    if (m_cAN_AGMO_Customized_Tractor)
        m_cAN_AGMO_Customized_Tractor->_setEContainer(CAN_AGMO_Customized_TractorProvider_ptr(),
                Can_agmo_customized_tractorPackage::_instance()->getCAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor());
    if (_cAN_AGMO_Customized_Tractor)
        _cAN_AGMO_Customized_Tractor->_setEContainer(_this(),
                Can_agmo_customized_tractorPackage::_instance()->getCAN_AGMO_Customized_TractorProvider__cAN_AGMO_Customized_Tractor());
    m_cAN_AGMO_Customized_Tractor = _cAN_AGMO_Customized_Tractor;
}

// Operations
void CAN_AGMO_Customized_TractorProvider::createMachines(std::istream & _stream)
{
    bool _newMachineCreated = false;
    if (!m_cAN_AGMO_Customized_Tractor)
    {
        setCAN_AGMO_Customized_Tractor(::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor_ptr(new ::nevonex::can_agmo_customized_tractor::CAN_AGMO_Customized_Tractor()));
        _newMachineCreated = true;
    }
    auto _machineObj = ::ecore::as < ::ecore::EObject > (m_cAN_AGMO_Customized_Tractor);
    if (_machineObj)
    {
        constructChildTypes(::ecore::EJavaObject(), _machineObj, "CAN_AGMO_Customized_Tractor");
        _machineObj->_initialize();
    }
    if (_newMachineCreated)
    {
        notifyPropertyChange("root", nullptr, m_cAN_AGMO_Customized_Tractor);
    }
    (void)_stream;
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__createMachines) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::constructChildTypes(::ecore::EJavaObject _parent, ::ecore::EObject_ptr _parentObj, ::ecore::EString const& _path)
{
    (void)_parent;
    if (!_parentObj) return;
    auto _topicObject = ::ecore::as < ::nevonex::common::TopicObject > (_parentObj);
    if (!_topicObject) return;
    ::ecore::EString _machinePath = _path;
    if (_machinePath.empty())
    {
        _machinePath = "CAN_AGMO_Customized_Tractor";
    }
    m_topicObjectMap[_machinePath] = _topicObject;
    m_topicObjectMap["CAN_AGMO_Customized_Tractor"] = _topicObject;
    m_topicObjectMap["can_agmo_customized_tractor"] = _topicObject;
    m_topicObjectMap["can_agmo_customized_tractor/0"] = _topicObject;
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__constructChildTypes) START*/
    /*PROTECTED REGION END*/
}

::nevonex::common::TopicObject_ptr CAN_AGMO_Customized_TractorProvider::getTopicElement(::ecore::EString const& _index)
{
    if (!m_cAN_AGMO_Customized_Tractor)
    {
        std::istringstream _emptyStream;
        createMachines(_emptyStream);
    }
    auto _it = m_topicObjectMap.find(_index);
    if (_it != m_topicObjectMap.end())
        return _it->second;
    if (boost::iequals(_index, "CAN_AGMO_Customized_Tractor")
            || boost::iequals(_index, "can_agmo_customized_tractor")
            || boost::iequals(_index, "can_agmo_customized_tractor/0"))
    {
        return ::ecore::as < ::nevonex::common::TopicObject > (m_cAN_AGMO_Customized_Tractor);
    }
    return nullptr;
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__getTopicElement) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::initMachineProvider()
{
    ::nevonex::types::IMachineProvider::initMachineProvider();
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__initMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::stopMachineProvider()
{
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__stopMachineProvider) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::start()
{
    ::nevonex::types::IMachineProvider::start();
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__start) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::stop()
{
    ::nevonex::types::IMachineProvider::stop();
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__stop) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::restart()
{
    ::nevonex::types::IMachineProvider::restart();
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__restart) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::processMessage(const std::string& _topic, const rapidjson::Value& _jsonPlData, std::int64_t _timestamp)
{
    if (_topic == "CAN_AGMO_Customized_Tractor.Receive_ACC_INFO.sub" || _topic == "/9455")
    {
        if (!m_cAN_AGMO_Customized_Tractor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_Customized_Tractor->updateInterfaceDetail(9455, !_jsonPlData.IsNull(), _timestamp))
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
        if (_item->IsObject() && !(_item->HasMember("tRZ_ACC_SIG1_V") || _item->HasMember("trz_acc_sig1_v") || _item->HasMember("TRZ_ACC_SIG1_V") || _item->HasMember("tRZ_ACC_SIG2_V") || _item->HasMember("trz_acc_sig2_v") || _item->HasMember("TRZ_ACC_SIG2_V") || _item->HasMember("tRZ_ACC_DIAG") || _item->HasMember("trz_acc_diag") || _item->HasMember("TRZ_ACC_DIAG") || _item->HasMember("tRZ_ACC_AUTO") || _item->HasMember("trz_acc_auto") || _item->HasMember("TRZ_ACC_AUTO")))
        {
            if (_item->HasMember("receive_ACC_INFO"))
            {
                _item = &(*_item)["receive_ACC_INFO"];
            }
            else if (_item->HasMember("Receive_ACC_INFO"))
            {
                _item = &(*_item)["Receive_ACC_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("tRZ_ACC_SIG1_V") || _item->HasMember("trz_acc_sig1_v") || _item->HasMember("TRZ_ACC_SIG1_V") || _item->HasMember("tRZ_ACC_SIG2_V") || _item->HasMember("trz_acc_sig2_v") || _item->HasMember("TRZ_ACC_SIG2_V") || _item->HasMember("tRZ_ACC_DIAG") || _item->HasMember("trz_acc_diag") || _item->HasMember("TRZ_ACC_DIAG") || _item->HasMember("tRZ_ACC_AUTO") || _item->HasMember("trz_acc_auto") || _item->HasMember("TRZ_ACC_AUTO")))
        {
            if (_item->HasMember("receive_ACC_INFO"))
            {
                _item = &(*_item)["receive_ACC_INFO"];
            }
            else if (_item->HasMember("Receive_ACC_INFO"))
            {
                _item = &(*_item)["Receive_ACC_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::RECEIVE_ACC_INFO_T_R > (new ::nevonex::fcal::RECEIVE_ACC_INFO_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _tRZ_ACC_SIG1_VField = nullptr;
        if ((*_item).HasMember("tRZ_ACC_SIG1_V"))
        {
            _tRZ_ACC_SIG1_VField = &(*_item)["tRZ_ACC_SIG1_V"];
        }
        else if ((*_item).HasMember("trz_acc_sig1_v"))
        {
            _tRZ_ACC_SIG1_VField = &(*_item)["trz_acc_sig1_v"];
        }
        else if ((*_item).HasMember("TRZ_ACC_SIG1_V"))
        {
            _tRZ_ACC_SIG1_VField = &(*_item)["TRZ_ACC_SIG1_V"];
        }
        if (_tRZ_ACC_SIG1_VField && _tRZ_ACC_SIG1_VField->IsNumber())
        {
            _value->setTRZ_ACC_SIG1_V(static_cast< ::ecore::EFloat >(_tRZ_ACC_SIG1_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_ACC_SIG2_VField = nullptr;
        if ((*_item).HasMember("tRZ_ACC_SIG2_V"))
        {
            _tRZ_ACC_SIG2_VField = &(*_item)["tRZ_ACC_SIG2_V"];
        }
        else if ((*_item).HasMember("trz_acc_sig2_v"))
        {
            _tRZ_ACC_SIG2_VField = &(*_item)["trz_acc_sig2_v"];
        }
        else if ((*_item).HasMember("TRZ_ACC_SIG2_V"))
        {
            _tRZ_ACC_SIG2_VField = &(*_item)["TRZ_ACC_SIG2_V"];
        }
        if (_tRZ_ACC_SIG2_VField && _tRZ_ACC_SIG2_VField->IsNumber())
        {
            _value->setTRZ_ACC_SIG2_V(static_cast< ::ecore::EFloat >(_tRZ_ACC_SIG2_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_ACC_DIAGField = nullptr;
        if ((*_item).HasMember("tRZ_ACC_DIAG"))
        {
            _tRZ_ACC_DIAGField = &(*_item)["tRZ_ACC_DIAG"];
        }
        else if ((*_item).HasMember("trz_acc_diag"))
        {
            _tRZ_ACC_DIAGField = &(*_item)["trz_acc_diag"];
        }
        else if ((*_item).HasMember("TRZ_ACC_DIAG"))
        {
            _tRZ_ACC_DIAGField = &(*_item)["TRZ_ACC_DIAG"];
        }
        if (_tRZ_ACC_DIAGField && _tRZ_ACC_DIAGField->IsNumber())
        {
            _value->setTRZ_ACC_DIAG(static_cast< ::ecore::EInt >(_tRZ_ACC_DIAGField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_ACC_AUTOField = nullptr;
        if ((*_item).HasMember("tRZ_ACC_AUTO"))
        {
            _tRZ_ACC_AUTOField = &(*_item)["tRZ_ACC_AUTO"];
        }
        else if ((*_item).HasMember("trz_acc_auto"))
        {
            _tRZ_ACC_AUTOField = &(*_item)["trz_acc_auto"];
        }
        else if ((*_item).HasMember("TRZ_ACC_AUTO"))
        {
            _tRZ_ACC_AUTOField = &(*_item)["TRZ_ACC_AUTO"];
        }
        if (_tRZ_ACC_AUTOField && _tRZ_ACC_AUTOField->IsNumber())
        {
            _value->setTRZ_ACC_AUTO(static_cast< ::ecore::EInt >(_tRZ_ACC_AUTOField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setTRZ_ACC_SIG1_V(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setTRZ_ACC_SIG2_V(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setTRZ_ACC_DIAG(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setTRZ_ACC_AUTO(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
        }
        m_cAN_AGMO_Customized_Tractor->setReceive_ACC_INFO(_value);
        return;
    }

    if (_topic == "CAN_AGMO_Customized_Tractor.Receive_FNR_INFO.sub" || _topic == "/9456")
    {
        if (!m_cAN_AGMO_Customized_Tractor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_Customized_Tractor->updateInterfaceDetail(9456, !_jsonPlData.IsNull(), _timestamp))
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
        if (_item->IsObject() && !(_item->HasMember("tRZ_FNR_SIG1_V") || _item->HasMember("trz_fnr_sig1_v") || _item->HasMember("TRZ_FNR_SIG1_V") || _item->HasMember("tRZ_FNR_SIG2_V") || _item->HasMember("trz_fnr_sig2_v") || _item->HasMember("TRZ_FNR_SIG2_V") || _item->HasMember("tRZ_FNR_DIAG") || _item->HasMember("trz_fnr_diag") || _item->HasMember("TRZ_FNR_DIAG") || _item->HasMember("tRZ_FNR_AUTO") || _item->HasMember("trz_fnr_auto") || _item->HasMember("TRZ_FNR_AUTO") || _item->HasMember("tRZ_FNR_STATE") || _item->HasMember("trz_fnr_state") || _item->HasMember("TRZ_FNR_STATE")))
        {
            if (_item->HasMember("receive_FNR_INFO"))
            {
                _item = &(*_item)["receive_FNR_INFO"];
            }
            else if (_item->HasMember("Receive_FNR_INFO"))
            {
                _item = &(*_item)["Receive_FNR_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("tRZ_FNR_SIG1_V") || _item->HasMember("trz_fnr_sig1_v") || _item->HasMember("TRZ_FNR_SIG1_V") || _item->HasMember("tRZ_FNR_SIG2_V") || _item->HasMember("trz_fnr_sig2_v") || _item->HasMember("TRZ_FNR_SIG2_V") || _item->HasMember("tRZ_FNR_DIAG") || _item->HasMember("trz_fnr_diag") || _item->HasMember("TRZ_FNR_DIAG") || _item->HasMember("tRZ_FNR_AUTO") || _item->HasMember("trz_fnr_auto") || _item->HasMember("TRZ_FNR_AUTO") || _item->HasMember("tRZ_FNR_STATE") || _item->HasMember("trz_fnr_state") || _item->HasMember("TRZ_FNR_STATE")))
        {
            if (_item->HasMember("receive_FNR_INFO"))
            {
                _item = &(*_item)["receive_FNR_INFO"];
            }
            else if (_item->HasMember("Receive_FNR_INFO"))
            {
                _item = &(*_item)["Receive_FNR_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::RECEIVE_FNR_INFO_T_R > (new ::nevonex::fcal::RECEIVE_FNR_INFO_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _tRZ_FNR_SIG1_VField = nullptr;
        if ((*_item).HasMember("tRZ_FNR_SIG1_V"))
        {
            _tRZ_FNR_SIG1_VField = &(*_item)["tRZ_FNR_SIG1_V"];
        }
        else if ((*_item).HasMember("trz_fnr_sig1_v"))
        {
            _tRZ_FNR_SIG1_VField = &(*_item)["trz_fnr_sig1_v"];
        }
        else if ((*_item).HasMember("TRZ_FNR_SIG1_V"))
        {
            _tRZ_FNR_SIG1_VField = &(*_item)["TRZ_FNR_SIG1_V"];
        }
        if (_tRZ_FNR_SIG1_VField && _tRZ_FNR_SIG1_VField->IsNumber())
        {
            _value->setTRZ_FNR_SIG1_V(static_cast< ::ecore::EFloat >(_tRZ_FNR_SIG1_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_FNR_SIG2_VField = nullptr;
        if ((*_item).HasMember("tRZ_FNR_SIG2_V"))
        {
            _tRZ_FNR_SIG2_VField = &(*_item)["tRZ_FNR_SIG2_V"];
        }
        else if ((*_item).HasMember("trz_fnr_sig2_v"))
        {
            _tRZ_FNR_SIG2_VField = &(*_item)["trz_fnr_sig2_v"];
        }
        else if ((*_item).HasMember("TRZ_FNR_SIG2_V"))
        {
            _tRZ_FNR_SIG2_VField = &(*_item)["TRZ_FNR_SIG2_V"];
        }
        if (_tRZ_FNR_SIG2_VField && _tRZ_FNR_SIG2_VField->IsNumber())
        {
            _value->setTRZ_FNR_SIG2_V(static_cast< ::ecore::EFloat >(_tRZ_FNR_SIG2_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_FNR_DIAGField = nullptr;
        if ((*_item).HasMember("tRZ_FNR_DIAG"))
        {
            _tRZ_FNR_DIAGField = &(*_item)["tRZ_FNR_DIAG"];
        }
        else if ((*_item).HasMember("trz_fnr_diag"))
        {
            _tRZ_FNR_DIAGField = &(*_item)["trz_fnr_diag"];
        }
        else if ((*_item).HasMember("TRZ_FNR_DIAG"))
        {
            _tRZ_FNR_DIAGField = &(*_item)["TRZ_FNR_DIAG"];
        }
        if (_tRZ_FNR_DIAGField && _tRZ_FNR_DIAGField->IsNumber())
        {
            _value->setTRZ_FNR_DIAG(static_cast< ::ecore::EInt >(_tRZ_FNR_DIAGField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_FNR_AUTOField = nullptr;
        if ((*_item).HasMember("tRZ_FNR_AUTO"))
        {
            _tRZ_FNR_AUTOField = &(*_item)["tRZ_FNR_AUTO"];
        }
        else if ((*_item).HasMember("trz_fnr_auto"))
        {
            _tRZ_FNR_AUTOField = &(*_item)["trz_fnr_auto"];
        }
        else if ((*_item).HasMember("TRZ_FNR_AUTO"))
        {
            _tRZ_FNR_AUTOField = &(*_item)["TRZ_FNR_AUTO"];
        }
        if (_tRZ_FNR_AUTOField && _tRZ_FNR_AUTOField->IsNumber())
        {
            _value->setTRZ_FNR_AUTO(static_cast< ::ecore::EInt >(_tRZ_FNR_AUTOField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_FNR_STATEField = nullptr;
        if ((*_item).HasMember("tRZ_FNR_STATE"))
        {
            _tRZ_FNR_STATEField = &(*_item)["tRZ_FNR_STATE"];
        }
        else if ((*_item).HasMember("trz_fnr_state"))
        {
            _tRZ_FNR_STATEField = &(*_item)["trz_fnr_state"];
        }
        else if ((*_item).HasMember("TRZ_FNR_STATE"))
        {
            _tRZ_FNR_STATEField = &(*_item)["TRZ_FNR_STATE"];
        }
        if (_tRZ_FNR_STATEField && _tRZ_FNR_STATEField->IsNumber())
        {
            _value->setTRZ_FNR_STATE(static_cast< ::ecore::EInt >(_tRZ_FNR_STATEField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setTRZ_FNR_SIG1_V(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setTRZ_FNR_SIG2_V(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setTRZ_FNR_DIAG(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setTRZ_FNR_AUTO(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setTRZ_FNR_STATE(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
        }
        m_cAN_AGMO_Customized_Tractor->setReceive_FNR_INFO(_value);
        return;
    }

    if (_topic == "CAN_AGMO_Customized_Tractor.Receive_HYD_INFO.sub" || _topic == "/9457")
    {
        if (!m_cAN_AGMO_Customized_Tractor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_Customized_Tractor->updateInterfaceDetail(9457, !_jsonPlData.IsNull(), _timestamp))
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
        if (_item->IsObject() && !(_item->HasMember("tRZ_HYD_SIG1_V") || _item->HasMember("trz_hyd_sig1_v") || _item->HasMember("TRZ_HYD_SIG1_V") || _item->HasMember("tRZ_HYD_SIG2_V") || _item->HasMember("trz_hyd_sig2_v") || _item->HasMember("TRZ_HYD_SIG2_V") || _item->HasMember("tRZ_HYD_DIAG") || _item->HasMember("trz_hyd_diag") || _item->HasMember("TRZ_HYD_DIAG") || _item->HasMember("tRZ_HYD_AUTO") || _item->HasMember("trz_hyd_auto") || _item->HasMember("TRZ_HYD_AUTO")))
        {
            if (_item->HasMember("receive_HYD_INFO"))
            {
                _item = &(*_item)["receive_HYD_INFO"];
            }
            else if (_item->HasMember("Receive_HYD_INFO"))
            {
                _item = &(*_item)["Receive_HYD_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("tRZ_HYD_SIG1_V") || _item->HasMember("trz_hyd_sig1_v") || _item->HasMember("TRZ_HYD_SIG1_V") || _item->HasMember("tRZ_HYD_SIG2_V") || _item->HasMember("trz_hyd_sig2_v") || _item->HasMember("TRZ_HYD_SIG2_V") || _item->HasMember("tRZ_HYD_DIAG") || _item->HasMember("trz_hyd_diag") || _item->HasMember("TRZ_HYD_DIAG") || _item->HasMember("tRZ_HYD_AUTO") || _item->HasMember("trz_hyd_auto") || _item->HasMember("TRZ_HYD_AUTO")))
        {
            if (_item->HasMember("receive_HYD_INFO"))
            {
                _item = &(*_item)["receive_HYD_INFO"];
            }
            else if (_item->HasMember("Receive_HYD_INFO"))
            {
                _item = &(*_item)["Receive_HYD_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::RECEIVE_HYD_INFO_T_R > (new ::nevonex::fcal::RECEIVE_HYD_INFO_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _tRZ_HYD_SIG1_VField = nullptr;
        if ((*_item).HasMember("tRZ_HYD_SIG1_V"))
        {
            _tRZ_HYD_SIG1_VField = &(*_item)["tRZ_HYD_SIG1_V"];
        }
        else if ((*_item).HasMember("trz_hyd_sig1_v"))
        {
            _tRZ_HYD_SIG1_VField = &(*_item)["trz_hyd_sig1_v"];
        }
        else if ((*_item).HasMember("TRZ_HYD_SIG1_V"))
        {
            _tRZ_HYD_SIG1_VField = &(*_item)["TRZ_HYD_SIG1_V"];
        }
        if (_tRZ_HYD_SIG1_VField && _tRZ_HYD_SIG1_VField->IsNumber())
        {
            _value->setTRZ_HYD_SIG1_V(static_cast< ::ecore::EFloat >(_tRZ_HYD_SIG1_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_HYD_SIG2_VField = nullptr;
        if ((*_item).HasMember("tRZ_HYD_SIG2_V"))
        {
            _tRZ_HYD_SIG2_VField = &(*_item)["tRZ_HYD_SIG2_V"];
        }
        else if ((*_item).HasMember("trz_hyd_sig2_v"))
        {
            _tRZ_HYD_SIG2_VField = &(*_item)["trz_hyd_sig2_v"];
        }
        else if ((*_item).HasMember("TRZ_HYD_SIG2_V"))
        {
            _tRZ_HYD_SIG2_VField = &(*_item)["TRZ_HYD_SIG2_V"];
        }
        if (_tRZ_HYD_SIG2_VField && _tRZ_HYD_SIG2_VField->IsNumber())
        {
            _value->setTRZ_HYD_SIG2_V(static_cast< ::ecore::EFloat >(_tRZ_HYD_SIG2_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_HYD_DIAGField = nullptr;
        if ((*_item).HasMember("tRZ_HYD_DIAG"))
        {
            _tRZ_HYD_DIAGField = &(*_item)["tRZ_HYD_DIAG"];
        }
        else if ((*_item).HasMember("trz_hyd_diag"))
        {
            _tRZ_HYD_DIAGField = &(*_item)["trz_hyd_diag"];
        }
        else if ((*_item).HasMember("TRZ_HYD_DIAG"))
        {
            _tRZ_HYD_DIAGField = &(*_item)["TRZ_HYD_DIAG"];
        }
        if (_tRZ_HYD_DIAGField && _tRZ_HYD_DIAGField->IsNumber())
        {
            _value->setTRZ_HYD_DIAG(static_cast< ::ecore::EInt >(_tRZ_HYD_DIAGField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_HYD_AUTOField = nullptr;
        if ((*_item).HasMember("tRZ_HYD_AUTO"))
        {
            _tRZ_HYD_AUTOField = &(*_item)["tRZ_HYD_AUTO"];
        }
        else if ((*_item).HasMember("trz_hyd_auto"))
        {
            _tRZ_HYD_AUTOField = &(*_item)["trz_hyd_auto"];
        }
        else if ((*_item).HasMember("TRZ_HYD_AUTO"))
        {
            _tRZ_HYD_AUTOField = &(*_item)["TRZ_HYD_AUTO"];
        }
        if (_tRZ_HYD_AUTOField && _tRZ_HYD_AUTOField->IsNumber())
        {
            _value->setTRZ_HYD_AUTO(static_cast< ::ecore::EInt >(_tRZ_HYD_AUTOField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setTRZ_HYD_SIG1_V(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setTRZ_HYD_SIG2_V(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setTRZ_HYD_DIAG(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setTRZ_HYD_AUTO(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
        }
        m_cAN_AGMO_Customized_Tractor->setReceive_HYD_INFO(_value);
        return;
    }

    if (_topic == "CAN_AGMO_Customized_Tractor.Receive_SFT_INFO.sub" || _topic == "/9459")
    {
        if (!m_cAN_AGMO_Customized_Tractor)
        {
            std::istringstream _emptyStream;
            createMachines(_emptyStream);
        }

        if (!m_cAN_AGMO_Customized_Tractor->updateInterfaceDetail(9459, !_jsonPlData.IsNull(), _timestamp))
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
        if (_item->IsObject() && !(_item->HasMember("tRZ_SFT_SIG1_V") || _item->HasMember("trz_sft_sig1_v") || _item->HasMember("TRZ_SFT_SIG1_V") || _item->HasMember("tRZ_SFT_SIG2_V") || _item->HasMember("trz_sft_sig2_v") || _item->HasMember("TRZ_SFT_SIG2_V") || _item->HasMember("tRZ_SFT_DIAG") || _item->HasMember("trz_sft_diag") || _item->HasMember("TRZ_SFT_DIAG") || _item->HasMember("tRZ_SFT_AUTO") || _item->HasMember("trz_sft_auto") || _item->HasMember("TRZ_SFT_AUTO") || _item->HasMember("tRZ_SFT_STATE") || _item->HasMember("trz_sft_state") || _item->HasMember("TRZ_SFT_STATE")))
        {
            if (_item->HasMember("receive_SFT_INFO"))
            {
                _item = &(*_item)["receive_SFT_INFO"];
            }
            else if (_item->HasMember("Receive_SFT_INFO"))
            {
                _item = &(*_item)["Receive_SFT_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        if (_item->IsObject() && !(_item->HasMember("tRZ_SFT_SIG1_V") || _item->HasMember("trz_sft_sig1_v") || _item->HasMember("TRZ_SFT_SIG1_V") || _item->HasMember("tRZ_SFT_SIG2_V") || _item->HasMember("trz_sft_sig2_v") || _item->HasMember("TRZ_SFT_SIG2_V") || _item->HasMember("tRZ_SFT_DIAG") || _item->HasMember("trz_sft_diag") || _item->HasMember("TRZ_SFT_DIAG") || _item->HasMember("tRZ_SFT_AUTO") || _item->HasMember("trz_sft_auto") || _item->HasMember("TRZ_SFT_AUTO") || _item->HasMember("tRZ_SFT_STATE") || _item->HasMember("trz_sft_state") || _item->HasMember("TRZ_SFT_STATE")))
        {
            if (_item->HasMember("receive_SFT_INFO"))
            {
                _item = &(*_item)["receive_SFT_INFO"];
            }
            else if (_item->HasMember("Receive_SFT_INFO"))
            {
                _item = &(*_item)["Receive_SFT_INFO"];
            }
            else if (_item->HasMember("CAN_AGMO_Customized_Tractor"))
            {
                _item = &(*_item)["CAN_AGMO_Customized_Tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor"))
            {
                _item = &(*_item)["can_agmo_customized_tractor"];
            }
            else if (_item->HasMember("can_agmo_customized_tractor/0"))
            {
                _item = &(*_item)["can_agmo_customized_tractor/0"];
            }
            else if (_item->HasMember("0"))
            {
                _item = &(*_item)["0"];
            }
        }
        auto _value = ::ecore::Ptr < ::nevonex::fcal::RECEIVE_SFT_INFO_T_R > (new ::nevonex::fcal::RECEIVE_SFT_INFO_T_R());
        if (_item->IsObject())
        {
        const rapidjson::Value* _tRZ_SFT_SIG1_VField = nullptr;
        if ((*_item).HasMember("tRZ_SFT_SIG1_V"))
        {
            _tRZ_SFT_SIG1_VField = &(*_item)["tRZ_SFT_SIG1_V"];
        }
        else if ((*_item).HasMember("trz_sft_sig1_v"))
        {
            _tRZ_SFT_SIG1_VField = &(*_item)["trz_sft_sig1_v"];
        }
        else if ((*_item).HasMember("TRZ_SFT_SIG1_V"))
        {
            _tRZ_SFT_SIG1_VField = &(*_item)["TRZ_SFT_SIG1_V"];
        }
        if (_tRZ_SFT_SIG1_VField && _tRZ_SFT_SIG1_VField->IsNumber())
        {
            _value->setTRZ_SFT_SIG1_V(static_cast< ::ecore::EFloat >(_tRZ_SFT_SIG1_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_SFT_SIG2_VField = nullptr;
        if ((*_item).HasMember("tRZ_SFT_SIG2_V"))
        {
            _tRZ_SFT_SIG2_VField = &(*_item)["tRZ_SFT_SIG2_V"];
        }
        else if ((*_item).HasMember("trz_sft_sig2_v"))
        {
            _tRZ_SFT_SIG2_VField = &(*_item)["trz_sft_sig2_v"];
        }
        else if ((*_item).HasMember("TRZ_SFT_SIG2_V"))
        {
            _tRZ_SFT_SIG2_VField = &(*_item)["TRZ_SFT_SIG2_V"];
        }
        if (_tRZ_SFT_SIG2_VField && _tRZ_SFT_SIG2_VField->IsNumber())
        {
            _value->setTRZ_SFT_SIG2_V(static_cast< ::ecore::EFloat >(_tRZ_SFT_SIG2_VField->GetFloat()));
        }
        const rapidjson::Value* _tRZ_SFT_DIAGField = nullptr;
        if ((*_item).HasMember("tRZ_SFT_DIAG"))
        {
            _tRZ_SFT_DIAGField = &(*_item)["tRZ_SFT_DIAG"];
        }
        else if ((*_item).HasMember("trz_sft_diag"))
        {
            _tRZ_SFT_DIAGField = &(*_item)["trz_sft_diag"];
        }
        else if ((*_item).HasMember("TRZ_SFT_DIAG"))
        {
            _tRZ_SFT_DIAGField = &(*_item)["TRZ_SFT_DIAG"];
        }
        if (_tRZ_SFT_DIAGField && _tRZ_SFT_DIAGField->IsNumber())
        {
            _value->setTRZ_SFT_DIAG(static_cast< ::ecore::EInt >(_tRZ_SFT_DIAGField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_SFT_AUTOField = nullptr;
        if ((*_item).HasMember("tRZ_SFT_AUTO"))
        {
            _tRZ_SFT_AUTOField = &(*_item)["tRZ_SFT_AUTO"];
        }
        else if ((*_item).HasMember("trz_sft_auto"))
        {
            _tRZ_SFT_AUTOField = &(*_item)["trz_sft_auto"];
        }
        else if ((*_item).HasMember("TRZ_SFT_AUTO"))
        {
            _tRZ_SFT_AUTOField = &(*_item)["TRZ_SFT_AUTO"];
        }
        if (_tRZ_SFT_AUTOField && _tRZ_SFT_AUTOField->IsNumber())
        {
            _value->setTRZ_SFT_AUTO(static_cast< ::ecore::EInt >(_tRZ_SFT_AUTOField->GetDouble()));
        }
        const rapidjson::Value* _tRZ_SFT_STATEField = nullptr;
        if ((*_item).HasMember("tRZ_SFT_STATE"))
        {
            _tRZ_SFT_STATEField = &(*_item)["tRZ_SFT_STATE"];
        }
        else if ((*_item).HasMember("trz_sft_state"))
        {
            _tRZ_SFT_STATEField = &(*_item)["trz_sft_state"];
        }
        else if ((*_item).HasMember("TRZ_SFT_STATE"))
        {
            _tRZ_SFT_STATEField = &(*_item)["TRZ_SFT_STATE"];
        }
        if (_tRZ_SFT_STATEField && _tRZ_SFT_STATEField->IsNumber())
        {
            _value->setTRZ_SFT_STATE(static_cast< ::ecore::EInt >(_tRZ_SFT_STATEField->GetDouble()));
        }
        }
        else if (_item->IsArray())
        {
            if ((*_item).Size() > 0 && (*_item)[0].IsNumber())
            {
                _value->setTRZ_SFT_SIG1_V(static_cast< ::ecore::EFloat >((*_item)[0].GetFloat()));
            }
            if ((*_item).Size() > 1 && (*_item)[1].IsNumber())
            {
                _value->setTRZ_SFT_SIG2_V(static_cast< ::ecore::EFloat >((*_item)[1].GetFloat()));
            }
            if ((*_item).Size() > 2 && (*_item)[2].IsNumber())
            {
                _value->setTRZ_SFT_DIAG(static_cast< ::ecore::EInt >((*_item)[2].GetDouble()));
            }
            if ((*_item).Size() > 3 && (*_item)[3].IsNumber())
            {
                _value->setTRZ_SFT_AUTO(static_cast< ::ecore::EInt >((*_item)[3].GetDouble()));
            }
            if ((*_item).Size() > 4 && (*_item)[4].IsNumber())
            {
                _value->setTRZ_SFT_STATE(static_cast< ::ecore::EInt >((*_item)[4].GetDouble()));
            }
        }
        m_cAN_AGMO_Customized_Tractor->setReceive_SFT_INFO(_value);
        return;
    }

    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__processMessage) START*/
    /*PROTECTED REGION END*/
}

void CAN_AGMO_Customized_TractorProvider::onConnectionStatusChange(const std::string & index, ::nevonex::machine::MachineConnectionInfo_ptr)
{
    /*PROTECTED REGION ID(CAN_AGMO_Customized_TractorProvider__onConnectionStatusChange) START*/
    /*PROTECTED REGION END*/
}

// PropertyChange delegation (diamond inheritance requires explicit override)
void CAN_AGMO_Customized_TractorProvider::addPropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::addPropertyChangeListener(_listener);
}

void CAN_AGMO_Customized_TractorProvider::removePropertyChangeListener(
        ::nevonex::types::PropertyChangeListener _listener)
{
    using namespace ::nevonex::types;
    PropertyChange::removePropertyChangeListener(_listener);
}

void CAN_AGMO_Customized_TractorProvider::notifyPropertyChange(::ecore::EString const &_name,
        ::ecore::EJavaObject const &_oldValue,
        ::ecore::EJavaObject const &_newValue)
{
    using namespace ::nevonex::types;
    PropertyChange::notifyPropertyChange(_name, _oldValue, _newValue);
}
