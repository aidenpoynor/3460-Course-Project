#include "ProcessCollector.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>

std::vector<ProcessInfo> ProcessCollector::collect() {
    std::vector<ProcessInfo> processes;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        const std::string name = entry.path().filename().string();
        const bool numeric = !name.empty() &&
            std::all_of(name.begin(), name.end(),
                        [](unsigned char c) { return std::isdigit(c); });
        if (!numeric) continue;

        std::ifstream comm(entry.path() / "comm");
        std::string process_name;
        if (!std::getline(comm, process_name)) continue;

        ProcessInfo info;
        info.pid = std::stoi(name);
        info.name = process_name;
        processes.push_back(info);
    }

    return processes;
}
