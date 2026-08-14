/*
 * AppMain/RddfParser.hpp
 */

#ifndef APPMAIN_RDDFPARSER_HPP
#define APPMAIN_RDDFPARSER_HPP

#include <optional>
#include <string>
#include <vector>

namespace AppMain
{

struct RddfWaypoint
{
    int lineNo;
    int index;
    double lat;           // decimal degrees
    double lon;           // decimal degrees
    double speed;         // km/h, signed
    int implementFlag;    // 0=up, 1=down
};

struct RddfParseResult
{
    std::optional<std::vector<RddfWaypoint>> waypoints;
    std::string errorMessage; // empty if success
};

class RddfParser
{
public:
    static RddfParseResult parse(const std::string& filePath);
};

} // AppMain

#endif // APPMAIN_RDDFPARSER_HPP
