#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dls2/util/waypointer.hpp"

namespace
{
bool check(bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << std::endl;
        return false;
    }
    return true;
}

std::string writeConfigFile(
    const std::string& file_name,
    bool monotonic,
    int lookahead,
    double panic_threshold)
{
    const auto path = std::filesystem::temp_directory_path() / file_name;
    std::ofstream config_file(path);
    config_file << "monotonic: " << (monotonic ? "true" : "false") << "\n";
    config_file << "lookahead: " << lookahead << "\n";
    config_file << "panic_threshold: " << panic_threshold << "\n";
    config_file.close();
    return path.string();
}

bool testInitRejectsEmptyPath()
{
    const std::string config_path =
        writeConfigFile("euclidean_waypointer_empty.yaml", true, 0, 5.0);

    dls::utils::EuclideanWaypointer waypointer(config_path);
    const std::vector<dls::utils::Waypoint> empty_path;

    return check(!waypointer.init(empty_path), "empty path should be rejected");
}

bool testMonotonicSearchUsesLookahead()
{
    const std::string config_path =
        writeConfigFile("euclidean_waypointer_lookahead.yaml", true, 1, 100.0);

    dls::utils::EuclideanWaypointer waypointer(config_path);
    const std::vector<dls::utils::Waypoint> path{
        {0.0, 0.0},
        {1.0, 0.0},
        {2.0, 0.0},
        {3.0, 0.0},
    };

    if (!check(waypointer.init(path), "path init should succeed")) {
        return false;
    }

    dls::utils::Waypoint waypoint{};
    if (!check(waypointer.run({0.2, 0.0}, waypoint), "first run should succeed")) {
        return false;
    }
    if (!check(waypoint.first == 1.0 && waypoint.second == 0.0,
               "first run should pick lookahead waypoint (1.0, 0.0)")) {
        return false;
    }

    if (!check(waypointer.run({1.9, 0.0}, waypoint), "second run should succeed")) {
        return false;
    }
    return check(waypoint.first == 3.0 && waypoint.second == 0.0,
                 "second run should progress monotonically to (3.0, 0.0)");
}

bool testPanicFallbackRescansWholePath()
{
    const std::string config_path =
        writeConfigFile("euclidean_waypointer_panic.yaml", true, 0, 5.0);

    dls::utils::EuclideanWaypointer waypointer(config_path);
    const std::vector<dls::utils::Waypoint> path{
        {0.0, 0.0},
        {10.0, 0.0},
        {20.0, 0.0},
    };

    if (!check(waypointer.init(path), "panic test path init should succeed")) {
        return false;
    }

    dls::utils::Waypoint waypoint{};
    if (!check(waypointer.run({19.5, 0.0}, waypoint),
               "pre-panic run should succeed")) {
        return false;
    }
    if (!check(waypoint.first == 20.0 && waypoint.second == 0.0,
               "pre-panic run should pick final waypoint")) {
        return false;
    }

    if (!check(waypointer.run({0.1, 0.0}, waypoint),
               "panic fallback run should succeed")) {
        return false;
    }
    return check(waypoint.first == 0.0 && waypoint.second == 0.0,
                 "panic fallback should rescan from start and recover waypoint 0");
}
} // namespace

int main()
{
    const bool ok =
        testInitRejectsEmptyPath() &&
        testMonotonicSearchUsesLookahead() &&
        testPanicFallbackRescansWholePath();
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
