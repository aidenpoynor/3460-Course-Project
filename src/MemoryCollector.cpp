#include "MemoryCollector.h"
#include <fstream>
#include <sstream>
#include <string>

MemoryInfo MemoryCollector::collect() {
    std::ifstream in("/proc/meminfo");
    std::uint64_t total_kb = 0, available_kb = 0;
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream row(line);
        std::string key;
        std::uint64_t value = 0;
        if (!(row >> key >> value)) continue;
        if (key == "MemTotal:") total_kb = value;
        else if (key == "MemAvailable:") available_kb = value;
    }

    MemoryInfo info;
    info.total_kb = total_kb;
    info.available_kb = available_kb;
    info.used_kb = total_kb - available_kb;
    return info;
}
