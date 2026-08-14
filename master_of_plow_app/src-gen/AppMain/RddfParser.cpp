/*
 * AppMain/RddfParser.cpp
 */

#include <AppMain/RddfParser.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace AppMain
{

RddfParseResult RddfParser::parse(const std::string& filePath)
{
    RddfParseResult result;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        result.errorMessage = "Failed to open file: " + filePath;
        return result;
    }

    std::vector<RddfWaypoint> waypoints;
    std::string line;
    int rawLineNumber = 0;   // physical file line counter (for error messages)
    int autoIndex = 1;       // auto-generated lineNo/index for legacy 7-column format

    while (std::getline(file, line))
    {
        ++rawLineNumber;

        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        // Split by tab
        std::vector<std::string> cols;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, '\t'))
        {
            cols.push_back(token);
        }

        int colCount = static_cast<int>(cols.size());

        if (colCount != 7 && colCount != 9)
        {
            result.errorMessage = "Invalid column count: expected 7 or 9, got "
                                  + std::to_string(colCount);
            return result;
        }

        RddfWaypoint wp;

        try
        {
            if (colCount == 9)
            {
                // Format: lineNo \t index \t lat \t lon \t res1 \t res2 \t res3 \t speed \t implementFlag
                wp.lineNo       = std::stoi(cols[0]);
                wp.index        = std::stoi(cols[1]);
                wp.lat          = std::stod(cols[2]);
                wp.lon          = std::stod(cols[3]);
                // cols[4]=res1, cols[5]=res2, cols[6]=res3 — ignored
                wp.speed        = std::stod(cols[7]);
                wp.implementFlag = std::stoi(cols[8]);

                const int expectedIndex =
                        static_cast<int>(waypoints.size()) + 1;
                if (wp.index != expectedIndex)
                {
                    result.errorMessage =
                            "Invalid waypoint index at line " +
                            std::to_string(rawLineNumber) + ": expected " +
                            std::to_string(expectedIndex) + ", got " +
                            std::to_string(wp.index);
                    return result;
                }
            }
            else // colCount == 7 (legacy)
            {
                // Format: lat \t lon \t res1 \t res2 \t res3 \t speed \t implementFlag
                // lineNo and index are auto-generated sequentially
                wp.lineNo       = autoIndex;
                wp.index        = autoIndex;
                wp.lat          = std::stod(cols[0]);
                wp.lon          = std::stod(cols[1]);
                // cols[2]=res1, cols[3]=res2, cols[4]=res3 — ignored
                wp.speed        = std::stod(cols[5]);
                wp.implementFlag = std::stoi(cols[6]);
                ++autoIndex;
            }
        }
        catch (const std::exception& e)
        {
            result.errorMessage = "Parse error at line " + std::to_string(rawLineNumber)
                                  + ": " + e.what();
            return result;
        }

        waypoints.push_back(wp);
    }

    result.waypoints = std::move(waypoints);
    return result;
}

} // AppMain
