#ifndef APPMAIN_ORCHESTRATION_RECORDINGARCHIVE_HPP
#define APPMAIN_ORCHESTRATION_RECORDINGARCHIVE_HPP

#include <functional>
#include <string>

namespace AppMain { namespace orchestration {

class RecordingArchive {
public:
    enum class FaultPoint { AfterCsvRename, AfterSidecarRename, DirectorySync };
    using FaultHook=std::function<void(FaultPoint)>;
    explicit RecordingArchive(std::string directory,FaultHook hook={}):directory_(std::move(directory)),hook_(std::move(hook)){}
    bool commit(const std::string& id,const std::string& csv,const std::string& sidecar,
                std::string& error) const;
    bool load(const std::string& id,std::size_t maxBytes,std::string& bytes) const;
private:
    std::string directory_;
    FaultHook hook_;
};

}} // namespace AppMain::orchestration
#endif
