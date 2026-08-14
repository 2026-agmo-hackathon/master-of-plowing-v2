#include <AppMain/web/OrchestrationProtocol.hpp>

#include <AppMain/MainController.hpp>
#include <AppMain/web/WebSocketEndPoint.hpp>

#include <algorithm>
#include <thread>

namespace AppMain {
namespace web {
namespace {

std::string base64(const char* bytes, std::size_t size)
{
    static const char alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(((size + 2) / 3) * 4);
    for (std::size_t i = 0; i < size; i += 3) {
        const unsigned a = static_cast<unsigned char>(bytes[i]);
        const unsigned b = i + 1 < size ? static_cast<unsigned char>(bytes[i + 1]) : 0;
        const unsigned c = i + 2 < size ? static_cast<unsigned char>(bytes[i + 2]) : 0;
        const unsigned value = (a << 16) | (b << 8) | c;
        out.push_back(alphabet[(value >> 18) & 63]);
        out.push_back(alphabet[(value >> 12) & 63]);
        out.push_back(i + 1 < size ? alphabet[(value >> 6) & 63] : '=');
        out.push_back(i + 2 < size ? alphabet[value & 63] : '=');
    }
    return out;
}

void publishJson(const Json::Value& value)
{
    auto endpoint = WebSocketEndPoint::getInstance();
    if (endpoint) endpoint->publishMessage(value);
}

bool requiredString(const Json::Value& message, const char* key,
                    std::string& value)
{
    if (!message[key].isString() || message[key].asString().empty()) return false;
    value = message[key].asString();
    return true;
}

} // namespace

bool handleOrchestrationMessage(const Json::Value& message,
                                MainController& controller)
{
    if (!message.isObject() || !message["kind"].isString()) return false;
    const std::string kind = message["kind"].asString();
    if (kind == "get_orchestration_state") {
        WebSocketEndPoint::getInstance()->publishMessage(
            orchestrationSnapshotJson(controller.getOrchestrationSnapshot()));
        controller.requestOrchestrationRefresh();
        return true;
    }
    if (kind == "start_run") {
        orchestration::StartRequest request;
        if (!requiredString(message,"teamName",request.teamName)
            || !requiredString(message,"mapId",request.mapId)
            || !requiredString(message,"tractorId",request.tractorId)
            || !requiredString(message,"implementId",request.implementId)) {
            Json::Value error;
            error["kind"]="orchestration_request_error";
            error["error"]="team and all catalog selections are required";
            publishJson(error);
            return true;
        }
        controller.requestOrchestrationStart(request);
        return true;
    }
    if(kind=="sealed_capture_durable"&&message["runId"].isString()) {
        if(!controller.acknowledgeSealedCapture(message["runId"].asString())) {
            Json::Value error;error["kind"]="orchestration_request_error";
            error["error"]="sealed capture acknowledgement is not authorized";publishJson(error);
        }
        return true;
    }
    if(kind=="begin_sealed_capture"&&message["runId"].isString()) {
        if(!controller.beginSealedCapture(message["runId"].asString())) {
            Json::Value error;error["kind"]="orchestration_request_error";
            error["error"]="sealed capture is not authorized";publishJson(error);
        }
        return true;
    }
    if(kind=="sealed_capture_failed"&&message["runId"].isString()) {
        (void)controller.failSealedCapture(message["runId"].asString());
        return true;
    }
    if(kind=="leaderboard_submitted"&&message["runId"].isString()&&message["verdict"].isString()) {
        if(!controller.markLeaderboardSubmitted(message["runId"].asString(),message["verdict"].asString())) {
            Json::Value error;error["kind"]="orchestration_request_error";error["error"]="submitted recording ledger update failed";publishJson(error);
        }
        return true;
    }
    if(kind=="reset_cleanup_durable"&&message["runId"].isString()
        &&message["resetGeneration"].isUInt64()) {
        const auto before=controller.getOrchestrationSnapshot();
        if(before.resetGeneration==message["resetGeneration"].asUInt64()
            &&controller.acknowledgeResetCleanup(message["runId"].asString())) {
            Json::Value event;event["kind"]="run_reset";
            event["runId"]=message["runId"].asString();
            event["resetGeneration"]=message["resetGeneration"];
            publishJson(event);
        }
        return true;
    }
    if (kind == "set_sim_setup") {
        orchestration::StartRequest request;
        if (!message["teamName"].isString()
            || !requiredString(message,"mapId",request.mapId)
            || !requiredString(message,"tractorId",request.tractorId)
            || !requiredString(message,"implementId",request.implementId)) {
            Json::Value error; error["kind"]="orchestration_request_error";
            error["error"]="all simulator setup selections are required";
            publishJson(error); return true;
        }
        request.teamName=message["teamName"].asString();
        controller.requestOrchestrationSetup(request);
        return true;
    }
    if (kind == "finish_run") {
        controller.requestOrchestrationFinish(false);
        return true;
    }
    if (kind == "reset_run") {
        if(!message["runId"].isString()
            || !controller.beginResetCleanup(message["runId"].asString())) {
            Json::Value error;error["kind"]="orchestration_request_error";
            error["error"]="run reset cleanup is not authorized";publishJson(error);
            return true;
        }
        Json::Value event;event["kind"]="run_cleanup_prepare";
        const auto state=controller.getOrchestrationSnapshot();
        event["runId"]=message["runId"].asString();
        event["resetGeneration"]=Json::UInt64(state.resetGeneration);
        publishJson(event);
        return true;
    }
    if(kind=="prepare_purge_all") {
        const auto before=controller.getOrchestrationSnapshot();
        const std::string runId=before.recordingId;
        const bool accepted=runId.empty()
            ? controller.beginPurgeAllCleanup()
            : controller.beginResetCleanup(runId);
        if(!accepted) {
            Json::Value error;error["kind"]="orchestration_request_error";
            error["error"]="purge-all cleanup is not authorized";publishJson(error);
            return true;
        }
        const auto state=controller.getOrchestrationSnapshot();
        Json::Value event;event["kind"]="run_cleanup_prepare";
        event["runId"]=runId;
        event["resetGeneration"]=Json::UInt64(state.resetGeneration);
        publishJson(event);return true;
    }
    if(kind=="reset_cleanup_prepared"&&message["runId"].isString()
        &&message["resetGeneration"].isUInt64()) {
        const auto state=controller.getOrchestrationSnapshot();
        if(state.recordingId!=message["runId"].asString()
            || state.resetGeneration!=message["resetGeneration"].asUInt64()
            || state.postRun!=orchestration::PostRunState::ResetCleanupPreparing) {
            Json::Value error;error["kind"]="orchestration_request_error";
            error["error"]="run reset cleanup preparation is stale";publishJson(error);
            return true;
        }
        // 리셋을 성립시킨 클라이언트를 먼저 기록한다. 그래야 뒤늦게 도착한
        // 다른 탭의 실패 보고가 이미 시작된 리셋을 취소하지 못한다.
        // Record the client that admitted this reset first, so a later failure
        // report from another tab cannot cancel a reset already under way.
        (void)controller.noteResetCleanupPrepared(message["resetGeneration"].asUInt64());
        controller.requestOrchestrationReset();
        return true;
    }
    if(kind=="reset_cleanup_failed"&&message["runId"].isString()
        &&message["resetGeneration"].isUInt64()) {
        const auto state=controller.getOrchestrationSnapshot();
        if(state.resetGeneration==message["resetGeneration"].asUInt64())
            (void)controller.failResetCleanup(message["runId"].asString());
        return true;
    }
    if(kind=="list_unsubmitted_records") {
        Json::Value response;response["kind"]="unsubmitted_records";response["records"]=Json::arrayValue;
        for(const auto& r:controller.listUnsubmittedRecordings()) {Json::Value row;row["runId"]=r.runId;row["teamName"]=r.teamName;row["createdAt"]=r.createdAt;row["bytes"]=Json::UInt64(r.bytes);row["csvAvailable"]=r.csvAvailable;if(!r.error.empty())row["error"]=r.error;response["records"].append(row);}
        publishJson(response);return true;
    }
    if(kind=="purge_all_unsubmitted") {
        Json::Value error;error["kind"]="orchestration_request_error";
        error["error"]="purge-all requires backend cleanup preparation";
        publishJson(error);return true;
    }
    if (kind == "get_recording" && message["id"].isString()) {
        const std::string id=message["id"].asString();
        const auto state=controller.getOrchestrationSnapshot();
        const auto records=controller.listUnsubmittedRecordings();
        const bool listed=std::any_of(records.begin(),records.end(),
            [&](const orchestration::UnsubmittedRecord& record) {
                return record.runId==id && record.csvAvailable;
            });
        if ((state.recordingId!=id || state.recordingSha256.empty()) && !listed) {
            Json::Value error; error["kind"]="recording_error";
            error["id"]=id; error["error"]="recording is not authorized";
            publishJson(error); return true;
        }
        const std::string sha=state.recordingId==id
            ? state.recordingSha256 : std::string{};
        [&controller,id,sha] {
            std::string bytes;
            if (!controller.loadRecording(id,bytes)) {
                Json::Value error; error["kind"]="recording_error";
                error["id"]=id; error["error"]="recording not found";
                publishJson(error); return;
            }
            Json::Value begin; begin["kind"]="recording_begin";
            begin["id"]=id; begin["totalBytes"]=Json::UInt64(bytes.size());
            begin["sha256"]=sha;
            publishJson(begin);
            const std::size_t chunkSize=32u*1024u;
            std::size_t index=0;
            for (std::size_t offset=0; offset<bytes.size(); offset+=chunkSize,++index) {
                const std::size_t count=std::min(chunkSize,bytes.size()-offset);
                Json::Value chunk; chunk["kind"]="recording_chunk";
                chunk["id"]=id; chunk["index"]=Json::UInt64(index);
                chunk["base64"]=base64(bytes.data()+offset,count);
                publishJson(chunk);
            }
            Json::Value end; end["kind"]="recording_end";
            end["id"]=id; end["chunks"]=Json::UInt64(index);
            publishJson(end);
        }();
        return true;
    }
    return false;
}

} // namespace web
} // namespace AppMain
