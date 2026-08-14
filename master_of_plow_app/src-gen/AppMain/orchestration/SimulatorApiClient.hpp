#ifndef APPMAIN_ORCHESTRATION_SIMULATORAPICLIENT_HPP
#define APPMAIN_ORCHESTRATION_SIMULATORAPICLIENT_HPP

#include <AppMain/orchestration/RunOrchestrator.hpp>
#include <AppMain/orchestration/RecordingArchive.hpp>

namespace AppMain {
namespace orchestration {

class SimulatorApiClient : public ISimulatorApi {
public:
    struct Response;
    explicit SimulatorApiClient(int port = 8766);
    ApiResult catalog(Catalog& out) override;
    ApiResult selection(Selection& out) override;
    ApiResult selectMap(const std::string& id) override;
    ApiResult selectTractor(const std::string& id) override;
    ApiResult selectImplement(const std::string& id) override;
    ApiResult simulatorRefresh() override;
    ApiResult recordingReset() override;
    ApiResult recordingStart() override;
    ApiResult recordingStop() override;
    ApiResult recordingState(RecorderState& out) override;
    ApiResult engine(bool on) override;
    ApiResult recording(Recording& out) override;

private:
    Response request(const std::string& method, const std::string& path,
                     const std::string& body = {}) const;
    ApiResult postId(const std::string& path, const std::string& id);
    static std::string defaultGateway();
    std::string host_;
    int port_;
};

class FileRecordingStore : public IRecordingStore {
public:
    using FaultPoint = RecordingArchive::FaultPoint;
    using FaultHook = RecordingArchive::FaultHook;
    explicit FileRecordingStore(std::string directory = {}, FaultHook faultHook = {});
    ApiResult persist(const std::string& teamName, const std::string& stableRunId,
                      const std::string& rawCsv,
                      std::string& downloadId, std::string& sha256) override;
    ApiResult markSubmitted(const std::string& recordingId,const std::string& verdict) override;
    ApiResult purgeUnsubmitted() override;
    std::vector<UnsubmittedRecord> listUnsubmitted() const override;
    bool load(const std::string& downloadId, std::string& bytes) const;
private:
    static std::string sanitizedTeam(const std::string& teamName);
    std::string directory_;
    FaultHook faultHook_;
};

} // namespace orchestration
} // namespace AppMain
#endif
