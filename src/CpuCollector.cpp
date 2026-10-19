#include "CpuCollector.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

CpuCounters collectCpuCounters() {
    std::ifstream input("/proc/stat");
    std::string line, label;
    CpuCounters counters{};
    if (!std::getline(input, line))
        throw std::runtime_error("Cannot read /proc/stat");
    std::istringstream row(line);
    if (!(row >> label >> counters.user >> counters.nice
              >> counters.system >> counters.idle >> counters.iowait
              >> counters.irq >> counters.softirq >> counters.steal)
        || label != "cpu")
        throw std::runtime_error("Invalid aggregate CPU counters");
    return counters;
}
