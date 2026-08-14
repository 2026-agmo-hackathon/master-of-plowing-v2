#include <AppMain/RddfParser.hpp>
#include <AppMain/RddfValidator.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: rddf_validate PATH\n";
        return 2;
    }

    const auto parsed = AppMain::RddfParser::parse(argv[1]);
    if (!parsed.waypoints.has_value()) {
        std::cerr << "parse failed: " << parsed.errorMessage << "\n";
        return 1;
    }
    const auto& waypoints = parsed.waypoints.value();
    if (waypoints.empty()) {
        std::cerr << "validation failed: empty waypoints\n";
        return 1;
    }

    const auto result = AppMain::RddfValidator::validate(waypoints);
    if (!result.accepted) {
        std::cerr << "validation failed: " << result.reason << "\n";
        return 1;
    }
    for (const auto& warning : result.warnings) {
        std::cerr << "warning: " << warning << "\n";
    }
    std::cout << "accepted " << waypoints.size() << " waypoints\n";
    return 0;
}
