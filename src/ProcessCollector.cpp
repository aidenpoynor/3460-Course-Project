#include "ProcessCollector.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace {

// Helper used by the existing ProcessCollector; line is one stat record.
bool read_process_cpu(const std::string& line, ProcessInfo& p) {
    const std::size_t close = line.rfind(')');
    if (close == std::string::npos) return false;
    std::istringstream fields(line.substr(close + 1));
    std::string ignored;
    for (int field = 3; field <= 13; ++field)
        if (!(fields >> ignored)) return false;
    std::uint64_t utime{}, stime{};
    if (!(fields >> utime >> stime)) return false;
    for (int field = 16; field <= 21; ++field)
        if (!(fields >> ignored)) return false;
    if (!(fields >> p.start_ticks)) return false;
    p.cpu_ticks = utime + stime;
    return true;
}

// Returns VmRSS in kB, or an empty optional when the line is absent.
std::optional<std::uint64_t> read_vm_rss(const std::filesystem::path& status_path) {
    std::ifstream in(status_path);
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("VmRSS:", 0) != 0) continue;
        std::istringstream row(line.substr(6));
        std::uint64_t value{};
        if (row >> value) return value;
        return std::nullopt;
    }
    return std::nullopt;
}

}

std::vector<ProcessInfo> collectProcessInfo() {
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

        std::ifstream stat(entry.path() / "stat");
        std::string stat_line;
        if (!std::getline(stat, stat_line)) continue;

        ProcessInfo info;
        info.pid = std::stoi(name);
        info.name = process_name;
        if (!read_process_cpu(stat_line, info)) continue;
        info.memory_kb = read_vm_rss(entry.path() / "status");
        processes.push_back(info);
    }

    return processes;
}
