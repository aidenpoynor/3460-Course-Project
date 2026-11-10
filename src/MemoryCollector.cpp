#include "MemoryCollector.h"
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace {

bool parse_kilobytes(std::istringstream& row, std::uint64_t& value) {
    std::string number;
    std::string unit;
    std::string extra;
    if (!(row >> number >> unit) || unit != "kB" || (row >> extra)) return false;

    const auto result = std::from_chars(number.data(), number.data() + number.size(), value);
    return result.ec == std::errc{} && result.ptr == number.data() + number.size();
}

}

MemoryInfo collectMemoryInfo() {
    std::ifstream in("/proc/meminfo");
    if (!in) throw std::runtime_error("Cannot read /proc/meminfo");

    std::uint64_t total_kb = 0;
    std::uint64_t available_kb = 0;
    bool found_total = false;
    bool found_available = false;
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream row(line);
        std::string key;
        if (!(row >> key)) continue;

        if (key == "MemTotal:") {
            if (found_total || !parse_kilobytes(row, total_kb))
                throw std::runtime_error("Invalid MemTotal value in /proc/meminfo");
            found_total = true;
        } else if (key == "MemAvailable:") {
            if (found_available || !parse_kilobytes(row, available_kb))
                throw std::runtime_error("Invalid MemAvailable value in /proc/meminfo");
            found_available = true;
        }
    }

    if (!in.eof()) throw std::runtime_error("Error reading /proc/meminfo");
    if (!found_total || !found_available)
        throw std::runtime_error("Missing required memory values in /proc/meminfo");
    if (available_kb > total_kb)
        throw std::runtime_error("MemAvailable exceeds MemTotal in /proc/meminfo");

    MemoryInfo info;
    info.total_kb = total_kb;
    info.available_kb = available_kb;
    info.used_kb = total_kb - available_kb;
    return info;
}
