#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include "AppMain/RddfParser.hpp"

namespace {

std::string makeTempFile(const std::string& content) {
    char path[] = "/tmp/rddf_test_XXXXXX";
    int fd = mkstemp(path);
    if (fd == -1) return "";
    const ssize_t bytesWritten = write(fd, content.data(), content.size());
    if (bytesWritten != static_cast<ssize_t>(content.size())) {
        close(fd);
        std::remove(path);
        return "";
    }
    close(fd);
    return std::string(path);
}

void cleanup(const std::string& path) { std::remove(path.c_str()); }

}  // anonymous namespace

using namespace AppMain;

TEST(RddfParserTest, ParsesValid9Columns) {
    std::string content =
        "1\t1\t37.5665\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n"
        "2\t2\t37.5666\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n"
        "3\t3\t37.5667\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n";
    auto path = makeTempFile(content);
    auto result = RddfParser::parse(path);
    cleanup(path);

    ASSERT_TRUE(result.waypoints.has_value());
    EXPECT_EQ(result.waypoints->size(), 3u);
    EXPECT_DOUBLE_EQ(result.waypoints->at(0).speed, 4.0);
    EXPECT_EQ(result.waypoints->at(0).implementFlag, 1);
}

TEST(RddfParserTest, RejectsInvalidColumnCount) {
    std::string content = "1\t1\t37.5\t126.9\t0.1\t0.0\n";  // 6 cols
    auto path = makeTempFile(content);
    auto result = RddfParser::parse(path);
    cleanup(path);

    EXPECT_FALSE(result.waypoints.has_value());
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST(RddfParserTest, EmptyFileReturnsEmptyVector) {
    auto path = makeTempFile("");
    auto result = RddfParser::parse(path);
    cleanup(path);

    ASSERT_TRUE(result.waypoints.has_value());
    EXPECT_TRUE(result.waypoints->empty());
}

TEST(RddfParserTest, RejectsNonNumericValues) {
    std::string content = "1\t1\tNOT_A_NUMBER\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n";
    auto path = makeTempFile(content);
    auto result = RddfParser::parse(path);
    cleanup(path);

    EXPECT_FALSE(result.waypoints.has_value());
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST(RddfParserTest, ParsesLegacy7Columns) {
    std::string content =
        "37.5665\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n"
        "37.5666\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n"
        "37.5667\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n";
    auto path = makeTempFile(content);
    auto result = RddfParser::parse(path);
    cleanup(path);

    ASSERT_TRUE(result.waypoints.has_value());
    EXPECT_EQ(result.waypoints->size(), 3u);
    EXPECT_EQ(result.waypoints->at(0).lineNo, 1);  // auto-generated
    EXPECT_EQ(result.waypoints->at(0).index, 1);
    EXPECT_DOUBLE_EQ(result.waypoints->at(0).speed, 4.0);
}

TEST(RddfParserTest, PreservesSubmittedSpeedExactly) {
    const std::string content =
        "1\t1\t37.5665\t126.978\t0.1\t0.0\t0.0\t8.125\t1\n";
    const auto path = makeTempFile(content);
    const auto result = RddfParser::parse(path);
    cleanup(path);

    ASSERT_TRUE(result.waypoints.has_value());
    ASSERT_EQ(result.waypoints->size(), 1u);
    EXPECT_DOUBLE_EQ(result.waypoints->front().speed, 8.125);
}

TEST(RddfParserTest, RejectsNonSequentialSubmittedIndex) {
    const std::string content =
        "1\t1\t37.5665\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n"
        "2\t3\t37.5666\t126.978\t0.1\t0.0\t0.0\t4.0\t1\n";
    const auto path = makeTempFile(content);
    const auto result = RddfParser::parse(path);
    cleanup(path);

    EXPECT_FALSE(result.waypoints.has_value());
    EXPECT_NE(result.errorMessage.find("expected 2, got 3"), std::string::npos);
}
