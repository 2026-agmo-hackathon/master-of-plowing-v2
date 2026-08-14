#include <gtest/gtest.h>
#include <AppMain/orchestration/RecordingArchive.hpp>

#include <filesystem>
#include <iterator>
#include <stdexcept>
#include <unistd.h>

using AppMain::orchestration::RecordingArchive;

namespace {
struct TempDir {
    std::filesystem::path path=std::filesystem::temp_directory_path()/
        ("mop-archive-"+std::to_string(::getpid())+"-"+std::to_string(counter++));
    TempDir(){std::filesystem::create_directories(path);}
    ~TempDir(){std::error_code ec;std::filesystem::remove_all(path,ec);}
    static int counter;
};
int TempDir::counter=0;
std::size_t fileCount(const std::filesystem::path& p){return std::distance(std::filesystem::directory_iterator(p),std::filesystem::directory_iterator{});}
}

class ArchiveFaultTest : public testing::TestWithParam<RecordingArchive::FaultPoint> {};
TEST_P(ArchiveFaultTest, FailureLeavesNoVisibleIncompleteAndRetryCreatesOnePair)
{
    TempDir temp;bool inject=true;
    RecordingArchive archive(temp.path.string(),[&](RecordingArchive::FaultPoint point){if(inject&&point==GetParam())throw std::runtime_error("injected");});
    std::string error;
    EXPECT_FALSE(archive.commit("team-run-7.csv","a,b\n1,2\n","{}",error));
    EXPECT_EQ(fileCount(temp.path),0u);
    std::string bytes;EXPECT_FALSE(archive.load("team-run-7.csv",1024,bytes));
    inject=false;error.clear();
    ASSERT_TRUE(archive.commit("team-run-7.csv","a,b\n1,2\n","{}",error))<<error;
    EXPECT_EQ(fileCount(temp.path),2u);
    ASSERT_TRUE(archive.load("team-run-7.csv",1024,bytes));EXPECT_EQ(bytes,"a,b\n1,2\n");
}
INSTANTIATE_TEST_SUITE_P(AllCommitBoundaries,ArchiveFaultTest,testing::Values(
    RecordingArchive::FaultPoint::AfterCsvRename,
    RecordingArchive::FaultPoint::AfterSidecarRename,
    RecordingArchive::FaultPoint::DirectorySync));
