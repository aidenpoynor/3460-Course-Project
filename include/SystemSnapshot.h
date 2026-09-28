#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct MemoryInfo {
    std::uint64_t total_kb{};
    std::uint64_t available_kb{};
    std::uint64_t used_kb{};
};

struct ProcessInfo {
    int pid{};
    std::string name;
};

struct SystemSnapshot {
    MemoryInfo memory;
    std::vector<ProcessInfo> processes;
};
