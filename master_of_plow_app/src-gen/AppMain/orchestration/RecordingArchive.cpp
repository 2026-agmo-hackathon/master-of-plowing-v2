#include <AppMain/orchestration/RecordingArchive.hpp>

#include <filesystem>
#include <stdexcept>
#include <system_error>
#include <fcntl.h>
#include <unistd.h>

namespace AppMain { namespace orchestration { namespace {
void removeIfPresent(const std::filesystem::path& path){std::error_code ec;std::filesystem::remove(path,ec);}
void writeExclusive(const std::filesystem::path& path,const std::string& bytes)
{
    const int fd=::open(path.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW,0600);
    if(fd<0)throw std::runtime_error("exclusive archive creation failed");
    std::size_t offset=0;
    while(offset<bytes.size()){
        const ssize_t n=::write(fd,bytes.data()+offset,bytes.size()-offset);
        if(n<=0){::close(fd);::unlink(path.c_str());throw std::runtime_error("archive write failed");}
        offset+=static_cast<std::size_t>(n);
    }
    if(::fsync(fd)!=0||::close(fd)!=0){::unlink(path.c_str());throw std::runtime_error("archive sync failed");}
}
}

bool RecordingArchive::commit(const std::string& id,const std::string& csv,
        const std::string& sidecar,std::string& error) const
{
    const auto dir=std::filesystem::path(directory_);
    const auto final=dir/id,tmp=std::filesystem::path(final.string()+".tmp");
    const auto marker=std::filesystem::path(final.string()+".json");
    const auto markerTmp=std::filesystem::path(marker.string()+".tmp");
    try {
        std::filesystem::create_directories(dir);
        std::error_code ec;
        if(std::filesystem::symlink_status(dir,ec).type()!=std::filesystem::file_type::directory||ec)
            throw std::runtime_error("archive directory is not a real directory");
        removeIfPresent(tmp);removeIfPresent(markerTmp);
        if(!std::filesystem::exists(marker))removeIfPresent(final);
        writeExclusive(tmp,csv);writeExclusive(markerTmp,sidecar);
        std::filesystem::rename(tmp,final);
        if(hook_)hook_(FaultPoint::AfterCsvRename);
        std::filesystem::rename(markerTmp,marker);
        if(hook_)hook_(FaultPoint::AfterSidecarRename);
        if(hook_)hook_(FaultPoint::DirectorySync);
        const int dfd=::open(dir.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW);
        if(dfd<0||::fsync(dfd)!=0){if(dfd>=0)::close(dfd);throw std::runtime_error("archive directory sync failed");}
        ::close(dfd);return true;
    } catch(const std::exception& e) {
        error=e.what();removeIfPresent(tmp);removeIfPresent(markerTmp);removeIfPresent(marker);removeIfPresent(final);
        const int dfd=::open(dir.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW);
        if(dfd>=0){(void)::fsync(dfd);::close(dfd);}return false;
    }
}

bool RecordingArchive::load(const std::string& id,std::size_t maxBytes,std::string& bytes) const
{
    if(id.find('/')!=std::string::npos||id.find("..")!=std::string::npos)return false;
    const auto path=std::filesystem::path(directory_)/id,marker=std::filesystem::path(path.string()+".json");
    std::error_code ec;
    if(std::filesystem::is_symlink(marker,ec)||ec||!std::filesystem::is_regular_file(marker,ec)||ec
       ||std::filesystem::is_symlink(path,ec)||ec||!std::filesystem::is_regular_file(path,ec)||ec
       ||std::filesystem::file_size(path,ec)>maxBytes||ec)return false;
    const int fd=::open(path.c_str(),O_RDONLY|O_NOFOLLOW);if(fd<0)return false;
    bytes.clear();char buffer[8192];ssize_t n=0;
    while((n=::read(fd,buffer,sizeof(buffer)))>0){if(bytes.size()+static_cast<std::size_t>(n)>maxBytes){::close(fd);bytes.clear();return false;}bytes.append(buffer,static_cast<std::size_t>(n));}
    ::close(fd);return n==0;
}

}} // namespace AppMain::orchestration
