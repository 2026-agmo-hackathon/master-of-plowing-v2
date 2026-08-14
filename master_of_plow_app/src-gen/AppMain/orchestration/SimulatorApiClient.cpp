#include <AppMain/orchestration/SimulatorApiClient.hpp>
#include <AppMain/orchestration/CatalogContract.hpp>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/SHA2Engine.h>
#include <Poco/DigestStream.h>
#include <json/json.h>

#include <chrono>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace AppMain {
namespace orchestration {

struct SimulatorApiClient::Response {
    int status = 0;
    std::string body, contentType, disposition;
    bool recLive = false, recRecording = false;
    long long snapshotAgeMs = -1;
};

namespace {
ApiResult apiResult(const SimulatorApiClient::Response& r)
{
    if (r.status >= 200 && r.status < 300) return {true, r.status, {}};
    Json::Value v; Json::CharReaderBuilder b; std::string err;
    std::istringstream in(r.body);
    Json::parseFromStream(b, in, &v, &err);
    return {false, r.status, v.get("error", r.body).asString()};
}
bool parseJson(const std::string& body, Json::Value& out)
{
    Json::CharReaderBuilder b; std::string err; std::istringstream in(body);
    return Json::parseFromStream(b, in, &out, &err);
}
long long integer(const Json::Value& v, const char* key, long long fallback=-1)
{
    return v.isMember(key) && v[key].isNumeric() ? v[key].asInt64() : fallback;
}
bool boolean(const Json::Value& v, const char* key, bool fallback=false)
{
    return v.isMember(key) && v[key].isBool() ? v[key].asBool() : fallback;
}
std::string text(const Json::Value& v, const char* key)
{
    return v.isMember(key) && v[key].isString() ? v[key].asString() : std::string();
}
std::optional<double> number(const Json::Value& value,const char* key)
{
    return value.isMember(key)&&value[key].isNumeric()
        ? std::optional<double>(value[key].asDouble()):std::nullopt;
}
std::optional<int> integerValue(const Json::Value& value,const char* key)
{
    return value.isMember(key)&&value[key].isInt()
        ? std::optional<int>(value[key].asInt()):std::nullopt;
}
void parseSelection(const Json::Value& v, Selection& s)
{
    s.mapId=text(v,"mapId"); s.tractorId=text(v,"tractorId"); s.implementId=text(v,"implementId");
    s.stopped=boolean(v,"stopped"); s.reactRunning=boolean(v,"reactRunning");
    s.live=boolean(v,"live");
    s.snapshotAgeMs=integer(v,"snapshotAgeMs"); s.simElapsedMs=integer(v,"simElapsedMs");
}

}

SimulatorApiClient::SimulatorApiClient(int port) : host_(defaultGateway()), port_(port) {}

std::string SimulatorApiClient::defaultGateway()
{
    if (const char* forced=std::getenv("SEAMOS_SIMULATOR_HOST")) return forced;
    std::ifstream in("/proc/net/route"); std::string line;
    while (std::getline(in,line)) {
        std::istringstream row(line); std::string iface,dest,gateway;
        if (!(row>>iface>>dest>>gateway) || dest!="00000000" || gateway.size()!=8) continue;
        unsigned long raw=std::stoul(gateway,nullptr,16);
        std::ostringstream ip; ip<<(raw&255)<<'.'<<((raw>>8)&255)<<'.'<<((raw>>16)&255)<<'.'<<((raw>>24)&255);
        return ip.str();
    }
    return "192.168.40.100";
}

SimulatorApiClient::Response SimulatorApiClient::request(const std::string& method,
        const std::string& path, const std::string& body) const
{
    Response out;
    try {
        Poco::Net::HTTPClientSession session(host_,port_);
        session.setTimeout(Poco::Timespan(3,0));
        Poco::Net::HTTPRequest req(method,path,Poco::Net::HTTPMessage::HTTP_1_1);
        req.setContentType("application/json"); req.setContentLength(body.size());
        session.sendRequest(req)<<body;
        Poco::Net::HTTPResponse response;
        std::istream& stream=session.receiveResponse(response);
        const std::size_t cap=path=="/api/simulator/rec"
            ? RunOrchestrator::MAX_RECORDING_BYTES : 1024u*1024u;
        const std::streamsize contentLength=response.hasContentLength()
            ? response.getContentLength():-1;
        if(contentLength>=0 && static_cast<std::size_t>(contentLength)>cap)
            throw std::runtime_error("simulator response exceeds size cap");
        std::string bodyBytes;
        bodyBytes.reserve(contentLength>=0
            ? static_cast<std::size_t>(contentLength):4096u);
        char buffer[8192];
        while(stream.good()) {
            stream.read(buffer,sizeof(buffer));
            const std::streamsize count=stream.gcount();
            if(count<=0)break;
            if(bodyBytes.size()+static_cast<std::size_t>(count)>cap)
                throw std::runtime_error("simulator response exceeds size cap");
            bodyBytes.append(buffer,static_cast<std::size_t>(count));
        }
        out.status=response.getStatus(); out.body=std::move(bodyBytes);
        out.contentType=response.getContentType();
        out.disposition=response.get("Content-Disposition","");
        const std::string live=response.get("X-Sim-Live","false");
        const std::string recording=response.get("X-Rec-Recording","false");
        out.recLive=live=="true"||live=="1";
        out.recRecording=recording=="true"||recording=="1";
        const std::string age=response.get("X-Sim-Snapshot-Age-Ms","");
        if (!age.empty() && age!="unknown") out.snapshotAgeMs=std::stoll(age);
    } catch (const std::exception& e) { out.status=503; out.body=e.what(); }
    return out;
}

ApiResult SimulatorApiClient::selection(Selection& out)
{
    const Response r=request("GET","/api/simulator/selection"); Json::Value v;
    ApiResult a=apiResult(r); if (!a.ok || !parseJson(r.body,v)) return a.ok?ApiResult{false,502,"invalid selection JSON"}:a;
    parseSelection(v,out); return a;
}

ApiResult SimulatorApiClient::catalog(Catalog& out)
{
    auto load=[&](const char* path,const char* key,std::vector<CatalogItem>& dst,bool tractors) {
        const Response r=request("GET",path); ApiResult a=apiResult(r); Json::Value v;
        if (!a.ok || !parseJson(r.body,v)) return a.ok?ApiResult{false,502,"invalid catalog JSON"}:a;
        if(!v.isObject()) return ApiResult{false,502,"invalid catalog JSON"};
        if(!boolean(v,"live"))
            return ApiResult{false,503,std::string(key)+" catalog is not live"};
        const long long age=integer(v,"snapshotAgeMs");
        if(age<0 || age>RunOrchestrator::MAX_CATALOG_AGE_MS)
            return ApiResult{false,503,std::string(key)+" catalog is stale"};
        const Json::Value& rows=v[key];
        if (!rows.isArray()) return ApiResult{false,502,"invalid catalog JSON"};
        for (const auto& entry:rows) {
            CatalogItem parsed;
            if (tractors) {
                const Json::Value& steering=entry["steering"];
                RawTractorContract raw;
                raw.id=text(entry,"id"); raw.label=text(entry,"label");
                raw.wheelbaseM=number(entry,"wheelbaseM");
                raw.maxForwardGear=integerValue(entry,"maxForwardGear");
                if (steering.isObject()) {
                    raw.maxRateDegPerSec=number(steering,"maxRateDegPerSec");
                    raw.tauSec=number(steering,"tauSec");
                    raw.ratio=number(steering,"ratio");
                    raw.maxFrontWheelAngleDeg=number(steering,"maxFrontWheelAngleDeg");
                    raw.frontWheelTauSec=number(steering,"frontWheelTauSec");
                }
                std::string contractError;
                if (!makeTractorCatalogItem(raw,parsed,contractError))
                    return ApiResult{false,502,contractError};
            } else {
                parsed.id=text(entry,"id");
                parsed.label=text(entry,"label");
                if(parsed.label.empty())parsed.label=text(entry,"name");
                if(parsed.id.empty())return ApiResult{false,502,"catalog item missing id"};
                parsed.widthM=entry.get("blade_width_m",entry.get("widthM",0.0)).asDouble();
            }
            dst.push_back(parsed);
        }
        return a;
    };
    ApiResult a=load("/api/simulator/maps","maps",out.maps,false); if(!a.ok)return a;
    a=load("/api/simulator/tractors","tractors",out.tractors,true); if(!a.ok)return a;
    a=load("/api/simulator/implements","implements",out.implements,false); if(!a.ok)return a;
    ensureNoneImplement(out.implements);
    return selection(out.selection);
}

ApiResult SimulatorApiClient::postId(const std::string& path,const std::string& id)
{ Json::Value v;v["id"]=id;Json::StreamWriterBuilder b;b["indentation"]="";return apiResult(request("POST",path,Json::writeString(b,v))); }
ApiResult SimulatorApiClient::selectMap(const std::string& id){return postId("/api/simulator/map",id);}
ApiResult SimulatorApiClient::selectTractor(const std::string& id){return postId("/api/simulator/tractor",id);}
ApiResult SimulatorApiClient::selectImplement(const std::string& id)
{ return apiResult(request("POST","/api/simulator/implement",implementSelectionBody(id))); }
ApiResult SimulatorApiClient::simulatorRefresh(){return apiResult(request("POST","/api/simulator/refresh","{}"));}
ApiResult SimulatorApiClient::recordingReset(){return apiResult(request("POST","/api/simulator/rec/reset"));}
ApiResult SimulatorApiClient::recordingStart(){return apiResult(request("POST","/api/simulator/rec/start","{}"));}
ApiResult SimulatorApiClient::recordingStop(){return apiResult(request("POST","/api/simulator/rec/stop","{}"));}
ApiResult SimulatorApiClient::recordingState(RecorderState& out)
{
    const Response r=request("GET","/api/simulator/rec");
    const ApiResult result=apiResult(r);
    if (result.ok) {
        out.live=r.recLive;
        out.recording=r.recRecording;
        out.present=true;
        out.snapshotAgeMs=r.snapshotAgeMs;
        return result;
    }
    if (result.status==404) {
        // 404 는 "기록이 아직 없다"는 시뮬레이터의 확답이다 (Issue #9 §2.10).
        // 실패로 올려 보내면 호출자는 통신 오류와 구분하지 못하고, 실제로
        // 기록 정지 확인 루프가 이 응답만 받다가 타임아웃했다.
        //
        // A 404 is the simulator stating that no recording exists, not a
        // transport failure. Reporting it as an error left the recorder-stop
        // confirmation loop polling a definite answer until it timed out.
        out.live=r.recLive;
        out.recording=false;
        out.present=false;
        out.snapshotAgeMs=r.snapshotAgeMs;
        return {true,404,{}};
    }
    return result;
}
ApiResult SimulatorApiClient::engine(bool on){return apiResult(request("POST","/api/simulator/engine",on?"{\"on\":true}":"{\"on\":false}"));}
ApiResult SimulatorApiClient::recording(Recording& out)
{
    Response r=request("GET","/api/simulator/rec"); ApiResult a=apiResult(r);
    if(a.ok){out.bytes=r.body;out.contentType=r.contentType;out.disposition=r.disposition;out.live=r.recLive;out.recording=r.recRecording;out.snapshotAgeMs=r.snapshotAgeMs;}
    return a;
}
FileRecordingStore::FileRecordingStore(std::string directory,FaultHook faultHook)
    : faultHook_(std::move(faultHook))
{
    if(directory.empty()) { const char* env=std::getenv("SEAMOS_RECORDING_DIR"); directory_=env?env:"/app/out/recordings"; }
    else directory_=std::move(directory);
}
std::string FileRecordingStore::sanitizedTeam(const std::string& team)
{
    std::string out; for(unsigned char c:team){if(std::isalnum(c))out.push_back(c);else if(c==' '||c=='-'||c=='_')out.push_back('-');if(out.size()>=48)break;}
    return out.empty()?"team":out;
}
ApiResult FileRecordingStore::persist(const std::string& team,const std::string& stableRunId,
        const std::string& raw,
        std::string& id,std::string& sha)
{
    try {
        if(raw.empty() || raw.size()>RunOrchestrator::MAX_RECORDING_BYTES)
            throw std::runtime_error("recording size is invalid");
        if(stableRunId.empty() || stableRunId.size()>96
            || !std::all_of(stableRunId.begin(),stableRunId.end(),[](unsigned char c){
                return std::isalnum(c)||c=='-'||c=='_';
            }))throw std::runtime_error("stable run id is invalid");
        id=sanitizedTeam(team)+"-"+stableRunId+".csv";
        Poco::SHA2Engine hash(Poco::SHA2Engine::SHA_256);hash.update(raw);sha=Poco::DigestEngine::digestToHex(hash.digest());
        Json::Value meta;meta["team"]=team;meta["csv"]=id;meta["sha256"]=sha;meta["bytes"]=Json::UInt64(raw.size());
        meta["createdAtEpochMs"]=Json::Int64(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        Json::StreamWriterBuilder wb;wb["indentation"]="  ";const std::string side=Json::writeString(wb,meta);
        std::string archiveError;
        if(!RecordingArchive(directory_,faultHook_).commit(id,raw,side,archiveError))
            throw std::runtime_error(archiveError);
        return {true,200,{}};
    } catch(const std::exception& e){
        return {false,500,e.what()};
    }
}
bool FileRecordingStore::load(const std::string& id,std::string& bytes) const
{
    return RecordingArchive(directory_).load(id,RunOrchestrator::MAX_RECORDING_BYTES,bytes);
}
ApiResult FileRecordingStore::markSubmitted(const std::string& id,const std::string& verdict)
{
    try{
        if(id.empty()||id.find('/')!=std::string::npos||id.find("..")!=std::string::npos
            || (verdict!="SCORED"&&verdict!="INVALID"))throw std::runtime_error("invalid submitted recording");
        std::filesystem::create_directories(directory_);
        const auto ledger=std::filesystem::path(directory_)/"submitted-recordings.ledger";
        std::set<std::string> rows;std::ifstream in(ledger);std::string line;
        while(std::getline(in,line))if(!line.empty())rows.insert(line);
        rows.insert(id+"\t"+verdict);
        const auto tmp=std::filesystem::path(ledger.string()+".tmp");std::error_code ec;std::filesystem::remove(tmp,ec);
        const int fd=::open(tmp.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW,0600);if(fd<0)throw std::runtime_error("ledger create failed");
        std::string bytes;for(const auto& row:rows)bytes+=row+"\n";std::size_t off=0;
        while(off<bytes.size()){const ssize_t n=::write(fd,bytes.data()+off,bytes.size()-off);if(n<=0){::close(fd);throw std::runtime_error("ledger write failed");}off+=static_cast<std::size_t>(n);}
        if(::fsync(fd)!=0||::close(fd)!=0)throw std::runtime_error("ledger sync failed");
        std::filesystem::rename(tmp,ledger);const int dfd=::open(directory_.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW);if(dfd<0||::fsync(dfd)!=0){if(dfd>=0)::close(dfd);throw std::runtime_error("ledger directory sync failed");}::close(dfd);return {true,200,{}};
    }catch(const std::exception& e){return {false,500,e.what()};}
}
ApiResult FileRecordingStore::purgeUnsubmitted()
{
    try{
        std::set<std::string> submitted;const auto dir=std::filesystem::path(directory_),ledger=dir/"submitted-recordings.ledger";
        std::ifstream in(ledger);std::string line;while(std::getline(in,line)){const auto tab=line.find('\t');if(tab!=std::string::npos)submitted.insert(line.substr(0,tab));}
        std::vector<std::filesystem::path> remove;
        if(std::filesystem::exists(dir))for(const auto& entry:std::filesystem::directory_iterator(dir)){
            const auto name=entry.path().filename().string();if(entry.is_regular_file()&&entry.path().extension()==".csv"&&!submitted.count(name)){remove.push_back(entry.path());remove.push_back(std::filesystem::path(entry.path().string()+".json"));}
        }
        for(const auto& path:remove){std::error_code ec;if(std::filesystem::exists(path,ec)&&!ec)std::filesystem::remove(path,ec);if(ec)throw std::runtime_error("unsubmitted archive purge failed");}
        const int dfd=::open(dir.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW);if(dfd<0||::fsync(dfd)!=0){if(dfd>=0)::close(dfd);throw std::runtime_error("purge directory sync failed");}::close(dfd);return {true,200,{}};
    }catch(const std::exception& e){return {false,500,e.what()};}
}
std::vector<UnsubmittedRecord> FileRecordingStore::listUnsubmitted() const
{
    std::vector<UnsubmittedRecord> out;try{
        const auto dir=std::filesystem::path(directory_),ledger=dir/"submitted-recordings.ledger";std::set<std::string> submitted;
        std::ifstream in(ledger);std::string line;while(std::getline(in,line)){const auto tab=line.find('\t');if(tab!=std::string::npos)submitted.insert(line.substr(0,tab));}
        if(!std::filesystem::exists(dir))return out;
        for(const auto& entry:std::filesystem::directory_iterator(dir)){
            if(!entry.is_regular_file()||entry.path().extension()!=".csv")
                continue;
            const std::string id=entry.path().filename().string();
            if(submitted.count(id))continue;
            UnsubmittedRecord r;
            r.runId=id;
            r.csvAvailable=true;
            r.bytes=static_cast<std::size_t>(entry.file_size());
            std::ifstream meta(entry.path().string()+".json");
            Json::Value v;
            Json::CharReaderBuilder b;
            std::string err;
            if(meta&&Json::parseFromStream(b,meta,&v,&err)){
                r.teamName=text(v,"team");
                r.createdAt=std::to_string(integer(v,"createdAtEpochMs",0));
            }
            out.push_back(std::move(r));
        }
        std::sort(out.begin(),out.end(),[](const UnsubmittedRecord&a,const UnsubmittedRecord&b){return a.runId<b.runId;});
    }catch(const std::exception&){ }
    return out;
}

} // namespace orchestration
} // namespace AppMain
